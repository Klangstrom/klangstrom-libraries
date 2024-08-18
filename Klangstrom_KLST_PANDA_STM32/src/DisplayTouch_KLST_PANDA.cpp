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

#include "Display.h"
#include "System.h"
#include "GPIOListener.h"
#include "Console.h"
#include "FT5206.h"
#include "i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

extern I2C_HandleTypeDef hi2c4;

static GPIOListener fGPIOListener; // TODO could be moved to `Display.cpp`
static uint16_t     fGPIOPin = _DISPLAY_TOUCH_INTERRUPT_Pin;

static void touch_callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == fGPIOPin) {
        console_println("TOUCH");
    }
}

void touch_setup() {
    MX_I2C4_Init();
    HAL_Delay(100);
    FT5206_init(&hi2c4);
    FT5206_print_info();

    fGPIOListener.callback = touch_callback;
    system_register_gpio_listener(&fGPIOListener);

    // // store just the function callback
    // ArrayList_GPIOCallback touch_callbacks;
    // arraylist_GPIOCallback_add(&touch_callbacks, fTouchCallback);
    //
    // // store the structure containing the function callback and the GPIO pin
    // ArrayList_GPIOListener touch_callbacks_struct;
    // GPIOListener           mGPIOCallbackListener = {_DISPLAY_TOUCH_INTERRUPT_Pin, KLST_HAL_GPIO_EXTI_Callback};
    // arraylist_GPIOListener_add(&touch_callbacks_struct, &mGPIOCallbackListener);

#ifdef KLST_TOUCH_CONFIGURE_TOUCH_AS_NORMAL_GPIO
    /* GPIO */
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pins : _LED_00_Pin _LED_01_Pin */
    GPIO_InitStruct.Pin   = _DISPLAY_TOUCH_INTERRUPT_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
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

#ifdef __cplusplus
}
#endif
#endif // KLST_PANDA_STM32
#endif // KLST_PERIPHERAL_ENABLE_DISPLAY