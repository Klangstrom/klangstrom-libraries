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

@note(encoder buttons are on input capture pins ENCODER_00_BUTTON :: TIM1:CH3 + ENCODER_01_BUTTON :: TIM2:CH4 + TIM5:CH4 + TIM15:CH2 )

### timers

- TIM1+2     :: ENCODER_00–01
- TIM4:CH1–4 :: GPIO_04–07 PWM
- TIM8:CH1–4 :: GPIO_00–03 PWM
- TIM13–15   :: LED_00–02 PWM

#### GPIO_EXT

- TIM17:CH1  :: GPIO_EXT PWM
- TIM16:CH1  :: GPIO_EXT PWM
- TIM3:CH1–4 :: GPIO_EXT PWM

### SPI

- SPI2 :: SD-CARD
- SPI3 :: DISPLAY
- SPI6 :: SPI_00

### UART

- UART7  :: SERIAL_00
- UART8  :: SERIAL_01
    - USART2 :: SERIAL_DEBUG