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
#ifdef KLST_PANDA_STM32

#include "stm32h7xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "Klangstrom_ASP_KLST_STM32-Config.h"

#if defined(KLST_PANDA_ENABLE_USB_HOST) && defined(KLST_PANDA_ENABLE_USB_DEVICE)
#error "KLST_PANDA: USB Host and Device cannot be enabled at the same time"
#endif

#if !defined(KLST_PANDA_ENABLE_EXTERNAL_MEMORY) && defined(KLST_PANDA_ENABLE_DISPLAY)
#error "KLST_PANDA: external memory must be enabled when display is enabled"
#endif

#include "Klangstrom.h"



#ifdef __cplusplus
}
#endif

#endif // KLST_PANDA_STM32
