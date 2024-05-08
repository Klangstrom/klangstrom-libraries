/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
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

//#define KLST_DEBUG_ENVIRONMENT

#define KLST_ARCH                   0x10
#define KLST_ARCH_STM32             0x20
#define KLST_ARCH_EMU               0x30
#define KLST_ARCH_WORMS             0x40
#define KLST_ARCH_MASK              0xF0

#define KLST_BOARD                  0x01
#define KLST_BOARD_KLST_CORE        0x02
#define KLST_BOARD_KLST_TINY        0x03
#define KLST_BOARD_KLST_SHEEP       0x04
#define KLST_BOARD_KLST_PANDA       0x05
#define KLST_BOARD_KLST_CATERPILLAR 0x06
#define KLST_BOARD_MASK             0x0F

#ifndef KLST_ENV
#error "define KLST_ENV with (KLST_ARCH | KLST_BOARD) or (KLST_ARCH + KLST_BOARD) e.g KLST_PANDA on STM32: (KLST_BOARD_KLST_PANDA + KLST_ARCH_STM32) = (0x20 + 0x05) = 0x25"

#else

// KLST_CORE

#if ((KLST_ENV & KLST_BOARD_MASK) == KLST_BOARD_KLST_CORE) && ((KLST_ENV & KLST_ARCH_MASK) == KLST_ARCH_STM32)
#define KLST_CORE_STM32
#ifdef KLST_DEBUG_ENVIRONMENT
#warning "+++ klangstrom platform: KLST_CORE_STM32"
#endif // KLST_DEBUG_ENVIRONMENT
#endif // KLST_CORE_STM32

#if ((KLST_ENV & KLST_BOARD_MASK) == KLST_BOARD_KLST_CORE) && ((KLST_ENV & KLST_ARCH_MASK) == KLST_ARCH_EMU)
#define KLST_CORE_EMU
#ifdef KLST_DEBUG_ENVIRONMENT
#warning "+++ klangstrom platform: KLST_CORE_EMU"
#endif // KLST_DEBUG_ENVIRONMENT
#endif // KLST_CORE_EMU

#if ((KLST_ENV & KLST_BOARD_MASK) == KLST_BOARD_KLST_CORE) && ((KLST_ENV & KLST_ARCH_MASK) == KLST_ARCH_WORMS)
#define KLST_CORE_WORMS
#ifdef KLST_DEBUG_ENVIRONMENT
#warning "+++ klangstrom platform: KLST_CORE_WORMS"
#endif // KLST_DEBUG_ENVIRONMENT
#endif // KLST_CORE_WORMS

// KLST_TINY

#if ((KLST_ENV & KLST_BOARD_MASK) == KLST_BOARD_KLST_TINY) && ((KLST_ENV & KLST_ARCH_MASK) == KLST_ARCH_STM32)
#define KLST_TINY_STM32
#ifdef KLST_DEBUG_ENVIRONMENT
#warning "+++ klangstrom platform: KLST_TINY_STM32"
#endif // KLST_DEBUG_ENVIRONMENT
#endif // KLST_TINY_STM32

#if ((KLST_ENV & KLST_BOARD_MASK) == KLST_BOARD_KLST_TINY) && ((KLST_ENV & KLST_ARCH_MASK) == KLST_ARCH_EMU)
#define KLST_TINY_EMU
#ifdef KLST_DEBUG_ENVIRONMENT
#warning "+++ klangstrom platform: KLST_TINY_EMU"
#endif // KLST_DEBUG_ENVIRONMENT
#endif // KLST_TINY_EMU

#if ((KLST_ENV & KLST_BOARD_MASK) == KLST_BOARD_KLST_TINY) && ((KLST_ENV & KLST_ARCH_MASK) == KLST_ARCH_WORMS)
#define KLST_TINY_WORMS
#ifdef KLST_DEBUG_ENVIRONMENT
#warning "+++ klangstrom platform: KLST_TINY_WORMS"
#endif // KLST_DEBUG_ENVIRONMENT
#endif // KLST_TINY_WORMS

// KLST_SHEEP

#if ((KLST_ENV & KLST_BOARD_MASK) == KLST_BOARD_KLST_SHEEP) && ((KLST_ENV & KLST_ARCH_MASK) == KLST_ARCH_STM32)
#define KLST_SHEEP_STM32
#ifdef KLST_DEBUG_ENVIRONMENT
#warning "+++ klangstrom platform: KLST_SHEEP_STM32"
#endif // KLST_DEBUG_ENVIRONMENT
#endif // KLST_SHEEP_STM32

#if ((KLST_ENV & KLST_BOARD_MASK) == KLST_BOARD_KLST_SHEEP) && ((KLST_ENV & KLST_ARCH_MASK) == KLST_ARCH_EMU)
#define KLST_SHEEP_EMU
#ifdef KLST_DEBUG_ENVIRONMENT
#warning "+++ klangstrom platform: KLST_SHEEP_EMU"
#endif // KLST_DEBUG_ENVIRONMENT
#endif // KLST_SHEEP_EMU

#if ((KLST_ENV & KLST_BOARD_MASK) == KLST_BOARD_KLST_SHEEP) && ((KLST_ENV & KLST_ARCH_MASK) == KLST_ARCH_WORMS)
#define KLST_SHEEP_WORMS
#ifdef KLST_DEBUG_ENVIRONMENT
#warning "+++ klangstrom platform: KLST_SHEEP_WORMS"
#endif // KLST_DEBUG_ENVIRONMENT
#endif // KLST_SHEEP_WORMS

// KLST_PANDA

#if ((KLST_ENV & KLST_BOARD_MASK) == KLST_BOARD_KLST_PANDA) && ((KLST_ENV & KLST_ARCH_MASK) == KLST_ARCH_STM32)
#define KLST_PANDA_STM32
#ifdef KLST_DEBUG_ENVIRONMENT
#warning "+++ klangstrom platform: KLST_PANDA_STM32"
#endif // KLST_DEBUG_ENVIRONMENT
#endif // KLST_PANDA_STM32

#if ((KLST_ENV & KLST_BOARD_MASK) == KLST_BOARD_KLST_PANDA) && ((KLST_ENV & KLST_ARCH_MASK) == KLST_ARCH_EMU)
#define KLST_PANDA_EMU
#ifdef KLST_DEBUG_ENVIRONMENT
#warning "+++ klangstrom platform: KLST_PANDA_EMU"
#endif // KLST_DEBUG_ENVIRONMENT
#endif // KLST_PANDA_EMU

#if ((KLST_ENV & KLST_BOARD_MASK) == KLST_BOARD_KLST_PANDA) && ((KLST_ENV & KLST_ARCH_MASK) == KLST_ARCH_WORMS)
#define KLST_PANDA_WORMS
#ifdef KLST_DEBUG_ENVIRONMENT
#warning "+++ klangstrom platform: KLST_PANDA_WORMS"
#endif // KLST_DEBUG_ENVIRONMENT
#endif // KLST_PANDA_WORMS

// KLST_CATERPILLAR

#if ((KLST_ENV & KLST_BOARD_MASK) == KLST_BOARD_KLST_CATERPILLAR) && ((KLST_ENV & KLST_ARCH_MASK) == KLST_ARCH_STM32)
#define KLST_CATERPILLAR_STM32
#ifdef KLST_DEBUG_ENVIRONMENT
#warning "+++ klangstrom platform: KLST_CATERPILLAR_STM32"
#endif // KLST_DEBUG_ENVIRONMENT
#endif // KLST_CATERPILLAR_STM32

#if ((KLST_ENV & KLST_BOARD_MASK) == KLST_BOARD_KLST_CATERPILLAR) && ((KLST_ENV & KLST_ARCH_MASK) == KLST_ARCH_EMU)
#define KLST_CATERPILLAR_EMU
#ifdef KLST_DEBUG_ENVIRONMENT
#warning "+++ klangstrom platform: KLST_CATERPILLAR_EMU"
#endif // KLST_DEBUG_ENVIRONMENT
#endif // KLST_CATERPILLAR_EMU

#if ((KLST_ENV & KLST_BOARD_MASK) == KLST_BOARD_KLST_CATERPILLAR) && ((KLST_ENV & KLST_ARCH_MASK) == KLST_ARCH_WORMS)
#define KLST_CATERPILLAR_WORMS
#ifdef KLST_DEBUG_ENVIRONMENT
#warning "+++ klangstrom platform: KLST_CATERPILLAR_WORMS"
#endif // KLST_DEBUG_ENVIRONMENT
#endif // KLST_CATERPILLAR_WORMS

#endif // KLST_ENV
