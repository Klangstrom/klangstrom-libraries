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
#ifdef KLST_PANDA_STM32

#include <inttypes.h>

#include "usart.h"
#include "Console.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef KLST_DISABLE_PRINT_CLIENT

#ifndef CONSOLE_UART_INSTANCE
#define CONSOLE_UART_INSTANCE huart3
#endif // CONSOLE_UART_INSTANCE

#ifndef CONSOLE_UART_INSTANCE_INIT
#define CONSOLE_UART_INSTANCE_INIT MX_USART3_UART_Init
#endif // CONSOLE_UART_INSTANCE_INIT

extern UART_HandleTypeDef CONSOLE_UART_INSTANCE;

extern "C" int _write(int file, char* data, int len) {
    // @note needs to be implemented with `extern "C"` for printf to work
    (void) file;
    HAL_StatusTypeDef status = HAL_UART_Transmit(&CONSOLE_UART_INSTANCE, (uint8_t*) data, len, HAL_MAX_DELAY);
    return (status == HAL_OK ? len : 0);
}

void console_init_BSP() {
    CONSOLE_UART_INSTANCE_INIT();
}
#else
void console_init_BSP() {}
#endif // KLST_DISABLE_PRINT_CLIENT

static void rainbow(const char* text) {
    static const uint8_t num_colors = 7;
    const char*          colors[]   = {
        "\033[31m", // Red
        "\033[32m", // Green
        "\033[33m", // Yellow
        "\033[34m", // Blue (as Indigo)
        "\033[35m", // Violet
        "\033[36m", // Cyan (as another shade of Violet)
        "\033[37m"  // White (as another shade of Violet)
    };

    const char* reset_color = "\033[0m";
    static int  color_index = 0;

    console_status("%s%s%s", colors[color_index], text, reset_color);
    color_index++;
    color_index %= num_colors;
}

void console_system_info() {
    console_reset_color();
    const static bool mPrintImage = true;

    console_status("----------------------------------------------------------------------------------------------------");
    console_status("KLST_PANDA(STM32H723ZGT) @ %liMHz (%s)", HAL_RCC_GetSysClockFreq() / 1000000, __TIME__);
    console_status("----------------------------------------------------------------------------------------------------");
    if (mPrintImage) {
        console_status("                                                                                                    ");
        console_status("                        ▓▓████▓▓▒▒                                                                  ");
        console_status("                      ░░██████████                                                                  ");
        console_status("                      ██████████▓▓░░                                                                ");
        console_status("                      ████████████░░                                                                ");
        console_status("                      ████████████▒▒                                ▓▓▓▓██                          ");
        console_status("                      ██████████▓▓░░                              ▓▓██▓▓████                        ");
        console_status("                      ████████▓▓░░                            ░░████████████▒▒                      ");
        console_status("                      ▓▓████▒▒░░░░                              ▓▓████████████                      ");
        console_status("                      ▒▒██▒▒                  ░░░░  ░░            ████████████                      ");
        console_status("                        ░░░░              ░░░░    ░░░░░░░░░░░░░░  ░░██████████                      ");
        console_status("                        ░░        ░░░░  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░▓▓████████                      ");
        console_status("                                ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░██████▓▓                      ");
        console_status("                                ░░░░████▓▓░░░░░░░░░░░░░░░░░░░░░░░░░░░░██████                        ");
        console_status("                              ░░░░██████████░░░░░░░░░░░░░░░░░░░░░░░░░░░░                            ");
        console_status("                        ░░░░░░░░▒▒████▓▓████▒▒░░░░░░░░▒▒████░░░░░░░░░░░░                            ");
        console_status("                      ░░░░░░░░░░▓▓████▓▓██▓▓░░  ░░░░░░██████▓▓░░░░░░░░░░                            ");
        console_status("                      ░░░░░░░░░░████████░░          ░░████████░░░░░░░░░░                            ");
        console_status("                      ░░░░░░░░░░████▓▓                ██▓▓████░░░░░░░░░░░░                          ");
        console_status("                      ░░░░░░░░░░████                    ████▓▓░░░░░░░░░░                            ");
        console_status("                      ░░░░░░░░░░░░░░                    ▒▒██▒▒░░░░░░░░░░                            ");
        console_status("                      ░░░░░░░░░░░░        ░░██████▒▒      ██░░░░░░░░░░░░                            ");
        console_status("                      ░░░░░░░░░░░░░░  ░░░░▒▒▓▓████████░░░░▓▓░░░░░░░░░░                              ");
        console_status("                    ░░██░░░░░░░░░░░░░░░░░░░░██████████░░░░░░░░░░░░░░░░                              ");
        console_status("                  ░░████▓▓░░░░░░▒▒░░░░░░░░░░▒▒██████▒▒░░░░░░░░░░░░░░                                ");
        console_status("                  ▓▓██████▓▓▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░                              ");
        console_status("              ▒▒▓▓██████████████▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░▒▒████░░                            ");
        console_status("            ▓▓██████████████████████▓▓▓▓▓▓▓▓▒▒░░░░░░░░░░▒▒▓▓▓▓██████████▓▓▒▒                        ");
        console_status("          ░░▓▓████████████████████████████████████▓▓▒▒▒▒████████████████████                        ");
        console_status("          ██▓▓████████████████████████████████████████████████████████████████                      ");
        console_status("        ████▓▓████████████████████████████████████████████████████████████████▒▒                    ");
        console_status("        ▓▓▓▓████████████████████████████████████████████████████████████████████                    ");
        console_status("      ▓▓████████████████▓▓░░░░░░▒▒░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓██████████████████████▓▓                  ");
        console_status("      ██████████████████▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒██████████████████                  ");
        console_status("      ▓▓████████████████▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒██████████████████░░                ");
        console_status("    ▒▒▓▓████████████████▓▓░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒██████████████████▓▓                ");
        console_status("    ████████████████████▓▓░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒████████████████████                ");
        console_status("    ▓▓▓▓██████████████████░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒██████████████████▓▓                ");
        console_status("    ▓▓████████████████████▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒▓▓██████████████████▓▓                ");
        console_status("    ████████████████████████░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒██████████████████████                ");
        console_status("    ████████████████████████░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒██████████████████████                ");
        console_status("      ████████████████████▓▓░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒██████████████████████                ");
        console_status("      ████████████████████▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░██████████████████████████████▒▒        ");
        console_status("      ██████████████████▓▓▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▓▓████████████████████████████████▒▒    ");
        console_status("    ▒▒████████████████████▓▓▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▓▓████████████████████████████████▓▓▓▓  ");
        console_status("    ████████████████████████▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒████▓▓██████████████████████████████░░");
        console_status("  ▒▒████████████████████████▓▓▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░▓▓██████████████████████████████████████");
        console_status("  ████████████████████████████▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒████████████████████████████████████████");
        console_status("  ████████████████████████████▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░▒▒████████████████████████████████▓▓██████");
        console_status("  ████████████████████████████▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒██████████████████████████████████▓▓████");
        console_status("  ████████████████████████████▒▒▒▒░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒████████████████████████████▓▓▓▓▓▓▓▓██▓▓");
        console_status("░░▓▓██████████████████████████▒▒▒▒░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒██████████████████████████▓▓▓▓▓▓▓▓▓▓██  ");
        console_status("  ████████████████▓▓▓▓████████▒▒▒▒▒▒▒▒░░░░░░░░░░░░▒▒▒▒░░      ▒▒██████████████████████▓▓████████▓▓  ");
        console_status("  ████████████████▓▓██████████  ░░▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░            ████████████████████▓▓████████▒▒    ");
        console_status("  ██████████████████████████▓▓                                    ██████████████████▓▓▓▓▓▓██▓▓      ");
        console_status("    ████████████████▓▓██████                                        ██████████████████████          ");
        console_status("    ██████████████████████▒▒                                                                        %s", KLST_CONSOLE_LINE_ENDING);
    } else {
        console_status("%s", KLST_CONSOLE_LINE_ENDING);
    }

    //    for (int i = 0; i < 7; ++i) {
    //        rainbow("■□■ □■□□ □□□ ■ □□■■□■ □■■□ □■ ■□ ■□□ □■");
    //    }
}

#ifdef __cplusplus
}
#endif

#endif // KLST_PANDA_STM32
