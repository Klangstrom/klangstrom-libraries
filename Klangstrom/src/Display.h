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

#pragma once

#include <stdint.h>
#include <vector>

#include "TouchEvent.h"
#include "BitmapFont.h"

#define KLST_DISPLAY_RENDERER_SOFTWARE 1
#define KLST_DISPLAY_RENDERER_HAL_DMA2D 2
#define KLST_DISPLAY_RENDERER_RAW_DMA2D 3
#define KLST_DISPLAY_RENDERER_DMA2D 4

#define KLST_DISPLAY_RENDERER KLST_DISPLAY_RENDERER_DMA2D

static constexpr uint8_t BYTES_PER_PIXEL = 4;

typedef struct Point {
    Point()
        : x(0),
          y(0) {}
    Point(const uint16_t x, const uint16_t y)
        : x(x),
          y(y) {}

    int16_t x;
    int16_t y;
} Point;

typedef enum {
    CENTERED = 0x01,
    RIGHT    = 0x02,
    LEFT     = 0x03
} TextAlign;

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WEAK
#define WEAK __attribute__((weak))
#endif

typedef void (*Callback_0_VOID)();
typedef void (*Callback_1_TOUCHEVENTPTR)(TouchEvent*);

/**
 * callback to be implemented by client application
 */
WEAK void display_update_event();
WEAK void display_touch_event(TouchEvent* touchevent);

bool display_init(bool double_buffered = false, bool has_touchpanel = true); // implemented as BSP
void display_deinit();                                                       // implemented as BSP
void display_set_backlight(float brightness);                                // implemented as BSP
void display_enable_automatic_update(bool sync_to_v_blank);                  // implemented as BSP
void display_swap_buffer();                                                  // implemented as BSP
void display_switch_on();                                                    // implemented as BSP
void display_switch_off();                                                   // implemented as BSP
bool display_is_double_buffered();
void display_set_update_callback(Callback_0_VOID callback);
void display_fire_update_callback();
void display_set_touch_callback(Callback_1_TOUCHEVENTPTR callback);
void display_fire_touch_callback(TouchEvent* touchevent);
bool display_init_BSP(bool has_touchpanel);
void display_LTDC_init(); // implemented as BSP

volatile uint32_t LTDC_get_backbuffer_address(void);

void touch_init();                       // implemented as BSP
void touch_read(TouchEvent* touchevent); // implemented as BSP

void backlight_init(); // implemented as BSP

#ifdef __cplusplus
}
#endif

/* draw library */

#define ARGB_TO_RGBA(argb)             \
    (((argb) & 0xFF000000) >> 24) |    \
        (((argb) & 0x00FF0000) >> 8) | \
        (((argb) & 0x0000FF00) << 8) | \
        (((argb) & 0x000000FF) << 24)

#define RGBA_TO_ARGB(rgba)             \
    (((rgba) & 0xFF000000) >> 24) |    \
        (((rgba) & 0x00FF0000) >> 8) | \
        (((rgba) & 0x0000FF00) << 8) | \
        (((rgba) & 0x000000FF) << 24)

#define RGB_ALPHA_TO_ARGB(rgb, alpha) \
    (((alpha) << 24) | ((rgb) & 0x00FFFFFF))

#define RGBA(r, g, b, a) \
    (((uint32_t) (a) << 24) | ((uint32_t) (r) << 16) | ((uint32_t) (g) << 8) | ((uint32_t) (b)))

#define RGB(r, g, b) \
    (((uint32_t) (0xFF) << 24) | ((uint32_t) (r) << 16) | ((uint32_t) (g) << 8) | ((uint32_t) (b)))

#define BRIGHTNESS(b) \
    (((uint32_t) (0xFF) << 24) | ((uint32_t) (b) << 16) | ((uint32_t) (b) << 8) | ((uint32_t) (b)))

#define BRIGHTNESS_ALPHA(b, a) \
    (((uint32_t) (a) << 24) | ((uint32_t) (b) << 16) | ((uint32_t) (b) << 8) | ((uint32_t) (b)))

#define GET_ALPHA(argb) ((uint8_t) ((argb) >> 24))
#define GET_RED(argb) ((uint8_t) ((argb) >> 16))
#define GET_GREEN(argb) ((uint8_t) ((argb) >> 8))
#define GET_BLUE(argb) ((uint8_t) (argb))

/* NOTE all functions below are implemented BSP */

int16_t  display_get_width();
int16_t  display_get_height();
void     display_clear(uint32_t color);
void     display_pixel(uint16_t x, uint16_t y, uint32_t color);
uint32_t display_get_pixel(uint16_t x, uint16_t y);
void     display_line_horizontal(uint16_t x, uint16_t y, uint16_t length, uint32_t color); // BSP
void     display_line_vertical(uint16_t x, uint16_t y, uint16_t length, uint32_t color);   // BSP
void     display_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color);
void     display_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, bool filled);
void     display_rect_fill(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color); // BSP
void     display_rect_stroke(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
void     display_circle_stroke(uint16_t x, uint16_t y, uint16_t radius, uint32_t color);
void     display_image(uint32_t* data, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void     display_triangle_fill(uint16_t x0, uint16_t x1, uint16_t x2, uint16_t y0, uint16_t y1, uint16_t y2, uint32_t color);
void     display_polygon_stroke(const std::vector<Point>& points, uint32_t color);
void     display_polygon_fill(const std::vector<Point>& points, uint32_t color); // WIP
void     display_text(BitmapFont* font,
                      uint16_t    x,
                      uint16_t    y,
                      const char* text,
                      TextAlign   align,
                      uint32_t    color,
                      uint32_t    background_color);
void     display_char(BitmapFont* font,
                      uint16_t    x,
                      uint16_t    y,
                      uint8_t     ascii_char,
                      uint32_t    color,
                      uint32_t    background_color);