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

void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim) {
    println("HAL_TIM_TriggerCallback");
    if (htim == &htim4) {
        println("htim4");
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    println("HAL_GPIO_EXTI_Callback");
}

void KLST_PANDA_setup() {
    /* --- serial debug (USART3)*/
    serialdebug_setup();

    /* --- GPIO+LEDs */
    HAL_TIM_Base_Start_IT(&htim4);

    println("initializing GPIO");
    display_switch_off();

    println("initializing LEDs");
    LED_setup();
    LED_turn_off(LED_00);
    LED_turn_off(LED_01);

    /* --- external/internal RAM */

    println("initializing external RAM (MX:OCTOSPI1)");
    externalmemory_setup();

    println("testing external RAM");
    externalmemory_test();

#ifdef KLST_PANDA_TEST_INTERNAL_MEMORY
  println("test internal RAM");
  internalmemory_test_all();
#endif

    /* --- display (LTDC) */

    println("initializing display (LTDC) (MX:LTDC+DMA2D)");
    LTDC_setup();

    /* --- backlight */

    println("initializing LCD backlight PWM (MX:TIM3)");
    backlight_setup();
    backlight_set_brightness(0.5f);

    /* --- touch panel */

    println("initializing touch panel (FT5206) (MX:I2C4)");
    display_switch_on();
    touch_setup();
//    touch_read();

    /* --- audiocodec */

    println("initializing audiocodec (WM8904) (MX:DMA+SAI1+I2C4)");
    audiocodec_setup();

    /* --- MEMS microphones */

    println("initializing MEMS microphones (MX:BDMA+CRC+PDM2PCM+SAI4)");
    // TODO move to own context + distribute callbacks

    /* --- rotary encoder */

    println("initializing rotary encoders (MX:TIM1+TIM2)");
    encoder.setup(); // TODO implement

    /* --- SD card (SDMMC) */

    println("initializing SD card (SDMMC) (MX:FATFS+SDMMC2_SD)");
    sdcard_setup();
    sdcard_check_status();
    sdcard_write_test_file(false);

    /* --- end setup, begin loop --- */

    println("begin loop");
    display_switch_on();
}

void KLST_PANDA_loop() {
    frame_counter++;
    LED_toggle(LED_00);
    LTDC_loop();
    println("EOF");
    HAL_Delay(500);
}

#ifdef __cplusplus
}
#endif

