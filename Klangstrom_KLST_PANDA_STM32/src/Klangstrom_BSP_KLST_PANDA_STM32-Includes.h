/*
 * Klangstrom
 *
 * This file is part of the *Klangstrom* library (https://github.com/dennisppaul/klangstrom-libraries).
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

#include "Klangstrom_ASP_KLST_STM32-Config.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(KLST_PANDA_ENABLE_GPIO) || defined(KLST_PANDA_ENABLE_MECHANICAL_KEYS) || defined(KLST_PANDA_ENABLE_SD_CARD) || defined(KLST_PANDA_ENABLE_DISPLAY)
#include "gpio.h"
#endif

#if defined(KLST_PANDA_ENABLE_MECHANICAL_KEYS) || defined(KLST_PANDA_ENABLE_ENCODER) || defined(KLST_PANDA_ENABLE_DISPLAY)
#include "tim.h"
#endif

#if defined(KLST_PANDA_ENABLE_SERIAL_DEBUG) || defined(KLST_PANDA_ENABLE_IDC_SERIAL) || defined(KLST_PANDA_ENABLE_MIDI)
#include "usart.h"
#endif

#if defined(KLST_PANDA_ENABLE_EXTERNAL_MEMORY)
#include "octospi.h"
#endif

#if defined(KLST_PANDA_ENABLE_AUDIOCODEC) || defined(KLST_PANDA_ENABLE_IDC_SERIAL) || defined(KLST_PANDA_ENABLE_MIDI)
#include "dma.h"
#endif

#if defined(KLST_PANDA_ENABLE_AUDIOCODEC) || defined(KLST_PANDA_ENABLE_DISPLAY)
#include "i2c.h"
#endif

#if defined(KLST_PANDA_ENABLE_AUDIOCODEC) || defined(KLST_PANDA_ENABLE_ON_BOARD_MIC)
#include "sai.h"
#endif

#ifdef KLST_PANDA_ENABLE_ON_BOARD_MIC
#include "pdm2pcm.h"
#include "crc.h"
#endif

#ifdef KLST_PANDA_ENABLE_SD_CARD
#include "sdmmc.h"
#include "fatfs.h"
#endif

#ifdef KLST_PANDA_ENABLE_ADC_DAC
#include "adc.h"
#include "dac.h"
#endif

#ifdef KLST_PANDA_ENABLE_DISPLAY
#include "dma2D.h"
#include "ltdc.h"
#endif

#ifdef KLST_PANDA_ENABLE_USB_HOST
#include "usb_host.h"
#endif

#ifdef KLST_PANDA_ENABLE_USB_DEVICE
#include "usb_device.h"
#endif

#ifdef __cplusplus
}
#endif

#endif // KLST_PANDA_STM32
