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

#ifdef __cplusplus
extern "C" {
#endif

typedef struct AudioInfo {
    uint32_t sample_rate     = 48000;
    uint8_t  output_channels = 2;
    uint8_t  input_channels  = 1;
    uint16_t block_size      = 128;
    uint8_t  bit_depth       = 16;
} AudioInfo;

typedef struct AudioBlock {
    uint32_t sample_rate;
    uint8_t  output_channels;
    uint8_t  input_channels;
    uint16_t block_size;
    float**  output;
    float**  input;
    uint8_t  device;
} AudioBlock;

#ifdef __cplusplus
}
#endif
