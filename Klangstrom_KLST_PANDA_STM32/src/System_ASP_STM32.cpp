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
#include "Console.h"
#include "stm32h7xx_hal.h"
#include "AudioDevice_ASP_STM32.h"

#ifdef __cplusplus
extern "C" {
#endif

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
