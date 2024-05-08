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

#include "KlangstromPeripherals.h"

// TODO move to BSP files

#ifdef BOARD_KLST_PANDA
KlangstromPeripherals::KlangstromPeripherals() :
        number_of_encoders(2), number_of_mechanicalkeys(2) {
    encoders = new Encoder[number_of_encoders];
    mechanicalkeys = new MechanicalKey[number_of_mechanicalkeys];
}
#elif BOARD_KLST_SHEEP
KlangstromPeripherals::KlangstromPeripherals() :
        number_of_encoders(0), number_of_mechanicalkeys(0) {
    mechanicalkeys = new MechanicalKey[number_of_mechanicalkeys];
}
#else // for development purposes
KlangstromPeripherals::KlangstromPeripherals() :
        number_of_encoders(2), number_of_mechanicalkeys(2) {
    encoders = new RotaryEncoder*[number_of_encoders];
    for (uint8_t i = 0; i < number_of_encoders; ++i) {
        encoders[i] = nullptr;
    }

    mechanicalkeys = new MechanicalKey*[number_of_mechanicalkeys];
    for (uint8_t i = 0; i < number_of_mechanicalkeys; ++i) {
        mechanicalkeys[i] = nullptr;
    }
}

KlangstromPeripherals::~KlangstromPeripherals() {
    for (uint8_t i = 0; i < number_of_encoders; ++i) {
        delete encoders[i];
    }
    delete[] encoders;

    for (uint8_t i = 0; i < number_of_mechanicalkeys; ++i) {
        delete mechanicalkeys[i];
    }
    delete[] mechanicalkeys;
}
#endif
