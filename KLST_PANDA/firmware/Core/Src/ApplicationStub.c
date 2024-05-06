#if (KLST_BOARD==KLST_BOARD_WORMS)

#include "ApplicationStub.h"

int main(void) {
    setup();
    while (1) {
        loop();
    }
}

#endif // (KLST_BOARD==KLST_BOARD_WORMS)
