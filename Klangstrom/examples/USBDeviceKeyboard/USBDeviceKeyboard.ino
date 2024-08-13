/**
 * this example demonstrates how to use the debugging console to print messages to the serial console.
*/

#include "Arduino.h"
#include "usb_host.h"
#include "System.h"
#include "Console.h"

void setup() {
    system_init();
    MX_USB_HOST_Init();

    console_timestamp();
    console_println("World!");
}

uint32_t counter = 0;

void loop() {
    counter++;
    if ((counter % (65536 * 128) ) == 0) {
        console_println(".");
    }
    MX_USB_HOST_Process();
}
