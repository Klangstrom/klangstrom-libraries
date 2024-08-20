/**
 * this example demonstrates how to use the debugging console to print messages to the serial console.
*/

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "DACADC.h"

constexpr float fixed_value = 0.5f;

void setup() {
    system_init();

    adc_init();
    dac_init();
}

void loop() {
    dac_write(fixed_value);
    const float read_value = adc_read();
    console_println("ADC: %f", read_value);
    delay(250);
}
