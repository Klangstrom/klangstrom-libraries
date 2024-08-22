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

#if KLST_DISPLAY_RENDERER == KLST_DISPLAY_RENDERER_SOFTWARE

extern DMA2D_HandleTypeDef hdma2d;

uint32_t blend_colors(const uint32_t original_color, const uint32_t color, const uint8_t alpha) {
    const uint8_t  inv   = 0xFF - alpha;
    const uint8_t  r     = (GET_RED(original_color) * inv + GET_RED(color) * alpha) >> 8;
    const uint8_t  g     = (GET_GREEN(original_color) * inv + GET_GREEN(color) * alpha) >> 8;
    const uint8_t  b     = (GET_BLUE(original_color) * inv + GET_BLUE(color) * alpha) >> 8;
    const uint32_t blend = RGBA(r, g, b, 0xFF);
    return blend;
}

// TODO ok so this is brutal … but works ;)
static void DrawImage(uint32_t*      data,
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

static void FillRect(const uint32_t color,
                     const uint16_t x,
                     const uint16_t y,
                     const uint16_t width,
                     const uint16_t height) {
    const uint8_t alpha = GET_ALPHA(color);
    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
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

#endif // KLST_DISPLAY_RENDERER==KLST_DISPLAY_RENDERER_SOFTWARE
#endif // KLST_PANDA_STM32
#endif // KLST_PERIPHERAL_ENABLE_DISPLAY
