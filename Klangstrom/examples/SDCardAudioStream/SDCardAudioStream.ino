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

const uint16_t buffer_multiple     = 4;
uint16_t       buffer_read_counter = 0;

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

    audio_stream = new AudioStreamFloat32(audiodevice->audioinfo->block_size * buffer_multiple);
    console_timestamp(false);
    console_println("start reading from SD card");
    audio_stream->update();
    console_timestamp(false);
    console_println("done reading from SD card");

    audiodevice_resume(audiodevice);
}

void loop() {
    console_status("...");
    delay(1000);
}

void audioblock(AudioBlock* audio_block) {
    // at 48000 Hz audiorate a data rate of 48000 * 4 = 192000 bytes or 187.5KB per second is required by the SD card
    const int mBufferOffset = audio_block->block_size * buffer_read_counter;
    for (int i = 0; i < audio_block->block_size; ++i) {
        audio_block->output[0][i] = audio_stream->buffer()[i + mBufferOffset];
        audio_block->output[1][i] = audio_stream->buffer()[i + mBufferOffset];
    }
    buffer_read_counter++;
    if (buffer_read_counter >= buffer_multiple) {
        buffer_read_counter = 0;
        audio_stream->update();
    }
}
