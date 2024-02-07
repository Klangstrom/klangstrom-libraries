#include "main.h"
#include "KLST_PANDA-SerialDebug.h"

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


void print_debug(char *text) {
	printf("[%010" PRIu32 "] %s\r\n", (uint32_t) HAL_GetTick(), text);
}

void println(char *text) {
	printf("%s\r\n", text);
}
