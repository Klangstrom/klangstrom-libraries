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
#include "DisplayDrawLibrary_DMA2D_STM32.h"

#if KLST_DISPLAY_RENDERER == KLST_DISPLAY_RENDERER_RAW_DMA2D

/* NOTE these functions are deliberately not placed in `extern "C" {}` block to allow overloading */

extern DMA2D_HandleTypeDef hdma2d;

void display_clear(const uint32_t color) {
    display_rect_fill(0, 0, KLST_DISPLAY_WIDTH, KLST_DISPLAY_HEIGHT, color);
}


void display_rect_fill(const uint16_t x,
                             const uint16_t y,
                             const uint16_t width,
                             const uint16_t height,
                             const uint32_t color) {
    const uint32_t offset      = (x + y * KLST_DISPLAY_WIDTH) * BYTES_PER_PIXEL;
    const auto     destination = reinterpret_cast<uint32_t*>(LTDC_get_backbuffer_address() + offset);
    const bool     has_alpha   = GET_ALPHA(color) != 0xFF;
    const uint32_t operation   = has_alpha ? BLIT_OP_FILL_WITH_ALPHA : BLIT_OP_FILL;

    const BlitOp op_fill{
        .operation      = operation,             // operation
        .pSrc           = nullptr,               // (nU)
        .pClut          = nullptr,               // (nU)
        .pDst           = destination,           // framebuffer address
        .nSteps         = width,                 // width
        .nLoops         = height,                // height
        .srcLoopStride  = 0,                     // (nU)
        .dstLoopStride  = KLST_DISPLAY_WIDTH,    // buffer width
        .color          = color,                 // color
        .srcFormat      = 0,                     // (nU)
        .dstFormat      = DMA2D_OUTPUT_ARGB8888, // i.e `output` ( currently fixed to DMA2D_OUTPUT_ARGB8888 )
        .replaceBgAlpha = false,                 // only in `BLIT_OP_FILL_WITH_ALPHA`
        .replaceFgAlpha = false,                 // (nU)
    };
    // fDMA2DTransferComplete = false;
    setupDataFill(op_fill);
    // while (!fDMA2DTransferComplete) { ; }
    // TODO interrupt-based version does not work therefore `HAL_DMA2D_PollForTransfer` is used
    //      this should be fixed at some point
    if (HAL_DMA2D_PollForTransfer(&hdma2d, 5) != HAL_OK) {
        Error_Handler();
    }
}

void display_image(uint32_t*      data,
                         const uint16_t x,
                         const uint16_t y,
                         const uint16_t width,
                         const uint16_t height) {
    constexpr uint8_t BYTES_PER_PIXEL = 4;
    const uint32_t    offset          = (x + y * KLST_DISPLAY_WIDTH) * BYTES_PER_PIXEL;
    const auto        destination     = reinterpret_cast<uint32_t*>(LTDC_get_backbuffer_address() + offset);

    BlitOp op_data;
    op_data.operation      = BLIT_OP_COPY_ARGB8888; // (used)
    op_data.pSrc           = data;                  // image data address
    op_data.pClut          = nullptr;               // (nU)
    op_data.pDst           = destination;           // framebuffer address
    op_data.nSteps         = width;                 // (used)
    op_data.nLoops         = height;                // (used)
    op_data.srcLoopStride  = 0;                     // (used) TODO
    op_data.dstLoopStride  = KLST_DISPLAY_WIDTH;    // (used)
    op_data.color          = 0xFFFFFFFF;            // ARGB8888, alpha used for blending, RGB only used in `BLIT_OP_COPY_A4` + `BLIT_OP_COPY_A8`
    op_data.srcFormat      = DMA2D_INPUT_ARGB8888;  // (used) i.e `input`
    op_data.dstFormat      = DMA2D_OUTPUT_ARGB8888; // (used) i.e `output`
    op_data.replaceBgAlpha = false;                 // (used)
    op_data.replaceFgAlpha = false;                 // (used)
    setupDataCopy(op_data);
    if (HAL_DMA2D_PollForTransfer(&hdma2d, 5) != HAL_OK) {
        Error_Handler();
    }
}

#endif // KLST_DISPLAY_RENDERER == KLST_DISPLAY_RENDERER_RAW_DMA2D
#endif // KLST_PANDA_STM32
#endif // KLST_PERIPHERAL_ENABLE_DISPLAY
