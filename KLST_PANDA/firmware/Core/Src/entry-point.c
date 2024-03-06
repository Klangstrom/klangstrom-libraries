#define MX_OMIT_INIT
#define MX_OMIT_LOOP

#include "main.h"
#include "KLST_PANDA-Config.h"
#include "KLST_PANDA.h"

//#include <sys/time.h>
//int _gettimeofday(struct timeval *tv, void *tzvp) {
//    return 0;
//}

int main(void) {
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

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

    /* USER CODE BEGIN SysInit */
    KLST_PANDA_setup();

//    // TODO a bit of a hack to turn off the display backlight right away at startup
//    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
//    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);

#ifndef MX_OMIT_INIT
    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_I2C1_Init();
    MX_USART2_UART_Init();
    MX_ADC1_Init();
    MX_ADC2_Init();
    MX_SPI2_Init();
    MX_TIM23_Init();
    MX_TIM12_Init();
    MX_TIM24_Init();
    MX_TIM15_Init();
    /* USER CODE BEGIN 2 */
#endif
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
#ifndef MX_OMIT_LOOP
        /* USER CODE END WHILE */
        MX_USB_HOST_Process();

        /* USER CODE BEGIN 3 */
#endif
        KLST_PANDA_loop();
    }
    /* USER CODE END 3 */
}

/*
 * TODO after code generation:
 *
 * in `KLST_PANDA/firmware/Core/Src/main.c`
 *
 * - LINE 60: remove `static` from `void MPU_Config(void);`
 *
 * in `KLST_PANDA/firmware/USB_HOST/Target/usbh_conf.h`
 *
 * - LINE 130: add `\r` to `printf("\n\r");`
 * - LINE 141: add `\r` to `printf("\n\r");`
 * - LINE 151: add `\r` to `printf("\n\r");`
 *
 * ( line numbers may vary )
 */
