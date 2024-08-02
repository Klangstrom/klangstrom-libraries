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

#include <cstdint>

#ifndef WEAK
    #define WEAK __attribute__((weak))
#endif

const uint8_t AUDIO_DEVICE_TYPE_UNDEFINED             = 0;
const uint8_t AUDIO_DEVICE_KLST_PANDA_AUDIO_CODEC     = 1;
const uint8_t AUDIO_DEVICE_KLST_PANDA_STEREO_MIC      = 2;
const uint8_t AUDIO_DEVICE_CUSTOM                     = 127;
const uint8_t AUDIO_DEVICE_MAX_NUMBER_OF_DEVICE_TYPES = 128;
const uint8_t AUDIO_DEVICE_INIT_ERROR                 = 255;
const uint8_t AUDIO_DEVICE_ID_UNDEFINED               = 255;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct AudioInfo {
    uint32_t sample_rate     = 48000;
    uint8_t  output_channels = 2;
    uint8_t  input_channels  = 2;
    uint16_t block_size      = 128;
    uint8_t  bit_depth       = 16;
    uint8_t  device_type     = AUDIO_DEVICE_TYPE_UNDEFINED;
    uint8_t  device_id       = AUDIO_DEVICE_ID_UNDEFINED;
} AudioInfo;

typedef struct AudioBlock {
    uint32_t sample_rate;
    uint8_t  output_channels;
    uint8_t  input_channels;
    uint16_t block_size;
    float**  output;
    float**  input;
    uint8_t  device_id;
} AudioBlock;

#ifdef __cplusplus
}
#endif
