/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
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
#include <cstddef>

/* --- callback_interface --- */

extern "C" {
static AudioCodec *mAudioCodecClass = nullptr;

void audiocodec_register_class(AudioCodec *pAudioCodecClass) {
    mAudioCodecClass = pAudioCodecClass;
}

void audiocodec_callback_class(uint32_t *output, uint32_t *input, uint16_t length) {
    if (mAudioCodecClass) {
        mAudioCodecClass->callback_class(output, input, length);
    }
}
}

// TODO clean up the code below. flexible bit depth, buffer length etcetera

static const uint8_t KLANG_OUTPUT_CHANNELS = 2;
static const uint8_t KLANG_INPUT_CHANNELS = 2;
static const uint8_t KLANG_SAMPLES_PER_AUDIO_BLOCK = 128;
static const uint8_t LEFT = 0;
static const uint8_t RIGHT = 1;
static const uint8_t M_NUM_OF_BITS = 16;
static const float M_INT_SCALE = (1 << (M_NUM_OF_BITS - 1)) - 1.0;  // @todo(see if `-1.0` is required)
static const uint32_t M_MASK_LEFT = (1 << M_NUM_OF_BITS) - 1;
static const uint32_t M_MASK_RIGHT = ~(M_MASK_LEFT);

/**
 * fills buffer with new block of samples
 */
float fOutputBuffers[KLANG_OUTPUT_CHANNELS][KLANG_SAMPLES_PER_AUDIO_BLOCK];
float fInputBuffers[KLANG_INPUT_CHANNELS][KLANG_SAMPLES_PER_AUDIO_BLOCK];
void AudioCodec::callback_class(uint32_t *output, uint32_t *input, uint16_t length) {
    if (!callback_audioblock) {
        return;
    }
    /* unpack receive buffer to sample */
    for (int i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
        const uint32_t p = input[i];
        const int16_t mLeftInt = (p & M_MASK_LEFT);
        const int16_t mRightInt = (p & M_MASK_RIGHT) >> M_NUM_OF_BITS;
        fInputBuffers[LEFT][i] = mLeftInt / M_INT_SCALE;
        fInputBuffers[RIGHT][i] = mRightInt / M_INT_SCALE;
    }
    /* calculate next audio block */
    // NOTE the code below is NOT very elegant, however it seems as if this is the only way to pass a 2D array to a function.
    // the option to pass the arrays by performing a C-style cast to a double pointer ( e.g `(float **)fOutputBuffers` ) is not
    // allowed and may produce undefined behavior or a segmantation fault.
    // it might be a better idea to use dynamic memory allocation after all …
    // TODO this only needs to be done once at startup ...
    float *mInputBuffer[KLANG_INPUT_CHANNELS];
    float *mOutputBuffer[KLANG_OUTPUT_CHANNELS];
    for (size_t j = 0; j < KLANG_INPUT_CHANNELS; j++) {
        mInputBuffer[j] = fInputBuffers[j];
    }
    for (size_t j = 0; j < KLANG_OUTPUT_CHANNELS; j++) {
        mOutputBuffer[j] = fOutputBuffers[j];
    }

    if (callback_audioblock) {
        callback_audioblock(mInputBuffer, mOutputBuffer, KLANG_SAMPLES_PER_AUDIO_BLOCK);
    }

    /* pack sample for transmit buffer */
    for (int i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
        int16_t mLeftInt = (int16_t) (fOutputBuffers[LEFT][i] * M_INT_SCALE);
        int16_t mRightInt = (int16_t) (fOutputBuffers[RIGHT][i] * M_INT_SCALE);
        output[i] = ((uint32_t) (uint16_t) mLeftInt) << 0 | ((uint32_t) (uint16_t) mRightInt) << M_NUM_OF_BITS;
        // //@todo this assumes a 16bit sample depth!
        // int16_t mL = (int16_t) (fOutputBuffers[LEFT][i] * 32767.0f);
        // int16_t mR = (int16_t) (fOutputBuffers[RIGHT][i] * 32767.0f);
        // pTXBuffer[i] = ((uint32_t) (uint16_t) mL) << 0 | ((uint32_t) (uint16_t) mR) << 16;
    }
}

/* --- callback_interface --- */

AudioCodec::AudioCodec() :
        isInitialized(false) {
    audiocodec_register_class(this);
}

void AudioCodec::setup() {
    if (!isInitialized) {
        KLST_BSP_audiocodec_setup();
        isInitialized = true;
    } else {
        // TODO print warning
    }
}
