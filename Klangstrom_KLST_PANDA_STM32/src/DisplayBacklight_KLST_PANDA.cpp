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

#include <stdio.h>

#include "main.h"
#include "Display.h"
#include "Console.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

extern TIM_HandleTypeDef htim3;

static uint32_t frame_counter = 0;

void backlight_init() {
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
}

void display_set_backlight(const float brightness) {
    const uint32_t mPeriod = htim3.Init.Period;
    uint32_t       mPhase  = static_cast<uint32_t>(mPeriod * brightness);
    mPhase                 = MAX(1, MIN(mPeriod, mPhase));
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, mPhase > 0 ? mPhase : 1);
}

#ifdef __cplusplus
}
#endif
#endif // KLST_PANDA_STM32
#endif // KLST_PERIPHERAL_ENABLE_DISPLAY