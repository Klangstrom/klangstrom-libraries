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

#ifndef INC_KLST_PANDA_AUDIOCODEC_H_
#define INC_KLST_PANDA_AUDIOCODEC_H_

#include "stdint.h"

uint8_t audiocodec_setup();
void audiocodec_TX_full_complete_callback(SAI_HandleTypeDef *hsai);
void audiocodec_TX_half_complete_callback(SAI_HandleTypeDef *hsai);
void audiocodec_RX_full_complete_callback(SAI_HandleTypeDef *hsai);
void audiocodec_RX_half_complete_callback(SAI_HandleTypeDef *hsai);
void audiocodec_error_callback(SAI_HandleTypeDef *hsai);

#endif /* INC_KLST_PANDA_AUDIOCODEC_H_ */
