/**
 * this example demonstrates how to use mechanical keys.
*/

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "Key.h"

Key* key_LEFT;
Key* key_RIGHT;

void setup() {
    system_init();

    console_println("Mechanical Key Example");

    key_init();
    key_LEFT  = key_create(KEY_LEFT);
    key_RIGHT = key_create(KEY_RIGHT);
}

void loop() {}

void key_event(const Key* key) {
    if (key->device_type == KEY_LEFT) {
        console_println("Key LEFT: %i", key->pressed);
    }
    if (key->device_type == KEY_RIGHT) {
        console_println("Key RIGHT: %i", key->pressed);
    }
}
