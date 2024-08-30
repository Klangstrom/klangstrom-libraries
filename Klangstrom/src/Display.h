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

#include "TouchEvent.h"
#include "BitmapFont.h"
#include "Point.h"
#include "DisplayDrawInterface.h"

#define KLST_DISPLAY_RENDERER_SOFTWARE 1
#define KLST_DISPLAY_RENDERER_DMA2D 2
#define KLST_DISPLAY_RENDERER KLST_DISPLAY_RENDERER_DMA2D

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

static constexpr uint8_t BYTES_PER_PIXEL = 4;

typedef enum {
    INTERRUPT,
    POLLING,
    NO_TOUCHPANEL
} TouchPanelMode;

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

bool              display_init(bool double_buffered = false, TouchPanelMode touch_panel_mode = INTERRUPT); // implemented as BSP
void              display_deinit();                                                                        // implemented as BSP
void              display_set_backlight(float brightness);                                                 // implemented as BSP
void              display_enable_automatic_update(bool sync_to_v_blank);                                   // implemented as BSP
void              display_swap_buffer();                                                                   // implemented as BSP
void              display_switch_on();                                                                     // implemented as BSP
void              display_switch_off();                                                                    // implemented as BSP
bool              display_is_double_buffered();
void              display_set_update_callback(Callback_0_VOID callback);
void              display_fire_update_callback();
void              display_set_touch_callback(Callback_1_TOUCHEVENTPTR callback);
void              display_fire_touch_callback(TouchEvent* touchevent);
bool              display_init_BSP(TouchPanelMode touch_panel_mode);
volatile uint32_t display_get_backbuffer_address(void);
int16_t           display_get_width();
int16_t           display_get_height();

void display_LTDC_init(); // TODO rework this as BSP

// TODO add option to use touchscreen in polling mode
//     `bool touch_has_event();` + `void touch_mode(uint8_t mode);`, `mode=polling` or `mode=interrupt`
//     maybe merge with `has_touchpanel` flag so that mode could also be `no_touchpanel`
void touch_init(TouchPanelMode touch_panel_mode); // implemented as BSP
void touch_read(TouchEvent* touchevent);          // implemented as BSP
bool touch_has_event();                           // implemented as BSP

void backlight_init(); // implemented as BSP

#ifdef __cplusplus
}
#endif
