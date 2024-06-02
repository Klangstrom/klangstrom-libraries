#include "Arduino.h"
#include "Klangstrom.h"
#include "KlangstromAudioCodec.h"
#include "KlangstromSerialDebug.h"
#include "KlangstromLEDs.h"
#include "Wavetable.h"

void audioblock(float** input_signal, float** output_signal, uint16_t length);

SerialDebug console;
Klangstrom  klangstrom;
AudioCodec  audiocodec;
LEDs        leds;

float*    wavetable = new float[512];
Wavetable oscillator{wavetable, 512, 48000};

void setup() {
    /* init section */
    klangstrom.init();
    console.init();
    console.info();
    console.timestamp();
    console.println("starting init");
    audiocodec.init();
    leds.init();

    console.timestamp();
    console.println("finished init");

    /* setup section */
    console.timestamp();
    console.println("starting setup");
    klangstrom.setup();

    Wavetable::fill(wavetable, 512, Wavetable::WAVEFORM_SINE);

    console.timestamp();
    console.println("finished setup");
    console.println("---------------------------------------------------------");

    audiocodec.start();
}

void loop() {
    leds.toggle(LEDs::ALL);
    console.println("LED   : %f", leds.get(0));
    console.println("micros: %li", micros());
    console.println("sqrt  : %f", sqrt(24));
    console.println("sin   : %f", sin(2));
    console.println("random: %li", random(255));
    console.println("random: %li", random(0, 255));

    klangstrom.loop();
    delay(1000);
}

void audioblock(float** input_signal, float** output_signal, uint16_t length) {
    for (int i = 0; i < length; ++i) {
        output_signal[0][i] = oscillator.process();
        output_signal[1][i] = output_signal[0][i];
    }
}

// arduino-cli compile -b klangstrom:emulator:KLST_EMU:board=KLST_PANDA Test
// arduino-cli compile -b STMicroelectronics:stm32:Klangstrom Test