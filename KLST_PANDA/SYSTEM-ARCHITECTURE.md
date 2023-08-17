# KLANGSTROM / KLST_PANDA / SYSTEM ARCHITECTURE

![SYSTEM-ARCHITECTURE](SYSTEM-ARCHITECTURE.png)

## Core

the core is a `‌STM32H723ZGT` by ST, a 32-bit Arm® Cortex®-M7 CPU ( or MCU ) with 550MHz, 564KB RAM, 1024KB Flash, and FPU that comes in a LQFP-144 ( 20x20x1.4mm ) package.

- [STM32H723ZG](https://www.st.com/resource/en/datasheet/stm32h723zg.pdf) ( datasheet )

## External Memory

external memory is provided by `S70KL1282` by Infineon ( or more specifically `S70KL1282GABHV020` ), a 16MB ( or more specifically 128Mb ) DRAM with HYPERBUS™ interface 

- [S70KL1282, S70KS1282: 128 Mb HYPERRAMTM self-refresh DRAM (PSRAM)](https://www.infineon.com/dgdl/Infineon-S70KL1282_S70KS1282_3.0_V_1.8_V_128_Mb_(16_MB)_HYPERBUS_INTERFACE_HYPERRAM_(SELF-REFRESH_DRAM)-DataSheet-v02_00-EN.pdf?fileId=8ac78c8c7d0d8da4017d0ee9315b7210) ( datasheet )
- [Migrating from S70KL1281/S70KS1281 to S70KL1282/S70KS1282](https://www.infineon.com/dgdl/Infineon-AN229785_-_Migrating_from_S70KL1281_S70KS1281_to_S70KL1282_S70KS1282-ApplicationNotes-v01_00-EN.pdf?fileId=8ac78c8c7e7124d1017eb8fba9bb258c)

## Audio Codec

the audio codec is a `‌WM8904` by Cirrus Logic. it features stereo headphone amplifiers in a QFN-32 package. it can be interface via a stereo line-in ( 3.5mm, TRS ), a stereo line-out ( 3.5mm, TRS ) or a 3-ring audio connector ( 3.5mm, TRRS with CTIA allocation ) for stereo headphones and mono microphone.

- [WM8904](https://statics.cirrus.com/pubs/proDatasheet/WM8904_Rev4.1.pdf) ( datasheet )

## Display

the display is a 4.3" TFT LCD ( `ER-TFT043A2-3` ) with a resolution of 480×272px, 24Bit colors. it is connected via a 40-pin FPC connector and features a capacitive touch surface ( interfaced via an `‌FT5206 ` I2C-based controller ) and dimmable a backlight ( via `CAT4139‌` + PWM ).

the display is interfaced with the CPU via a parallel LTDC interface.

- [ER-TFT043A2-3 4.3 inch TFT LCD 480x270 Pixels Display Datasheet](https://www.buydisplay.com/download/manual/ER-TFT043A2-3_Datasheet.pdf)
- [FT5206](https://www.buydisplay.com/download/ic/FT5206.pdf)
- [CAT4139TD-GT3](https://www.onsemi.com/products/power-management/led-drivers/dc-dc-led-drivers/cat4139)

## On-Board Microphone

the on-board microphone is a mono MEMS microphone fitted directly onto the board and interfaced via a second I2S interface. it can be used in parallel with an external microphone or line-in signals.

- e.g [GMA4030H11-F26](https://jlcpcb.com/partdetail/Inghai-GMA4030H11F26/C498193)

## SD Card

the SD Card supports SDIO/SDMMC with a 4-bit wide data line and an automatic card detect functionality.

## USB-C Connectors

the board features two USB-C connectors. one that acts as *USB Device* and takes care of the power supply including the charging of the battery, while the other one can be used as a *USB Host* to connect keyboards, mouse or *USB MIDI Devices*.

## Battery

the battery is a `type 18650` cell fitted into an on-board battery holder, which allows to easily replace the battery. the battery can be charged via the USB-C connector.

the charging mechanism is realized via a `TP5400` an all-in-one battery charger and boost converter IC.

the board also features an on-off switch.

- with PCB mounted holder e.g [MY-18650-01](https://jlcpcb.com/partdetail/Myoung-MY_1865001/C2979183)

## Rotary Encoder

the rotary encoders 

## GPIO

==@TODO check te 27 pins … they might not be correct anymore ==

a 32-pin port hosts 27 General Purpose Inputs and Outputs (GPIO) and 5 power pins. the GPIO can either be used as standard input or output pins or can have the following extra functionalities:

- 4× programmable PWM pins
- 1× I2C ( 2 pins )
- 1× SPI ( 4 pins )
- 1× USART ( 2 pins ) ( ==@TODO also or exclusively map out as MIDI IN/OUT see KLST_OCTOPUS== )
- 1× DAC ( ==m@TODO also map out via audio connector== )
- 4× ADC ( ==@TODO also map out at least one via audio connector== )
- 10× standard digital in-/output pins

the power pins are used as follows:

- GND ( 3 pins )
- 5.0V
- 3.3V

```
    - maybe case with fake shrouded connectors ( see IDC housing )
```

## MIDI 

==@TODO==

- analog MIDI: 3.5mm mono audio jack ( TS )
- ==@TODO also or exclusively map out as MIDI IN/OUT see KLST_OCTOPUS==
- MIDI ( analog )
    - interfaced via 3.5mm stereo audio connector ( TRS with `Type A` allocation ) ==@TODO(decide on the Type see https://minimidi.world)==

## DAC + ADC

==@TODO==

- DAC: 3.5mm mono audio jack ( TS )
- ADC: MIDI: 3.5mm mono audio jack ( TS )
- range 0.0–3.3V
- @TODO(over voltage protection e.g https://electronics.stackexchange.com/questions/487790/how-to-protect-a-microcontroller-analog-pin-from-continuous-overvoltage)
- @TODO(reversed voltage protection)

```
    - connection to modular synth?!?
    - 12V tolerant ( or modular connection as extension board = 0.0—3.3V > -12.0–12.0V + -1.0–1.0V>-5.0>5.0V(?) )
```

## IDC Serial

==@TODO==

## Programmer + Debugger

==@TODO==
```
- programmer interface
    - STD14 ( which is an SWD programmer connector extended by a serial port for debugging )
    - pogo pin connector ( without housing on front-side )
    [TC2070-IDC-NL-050](https://www.tag-connect.com/product/tc2070-idc-nl-050) by Tag-Connect 
```

- programmer interface with UART with STD14 and pogo pin connector

## Dimmable LEDs

2 dimmable LEDs.

## Buttons

==@TODO==
```
- mechanical keybord keys
    - 2×
    - just below encoders?
    - with interrupts
    - https://github.com/daprice/keyswitches.pretty
```

- 2× programmable buttons ( ==@TODO(mechanical keybord keys?)== )
- reset button
- programmer button
- boot button

## Form Factor

==@TODO==
```
- case+shape
    - switch?
    - portrait mode?
    - something unique!!!
    - hole for a strap
    - black&white PCB
- 4× Mounting Holes
```

## Pin Map

![](./firmware/KLST_PANDA-STM32H723ZGT-BSP--pin_map.png)

./firmware/KLST_PANDA-STM32H723ZGT-BSP--pin_map.csv

## Timers

| TIMER | CHANNEL         | DESCRIPTION             |
|-------|-----------------|-------------------------|
| TIM1  | CH1+CH2+CH3     | ENCODER_00(+BUTTON)     |
| TIM2  | CH1+CH2+CH4     | ENCODER_01(+BUTTON)     |
| TIM3  | CH3             | DISPLAY_BACKLIGHT_PWM   |
| TIM4  | CH3+CH4         | MECH_BUTTON_00*01       |
| TIM12 | CH1+CH2         | LED_00+01               |
| TIM15 | CH1             | DISPLAY_TOUCH_INTERRUPT |
| TIM23 | CH1+CH2+CH3+CH4 | GPIO_PWM_00+01+02+03    |
| TIM24 | CH2             | BUTTON_PROGRAMMER       |

## Buses

### U(S)ART

| U(S)ART | DESCRIPTION        |
|---------|--------------------|
| USART2  | SERIAL_USR         |
| USART3  | SERIAL_DEBUG_TX    |
| UART4   | MIDI_ANALOG_IN+OUT |
| UART8   | IDC_SERIAL_01      |
| UART9   | IDC_SERIAL_02      |

### I2C

| I2C     | DESCRIPTION        |
|---------|--------------------|
| I2C1    | I2C_USR            |
| I2C4    | AUDIO_CODEC_I2C    |
| I2C4    | DISPLAY_TOUCH      |

@NOTE(`‌AUDIO_CODEC_I2C` and `DISPLAY_TOUCH‌` are both interfaced via `‌I2C4`, however, `I2C5` could be an alternative for one of them. `I2C5` pins are currently used for `GPIO_02+03_PWM+GPIO_03_PWM`. )

### ADC+DAC

| ADC/DAC | DESCRIPTION        |
|---------|--------------------|
| ADC1    | GPIO_05_ADC        |
| ADC2    | GPIO_04_ADC        |
| ADC3    | ADC_USR            |
| DAC1    | DAC_USR            |
