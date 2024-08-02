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
#ifdef KLST_ARCH_IS_STM32

#include "AudioCodec.h"
#include "KlangstromAudioCodec.h" // TODO remove this ASAP
#include "AudioCodec_ASP_STM32.h"

#ifdef __cplusplus
extern "C" {
#endif

void audiocodec_init_device_BSP(AudioDevice* audiodevice) {
    // TODO move functions from KlangstromAudioCodec.* over here
    KLST_BSP_audiocodec_init(audiodevice->audioinfo); // remove ASAP
    // TODO move functions from KlangstromAudioCodec.* over here
}

void audiocodec_set_peripheral_callbacks(AudioDevice* audiodevice, Callback_2_AUDIODEVICE_UI8 callback_rx, Callback_2_AUDIODEVICE_UI8 callback_tx) {
    audiodevice->peripherals->callback_rx = callback_rx;
    audiodevice->peripherals->callback_tx = callback_tx;
}

void audiocodec_deinit_peripherals(AudioDevice* audiodevice) {
    delete audiodevice->peripherals;
}

void audiocodec_deinit_BSP(AudioDevice* audiodevice) {
    // TODO move functions from KlangstromAudioCodec.* over here
    KLST_BSP_audiocodec_deinit(); // remove ASAP
    // TODO move functions from KlangstromAudioCodec.* over here
}


#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_STM32
#endif // KLST_PANDA_ENABLE_AUDIOCODEC
