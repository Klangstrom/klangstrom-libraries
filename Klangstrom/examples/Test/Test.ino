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
#include "Wavetable.h"

void audioblock(float **input_signal, float **output_signal, uint16_t length);

Klangstrom klangstrom;
AudioCodec audiocodec;
SerialDebug console;

float *wavetable = new float[512];
Wavetable oscillator{ wavetable, 512, 48000 };

void setup() {
  pinMode(PB14, OUTPUT);

  klangstrom.init();
  klangstrom.setup();

  console.info();

  audiocodec.setup();
  audiocodec.register_audioblock(audioblock);

  Wavetable::fill(wavetable, 512, Wavetable::WAVEFORM_SINE);
  oscillator.set_frequency(110);

  console.println("finished setup");
  console.timestamp();
  console.println("ARDUINO IDE");
}

void loop() {
  digitalWrite(PB14, HIGH);
  oscillator.set_frequency(110);
  delay(1000);
  digitalWrite(PB14, LOW);
  oscillator.set_frequency(220);
  delay(1000);
  klangstrom.loop();
}

void audioblock(float **input_signal, float **output_signal, uint16_t length) {
  for (int i = 0; i < length; ++i) {
    output_signal[0][i] = oscillator.process();
    output_signal[1][i] = output_signal[0][i];
  }
}
