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

#pragma once

#include <stdint.h>

#include "KlangstromEnvironment.h"
#include "KlangstromDefines.h"
#include "KlangstromPeripherals.h"
#include "KlangstromMechanicalKey.h"

class Klangstrom {
public:
    Klangstrom();
    void init();
    void setup();
    void loop();

    /**
     * creates a mechanical key according to
     */
    MechanicalKey* create_mechancial_key(int mechanical_key_id);
private:
    bool is_initialized;
};

/* --- Board Specific Implementations (BSP) --- */

#ifdef __cplusplus
extern "C" {
#endif

void KLST_BSP_init();
void KLST_BSP_setup();
void KLST_BSP_loop();
extern KlangstromPeripherals peripherals;

#ifdef __cplusplus
}
#endif
