/**
 * this example demonstrates how to use the debugging console to print messages to the serial console.
*/

#include "Arduino.h"
#include "USBHost.h"
#include "System.h"
#include "Console.h"

void setup() {
    system_init();
    usb_host_init();

    console_timestamp();
    console_println("USBHost!");
}

uint32_t counter = 0;

void loop() {
    counter++;
    if ((counter % (65536 * 128) ) == 0) {
        console_println(".");
    }
    usb_host_process();
}
