/**
 * this example demonstrates how to use the debugging console to print messages to the serial console.
*/

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "RotaryEncoder.h"

uint32_t counter = 0;

RotaryEncoder* encoder_00;
RotaryEncoder* encoder_01;

void setup() {
    system_init();

    console_println("RotaryEncoder example");

    encoder_00 = rotaryencoder_create(ROTARY_ENCODER_LEFT);
    encoder_01 = rotaryencoder_create(ROTARY_ENCODER_RIGHT);
}

void loop() {
    counter++;
    if (counter * 256 == 0) {
        console_println(".");
        // console_println("ROTARY_ENCODER_LEFT: %lu", rotaryencoder_get_rotation(ROTARY_ENCODER_LEFT));
    }
}
