#ifndef INC_KLST_PANDA_INCLUDES_H_
#define INC_KLST_PANDA_INCLUDES_H_

#include "KLST_PANDA-Config.h"

#if defined(KLST_PANDA_ENABLE_GPIO) || defined(KLST_PANDA_ENABLE_MECHANICAL_KEYS) || defined(KLST_PANDA_ENABLE_SD_CARD) || defined(KLST_PANDA_ENABLE_DISPLAY)
#include "gpio.h"
#endif

#if defined(KLST_PANDA_ENABLE_MECHANICAL_KEYS) || defined(KLST_PANDA_ENABLE_ENCODER) || defined(KLST_PANDA_ENABLE_DISPLAY)
#include "tim.h"
#endif

#if defined(KLST_PANDA_ENABLE_SERIAL_DEBUG) || defined(KLST_PANDA_ENABLE_IDC_SERIAL) || defined(KLST_PANDA_ENABLE_MIDI)
#include "usart.h"
#endif

#if defined(KLST_PANDA_ENABLE_EXTERNAL_MEMORY)
#include "octospi.h"
#endif

#if defined(KLST_PANDA_ENABLE_AUDIOCODEC) || defined(KLST_PANDA_ENABLE_IDC_SERIAL) || defined(KLST_PANDA_ENABLE_MIDI)
#include "dma.h"
#endif

#if defined(KLST_PANDA_ENABLE_AUDIOCODEC) || defined(KLST_PANDA_ENABLE_DISPLAY)
#include "i2c.h"
#endif

#if defined(KLST_PANDA_ENABLE_AUDIOCODEC) || defined(KLST_PANDA_ENABLE_ON_BOARD_MIC)
#include "sai.h"
#endif

#ifdef KLST_PANDA_ENABLE_ON_BOARD_MIC
//#include "pdm2pcm.h"
//#include "crc.h"
#endif

#ifdef KLST_PANDA_ENABLE_SD_CARD
#include "sdmmc.h"
#include "fatfs.h"
#endif

#ifdef KLST_PANDA_ENABLE_ADC_DAC
#include "adc.h"
#include "dac.h"
#endif

#ifdef KLST_PANDA_ENABLE_DISPLAY
#include "dma2D.h"
#include "ltdc.h"
#endif

#ifdef KLST_PANDA_ENABLE_USB_HOST
#include "usb_host.h"
#endif

#ifdef KLST_PANDA_ENABLE_USB_DEVICE
#include "usb_device.h"
#endif

#endif /* INC_KLST_PANDA_INCLUDES_H_ */
