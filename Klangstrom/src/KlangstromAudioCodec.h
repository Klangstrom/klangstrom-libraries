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

#pragma once

#include <stdint.h>

#include "KlangstromCallbackDefinitions.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t KLST_BSP_audiocodec_init();

/* --- callback_interface --- */

typedef struct AudioCodec AudioCodec;

void audiocodec_callback_class_i(uint32_t* input, uint32_t* output, uint16_t length);
void audiocodec_callback_class_f(float** input, float** output, uint16_t length);
void audiocodec_register_audio_device_c(AudioCodec* pClass);

/* --- callback_interface --- */

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include <iostream>
#include <functional>

void audioblock(float** input_signal, float** output_signal, uint16_t length);

class AudioCodec {
public:
    AudioCodec();
    void init();
    /* --- callback_interface --- */
    void callback_class(uint32_t* input, uint32_t* output, uint16_t length);
    void callback_class_f(float** input, float** output, uint16_t length);
    /* --- callback_interface --- */
    void register_audioblock(Callback_3_FLOATPTRPTR_FLOATPTRPTR_UI16 callback) {
        callback_audioblock = callback;
    }
    void start() {
        // TODO implement
    }
    void pause() {
        // TODO implement
    }
    void callback_audioblock_method(float** input_signal, float** output_signal, uint16_t length);

private:
    bool                                       isInitialized;
    Callback_3_FLOATPTRPTR_FLOATPTRPTR_UI16    callback_audioblock;
    std::function<void(float**, float**, int)> mBoundCallback;
};
#endif // __cplusplus
