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

#include "Klangstrom_BSP_KLST_PANDA_STM32-Config.h"
#ifdef KLST_PANDA_ENABLE_ON_BOARD_MIC

#include "string.h"
#include "pdm2pcm.h"
#include "KLST_PANDA-SerialDebug.h"
#include "KLST_PANDA-OnBoardMic.h"

//extern SAI_HandleTypeDef hsai_BlockA4;
//extern PDM_Filter_Handler_t PDM1_filter_handler;

#include "KlangstromDefines.h"

#define PDM_BUFFER_SIZE 128
uint16_t __attribute__((section(".dma_buffer"))) pdmRxBuf[PDM_BUFFER_SIZE];

uint16_t fifobuf[256];
uint8_t fifo_w_ptr = 0;
uint8_t fifo_r_ptr = 0;
uint8_t fifo_read_enabled = 0;

//uint16_t MidBuffer[16];
//uint8_t txstate = 0;
//uint8_t rxstate = 0;

//static void FifoWrite(uint16_t data) {
//    fifobuf[fifo_w_ptr] = data;
//    fifo_w_ptr++;
//}
//
//static uint16_t FifoRead() {
//    uint16_t val = fifobuf[fifo_r_ptr];
//    fifo_r_ptr++;
//    return val;
//}

//static void print_FIFO() {
//    uint32_t mSum = 0;
//    for (int i = 0; i < 16; i++) {
//        mSum += FifoRead();
//    }
//    mSum /= 16;
//    printf("%li\n\r", mSum);
//}

uint8_t onboardmic_setup() {
    HAL_StatusTypeDef status = HAL_OK;
//    status = HAL_SAI_Receive_DMA(&hsai_BlockA4, (uint8_t*) pdmRxBuf, PDM_BUFFER_SIZE);
    if (HAL_OK != status) {
        println("### ERROR initializing SAI MIC RX: %i", status);
    }

    memset(pdmRxBuf, 0, sizeof(pdmRxBuf));

    return status;
}

void onboardmic_loop() {
    for (int i = 0; i < PDM_BUFFER_SIZE; i++) {
        printf("0x%04X, ", pdmRxBuf[i]);
    }
    printf("\r\n");
}

void onboardmic_RX_full_complete_callback(SAI_HandleTypeDef *hsai) {
//    if (hsai == &hsai_BlockA4) {
////        PDM_Filter(&pdmRxBuf[64], &MidBuffer[0], &PDM1_filter_handler);
////        for (int i = 0; i < 16; i++) {
////            FifoWrite(MidBuffer[i]);
////        }
////        print_FIFO();
//    }
}

void onboardmic_RX_half_complete_callback(SAI_HandleTypeDef *hsai) {
//    if (hsai == &hsai_BlockA4) {
////        PDM_Filter(&pdmRxBuf[0], &MidBuffer[0], &PDM1_filter_handler);
////        for (int i = 0; i < 16; i++) {
////            FifoWrite(MidBuffer[i]);
////        }
////        print_FIFO();
//    }
}

void onboardmic_error_callback(SAI_HandleTypeDef *hsai) {
//    if (hsai == &hsai_BlockA4) {
//        println("### ERROR error in MIC SAI:BlockA4");
//    }
}

#endif // KLST_PANDA_ENABLE_ON_BOARD_MIC

#endif // KLST_PANDA_STM32
