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
#include "Console.h"
#include "Display.h"

/* NOTE these functions are deliberately not placed in `extern "C" {}` block to allow overloading */

int16_t display_get_width() {
    return KLST_DISPLAY_WIDTH;
}

int16_t display_get_height() {
    return KLST_DISPLAY_HEIGHT;
}

void display_pixel(const uint16_t x, const uint16_t y, const uint32_t color) {
    if (x < KLST_DISPLAY_WIDTH && y < KLST_DISPLAY_HEIGHT) {
        const uint32_t offset                                                       = x + y * KLST_DISPLAY_WIDTH;
        reinterpret_cast<volatile uint32_t*>(LTDC_get_backbuffer_address())[offset] = color;
    }
}

uint32_t display_get_pixel(const uint16_t x, const uint16_t y) {
    if (x < KLST_DISPLAY_WIDTH && y < KLST_DISPLAY_HEIGHT) {
        const uint32_t offset = x + y * KLST_DISPLAY_WIDTH;
        return reinterpret_cast<volatile uint32_t*>(LTDC_get_backbuffer_address())[offset];
    }
    return 0;
}

void display_line_horizontal(const uint16_t x, const uint16_t y, const uint16_t length, const uint32_t color) {
    // const uint32_t offset      = (x + y * KLST_DISPLAY_WIDTH) * BYTES_PER_PIXEL;
    // const auto     destination = reinterpret_cast<uint32_t*>(LTDC_get_backbuffer_address() + offset);
    // lineFromColor(destination,
    //               length,
    //               color);
    // TODO what does this do?
    // lineFromARGB8888(reinterpret_cast<uint8_t*>(destination),
    //                  reinterpret_cast<const uint32_t*>(destination),
    //                  length,
    //                  0xFF);
}

void display_rect(const uint16_t x,
                  const uint16_t y,
                  const uint16_t width,
                  const uint16_t height,
                  const uint32_t color,
                  const bool     filled) {
    if (filled) {
        display_rect_fill(x, y, width, height, color);
        //     DMA2D_FillRect(color, x, y, width, height); // HAL version
    } else {
        display_rect_stroke(x, y, width, height, color);
    }
}

void display_rect_stroke(const uint16_t x,
                         const uint16_t y,
                         const uint16_t width,
                         const uint16_t height,
                         const uint32_t color) {
}

#endif // KLST_PANDA_STM32
#endif // KLST_PERIPHERAL_ENABLE_DISPLAY
