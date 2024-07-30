#include "Arduino.h"
#include "Klangstrom.h"
#include "KlangstromAudioCodec.h"
#include "KlangstromSerialDebug.h"
#include "KlangstromLEDs.h"
#include "KlangstromSDCard.h"
#include "Wavetable.h"

void audioblock(float** input_signal, float** output_signal, uint16_t length);

SerialDebug console;
Klangstrom  klangstrom;
AudioCodec  audiocodec;
SDCard      sdcard;
LEDs        leds;
uint8_t     fLEDIntensity = 0;

float     wavetable[512];
Wavetable oscillator{wavetable, 512, 48000};

// #define TEST_SD_CARD

void setup() {
    /* init section */
    klangstrom.init();
    console.init();
    console.info();
    console.timestamp();
    console.println("starting init");
    uint8_t mDeviceID = audiocodec.init(48000, 2, 2, 128, 16, AUDIO_DEVICE_KLST_PANDA_AUDIO_CODEC);
    console.println("audio device ID: %i", mDeviceID);
    leds.init(); // TODO interferes with audiocodec
    
#ifdef TEST_SD_CARD
    sdcard.init();
#endif // TEST_SD_CARD

    console.timestamp();
    console.println("finished init");

    /* setup section */
    console.timestamp();
    console.println("starting setup");
    klangstrom.setup();

    Wavetable::fill(wavetable, 512, Wavetable::WAVEFORM_SINE);
    oscillator.set_amplitude(0.1f);

    console.timestamp();
    console.println("finished setup");
    console.println("---------------------------------------------------------");

    audiocodec.start();

#ifdef TEST_SD_CARD
    /* SD Card */
    sdcard.status();

    // sdcard.mount();
    // sdcard.format();

    sdcard.mount();
    std::vector<std::string> files;
    std::vector<std::string> directories;
    sdcard.list("/", files, directories);
    console.println("Files: %i", files.size());
    console.println("Directories: %i", directories.size());
    // sdcard.open("KLST.TXT");
    console.println("---------------------------------------------------------");
#endif // TEST_SD_CARD
}

void loop() {
    console.println("---");

    fLEDIntensity += 10;
    leds.set(0, fLEDIntensity / 255.0);
    leds.toggle(1);
    console.println("LED 0 : %f", leds.get(0)); // println/printf with float might not work
    console.println("LED 1 : %f", leds.get(1));
    oscillator.set_frequency(random(1, 4) * 55);

    klangstrom.loop();
    delay(1000);
}

void audioblock(AudioBlock* audio_block) {
    if (audio_block->device_id == 0) {
        for (int i = 0; i < audio_block->block_size; ++i) {
            for (int j = 0; j < audio_block->output_channels; ++j) {
                audio_block->output[0][i] = oscillator.process();
                audio_block->output[1][i] = audio_block->output[0][i];
            }
        }
    }
}
// arduino-cli compile -b klangstrom:emulator:KLST_EMU:board=KLST_PANDA Test
// arduino-cli compile -b STMicroelectronics:stm32:KLST_PANDA Test