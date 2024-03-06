#include "ApplicationStub.h" // this is important for C++
#include "KLST_PANDA.h"

Klangstrom klst;

void setup() {
    klst.setup();
//    klst.create_encoder(ENCODER_00);
}

void loop() {
    klst.loop();
}

//void WEAK encoder_event() {}
