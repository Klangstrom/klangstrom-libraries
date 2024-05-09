#include "KlangstromEnvironment.h"

#ifdef KLST_PANDA_STM32

#include "ApplicationStub.h" // this is important for C++

#include "Klangstrom.h"
#include "KlangstromMechanicalKey.h"
#include "KlangstromAudioCodec.h"

Klangstrom klangstrom;
#define MECHANICAL_KEY_00 0
MechanicalKey *mMechanicalKey;
AudioCodec audiocodec;

void setup() {
    klangstrom.init();
    mMechanicalKey = klangstrom.create_mechancial_key(MECHANICAL_KEY_00);
    klangstrom.setup();
    audiocodec.setup();
//    klst.create_encoder(ENCODER_00);
}

void loop() {
    klangstrom.loop();
}

void event(int event_type, uint8_t event_data) {
}

//void WEAK event(int event_type, uint8_t event_data) {}
//void WEAK encoder_event() {}

#endif // KLST_PANDA_STM32
