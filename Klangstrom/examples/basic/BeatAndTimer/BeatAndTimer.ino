/**
 * this example demonstrates how to use beat and timer.
*/

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "Beat.h"
#include "Timer.h"

uint8_t counter = 0;

Beat   beat;
Timer* timer;

void setup() {
    system_init();

    timer = timer_create(13);
    timer_resume(timer);
    timer_set_overflow(timer, 1000000 / 10);

    beat.init(7);
    beat.bpm(60);
    beat.start();
}

void loop() {
    counter++;
    if (counter == 16) {
        beat.pause();
        console_println("pausing beat");
    }
    if (counter == 32) {
        beat.resume();
        counter = 0;
        console_println("resuming beat");
    }

    console_println("%02i loop", counter);
    delay(500);
}

void beat_event(const uint8_t beat_id, const uint16_t beat_counter) {
    console_println("beat: ID %i, counter %03i", beat_id, beat_counter);
}

void timer_event(Timer* timer) {
    console_println("timer: ID %i", timer->timer_id);
}