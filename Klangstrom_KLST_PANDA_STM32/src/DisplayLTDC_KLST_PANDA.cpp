/*
* Klangstrom
*
* This file is part of the *Klangstrom* library (https://github.com/dennisppaul/klangstrom-libraries).
* Copyright (c) 2024 Dennis P Paul.
*
* This library is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3.
*
* This library is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "Klangstrom.h"
#ifdef KLST_PERIPHERAL_ENABLE_DISPLAY
#ifdef KLST_PANDA_STM32

#include "main.h"
#include "Display.h"
#include "Console.h"

#ifdef __cplusplus
extern "C" {
#endif

// from main
//#define KLST_DISPLAY_FRAMEBUFFER_ADDRESS 0x90000000
//#define KLST_DISPLAY_WIDTH 480
//#define KLST_DISPLAY_HEIGHT 272

#define KLST_DISPLAY_FRAMEBUFFER_SIZE (KLST_DISPLAY_WIDTH * KLST_DISPLAY_HEIGHT * 4)
#define FRAMEBUFFER1_ADDR (KLST_DISPLAY_FRAMEBUFFER_ADDRESS)
#define FRAMEBUFFER2_ADDR (KLST_DISPLAY_FRAMEBUFFER_ADDRESS + KLST_DISPLAY_FRAMEBUFFER_SIZE)
#define FRAMEBUFFER1 0
#define FRAMEBUFFER2 1

/*
 * framebuffer addresses:
 *
 * FRAMEBUFFER1_ADDR :: 0x90000000
 * FRAMEBUFFER2_ADDR :: 0x9007F800
 */

//  values for ER-TFT043A2-3 display with ST7282 driver
//
//  LTDC_CLOCK_RATE          9.5   // ( TotalWidth * TotalHeigh * RefreshRate ) / 1000000 = 9.45 MHz at ( 534 * 295 * 60Hz )
//  DISPLAY_HSYNC            3     /* Horizontal synchronization */
//  DISPLAY_HBP              43    /* Horizontal back porch      */
//  DISPLAY_HFP              8     /* Horizontal front porch     */
//  DISPLAY_VSYNC            3     /* Vertical synchronization   */
//  DISPLAY_VBP              12    /* Vertical back porch        */
//  DISPLAY_VFP              8     /* Vertical front porch       */
//  DISPLAY_WIDTH            480
//  DISPLAY_HEIGHT           272
//  DISPLAY_FRAMEBUFFER_SIZE (DISPLAY_WIDTH * DISPLAY_HEIGHT * 4) // 32bit TODO(check if this 32bit or 24bit)
//  HorizontalSync     = (DISPLAY_HSYNC - 1);
//  VerticalSync       = (DISPLAY_VSYNC - 1);
//  AccumulatedHBP     = (DISPLAY_HSYNC + DISPLAY_HBP - 1);
//  AccumulatedVBP     = (DISPLAY_VSYNC + DISPLAY_VBP - 1);
//  AccumulatedActiveW = (DISPLAY_WIDTH + DISPLAY_HSYNC + DISPLAY_HBP - 1);
//  AccumulatedActiveH = (DISPLAY_HEIGHT + DISPLAY_VSYNC + DISPLAY_VBP - 1);
//  TotalWidth         = (DISPLAY_WIDTH + DISPLAY_HSYNC + DISPLAY_HBP + DISPLAY_HFP - 1);
//  TotalHeigh         = (DISPLAY_HEIGHT + DISPLAY_VSYNC + DISPLAY_VBP + DISPLAY_VFP - 1);

extern LTDC_HandleTypeDef  hltdc;
extern DMA2D_HandleTypeDef hdma2d;

void          HAL_LTDC_ReloadEventCallback(LTDC_HandleTypeDef* hltdc_handle);
__IO uint32_t LTDC_get_backbuffer_address(void);
static void   DMA2D_FillRect(uint32_t color, uint32_t x, uint32_t y, uint32_t width, uint32_t height);
static void   DMA2D_XferCpltCallback(DMA2D_HandleTypeDef* handle);
static void   DMA2D_XferErrorCallback(DMA2D_HandleTypeDef* handle);

static uint32_t fVSYNCDuration         = 0;
static uint32_t fVSYNCStart            = 0;
static uint8_t  active_framebuffer     = FRAMEBUFFER1;
static bool     fSyncToVBlank          = false;
volatile bool   fDMA2DTransferComplete = false;

// #define KLST_USE_SYNC_TO_V_BLANK_AS_UPDATE_TRIGGER

void display_enable_automatic_update(const bool sync_to_v_blank) {
    if (!fSyncToVBlank && sync_to_v_blank) {
        console_println("start loop");
#ifdef KLST_USE_SYNC_TO_V_BLANK_AS_UPDATE_TRIGGER
        HAL_LTDC_Reload(&hltdc, LTDC_RELOAD_VERTICAL_BLANKING);
#else
        HAL_LTDC_ProgramLineEvent(&hltdc, 0);
#endif // KLST_USE_SYNC_TO_V_BLANK_AS_UPDATE_TRIGGER
    }
    fSyncToVBlank = sync_to_v_blank;
}

#ifdef KLST_USE_SYNC_TO_V_BLANK_AS_UPDATE_TRIGGER
void HAL_LTDC_ReloadEventCallback(LTDC_HandleTypeDef* hltdc_handle) {
    fVSYNCDuration = HAL_GetTick() - fVSYNCStart;
    display_fire_update_callback();
    display_update();
    if (fSyncToVBlank) {
        HAL_LTDC_Reload(hltdc_handle, LTDC_RELOAD_VERTICAL_BLANKING);
    }
    fVSYNCStart = HAL_GetTick();
}
#else
void HAL_LTDC_LineEvenCallback(LTDC_HandleTypeDef* hltdc_handle) {
    display_fire_update_callback();
    display_update();
    if (fSyncToVBlank) {
        HAL_LTDC_ProgramLineEvent(hltdc_handle, 0);
    }
}
#endif // KLST_USE_SYNC_TO_V_BLANK_AS_UPDATE_TRIGGER

void display_deinit() {
    HAL_LTDC_DeInit(&hltdc);
}

inline __IO uint32_t LTDC_get_backbuffer_address(void) {
    if (!display_is_double_buffered()) {
        return FRAMEBUFFER1_ADDR;
    }
    if (active_framebuffer == FRAMEBUFFER1) {
        return FRAMEBUFFER2_ADDR;
    }
    return FRAMEBUFFER1_ADDR;
}

void display_update(void) {
    if (!display_is_double_buffered()) {
        return;
    }
    //	LTDC->SRCR = LTDC_SRCR_VBR;
    if (active_framebuffer == FRAMEBUFFER1) {
        LTDC_Layer1->CFBAR = FRAMEBUFFER2_ADDR;
        //		HAL_LTDC_SetAddress(&hltdc, FRAMEBUFFER2, 1);
        active_framebuffer = FRAMEBUFFER2;
    } else {
        LTDC_Layer1->CFBAR = FRAMEBUFFER1_ADDR;
        //		HAL_LTDC_SetAddress(&hltdc, FRAMEBUFFER1, 1);
        active_framebuffer = FRAMEBUFFER1;
    }
    // `HAL_LTDC_Reload` keeps loop on vertical blanking alive.
    //	HAL_LTDC_Reload(&hltdc, LTDC_RELOAD_VERTICAL_BLANKING);
    LTDC->SRCR = LTDC_SRCR_VBR; // not sure if it is better to first switch buffer and the reload or viceversa
    //	while ((LTDC->CDSR & LTDC_CDSR_VSYNCS) == 0)
    //		;
}

constexpr static uint8_t BYTES_PER_PIXEL = 4;

static void DMA2D_DrawImage(uint32_t*      data,
                            const uint32_t x,
                            const uint32_t y,
                            const uint32_t width,
                            const uint32_t height) {
    DMA2D_HandleTypeDef hdma2d;
    hdma2d.Instance = DMA2D;

    hdma2d.Init.Mode         = DMA2D_M2M;
    hdma2d.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
    hdma2d.Init.OutputOffset = KLST_DISPLAY_WIDTH - width;

    // Foreground
    hdma2d.LayerCfg[1].AlphaMode      = DMA2D_NO_MODIF_ALPHA;
    hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
    hdma2d.LayerCfg[1].InputOffset    = 0;

    const uint32_t offset      = (x + y * KLST_DISPLAY_WIDTH) * BYTES_PER_PIXEL;
    const uint32_t destination = LTDC_get_backbuffer_address() + offset;

    HAL_DMA2D_Init(&hdma2d);
    HAL_DMA2D_ConfigLayer(&hdma2d, 1);
    HAL_DMA2D_Start(&hdma2d, (uint32_t) data,
                    destination,
                    width,
                    height);
    HAL_DMA2D_PollForTransfer(&hdma2d, 10);
}

static void _DMA2D_DrawImage(uint32_t*      data,
                             const uint32_t x,
                             const uint32_t y,
                             const uint32_t width,
                             const uint32_t height) {
    const uint32_t offset      = (x + y * KLST_DISPLAY_WIDTH) * BYTES_PER_PIXEL;
    const uint32_t destination = LTDC_get_backbuffer_address() + offset;

    DMA2D_HandleTypeDef hdma2d;
    hdma2d.Instance = DMA2D;

    hdma2d.Init.Mode         = DMA2D_M2M_BLEND;
    hdma2d.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
    hdma2d.Init.OutputOffset = KLST_DISPLAY_WIDTH - width;

    // Foreground
    hdma2d.LayerCfg[1].AlphaMode      = DMA2D_COMBINE_ALPHA;
    hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
    hdma2d.LayerCfg[1].InputOffset    = 0;
    hdma2d.LayerCfg[1].AlphaInverted  = DMA2D_REGULAR_ALPHA;

    // Background
    hdma2d.LayerCfg[0].AlphaMode      = DMA2D_NO_MODIF_ALPHA;
    hdma2d.LayerCfg[0].InputColorMode = DMA2D_INPUT_ARGB8888;
    hdma2d.LayerCfg[0].InputOffset    = KLST_DISPLAY_WIDTH - width;

    HAL_DMA2D_Init(&hdma2d);
    HAL_DMA2D_ConfigLayer(&hdma2d, 1);
    HAL_DMA2D_ConfigLayer(&hdma2d, 0);
    HAL_DMA2D_BlendingStart(&hdma2d, reinterpret_cast<uintptr_t>(data), destination, destination, width, height);
    HAL_DMA2D_PollForTransfer(&hdma2d, 10);
}

// TODO this seems very cumbersome, maybe we can simplify this
static void DMA2D_FillRect(const uint32_t color,
                           const uint32_t x,
                           const uint32_t y,
                           const uint32_t width,
                           const uint32_t height) {
    hdma2d.Instance          = DMA2D;
    hdma2d.Init.Mode         = DMA2D_R2M;
    hdma2d.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
    hdma2d.Init.OutputOffset = KLST_DISPLAY_WIDTH - width;
    HAL_DMA2D_Init(&hdma2d);
    // HAL_DMA2D_ConfigLayer(&hdma2d, 0); // TODO not sure about this
    // HAL_DMA2D_ConfigLayer(&hdma2d, 1);
    const uint32_t offset      = (x + y * KLST_DISPLAY_WIDTH) * BYTES_PER_PIXEL;
    const uint32_t destination = LTDC_get_backbuffer_address() + offset;
#define KLST_DISPLAY_USE_TRANSFER_POLLING
#ifdef KLST_DISPLAY_USE_TRANSFER_POLLING
    if (HAL_DMA2D_Start(&hdma2d,
                        color,
                        destination,
                        width,
                        height) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_DMA2D_PollForTransfer(&hdma2d, 5) != HAL_OK) {
        Error_Handler();
    }
#else
    fDMA2DTransferComplete = false;
    HAL_DMA2D_Start_IT(&hdma2d,
                       color,
                       destination,
                       width,
                       height);
    while (!fDMA2DTransferComplete) { ; }
#endif
}

void DMA2D_XferCpltCallback(DMA2D_HandleTypeDef* handle) {
    /* USER CODE BEGIN DMA2D_XferCpltCallback */
    // If the framebuffer is placed in Write Through cached memory (e.g. SRAM) then we need
    // to flush the Dcache prior to letting DMA2D accessing it. That's done
    // using SCB_CleanInvalidateDCache().
    // SCB_CleanInvalidateDCache(); // todo is this necessary
    /* USER CODE END DMA2D_XferCpltCallback */
    fDMA2DTransferComplete = true;
}

void DMA2D_XferErrorCallback(DMA2D_HandleTypeDef* handle) {
    console_error("DMA2D_XferErrorCallback");
}

void display_LTDC_init() {
    /* fill framebuffers with black */
    for (int i = 0; i < KLST_DISPLAY_FRAMEBUFFER_SIZE; i++) {
        reinterpret_cast<volatile uint8_t*>(FRAMEBUFFER1_ADDR)[i] = 0x88;
        reinterpret_cast<volatile uint8_t*>(FRAMEBUFFER2_ADDR)[i] = 0x88;
    }

    hdma2d.XferCpltCallback  = DMA2D_XferCpltCallback;
    hdma2d.XferErrorCallback = DMA2D_XferErrorCallback;

    // `HAL_LTDC_Reload` starts loop synced vertical blanking
    //	HAL_LTDC_Reload(&hltdc, LTDC_RELOAD_VERTICAL_BLANKING); // start reload look at 60Hz
}

#ifdef __cplusplus
}
#endif

/* drawing library */

int16_t display_get_width() {
    return KLST_DISPLAY_WIDTH;
}

int16_t display_get_height() {
    return KLST_DISPLAY_HEIGHT;
}

void display_rect(const uint16_t x,
                  const uint16_t y,
                  const uint16_t width,
                  const uint16_t height,
                  const uint32_t argb_color) {
    DMA2D_FillRect(argb_color, x, y, width, height);
}

void display_image(uint32_t*      data,
                   const uint32_t x,
                   const uint32_t y,
                   const uint32_t width,
                   const uint32_t height) {
    DMA2D_DrawImage(data, x, y, width, height);
}

void display_clear(const uint32_t argb_color) {
    DMA2D_FillRect(argb_color,
                   0,
                   0,
                   KLST_DISPLAY_WIDTH,
                   KLST_DISPLAY_HEIGHT);
}

void display_clear(const uint8_t r, const uint8_t g, const uint8_t b) {
    display_clear(RGB(r, g, b));
}

void display_set_pixel(uint16_t x, uint16_t y, uint32_t argb_color) {
    if (x < KLST_DISPLAY_WIDTH && y < KLST_DISPLAY_HEIGHT) {
        reinterpret_cast<volatile uint32_t*>(LTDC_get_backbuffer_address())[x + y * KLST_DISPLAY_WIDTH] = argb_color;
    }
}

#endif // KLST_PANDA_STM32
#endif // KLST_PERIPHERAL_ENABLE_DISPLAY