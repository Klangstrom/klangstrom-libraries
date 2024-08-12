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

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WEAK
#define WEAK __attribute__((weak))
#endif

typedef void (*Callback_0_VOID)();

/**
 * callback to be implemented by client application
 */
WEAK void display_update_event();

bool display_init();                          // implemented as BSP
void display_deinit();                        // implemented as BSP
void display_set_backlight(float brightness); // implemented as BSP

void display_switch_on();
void display_switch_off();

void LTDC_setup();                                    // TODO rename to `display_setup()`
void LTDC_loop();                                     // TODO ...
void LTDC_switch_framebuffer();                       // TODO ...
void LTDC_sync_to_vertical_blank(bool pSyncToVBlank); // TODO add callback

void touch_setup();
void touch_read();

void backlight_setup();
void backlight_loop();
void backlight_set_brightness(float brightness);

#ifdef __cplusplus
}
#endif