/**
 * this example demonstrates how to setup the audio code and generate a sine wave sound with the klangwellen library.
 */

#define KLANGWELLEN_WAVETABLE_INTERPOLATE_SAMPLES 1

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "AudioDevice.h"
#include "Wavetable.h"
#include "OscillatorFunction.h"

using namespace klangwellen;

float              wavetable[512];
Wavetable          oscillator_left{wavetable, 512, 48000};
OscillatorFunction oscillator_right{};

AudioDevice* audiodevice;

float osc_frequency     = 440.0f;
bool  audiocodec_paused = false;

void setup() {
    system_init();

    Wavetable::fill(wavetable, 512, KlangWellen::WAVEFORM_SINE);
    oscillator_left.set_amplitude(0.5f);

    oscillator_right.set_amplitude(0.5f);
    oscillator_right.set_waveform(KlangWellen::WAVEFORM_SINE);

    // long init section ...
    AudioInfo audioinfo;
    audioinfo.sample_rate     = 48000;
    audioinfo.output_channels = 2;
    audioinfo.input_channels  = 2;
    audioinfo.block_size      = 128;
    audioinfo.bit_depth       = 16;
    audiodevice               = audiodevice_init_audiocodec(&audioinfo);
    if (audiodevice->audioinfo->device_id == AUDIO_DEVICE_INIT_ERROR) {
        console_error("error initializing audio device");
    }
    audiodevice_resume(audiodevice);
    // ... or for short with default values and auto start
    // audiodevice = system_init_audiocodec();
}

void loop() {
    osc_frequency += 10.0f;
    if (osc_frequency > 880.0f) {
        osc_frequency = 220.0f;
    }
    oscillator_left.set_frequency(osc_frequency);
    oscillator_right.set_frequency(osc_frequency * 0.49f);
    console_println("frequency: %f", osc_frequency);

    delay(1000);
}

void audioblock(AudioBlock* audio_block) {
    for (int i = 0; i < audio_block->block_size; ++i) {
        audio_block->output[0][i] = oscillator_left.process();
        audio_block->output[1][i] = oscillator_right.process();
    }
}
