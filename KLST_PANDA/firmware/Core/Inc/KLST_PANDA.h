#ifndef INC_KLST_PANDA_H_
#define INC_KLST_PANDA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "KLST_PANDA-Config.h"

#if defined(KLST_PANDA_ENABLE_USB_HOST) && defined(KLST_PANDA_ENABLE_USB_DEVICE)
#error "KLST_PANDA: USB Host and Device cannot be enabled at the same time"
#endif

#if !defined(KLST_PANDA_ENABLE_EXTERNAL_MEMORY) && defined(KLST_PANDA_ENABLE_DISPLAY)
#error "KLST_PANDA: external memory must be enabled when display is enabled"
#endif

#include "Klangstrom.h"

#ifdef __cplusplus
}
#endif

#endif /* INC_KLST_PANDA_H_ */
