#ifndef SRC_KLST_PANDA_TOUCH_H_
#define SRC_KLST_PANDA_TOUCH_H_

/* FT5206 capacitive touch panel controller */
/* interfaced via I2C4 ( which is shared with audio codec ) */

void touch_setup();
void touch_read();

#endif /* SRC_KLST_PANDA_TOUCH_H_ */
