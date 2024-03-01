#ifndef INC_KLST_PANDA_AUDIOCODEC_H_
#define INC_KLST_PANDA_AUDIOCODEC_H_

#include "stdint.h"

uint8_t audiocodec_setup();
void audiocodec_TX_full_complete_callback(SAI_HandleTypeDef *hsai);
void audiocodec_TX_half_complete_callback(SAI_HandleTypeDef *hsai);
void audiocodec_RX_full_complete_callback(SAI_HandleTypeDef *hsai);
void audiocodec_RX_half_complete_callback(SAI_HandleTypeDef *hsai);
void audiocodec_error_callback(SAI_HandleTypeDef *hsai);

#endif /* INC_KLST_PANDA_AUDIOCODEC_H_ */
