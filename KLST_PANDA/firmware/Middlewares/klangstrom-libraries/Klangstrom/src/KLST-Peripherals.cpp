#include "KLST-Peripherals.h"

// TODO move to BSP files

#ifdef BOARD_KLST_PANDA
KLST_Peripherals::KLST_Peripherals() :
        number_of_encoders(2), number_of_mechanicalkeys(2) {
    encoders = new Encoder[number_of_encoders];
    mechanicalkeys = new MechanicalKey[number_of_mechanicalkeys];
}
#elif BOARD_KLST_SHEEP
KLST_Peripherals::KLST_Peripherals() :
        number_of_encoders(0), number_of_mechanicalkeys(0) {
    mechanicalkeys = new MechanicalKey[number_of_mechanicalkeys];
}
#else // for development purposes
KLST_Peripherals::KLST_Peripherals() :
        number_of_encoders(2), number_of_mechanicalkeys(2) {
    encoders = new RotaryEncoder*[number_of_encoders];
    for (uint8_t i = 0; i < number_of_encoders; ++i) {
        encoders[i] = nullptr;
    }

    mechanicalkeys = new MechanicalKey*[number_of_mechanicalkeys];
    for (uint8_t i = 0; i < number_of_mechanicalkeys; ++i) {
        mechanicalkeys[i] = nullptr;
    }
}

KLST_Peripherals::~KLST_Peripherals() {
    for (uint8_t i = 0; i < number_of_encoders; ++i) {
        delete encoders[i];
    }
    delete[] encoders;

    for (uint8_t i = 0; i < number_of_mechanicalkeys; ++i) {
        delete mechanicalkeys[i];
    }
    delete[] mechanicalkeys;
}
#endif
