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

#if __has_include("KlangstromPeripheralConfiguration.h")

#warning "Klangstrom peripherals are configured via external file"
#include "KlangstromPeripheralConfiguration.h"

#else

#define KLST_PANDA_ENABLE_GPIO
#define KLST_PANDA_ENABLE_SERIAL_DEBUG
#define KLST_PANDA_ENABLE_AUDIOCODEC
#define KLST_PANDA_ENABLE_LEDS
//#define KLST_PANDA_ENABLE_EXTERNAL_MEMORY
//#define KLST_PANDA_ENABLE_ON_BOARD_MIC
//#define KLST_PANDA_ENABLE_ENCODER
#define KLST_PANDA_ENABLE_SD_CARD
//#define KLST_PANDA_ENABLE_IDC_SERIAL
//#define KLST_PANDA_ENABLE_MIDI
//#define KLST_PANDA_ENABLE_MECHANICAL_KEYS
//#define KLST_PANDA_ENABLE_USB_HOST
//#define KLST_PANDA_ENABLE_USB_DEVICE
//#define KLST_PANDA_ENABLE_ADC_DAC
//#define KLST_PANDA_ENABLE_DISPLAY

#endif

/*
 #define KLST_CONFIG_MODE KLST_CONFIG_MODE_DEFAULT

 #define KLST_CONFIG_MODE_HEADER_FILE 0
 #define KLST_CONFIG_MODE_APPLICATION_DEFINES 1
 #define KLST_CONFIG_MODE_DEFAULT 2

 #define KLST_DEBUG_CONFIG_MODE

 #if __has_include("KLST-ApplicationConfig.h")
 #define KLST_CONFIG_MODE KLST_CONFIG_MODE_HEADER_FILE
 #elif \
    defined(KLST_PANDA_ENABLE_GPIO) || \
    defined(KLST_PANDA_ENABLE_SERIAL_DEBUG) || \
    defined(KLST_PANDA_ENABLE_EXTERNAL_MEMORY) || \
    defined(KLST_PANDA_ENABLE_AUDIOCODEC) || \
    defined(KLST_PANDA_ENABLE_ON_BOARD_MIC) || \
    defined(KLST_PANDA_ENABLE_ENCODER) || \
    defined(KLST_PANDA_ENABLE_SD_CARD) || \
    defined(KLST_PANDA_ENABLE_IDC_SERIAL) || \
    defined(KLST_PANDA_ENABLE_MIDI) || \
    defined(KLST_PANDA_ENABLE_MECHANICAL_KEYS) || \
    defined(KLST_PANDA_ENABLE_USB_HOST) || \
    defined(KLST_PANDA_ENABLE_USB_DEVICE) || \
    defined(KLST_PANDA_ENABLE_ADC_DAC) || \
    defined(KLST_PANDA_ENABLE_DISPLAY)
 #define KLST_CONFIG_MODE KLST_CONFIG_MODE_APPLICATION_DEFINES
 #else
 #define KLST_CONFIG_MODE KLST_CONFIG_MODE_DEFAULT
 #define KLST_PANDA_ENABLE_GPIO
 #define KLST_PANDA_ENABLE_SERIAL_DEBUG
 #define KLST_PANDA_ENABLE_EXTERNAL_MEMORY
 #define KLST_PANDA_ENABLE_AUDIOCODEC
 //#define KLST_PANDA_ENABLE_ON_BOARD_MIC
 //#define KLST_PANDA_ENABLE_ENCODER
 //#define KLST_PANDA_ENABLE_SD_CARD
 //#define KLST_PANDA_ENABLE_IDC_SERIAL
 //#define KLST_PANDA_ENABLE_MIDI
 //#define KLST_PANDA_ENABLE_MECHANICAL_KEYS
 //#define KLST_PANDA_ENABLE_USB_HOST
 //#define KLST_PANDA_ENABLE_USB_DEVICE
 //#define KLST_PANDA_ENABLE_ADC_DAC
 #define KLST_PANDA_ENABLE_DISPLAY
 #endif

 #ifdef KLST_DEBUG_CONFIG_MODE
 #if (KLST_CONFIG_MODE == KLST_CONFIG_MODE_HEADER_FILE)
 #warning "KLST using application specific configuration from 'KLST-ApplicationConfig.h'"
 #elif (KLST_CONFIG_MODE == KLST_CONFIG_MODE_APPLICATION_DEFINES)
 #warning "KLST using application specific configuration via defines"
 #elif (KLST_CONFIG_MODE == KLST_CONFIG_MODE_DEFAULT)
 #warning "KLST using default configuration"
 #endif
 #endif // KLST_DEBUG_CONFIG_MODE
 */

#endif // KLST_PANDA_STM32
