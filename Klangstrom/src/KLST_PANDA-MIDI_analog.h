#ifndef INC_KLST_PANDA_MIDI_ANALOG_H_
#define INC_KLST_PANDA_MIDI_ANALOG_H_

#include "stdint.h"

#define MIDI_ANALOG_DMA_BUFFER_SIZE 64

// TODO make it C++ and pass UART instance

void MIDI_analog_setup();
void MIDI_analog_loop();
void MIDI_analog_handle_start_receive();
void MIDI_analog_handle_rx(uint16_t length);

#endif /* INC_KLST_PANDA_MIDI_ANALOG_H_ */
