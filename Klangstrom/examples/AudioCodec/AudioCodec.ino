#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "AudioDevice.h"
#include "Wavetable.h"

float     wavetable[512];
Wavetable oscillator{wavetable, 512, 48000};

AudioDevice* audiodevice;

float mFrequency = 440.0f;

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
    audiodevice               = audiodevice_init(&audioinfo);
    if (audiodevice->audioinfo->device_id == AUDIO_DEVICE_INIT_ERROR) {
        console_timestamp();
        console_error("error initializing audio device");
    }
    // ... or for short with default values and auto start
    // audiodevice = system_init_audiocodec();
    // audiodevice_resume(audiodevice);
}

void loop() {
    mFrequency += 10.0f;
    if (mFrequency > 880.0f) {
        mFrequency = 220.0f;
    }
    oscillator.set_frequency(mFrequency);
    console_println("freq: %f", mFrequency);

    delay(1000);
}

void audioblock(AudioBlock* audio_block) {
    for (int i = 0; i < audio_block->block_size; ++i) {
        float mSample = oscillator.process();
        for (int j = 0; j < audio_block->output_channels; ++j) {
            audio_block->output[j][i] = mSample;
        }
    }
}

// arduino-cli compile -u -b klangstrom:emulator:KLST_EMU:board=KLST_PANDA AudioCodec
// arduino-cli compile -u -b STMicroelectronics:stm32:KLST_PANDA AudioCodec