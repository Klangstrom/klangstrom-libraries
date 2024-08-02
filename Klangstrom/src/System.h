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

/**
 * this include is required to tell the silly arduino build system to include the other libraries
 */
#ifdef KLST_ARCH_IS_STM32
#include "System_ASP_STM32.h" // TODO rename these
#elif defined(KLST_ARCH_IS_EMU)
#include "Klangstrom_BSP_KLST_EMU.h"
#include "System_ASP_EMU.h" // TODO rename these
#else
#warning "no implementation for Klangstrom found ( this might be intentional )"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "AudioDevice.h"
#include "ArrayList.h"

DEFINE_ARRAYLIST(AudioDevice*, AudioDevicePtr)

void                      system_init();
void                      system_register_audiodevice(AudioDevice* audiodevice);
uint16_t                  system_get_unique_device_ID();
AudioDevice*              system_init_AudioCodec(); // TODO implement
ArrayList_AudioDevicePtr* system_get_audiodevices();

void system_init_BSP();

void console_printf(const char* format, ...);  // TODO remove this ASAP /* for debugging only */
void console_println(const char* format, ...); // TODO remove this ASAP /* for debugging only */

#ifdef __cplusplus
}
#endif
