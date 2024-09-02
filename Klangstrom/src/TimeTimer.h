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

typedef enum {
    TICK_FORMAT, // default
    MICROSEC_FORMAT,
    HERTZ_FORMAT,
} TimerFormat_t;

using callback_function_t = std::function<void()>;

class TimeTimer {
public:
    void init(uint8_t instance);
    void attachInterrupt(const callback_function_t& callback_beat) { fCallback = callback_beat; }
    void setOverflow(uint32_t duration, TimerFormat_t format);
    void resume();
    void pause();
    bool isRunning() const;

    ~TimeTimer();

    static TimeTimer* instance();

protected:
    callback_function_t fCallback;
    int8_t fTimerID = -1;
};
