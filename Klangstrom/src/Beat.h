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


#include <functional>
#include <stdint.h>

#include "HardwareTimer.h"

#ifndef WEAK
#define WEAK __attribute__((weak))
#endif

#ifdef __cplusplus
extern "C" {
#endif

WEAK void beat(uint8_t beat_id, uint16_t beat_counter);

#ifdef __cplusplus
}
#endif

typedef void (*Callback_2_UI8_UI16)(uint8_t, uint16_t);

class Beat {
public:
    explicit Beat(const uint8_t beat_id) : device_id(beat_id),
                                           callback_beat(nullptr),
                                           beat_counter(0) {
        timer = new HardwareTimer(TIM5); // TODO this needs to be configured elsewhere
                                         //     ... maybe as constructor parameter
        timer->attachInterrupt(std::bind(&Beat::beat_event, this));
        set_callback(beat);
    }

    void bpm(const float beats_per_minute) const {
        if (beats_per_minute == 0) {
            return;
        }
        const uint32_t duration_us = (60.0f / beats_per_minute) * 1000000;
        timer->setOverflow(duration_us, MICROSEC_FORMAT);
        timer->resume();
    }

    void beat_event() {
        beat_counter++;
        if (callback_beat != nullptr) {
            callback_beat(device_id, beat_counter);
        }
    }
    void pause() const {
        timer->pause();
    }

    void start() {
        reset();
        resume();
    }

    void resume() const {
        timer->resume();
    }

    void reset() {
        beat_counter = 0;
    }

    void set_callback(const Callback_2_UI8_UI16& callback) {
        callback_beat = callback;
    }

private:
    HardwareTimer*      timer;
    const uint8_t       device_id;
    Callback_2_UI8_UI16 callback_beat;
    uint32_t            beat_counter;
};
