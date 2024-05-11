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

/* --- callback_interface --- */

typedef struct AudioCodec AudioCodec;
void audiocodec_callback_class(uint32_t *output, uint32_t *input, uint16_t length);
void audiocodec_register_class(AudioCodec *pClass);

/* --- callback_interface --- */

uint8_t KLST_BSP_audiocodec_setup();

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
class AudioCodec {
public:
    AudioCodec();
    void setup();
    /* --- callback_interface --- */
    void callback_class(uint32_t *output, uint32_t *input, uint16_t length);
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
private:
    bool isInitialized;
    Callback_3_FLOATPTRPTR_FLOATPTRPTR_UI16 callback_audioblock;
};
#endif // __cplusplus

