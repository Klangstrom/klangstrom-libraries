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
#if defined(KLST_PERIPHERAL_ENABLE_SERIAL_DEBUG) || \
    defined(KLST_PERIPHERAL_ENABLE_IDC_SERIAL) ||   \
    defined(KLST_PERIPHERAL_ENABLE_MIDI)
#ifdef KLST_ARCH_IS_STM32

#include "main.h"
#include "SerialDevice.h"
#include "SerialDevice_ASP_STM32.h"

#ifdef __cplusplus
extern "C" {
#endif

void serialdevice_send(SerialDevice* serialdevice, const uint8_t* data, uint16_t length) {
    for (int i = 0; i < length; ++i) {
        serialdevice->peripherals->buffer_tx[i] = data[i];
    }
    HAL_UART_Transmit_DMA(serialdevice->peripherals->uart_handle,
                          serialdevice->peripherals->buffer_tx,
                          length);
}

void serialdevice_deinit_BSP(SerialDevice* serialdevice) {
    delete[] serialdevice->data;
    if (serialdevice->peripherals != nullptr) {
        delete serialdevice->peripherals;
        serialdevice->peripherals = nullptr;
    }
}

void serialdevice_deinit_peripherals_BSP(SerialDevice* serialdevice) {
    delete serialdevice->peripherals;
    serialdevice->peripherals = nullptr;
}

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_STM32
#endif // KLST_PERIPHERAL_ENABLE_SERIAL_DEBUG || KLST_PERIPHERAL_ENABLE_IDC_SERIAL || KLST_PERIPHERAL_ENABLE_MIDI