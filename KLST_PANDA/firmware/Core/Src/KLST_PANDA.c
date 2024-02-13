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

static uint32_t frame_counter = 0;

void KLST_PANDA_setup() {
    /* --- serial debug (USART3)*/
    serialdebug_setup();

    /* --- GPIO+LEDs */

    println("initializing GPIO");
    display_switch_off();

    println("initializing LEDs");
    LED_setup();
    LED_turn_off(LED_00);
    LED_turn_off(LED_01);

    /* --- external/internal RAM */

    println("initializing I2C (I2C4)");

    /* --- external/internal RAM */

    println("initializing external RAM (MX:OCTOSPI1)");
    externalmemory_setup();

    println("testing external RAM");
    externalmemory_test();

//  println("test internal RAM");
//  internalmemory_test_all();

    /* --- LTDC+DMA2D */

    println("initializing LCD (MX:LTDC+DMA2D)");
    LTDC_setup();

    /* --- backlight */

    println("initializing LCD backlight PWM (MX:TIM3)");
    backlight_setup();

    /* --- touch panel ( requires I2C4 ) */

    println("initializing touch panel (FT5206)");
    display_switch_on();
//  touch_setup();

    /* --- audiocodec ( requires I2C4 + SAI1 ) */

    println("initializing audiocodec (WM8904) (MX:DMA+SAI1)");
    audiocodec_setup();

    /* --- MEMS microphones ( requires SAI4 ) */
    println("initializing MEMS microphones (MX:SAI4)");

    /* --- */
    println("begin loop");
    display_switch_on();
}

void KLST_PANDA_loop() {
    frame_counter++;
    LED_toggle(LED_00);
    LTDC_loop();
//      touch_read();
    // _DISPLAY_TOUCH_INTERRUPT
//      backlight_set_brightness((float) (frame_counter % 100) / 100.0);
    println("EOF");
    HAL_Delay(500);
}
