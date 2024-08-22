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

#if KLST_DISPLAY_RENDERER == KLST_DISPLAY_RENDERER_HAL_DMA2D

/* NOTE these functions are deliberately not placed in `extern "C" {}` block to allow overloading */

extern DMA2D_HandleTypeDef hdma2d;

static void DrawImage(uint32_t*      data,
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
    // HAL_DMA2D_ConfigLayer(&hdma2d, 0);
    // HAL_DMA2D_ConfigLayer(&hdma2d, 1);
    HAL_DMA2D_Start(&hdma2d,
                    reinterpret_cast<uint32_t>(data),
                    destination,
                    width,
                    height);
    HAL_DMA2D_PollForTransfer(&hdma2d, 20);
}

static void _DrawImage(uint32_t*      data,
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
    // HAL_DMA2D_ConfigLayer(&hdma2d, 1);
    // HAL_DMA2D_ConfigLayer(&hdma2d, 0);
    HAL_DMA2D_BlendingStart(&hdma2d, reinterpret_cast<uint32_t>(data), destination, destination, width, height);
    HAL_DMA2D_PollForTransfer(&hdma2d, 10);
}

// TODO this seems very cumbersome, maybe we can simplify this
static void FillRect(const uint32_t color,
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
    FillRect(color, x, y, width, height);
}

void display_clear(const uint32_t color) {
    FillRect(color, 0, 0, KLST_DISPLAY_WIDTH, KLST_DISPLAY_HEIGHT);
}

void display_image(uint32_t*      data,
                   const uint16_t x,
                   const uint16_t y,
                   const uint16_t width,
                   const uint16_t height) {
    DrawImage(data, x, y, width, height);
}
#endif // KLST_DISPLAY_RENDERER == KLST_DISPLAY_RENDERER_HAL_DMA2D
#endif // KLST_PANDA_STM32
#endif // KLST_PERIPHERAL_ENABLE_DISPLAY
