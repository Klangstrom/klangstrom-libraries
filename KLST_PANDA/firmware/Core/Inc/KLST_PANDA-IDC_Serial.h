#ifndef INC_KLST_PANDA_IDC_SERIAL_H_
#define INC_KLST_PANDA_IDC_SERIAL_H_

#include "stdint.h"
#include "main.h"

#define BUFFER_SIZE 256

extern  uint8_t RX_00_buffer[BUFFER_SIZE];
extern  uint8_t RX_01_buffer[BUFFER_SIZE];
extern  uint8_t RX_MIDI_buffer[BUFFER_SIZE];

extern volatile uint8_t RX_00_counter;
extern volatile uint8_t RX_01_counter;
extern volatile uint8_t RX_MIDI_counter;

// maybe make C++
void IDC_serial_setup();
void IDC_serial_loop();
uint8_t IDC_serial_handle_rx(USART_TypeDef* uart_instance);

#endif /* INC_KLST_PANDA_IDC_SERIAL_H_ */
