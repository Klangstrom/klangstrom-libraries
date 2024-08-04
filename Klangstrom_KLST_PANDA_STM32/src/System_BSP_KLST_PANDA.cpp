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

#include "KlangstromEnvironment.h"
#ifdef KLST_PANDA_STM32

#ifdef __cplusplus
extern "C" {
#endif

#include "AudioDevice.h"
#include "Console.h"
#include "Klangstrom_ASP_KLST_STM32-Config.h"         // TODO rename this
#include "Klangstrom_BSP_KLST_PANDA_STM32-Includes.h" // TODO rename this

AudioDevice* system_init_audiocodec() {
    AudioInfo audioinfo;
    audioinfo.sample_rate     = 48000;
    audioinfo.output_channels = 2;
    audioinfo.input_channels  = 2;
    audioinfo.block_size      = 128;
    audioinfo.bit_depth       = 16;
    AudioDevice* audiodevice  = audiodevice_init_audiocodec(&audioinfo);
    if (audioinfo.device_id == AUDIO_DEVICE_INIT_ERROR) {
        console_timestamp();
        console_error("error initializing audio device");
    }
    audiodevice_resume(audiodevice);
    return audiodevice;
}

static void system_init_BSP_KLST_PANDA();
static void system_init_BSP_KLST_PANDA_MX_Init_Modules();

static void system_init_BSP_KLST_PANDA_MX_Init_Modules() {
    // TODO really consider moving this to the periperhals
#ifdef KLST_PANDA_ENABLE_GPIO
    MX_GPIO_Init();
#endif // KLST_PANDA_ENABLE_GPIO

#if defined(KLST_PANDA_ENABLE_AUDIOCODEC) || defined(KLST_PANDA_ENABLE_DISPLAY)
    MX_I2C4_Init();
#endif // defined(KLST_PANDA_ENABLE_AUDIOCODEC) || defined(KLST_PANDA_ENABLE_DISPLAY)

    /* initialize DMA */
#if defined(KLST_PANDA_ENABLE_AUDIOCODEC) || \
    defined(KLST_PANDA_ENABLE_IDC_SERIAL) || \
    defined(KLST_PANDA_ENABLE_SD_CARD) ||    \
    defined(KLST_PANDA_ENABLE_MIDI)
    MX_DMA_Init();
#endif // defined(KLST_PANDA_ENABLE_AUDIOCODEC) || defined(KLST_PANDA_ENABLE_IDC_SERIAL) || defined(KLST_PANDA_ENABLE_MIDI)

#ifdef KLST_PANDA_ENABLE_MECHANICAL_KEYS
    MX_TIM4_Init();
#endif // KLST_PANDA_ENABLE_MECHANICAL_KEYS

#ifdef KLST_PANDA_ENABLE_EXTERNAL_MEMORY
    MX_OCTOSPI1_Init();
    HAL_Delay(100);
#endif // KLST_PANDA_ENABLE_EXTERNAL_MEMORY

#ifdef KLST_PANDA_ENABLE_ON_BOARD_MIC
    // MX_BDMA_Init();
    MX_CRC_Init();
    MX_PDM2PCM_Init();
    // MX_SAI4_Init();
#endif // KLST_PANDA_ENABLE_ON_BOARD_MIC

#ifdef KLST_PANDA_ENABLE_ENCODER
    MX_TIM1_Init();
    MX_TIM2_Init();
#endif // KLST_PANDA_ENABLE_ENCODER

#ifdef KLST_PANDA_ENABLE_SD_CARD
    // MX_FATFS_Init();
    // MX_SDMMC2_SD_Init();
#endif // KLST_PANDA_ENABLE_SD_CARD

#ifdef KLST_PANDA_ENABLE_IDC_SERIAL
    MX_UART9_Init();
    MX_UART8_Init();
#endif // KLST_PANDA_ENABLE_IDC_SERIAL

#ifdef KLST_PANDA_ENABLE_MIDI
    MX_UART4_Init();
#endif // KLST_PANDA_ENABLE_MIDI

#ifdef KLST_PANDA_ENABLE_USB_HOST
    MX_USB_HOST_Init();
#endif // KLST_PANDA_ENABLE_USB_HOST

#ifdef KLST_PANDA_ENABLE_USB_DEVICE
    MX_USB_DEVICE_Init();
#endif // KLST_PANDA_ENABLE_USB_DEVICE

#ifdef KLST_PANDA_ENABLE_ADC_DAC
    MX_ADC3_Init();
    MX_DAC1_Init();
#endif // KLST_PANDA_ENABLE_ADC_DAC

#ifdef KLST_PANDA_ENABLE_DISPLAY
    /* display+backlight+touch panel */
    MX_LTDC_Init();
    MX_DMA2D_Init();
    MX_TIM3_Init();
#endif // KLST_PANDA_ENABLE_DISPLAY
}

static void system_init_BSP_KLST_PANDA() {
    /* MPU Configuration--------------------------------------------------------*/
    MPU_Config();
    /* Enable the CPU Cache */

    /* Enable I-Cache---------------------------------------------------------*/
    SCB_EnableICache();

    /* Enable D-Cache---------------------------------------------------------*/
    SCB_EnableDCache();

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* Configure the peripherals common clocks */
    PeriphCommonClock_Config();
}

void system_init_BSP() {
    system_init_BSP_KLST_PANDA();
    system_init_BSP_KLST_PANDA_MX_Init_Modules();
}

#ifdef __cplusplus
}
#endif

#endif // KLST_PANDA_STM32
