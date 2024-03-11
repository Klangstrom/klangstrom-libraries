#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <inttypes.h>

#include "KLST_PANDA-SerialDebug.h"

extern UART_HandleTypeDef huart3;

int _write(int file, char *data, int len) {
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart3, (uint8_t*) data, len, 10);
    return (status == HAL_OK ? len : 0);
}

void serialdebug_setup() {
    printf("\r\n---------------------------------------------------------\r\n\r\n");
    printf("KLST_PANDA-STM32H723ZGT-BSP @ %liMHz (%s)\r\n", HAL_RCC_GetSysClockFreq() / 1000000, __TIME__);
    printf("\r\n---------------------------------------------------------\r\n\r\n");
}

void print(const char *format, ...) {
    printf("[%010" PRIu32 "] ", (uint32_t) HAL_GetTick());
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void println(const char *format, ...) {
    print(format);
    printf("\r\n");
}

void print_I2C_show_devices(I2C_HandleTypeDef *hi2c) {
    for (uint8_t i = 0; i < 255; i++) {
        HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(hi2c, i, 100, 1000);
//        if (hi2c->ErrorCode & HAL_I2C_ERROR_TIMEOUT) {
//            println("- I2C DEVICE TIMED OUT: 0x%02X", i);
//        }
        if (!status) {
            println("- I2C DEVICE READY: 0x%02X", i);
        }
    }
}

void print_binary8ui(uint8_t value) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
    printf("\r\n");
}

void print_binary16ui(uint16_t value) {
    for (int i = 15; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
    printf("\r\n");
}

void print_binary32ui(uint32_t value) {
    for (int i = 31; i >= 0; i--) {
        printf("%li", (value >> i) & 1);
    }
    printf("\r\n");
}
