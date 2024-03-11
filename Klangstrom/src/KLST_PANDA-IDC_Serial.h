#ifndef INC_KLST_PANDA_IDC_SERIAL_H_
#define INC_KLST_PANDA_IDC_SERIAL_H_

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
void IDC_serial_setup();
void IDC_serial_loop();
uint8_t IDC_serial_handle_rx(USART_TypeDef *uart_instance, uint16_t length);

#endif /* INC_KLST_PANDA_IDC_SERIAL_H_ */
