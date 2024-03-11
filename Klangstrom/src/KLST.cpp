#include "KLST.h"

KLST_Peripherals peripherals;

Klangstrom::Klangstrom() :
        is_initialized(false) {
}

void Klangstrom::init() {
    if (!is_initialized) {
        KLST_BSP_init();
        is_initialized = true;
    } else {
//        println("Klangstrom: already initialized");
    }
}

void Klangstrom::setup() {
    if (is_initialized) {
        KLST_BSP_setup();
    } else {
//        println("Klangstrom: system needs to be initialized with `init()` before `setup()` is called");
    }
}

void Klangstrom::loop() {
    if (is_initialized) {
        KLST_BSP_loop();
    }
}

MechanicalKey* Klangstrom::create_mechancial_key(int mechanical_key_id) {
    if (mechanical_key_id >= 0 && mechanical_key_id < peripherals.number_of_mechanicalkeys) {
        if (peripherals.mechanicalkeys[mechanical_key_id] != nullptr) {
            delete peripherals.mechanicalkeys[mechanical_key_id];
        }
        peripherals.mechanicalkeys[mechanical_key_id] = new MechanicalKey(mechanical_key_id);
        return peripherals.mechanicalkeys[mechanical_key_id];
    }
    return nullptr;
}

