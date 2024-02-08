#ifndef INC_KLST_PANDA_SERIALDEBUG_H_
#define INC_KLST_PANDA_SERIALDEBUG_H_

#include "main.h"

void serialdebug_setup();
void serialdebug_loop();
void print(const char *format, ...);
void println(const char *format, ...);
void print_I2C_show_devices(I2C_HandleTypeDef *hi2c);

#endif /* INC_KLST_PANDA_SERIALDEBUG_H_ */
