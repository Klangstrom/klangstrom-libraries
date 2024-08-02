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
    HAL_StatusTypeDef status = HAL_UART_Transmit(&CONSOLE_UART_INSTANCE, (uint8_t*) data, len, 10);
    return (status == HAL_OK ? len : 0);
}

void console_init_BSP() {
    CONSOLE_UART_INSTANCE_INIT();
}

void console_system_info() {
    console_printf("\n\r---------------------------------------------------------\n\r\n\r");
    console_printf("KLST_PANDA(STM32H723ZGT) @ %liMHz (%s)\n\r", HAL_RCC_GetSysClockFreq() / 1000000, __TIME__);
    console_printf("\n\r---------------------------------------------------------\n\r\n\r");
}

#ifdef __cplusplus
}
#endif

#endif // KLST_PANDA_STM32
