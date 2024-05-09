#pragma once

#define WEAK __attribute__((weak))

#ifdef __cplusplus
extern "C" {
#endif

void setup();
void loop();

#ifdef __cplusplus
}
#endif
