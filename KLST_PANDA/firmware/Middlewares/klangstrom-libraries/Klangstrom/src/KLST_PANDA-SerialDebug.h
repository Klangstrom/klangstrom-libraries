#ifndef INC_KLST_PANDA_SERIALDEBUG_H_
#define INC_KLST_PANDA_SERIALDEBUG_H_

#include "stm32h7xx_hal.h"

void serialdebug_setup();
void serialdebug_loop();
void print(const char *format, ...);
void println(const char *format, ...);
void print_I2C_show_devices(I2C_HandleTypeDef *hi2c);
void print_binary(uint16_t value);
void print_binary8ui(uint8_t value);
void print_binary16ui(uint16_t value);
void print_binary32ui(uint32_t value);

#endif /* INC_KLST_PANDA_SERIALDEBUG_H_ */
