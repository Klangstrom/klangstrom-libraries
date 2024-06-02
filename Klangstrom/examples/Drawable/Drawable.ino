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
    klangstrom.init();
    console.init();
    audiocodec.init();
    leds.init();
    klangstrom.setup();

    Wavetable::fill(wavetable, 512, Wavetable::WAVEFORM_SINE);
    oscillator.set_amplitude(0.1);
    audiocodec.start();
}

void loop() {
    leds.toggle(LEDs::ALL);

    klangstrom.loop();
    delay(100);
}

void audioblock(float** input_signal, float** output_signal, uint16_t length) {
    for (int i = 0; i < length; ++i) {
        output_signal[0][i] = oscillator.process();
        output_signal[1][i] = output_signal[0][i];
    }
}