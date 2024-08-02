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

#include "KlangstromAudio.h"
#include "KlangstromCallbackDefinitions.h"

struct AudioDevicePeripherals;

typedef struct AudioDevice {
    AudioInfo*               audioinfo;
    AudioBlock*              audioblock;
    AudioDevicePeripherals*  peripherals;
    Callback_1_AUDIOBLOCKPTR callback_audioblock;
} AudioDevice;

typedef void (*Callback_2_AUDIODEVICE_UI8)(AudioDevice*, uint8_t);

#ifdef __cplusplus
extern "C" {
#endif

WEAK void audioblock(AudioBlock* audio_block);

AudioDevice* audiocodec_init_device(AudioDevice* audiodevice);
AudioDevice* audiocodec_init(AudioInfo* audioinfo);
void         audiocodec_deinit(AudioDevice* audiodevice);
void         audiocodec_start(AudioDevice* audiodevice);
void         audiocodec_stop(AudioDevice* audiodevice);
void         audiocodec_init_peripherals(AudioDevice* audiodevice);
void         audiocodec_deinit_peripherals(AudioDevice* audiodevice);
void         audiocodec_set_peripheral_callbacks(AudioDevice* audiodevice, Callback_2_AUDIODEVICE_UI8 callback_rx, Callback_2_AUDIODEVICE_UI8 callback_tx);

void audiocodec_init_device_BSP(AudioDevice* audiodevice);

#ifdef __cplusplus
}
#endif