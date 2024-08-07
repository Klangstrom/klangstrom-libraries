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
#include "SDCard.h"

class AudioStreamFloat32 {
public:
    AudioStreamFloat32(uint32_t buffer_size) : fBufferSize(buffer_size) {
        fReadBuffer = new float[buffer_size];
        clear_buffer(fReadBuffer, buffer_size);
    }

    void update() {
        if (sdcard_file_eof() && fLoop) {
            sdcard_file_seek(0);
        }
        uint32_t mBytesRead = sdcard_file_read((uint8_t*) fReadBuffer, fBufferSize * sizeof(float));
        if (mBytesRead < fBufferSize * sizeof(float)) {
            clear_buffer(fReadBuffer + mBytesRead / sizeof(float), fBufferSize - mBytesRead / sizeof(float));
        }
        // if (mBytesRead == 0) {}
    }

    float* buffer() const {
        return fReadBuffer;
    }

    void set_loop(bool loop) {
        fLoop = loop;
    }

private:
    float*         fReadBuffer;
    const uint32_t fBufferSize;
    bool           fLoop = true;

    static void clear_buffer(float* buffer, int size) {
        memset(buffer, 0, sizeof(float) * size);
    }
};