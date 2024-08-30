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

/* NOTE these functions are deliberately not placed in `extern "C" {}` block to allow overloading */

#if KLST_DISPLAY_RENDERER == KLST_DISPLAY_RENDERER_DMA2D

#ifdef __cplusplus
extern "C" {
#endif

extern DMA2D_HandleTypeDef hdma2d;

static void DMA2D_XferCpltCallback(DMA2D_HandleTypeDef* handle);
static void DMA2D_XferErrorCallback(DMA2D_HandleTypeDef* handle);

void display_LTDC_init_DMA2D() {
    // NOTE not calling `MX_DMA2D_Init()`
    hdma2d.XferCpltCallback  = DMA2D_XferCpltCallback;
    hdma2d.XferErrorCallback = DMA2D_XferErrorCallback;

    hdma2d.Instance          = DMA2D;
    hdma2d.Init.Mode         = DMA2D_R2M;
    hdma2d.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
    hdma2d.Init.OutputOffset = 0;

    static constexpr uint8_t LAYER_INDEX           = 1;
    hdma2d.LayerCfg[LAYER_INDEX].InputOffset       = 0;
    hdma2d.LayerCfg[LAYER_INDEX].InputColorMode    = DMA2D_INPUT_ARGB8888;
    hdma2d.LayerCfg[LAYER_INDEX].AlphaMode         = DMA2D_NO_MODIF_ALPHA;
    hdma2d.LayerCfg[LAYER_INDEX].InputAlpha        = 0;
    hdma2d.LayerCfg[LAYER_INDEX].AlphaInverted     = DMA2D_REGULAR_ALPHA;
    hdma2d.LayerCfg[LAYER_INDEX].RedBlueSwap       = DMA2D_RB_REGULAR;
    hdma2d.LayerCfg[LAYER_INDEX].ChromaSubSampling = DMA2D_NO_CSS;

    if (HAL_DMA2D_Init(&hdma2d) != HAL_OK) {
        Error_Handler();
    }

    if (HAL_DMA2D_ConfigLayer(&hdma2d, LAYER_INDEX) != HAL_OK) {
        Error_Handler();
    }
}

static void DMA2D_FillRect(const uint32_t color, const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height) {
    hdma2d.Init.Mode         = DMA2D_R2M;
    hdma2d.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
    hdma2d.Init.OutputOffset = display_get_width() - width;

#define RECT_HAS_ALPHA 0 // TODO alpha with filled rects is not working yet WIP
#if RECT_HAS_ALPHA
    const uint32_t offset      = (x + y * display_get_width()) * BYTES_PER_PIXEL;
    const uint32_t destination = display_get_backbuffer_address() + offset;

    // Foreground
    hdma2d.LayerCfg[1].AlphaMode      = DMA2D_NO_MODIF_ALPHA;
    hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
    hdma2d.LayerCfg[1].InputOffset    = 0;
    hdma2d.LayerCfg[1].AlphaInverted  = DMA2D_REGULAR_ALPHA;

    // Background
    hdma2d.LayerCfg[0].AlphaMode      = DMA2D_NO_MODIF_ALPHA;
    hdma2d.LayerCfg[0].InputColorMode = DMA2D_INPUT_ARGB8888;
    hdma2d.LayerCfg[0].InputOffset    = display_get_width() - width;

    HAL_DMA2D_Init(&hdma2d);
    HAL_DMA2D_ConfigLayer(&hdma2d, 1);
    HAL_DMA2D_ConfigLayer(&hdma2d, 0);
    HAL_DMA2D_BlendingStart(&hdma2d,
                            color,
                            destination,
                            destination,
                            width,
                            height);
#else
    HAL_DMA2D_Init(&hdma2d);
    HAL_DMA2D_Start(
        &hdma2d,
        color,
        display_get_backbuffer_address() + (x + y * display_get_width()) * 4,
        width,
        height);
#endif
    HAL_DMA2D_PollForTransfer(&hdma2d, 10);
}

void DMA2D_DrawImage(uint32_t*      data,
                     const uint16_t x,
                     const uint16_t y,
                     const uint16_t width,
                     const uint16_t height) {
    hdma2d.Init.Mode         = DMA2D_M2M;
    hdma2d.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
    hdma2d.Init.OutputOffset = display_get_width() - width;

    const uint32_t offset      = (x + y * display_get_width()) * BYTES_PER_PIXEL;
    const uint32_t destination = display_get_backbuffer_address() + offset;

    HAL_DMA2D_Init(&hdma2d);
    HAL_DMA2D_Start(&hdma2d,
                    reinterpret_cast<uint32_t>(data),
                    destination,
                    width,
                    height);
    HAL_DMA2D_PollForTransfer(&hdma2d, 10);
}

void DMA2D_DrawImageAlpha(uint32_t* data, uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    const uint32_t offset      = (x + y * display_get_width()) * BYTES_PER_PIXEL;
    const uint32_t destination = display_get_backbuffer_address() + offset;

    hdma2d.Init.Mode         = DMA2D_M2M_BLEND;
    hdma2d.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
    hdma2d.Init.OutputOffset = display_get_width() - width;

    // Foreground
    hdma2d.LayerCfg[1].AlphaMode      = DMA2D_NO_MODIF_ALPHA;
    hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
    hdma2d.LayerCfg[1].InputOffset    = 0;
    hdma2d.LayerCfg[1].AlphaInverted  = DMA2D_REGULAR_ALPHA;

    // Background
    hdma2d.LayerCfg[0].AlphaMode      = DMA2D_NO_MODIF_ALPHA;
    hdma2d.LayerCfg[0].InputColorMode = DMA2D_INPUT_ARGB8888;
    hdma2d.LayerCfg[0].InputOffset    = display_get_width() - width;

    HAL_DMA2D_Init(&hdma2d);
    HAL_DMA2D_ConfigLayer(&hdma2d, 1);
    HAL_DMA2D_ConfigLayer(&hdma2d, 0);
    HAL_DMA2D_BlendingStart(&hdma2d,
                            reinterpret_cast<uint32_t>(data),
                            destination,
                            destination,
                            width,
                            height);
    HAL_DMA2D_PollForTransfer(&hdma2d, 10);
}

void DMA2D_XferCpltCallback(DMA2D_HandleTypeDef* handle) {
    /* USER CODE BEGIN DMA2D_XferCpltCallback */
    // If the framebuffer is placed in Write Through cached memory (e.g. SRAM) then we need
    // to flush the Dcache prior to letting DMA2D accessing it. That's done
    // using SCB_CleanInvalidateDCache().
    // SCB_CleanInvalidateDCache(); // TODO is this necessary see also `flushLine`
    /* USER CODE END DMA2D_XferCpltCallback */
    // console_println("Xfer complete");
    // // TODO is this the right location
    // WRITE_REG(DMA2D->IFCR, DMA2D_FLAG_TC | DMA2D_FLAG_CE | DMA2D_FLAG_TE);
    // while ((READ_REG(DMA2D->CR) & DMA2D_CR_START) != 0U)
    //     ;
    // TODO this is still broken :(
    // fDMA2DTransferComplete = true;
}

void DMA2D_XferErrorCallback(DMA2D_HandleTypeDef* handle) {
    console_error("DMA2D_XferErrorCallback");
}

#ifdef __cplusplus
}
#endif

/* implementation of `DisplayDrawInterface` */

void display_clear_BSP(const uint32_t color) {
    DMA2D_FillRect(color, 0, 0, display_get_width(), display_get_height());
}

void display_set_pixel_BSP(const uint16_t x,
                           const uint16_t y,
                           const uint32_t color) {
    if (x < display_get_width() && y < display_get_height()) {
        const uint32_t offset                                                          = x + y * display_get_width();
        reinterpret_cast<volatile uint32_t*>(display_get_backbuffer_address())[offset] = color;
    }
}

uint32_t display_get_pixel_BSP(const uint16_t x, const uint16_t y) {
    if (x < display_get_width() && y < display_get_height()) {
        const uint32_t offset = x + y * display_get_width();
        return reinterpret_cast<volatile uint32_t*>(display_get_backbuffer_address())[offset];
    }
    return 0;
}

void display_rect_fill_BSP(const uint16_t x,
                           const uint16_t y,
                           const uint16_t width,
                           const uint16_t height,
                           const uint32_t color) {
    DMA2D_FillRect(color, x, y, width, height);
}

void display_line_horizontal_BSP(const uint16_t x,
                                 const uint16_t y,
                                 const uint16_t length,
                                 const uint32_t color) {
    DMA2D_FillRect(color, x, y, length, 1);
}

void display_line_vertical_BSP(const uint16_t x,
                               const uint16_t y,
                               const uint16_t length,
                               const uint32_t color) {
    DMA2D_FillRect(color, x, y, 1, length);
}

void display_image_BSP(uint32_t*      data,
                       const uint16_t x,
                       const uint16_t y,
                       const uint16_t width,
                       const uint16_t height) {
    DMA2D_DrawImageAlpha(data, x, y, width, height);
}

static void DrawChar(BitmapFont*    font,
                     const uint16_t x,
                     uint16_t       y,
                     const uint8_t* c,
                     const uint32_t color,
                     const uint32_t background_color) {
    uint32_t line;

    const uint16_t height = font->Height;
    const uint16_t width  = font->Width;
    const uint8_t  offset = 8 * ((width + 7) / 8) - width;

    for (uint16_t i = 0; i < height; i++) {
        const uint8_t* pchar = const_cast<uint8_t*>(c) + (width + 7) / 8 * i;
        switch ((width + 7) / 8) {
            case 1:
                line = pchar[0];
                break;
            case 2:
                line = pchar[0] << 8 | pchar[1];
                break;
            case 3:
            default:
                line = pchar[0] << 16 | (pchar[1] << 8) | pchar[2];
                break;
        }

        for (uint16_t j = 0; j < width; j++) {
            if (line & (1 << (width - j + offset - 1))) {
                display_set_pixel_BSP(x + j, y, color);
            } else {
                const uint8_t alpha = GET_ALPHA(background_color);
                if (alpha == 0xFF) {
                    display_set_pixel_BSP((x + j), y, background_color);
                } else if (alpha > 0x00) {
                    const uint32_t pixel = display_get_pixel_BSP((x + j), y);
                    const uint8_t  red   = (GET_RED(background_color) * (0xFF - alpha) + GET_RED(pixel) * alpha) / 0xFF;
                    const uint8_t  green = (GET_GREEN(background_color) * (0xFF - alpha) + GET_GREEN(pixel) * alpha) / 0xFF;
                    const uint8_t  blue  = (GET_BLUE(background_color) * (0xFF - alpha) + GET_BLUE(pixel) * alpha) / 0xFF;
                    display_set_pixel_BSP((x + j), y, RGBA(red, green, blue, 0xFF));
                }
            }
        }
        y++;
    }
}

void display_char_BSP(BitmapFont*    font,
                      const uint16_t x,
                      const uint16_t y,
                      const uint8_t  ascii_char,
                      const uint32_t color,
                      const uint32_t background_color) {
    DrawChar(font,
             x,
             y,
             &font->table[(ascii_char - ' ') * font->Height * ((font->Width + 7) / 8)],
             color,
             background_color);
}

#endif // KLST_DISPLAY_RENDERER==KLST_DISPLAY_RENDERER_DMA2D
#endif // KLST_PANDA_STM32
#endif // KLST_PERIPHERAL_ENABLE_DISPLAY
