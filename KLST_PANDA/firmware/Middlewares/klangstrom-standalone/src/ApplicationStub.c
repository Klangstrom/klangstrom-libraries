#include <stddef.h>
#include <errno.h>
#include <unistd.h>

#include "ApplicationStub.h"

int getentropy(void* buffer, size_t length) {
    buffer = buffer;
    length = length;
    return -ENOSYS;
}

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
