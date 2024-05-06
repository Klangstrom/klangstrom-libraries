#if (KLST_BOARD==KLST_BOARD_WORMS)

#ifndef INC_APPLICATIONSTUB_H_
#define INC_APPLICATIONSTUB_H_

#define WEAK __attribute__((weak))

#ifdef __cplusplus
extern "C" {
#endif

void setup();
void loop();

#ifdef __cplusplus
}
#endif

#endif /* INC_APPLICATIONSTUB_H_ */

#endif // (KLST_BOARD==KLST_BOARD_WORMS)
