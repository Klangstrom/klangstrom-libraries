/*
* Klangstrom
*
* This file is part of the *Klangstrom* library (https://github.com/dennisppaul/klangstrom-libraries).
* Copyright (c) 2024 Dennis P Paul.
*
* This library is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3.
*
* This library is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    TIM1_BASE,
    TIM2_BASE,
    TIM3_BASE,
    TIM4_BASE,
    TIM5_BASE,
    TIM6_BASE,
    TIM7_BASE,
    TIM8_BASE,
    TIM9_BASE,
    TIM10_BASE,
    TIM11_BASE,
    TIM12_BASE,
    TIM13_BASE,
    TIM14_BASE,
    TIM15_BASE,
    TIM16_BASE,
    TIM17_BASE,
    TIM18_BASE,
    TIM19_BASE,
    TIM20_BASE,
    TIM21_BASE,
    TIM22_BASE
} TIM_TypeDef;

#define TIM1 ((TIM_TypeDef*) TIM1_BASE)
#define TIM2 ((TIM_TypeDef*) TIM2_BASE)
#define TIM3 ((TIM_TypeDef*) TIM3_BASE)
#define TIM4 ((TIM_TypeDef*) TIM4_BASE)
#define TIM5 ((TIM_TypeDef*) TIM5_BASE)
#define TIM6 ((TIM_TypeDef*) TIM6_BASE)
#define TIM7 ((TIM_TypeDef*) TIM7_BASE)
#define TIM8 ((TIM_TypeDef*) TIM8_BASE)
#define TIM9 ((TIM_TypeDef*) TIM9_BASE)
#define TIM10 ((TIM_TypeDef*) TIM10_BASE)
#define TIM11 ((TIM_TypeDef*) TIM11_BASE)
#define TIM12 ((TIM_TypeDef*) TIM12_BASE)
#define TIM13 ((TIM_TypeDef*) TIM13_BASE)
#define TIM14 ((TIM_TypeDef*) TIM14_BASE)
#define TIM15 ((TIM_TypeDef*) TIM15_BASE)
#define TIM16 ((TIM_TypeDef*) TIM16_BASE)
#define TIM17 ((TIM_TypeDef*) TIM17_BASE)
#define TIM18 ((TIM_TypeDef*) TIM18_BASE)
#define TIM19 ((TIM_TypeDef*) TIM19_BASE)
#define TIM20 ((TIM_TypeDef*) TIM20_BASE)
#define TIM21 ((TIM_TypeDef*) TIM21_BASE)
#define TIM22 ((TIM_TypeDef*) TIM22_BASE)

#ifdef __cplusplus
}
#endif
