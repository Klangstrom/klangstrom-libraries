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

#pragma once

#include "KlangstromEnvironment.h"
#ifdef KLST_PANDA_STM32

#include "stdint.h"
#include "main.h"

#define BUFFER_SIZE 256
#define DMA_BUFFER_SIZE 64

extern uint8_t RX_00_buffer[BUFFER_SIZE];
//extern  uint8_t RX_01_buffer[BUFFER_SIZE];
extern uint8_t __attribute__((section(".dma_buffer"))) RX_01_DMA_buffer[DMA_BUFFER_SIZE];
extern uint8_t __attribute__((section(".dma_buffer"))) TX_01_DMA_buffer[DMA_BUFFER_SIZE];

extern volatile uint8_t RX_00_counter;
//extern volatile uint8_t RX_01_counter;

// TODO make C++
void    IDC_serial_setup();
void    IDC_serial_loop();
uint8_t IDC_serial_handle_rx(USART_TypeDef* uart_instance, uint16_t length);

#endif // KLST_PANDA_STM32
