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
#ifdef KLST_ARCH_IS_EMU

#include "System.h"
#include "Console.h"
#include "HAL_ASP_EMU.h"

#ifdef __cplusplus
extern "C" {
#endif

AudioDevice* system_init_audiocodec() { // TOOD this is BSP
    AudioInfo audioinfo;
    audioinfo.sample_rate     = 48000;
    audioinfo.output_channels = 2;
    audioinfo.input_channels  = 2;
    audioinfo.block_size      = 128;
    audioinfo.bit_depth       = 16;
    AudioDevice* audiodevice  = audiodevice_init_audiocodec(&audioinfo);
    if (audioinfo.device_id == AUDIO_DEVICE_INIT_ERROR) {
        console_timestamp();
        console_error("error initializing audio device");
    }
    audiodevice_resume(audiodevice);
    return audiodevice;
}

void system_init_BSP() {
}

uint32_t system_get_tick_BSP() {
    return HAL_GetTick();
}

void system_reset_cycles() {
    // TODO implemenmt this
}

uint32_t system_get_cycles_BSP() {
    // Get the current time in nanoseconds
    auto now     = std::chrono::high_resolution_clock::now();
    auto time_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();

    // Convert the time to MCU cycles
    uint64_t mcu_cycles = static_cast<uint64_t>((time_ns * system_clock_frequency()) / 1e9);
    return mcu_cycles;
}

uint32_t system_clock_frequency() {
    return ???;
}

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_EMU
