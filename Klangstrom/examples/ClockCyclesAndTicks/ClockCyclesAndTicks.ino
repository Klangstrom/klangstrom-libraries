/**
 * this example demonstrates how to measure time in ticks and clock cycles.
*/

#include "Arduino.h"
#include "System.h"
#include "Console.h"

void setup() {
    system_init();

    console_println("showing clock cycles since counter was enabled:");
    system_enable_cycle_counter(true);
    delay(10);
    uint32_t cycles = system_get_cycles();
    system_enable_cycle_counter(false);
    console_print("at a clock frequency of %lu Hz ", system_clock_frequency());
    console_println("%lu clock cycles would require %f us", cycles, system_cycles_to_micros(cycles));

    console_println("");
    console_println("showing time in ticks:");
    uint32_t ticks = system_get_ticks();
    delay(10);
    console_println("%lu ticks are equal to 10 ms in this configuration", (system_get_ticks() - ticks));
}

void loop() {}
