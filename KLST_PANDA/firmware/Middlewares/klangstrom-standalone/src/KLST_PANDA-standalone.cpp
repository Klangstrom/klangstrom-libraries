#include "KlangstromEnvironment.h"

#ifdef KLST_PANDA_STM32

#include "ApplicationStub.h" // this is important for C++

#include "Klangstrom.h"
#include "KlangstromMechanicalKey.h"
#include "KlangstromAudioCodec.h"
#include "KlangstromSerialDebug.h"
#include "Wavetable.h"

void audioblock(float **input_signal, float **output_signal, uint16_t length);

Klangstrom klangstrom;
AudioCodec audiocodec;
SerialDebug console;

float *wavetable = new float[512];
Wavetable oscillator { wavetable, 512, 48000 };

int mFrequency = 110;

void setup() {
    /* init section */
    klangstrom.init();
    console.init();
    console.info();

    console.timestamp();
    console.println("starting init");
    audiocodec.init();

    console.timestamp();
    console.println("finished init");

    /* setup section */
    console.timestamp();
    console.println("starting setup");
    klangstrom.setup();
    audiocodec.register_audioblock(audioblock);

    Wavetable::fill(wavetable, 512, Wavetable::WAVEFORM_SINE);

    console.timestamp();
    console.println("finished setup");
    console.println("---------------------------------------------------------");
}

void loop() {
    klangstrom.loop();
    mFrequency *= 1.11;
    if (mFrequency > 440) {
        mFrequency = 110;
    }
    oscillator.set_frequency(mFrequency);
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
