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

#include "RotaryEncoder.h"

#ifdef __cplusplus
extern "C" {
#endif

static ArrayList_RotaryEncoderPtr fSerialDeviceListeners;

RotaryEncoder* rotaryencoder_create(uint8_t device_type) {
    auto* encoder        = new RotaryEncoder();
    encoder->device_type = device_type;
    rotaryencoder_init_peripherals_BSP(encoder);
    rotaryencoder_init_BSP(encoder);
    rotaryencoder_register_listener(encoder);
    rotaryencoder_start(encoder);
    return encoder;
}

void rotaryencoder_delete(RotaryEncoder* encoder) {
    rotaryencoder_deinit_peripherals_BSP(encoder);
    rotaryencoder_deinit_BSP(encoder);
    delete encoder;
}

void rotaryencoder_register_listener(RotaryEncoder* encoder) {
    arraylist_RotaryEncoderPtr_add(&fSerialDeviceListeners, encoder);
}

ArrayList_RotaryEncoderPtr* rotaryencoder_get_listeners() {
    return &fSerialDeviceListeners;
}

#ifdef __cplusplus
}
#endif
