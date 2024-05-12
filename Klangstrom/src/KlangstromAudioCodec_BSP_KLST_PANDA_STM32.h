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

#pragma once

#include "KlangstromEnvironment.h"
#ifdef KLST_PANDA_STM32

#include "stm32h7xx_hal.h"

void audiocodec_TX_full_complete_callback(SAI_HandleTypeDef *hsai);
void audiocodec_TX_half_complete_callback(SAI_HandleTypeDef *hsai);
void audiocodec_RX_full_complete_callback(SAI_HandleTypeDef *hsai);
void audiocodec_RX_half_complete_callback(SAI_HandleTypeDef *hsai);
void audiocodec_error_callback(SAI_HandleTypeDef *hsai);

#endif // KLST_PANDA_STM32
