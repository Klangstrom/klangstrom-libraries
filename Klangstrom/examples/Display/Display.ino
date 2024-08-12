/**
 * this example demonstrates how to use LEDs.
 */

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "Display.h"

uint8_t fLEDIntensity = 0;

void setup() {
    system_init();

    display_init();
    display_set_backlight(0.5f);
}

void loop() {
    console_println("...");
    LTDC_loop();
    backlight_loop();
    delay(500);
}
