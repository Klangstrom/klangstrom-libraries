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

#include "KlangstromEnvironment.h"

#ifdef KLST_PANDA_STM32
#include "Klangstrom_BSP_KLST_PANDA_STM32.h"
#elif defined(KLST_ARCH_IS_EMU)
#include "Klangstrom_BSP_KLST_EMU.h"
#else
#warning "no implementation for Klangstrom found ( this might be intentional )"
#endif

class Klangstrom {
public:
    Klangstrom();
    void init();
    void setup() const;
    void loop() const;

private:
    bool is_initialized;
};

/* --- Board Specific (BSP) Implementations --- */

#ifdef __cplusplus
extern "C" {
#endif

void KLST_BSP_init();
void KLST_BSP_setup();
void KLST_BSP_loop();

#ifdef __cplusplus
}
#endif
