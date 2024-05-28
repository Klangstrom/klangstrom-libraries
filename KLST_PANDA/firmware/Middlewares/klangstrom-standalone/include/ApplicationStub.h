#pragma once

#include <stdint.h>

#define WEAK __attribute__((weak))

#ifdef __cplusplus
extern "C" {
#endif

void setup();
void loop();
void delay(uint32_t delay_ms);

#ifdef __cplusplus
}
#endif
