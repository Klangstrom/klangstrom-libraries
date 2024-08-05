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
static bool fIsMuted                = false;

//#ifndef CONSOLE_PRINT_BUFFER_SIZE
//#define CONSOLE_PRINT_BUFFER_SIZE 128
//#endif // CONSOLE_PRINT_BUFFER_SIZE

void console_mute(bool mute) {
    fIsMuted = mute;
}

void console_init() {
    if (!fSerialDebugInitialized) {
        fSerialDebugInitialized = true;
        console_init_BSP();
    }
}

void console_set_color_red() {
    console_printf("\033[31m");
}

void console_set_color_green() {
    console_printf("\033[32m");
}

void console_reset_color() {
    console_printf("\033[0m");
}

void console_clear() {
    console_printf("\033[2J\033[H");
}

#ifndef KLST_DISABLE_PRINT_CLIENT
void _console_printf_(const char* format, ...) {
    if (fIsMuted) {
        return;
    }
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

void _console_println_(const char* format, ...) {
    if (fIsMuted) {
        return;
    }
    console_init();
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf(CONSOLE_LINE_ENDING);

    //    char    buffer[CONSOLE_PRINT_BUFFER_SIZE];
    //    va_list args;
    //    va_start(args, format);
    //    vsnprintf(buffer, CONSOLE_PRINT_BUFFER_SIZE, format, args);
    //    va_end(args);
    //    KLST_BSP_serialdebug_printf("%s\n\r", buffer);
}

void _console_print_(const char* format, ...) {
    if (fIsMuted) {
        return;
    }
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
    //    KLST_BSP_serialdebug_printf("%s\n\r", buffer);
}
#endif // KLST_DISABLE_PRINT_CLIENT

#ifndef KLST_DISABLE_PRINT_DEBUG
void _console_status_(const char* format, ...) {
    console_set_color_green();
    console_timestamp();

    if (fIsMuted) {
        return;
    }
    console_init();
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf(CONSOLE_LINE_ENDING);

    console_reset_color();
}

void _console_error_(const char* format, ...) {
    console_set_color_red();
    console_timestamp();

    if (fIsMuted) {
        return;
    }
    console_init();
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf(CONSOLE_LINE_ENDING);

    console_reset_color();
}
#endif // KLST_DISABLE_PRINT_DEBUG

#ifdef __cplusplus
}
#endif
