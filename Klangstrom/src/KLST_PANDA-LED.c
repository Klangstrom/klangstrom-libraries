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
#include "KLST_PANDA-LED.h"

void LED_setup() {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, _LED_00_Pin | _LED_01_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : _LED_00_Pin _LED_01_Pin */
	GPIO_InitStruct.Pin = _LED_00_Pin | _LED_01_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void LED_turn_on(uint8_t LED) {
	if (LED == LED_00) {
		HAL_GPIO_WritePin(_LED_00_GPIO_Port, _LED_00_Pin, GPIO_PIN_SET);
	}
	if (LED == LED_01) {
		HAL_GPIO_WritePin(_LED_01_GPIO_Port, _LED_01_Pin, GPIO_PIN_SET);
	}
}

void LED_turn_off(uint8_t LED) {
	if (LED == LED_00) {
		HAL_GPIO_WritePin(_LED_00_GPIO_Port, _LED_00_Pin, GPIO_PIN_RESET);
	}
	if (LED == LED_01) {
		HAL_GPIO_WritePin(_LED_01_GPIO_Port, _LED_01_Pin, GPIO_PIN_RESET);
	}
}

void LED_toggle(uint8_t LED) {
	if (LED == LED_00) {
		HAL_GPIO_TogglePin(_LED_00_GPIO_Port, _LED_00_Pin);
	}
	if (LED == LED_01) {
		HAL_GPIO_TogglePin(_LED_01_GPIO_Port, _LED_01_Pin);
	}
}
