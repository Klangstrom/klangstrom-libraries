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

void AudioCodec::callback_class(uint32_t *output, uint32_t *input, uint16_t length) {
    // TODO trigger registered application callback e.g `audioblock()`
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
