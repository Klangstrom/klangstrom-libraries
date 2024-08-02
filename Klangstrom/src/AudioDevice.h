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

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WEAK
#define WEAK __attribute__((weak))
#endif

const uint8_t AUDIO_DEVICE_TYPE_UNDEFINED             = 0;
const uint8_t AUDIO_DEVICE_KLST_PANDA_AUDIO_CODEC     = 1;
const uint8_t AUDIO_DEVICE_KLST_PANDA_STEREO_MIC      = 2;
const uint8_t AUDIO_DEVICE_CUSTOM                     = 127;
const uint8_t AUDIO_DEVICE_MAX_NUMBER_OF_DEVICE_TYPES = 128;
const uint8_t AUDIO_DEVICE_INIT_ERROR                 = 254;
const uint8_t AUDIO_DEVICE_ID_UNDEFINED               = 255;

static const uint8_t CALLBACK_TX_ERROR         = 0;
static const uint8_t CALLBACK_TX_COMPLETE      = 1;
static const uint8_t CALLBACK_TX_HALF_COMPLETE = 2;
static const uint8_t CALLBACK_RX_ERROR         = 3;
static const uint8_t CALLBACK_RX_COMPLETE      = 4;
static const uint8_t CALLBACK_RX_HALF_COMPLETE = 5;

typedef struct AudioInfo {
    uint32_t sample_rate;
    uint8_t  output_channels;
    uint8_t  input_channels;
    uint16_t block_size;
    uint8_t  bit_depth;
    uint8_t  device_type;
    uint8_t  device_id;
    //    uint32_t sample_rate     = 48000;
    //    uint8_t  output_channels = 2;
    //    uint8_t  input_channels  = 2;
    //    uint16_t block_size      = 128;
    //    uint8_t  bit_depth       = 16;
    //    uint8_t  device_type     = AUDIO_DEVICE_TYPE_UNDEFINED;
    //    uint8_t  device_id       = AUDIO_DEVICE_ID_UNDEFINED;
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

struct AudioDevicePeripherals; /* BSP or ASP implementation */

typedef void (*Callback_1_AUDIOBLOCKPTR)(AudioBlock*);

typedef struct AudioDevice {
    AudioInfo*               audioinfo;
    AudioBlock*              audioblock;
    struct AudioDevicePeripherals*  peripherals;
    Callback_1_AUDIOBLOCKPTR callback_audioblock;
} AudioDevice;

typedef void (*Callback_2_AUDIODEVICE_UI8)(AudioDevice*, uint8_t);

/**
 * callback to be implemented by client application
 * @param audioblock
 */
WEAK void audioblock(AudioBlock* audioblock);

/**
 * initializes audio device with info as specified in `audioinfo`.
 * note, that `audioinfo` is copied.
 * after initialization `audioinfo.device_id` will contain the ID of the device.
 * if intialization fails, `audioinfo.device_id` will be set to `AUDIO_DEVICE_INIT_ERROR`.
 * @param audioinfo
 * @return
 */
AudioDevice* audiodevice_init(AudioInfo* audioinfo);
/**
 *  initializes audio device with a `audiodevice`.
 *  this is only used for custom instialization e.g when specifiying custom callbacks
 * @param audiodevice
 */
void audiodevice_init_device(AudioDevice* audiodevice);
void audiodevice_deinit(AudioDevice* audiodevice);
void audiodevice_start(AudioDevice* audiodevice);
void audiodevice_stop(AudioDevice* audiodevice);
void audiodevice_init_peripherals(AudioDevice* audiodevice);
void audiodevice_deinit_peripherals(AudioDevice* audiodevice);
void audiodevice_set_peripheral_callbacks(AudioDevice*               audiodevice,
                                          Callback_2_AUDIODEVICE_UI8 callback_rx,
                                          Callback_2_AUDIODEVICE_UI8 callback_tx,
                                          Callback_2_AUDIODEVICE_UI8 callback_error);

void audiodevice_init_device_BSP(AudioDevice* audiodevice);
void audiodevice_deinit_BSP(AudioDevice* audiodevice);

#ifdef __cplusplus
}
#endif
