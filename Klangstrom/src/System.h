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
#include "System_ASP_EMU.h" // TODO rename these
#else
#warning "no implementation for Klangstrom found ( this might be intentional )"
#endif

#include "AudioDevice.h"
#include "SerialDevice.h"
#include "ArrayList.h"
#include "Tools.h"

#ifdef __cplusplus
extern "C" {
#endif

DEFINE_ARRAYLIST(AudioDevice*, AudioDevicePtr)
DEFINE_ARRAYLIST(SerialDevice*, SerialDevicePtr)

void                       system_init();
void                       system_register_audiodevice(AudioDevice* audiodevice);
ArrayList_AudioDevicePtr*  system_get_registered_audiodevices();
void                       system_register_serialdevice(SerialDevice* serialdevice);
ArrayList_SerialDevicePtr* system_get_registered_serialdevices();
uint16_t                   system_get_unique_device_ID();
bool                       system_is_initialized();
uint32_t                   system_get_tick();
uint32_t                   system_get_cycles();
uint32_t                   system_clock_frequency(); // implemented as BSP
float                      system_cycles_to_micros(uint32_t cycles);

/**
 * @brief initialize the default audio codec and starts it
 * @return instance to audio device (audio codec). needs to be freed by caller
 */
AudioDevice* system_init_audiocodec(); // NOTE implemented as BSP for now

void     system_init_BSP();
uint32_t system_get_tick_BSP();
uint32_t system_get_cycles_BSP();

#ifdef __cplusplus
}
#endif
