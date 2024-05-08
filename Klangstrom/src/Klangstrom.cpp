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

#include "Klangstrom.h"

KlangstromPeripherals peripherals;

Klangstrom::Klangstrom() :
        is_initialized(false) {
}

void Klangstrom::init() {
    if (!is_initialized) {
        KLST_BSP_init();
        is_initialized = true;
    } else {
//        println("Klangstrom: already initialized");
    }
}

void Klangstrom::setup() {
    if (is_initialized) {
        KLST_BSP_setup();
    } else {
//        println("Klangstrom: system needs to be initialized with `init()` before `setup()` is called");
    }
}

void Klangstrom::loop() {
    if (is_initialized) {
        KLST_BSP_loop();
    }
}

MechanicalKey* Klangstrom::create_mechancial_key(int mechanical_key_id) {
    if (mechanical_key_id >= 0 && mechanical_key_id < peripherals.number_of_mechanicalkeys) {
        if (peripherals.mechanicalkeys[mechanical_key_id] != nullptr) {
            delete peripherals.mechanicalkeys[mechanical_key_id];
        }
        peripherals.mechanicalkeys[mechanical_key_id] = new MechanicalKey(mechanical_key_id);
        return peripherals.mechanicalkeys[mechanical_key_id];
    }
    return nullptr;
}

