#include "ApplicationStub.h"

int main(void) {
    setup();
    while (1) {
        loop();
    }
}

#include "KlangstromEnvironment.h"
#ifdef KLST_PANDA_STM32

#include "stm32h7xx_hal.h"

void delay(uint32_t delay_ms) {
    HAL_Delay(delay_ms);
}

#endif // KLST_PANDA_STM32
