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
#ifdef KLST_ARCH_IS_STM32

#include <stdint.h>
#include "Console.h"
#include "TimeTimer.h"
#include "tim.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * TODO maybe move this to System if other peripherals need timer interrupts too
 */
// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
//     console_status("HAL_TIM_PeriodElapsedCallback");
// }

#ifdef __cplusplus
}
#endif

void TimeTimer::init(const uint8_t instance) {
    // Duration in Seconds = (ARR * PSC) / SYS_CLK
    // Duration in Seconds = (ARR * 5500) / 275000000 // for ARR=1
    // Duration in Seconds = 0.00002sec = 20us

    if (instance == 7) {
        // each period should last for 20us
        fTimerID = 7;
        MX_TIM7_Init();

        htim7.Instance               = TIM7;
        htim7.Init.Prescaler         = 5500 - 1;
        htim7.Init.CounterMode       = TIM_COUNTERMODE_UP;
        htim7.Init.Period            = (50000 + 150) - 1; // TODO somehow the timer is off by 150 ticks i.e 3000us
        htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
        if (HAL_TIM_Base_Init(&htim7) != HAL_OK) {
            Error_Handler();
        }

        const HAL_StatusTypeDef result = HAL_TIM_Base_Start_IT(&htim7);
        if (result != HAL_OK) {
            console_error("TimeTimer init: instance %i failed to start", instance);
        }
    } else if (instance == 13) {
        fTimerID = 13;
        MX_TIM13_Init();
        const HAL_StatusTypeDef result = HAL_TIM_Base_Start_IT(&htim13);
        if (result != HAL_OK) {
            console_error("TimeTimer init: instance %i failed to start", instance);
        }
    } else if (instance == 14) {
        fTimerID = 14;
        MX_TIM14_Init();
        const HAL_StatusTypeDef result = HAL_TIM_Base_Start_IT(&htim14);
        if (result != HAL_OK) {
            console_error("TimeTimer init: instance %i failed to start", instance);
        }
    } else {
        fTimerID = -1;
        console_error("TimeTimer init: instance %i not supported", instance);
    }
}

void TimeTimer::setOverflow(uint32_t duration, TimerFormat_t format) {}

void TimeTimer::resume() {
    console_println("TimeTimer_STM32: instance %i", instance);
}

void TimeTimer::pause() {
}

bool TimeTimer::isRunning() const { return true; }

TimeTimer::~TimeTimer() {}

TimeTimer* TimeTimer::instance() {
    return new TimeTimer();
}

#endif // KLST_ARCH_IS_STM32