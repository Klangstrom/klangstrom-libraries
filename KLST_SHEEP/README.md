# KLANGSTROM / KLST_SHEEP

KLST_SHEEP is a small version of the KLST_SHEEP board:

- featuring *KLANGSTROM* library
- programmable in Arduino IDE via USB ( *DFUse Mode* )
- debugging + communication via *virtual USB Serial Port*

## feature list (20210322)

- STM32H743 MCU with 480MHz, 864KB RAM, 2048KB Flash
- WM8731 audio codec with 2× audio DAC + 2× audio ADC ( 16/24BIT )
- 2× ADC ( 12BIT, opt 6 extra channels )
- 2× DAC ( 12BIT )
- 3× UART ( serial )
- 8× GPIO ( with PWM )
- 1× USB Host
- 1× USB Device ( + Power Supply )
- 1× I2C 
- 1× SPI 
- 2× rotary encoders with push buttons
- 3× programmable LEDs + 1× power LED
- 11× timers
- 1× SD card reader
- 1× JTAG/SWD interface + Serial Debug ( 14-pin )
- programmer + reset + boot flash buttons
- 4× Mounting Holes

## technical details

### timers

- TIM1       :: ENCODER_00_A+B
- TIM1:CH3   :: ENCODER_00_BUTTON
- TIM2       :: ENCODER_01_A+B
- TIM2:CH4   :: ENCODER_01_BUTTON

### GPIO PWM

- TIM3:CH1–4 :: LED / PWM
- TIM4:CH1–4 :: LED / PWM
- TIM8:CH1–4 :: LED / PWM
- TIM13:CH1  :: LED / PWM
- TIM14:CH1  :: LED / PWM
- TIM15:CH1  :: LED / PWM
- TIM16:CH1  :: LED / PWM

### SPI

- SPI2 :: SD-CARD
- SPI3 :: DISPLAY
- SPI6 :: SPI_USR

### UART

- UART7  :: SERIAL_00
- UART8  :: SERIAL_01
- USART2 :: SERIAL_DEBUG

## PIN assignment

| PIN  | LABEL                       |
|------|-----------------------------|
| PA0  | ENCODER_01_A                |
| PA1  | ENCODER_01_B                |
| PA2  | LED_15                      |
| PA3  | ENCODER_01_BUTTON           |
| PA4  | DAC_00                      |
| PA5  | DAC_01                      |
| PA6  | LED_14                      |
| PA7  | LED_13                      |
| PA8  | ENCODER_00_A                |
| PA9  | ENCODER_00_B                |
| PA10 | ENCODER_00_BUTTON           |
| PA11 | USB_DEVICE-                 |
| PA12 | USB_DEVICE+                 |
| PA13 | SWD_SWDIO                   |
| PA14 | SWD_SWCLK                   |
| PA15 | SPI_DISPLAY_CS              |
| PB0  | LED_12                      |
| PB1  | LED_11                      |
| PB2  | GPIO_11                     |
| PB3  | SWD_SWO                     |
| PB4  | LED_02                      |
| PB5  | LED_01                      |
| PB6  | I2C_USR_SCL                 |
| PB7  | I2C_USR_SDA                 |
| PB8  | LED_00                      |
| PB9  | BUTTON_PRG                  |
| PB10 | AUDIOCODEC_I2C_SCL          |
| PB11 | AUDIOCODEC_I2C_SDA          |
| PB12 | SD_CARD_CS                  |
| PB13 | SD_CARD_SCK                 |
| PB14 | USB_HOST-                   |
| PB15 | USB_HOST+                   |
| PC0  | GPIO_12                     |
| PC1  | SDCARD_CARD_DETECT          |
| PC2  | SD_CARD_MISO                |
| PC3  | SD_CARD_MOSI                |
| PC4  | ADC_00                      |
| PC5  | ADC_01                      |
| PC6  | LED_06                      |
| PC7  | LED_05                      |
| PC8  | LED_04                      |
| PC9  | LED_03                      |
| PC10 | SPI_DISPLAY_SCK             |
| PC11 | SPI_DISPLAY_MISO            |
| PC12 | SPI_DISPLAY_MOSI            |
| PC13 | GPIO_15                     |
| PC14 | GPIO_14                     |
| PC15 | GPIO_13                     |
| PD0  | SPI_DISPLAY_REGISTER_SELECT |
| PD1  | SPI_DISPLAY_RESET           |
| PD2  | GPIO_03                     |
| PD3  | GPIO_02                     |
| PD4  | GPIO_01                     |
| PD5  | SERIAL_DEBUG_TX             |
| PD6  | SERIAL_DEBUG_RX             |
| PD7  | GPIO_00                     |
| PD8  | GPIO_07                     |
| PD9  | GPIO_06                     |
| PD10 | GPIO_05                     |
| PD11 | GPIO_04                     |
| PD12 | LED_10                      |
| PD13 | LED_09                      |
| PD14 | LED_08                      |
| PD15 | LED_07                      |
| PE0  | SERIAL_01_RX                |
| PE1  | SERIAL_01_TX                |
| PE2  | AUDIOCODEC_SAI1_MCLK_A      |
| PE3  | AUDIOCODEC_SAI1_SD_B        |
| PE4  | AUDIOCODEC_SAI1_FS_A        |
| PE5  | AUDIOCODEC_SAI1_SCK_A       |
| PE6  | AUDIOCODEC_SAI1_SD_A        |
| PE7  | SERIAL_00_RX                |
| PE8  | SERIAL_00_TX                |
| PE9  | GPIO_10                     |
| PE10 | GPIO_09                     |
| PE11 | SPI_USR_CS                  |
| PE12 | SPI_USR_SCK                 |
| PE13 | SPI_USR_MISO                |
| PE14 | SPI_USR_MOSI                |
| PE15 | GPIO_08                     |
