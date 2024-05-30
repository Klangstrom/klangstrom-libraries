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

#include "KlangstromEnvironment.h"
#ifdef KLST_PANDA_STM32
#include "Klangstrom_BSP_KLST_PANDA_STM32-Config.h"
#ifdef KLST_PANDA_ENABLE_AUDIOCODEC

#ifdef __cplusplus

#include <iostream>
#include <functional>

void register_audio_device(const std::function<void(float**, float**, int)> callback) {
}

#endif // __cplusplus
#endif // KLST_PANDA_ENABLE_AUDIOCODEC
#endif // KLST_PANDA_STM32
