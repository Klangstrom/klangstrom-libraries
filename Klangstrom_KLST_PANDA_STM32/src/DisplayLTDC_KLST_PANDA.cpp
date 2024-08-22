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
#define BYTES_PER_PIXEL ((uint8_t) 4)

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

void        HAL_LTDC_ReloadEventCallback(LTDC_HandleTypeDef* hltdc_handle);
static void DMA2D_FillRect(uint32_t color, uint32_t x, uint32_t y, uint32_t width, uint32_t height);
static void DMA2D_XferCpltCallback(DMA2D_HandleTypeDef* handle);
static void DMA2D_XferErrorCallback(DMA2D_HandleTypeDef* handle);

static uint32_t fVSYNCDuration         = 0;
static uint32_t fVSYNCStart            = 0;
static uint8_t  active_framebuffer     = FRAMEBUFFER1;
static bool     fSyncToVBlank          = false;
volatile bool   fDMA2DTransferComplete = false;

#define KLST_USE_SYNC_TO_V_BLANK_AS_UPDATE_TRIGGER

static void cleanup_dma2d() {
    /* Wait for DMA2D to finish last run */
    while ((READ_REG(DMA2D->CR) & DMA2D_CR_START) != 0U)
        ;

    /* Clear transfer flags */
    WRITE_REG(DMA2D->IFCR, DMA2D_FLAG_TC | DMA2D_FLAG_CE | DMA2D_FLAG_TE);
}

static void cleanup_memory() {
    if (SCB->CCR & SCB_CCR_DC_Msk) {
        SCB_CleanInvalidateDCache();
    }
}

static void enable_reload() {
#ifdef KLST_USE_SYNC_TO_V_BLANK_AS_UPDATE_TRIGGER
    HAL_LTDC_Reload(&hltdc, LTDC_RELOAD_VERTICAL_BLANKING);
#else
    HAL_LTDC_ProgramLineEvent(&hltdc, 0);
#endif // KLST_USE_SYNC_TO_V_BLANK_AS_UPDATE_TRIGGER
}


void display_enable_automatic_update(const bool sync_to_v_blank) {
    if (!fSyncToVBlank && sync_to_v_blank) {
        enable_reload();
    }
    fSyncToVBlank = sync_to_v_blank;
}

#ifdef KLST_USE_SYNC_TO_V_BLANK_AS_UPDATE_TRIGGER
void HAL_LTDC_ReloadEventCallback(LTDC_HandleTypeDef* hltdc_handle) {
#else
void HAL_LTDC_LineEventCallback(LTDC_HandleTypeDef* hltdc_handle) {
#endif // KLST_USE_SYNC_TO_V_BLANK_AS_UPDATE_TRIGGER
    if (hltdc_handle->Instance == LTDC) {
        display_fire_update_callback();
        cleanup_dma2d();
        cleanup_memory();
        display_swap_buffer();
        if (fSyncToVBlank) {
            enable_reload();
        }
    }
}

void display_deinit() {
    HAL_LTDC_DeInit(&hltdc);
}

volatile uint32_t LTDC_get_backbuffer_address(void) {
    if (!display_is_double_buffered()) {
        return FRAMEBUFFER1_ADDR;
    }
    if (active_framebuffer == FRAMEBUFFER1) {
        return FRAMEBUFFER2_ADDR;
    }
    return FRAMEBUFFER1_ADDR;
}

void display_swap_buffer(void) {
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
}

void DMA2D_XferCpltCallback(DMA2D_HandleTypeDef* handle) {
    /* USER CODE BEGIN DMA2D_XferCpltCallback */
    // If the framebuffer is placed in Write Through cached memory (e.g. SRAM) then we need
    // to flush the Dcache prior to letting DMA2D accessing it. That's done
    // using SCB_CleanInvalidateDCache().
    // SCB_CleanInvalidateDCache(); // TODO is this necessary see also `flushLine`
    /* USER CODE END DMA2D_XferCpltCallback */
    console_println("Xfer complete");
    // // TODO is this the right location
    // WRITE_REG(DMA2D->IFCR, DMA2D_FLAG_TC | DMA2D_FLAG_CE | DMA2D_FLAG_TE);
    // while ((READ_REG(DMA2D->CR) & DMA2D_CR_START) != 0U)
    //     ;
    // TODO this is still broken :(
    fDMA2DTransferComplete = true;
}

void DMA2D_XferErrorCallback(DMA2D_HandleTypeDef* handle) {
    console_error("DMA2D_XferErrorCallback");
}

void display_LTDC_init() {
    /* fill framebuffers with black */
    for (int i = 0; i < KLST_DISPLAY_FRAMEBUFFER_SIZE; i++) {
        reinterpret_cast<uint8_t*>(FRAMEBUFFER1_ADDR)[i] = 0x00;
        reinterpret_cast<uint8_t*>(FRAMEBUFFER2_ADDR)[i] = 0x00;
    }

    hdma2d.XferCpltCallback  = DMA2D_XferCpltCallback;
    hdma2d.XferErrorCallback = DMA2D_XferErrorCallback;
}

#ifdef __cplusplus
}
#endif


#endif // KLST_PANDA_STM32
#endif // KLST_PERIPHERAL_ENABLE_DISPLAY