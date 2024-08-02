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

#include <stdio.h>
#include <stdarg.h>

#include "Console.h"

#ifdef __cplusplus
extern "C" {
#endif

static bool fSerialDebugInitialized = false;

//#ifndef CONSOLE_PRINT_BUFFER_SIZE
//#define CONSOLE_PRINT_BUFFER_SIZE 128
//#endif // CONSOLE_PRINT_BUFFER_SIZE

void console_init() {
    if (!fSerialDebugInitialized) {
        fSerialDebugInitialized = true;
        console_init_BSP();
    }
}

void console_printf(const char* format, ...) {
    console_init();
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    //    char    buffer[CONSOLE_PRINT_BUFFER_SIZE];
    //    va_list args;
    //    va_start(args, format);
    //    vsnprintf(buffer, CONSOLE_PRINT_BUFFER_SIZE, format, args);
    //    va_end(args);
    //    KLST_BSP_serialdebug_printf("%s", buffer);
}

void console_println(const char* format, ...) {
    console_init();
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n\r");

    //    char    buffer[CONSOLE_PRINT_BUFFER_SIZE];
    //    va_list args;
    //    va_start(args, format);
    //    vsnprintf(buffer, CONSOLE_PRINT_BUFFER_SIZE, format, args);
    //    va_end(args);
    //    KLST_BSP_serialdebug_printf("%s\n\r", buffer);
}

#ifdef __cplusplus
}
#endif
