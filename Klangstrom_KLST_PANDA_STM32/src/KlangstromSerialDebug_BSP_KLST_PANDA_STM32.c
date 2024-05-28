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
#ifdef KLST_PANDA_ENABLE_SERIAL_DEBUG

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <inttypes.h>

#include "stm32h7xx_hal.h"
#include "usart.h"

#include "KlangstromSerialDebug.h"

extern UART_HandleTypeDef huart3; // TODO maybe pass this as a parameter

int _write(int file, char *data, int len) {
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart3, (uint8_t*) data, len, 10);
    return (status == HAL_OK ? len : 0);
}

void KLST_BSP_serialdebug_init() {
    MX_USART3_UART_Init();
}

void KLST_BSP_serialdebug_println(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\r\n");
}

void KLST_BSP_serialdebug_info() {
    printf("\r\n---------------------------------------------------------\r\n\r\n");
    printf("KLST_PANDA-STM32H723ZGT-BSP @ %liMHz (%s)\r\n", HAL_RCC_GetSysClockFreq() / 1000000, __TIME__);
    printf("\r\n---------------------------------------------------------\r\n\r\n");
}

void KLST_BSP_serialdebug_timestamp() {
    printf("[%010" PRIu32 "] ", (uint32_t) HAL_GetTick());
}

void KLST_BSP_serialdebug_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void console_binary(uint32_t value, uint8_t number_of_bits) {
    for (int i = (number_of_bits - 1); i >= 0; i--) {
        KLST_BSP_serialdebug_printf("%d", (value >> i) & 1);
    }
}

#endif // KLST_PANDA_ENABLE_SERIAL_DEBUG
#endif // KLST_PANDA_STM32

