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

#include "System.h"
#include "Console.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SYSTEM_INITIAL_NUM_AUDIO_DEVICES 3

static ArrayList_AudioDevicePtr fAudioDeviceListeners;
static uint16_t                 fDeviceID;
static bool                     fSystemInitialized = false;

void system_init() {
    fDeviceID          = 0;
    fSystemInitialized = false;
    arraylist_AudioDevicePtr_init(&fAudioDeviceListeners, SYSTEM_INITIAL_NUM_AUDIO_DEVICES);
    system_init_BSP();
    console_clear();
    console_system_info();

    console_status("Sub-System (ASP/BSP) initialized");
    console_status("System initialized");
    console_status("");

    fSystemInitialized = true;
}

bool system_is_initialized() {
    return fSystemInitialized;
}

uint16_t system_get_unique_device_ID() {
    return fDeviceID++;
}

void system_register_audiodevice(AudioDevice* audiodevice) {
    arraylist_AudioDevicePtr_add(&fAudioDeviceListeners, audiodevice);
}

ArrayList_AudioDevicePtr* system_get_registered_audiodevices() {
    return &fAudioDeviceListeners;
}

#ifdef __cplusplus
}
#endif
