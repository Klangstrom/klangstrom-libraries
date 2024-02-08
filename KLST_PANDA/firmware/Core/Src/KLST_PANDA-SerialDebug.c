#include "KLST_PANDA-SerialDebug.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <inttypes.h>

extern UART_HandleTypeDef huart3;

int _write(int file, char *data, int len) {
	HAL_StatusTypeDef status = HAL_UART_Transmit(&huart3, (uint8_t*) data, len, 0xFFFF);
	return (status == HAL_OK ? len : 0);
}

void serialdebug_setup() {
	printf("\r\n---------------------------------------------------------\r\n");
	printf("\r\nKLST_PANDA-STM32H723ZGT-BSP (%s)\r\n\r\n", __TIME__);
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
		if (!HAL_I2C_IsDeviceReady(hi2c, i, 100, HAL_MAX_DELAY)) {
			println("DEVICE READY: 0x%X", i);
		}
	}
}
