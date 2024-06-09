#include "Arduino.h"

void setup() {
  Serial.begin(115200);
  Serial8.begin(115200);
  Serial9.begin(115200);
  Serial.println("Hello!");
#ifdef ENABLE_HWSERIAL4
  Serial.println("MIDI Analog");
#endif  // ENABLE_HWSERIAL1
#ifdef ENABLE_HWSERIAL4
  Serial.println("MIDI Analog");
#endif  // ENABLE_HWSERIAL1
#ifdef ENABLE_HWSERIAL4
  Serial.println("MIDI Analog");
#endif  // ENABLE_HWSERIAL1
#ifdef ENABLE_HWSERIAL4
  Serial.println("MIDI Analog");
#endif  // ENABLE_HWSERIAL1
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  Serial.println("3");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
