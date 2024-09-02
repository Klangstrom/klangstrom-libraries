/**
 * this example demonstrates how to use the debugging console to print messages to the serial console.
*/

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "Beat.h"

uint8_t counter = 0;

Beat fBeat{23};

void setup() {
    system_init();

    fBeat.bpm(120);
    fBeat.start();
}

void loop() {
    counter++;
    if (counter == 16) {
        fBeat.pause();
        console_println("pausing beat");
    }
    if (counter == 32) {
        fBeat.resume();
        counter = 0;
        console_println("resuming beat");
    }

    console_println("%02i loop", counter);
    delay(500);
}

void beat(const uint8_t beat_id, const uint16_t beat_counter) {
    console_println("beat: ID %i, counter %03i", beat_id, beat_counter);
}
