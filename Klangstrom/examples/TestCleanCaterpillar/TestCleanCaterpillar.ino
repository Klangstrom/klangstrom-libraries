#include "Arduino.h"

void setup() {
  Serial.begin(115200);
  Serial.println("Hello!");
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  Serial.println("3");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
