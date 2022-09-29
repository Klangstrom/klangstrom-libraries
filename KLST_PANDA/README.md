# KLANGSTROM / KLST_PANDA

- STM32H723ZG MCU with 550MHz, 564KB RAM, 1024KB Flash
- WM8904 audio codec with 2× audio DAC + 2× audio ADC ( 16/24BIT )


---

==@TODO(add modular circuitry at least as IDC extension plug)==

    KLST_SHEEP is a *wolf in sheep's clothing* it is similar to KLST_TINY however comes with a stronger MCU and a screen:
    
    - featuring *KLANGSTROM* library
    - programmable in Arduino IDE via USB ( *DFUse Mode* )
    - debugging + communication via *virtual USB Serial Port*

## feature list

- [x] STM32H723ZG MCU with 550MHz, 564KB RAM, 1024KB Flash
- [x] WM8904 audio codec with 2× audio DAC + 2× audio ADC ( 16/24BIT )
- [ ] 1× LINE OUT ( stereo )
- [ ] 1× LINE IN ( stereo )
- [ ] 1× HEADPHONE + MIC ( mono )
- [ ] 2× ADC ( 12BIT, opt 6 extra channels )
- [ ] 2× DAC ( 12BIT )
- [ ] 2× UART ( serial )
- [?] 16× GPIO
- [ ] 1× USB Host
- [ ] 1× USB Device ( + Power Supply )
- [ ] 1× I2C 
- [ ] 1× SPI 
- [ ] 2× rotary encoders with push buttons
- [ ] 16× programmable LEDs with PWM + 1× power LED
- [ ] 1× SD card reader
- [ ] 1× JTAG/SWD interface + serial debug ( 14-pin )
- [ ] programmer + reset + boot flash buttons
- [ ] 4× Mounting Holes

## technical details

### timers

| TIMER | FUNCTION                         |
|-------|----------------------------------|
| TIM1  | ENCODER_00:CH1+CH2+CH3           |
| TIM2  | ENCODER_01:CH1+CH2+CH4           |
| TIM3  | PWM:CH1+CH3                      |
| TIM4  | PWM:CH3+CH4                      |
| TIM5  | (UNUSED)                         |
| TIM6  | (UNUSED)                         |
| TIM7  | (UNUSED)                         |
| TIM8  | (UNUSED)                         |
| TIM12 | @TODO(INPUT_CAPTURE)             |
| TIM13 | (UNUSED)                         |
| TIM14 | (UNUSED)                         |
| TIM15 | (UNUSED)                         |
| TIM16 | (UNUSED)                         |
| TIM17 | (UNUSED)                         |
| TIM23 | PWM:CH1+CH2+CH3+CH4              |
| TIM24 | PWM:CH2                          |

#### encoders

timers `TIM1` + `TIM2` are used to handle encoder events.

| FUNCTION          | TIMER:CHANNEL |
|-------------------|---------------|
| ENCODER_00_A+B    | TIM1          |
| ENCODER_00_BUTTON | TIM1:CH3      |
| ENCODER_01_A+B    | TIM2          |
| ENCODER_01_BUTTON | TIM2:CH4      |

#### [ ] timers for LEDs with PWM

timers `TIM?` + `TIM?` are used for PWM signal generation for LEDs.

| TIMER | CHANNEL |
|-------|---------|
| TIM?  | CH1–4   |
| TIM?  | CH1     |


LEDs are mapped to the following timers:

| LED    | PIN   | TIMER:CHANNEL |
|--------|-------|---------------|
| LED_?? | P??   | TIM?:CH?      |

#### [ ] timer-based PWM alternative PINs

most GPIO pins do not have PWM capabilities. however, the following peripheral pins can be reconfigured to produce PWM signals ( at the expanse of other peripherals ):

| FUNCTION       | PIN  | TIMER:CHANNEL        |
|----------------|------|----------------------|
| ??????         | P??  | TIM?:CH? / TIM?:CH?  |

### I2C

| FUNCTION   | PERIPHERAL |
|------------|------------|
| AUDIOCODEC | I2CX       |

### SPI

| FUNCTION | PERIPHERAL |
|----------|------------|
| SD-CARD  | SPIX       |

### UART

| FUNCTION     | PERIPHERAL |
|--------------|------------|
| SERIAL_00    | UARTXX     |

## PIN assignment

| PIN  | LABEL                       |
|------|-----------------------------|
| P??  | XXXXXX                      |


---

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

| TIMER | FUNCTION                         |
|-------|----------------------------------|
| TIM1  | ENCODER_00                       |
| TIM2  | ENCODER_01                       |
| TIM3  | LED_PWM                          |
| TIM4  | LED_PWM                          |
| TIM5  | (UNUSED)                         |
| TIM6  | BEAT_CALLBACK                    |
| TIM7  | TIMER_SERVO                      |
| TIM8  | LED_PWM                          |
| TIM9  | (nA)                             |
| TIM10 | (nA)                             |
| TIM11 | (nA)                             |
| TIM12 | TIMER_TONE                       |
| TIM13 | LED_PWM                          |
| TIM14 | LED_PWM                          |
| TIM15 | LED_PWM                          |
| TIM16 | LED_PWM                          |
| TIM17 | BUTTON_PRG_IT                    |

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

LEDs are mapped to the following timers:

| LED    | PIN   | TIMER:CHANNEL |
|--------|-------|---------------|
| LED_00 | PB8   | TIM16:CH1     |
| LED_01 | PB5   | TIM3:CH2      |
| LED_02 | PB4   | TIM3:CH1      |
| LED_03 | PC9   | TIM8:CH4      |
| LED_04 | PC8   | TIM8:CH3      |
| LED_05 | PC7*  | TIM8:CH2      |
| LED_06 | PC6*  | TIM8:CH1      |
| LED_07 | PD15  | TIM4:CH4      |
| LED_08 | PD14  | TIM4:CH3      |
| LED_09 | PD13  | TIM4:CH2      |
| LED_10 | PD12  | TIM4:CH1      |
| LED_11 | PB1   | TIM3:CH4      |
| LED_12 | PB0   | TIM3:CH3      |
| LED_13 | PA7   | TIM14:CH1     |
| LED_14 | PA6*  | TIM13:CH1     |
| LED_15 | PA2   | TIM15:CH1     |

#### timer-based PWM alternative PINs

most GPIO pins do not have PWM capabilities. however, the following peripheral pins can be reconfigured to produce PWM signals ( at the expanse of other peripherals ):

| FUNCTION       | PIN  | TIMER:CHANNEL        |
|----------------|------|----------------------|
| DAC_01         | PA5  | TIM2:CH1 / TIM8:CH1N |
| USB_DEVICE-    | PA11 | TIM1:CH4             |
| SPI_DISPLAY_CS | PA15 | TIM2:CH1             |
| I2C_USR_SCL    | PB6  | TIM4:CH1             |
| I2C_USR_SDA    | PB7  | TIM4:CH2             |
| GPIO_10        | PE9  | TIM1:CH1             |
| SPI_USR_CS     | PE11 | TIM1:CH2             |
| SPI_USR_MISO   | PE13 | TIM1:CH3             |
| SPI_USR_MOSI   | PE14 | TIM1:CH4             |

### I2C

| FUNCTION   | PERIPHERAL |
|------------|------------|
| AUDIOCODEC | I2C1       |
| I2C_USR    | I2C2       |

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
| PC4  | ADC_00                      |
| PC5  | ADC_01                      |
| PB6  | AUDIOCODEC_I2C_SCL          |
| PB7  | AUDIOCODEC_I2C_SDA          |
| PE4  | AUDIOCODEC_SAI1_FS_A        |
| PE2  | AUDIOCODEC_SAI1_MCLK_A      |
| PE5  | AUDIOCODEC_SAI1_SCK_A       |
| PE6  | AUDIOCODEC_SAI1_SD_A        |
| PE3  | AUDIOCODEC_SAI1_SD_B        |
| PB9  | BUTTON_PRG                  |
| PA4  | DAC_00                      |
| PA5  | DAC_01                      |
| PA0  | ENCODER_00_A                |
| PA1  | ENCODER_00_B                |
| PA3  | ENCODER_00_BUTTON           |
| PA8  | ENCODER_01_A                |
| PA9  | ENCODER_01_B                |
| PA10 | ENCODER_01_BUTTON           |
| PD7  | GPIO_00                     |
| PD4  | GPIO_01                     |
| PD3  | GPIO_02                     |
| PD2  | GPIO_03                     |
| PD11 | GPIO_04                     |
| PD10 | GPIO_05                     |
| PD9  | GPIO_06                     |
| PD8  | GPIO_07                     |
| PE15 | GPIO_08                     |
| PE10 | GPIO_09                     |
| PE9  | GPIO_10                     |
| PB2  | GPIO_11                     |
| PC0  | GPIO_12                     |
| PC15 | GPIO_13                     |
| PC14 | GPIO_14                     |
| PC13 | GPIO_15                     |
| PB10 | I2C_USR_SCL                 |
| PB11 | I2C_USR_SDA                 |
| PB8  | LED_00                      |
| PB5  | LED_01                      |
| PB4  | LED_02                      |
| PC9  | LED_03                      |
| PC8  | LED_04                      |
| PC7  | LED_05                      |
| PC6  | LED_06                      |
| PD15 | LED_07                      |
| PD14 | LED_08                      |
| PD13 | LED_09                      |
| PD12 | LED_10                      |
| PB1  | LED_11                      |
| PB0  | LED_12                      |
| PA7  | LED_13                      |
| PA6  | LED_14                      |
| PA2  | LED_15                      |
| PB12 | SDCARD_CS                   |
| PC2  | SDCARD_MISO                 |
| PC3  | SDCARD_MOSI                 |
| PB13 | SDCARD_SCK                  |
| PC1  | SDCARD_CARD_DETECT          |
| PE7  | SERIAL_00_RX                |
| PE8  | SERIAL_00_TX                |
| PE0  | SERIAL_01_RX                |
| PE1  | SERIAL_01_TX                |
| PD6  | SERIAL_DEBUG_RX             |
| PD5  | SERIAL_DEBUG_TX             |
| PA15 | SPI_DISPLAY_CS              |
| PC11 | SPI_DISPLAY_MISO            |
| PC12 | SPI_DISPLAY_MOSI            |
| PD0  | SPI_DISPLAY_REGISTER_SELECT |
| PD1  | SPI_DISPLAY_RESET           |
| PC10 | SPI_DISPLAY_SCK             |
| PE11 | SPI_USR_CS                  |
| PE13 | SPI_USR_MISO                |
| PE14 | SPI_USR_MOSI                |
| PE12 | SPI_USR_SCK                 |
| PA14 | SWD_SWCLK                   |
| PA13 | SWD_SWDIO                   |
| PB3  | SWD_SWO                     |
| PA11 | USB_DEVICE-                 |
| PA12 | USB_DEVICE+                 |
| PB14 | USB_HOST-                   |
| PB15 | USB_HOST+                   |
