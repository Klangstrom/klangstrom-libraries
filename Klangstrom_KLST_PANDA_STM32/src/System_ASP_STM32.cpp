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
#ifdef KLST_ARCH_IS_STM32

#include "System.h"
#include "ArrayList.h"
#include "stm32h7xx_hal.h"
#include "AudioDevice_ASP_STM32.h"
#include "SerialDevice_ASP_STM32.h"
#include "Console.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t system_get_tick_BSP() {
    return HAL_GetTick();
}

#define ARM_CM_DEMCR (*(uint32_t*) 0xE000EDFC)
#define ARM_CM_DWT_CTRL (*(uint32_t*) 0xE0001000)
#define ARM_CM_DWT_CYCCNT (*(uint32_t*) 0xE0001004)

static bool cycle_counter_enabled = false;
static void enable_cycle_counter() {
    if (ARM_CM_DWT_CTRL != 0) {  // See if DWT is available
        ARM_CM_DEMCR |= 1 << 24; // Set bit 24
        ARM_CM_DWT_CYCCNT = 0;
        ARM_CM_DWT_CTRL |= 1 << 0; // Set bit 0
    }
}

void system_reset_cycles() {
    ARM_CM_DWT_CYCCNT = 0;
}

uint32_t system_get_cycles_BSP() {
    if (!cycle_counter_enabled) {
        enable_cycle_counter();
        cycle_counter_enabled = true;
    }
    return ARM_CM_DWT_CYCCNT;
    //    return DWT->CYCCNT; // TODO test both
}

uint32_t system_clock_frequency() {
    return HAL_RCC_GetSysClockFreq();
}

/* ----------------- SAI ---------------- */

static void SAI_TX_event(SAI_HandleTypeDef* hsai, uint8_t callback_event) {
    ArrayList_AudioDevicePtr* fAudioDeviceListeners = system_get_registered_audiodevices();
    for (size_t i = 0; i < fAudioDeviceListeners->size; i++) {
        AudioDevice* ad = arraylist_AudioDevicePtr_get(fAudioDeviceListeners, i);
        if (ad != nullptr) {
            if (ad->peripherals->audiodevice_sai_tx->Instance == hsai->Instance) {
                if (ad->peripherals->callback_tx != nullptr) {
                    ad->peripherals->callback_tx(ad, callback_event);
                }
            }
        }
    }
}

void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef* hsai) {
    SAI_TX_event(hsai, CALLBACK_FULL_COMPLETE);
}

void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef* hsai) {
    SAI_TX_event(hsai, CALLBACK_HALF_COMPLETE);
}

static void SAI_RX_event(SAI_HandleTypeDef* hsai, uint8_t callback_event) {
    ArrayList_AudioDevicePtr* fAudioDeviceListeners = system_get_registered_audiodevices();
    for (size_t i = 0; i < fAudioDeviceListeners->size; i++) {
        AudioDevice* ad = arraylist_AudioDevicePtr_get(fAudioDeviceListeners, i);
        if (ad != nullptr) {
            if (ad->peripherals->audiodevice_sai_rx->Instance == hsai->Instance) {
                if (ad->peripherals->callback_rx != nullptr) {
                    ad->peripherals->callback_rx(ad, callback_event);
                }
            }
        }
    }
}

void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef* hsai) {
    SAI_RX_event(hsai, CALLBACK_FULL_COMPLETE);
}

void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef* hsai) {
    SAI_RX_event(hsai, CALLBACK_HALF_COMPLETE);
}

void HAL_SAI_ErrorCallback(SAI_HandleTypeDef* hsai) {
    ArrayList_AudioDevicePtr* fAudioDeviceListeners = system_get_registered_audiodevices();
    for (size_t i = 0; i < fAudioDeviceListeners->size; i++) {
        AudioDevice* ad = arraylist_AudioDevicePtr_get(fAudioDeviceListeners, i);
        if (ad != nullptr) {
            if (ad->peripherals->audiodevice_sai_tx->Instance == hsai->Instance) {
                if (ad->peripherals->callback_error != nullptr) {
                    ad->peripherals->callback_error(ad, CALLBACK_TX_ERROR);
                }
            }
            if (ad->peripherals->audiodevice_sai_rx->Instance == hsai->Instance) {
                if (ad->peripherals->callback_error != nullptr) {
                    ad->peripherals->callback_error(ad, CALLBACK_RX_ERROR);
                }
            }
        }
    }
}

/* --------------- U(S)ART -------------- */

static void start_receive(SerialDevice* serialdevice) {
    HAL_UARTEx_ReceiveToIdle_DMA(serialdevice->peripherals->uart_handle,
                                 serialdevice->peripherals->buffer_rx,
                                 serialdevice->data_buffer_size);
    __HAL_DMA_DISABLE_IT(serialdevice->peripherals->dma_handle_rx, DMA_IT_HT);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size) {
    console_status("HAL_UARTEx_RxEventCallback");
    ArrayList_SerialDevicePtr* mSerialDevices = system_get_registered_serialdevices();
    for (size_t i = 0; i < mSerialDevices->size; i++) {
        SerialDevice* sd = arraylist_SerialDevicePtr_get(mSerialDevices, i);
        if (sd != nullptr) {
            if (sd->peripherals->uart_handle->Instance == huart->Instance) {
                if (sd->callback_serial != nullptr) {
                    sd->callback_serial(sd);
                    if (Size > sd->data_buffer_size) {
                        Size = sd->data_buffer_size;
                        console_error("serial RX buffer overflow");
                    }
                    /* copy data to buffer */
                    for (int j = 0; j < Size; ++j) {
                        sd->data[j] = sd->peripherals->buffer_rx[j];
                    }
                    sd->length = Size;
                    start_receive(sd);
                }
            }
        }
    }
    // void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {}
}


void HAL_UART_ErrorCallback(UART_HandleTypeDef* huart) {
    console_error("HAL_UART_ErrorCallback");
    ArrayList_SerialDevicePtr* mSerialDevices = system_get_registered_serialdevices();
    for (size_t i = 0; i < mSerialDevices->size; i++) {
        SerialDevice* sd = arraylist_SerialDevicePtr_get(mSerialDevices, i);
        if (sd != nullptr) {
            if (sd->peripherals->uart_handle->Instance == huart->Instance) {
                start_receive(sd);
            }
        }
    }
}

#ifdef SYSTEM_HANDLE_TIMEOFDAY_WARNING
#include <sys/time.h>
[[maybe_unused]] int _gettimeofday(struct timeval* tv, void* tzvp) {
    (void) tv;
    (void) tzvp;
    return 0;
}
#endif // SYSTEM_HANDLE_TIMEOFDAY_WARNING

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_STM32
