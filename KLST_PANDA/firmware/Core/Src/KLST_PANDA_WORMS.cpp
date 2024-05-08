// TODO move this into another folder

#if (KLST_BOARD==KLST_BOARD_WORMS)

#include "ApplicationStub.h" // this is important for C++

#include "Klangstrom.h"
#include "KLST-MechanicalKey.h"

Klangstrom klst;
#define MECHANICAL_KEY_00 0
MechanicalKey *mMechanicalKey;

void setup() {
    klst.init();
    mMechanicalKey = klst.create_mechancial_key(MECHANICAL_KEY_00);
    klst.setup();
//    klst.create_encoder(ENCODER_00);
}

void loop() {
    klst.loop();
}

void event(int event_type, uint8_t event_data) {
}

//void WEAK event(int event_type, uint8_t event_data) {}
//void WEAK encoder_event() {}

#endif // (KLST_BOARD==KLST_BOARD_WORMS)
