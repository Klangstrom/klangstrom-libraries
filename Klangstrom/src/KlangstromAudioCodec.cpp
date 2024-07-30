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

#include "KlangstromAudioCodec.h"

/* --- callback_interface --- */

extern "C" {
static Callback_1_AUDIOBLOCKPTR fAudioCodecCallback = AudioCodec::process_audioblock_data;

//void audiocodec_register_callback(Callback_1_AUDIOBLOCKPTR pAudioCodecCallback) {
//    fAudioCodecCallback = pAudioCodecCallback;
//}

void KLST_BSP_audiocodec_process_audioblock_data(AudioBlock* audio_block) {
    /* fill buffer with zeros if callback is not set */
    if (!fAudioCodecCallback) {
        for (int j = 0; j < audio_block->output_channels; ++j) {
            for (int i = 0; i < audio_block->block_size; i++) {
                audio_block->output[j][i] = 0;
            }
        }
        return;
    }
    fAudioCodecCallback(audio_block);
}

} // extern "C"

/**
 * default audio block callback. can be overwritten in application
 * @param audio_block
 */
void WEAK audioblock(AudioBlock* audio_block) {
    //void WEAK audioblock(float** input_signal, float** output_signal, uint16_t length) {
    for (int i = 0; i < audio_block->block_size; ++i) {
        for (int j = 0; j < audio_block->output_channels; ++j) {
            audio_block->output[j][i] = 0.0;
        }
    }
}

/**
 * fills buffer with new block of samples from device with ID specified in `AudioBlock`
 * TODO look into implementinng an integer version as in `callback_class_i` for more efficient MCU
 */
void AudioCodec::process_audioblock_data(AudioBlock* audio_block) {
    const uint8_t id = audio_block->device_id;
#ifdef USE_MUTEX
    std::lock_guard<std::mutex> lock(instances_mutex);
#endif
    if (id >= 0 && id < instances.size() && instances[id]) {
        if (instances[id]->callback_audioblock) {
            instances[id]->callback_audioblock(audio_block);
        }
    }
}

/* --- callback_interface --- */

std::vector<AudioCodec*> AudioCodec::instances;
//bool                     AudioCodec::fRegisteredCallback = false;
#ifdef USE_MUTEX
std::mutex AudioCodec::instances_mutex;
#endif

AudioCodec::AudioCodec() : isInitialized(false),
                           fAudioInfo(nullptr),
                           mDeleteAudioInfo(false),
                           callback_audioblock(audioblock) {
    //    if (!fRegisteredCallback) {
    //        audiocodec_register_callback(AudioCodec::process_audioblock_data);
    //        fRegisteredCallback = true;
    //    }
}

AudioCodec::~AudioCodec() {
    {
#ifdef USE_MUTEX
        // TODO mutex lock causes:
        // TODO libc++abi: terminating due to uncaught exception of type std::__1::system_error: mutex lock failed: Invalid argument
//        std::lock_guard<std::mutex> lock(instances_mutex);
#endif
        if (id < instances.size()) {
            instances[id] = nullptr;
        }
    }
    if (mDeleteAudioInfo) {
        delete fAudioInfo;
    }
}

uint8_t AudioCodec::init(uint32_t sample_rate,
                         uint8_t  output_channels,
                         uint8_t  input_channels,
                         uint16_t block_size,
                         uint8_t  bit_depth,
                         uint8_t  device_type) {
    if (!isInitialized) {
        fAudioInfo                  = new AudioInfo();
        fAudioInfo->sample_rate     = sample_rate;
        fAudioInfo->output_channels = output_channels;
        fAudioInfo->input_channels  = input_channels;
        fAudioInfo->block_size      = block_size;
        fAudioInfo->bit_depth       = bit_depth;
        fAudioInfo->device_type     = device_type;
        mDeleteAudioInfo            = true;
        return init(fAudioInfo);
    }
    return AUDIO_DEVICE_INIT_ERROR;
}

uint8_t AudioCodec::init(AudioInfo* audioinfo) {
    uint8_t deviceID = AUDIO_DEVICE_INIT_ERROR;
    if (!isInitialized) {
        if (audioinfo == nullptr) {
            fAudioInfo       = new AudioInfo();
            mDeleteAudioInfo = true;
        } else {
            fAudioInfo = audioinfo;
        }
        deviceID = KLST_BSP_audiocodec_init(fAudioInfo);
        if (deviceID == AUDIO_DEVICE_INIT_ERROR) {
            return deviceID;
        } else {
            BSP_init(fAudioInfo);
            isInitialized = true;
            id            = deviceID;
            /* register instance in collector */
            {
#ifdef USE_MUTEX
                std::lock_guard<std::mutex> lock(instances_mutex);
#endif
                if (instances.size() <= id) {
                    instances.resize(id + 1, nullptr);
                }
                instances[id] = this;
            }
        }
    }
    return deviceID;
}
