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

#include "KlangstromEnvironment.h"
#ifdef KLST_PANDA_STM32
#include "Klangstrom_BSP_KLST_PANDA_STM32-Config.h"
#ifdef KLST_PANDA_ENABLE_LEDS

#include "tim.h"

#include "KlangstromLEDs.h"
#include "KlangstromSerialDebug.h"

void KLST_BSP_leds_init() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, _LED_00_Pin | _LED_01_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : _LED_00_Pin _LED_01_Pin */
    GPIO_InitStruct.Pin   = _LED_00_Pin | _LED_01_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* --- */

    MX_TIM12_Init();

    /* --- */

    if (HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_2) != HAL_OK) {
        Error_Handler();
    }
}

void KLST_BSP_leds_set(const int id, float intensity) {
    uint32_t mDutyCycle = static_cast<uint32_t>(65535 * intensity);
    if (id == 0) {
        __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, mDutyCycle);
    } else if (id == 1) {
        __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_2, mDutyCycle);
    }
}

uint8_t KLST_BSP_leds_total() {
    return 2;
}

#include "KlangstromLEDs.h"

void LEDs::KLST_BSP_init() {}

#endif // KLST_PANDA_ENABLE_LEDS
#endif // KLST_PANDA_STM32
