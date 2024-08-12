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

#include "KlangstromEnvironment.h"

#if defined(KLST_ARCH_IS_STM32)
#define KLST_CONSOLE_LINE_ENDING "\r\n" // used in `Console.h`
#elif defined(KLST_ARCH_IS_EMU)
#define KLST_CONSOLE_LINE_ENDING "\n\r" // used in `Console.h` // TODO why is this reversed in CLI terminal
#endif
//#define KLST_DISABLE_PRINT_CLIENT        // used in `Console.h`
#define KLST_DISABLE_SDCARD_PRINT_CLIENT // used in `SCCard_BSP_STM32.cpp`

// TODO add the other defines that are used in codebase
