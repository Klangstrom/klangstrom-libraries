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

#include "main.h"

#include "KlangstromSerialDebug.h"
#include "KLST_PANDA-Touch.h"
#include "FT5206.h"

extern I2C_HandleTypeDef hi2c4;

void touch_setup() {
    FT5206_init(&hi2c4);
    FT5206_print_info();

#ifdef TOUCH_CONFIGURE_TOUCH_AS_NORMAL_GPIO
    /* GPIO */
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pins : _LED_00_Pin _LED_01_Pin */
    GPIO_InitStruct.Pin = _DISPLAY_TOUCH_INTERRUPT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(_DISPLAY_TOUCH_INTERRUPT_GPIO_Port, &GPIO_InitStruct);
#endif
}

void touch_read() {
    // TODO return values
//	if (!HAL_GPIO_ReadPin(_DISPLAY_TOUCH_INTERRUPT_GPIO_Port, _DISPLAY_TOUCH_INTERRUPT_Pin)) {
//		FT5206_read();
//	}
    FT5206_read();
}
