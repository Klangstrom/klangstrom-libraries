#ifndef INC_KLST_PANDA_ONBOARDMIC_H_
#define INC_KLST_PANDA_ONBOARDMIC_H_

#include "stdint.h"
#include "main.h"

uint8_t onboardmic_setup();
void onboardmic_loop();
void onboardmic_RX_full_complete_callback(SAI_HandleTypeDef *hsai);
void onboardmic_RX_half_complete_callback(SAI_HandleTypeDef *hsai);
void onboardmic_error_callback(SAI_HandleTypeDef *hsai);

#endif /* INC_KLST_PANDA_ONBOARDMIC_H_ */
