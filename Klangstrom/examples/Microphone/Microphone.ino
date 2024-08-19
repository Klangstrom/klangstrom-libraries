/**
 * this example demonstrates how to use the debugging console to print messages to the serial console.
*/

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "Microphone.h"

#include <algorithm>

float max0 = 0.0;
float max1 = 0.0;
float sum0 = 0.0;
float sum1 = 0.0;

void setup() {
    system_init();
    console_init();
    microphone_init();
}

void loop() {
    console_clear();
    console_println("ENERGY: %09.3f / %09.3f", sum0, sum1);
    console_println("PEAK  : %09.3f / %09.3f", max0, max1);
    delay(100);
}

void audioblock(AudioBlock* audio_block) {
    sum0 = 0.0;
    sum1 = 0.0;
    max0 *= 0.75f;
    max1 *= 0.75f;
    for (int i = 0; i < audio_block->block_size; ++i) {
        sum0 += abs(audio_block->input[0][i]);
        sum1 += abs(audio_block->input[1][i]);
        max0 = std::max(max0, abs(audio_block->input[0][i]));
        max1 = std::max(max1, abs(audio_block->input[1][i]));
    }
    sum0 /= audio_block->block_size;
    sum1 /= audio_block->block_size;
}
