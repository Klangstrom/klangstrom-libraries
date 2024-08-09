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

#include "Klangstrom_ASP_KLST_STM32-Config.h" // TODO change this to KLST_STM32 aka 'Architecture Specific' (ASP)
#ifdef KLST_PANDA_ENABLE_IDC_SERIAL
#include "KlangstromEnvironment.h"
#ifdef KLST_PANDA_STM32

#include "main.h"
#include "usart.h"
#include "Console.h"
#include "SerialDevice.h"
#include "DMAMemoryAllocator.h"
#include "SerialDevice_ASP_STM32.h"

#ifdef __cplusplus
extern "C" {
#endif

extern UART_HandleTypeDef huart8;
extern UART_HandleTypeDef huart9;
extern DMA_HandleTypeDef  hdma_uart8_rx;
extern DMA_HandleTypeDef  hdma_uart8_tx;
extern DMA_HandleTypeDef  hdma_uart9_rx;
extern DMA_HandleTypeDef  hdma_uart9_tx;

void serialdevice_send(SerialDevice* serialdevice, const uint8_t* data, uint16_t length) {
    for (int i = 0; i < length; ++i) {
        serialdevice->peripherals->buffer_tx[i] = data[i];
    }
    HAL_UART_Transmit_DMA(serialdevice->peripherals->uart_handle,
                          serialdevice->peripherals->buffer_tx,
                          length);
}

static void start_receive(SerialDevice* serialdevice) {
    HAL_UARTEx_ReceiveToIdle_DMA(serialdevice->peripherals->uart_handle,
                                 serialdevice->peripherals->buffer_rx,
                                 serialdevice->data_buffer_size);
    __HAL_DMA_DISABLE_IT(serialdevice->peripherals->dma_handle_rx, DMA_IT_HT);
}

void serialdevice_init_BSP(SerialDevice* serialdevice) {
    if (serialdevice->peripherals == nullptr) {
        console_error("ERROR: peripherals not initialized");
        return;
    }
    if (serialdevice->device_type < SERIAL_DEVICE_MAX_NUMBER_OF_DEVICE_TYPES) {
        if (serialdevice->device_type == SERIAL_DEVICE_TYPE_IDC_00) {
            console_status("device type: IDC_00");
            MX_UART8_Init();
            start_receive(serialdevice);
        }
        if (serialdevice->device_type == SERIAL_DEVICE_TYPE_IDC_01) {
            console_status("device type: IDC_01");
            MX_UART9_Init();
            start_receive(serialdevice);
        }
    } else {
        console_status("device type: custom");
    }
}

void serialdevice_deinit_BSP(SerialDevice* serialdevice) {
    delete[] serialdevice->data;
    if (serialdevice->peripherals != nullptr) {
        delete serialdevice->peripherals;
        serialdevice->peripherals = nullptr;
    }
}

void serialdevice_init_peripherals_BSP(SerialDevice* serialdevice) {
    serialdevice->peripherals = new SerialPeripherals();

    if (serialdevice->device_type < SERIAL_DEVICE_MAX_NUMBER_OF_DEVICE_TYPES) {
        console_status("device type: type: %i", serialdevice->device_type);

        if (serialdevice->device_type == SERIAL_DEVICE_TYPE_MIDI_IN ||
            serialdevice->device_type == SERIAL_DEVICE_TYPE_MIDI_OUT) {
            console_status("device type: is MIDI");
        }

        if (serialdevice->device_type == SERIAL_DEVICE_TYPE_IDC_00) {
            console_status("device type: is IDC_00");
            serialdevice->peripherals                = new SerialPeripherals;
            serialdevice->peripherals->uart_handle   = &huart8;
            serialdevice->peripherals->dma_handle_rx = &hdma_uart8_rx;
            serialdevice->peripherals->dma_handle_tx = &hdma_uart8_tx;
            serialdevice->peripherals->buffer_rx     = (uint8_t*) dma_malloc(serialdevice->peripherals->buffer_size);
            serialdevice->peripherals->buffer_tx     = (uint8_t*) dma_malloc(serialdevice->peripherals->buffer_size);
        }
        if (serialdevice->device_type == SERIAL_DEVICE_TYPE_IDC_01) {
            console_status("device type: is IDC_01");
            serialdevice->peripherals                = new SerialPeripherals;
            serialdevice->peripherals->uart_handle   = &huart9;
            serialdevice->peripherals->dma_handle_rx = &hdma_uart9_rx;
            serialdevice->peripherals->dma_handle_tx = &hdma_uart9_tx;
            serialdevice->peripherals->buffer_rx     = (uint8_t*) dma_malloc(serialdevice->peripherals->buffer_size);
            serialdevice->peripherals->buffer_tx     = (uint8_t*) dma_malloc(serialdevice->peripherals->buffer_size);
        }
    } else {
        console_status("device type: custom");
    }
}

void serialdevice_deinit_peripherals_BSP(SerialDevice* serialdevice) {
    delete serialdevice->peripherals;
    serialdevice->peripherals = nullptr;
}

#ifdef __cplusplus
}
#endif

#endif // KLST_PANDA_STM32
#endif // KLST_PANDA_ENABLE_IDC_SERIAL