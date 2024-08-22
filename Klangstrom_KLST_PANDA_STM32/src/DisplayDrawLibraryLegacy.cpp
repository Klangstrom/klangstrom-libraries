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

/* NOTE these functions are deliberately not placed in `extern "C" {}` block to allow overloading */

// extern LTDC_HandleTypeDef  hltdc;
extern DMA2D_HandleTypeDef hdma2d;

static constexpr uint8_t BYTES_PER_PIXEL = 4;

uint32_t blend_colors(const uint32_t original_color, const uint32_t color, const uint8_t alpha) {
    const uint8_t  inv   = 0xFF - alpha;
    const uint8_t  r     = (GET_RED(original_color) * inv + GET_RED(color) * alpha) >> 8;
    const uint8_t  g     = (GET_GREEN(original_color) * inv + GET_GREEN(color) * alpha) >> 8;
    const uint8_t  b     = (GET_BLUE(original_color) * inv + GET_BLUE(color) * alpha) >> 8;
    const uint32_t blend = RGBA(r, g, b, 0xFF);
    return blend;
}

// TODO ok so this is brutal … but works ;)
static void DMA2D_DrawImage(uint32_t*      data,
                            const uint16_t x,
                            const uint16_t y,
                            const uint16_t width,
                            const uint16_t height) {
    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            const uint32_t color = data[j + i * width];
            const uint8_t  alpha = GET_ALPHA(color);
            if (alpha == 0xFF) {
                display_pixel(x + j, y + i, color);
            } else {
                const uint32_t original_color = display_get_pixel(x + j, y + i);
                const uint32_t blend_color    = blend_colors(original_color, color, alpha);
                display_pixel(x + j, y + i, blend_color);
            }
        }
    }
}


/* HAL DMA2D version */

static void ___DMA2D_DrawImage(uint32_t*      data,
                               const uint16_t x,
                               const uint16_t y,
                               const uint16_t width,
                               const uint16_t height) {
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
    HAL_DMA2D_Start(&hdma2d, reinterpret_cast<uint32_t>(data),
                    destination,
                    width,
                    height);
    HAL_DMA2D_PollForTransfer(&hdma2d, 20);
}

static void _DMA2D_DrawImage(uint32_t*      data,
                             const uint16_t x,
                             const uint16_t y,
                             const uint16_t width,
                             const uint16_t height) {
    const uint32_t offset      = (x + y * KLST_DISPLAY_WIDTH) * BYTES_PER_PIXEL;
    const uint32_t destination = LTDC_get_backbuffer_address() + offset;

    DMA2D_HandleTypeDef hdma2d;
    hdma2d.Instance          = DMA2D;
    hdma2d.Init.Mode         = DMA2D_M2M_BLEND;
    hdma2d.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
    hdma2d.Init.OutputOffset = KLST_DISPLAY_WIDTH - width;

    // Foreground
    hdma2d.LayerCfg[1].AlphaMode      = DMA2D_NO_MODIF_ALPHA;
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
    HAL_DMA2D_BlendingStart(&hdma2d, reinterpret_cast<uint32_t>(data), destination, destination, width, height);
    HAL_DMA2D_PollForTransfer(&hdma2d, 10);
}

// TODO this seems very cumbersome, maybe we can simplify this
static void DMA2D_FillRect(const uint32_t color,
                           const uint16_t x,
                           const uint16_t y,
                           const uint16_t width,
                           const uint16_t height) {
    hdma2d.Instance          = DMA2D;
    hdma2d.Init.Mode         = DMA2D_R2M;
    hdma2d.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
    hdma2d.Init.OutputOffset = KLST_DISPLAY_WIDTH - width;
    HAL_DMA2D_Init(&hdma2d);
    // HAL_DMA2D_ConfigLayer(&hdma2d, 0); // TODO not sure about this
    // HAL_DMA2D_ConfigLayer(&hdma2d, 1); // TODO not sure about this
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
    if (HAL_DMA2D_PollForTransfer(&hdma2d, 10) != HAL_OK) {
        Error_Handler();
    }
#else
    // tearDown();
    fDMA2DTransferComplete = false;
    HAL_DMA2D_Start_IT(&hdma2d,
                       color,
                       destination,
                       width,
                       height);
    // ReSharper disable once CppDFALoopConditionNotUpdated
    // while (!fDMA2DTransferComplete) { ; }
#endif
}

void display_rect_fill(const uint16_t x,
                       const uint16_t y,
                       const uint16_t width,
                       const uint16_t height,
                       const uint32_t color) {
    DMA2D_FillRect(color, x, y, width, height);
}

void display_clear(const uint32_t color) {
    DMA2D_FillRect(color, 0, 0, KLST_DISPLAY_WIDTH, KLST_DISPLAY_HEIGHT);
}

void display_image(uint32_t*      data,
                   const uint16_t x,
                   const uint16_t y,
                   const uint16_t width,
                   const uint16_t height) {
    DMA2D_DrawImage(data, x, y, width, height);
}

#endif // KLST_PANDA_STM32
#endif // KLST_PERIPHERAL_ENABLE_DISPLAY
