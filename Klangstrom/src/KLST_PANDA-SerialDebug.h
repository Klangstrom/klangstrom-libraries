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
