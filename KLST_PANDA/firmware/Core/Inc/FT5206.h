#ifndef INC_FT5206_H_
#define INC_FT5206_H_

#include "stm32h7xx_hal.h"

#define FT5206_I2C_ADDRESS                0x70 // or 0x71? used to be 0x38, why?
#define FT5206_NUMBER_OF_REGISTERS        31 // there are more registers, but this is enough to get all 5 touch coordinates.
#define FT5206_NUMBER_OF_TOTAL_REGISTERS  0xFE
#define FT5206_DEVICE_MODE                0x00

#define FT5206_GEST_ID                    0x01
#define FT5206_GEST_ID_MOVE_UP             0x10
#define FT5206_GEST_ID_MOVE_LEFT           0x14
#define FT5206_GEST_ID_MOVE_DOWN           0x18
#define FT5206_GEST_ID_MOVE_RIGHT          0x1C
#define FT5206_GEST_ID_ZOOM_IN             0x48
#define FT5206_GEST_ID_ZOOM_OUT            0x49
#define FT5206_GEST_ID_NO_GESTURE          0x00

#define FT5206_ID_G_AUTO_CLB_MODE         0xA0
#define FT5206_ID_G_AUTO_CLB_MODE_ENABLE   0x00
#define FT5206_ID_G_AUTO_CLB_MODE_DISABLE  0xFF
#define FS5206_TOUCH_LIB_VERSION_H        0xA1
#define FS5206_TOUCH_LIB_VERSION_L        0xA2
#define FT5206_ID_G_STATE                 0xA7
#define FT5206_ID_G_STATE_CONFIGURE        0x00
#define FT5206_ID_G_STATE_WORK             0x01
#define FT5206_ID_G_STATE_CALIBRATION      0x02
#define FT5206_ID_G_STATE_FACTORY          0x03
#define FT5206_ID_G_STATE_AUTO_CALIBR      0x04

#define FT5206_TD_STATUS                  0x02
#define FT5206_TOUCH_XH                   0x03
#define FT5206_TOUCH_XL                   0x04
#define FT5206_TOUCH_YH                   0x05
#define FT5206_TOUCH_YL                   0x06

void FT5206_init(I2C_HandleTypeDef *hi2c_handle);
void FT5206_read();
void FT5206_print_info();

// TODO(add calibration and touch threshold configuration)

#endif /* INC_FT5206_H_ */
