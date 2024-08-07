/**
 * this example demonstrates how to use an SD card.
*/

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "SDCard.h"
#include "AudioDevice.h"
#include "AudioStreamFloat32.h"

AudioDevice*        audiodevice;
AudioStreamFloat32* audio_stream;
bool                update_buffer = true;
AudioBlock*         audio_block   = nullptr;

void setup() {
    system_init();
    console_init();
    sdcard_init();

    if (sdcard_detected()) {
        sdcard_mount();
        sdcard_file_open("TEILCHEN.RAW", FILE_READ_ONLY);
    }

    audiodevice = system_init_audiocodec();
    audiodevice_pause(audiodevice);

    audio_stream = new AudioStreamFloat32(audiodevice->audioinfo->block_size);

    audiodevice_resume(audiodevice);
}

void loop() {
    //    console_status("...");
    //    delay(1000);
    if (update_buffer) {
        audio_stream->update();
        for (int i = 0; i < audio_block->block_size; ++i) {
            audio_block->output[0][i] = audio_stream->buffer()[i];
            audio_block->output[1][i] = audio_stream->buffer()[i];
        }
        update_buffer = false;
    }
}

void audioblock(AudioBlock* _audio_block) {
    update_buffer = true;
    audio_block   = _audio_block;
}