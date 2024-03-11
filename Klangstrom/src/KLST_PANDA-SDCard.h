#ifndef INC_KLST_PANDA_SDCARD_H_
#define INC_KLST_PANDA_SDCARD_H_

#include <stdbool.h>

void sdcard_setup();
bool sdcard_detected();
void sdcard_check_status();
void sdcard_write_test_file(bool format_volume);

#endif /* INC_KLST_PANDA_SDCARD_H_ */
