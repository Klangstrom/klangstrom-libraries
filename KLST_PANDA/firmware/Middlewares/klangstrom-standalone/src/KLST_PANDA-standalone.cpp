#include "KlangstromEnvironment.h"

#ifdef KLST_PANDA_STM32

#include "ApplicationStub.h" // this is important for C++

#include "Klangstrom.h"
#include "KlangstromMechanicalKey.h"
#include "KlangstromAudioCodec.h"
#include "KlangstromSerialDebug.h"
#include "KlangstromLEDs.h"
#include "Wavetable.h"

Klangstrom klangstrom;
AudioCodec audiocodec;
SerialDebug console;
LEDs leds;

float *wavetable = new float[512];
Wavetable oscillator { wavetable, 512, 48000 };

int mFrequency = 27.5;
float mIntensity = 0.0f;

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
}

void loop() {
    klangstrom.loop();
    mFrequency *= 2;
    if (mFrequency > 440) {
        mFrequency = 27.5;
    }
    oscillator.set_frequency(mFrequency);

    mIntensity += 0.1f;
    if (mIntensity > 1.0f) {
        mIntensity = 0.0f;
    }
    leds.set(0, mIntensity);
    leds.toggle(1);
    console.println("LED intensity: %f", leds.get(0));

    console.timestamp();
    console.println("EOF");
    delay(500);
}

void event(int event_type, uint8_t event_data) {
}

//void WEAK event(int event_type, uint8_t event_data) {}
//void WEAK encoder_event() {}

void audioblock(float **input_signal, float **output_signal, uint16_t length) {
    for (int i = 0; i < length; ++i) {
        output_signal[0][i] = oscillator.process();
        output_signal[1][i] = output_signal[0][i];
    }
}

#endif // KLST_PANDA_STM32
