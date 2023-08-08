# KLANGSTROM / KLST_PANDA / COMPONENTS

- [x] ==@TODO(aligned with `/Users/dennisppaul/Documents/dennisppaul/projects/klangstrom/git/klangstrom-developer-diary/_posts/2023-04-08-KLST_PANDA-final-design-concept.md`)==
- [x] ==@TODO(aligned with `/Users/dennisppaul/Documents/dennisppaul/projects/klangstrom/git/klangstrom-developer-diary/_posts/2023-04-17-KLST_PANDA-Prototyping.md`)==
- [x] ==@TODO(align with `/Users/dennisppaul/Documents/dennisppaul/projects/klangstrom/hardware/KLST_PANDA/KLST_PANDA--specs.md`)==

![SYSTEM-ARCHITECTURE](SYSTEM-ARCHITECTURE.png)

## Core

`‌STM32H723ZGT` by ST is a 32-bit Arm® Cortex®-M7 CPU ( or MCU ) with 550MHz, 564KB RAM, 1024KB Flash, and FPU that comes in a LQFP-144 ( 20x20x1.4mm ) package.

- [STM32H723ZG](https://www.st.com/resource/en/datasheet/stm32h723zg.pdf) ( datasheet )

## External Memory

`S70KL1282` by Infineon ( or more specifically `S70KL1282GABHV020` ) is a 16MB ( or more specifically 128Mb ) DRAM with HYPERBUS™ interface 

- [S70KL1282, S70KS1282: 128 Mb HYPERRAMTM self-refresh DRAM (PSRAM)](https://www.infineon.com/dgdl/Infineon-S70KL1282_S70KS1282_3.0_V_1.8_V_128_Mb_(16_MB)_HYPERBUS_INTERFACE_HYPERRAM_(SELF-REFRESH_DRAM)-DataSheet-v02_00-EN.pdf?fileId=8ac78c8c7d0d8da4017d0ee9315b7210) ( datasheet )
- [Migrating from S70KL1281/S70KS1281 to S70KL1282/S70KS1282](https://www.infineon.com/dgdl/Infineon-AN229785_-_Migrating_from_S70KL1281_S70KS1281_to_S70KL1282_S70KS1282-ApplicationNotes-v01_00-EN.pdf?fileId=8ac78c8c7e7124d1017eb8fba9bb258c)

## Audio Codec

`‌WM8904` by Cirrus Logic is a stereo codec featuring stereo headphone amplifiers in a QFN-32 package.

the audio codec can be interface via a stereo line-in, a stereo line-out or a 3-ring audio connector ( CTIA ) for stereo headphones and mono microphone.

- [WM8904](https://statics.cirrus.com/pubs/proDatasheet/WM8904_Rev4.1.pdf) ( datasheet )

## Display

the display is a 4.3" TFT LCD with a resolution of 480×272px, 24Bit colors, a capacitive touch surface and dimmable backlight.

==@TODO==
```
- 4.3" 480×272 ( ==which?== 40-pin FPC connector )
- capacitive touch ( ADS7843 )
- dimmable backlight ( PWM+FAN5333 )
- LTDC interface ( parallel ) 
- see /Users/dennisppaul/Documents/dennisppaul/projects/klangstrom/hardware/KLST_PANDA/research.KLST_PANDA--TFTs/research.KLST_PANDA--TFTs.md
```

## On-Board Microphone

the on-board microphone is a mono MEMS microphone fitted directly onto the board. it can be used in parallel with an external microphone or line-in signals.

==@TODO==
```
    - MEMS microphone ( via I2S )
    - e.g [GMA4030H11-F26](https://jlcpcb.com/partdetail/Inghai-GMA4030H11F26/C498193)
```

## SD Card

the SD Card supports SDIO/SDMMC with a 4-bit wide data line and an automatic card detect functionality.

## USB-C Connectors

two USB-C connectors. one that acts as *USB Device* and takes care of the power supply including the charging of the battery, while the other one can be used as a *USB Host* to connect keyboards, mouse or *USB MIDI Devices*.

## Battery

the battery is a 18650 cell fitted into an on-board battery holder, which allows to easily replace the battery. the battery can be charged via the USB-C connector.

==@TODO==
```
    - battery type 18650
    - TP5400 ( all-in-one battery charger + boost converter IC )
    - with PCB mounted holder e.g [MY-18650-01](https://jlcpcb.com/partdetail/Myoung-MY_1865001/C2979183)
    - on/off switch
```

## Rotary Encoder

the rotary encoders 

## GPIO

==@TODO check te 27 pins … they might not be correct anymore ==

a 32-pin port hosts 27 General Purpose Inputs and Outputs (GPIO) and 5 power pins. the GPIO can either be used as standard input or output pins or can have the following extra functionalities:

- 4× programmable PWM pins
- 1× I2C ( 2 pins )
- 1× SPI ( 4 pins )
- 1× USART ( 2 pins ) ( ==@TODO also map out as MIDI IN/OUT see KLST_OCTOPUS== )
- 1× DAC ( ==m@TODO also ap out via audio connector== )
- 4× ADC ( ==@TODO also map out at least one via audio connector== )
- 10× standard digital in-/output pins

the power pins are used as follows:

- GND ( 3 pins )
- 5.0V
- 3.3V

### Analog MIDI + 1 DAC + 1 ADC

## IDC Serial

==@TODO==

## Programmer + Debugger

==@TODO==
```
- programmer interface
    - STD14
    - pogo pin connector ( without housing on front-side )
```

- programmer interface with UART with STD14 or pogo pin connector

## Dimmable LEDs

==@TODO==

2 dimmable LEDs.

## Buttons

==@TODO==
```
- mechanical keybord keys
    - 2×
    - just below encoders?
    - with interrupts
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

==@TODO(align/check with `./README.md`)==

![](./firmware/KLST_PANDA-STM32H723ZGT-BSP--pin_map.png)

./firmware/KLST_PANDA-STM32H723ZGT-BSP--pin_map.csv

## Timers

| TIMER | CHANNEL         | DESCRIPTION           |
|-------|-----------------|-----------------------|
| TIM1  | CH1+CH2+CH3     | ENCODER_00(+BUTTON)   |
| TIM2  | CH1+CH2+CH4     | ENCODER_01(+BUTTON)   |
| TIM3  | CH3             | DISPLAY_BACKLIGHT_PWM |
| TIM4  | CH3+CH4         | MECH_BUTTON_00*01     |
| TIM12 | CH1+CH2         | LED_00+01             |
| TIM23 | CH1+CH2+CH3+CH4 | GPIO_PWM_00+01+02+03  |
| TIM24 | CH2             | BUTTON_PROGRAMMER     |