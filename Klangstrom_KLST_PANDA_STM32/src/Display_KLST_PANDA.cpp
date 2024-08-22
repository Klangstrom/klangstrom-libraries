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
#if defined(KLST_PANDA_STM32)

#ifndef KLST_PERIPHERAL_ENABLE_EXTERNAL_MEMORY
#error "KLST_PERIPHERAL_ENABLE_EXTERNAL_MEMORY must be defined for display"
#endif

#include "main.h"
#include "tim.h"
#include "ltdc.h"
#include "dma2d.h"
#include "Display.h"
#include "Console.h"

#ifdef __cplusplus
extern "C" {
#endif

bool display_init_BSP() {
    /* display = LTDC + backlight + touch panel */
    MX_LTDC_Init();
    // MX_DMA2D_Init();
    MX_TIM3_Init();

    display_switch_off();
    display_LTDC_init();
    display_switch_on();
    touch_init();
    backlight_init();
    display_set_backlight(0.5f);

    return true;
}

void display_switch_on() {
    HAL_GPIO_WritePin(_DISPLAY_ON_OFF_GPIO_Port, _DISPLAY_ON_OFF_Pin, GPIO_PIN_SET);
}

void display_switch_off() {
    HAL_GPIO_WritePin(_DISPLAY_ON_OFF_GPIO_Port, _DISPLAY_ON_OFF_Pin, GPIO_PIN_RESET);
}

#ifdef __cplusplus
}
#endif

#endif
#endif // KLST_PANDA_STM32
