#ifndef INC_KLST_PANDA_H_
#define INC_KLST_PANDA_H_

#include "KLST_PANDA-Config.h"

#if defined( KLST_PANDA_ENABLE_USB_HOST) && defined (KLST_PANDA_ENABLE_USB_DEVICE)
#error "KLST_PANDA: USB Host and Device cannot be enabled at the same time"
#endif

#ifdef __cplusplus
extern "C" {
#endif

void KLST_PANDA_setup();
void KLST_PANDA_loop();

#ifdef __cplusplus
}
#endif

#endif /* INC_KLST_PANDA_H_ */
