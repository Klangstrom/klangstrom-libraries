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

#include "KlangstromEnvironment.h"
#ifdef KLST_ARCH_IS_EMU

#include <chrono>

#include "Console.h"
#include "stm32_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

using namespace std::chrono;

const static uint64_t mStartTimeOffset = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();

uint32_t HAL_GetTick() {
    const uint64_t mCurrentTime = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
    const uint32_t mDelta       = mCurrentTime - mStartTimeOffset;
    return static_cast<uint32_t>(mDelta);
}

uint32_t HAL_RCC_GetSysClockFreq() {
#if defined(GENERIC_EMU)
    return 0;
#elif defined(KLST_CORE_EMU)
    return 0;
#elif defined(KLST_TINY_EMU)
    return 0;
#elif defined(KLST_SHEEP_EMU)
    return 0;
#elif defined(KLST_PANDA_EMU)
    return 550000000;
#elif defined(KLST_CATERPILLAR_EMU)
    return 550000000;
#else
    return 0;
#endif
}

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_EMU
