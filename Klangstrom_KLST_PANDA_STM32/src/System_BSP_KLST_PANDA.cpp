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

#include "KlangstromEnvironment.h"
#ifdef KLST_PANDA_STM32

#ifdef __cplusplus
extern "C" {
#endif

#include "AudioDevice.h"
#include "Console.h"

AudioDevice* system_init_audiocodec() {
    AudioInfo audioinfo;
    audioinfo.sample_rate     = 48000;
    audioinfo.output_channels = 2;
    audioinfo.input_channels  = 2;
    audioinfo.block_size      = 128;
    audioinfo.bit_depth       = 16;
    AudioDevice* audiodevice  = audiodevice_init(&audioinfo);
    if (audioinfo.device_id == AUDIO_DEVICE_INIT_ERROR) {
        console_timestamp();
        console_println("error initializing audio device");
    }
    audiodevice_start(audiodevice);
    return audiodevice;
}

#ifdef __cplusplus
}
#endif

#endif // KLST_PANDA_STM32
