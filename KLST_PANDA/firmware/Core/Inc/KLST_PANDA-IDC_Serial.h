#ifndef INC_KLST_PANDA_IDC_SERIAL_H_
#define INC_KLST_PANDA_IDC_SERIAL_H_

#include "stdbool.h"
#include "stdint.h"
#include "main.h"

extern volatile bool receive_00;
extern volatile bool receive_01;
extern volatile bool receive_MIDI;

// maybe make C++
void IDC_serial_setup();
void IDC_serial_loop();
void IDC_serial_start_rx_interrupt(USART_TypeDef* uart_instance);

#endif /* INC_KLST_PANDA_IDC_SERIAL_H_ */
