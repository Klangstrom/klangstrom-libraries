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

#pragma once

#include <cstdint>
#include "ArrayList.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WEAK
#define WEAK __attribute__((weak))
#endif

static constexpr uint8_t ROTARY_ENCODER_ID_UNDEFINED = 255;
static constexpr uint8_t ROTARY_ENCODER_LEFT         = 0;
static constexpr uint8_t ROTARY_ENCODER_RIGHT        = 1;
static constexpr uint8_t ROTARY_ENCODER_00           = ROTARY_ENCODER_LEFT;
static constexpr uint8_t ROTARY_ENCODER_01           = ROTARY_ENCODER_RIGHT;

struct RotaryEncoderPeripherals; /* BSP or ASP implementation */

typedef struct RotaryEncoder {
    RotaryEncoderPeripherals* peripherals = nullptr;
    uint8_t                   device_type = ROTARY_ENCODER_ID_UNDEFINED;
    uint8_t                   device_id   = ROTARY_ENCODER_ID_UNDEFINED;
} RotaryEncoder;

DEFINE_ARRAYLIST(RotaryEncoder*, RotaryEncoderPtr)

RotaryEncoder* rotaryencoder_create(uint8_t device_type);
void           rotaryencoder_delete(RotaryEncoder* encoder);

int32_t                     rotaryencoder_get_rotation(const RotaryEncoder* encoder); // implemented as BSP
void                        rotaryencoder_start(const RotaryEncoder* encoder);
void                        rotaryencoder_stop(RotaryEncoder* encoder);
void                        rotaryencoder_register_listener(RotaryEncoder* encoder);
ArrayList_RotaryEncoderPtr* rotaryencoder_get_listeners();

bool rotaryencoder_init_BSP(RotaryEncoder* encoder);
void rotaryencoder_deinit_BSP(RotaryEncoder* encoder);
bool rotaryencoder_init_peripherals_BSP(RotaryEncoder* encoder);
void rotaryencoder_deinit_peripherals_BSP(RotaryEncoder* encoder);

#ifdef __cplusplus
}
#endif
