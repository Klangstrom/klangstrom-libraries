/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2024 Dennis P Paul.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "KLST.h"
#include "KLST_PANDA-Includes.h"
#include "KLST_PANDA.h"
#include "KLST_PANDA-Backlight.h"
#include "KLST_PANDA-ExternalMemory.h"
#include "KLST_PANDA-InternalMemory.h"
#include "KLST_PANDA-LED.h"
#include "KLST_PANDA-LTDC.h"
#include "KLST_PANDA-SerialDebug.h"
#include "KLST_PANDA-Touch.h"
#include "KLST_PANDA-AudioCodec.h"
#include "KLST_PANDA-RotaryEncoder.h"
#include "KLST_PANDA-SDCard.h"
#include "KLST_PANDA-IDC_Serial.h"
#include "KLST_PANDA-MIDI_analog.h"
#include "KLST_PANDA-ADCDAC.h"
#include "KLST_PANDA-OnBoardMic.h"

#include <sys/time.h>
int _gettimeofday(struct timeval *tv, void *tzvp) {
    return 0;
}

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart8;
extern UART_HandleTypeDef huart9;
extern DMA_HandleTypeDef hdma_uart8_rx;

static void KLST_PANDA_MX_Init_Modules();

/* ----------------------------------------------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------------------------------------------- */

static uint32_t frame_counter = 0;

// TODO move KLST_PANDA components to generic ( i.e no conection to STM32 ) classes
// TODO distribute callbacks

void KLST_BSP_init() {
    /* MPU Configuration--------------------------------------------------------*/
    MPU_Config();
    /* Enable the CPU Cache */

    /* Enable I-Cache---------------------------------------------------------*/
    SCB_EnableICache();

    /* Enable D-Cache---------------------------------------------------------*/
    SCB_EnableDCache();

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* Configure the peripherals common clocks */
    PeriphCommonClock_Config();
}

static void KLST_PANDA_MX_Init_Modules() {
    bool mEnabledDMA = false;
    bool mEnabledI2C4 = false;
#ifdef KLST_PANDA_ENABLE_GPIO
    MX_GPIO_Init();
#endif // KLST_PANDA_ENABLE_GPIO

#ifdef KLST_PANDA_ENABLE_MECHANICAL_KEYS
    MX_TIM4_Init();
#endif // KLST_PANDA_ENABLE_MECHANICAL_KEYS

#ifdef KLST_PANDA_ENABLE_SERIAL_DEBUG
    MX_USART3_UART_Init();
#endif // KLST_PANDA_ENABLE_SERIAL_DEBUG

#ifdef KLST_PANDA_ENABLE_EXTERNAL_MEMORY
    MX_OCTOSPI1_Init();
    HAL_Delay(100);
#endif // KLST_PANDA_ENABLE_EXTERNAL_MEMORY

#ifdef KLST_PANDA_ENABLE_ON_BOARD_MIC
//    MX_BDMA_Init();
//    MX_CRC_Init();
//    MX_PDM2PCM_Init();
//    MX_SAI4_Init();
#endif // KLST_PANDA_ENABLE_ON_BOARD_MIC

#ifdef KLST_PANDA_ENABLE_ENCODER
    MX_TIM1_Init();
    MX_TIM2_Init();
#endif // KLST_PANDA_ENABLE_ENCODER

#ifdef KLST_PANDA_ENABLE_SD_CARD
    MX_FATFS_Init();
    MX_SDMMC2_SD_Init();
#endif // KLST_PANDA_ENABLE_SD_CARD

#ifdef KLST_PANDA_ENABLE_IDC_SERIAL
    if (!mEnabledDMA) {
        MX_DMA_Init();
        mEnabledDMA = true;
    }
    MX_UART9_Init();
    MX_UART8_Init();
#endif // KLST_PANDA_ENABLE_IDC_SERIAL

#ifdef KLST_PANDA_ENABLE_MIDI
    if (!mEnabledDMA) {
        MX_DMA_Init();
        mEnabledDMA = true;
    }
    MX_UART4_Init();
#endif // KLST_PANDA_ENABLE_MIDI

#ifdef KLST_PANDA_ENABLE_USB_HOST
    MX_USB_HOST_Init();
#endif // KLST_PANDA_ENABLE_USB_HOST

#ifdef KLST_PANDA_ENABLE_USB_DEVICE
    MX_USB_DEVICE_Init();
#endif // KLST_PANDA_ENABLE_USB_DEVICE

#ifdef KLST_PANDA_ENABLE_ADC_DAC
    MX_ADC3_Init();
    MX_DAC1_Init();
#endif // KLST_PANDA_ENABLE_ADC_DAC

#ifdef KLST_PANDA_ENABLE_DISPLAY
    /* display+backlight+touch panel */
    MX_LTDC_Init();
    MX_DMA2D_Init();
    MX_TIM3_Init();
    if (!mEnabledI2C4) {
        MX_I2C4_Init();
        mEnabledI2C4 = true;
    }
#else
    /* turn display and backlight off when display is not used */
    static const uint8_t _DISPLAY_ON_OFF_Pin_ID = 4; // PC4
    GPIOC->MODER &= ~(0x3 << (_DISPLAY_ON_OFF_Pin_ID * 2));
    GPIOC->MODER |= ((1 & 0x3) << (_DISPLAY_ON_OFF_Pin_ID * 2));
    GPIOC->OTYPER &= ~(1 << _DISPLAY_ON_OFF_Pin_ID);
    //  GPIOC->ODR    |=  (  1 << _DISPLAY_ON_OFF_Pin_ID); // HIGH
    //  GPIOC->ODR    &= ~(  1 << _DISPLAY_ON_OFF_Pin_ID); // LOW
    HAL_GPIO_WritePin(_DISPLAY_ON_OFF_GPIO_Port, _DISPLAY_ON_OFF_Pin, GPIO_PIN_RESET);

    __HAL_RCC_GPIOC_CLK_ENABLE();
    static const uint8_t _DISPLAY_BACKLIGHT_PWM_Pin_ID = 8; // PC8
    GPIOC->MODER &= ~(0x3 << (_DISPLAY_BACKLIGHT_PWM_Pin_ID * 2));
    GPIOC->MODER |= ((1 & 0x3) << (_DISPLAY_BACKLIGHT_PWM_Pin_ID * 2));
    GPIOC->OTYPER &= ~(1 << _DISPLAY_BACKLIGHT_PWM_Pin_ID);
    //  GPIOC->ODR    |=  (  1 << _DISPLAY_BACKLIGHT_PWM_Pin_ID); // HIGH
    //  GPIOC->ODR    &= ~(  1 << _DISPLAY_BACKLIGHT_PWM_Pin_ID); // LOW
    HAL_GPIO_WritePin(_DISPLAY_BACKLIGHT_PWM_GPIO_Port, _DISPLAY_BACKLIGHT_PWM_Pin, GPIO_PIN_RESET);

    //    // TODO a bit of a hack to turn off the display backlight right away at startup
    //    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
    //    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
#endif // KLST_PANDA_ENABLE_DISPLAY

#ifdef KLST_PANDA_ENABLE_AUDIOCODEC
    if (!mEnabledDMA) {
        MX_DMA_Init();
        mEnabledDMA = true;
    }
    if (!mEnabledI2C4) {
        MX_I2C4_Init();
        mEnabledI2C4 = true;
    }
    MX_SAI1_Init();
#endif // KLST_PANDA_ENABLE_AUDIOCODEC
}

void KLST_BSP_setup() {
    KLST_PANDA_MX_Init_Modules();

    /* --- serial debug (USART3)*/
#ifdef KLST_PANDA_ENABLE_SERIAL_DEBUG
    serialdebug_setup();
#endif // KLST_PANDA_ENABLE_SERIAL_DEBUG

#ifdef KLST_PANDA_ENABLE_MECHANICAL_KEYS
    /* --- TODO move mechanical keys to file */
    println("initializing mechanical keys (MX:TIM4)");
    HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_4);
#endif // KLST_PANDA_ENABLE_MECHANICAL_KEYS

#ifdef KLST_PANDA_ENABLE_GPIO
    println("initializing GPIO");
    println("initializing LEDs");
    LED_setup();
    LED_turn_off(LED_00);
    LED_turn_off(LED_01);
#endif // KLST_PANDA_ENABLE_GPIO

#ifdef KLST_PANDA_ENABLE_EXTERNAL_MEMORY
    println("initializing external RAM (MX:OCTOSPI1)");
    externalmemory_setup();

    println("testing external RAM");
    externalmemory_test();

#ifdef KLST_PANDA_TEST_INTERNAL_MEMORY
println("test internal RAM");
internalmemory_test_all();
#endif
#endif // KLST_PANDA_ENABLE_EXTERNAL_MEMORY

#ifdef KLST_PANDA_ENABLE_ON_BOARD_MIC
    println("initializing MEMS microphones (MX:BDMA+CRC+PDM2PCM+SAI4)");
    onboardmic_setup();
#endif // KLST_PANDA_ENABLE_ON_BOARD_MIC

#ifdef KLST_PANDA_ENABLE_ENCODER
    println("initializing rotary encoders (MX:TIM1+TIM2)");
    if (peripherals.encoders[0] != nullptr) {
        peripherals.encoders[0]->setup(); // TODO implement
    }
    /* --- TODO move encoder to file */
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
    HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_3);

    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_4);
#endif // KLST_PANDA_ENABLE_ENCODER

#ifdef KLST_PANDA_ENABLE_SD_CARD
    /* --- SD card (SDMMC) */
    println("initializing SD card (SDMMC) (MX:FATFS+SDMMC2_SD)");
    sdcard_setup();
    sdcard_check_status();
    sdcard_write_test_file(false);
#endif // KLST_PANDA_ENABLE_SD_CARD

#ifdef KLST_PANDA_ENABLE_IDC_SERIAL
    println("initializing IDC serial (MX:DMA+UART8+UART9)");
    IDC_serial_setup();
#endif // KLST_PANDA_ENABLE_IDC_SERIAL

#ifdef KLST_PANDA_ENABLE_MIDI
    println("initializing MIDI analog (MX:DMA+UART4)");
    MIDI_analog_setup();
#endif // KLST_PANDA_ENABLE_MIDI

#ifdef KLST_PANDA_ENABLE_ADC_DAC
    println("initializing ADC + DAC (MX:ADC3+DAC1)");
    ADC_setup();
    DAC_setup();
#endif // KLST_PANDA_ENABLE_ADC_DAC

//#if defined(KLST_PANDA_ENABLE_DISPLAY) || defined(KLST_PANDA_ENABLE_AUDIOCODEC)
//    TODO somehow this causes issues
//    println("looking for I2C devices on I2C4");
//    print_I2C_show_devices(&hi2c4);
//#endif

#ifdef KLST_PANDA_ENABLE_DISPLAY
    println("initializing display (LTDC) (MX:LTDC+DMA2D)");
    display_switch_off();
    LTDC_setup();

    /* --- touch panel */

    println("initializing touch panel (FT5206) (MX:I2C4)");
    display_switch_on(); // TODO maybe turn off?
    touch_setup();
//    touch_read();

    /* --- backlight */

    println("initializing LCD backlight PWM (MX:TIM3)");
    backlight_setup();
    backlight_set_brightness(0.5f);
#endif // KLST_PANDA_ENABLE_DISPLAY

#ifdef KLST_PANDA_ENABLE_AUDIOCODEC
    println("initializing audiocodec (WM8904) (MX:DMA+SAI1+I2C4)");
    audiocodec_setup();
#endif // KLST_PANDA_ENABLE_AUDIOCODEC

//    printf("***** WARNING REMOVE THE LINES BELOW *****");
//    printf("***** TESTING MIDI ANALOG UART *****");
//    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
//    /*Configure GPIO pin : _MIDI_ANALOG_OUT_Pin */
//    GPIO_InitStruct.Pin = _MIDI_ANALOG_OUT_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    HAL_GPIO_Init(_MIDI_ANALOG_OUT_GPIO_Port, &GPIO_InitStruct);
//    /*Configure GPIO pin : _MIDI_ANALOG_IN_Pin */
//    GPIO_InitStruct.Pin = _MIDI_ANALOG_IN_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    HAL_GPIO_Init(_MIDI_ANALOG_IN_GPIO_Port, &GPIO_InitStruct);

    /* --- --------------------- --- */
    /* --- end setup, begin loop --- */
    /* --- --------------------- --- */
    println("");
    println("begin loop");
    println("");
}

void KLST_BSP_loop() {
    frame_counter++;
#ifdef KLST_PANDA_ENABLE_DISPLAY
    LTDC_loop();
#endif // KLST_PANDA_ENABLE_DISPLAY

#ifdef KLST_PANDA_ENABLE_IDC_SERIAL
    IDC_serial_loop();
#endif // KLST_PANDA_ENABLE_IDC_SERIAL

#ifdef KLST_PANDA_ENABLE_MIDI
    MIDI_analog_loop();
#endif // KLST_PANDA_ENABLE_MIDI

#ifdef KLST_PANDA_ENABLE_ENCODER
    println("ENCODER_00: %i", ((TIM1->CNT) >> 2));
    println("ENCODER_01: %i", ((TIM2->CNT) >> 2));
#endif // KLST_PANDA_ENABLE_ENCODER

#ifdef KLST_PANDA_ENABLE_ADC_DAC
    static const uint8_t mDACPeriod = 25;
    const float mDACvalue = (float) (frame_counter % mDACPeriod) / (float) mDACPeriod;
    DAC_write(mDACvalue);
    float mADCValue = ADC_read();
    println("ADC: %f", mADCValue);
#endif // KLST_PANDA_ENABLE_ADC_DAC

#ifdef KLST_PANDA_ENABLE_GPIO
    LED_toggle(LED_00);
    LED_toggle(LED_01);
#endif // KLST_PANDA_ENABLE_GPIO

#ifdef KLST_PANDA_ENABLE_ON_BOARD_MIC
    onboardmic_loop();
#endif // KLST_PANDA_ENABLE_ON_BOARD_MIC

    // TODO make LED task
#ifdef KLST_PANDA_ENABLE_USB_HOST
    MX_USB_HOST_Process();
    HAL_Delay(10);
#else
    println("EOF");
    HAL_Delay(500);
#endif // KLST_PANDA_ENABLE_USB_HOST
}

/* ----------------------------------------------------------------------------------------------------------------- */
/* --- CALLBACKS --------------------------------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------------------------------------- */

// void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {}
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
//#define  HAL_UART_ERROR_NONE             (0x00000000U)    /*!< No error                */
//#define  HAL_UART_ERROR_PE               (0x00000001U)    /*!< Parity error            */
//#define  HAL_UART_ERROR_NE               (0x00000002U)    /*!< Noise error             */
//#define  HAL_UART_ERROR_FE               (0x00000004U)    /*!< Frame error             */
//#define  HAL_UART_ERROR_ORE              (0x00000008U)    /*!< Overrun error           */
//#define  HAL_UART_ERROR_DMA              (0x00000010U)    /*!< DMA transfer error      */
//#define  HAL_UART_ERROR_RTO              (0x00000020U)    /*!< Receiver Timeout error  */
    println("HAL_UART_ErrorCallback: %i", huart->ErrorCode);
    if (huart->Instance == UART9) {
        println("UART9");
        RX_00_counter = 0;
        IDC_serial_handle_rx(UART9, 0);
        return;
    }
    if (huart->Instance == UART8) {
        IDC_serial_handle_rx(UART8, 0);
        return;
    }
    if (huart->Instance == UART4) {
        println("UART4");
        MIDI_analog_handle_start_receive();
        return;
    }

    println("unknown serial port");
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    LED_toggle(LED_01);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == UART9) {
        RX_00_buffer[RX_00_counter] = IDC_serial_handle_rx(UART9, 0);
        RX_00_counter++;
        return;
    }
//    if (huart->Instance == UART8) {
//        RX_01_buffer[RX_01_counter] = IDC_serial_handle_rx(UART8, 0);
//        RX_01_counter++;
//        return;
//    }

//    if (huart->Instance == UART4) { // (huart == &huart4)
//        RX_MIDI_buffer[RX_MIDI_counter] = IDC_serial_handle_rx(UART4, 0);
//        RX_MIDI_counter++;
//        return;
//    }

    println("unknown serial port");
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    if (huart->Instance == UART8) {
        IDC_serial_handle_rx(UART8, Size);
        return;
    }
    if (huart->Instance == UART9) {
        // TODO implement UART9 as DMA
        return;
    }
    if (huart->Instance == UART4) {
        MIDI_analog_handle_rx(Size);
        return;
    }
    println("unknown serial port");
}

#ifdef KLST_PANDA_ENABLE_MECHANICAL_KEYS
// TODO move to own file
static bool MECH_00_state = false;
static bool MECH_01_state = false;
#endif // KLST_PANDA_ENABLE_MECHANICAL_KEYS

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
#ifdef KLST_PANDA_ENABLE_MECHANICAL_KEYS
    if (htim == &htim4) {
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4) {
            MECH_01_state = !HAL_GPIO_ReadPin(_MECH_BUTTON_01_GPIO_Port, _MECH_BUTTON_01_Pin);
            println("MECH_01: %s", (MECH_01_state ? "DOWN" : "UP"));
        }
    }
#endif
#ifdef KLST_PANDA_ENABLE_ENCODER
    if (htim == &htim1) {
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3) {
            println("ENCODER_00: BUTTON");
        }
    }
    if (htim == &htim2) {
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4) {
            println("ENCODER_01: BUTTON");
        }
    }
#endif // KLST_PANDA_ENABLE_ENCODER
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
#ifdef KLST_PANDA_ENABLE_MECHANICAL_KEYS
    if (GPIO_Pin == _MECH_BUTTON_00_Pin) {
        MECH_00_state = !HAL_GPIO_ReadPin(_MECH_BUTTON_00_GPIO_Port, _MECH_BUTTON_00_Pin);
        println("MECH_00: %s", (MECH_00_state ? "DOWN" : "UP"));
    }
#endif // KLST_PANDA_ENABLE_MECHANICAL_KEYS
#ifdef KLST_PANDA_ENABLE_DISPLAY
    if (GPIO_Pin == _DISPLAY_TOUCH_INTERRUPT_Pin) {
        println("TOUCH");
    }
#endif // KLST_PANDA_ENABLE_DISPLAY
}

void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai) {
#ifdef KLST_PANDA_ENABLE_AUDIOCODEC
    audiocodec_TX_full_complete_callback(hsai);
#endif // KLST_PANDA_ENABLE_AUDIOCODEC
}

void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai) {
#ifdef KLST_PANDA_ENABLE_AUDIOCODEC
    audiocodec_TX_half_complete_callback(hsai);
#endif // KLST_PANDA_ENABLE_AUDIOCODEC
}

void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai) {
#ifdef KLST_PANDA_ENABLE_AUDIOCODEC
    audiocodec_RX_full_complete_callback(hsai);
#endif // KLST_PANDA_ENABLE_AUDIOCODEC
#ifdef KLST_PANDA_ENABLE_ON_BOARD_MIC
    onboardmic_RX_full_complete_callback(hsai);
#endif // KLST_PANDA_ENABLE_ON_BOARD_MIC
}

void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai) {
#ifdef KLST_PANDA_ENABLE_AUDIOCODEC
    audiocodec_RX_half_complete_callback(hsai);
#endif // KLST_PANDA_ENABLE_AUDIOCODEC
#ifdef KLST_PANDA_ENABLE_ON_BOARD_MIC
    onboardmic_RX_half_complete_callback(hsai);
#endif // KLST_PANDA_ENABLE_ON_BOARD_MIC
}

void HAL_SAI_ErrorCallback(SAI_HandleTypeDef *hsai) {
#ifdef KLST_PANDA_ENABLE_AUDIOCODEC
    audiocodec_error_callback(hsai);
#endif // KLST_PANDA_ENABLE_AUDIOCODEC
#ifdef KLST_PANDA_ENABLE_ON_BOARD_MIC
    onboardmic_error_callback(hsai);
#endif // KLST_PANDA_ENABLE_ON_BOARD_MIC
}

#ifdef __cplusplus
}
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

