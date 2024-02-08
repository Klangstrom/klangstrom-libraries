#include "main.h"
#include "FT5206.h"
#include "KLST_PANDA-SerialDebug.h"

static HAL_StatusTypeDef ret;
static I2C_HandleTypeDef *hi2c = 0;

static uint16_t word(uint8_t highByte, uint8_t lowByte) {
	return (uint16_t) (highByte << 8) | lowByte;
}

void FT5206_init(I2C_HandleTypeDef *hi2c_handle) {
	hi2c = hi2c_handle;

	if (!hi2c) {
		return;
	}
	print_I2C_show_devices(hi2c);

	/*
	 DEVICE READY: 0x34
	 DEVICE READY: 0x35
	 DEVICE READY: 0x70
	 DEVICE READY: 0x71
	 */

	uint8_t buf[2];
	buf[0] = FT5206_DEVICE_MODE;
	buf[1] = 0x00;
	ret = HAL_I2C_Master_Transmit(hi2c, FT5206_I2C_ADDRESS, buf, 2, HAL_MAX_DELAY);

	if (ret != HAL_OK) {
		println("transmit I2C: ERROR");
//	} else {
//		print_debug("transmit I2C4: OK");
	}

}

void FT5206_read() {
	if (!hi2c) {
		return;
	}
	uint8_t buf[FT5206_NUMBER_OF_REGISTERS];
	ret = HAL_I2C_Master_Receive(hi2c, FT5206_I2C_ADDRESS, buf, FT5206_NUMBER_OF_REGISTERS, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		println("receive I2C : ERROR");
		return;
	}

	println("gesture id   : %i", buf[FT5206_GEST_ID]);

	uint8_t nr_of_touches = buf[FT5206_TD_STATUS] & 0xF;
	println("nr_of_touches: %i", nr_of_touches);

	for (uint8_t i = 0; i < nr_of_touches; i++) {
		const uint16_t x = word(buf[FT5206_TOUCH_XH + i * 6] & 0x0f, buf[FT5206_TOUCH_XL + i * 6]);
		const uint16_t y = word(buf[FT5206_TOUCH_YH + i * 6] & 0x0f, buf[FT5206_TOUCH_YL + i * 6]);
		println("touch %X      : %i, %i", i, x, y);
	}
}

void FT5206_print_info() {
	if (!hi2c) {
		return;
	}
	uint8_t buf[FT5206_NUMBER_OF_TOTAL_REGISTERS];
	ret = HAL_I2C_Master_Receive(hi2c, FT5206_I2C_ADDRESS, buf, FT5206_NUMBER_OF_TOTAL_REGISTERS, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		println("receive I2C4 : ERROR");
		return;
	}

	println("Library version: %i.%i", buf[FS5206_TOUCH_LIB_VERSION_H], buf[FS5206_TOUCH_LIB_VERSION_L]);
}
