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
#ifdef KLST_ARCH_IS_STM32

#include "System.h"
#include "AudioDevice.h"

#ifdef __cplusplus
extern "C" {
#endif

WEAK void audioblock(AudioBlock* audio_block) {
    for (int i = 0; i < audio_block->block_size; ++i) {
        for (int j = 0; j < audio_block->output_channels; ++j) {
            audio_block->output[j][i] = 0.0;
        }
    }
}

static bool fCleanUpMemory = false;

AudioDevice* audiodevice_init(AudioInfo* audioinfo) {
    AudioDevice* audiodevice                = new AudioDevice();
    audiodevice->audioinfo                  = new AudioInfo();
    audiodevice->audioinfo->sample_rate     = audioinfo->sample_rate;
    audiodevice->audioinfo->output_channels = audioinfo->output_channels;
    audiodevice->audioinfo->input_channels  = audioinfo->input_channels;
    audiodevice->audioinfo->block_size      = audioinfo->block_size;
    audiodevice->audioinfo->bit_depth       = audioinfo->bit_depth;
    audiodevice->audioinfo->device_type     = audioinfo->device_type;
    audiodevice->audioinfo->device_id       = AUDIO_DEVICE_ID_UNDEFINED;

    audiodevice_init_peripherals(audiodevice);

    audiodevice->audioblock                  = new AudioBlock();
    audiodevice->audioblock->output_channels = audiodevice->audioinfo->output_channels;
    audiodevice->audioblock->input_channels  = audiodevice->audioinfo->input_channels;
    audiodevice->audioblock->block_size      = audiodevice->audioinfo->block_size;
    audiodevice->audioblock->output          = new float*[audiodevice->audioblock->output_channels];
    audiodevice->audioblock->input           = new float*[audiodevice->audioblock->input_channels];
    for (int i = 0; i < audiodevice->audioblock->output_channels; ++i) {
        audiodevice->audioblock->output[i] = new float[audiodevice->audioblock->block_size];
    }
    for (int i = 0; i < audiodevice->audioblock->input_channels; ++i) {
        audiodevice->audioblock->input[i] = new float[audiodevice->audioblock->block_size];
    }

    audiodevice_init_device(audiodevice);
    fCleanUpMemory = true;

    return audiodevice;
}

void audiodevice_init_device(AudioDevice* audiodevice) {
    audiodevice_init_device_BSP(audiodevice);
    audiodevice->audioinfo->device_id = system_get_unique_device_ID();
    audiodevice->callback_audioblock  = audioblock;
    system_register_audiodevice(audiodevice);
}

void audiodevice_deinit(AudioDevice* audiodevice) {
    audiodevice_deinit_BSP(audiodevice);
    audiodevice_deinit_peripherals(audiodevice);
    if (fCleanUpMemory) {
        for (int i = 0; i < audiodevice->audioblock->output_channels; ++i) {
            delete[] audiodevice->audioblock->output[i];
        }
        delete[] audiodevice->audioblock->output;
        for (int i = 0; i < audiodevice->audioblock->input_channels; ++i) {
            delete[] audiodevice->audioblock->input[i];
        }
        delete[] audiodevice->audioblock->input;
        delete audiodevice->audioblock;
        delete audiodevice->audioinfo;
        delete audiodevice;
    }
}

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_STM32