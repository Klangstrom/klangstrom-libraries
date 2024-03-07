#define MX_OMIT_INIT
#define MX_OMIT_LOOP

#ifdef PLATFORM_FIRMWARE

//#include "main.h"
#include "ApplicationStub.h"

int main(void) {
//    /* USER CODE BEGIN 1 */
//
//    /* USER CODE END 1 */
//
//    /* MPU Configuration--------------------------------------------------------*/
//    MPU_Config();
//    /* Enable the CPU Cache */
//
//    /* Enable I-Cache---------------------------------------------------------*/
//    SCB_EnableICache();
//
//    /* Enable D-Cache---------------------------------------------------------*/
//    SCB_EnableDCache();
//
//    /* MCU Configuration--------------------------------------------------------*/
//
//    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
//    HAL_Init();
//
//    /* USER CODE BEGIN Init */
//
//    /* USER CODE END Init */
//
//    /* Configure the system clock */
//    SystemClock_Config();
//
//    /* Configure the peripherals common clocks */
//    PeriphCommonClock_Config();
//    /* USER CODE BEGIN SysInit */
    setup();
//
//#ifndef MX_OMIT_INIT
//    /* USER CODE END SysInit */
//
//    /* Initialize all configured peripherals */
//    MX_I2C1_Init();
//    MX_USART2_UART_Init();
//    MX_ADC1_Init();
//    MX_ADC2_Init();
//    MX_SPI2_Init();
//    MX_TIM23_Init();
//    MX_TIM12_Init();
//    MX_TIM24_Init();
//    MX_TIM15_Init();
//    /* USER CODE BEGIN 2 */
//#endif
//    /* USER CODE END 2 */
//
//    /* Infinite loop */
//    /* USER CODE BEGIN WHILE */
    while (1) {
//#ifndef MX_OMIT_LOOP
//        /* USER CODE END WHILE */
//        MX_USB_HOST_Process();
//
//        /* USER CODE BEGIN 3 */
//#endif
        loop();
    }
//    /* USER CODE END 3 */
}

#endif // PLATFORM_FIRMWARE
