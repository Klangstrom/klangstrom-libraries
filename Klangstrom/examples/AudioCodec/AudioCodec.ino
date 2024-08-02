#include "Arduino.h"
#include "System.h"
#include "AudioCodec.h"
#include "Wavetable.h"

float     wavetable[512];
Wavetable oscillator{wavetable, 512, 48000};

AudioDevice* audiodevice;

void setup() {
    system_init();

    AudioInfo audioinfo;
    audioinfo.sample_rate     = 48000;
    audioinfo.output_channels = 2;
    audioinfo.input_channels  = 2;
    audioinfo.block_size      = 128;
    audioinfo.bit_depth       = 16;
    audiodevice               = audiocodec_init(&audioinfo);
    if (audioinfo.device_id == AUDIO_DEVICE_INIT_ERROR) {
        // handle error
    }
    audiocodec_start(audiodevice);
}

void loop() {
}

void audioblock(AudioBlock* audio_block) {
    if (audio_block->device_id == 0) {
        for (int i = 0; i < audio_block->block_size; ++i) {
            float mSample = oscillator.process();
            for (int j = 0; j < audio_block->output_channels; ++j) {
                audio_block->output[j][i] = mSample;
            }
        }
    }
}

// arduino-cli compile -u -b klangstrom:emulator:KLST_EMU:board=KLST_PANDA AudioCodec
// arduino-cli compile -u -b STMicroelectronics:stm32:KLST_PANDA AudioCodec