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

#include "KlangstromEnvironment.h"
#ifdef KLST_ARCH_IS_EMU
#define USE_MUTEX
#endif

#include <iostream>
#include <functional>
#ifdef USE_MUTEX
#include <mutex>
#endif

#include "KlangstromCallbackDefinitions.h"
#include "KlangstromAudio.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * initializes audio codec. this function is required to interpret `AudioInfo`
 * and start the underlying audio hardware.
 * @param audioinfo configuration for audio codec
 * @return ID of audio codec
 */
uint8_t KLST_BSP_audiocodec_init(AudioInfo* audioinfo);
uint8_t KLST_BSP_audiocodec_deinit(); // TODO implement
uint8_t KLST_BSP_audiocodec_start();  // TODO implement
uint8_t KLST_BSP_audiocodec_stop();   // TODO implement

/* --- callback_interface --- */

/**
 * register callback to `AudioCodec::process_audioblock_data` for BSP.
 * this function is called the first time an audio codec is instantiated.
 * TODO will there also be an integer version to reduce overhead on MCU?
 */
void audiocodec_register_callback(Callback_1_AUDIOBLOCKPTR);

/**
 * called by Low Level BSP when new audio data is requested or provided.
 * @param audio_block
 */
void KLST_BSP_audiocodec_process_audioblock_data(AudioBlock* audio_block);

/* --- callback_interface --- */

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/**
 * weak callback for client application ( i.e sketch ) to implement
 * @param audio_block
 */
WEAK void audioblock(AudioBlock* audio_block);

class AudioCodec {
public:
    AudioCodec();
    ~AudioCodec();

    // TODO maybe drop this first init method
    uint8_t init(uint32_t sample_rate     = 48000,
                 uint8_t  output_channels = 2,
                 uint8_t  input_channels  = 1,
                 uint16_t block_size      = 128,
                 uint8_t  bit_depth       = 16,
                 uint8_t  device_type     = AUDIO_DEVICE_UNDEFINED);
    uint8_t init(AudioInfo* audioinfo = nullptr);
    void    BSP_init(AudioInfo* audioinfo = nullptr);

    /* --- callback_interface --- */

    /**
     * static callback for all audio codecs ( or devices ). the ID specified in
     * `audio_block` is used to determine the device and will forward the call
     * to it.
     * @param audio_block
     */
    static void process_audioblock_data(AudioBlock* audio_block);

    /* --- callback_interface --- */

    void start() {
        // TODO implement KLST_BSP_audiocodec_start
    }
    void stop() {
        // TODO implement KLST_BSP_audiocodec_stop
    }

    void set_callback(Callback_1_AUDIOBLOCKPTR callback) {
        callback_audioblock = callback;
    }

private:
    static std::vector<AudioCodec*> instances;
#ifdef USE_MUTEX
    static std::mutex instances_mutex;
#endif
    static bool fRegisteredCallback;

    uint8_t                  id;
    bool                     isInitialized;
    AudioInfo*               fAudioInfo;
    bool                     mDeleteAudioInfo;
    Callback_1_AUDIOBLOCKPTR callback_audioblock;
};
#endif // __cplusplus
