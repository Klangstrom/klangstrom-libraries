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

#include "InternalMicrophone.h"

#include "Klangstrom.h"
#if defined(KLST_PANDA_STM32) || defined(KLST_CATERPILLAR_STM32)
#ifdef KLST_PERIPHERAL_ENABLE_ON_BOARD_MIC

#include "Console.h"
#include "dfsdm.h"
#include "InternalMicrophone.h"

#ifdef __cplusplus
extern "C" {
#endif
#define FFT_SampleNum 2048
int32_t Buff[FFT_SampleNum * 2] = {0};

void internalmicrophone_init_BSP() {
    MX_DFSDM1_Init();
    if (HAL_DFSDM_FilterRegularStart_DMA(&hdfsdm1_filter0, Buff, FFT_SampleNum) != HAL_OK) {
        Error_Handler();
    }
}

/**
     * @brief  Half regular conversion complete callback.
     * @param  hdfsdm_filter : DFSDM filter handle.
     * @retval None
     */
void HAL_DFSDM_FilterRegConvHalfCpltCallback(DFSDM_Filter_HandleTypeDef* hdfsdm_filter) {
    /*static int i = 0;
     printf("%d\r\n", i);
     i++;*/
}

/**
     * @brief  Regular conversion complete callback.
     * @note   In interrupt mode, user has to read conversion value in this function
     using HAL_DFSDM_FilterGetRegularValue.
     * @param  hdfsdm_filter : DFSDM filter handle.
     * @retval None
     */
void HAL_DFSDM_FilterRegConvCpltCallback(DFSDM_Filter_HandleTypeDef* hdfsdm_filter) {
    console_status("DFSDM complete");
    //    if (flag) {
    //        for (uint32_t i = 0; i < FFT_SampleNum; i++) {
    //            FFT_inp_int32[i] = Buff[i];
    //        }
    //        flag = false;
    //    }
}

#ifdef __cplusplus
}
#endif

#endif // KLST_PERIPHERAL_ENABLE_ON_BOARD_MIC
#endif // defined(KLST_PANDA_STM32) || defined(KLST_CATERPILLAR_STM32)