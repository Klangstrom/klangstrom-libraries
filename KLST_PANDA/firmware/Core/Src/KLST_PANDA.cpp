#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
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
#include "KLST_PANDA-MechanicalKey.h"
#include "KLST_PANDA-IDC_Serial.h"

#ifdef KLST_PANDA_ENABLE_SD_CARD
#include "fatfs.h"
#endif // KLST_PANDA_ENABLE_SD_CARD
#ifdef KLST_PANDA_ENABLE_ON_BOARD_MIC
#include "pdm2pcm.h"
#endif // KLST_PANDA_ENABLE_ON_BOARD_MIC
#ifdef KLST_PANDA_ENABLE_USB_HOST
#include "usb_host.h"
#endif // KLST_PANDA_ENABLE_USB_HOST
#ifdef KLST_PANDA_ENABLE_USB_DEVICE
#include "usb_device.h"
#endif // KLST_PANDA_ENABLE_USB_DEVICE

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart8;
extern UART_HandleTypeDef huart9;
extern DMA_HandleTypeDef hdma_uart8_rx;

RotaryEncoder encoder;
MechanicalKey mechanicalkey;

static uint32_t frame_counter = 0;

// TODO move KLST_PANDA components to generic ( i.e no conection to STM32 ) classes
// TODO distribute callbacks

static void KLST_PANDA_MX_Init_Modules() {
    bool mEnabledDMA = false;
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
#endif // KLST_PANDA_ENABLE_EXTERNAL_MEMORY

#ifdef KLST_PANDA_ENABLE_DISPLAY
// display+backlight+touch panel
    MX_LTDC_Init();
    MX_DMA2D_Init();
    MX_TIM3_Init();
    MX_I2C4_Init();
#endif // KLST_PANDA_ENABLE_DISPLAY

#ifdef KLST_PANDA_ENABLE_AUDIOCODEC
    if (!mEnabledDMA) {
        MX_DMA_Init();
        mEnabledDMA = true;
    }
    MX_SAI1_Init();
#endif // KLST_PANDA_ENABLE_AUDIOCODEC

#ifdef KLST_PANDA_ENABLE_ON_BOARD_MIC
    MX_BDMA_Init();
    MX_CRC_Init();
    MX_PDM2PCM_Init();
    MX_SAI4_Init();
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
    MX_UART4_Init();
#endif // KLST_PANDA_ENABLE_IDC_SERIAL

#ifdef KLST_PANDA_ENABLE_USB_HOST
    MX_USB_HOST_Init();
#endif // KLST_PANDA_ENABLE_USB_HOST

#ifdef KLST_PANDA_ENABLE_USB_DEVICE
    MX_USB_DEVICE_Init();
#endif // KLST_PANDA_ENABLE_USB_DEVICE
}

void KLST_PANDA_setup() {
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

#ifdef KLST_PANDA_ENABLE_DISPLAY
    println("initializing display (LTDC) (MX:LTDC+DMA2D)");
    display_switch_off();
    LTDC_setup();

    /* --- backlight */

    println("initializing LCD backlight PWM (MX:TIM3)");
    backlight_setup();
    backlight_set_brightness(0.5f);

    /* --- touch panel */

    println("initializing touch panel (FT5206) (MX:I2C4)");
    display_switch_on(); // TODO maybe turn off?
    touch_setup();
//    touch_read();
#endif // KLST_PANDA_ENABLE_DISPLAY

#ifdef KLST_PANDA_ENABLE_AUDIOCODEC
    println("initializing audiocodec (WM8904) (MX:DMA+SAI1+I2C4)");
    audiocodec_setup();
#endif // KLST_PANDA_ENABLE_AUDIOCODEC

#ifdef KLST_PANDA_ENABLE_ON_BOARD_MIC
    println("initializing MEMS microphones (MX:BDMA+CRC+PDM2PCM+SAI4)");
// TODO move to own context + distribute callbacks
#endif // KLST_PANDA_ENABLE_ON_BOARD_MIC

#ifdef KLST_PANDA_ENABLE_ENCODER
    println("initializing rotary encoders (MX:TIM1+TIM2)");
    encoder.setup(); // TODO implement
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
    println("initializing IDC serial (MX:UART4+UART8+UART9)");
    println("UART: note UART8 is configured for DMA");
    IDC_serial_setup();
#endif // KLST_PANDA_ENABLE_IDC_SERIAL

    /* --- --------------------- --- */
    /* --- end setup, begin loop --- */
    /* --- --------------------- --- */
    println("");
    println("begin loop");
    println("");
    display_switch_on();
}

void KLST_PANDA_loop() {
    frame_counter++;
#ifdef KLST_PANDA_ENABLE_DISPLAY
    LTDC_loop();
#endif // KLST_PANDA_ENABLE_DISPLAY

#ifdef KLST_PANDA_ENABLE_IDC_SERIAL
    IDC_serial_loop();
#endif // KLST_PANDA_ENABLE_IDC_SERIAL

#ifdef KLST_PANDA_ENABLE_ENCODER
    println("ENCODER_00: %i", ((TIM1->CNT) >> 2));
    println("ENCODER_01: %i", ((TIM2->CNT) >> 2));
#endif // KLST_PANDA_ENABLE_ENCODER

    LED_toggle(LED_00);

    // TODO make LED task
#ifdef KLST_PANDA_ENABLE_USB_HOST
    MX_USB_HOST_Process();
    HAL_Delay(10);
#else
    println("EOF");
    HAL_Delay(500);
#endif // KLST_PANDA_ENABLE_USB_HOST
}

/* --- CALLBACKS --- */

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
        IDC_serial_handle_rx(UART9);
    } else if (huart->Instance == UART8) {
//        println("UART8");
//        RX_01_counter = 0;
//        IDC_serial_handle_rx(UART8);
        HAL_UARTEx_ReceiveToIdle_DMA(&huart8, RX_01_DMA_buffer, DMA_BUFFER_SIZE);
        __HAL_DMA_DISABLE_IT(&hdma_uart8_rx, DMA_IT_HT);
    } else if (huart->Instance == UART4) {
        println("UART4");
        RX_MIDI_counter = 0;
        IDC_serial_handle_rx(UART4);
    } else {
        println("unknown serial port");
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    LED_toggle(LED_01);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == UART9) {
        RX_00_buffer[RX_00_counter] = IDC_serial_handle_rx(UART9);
        RX_00_counter++;
//    } else if (huart->Instance == UART8) {
//        RX_01_buffer[RX_01_counter] = IDC_serial_handle_rx(UART8);
//        RX_01_counter++;
    } else if (huart->Instance == UART4) { // (huart == &huart4)
        RX_MIDI_buffer[RX_MIDI_counter] = IDC_serial_handle_rx(UART4);
        RX_MIDI_counter++;
    } else {
        println("unknown serial port");
    }
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

#ifdef __cplusplus
}
#endif

