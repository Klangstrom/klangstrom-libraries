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

#ifndef INC_KLST_PANDA_H_
#define INC_KLST_PANDA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "KLST_PANDA-Config.h"

#if defined(KLST_PANDA_ENABLE_USB_HOST) && defined(KLST_PANDA_ENABLE_USB_DEVICE)
#error "KLST_PANDA: USB Host and Device cannot be enabled at the same time"
#endif

#if !defined(KLST_PANDA_ENABLE_EXTERNAL_MEMORY) && defined(KLST_PANDA_ENABLE_DISPLAY)
#error "KLST_PANDA: external memory must be enabled when display is enabled"
#endif

#include "KLST.h"

#ifdef __cplusplus
}
#endif

#endif /* INC_KLST_PANDA_H_ */
