# KLANGSTROM / KLST_TINY

KLST_TINY is a small version of the KLST_CORE board:

- featuring *KLANGSTROM* library
- programmable in Arduino IDE via USB ( *DFUse Mode* )
- debugging + communication via *virtual USB Serial Port*

## feature list (20210322)

- STM32F446 MCU with 180MHz, 128KB RAM, 512KB Flash
- WM8731 audio codec with 2× audio DAC + 2× audio ADC ( 16/24BIT )
- 2× ADC ( 12BIT, opt 6 extra channels )
- 2× DAC ( 12BIT )
- 3× UART ( serial )
- 8× GPIO
- 1× USB ( device or host )
- 1× I2C 
- 1× SPI 
- 3× rotary encoders with push buttons ( TIM2, TIM3, TIM8 )
- 3× programmable LEDs + 1× power LED
- 11× timers
- 1× SD card reader
- 1× JTAG/SWD interface ( 10-pin )
- programmer + reset + boot flash buttons
- 4× Mounting Holes

## technical details

### timers

| TIMER | FUNCTION                         |
|-------|----------------------------------|
| TIM1  | (UNUSED)                         |
| TIM2  | ENCODER_02                       |
| TIM3  | ENCODER_00                       |
| TIM4  | (UNUSED)                         |
| TIM5  | BEAT_CALLBACK                    |
| TIM6  | TIMER_TONE                       |
| TIM7  | TIMER_SERVO                      |
| TIM8  | ENCODER_01                       |
| TIM9  | (UNUSED)                         |
| TIM10 | (UNUSED)                         |
| TIM11 | (UNUSED)                         |
| TIM12 | (UNUSED)                         |
| TIM13 | (UNUSED)                         |
| TIM14 | (UNUSED)                         |

#### timer-based PWM alternative PINs

| FUNCTION        | PIN  | TIMER:CHANNEL        |
|-----------------|------|----------------------|
| ADC_00          | PB0  | TIM1:CH2N            |
| ADC_01          | PB1  | TIM1:CH3N            |
| GPIO_05         | PA2  | TIM9:CH1             |
| GPIO_06         | PA3  | TIM9:CH2             |
| I2C_00_SCL      | PB8  | TIM10:CH1 / TIM4:CH3 |
| I2C_00_SDA      | PB7  | TIM4:CH2             |
| SPI_SDCARD_MISO | PB14 | TIM1:CH2N            |
| SPI_SDCARD_MOSI | PB15 | TIM1:CH3N            |
| SPI_SDCARD_SCK  | PB13 | TIM1:CH1N            |
| UART_00_RX      | PA10 | TIM1:CH3             |
| UART_00_TX      | PB6  | TIM4:CH1             |

## PIN assignment

| PIN  | LABEL                       |
|------|-----------------------------|
| PB0  | ADC_00                      |
| PB1  | ADC_01                      |
| PA8  | AUDIOCODEC_I2C_SCL          |
| PC9  | AUDIOCODEC_I2C_SDA          |
| PB9  | AUDIOCODEC_SAI_FS_B         |
| PC0  | AUDIOCODEC_SAI_MCLK_B       |
| PB12 | AUDIOCODEC_SAI_SCK_B        |
| PC1  | AUDIOCODEC_SAI_SD_A         |
| PA9  | AUDIOCODEC_SAI_SD_B         |
| PA4  | DAC_00                      |
| PA5  | DAC_01                      |
| PA6  | ENCODER_00_A                |
| PA7  | ENCODER_00_B                |
| PC4  | ENCODER_00_BUTTON           |
| PC6  | ENCODER_01_A                |
| PC7  | ENCODER_01_B                |
| PC8  | ENCODER_01_BUTTON           |
| PA0  | ENCODER_02_A                |
| PA1  | ENCODER_02_B                |
| PB4  | ENCODER_02_BUTTON           |
| PC13 | GPIO_00                     |
| PC14 | GPIO_01                     |
| PC15 | GPIO_02                     |
| PC2  | GPIO_03                     |
| PC3  | GPIO_04                     |
| PA2  | GPIO_05                     |
| PA3  | GPIO_06                     |
| PC5  | GPIO_07                     |
| PB2  | GPIO_08                     |
| PB10 | GPIO_09                     |
| PB8  | I2C_00_SCL                  |
| PB7  | I2C_00_SDA                  |
| PB5  | LED_00                      |
| PD2  | LED_01                      |
| PC12 | LED_02                      |
| PB14 | SPI_SDCARD_MISO             |
| PB15 | SPI_SDCARD_MOSI             |
| PB13 | SPI_SDCARD_SCK              |
| PA14 | SWD_SWCLK                   |
| PA13 | SWD_SWDIO                   |
| PA15 | SWD_SWI                     |
| PB3  | SWD_SWO                     |
| PA10 | UART_00_RX                  |
| PB6  | UART_00_TX                  |
| PC11 | UART_01_RX                  |
| PC10 | UART_01_TX                  |
| PA11 | USB_DEVICE-                 |
| PA12 | USB_DEVICE+                 |
