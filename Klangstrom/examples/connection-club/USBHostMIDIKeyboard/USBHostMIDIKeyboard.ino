/**
 * this example demonstrates how to use the debugging console to print messages to the serial console.
*/

#include "Arduino.h"
#include "USBHost.h"
#include "System.h"
#include "Console.h"
#include "LED.h"

#include "AudioDevice.h"
#include "Wavetable.h"
#include "ADSR.h"
#include "Noise.h"
#include "Reverb.h"

AudioDevice* audiodevice;

using namespace klangwellen;
float     wavetable[512];
Wavetable oscillator{wavetable, 512, 48000};
ADSR      adsr;
ADSR      adsr_tick;
Noise     noise;
Reverb    reverb;

constexpr uint8_t ppqn_counter_start = 5;
uint32_t          ppqn_counter       = ppqn_counter_start;

void setup() {
    system_init();

    led_init();
    led_on(0);
    led_off(0);

    usb_host_init();
    usb_host_callback_midi_clock_tick(midi_clock_tick);
    usb_host_callback_midi_note_on(midi_note_on);
    usb_host_callback_midi_note_off(midi_note_off);
    usb_host_callback_midi_clock_start(midi_clock_start);

    oscillator.set_amplitude(0.25f);
    oscillator.set_frequency(220);
    oscillator.set_waveform(KlangWellen::WAVEFORM_TRIANGLE);

    adsr_tick.set_attack(0.01);
    adsr_tick.set_decay(0.02);
    adsr_tick.set_sustain(0.0);
    adsr_tick.set_release(0.01);

    noise.set_type(KlangWellen::NOISE_WHITE);

    audiodevice = system_init_audiocodec();
}

void midi_clock_start() {
    ppqn_counter = ppqn_counter_start;
}

uint32_t counter = 5;

void loop() {
    counter++;
    if ((counter % (65536 * 128)) == 0) {
        console_println(".");
    }
    usb_host_process();
}

void beat(uint32_t beat_counter) {
    led_toggle(0);
    if (beat_counter % 4 == 1) {
        led_toggle(1);
        noise.set_amplitude(0.5);
    } else {
        noise.set_amplitude(0.1);
    }
    adsr_tick.start();
}

void midi_clock_tick() {
    ppqn_counter++;
    /* 16th note */
    if (ppqn_counter % 6 == 0) {
        beat(ppqn_counter / 6);
    }
}

void midi_note_on(const uint8_t channel, const uint8_t note, const uint8_t velocity) {
    led_on(1);
    oscillator.set_frequency(KlangWellen::midi_note_to_frequency(note));
    adsr.start();
}

void midi_note_off(const uint8_t channel, const uint8_t note, const uint8_t velocity) {
    led_off(1);
    adsr.stop();
}

void audioblock(AudioBlock* audio_block) {
    for (int i = 0; i < audio_block->block_size; ++i) {
        float sample = oscillator.process();
        sample *= adsr.process();
        sample                    = KlangWellen::clamp(sample * 4.0) * 0.8;
        sample                    = reverb.process(sample);
        audio_block->output[0][i] = sample;
        audio_block->output[1][i] = noise.process() * adsr_tick.process();
    }
}