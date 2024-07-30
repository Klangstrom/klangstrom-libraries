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
//static AudioCodec* mAudioCodecClass = nullptr;
static Callback_1_AUDIOBLOCKPTR fAudioCodecCallback;

void audiocodec_register_callback(Callback_1_AUDIOBLOCKPTR pAudioCodecCallback) {
    fAudioCodecCallback = pAudioCodecCallback;
}

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

//void audiocodec_register_audio_device(AudioCodec* pAudioCodecClass) {
//    mAudioCodecClass = pAudioCodecClass;
//}

///**
// * callback for underlying hardware layer … with either uint32 or float type
//*/
//void audiocodec_callback_class_i(uint32_t* input, uint32_t* output, uint16_t length) {
//    if (mAudioCodecClass) {
//        mAudioCodecClass->callback_class_i(input, output, length);
//    }
//}
//
//void audiocodec_callback_class_f(float** input, float** output, uint16_t length) {
//    if (mAudioCodecClass) {
//        mAudioCodecClass->callback_class_f(input, output, length);
//    }
//}
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

// TODO clean up the code below. flexible bit depth, buffer length etcetera

/**
 * fills buffer with new block of samples
 */
//void AudioCodec::callback_class_i(uint32_t* input, uint32_t* output, uint16_t length) {
//    // TODO `length` does not necessarily equal `KLANG_SAMPLES_PER_AUDIO_BLOCK`
//    // TODO if length > KLANG_SAMPLES_PER_AUDIO_BLOCK try to chop it down
//    if (!callback_audioblock) {
//        for (int i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
//            output[i] = 0;
//        }
//        return;
//    }
//
//    float fInputBuffers[KLANG_INPUT_CHANNELS][KLANG_SAMPLES_PER_AUDIO_BLOCK];
//    float fOutputBuffers[KLANG_OUTPUT_CHANNELS][KLANG_SAMPLES_PER_AUDIO_BLOCK];
//
//    /* unpack receive buffer to sample */
//    for (int i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
//        const uint32_t p                      = input[i];
//        const auto     mLeftInt               = (int16_t) (p & M_MASK_LEFT);
//        const auto     mRightInt              = (int16_t) (p & M_MASK_RIGHT) >> M_NUM_OF_BITS;
//        fInputBuffers[KLANG_CHANNEL_LEFT][i]  = static_cast<float>(mLeftInt) / M_INT_SCALE;
//        fInputBuffers[KLANG_CHANNEL_RIGHT][i] = static_cast<float>(mRightInt) / M_INT_SCALE;
//    }
//
//    /* calculate next audio block */
//    float* mInputBuffer[KLANG_INPUT_CHANNELS];
//    float* mOutputBuffer[KLANG_OUTPUT_CHANNELS];
//    for (size_t j = 0; j < KLANG_INPUT_CHANNELS; j++) {
//        mInputBuffer[j] = fInputBuffers[j];
//    }
//    for (size_t j = 0; j < KLANG_OUTPUT_CHANNELS; j++) {
//        mOutputBuffer[j] = fOutputBuffers[j];
//    }
//
//    callback_audioblock(mInputBuffer, mOutputBuffer, KLANG_SAMPLES_PER_AUDIO_BLOCK);
//
//    /* pack sample for transmit buffer */
//    for (int i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
//        auto mLeftInt  = (int16_t) (fOutputBuffers[KLANG_CHANNEL_LEFT][i] * M_INT_SCALE);
//        auto mRightInt = (int16_t) (fOutputBuffers[KLANG_CHANNEL_RIGHT][i] * M_INT_SCALE);
//        output[i]      = ((uint32_t) (uint16_t) mLeftInt) << 0 | ((uint32_t) (uint16_t) mRightInt) << M_NUM_OF_BITS;
//    }
//}

//void AudioCodec::callback_class_f(float** input, float** output, uint16_t length) {
//    if (!callback_audioblock) {
//        for (int j = 0; j < KLANG_OUTPUT_CHANNELS; ++j) {
//            for (int i = 0; i < length; i++) {
//                output[j][i] = 0;
//            }
//        }
//        return;
//    }
//
//    /* if (length > KLANG_SAMPLES_PER_AUDIO_BLOCK) try to chop it down */
//    if (length > KLANG_SAMPLES_PER_AUDIO_BLOCK) {
//        // TODO this scenario is not ideal and should be avoided … maybe issue a warning somewhere?
//        const uint16_t blockSize        = KLANG_SAMPLES_PER_AUDIO_BLOCK;
//        const uint8_t  mIterations      = length / blockSize;
//        const uint16_t remainingSamples = length % blockSize;
//
//        for (uint8_t i = 0; i < mIterations; i++) {
//            float* mInputSection[KLANG_INPUT_CHANNELS];
//            float* mOutputSection[KLANG_OUTPUT_CHANNELS];
//
//            for (int channel = 0; channel < KLANG_INPUT_CHANNELS; channel++) {
//                mInputSection[channel] = &input[channel][i * blockSize];
//            }
//
//            for (int channel = 0; channel < KLANG_OUTPUT_CHANNELS; channel++) {
//                mOutputSection[channel] = &output[channel][i * blockSize];
//            }
//
//            callback_audioblock(mInputSection, mOutputSection, blockSize);
//        }
//
//        /* process any remaining samples */
//        if (remainingSamples > 0) {
//            float* mInputSection[KLANG_INPUT_CHANNELS];
//            float* mOutputSection[KLANG_OUTPUT_CHANNELS];
//
//            for (int channel = 0; channel < KLANG_INPUT_CHANNELS; channel++) {
//                mInputSection[channel] = &input[channel][mIterations * blockSize];
//            }
//
//            for (int channel = 0; channel < KLANG_OUTPUT_CHANNELS; channel++) {
//                mOutputSection[channel] = &output[channel][mIterations * blockSize];
//            }
//
//            callback_audioblock(mInputSection, mOutputSection, remainingSamples);
//        }
//    } else {
//        callback_audioblock(input, output, length);
//    }
//}

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

//void AudioCodec::callback_audioblock_method(float** input_signal, float** output_signal, uint16_t length) {
//    if (callback_audioblock) {
//        callback_audioblock(input_signal, output_signal, length);
//    }
//}

std::vector<AudioCodec*> AudioCodec::instances;
bool                     AudioCodec::fRegisteredCallback = false;
#ifdef USE_MUTEX
std::mutex AudioCodec::instances_mutex;
#endif

AudioCodec::AudioCodec() : isInitialized(false),
                           fAudioInfo(nullptr),
                           mDeleteAudioInfo(false),
                           callback_audioblock(audioblock) {
    if (!fRegisteredCallback) {
        audiocodec_register_callback(AudioCodec::process_audioblock_data);
        fRegisteredCallback = true;
    }
}

AudioCodec::~AudioCodec() {
    if (mDeleteAudioInfo) {
        delete fAudioInfo;
    }
    // TODO mutex lock causes:
    // libc++abi: terminating due to uncaught exception of type std::__1::system_error: mutex lock failed: Invalid argument
    //    std::lock_guard<std::mutex> lock(instances_mutex);
    if (id < instances.size()) {
        instances[id] = nullptr;
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
