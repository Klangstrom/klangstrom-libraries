#ifndef INC_APPLICATIONSTUB_H_
#define INC_APPLICATIONSTUB_H_

#ifdef __cplusplus
extern "C" {
#endif

#define WEAK __attribute__((weak))

void setup();
void loop();

#ifdef __cplusplus
}
#endif

#endif /* INC_APPLICATIONSTUB_H_ */
