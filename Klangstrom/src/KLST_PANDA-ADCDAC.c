#include "main.h"
#include "KLST_PANDA-SerialDebug.h"
#include "KLST_PANDA-ADCDAC.h"

extern DAC_HandleTypeDef hdac1;
extern ADC_HandleTypeDef hadc3;

static uint16_t ADC_DAC_RESOLUTION = (1 << 12);

void ADC_setup() {
//    HAL_StatusTypeDef status = HAL_ADC_Start(&hadc3);
//    if (status) {
//        println("ADC: error at start time: %i", status);
//    }
}

void DAC_setup() {
    HAL_StatusTypeDef status = HAL_DAC_Start(&hdac1, DAC1_CHANNEL_1);
    if (status) {
        println("DAC: error at start time: %i", status);
    }
//    HAL_DAC_Stop(&hdac1, DAC1_CHANNEL_1);
}

void DAC_write(float value) {
    if (value > 1.0) {
        value = 1.0;
    } else if (value < 0.0) {
        value = 0.0;
    }
    uint32_t mData = value * ADC_DAC_RESOLUTION;
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, mData);
}

float ADC_read() {
    // TODO consider using DMA
    HAL_ADC_Start(&hadc3);
    HAL_ADC_PollForConversion(&hadc3, 3);
    uint32_t mData = HAL_ADC_GetValue(&hadc3);
    HAL_ADC_Stop(&hadc3);
    return (float) mData / (float) ADC_DAC_RESOLUTION;
}
