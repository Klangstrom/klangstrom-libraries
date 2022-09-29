/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_FRAME_BUFFER 0x90000000
#define AUDIOCODEC_01_MCLK_A_Pin GPIO_PIN_2
#define AUDIOCODEC_01_MCLK_A_GPIO_Port GPIOE
#define AUDIOCODEC_00_SD_B_Pin GPIO_PIN_3
#define AUDIOCODEC_00_SD_B_GPIO_Port GPIOE
#define SPI_00_CS_Pin GPIO_PIN_4
#define SPI_00_CS_GPIO_Port GPIOE
#define SPI_00_MISO_Pin GPIO_PIN_5
#define SPI_00_MISO_GPIO_Port GPIOE
#define GPIO_PWM_03_Pin GPIO_PIN_0
#define GPIO_PWM_03_GPIO_Port GPIOF
#define GPIO_PWM_04_Pin GPIO_PIN_1
#define GPIO_PWM_04_GPIO_Port GPIOF
#define GPIO_PWM_05_Pin GPIO_PIN_2
#define GPIO_PWM_05_GPIO_Port GPIOF
#define GPIO_PWM_06_Pin GPIO_PIN_3
#define GPIO_PWM_06_GPIO_Port GPIOF
#define AUDIOCODEC_00_MCLK_B_Pin GPIO_PIN_7
#define AUDIOCODEC_00_MCLK_B_GPIO_Port GPIOF
#define AUDIOCODEC_00_SCK_B_Pin GPIO_PIN_8
#define AUDIOCODEC_00_SCK_B_GPIO_Port GPIOF
#define AUDIOCODEC_00_FS_B_Pin GPIO_PIN_9
#define AUDIOCODEC_00_FS_B_GPIO_Port GPIOF
#define RCC_OSC_IN_Pin GPIO_PIN_0
#define RCC_OSC_IN_GPIO_Port GPIOH
#define RCC_OSC_OUT_Pin GPIO_PIN_1
#define RCC_OSC_OUT_GPIO_Port GPIOH
#define SPI_01_MISO_Pin GPIO_PIN_2
#define SPI_01_MISO_GPIO_Port GPIOC
#define ADC_02_Pin GPIO_PIN_3
#define ADC_02_GPIO_Port GPIOC
#define ENCODER_01_A_Pin GPIO_PIN_0
#define ENCODER_01_A_GPIO_Port GPIOA
#define DAC_00_Pin GPIO_PIN_4
#define DAC_00_GPIO_Port GPIOA
#define ADC_00_Pin GPIO_PIN_4
#define ADC_00_GPIO_Port GPIOC
#define AUDIOCODEC_00_SD_A_Pin GPIO_PIN_2
#define AUDIOCODEC_00_SD_A_GPIO_Port GPIOB
#define AUDIOCODEC_01_SD_B_Pin GPIO_PIN_11
#define AUDIOCODEC_01_SD_B_GPIO_Port GPIOF
#define GPIO_PWM_07_Pin GPIO_PIN_12
#define GPIO_PWM_07_GPIO_Port GPIOF
#define ADC_01_Pin GPIO_PIN_13
#define ADC_01_GPIO_Port GPIOF
#define I2C_01_SCL_Pin GPIO_PIN_14
#define I2C_01_SCL_GPIO_Port GPIOF
#define I2C_01_SDA_Pin GPIO_PIN_15
#define I2C_01_SDA_GPIO_Port GPIOF
#define SERIAL_02_RX_Pin GPIO_PIN_0
#define SERIAL_02_RX_GPIO_Port GPIOG
#define SERIAL_02_TX_Pin GPIO_PIN_1
#define SERIAL_02_TX_GPIO_Port GPIOG
#define ENCODER_00_A_Pin GPIO_PIN_9
#define ENCODER_00_A_GPIO_Port GPIOE
#define ENCODER_00_B_Pin GPIO_PIN_11
#define ENCODER_00_B_GPIO_Port GPIOE
#define SPI_00_SCK_Pin GPIO_PIN_12
#define SPI_00_SCK_GPIO_Port GPIOE
#define SPI_00_MOSI_Pin GPIO_PIN_14
#define SPI_00_MOSI_GPIO_Port GPIOE
#define ENCODER_01_BUTTON_Pin GPIO_PIN_11
#define ENCODER_01_BUTTON_GPIO_Port GPIOB
#define GPIO_PWM_02_Pin GPIO_PIN_14
#define GPIO_PWM_02_GPIO_Port GPIOB
#define SPI_01_MOSI_Pin GPIO_PIN_15
#define SPI_01_MOSI_GPIO_Port GPIOB
#define SERIAL_00_TX_Pin GPIO_PIN_8
#define SERIAL_00_TX_GPIO_Port GPIOD
#define SERIAL_00_RX_Pin GPIO_PIN_9
#define SERIAL_00_RX_GPIO_Port GPIOD
#define AUDIOCODEC_01_SD_A_Pin GPIO_PIN_11
#define AUDIOCODEC_01_SD_A_GPIO_Port GPIOD
#define AUDIOCODEC_01_FS_A_Pin GPIO_PIN_12
#define AUDIOCODEC_01_FS_A_GPIO_Port GPIOD
#define AUDIOCODEC_01_SCK_A_Pin GPIO_PIN_13
#define AUDIOCODEC_01_SCK_A_GPIO_Port GPIOD
#define GPIO_PWM_01_Pin GPIO_PIN_14
#define GPIO_PWM_01_GPIO_Port GPIOD
#define GPIO_PWM_00_Pin GPIO_PIN_15
#define GPIO_PWM_00_GPIO_Port GPIOD
#define CARD_SDMMC_D0_Pin GPIO_PIN_8
#define CARD_SDMMC_D0_GPIO_Port GPIOC
#define CARD_SDMMC_D1_Pin GPIO_PIN_9
#define CARD_SDMMC_D1_GPIO_Port GPIOC
#define USB_DEVICE_HOST_VBUS_Pin GPIO_PIN_9
#define USB_DEVICE_HOST_VBUS_GPIO_Port GPIOA
#define ENCODER_00_BUTTON_Pin GPIO_PIN_10
#define ENCODER_00_BUTTON_GPIO_Port GPIOA
#define USB_DEVICE_HOST_MINUS_Pin GPIO_PIN_11
#define USB_DEVICE_HOST_MINUS_GPIO_Port GPIOA
#define USB_DEVICE_HOST_PLUS_Pin GPIO_PIN_12
#define USB_DEVICE_HOST_PLUS_GPIO_Port GPIOA
#define SWD_IO_Pin GPIO_PIN_13
#define SWD_IO_GPIO_Port GPIOA
#define SWD_CLK_Pin GPIO_PIN_14
#define SWD_CLK_GPIO_Port GPIOA
#define CARD_SDMMC_D2_Pin GPIO_PIN_10
#define CARD_SDMMC_D2_GPIO_Port GPIOC
#define CARD_SDMMC_D3_Pin GPIO_PIN_11
#define CARD_SDMMC_D3_GPIO_Port GPIOC
#define CARD_SDMMC_CK_Pin GPIO_PIN_12
#define CARD_SDMMC_CK_GPIO_Port GPIOC
#define CARD_SDMMC_CMD_Pin GPIO_PIN_2
#define CARD_SDMMC_CMD_GPIO_Port GPIOD
#define SPI_01_SCK_Pin GPIO_PIN_3
#define SPI_01_SCK_GPIO_Port GPIOD
#define SERIAL_DEBUG_TX_Pin GPIO_PIN_5
#define SERIAL_DEBUG_TX_GPIO_Port GPIOD
#define SERIAL_DEBUG_RX_Pin GPIO_PIN_6
#define SERIAL_DEBUG_RX_GPIO_Port GPIOD
#define ENCODER_01_B_Pin GPIO_PIN_3
#define ENCODER_01_B_GPIO_Port GPIOB
#define SPI_01_CS_Pin GPIO_PIN_4
#define SPI_01_CS_GPIO_Port GPIOB
#define I2C_00_SCL_Pin GPIO_PIN_6
#define I2C_00_SCL_GPIO_Port GPIOB
#define I2C_00_SDA_Pin GPIO_PIN_7
#define I2C_00_SDA_GPIO_Port GPIOB
#define SERIAL_01_RX_Pin GPIO_PIN_0
#define SERIAL_01_RX_GPIO_Port GPIOE
#define SERIAL_01_TX_Pin GPIO_PIN_1
#define SERIAL_01_TX_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */
/*
 * – OCTOSPI1 addressable space is from `0x90000000` to `0x9FFFFFFF`.
 * – OCTOSPI2 addressable space is from `0x70000000` to `0x7FFFFFFF`.
 */
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
