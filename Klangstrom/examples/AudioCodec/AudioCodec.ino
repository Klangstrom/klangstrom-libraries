/**
 * this example demonstrates how to setup the audio code and generate a sine wave sound.
 */

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "AudioDevice.h"
#include "Wavetable.h"

float     wavetable[512];
Wavetable oscillator{wavetable, 512, 48000};

AudioDevice* audiodevice;

float osc_frequency = 440.0f;

void setup() {
    system_init();

    Wavetable::fill(wavetable, 512, Wavetable::WAVEFORM_SINE);
    oscillator.set_amplitude(0.1f);

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
    oscillator.set_frequency(osc_frequency);
    console_println("frequency: %f", osc_frequency);

    delay(1000);
}

void audioblock(AudioBlock* audio_block) {
    for (int i = 0; i < audio_block->block_size; ++i) {
        float mSample = oscillator.process();
        for (int j = 0; j < audio_block->output_channels; ++j) {
            audio_block->output[j][i] = audio_block->input[j][i] + mSample;
        }
    }
}

// arduino-cli compile -u -b klangstrom:emulator:KLST_EMU:board=KLST_PANDA AudioCodec
// arduino-cli compile -u -b STMicroelectronics:stm32:KLST_PANDA AudioCodec