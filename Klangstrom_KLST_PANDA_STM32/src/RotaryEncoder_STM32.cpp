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

#include <string>
#ifdef KLST_PERIPHERAL_ENABLE_ENCODER
#ifdef KLST_ARCH_IS_STM32

#include "main.h"
#include "tim.h"
#include "RotaryEncoder.h"
#include "RotaryEncoder_STM32.h"
#include "Console.h"

#ifdef __cplusplus
extern "C" {
#endif

static void enable_rotation_interrupts(const RotaryEncoderPeripherals* r) {
    __HAL_TIM_ENABLE_IT(r->timer_handle, TIM_IT_CC1);
    __HAL_TIM_ENABLE_IT(r->timer_handle, TIM_IT_CC2);
}

bool rotaryencoder_init_peripherals_BSP(RotaryEncoder* encoder) {
    encoder->peripherals                  = new RotaryEncoderPeripherals();
    RotaryEncoderPeripherals& peripherals = *encoder->peripherals;
    if (encoder->device_type == ROTARY_ENCODER_LEFT) {
        peripherals.timer_handle        = &htim1;
        peripherals.button_channel      = HAL_TIM_ACTIVE_CHANNEL_3;
        peripherals.button_channel_i    = TIM_CHANNEL_3;
        peripherals.gpio_port           = _ENCODER_00_BUTTON_GPIO_Port;
        peripherals.gpio_pin            = _ENCODER_00_BUTTON_Pin;
        peripherals.encoder_channel_a   = HAL_TIM_ACTIVE_CHANNEL_1;
        peripherals.encoder_channel_a_i = TIM_CHANNEL_1;
        peripherals.encoder_channel_b   = HAL_TIM_ACTIVE_CHANNEL_2;
        peripherals.encoder_channel_a_i = TIM_CHANNEL_2;
        peripherals.bsp_init            = MX_TIM1_Init;
        return true;
    }
    if (encoder->device_type == ROTARY_ENCODER_RIGHT) {
        peripherals.timer_handle        = &htim2;
        peripherals.button_channel      = HAL_TIM_ACTIVE_CHANNEL_4;
        peripherals.button_channel_i    = TIM_CHANNEL_4;
        peripherals.gpio_port           = _ENCODER_01_BUTTON_GPIO_Port;
        peripherals.gpio_pin            = _ENCODER_01_BUTTON_Pin;
        peripherals.encoder_channel_a   = HAL_TIM_ACTIVE_CHANNEL_1;
        peripherals.encoder_channel_a_i = TIM_CHANNEL_1;
        peripherals.encoder_channel_b   = HAL_TIM_ACTIVE_CHANNEL_2;
        peripherals.encoder_channel_b_i = TIM_CHANNEL_2;
        peripherals.bsp_init            = MX_TIM2_Init;
        return true;
    }
    return false;
}

bool rotaryencoder_init_BSP(RotaryEncoder* encoder) {
    if (encoder->peripherals == nullptr) {
        console_error("ERROR: peripherals not initialized");
        return false;
    }
    if (encoder->device_type == ROTARY_ENCODER_LEFT || encoder->device_type == ROTARY_ENCODER_RIGHT) {
        console_status("device type: %s", encoder->device_type == ROTARY_ENCODER_LEFT ? "ROTARY_ENCODER_LEFT" : "ROTARY_ENCODER_RIGHT");
        encoder->peripherals->bsp_init();
        enable_rotation_interrupts(encoder->peripherals);
        return true;
    }
    console_status("device type: custom(%i)", encoder->device_type);
    return false;
}
void rotaryencoder_start(const RotaryEncoder* encoder) {
    const auto& peripherals = *encoder->peripherals;
    HAL_TIM_Encoder_Start(peripherals.timer_handle, peripherals.encoder_channel_a_i);
    HAL_TIM_Encoder_Start(peripherals.timer_handle, peripherals.encoder_channel_b_i);
    HAL_TIM_IC_Start_IT(peripherals.timer_handle, peripherals.button_channel_i);
    // HAL_TIM_IC_Start_IT(peripherals.timer_handle, peripherals.encoder_channel_a_i);
    // HAL_TIM_IC_Start_IT(peripherals.timer_handle, peripherals.encoder_channel_b_i);
}

static int32_t get_encoder_value(const TIM_HandleTypeDef* htim) {
    return static_cast<int16_t>(__HAL_TIM_GET_COUNTER(htim)) * -1;
}

int32_t rotaryencoder_get_rotation(const RotaryEncoder* encoder) {
    if (encoder->peripherals->timer_handle == nullptr) {
        return 0;
    }
    return get_encoder_value(encoder->peripherals->timer_handle);
}

static void print_encoder_status(const TIM_HandleTypeDef* htim, const RotaryEncoderPeripherals& r, const char* name) {
    if (htim->Channel == r.button_channel) {
        const bool button_state = !HAL_GPIO_ReadPin(r.gpio_port, r.gpio_pin);
        console_status("%s: BUTTON %i", name, button_state);
    } else if (htim->Channel == r.encoder_channel_a || htim->Channel == r.encoder_channel_b) {
        const auto encoder_value = get_encoder_value(htim);
        console_status("%s: ROTATE %i", name, encoder_value);
    }
}

/**
 * event handler for rotary encoder ( button and rotation )
 * TODO maybe move this to System
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim) {
    const ArrayList_RotaryEncoderPtr& listeners = *rotaryencoder_get_listeners();
    for (size_t i = 0; i < listeners.size; i++) {
        const RotaryEncoder* encoder = listeners.data[i];
        if (encoder->peripherals->timer_handle == htim) {
            std::string name = std::string("TODO ( add callback ) LISTENER_") + std::to_string(i);
            print_encoder_status(htim, *encoder->peripherals, name.c_str());
        }
    }
}

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_STM32
#endif // KLST_PERIPHERAL_ENABLE_ENCODER