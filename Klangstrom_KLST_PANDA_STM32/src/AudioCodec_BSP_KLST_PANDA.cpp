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

#include "Klangstrom_ASP_KLST_STM32-Config.h" // TODO change this to KLST_STM32 aka 'Architecture Specific' (ASP)
#ifdef KLST_PANDA_ENABLE_AUDIOCODEC
#include "KlangstromEnvironment.h"
#ifdef KLST_PANDA_STM32

#include "AudioCodec.h"
#include "KlangstromAudioCodec.h" // TODO remove this ASAP
#include "AudioCodec_ASP_STM32.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef AUDIOCODEC_RX
#define AUDIOCODEC_RX hsai_BlockA1
#endif // AUDIOCODEC_RX
#ifndef AUDIOCODEC_TX
#define AUDIOCODEC_TX hsai_BlockB1
#endif // AUDIOCODEC_TX
#ifndef AUDIOCODEC_CONFIG
#define AUDIOCODEC_CONFIG hi2c4
#endif // AUDIOCODEC_CONFIG

extern SAI_HandleTypeDef AUDIOCODEC_RX;
extern SAI_HandleTypeDef AUDIOCODEC_TX;
extern I2C_HandleTypeDef AUDIOCODEC_CONFIG;

void audiocodec_init_peripherals(AudioDevice* audiodevice) {
    audiodevice->peripherals                    = new AudioDevicePeripherals();
    audiodevice->peripherals->audiocodec_sai_rx = AUDIOCODEC_RX;
    audiodevice->peripherals->audiocodec_sai_tx = AUDIOCODEC_TX;
    audiodevice->peripherals->audiocodec_config = AUDIOCODEC_CONFIG;
}

#ifdef __cplusplus
}
#endif

#endif // KLST_PANDA_STM32
#endif // KLST_PANDA_ENABLE_AUDIOCODEC
