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

extern TIM_HandleTypeDef htim4;

RotaryEncoder encoder;

static uint32_t frame_counter = 0;

// TODO move KLST_PANDA components to generic ( i.e no conection to STM32 ) classes
// TODO distribute callbacks

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (htim == &htim4) {
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4) {
            println("MECH_01");
        }
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == _MECH_BUTTON_00_Pin) {
        println("MECH_00");
    }
    if (GPIO_Pin == _DISPLAY_TOUCH_INTERRUPT_Pin) {
        println("TOUCH");
    }
}

void KLST_PANDA_setup() {
    /* --- serial debug (USART3)*/
#ifdef KLST_PANDA_ENABLE_SERIAL_DEBUG
    serialdebug_setup();
#endif // KLST_PANDA_ENABLE_SERIAL_DEBUG

    /* --- TODO move mechanical keys to file */
    HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_4);

    /* --- GPIO+LEDs */
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
#endif // KLST_PANDA_ENABLE_ENCODER

#ifdef KLST_PANDA_ENABLE_SD_CARD
    /* --- SD card (SDMMC) */
    println("initializing SD card (SDMMC) (MX:FATFS+SDMMC2_SD)");
    sdcard_setup();
    sdcard_check_status();
    sdcard_write_test_file(false);
#endif // KLST_PANDA_ENABLE_SD_CARD

    /* --- end setup, begin loop --- */
    println("");
    println("begin loop");
    println("");
    display_switch_on();
}

void KLST_PANDA_loop() {
    frame_counter++;
    LED_toggle(LED_00);
#ifdef KLST_PANDA_ENABLE_DISPLAY
    LTDC_loop();
#endif // KLST_PANDA_ENABLE_DISPLAY
    println("EOF");
    HAL_Delay(500);
}

#ifdef __cplusplus
}
#endif

