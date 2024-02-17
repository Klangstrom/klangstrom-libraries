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

RotaryEncoder encoder;

static uint32_t frame_counter = 0;

// TODO move KLST_PANDA components to generic ( i.e no conection to STM32 ) classes
// TODO distribute callbacks

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

    println("initializing external RAM (MX:OCTOSPI1)");
    externalmemory_setup();

    println("testing external RAM");
    externalmemory_test();

#ifdef KLST_PANDA_TEST_INTERNAL_MEMORY
  println("test internal RAM");
  internalmemory_test_all();
#endif

    /* --- LTDC+DMA2D */

    println("initializing LCD (MX:LTDC+DMA2D)");
    LTDC_setup();

    /* --- backlight */

    println("initializing LCD backlight PWM (MX:TIM3)");
    backlight_setup();
    backlight_set_brightness(0.5f);

    /* --- touch panel ( requires I2C4 ) */

    println("initializing touch panel (FT5206) (MX:I2C4)");
    display_switch_on();
    touch_setup();
//    touch_read();

    /* --- audiocodec ( requires I2C4 + SAI1 ) */

    println("initializing audiocodec (WM8904) (MX:DMA+SAI1)");
    audiocodec_setup();

    /* --- MEMS microphones ( requires SAI4 ) */

    println("initializing MEMS microphones (MX:SAI4)");
    // TODO move to own context + distribute callbacks

    /* --- MEMS microphones ( requires SAI4 ) */

    println("initializing rotary encoders (MX:TIM1+TIM2)");
    encoder.setup();

    /* --- */
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

