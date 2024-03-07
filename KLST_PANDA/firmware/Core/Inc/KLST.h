#ifndef INC_KLANGSTROM_H_
#define INC_KLANGSTROM_H_

#include <stdint.h>

#include "KLST-Peripherals.h"
#include "KLST-MechanicalKey.h"

class Klangstrom {
public:
    Klangstrom();
    void init();
    void setup();
    void loop();

    /**
     * creates a mechanical key according to
     */
    MechanicalKey* create_mechancial_key(int mechanical_key_id);
private:
    bool is_initialized;
};

/* --- Board Specific Implementations (BSP) --- */

#ifdef __cplusplus
extern "C" {
#endif

void KLST_BSP_init();
void KLST_BSP_setup();
void KLST_BSP_loop();
extern KLST_Peripherals peripherals;

#ifdef __cplusplus
}
#endif

#endif /* INC_KLANGSTROM_H_ */
