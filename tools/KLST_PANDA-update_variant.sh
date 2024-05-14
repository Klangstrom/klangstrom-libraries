#!/bin/zsh

./collect_variant_files.sh ../KLST_PANDA/firmware/KLST_PANDA-variant-files.txt ../KLST_PANDA/firmware ../KLST_PANDA/variant
./copy_variant.sh ../KLST_PANDA/variant $HOME/Library/Arduino15/packages/STMicroelectronics/hardware/stm32/2.7.1/variants/STM32H7xx/H723Z\(E-G\)T_H730ZBT_H733ZGT
