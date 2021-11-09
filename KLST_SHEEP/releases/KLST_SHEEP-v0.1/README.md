# KLANGSTROM / KLST_SHEEP

KLST_SHEEP is a *wolf in sheep's clothing* it is similar to KLST_TINY however comes with a stronger MCU and a screen:

- featuring *KLANGSTROM* library
- programmable in Arduino IDE via USB ( *DFUse Mode* )
- debugging + communication via *virtual USB Serial Port*

## feature list

- STM32H743 MCU with 480MHz, 864KB RAM, 2048KB Flash
- WM8731 audio codec with 2× audio DAC + 2× audio ADC ( 16/24BIT )
- 1× LINE OUT ( stereo )
- 1× LINE IN ( stereo )
- 1× HEADPHONE + MIC ( mono )
- 2× ADC ( 12BIT, opt 6 extra channels )
- 2× DAC ( 12BIT )
- 2× UART ( serial )
- 16× GPIO
- 1× USB Host
- 1× USB Device ( + Power Supply )
- 1× I2C 
- 1× SPI 
- 2× rotary encoders with push buttons
- 16× programmable LEDs with PWM + 1× power LED
- 1× SD card reader
- 1× JTAG/SWD interface + serial debug ( 14-pin )
- programmer + reset + boot flash buttons
- 4× Mounting Holes

## technical details

### timers

timers `TIM5` + `TIM6` + `TIM7` + `TIM12` can be used for internal interrupts.

#### encoders

timers `TIM1` + `TIM2` are used to handle encoder events.

| FUNCTION          | TIMER:CHANNEL |
|-------------------|---------------|
| ENCODER_00_A+B    | TIM1          |
| ENCODER_00_BUTTON | TIM1:CH3      |
| ENCODER_01_A+B    | TIM2          |
| ENCODER_01_BUTTON | TIM2:CH4      |

#### timers for LEDs with PWM

timers `TIM3` + `TIM4` + `TIM8` + `TIM13` + `TIM14` + `TIM15` + `TIM16` are used for PWM signal generation for LEDs.

| TIMER | CHANNEL |
|-------|---------|
| TIM3  | CH1–4   |
| TIM4  | CH1–4   |
| TIM8  | CH1–4   |
| TIM13 | CH1     |
| TIM14 | CH1     |
| TIM15 | CH1     |
| TIM16 | CH1     |

#### programmer button

timer `TIM17` is used for `BUTTON_PRG`.

#### timer-based PWM alternative PINs

there are no GPIO pins with assigned PWM capabilities. however, the following peripheral pins can be reconfigured to produce PWM signals ( at the expanse of other peripherals ):

| FUNCTION       | PIN  | TIMER                |
|----------------|------|----------------------|
| DAC_01         | PA5  | TIM2:CH1 / TIM8:CH1N |
| USB_DEVICE-    | PA11 | TIM1_CH4             |
| SPI_DISPLAY_CS | PA15 | TIM2_CH1             |
| I2C_USR_SCL    | PB6  | TIM4_CH1             |
| I2C_USR_SDA    | PB7  | TIM4_CH2             |
| GPIO_10        | PE9  | TIM1_CH1             |
| SPI_USR_CS     | PE11 | TIM1_CH2             |
| SPI_USR_MISO   | PE13 | TIM1_CH3             |
| SPI_USR_MOSI   | PE14 | TIM1_CH4             |

### SPI

| FUNCTION | PERIPHERAL |
|----------|------------|
| SD-CARD  | SPI2       |
| DISPLAY  | SPI3       |
| SPI_USR  | SPI6       |

### UART

| FUNCTION     | PERIPHERAL |
|--------------|------------|
| SERIAL_00    | UART7      |
| SERIAL_01    | UART8      |
| SERIAL_DEBUG | USART2     |

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
