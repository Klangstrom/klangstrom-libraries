#ifdef KLST_ENV
#warning KLST_ENV is defined as ...
#if (KLST_ENV == 0x36)
#warning... 0x36 aka KLST_PANDA_STM32
#endif
#else
#warning KLST_ENV not defined
#endif

#include "Klangstrom.h"
#include "KlangstromAudioCodec.h"
#include "KlangstromSerialDebug.h"
#include "KlangstromLEDs.h"
#include "Wavetable.h"

void audioblock(float **input_signal, float **output_signal, uint16_t length);

SerialDebug console;
Klangstrom klangstrom;
AudioCodec audiocodec;
LEDs leds;

float *wavetable = new float[512];
Wavetable oscillator{ wavetable, 512, 48000 };

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
  leds.toggle(LEDs::ALL);
  console.println("LED: %d", leds.get(0));

  klangstrom.loop();
  delay(100);
}

void audioblock(float **input_signal, float **output_signal, uint16_t length) {
  for (int i = 0; i < length; ++i) {
    output_signal[0][i] = oscillator.process();
    output_signal[1][i] = output_signal[0][i];
  }
}
