#ifndef INC_KLST_PERIPHERALS_H_
#define INC_KLST_PERIPHERALS_H_

#include <stdint.h>

#include "KLST-MechanicalKey.h"
#include "KLST_PANDA-RotaryEncoder.h"

class KLST_Peripherals {
public:
    KLST_Peripherals();
    ~KLST_Peripherals();
    const uint8_t number_of_encoders;
    RotaryEncoder **encoders;
    const uint8_t number_of_mechanicalkeys;
    MechanicalKey **mechanicalkeys;
};

#endif /* INC_KLST_PERIPHERALS_H_ */
