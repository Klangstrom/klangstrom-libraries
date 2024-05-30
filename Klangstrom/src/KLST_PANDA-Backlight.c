/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
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

#include "KlangstromEnvironment.h"
#ifdef KLST_PANDA_STM32

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#include <stdio.h>

#include "stm32h7xx_hal.h"

#include "KlangstromSerialDebug.h"
#include "KLST_PANDA-backlight.h"

extern TIM_HandleTypeDef htim3;

static uint32_t frame_counter = 0;
static const uint32_t fPeriod = 32768; // TODO is there a better way to do this?

void backlight_setup() {
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
}

void backlight_loop() {
	frame_counter++;
	const uint8_t mPhaseDivider = ((1 << (frame_counter % 5 + 2)));
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, fPeriod / mPhaseDivider);
	KLST_BSP_serialdebug_println("LCD backlight divider: %i", mPhaseDivider);
}

void backlight_set_brightness(float brightness) {
	uint32_t mPhase = (uint32_t) (fPeriod * brightness);
	mPhase = MAX(1, MIN(fPeriod, mPhase));
	KLST_BSP_serialdebug_println("backlight brightness: %f %li/%li", brightness, mPhase, fPeriod);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, mPhase > 0 ? mPhase : 1);
}

#endif // KLST_PANDA_STM32
