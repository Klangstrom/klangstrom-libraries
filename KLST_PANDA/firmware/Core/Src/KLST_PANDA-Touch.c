#include "main.h"

#include "KLST_PANDA-SerialDebug.h"
#include "KLST_PANDA-Touch.h"
#include "FT5206.h"

extern I2C_HandleTypeDef hi2c4;

void touch_setup() {
	FT5206_init(&hi2c4);
	FT5206_print_info();

	/* GPIO */
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pins : _LED_00_Pin _LED_01_Pin */
	GPIO_InitStruct.Pin = _DISPLAY_TOUCH_INTERRUPT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(_DISPLAY_TOUCH_INTERRUPT_GPIO_Port, &GPIO_InitStruct);
}

void touch_read() {
//	if (!HAL_GPIO_ReadPin(_DISPLAY_TOUCH_INTERRUPT_GPIO_Port, _DISPLAY_TOUCH_INTERRUPT_Pin)) {
//		FT5206_read();
//	}
	FT5206_read();
}
