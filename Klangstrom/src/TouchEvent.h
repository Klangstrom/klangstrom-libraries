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

#define KLST_MAX_NUMBER_OF_TOUCHES 5

typedef struct TouchEvent {
    uint16_t x[KLST_MAX_NUMBER_OF_TOUCHES];
    uint16_t y[KLST_MAX_NUMBER_OF_TOUCHES];
    uint8_t  number_of_touches;
    uint8_t  gesture_id;
} TouchEvent;