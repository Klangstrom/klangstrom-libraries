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

#include "Klangstrom.h"

#include <DMAMemoryAllocator.h>

#ifdef KLST_PERIPHERAL_ENABLE_ADC_DAC
#ifdef KLST_PANDA_STM32

#include "main.h"
#include "bdma.h"
#include "adc.h"
#include "dac.h"
#include "DACADC.h"
#include "Console.h"

#ifdef __cplusplus
extern "C" {
#endif

extern DAC_HandleTypeDef hdac1;
extern ADC_HandleTypeDef hadc3;

#define KLST_ADC_POLLING_DURATION_MS 2
static constexpr uint16_t DAC_RESOLUTION = 1 << 12;
static constexpr float    ADC_RESOLUTION = 1.0f / static_cast<float>(DAC_RESOLUTION);

// #define KLST_ADC_CONTINOUS_MODE

#ifdef KLST_ADC_CONTINOUS_MODE
uint32_t* fADCBuffer;
#endif // KLST_ADC_CONTINOUS_MODE

void adc_init() {
#ifdef KLST_ADC_CONTINOUS_MODE
#endif // KLST_ADC_CONTINOUS_MODE
    MX_ADC3_Init();
    // HAL_ADCEx_Calibration_Start(&hadc3, ADC_CALIB_OFFSET_LINEARITY, ADC_SINGLE_ENDED);
#ifdef KLST_ADC_CONTINOUS_MODE
    MX_BDMA_Init();
    fADCBuffer                  = static_cast<uint32_t*>(dma_malloc(4));
    const HAL_StatusTypeDef ret = HAL_ADC_Start_DMA(&hadc3, fADCBuffer, 1);
    if (ret) {
        console_error("ADC: error at start time: %i", ret);
    }
#endif // KLST_ADC_CONTINOUS_MODE
}

void dac_init() {
    MX_DAC1_Init();
    dac_start();
}

void dac_start() {
    const HAL_StatusTypeDef status = HAL_DAC_Start(&hdac1, DAC1_CHANNEL_1);
    if (status) {
        console_error("DAC: error at start time: %i", status);
    }
}

void dac_stop() {
    HAL_DAC_Stop(&hdac1, DAC1_CHANNEL_1);
}

void dac_write(float value) {
    if (value > 1.0) {
        value = 1.0;
    } else if (value < 0.0) {
        value = 0.0;
    }
    const uint32_t mData = value * DAC_RESOLUTION;
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, mData);
}

void adc_start() {}

void adc_stop() {}

float adc_read() {
#ifdef KLST_ADC_CONTINOUS_MODE
    return static_cast<float>(fADCBuffer[0]) * ADC_RESOLUTION;
#else
    // TODO consider using DMA and run in background
    HAL_ADC_Start(&hadc3);
#define KLST_ADC_POLLING
#ifdef KLST_ADC_POLLING
    HAL_ADC_PollForConversion(&hadc3, KLST_ADC_POLLING_DURATION_MS);
    const uint32_t mData = HAL_ADC_GetValue(&hadc3);
#else
    const uint32_t mData = HAL_ADC_GetValue(&hadc3);
    __HAL_ADC_CLEAR_FLAG(&hadc3, ADC_FLAG_EOS);
#endif
    HAL_ADC_Stop(&hadc3);
    return static_cast<float>(mData) * ADC_RESOLUTION;
#endif // KLST_ADC_CONTINOUS_MODE
}

#ifdef __cplusplus
}
#endif

#endif // KLST_PANDA_STM32
#endif // KLST_PERIPHERAL_ENABLE_ADC_DAC
