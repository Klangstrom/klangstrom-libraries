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

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "MIDI_interface.h"
#include "usbh_MIDI.h"
#include "usbh_core.h"

#include "USBHostConstants.h"

#ifdef __cplusplus
extern "C" {
#endif

/* --- EVENT_TYPE (MIDI) --- */

void MIDI_Application(void);

/* --- transmit --- */

void usb_host_transmit_midi_message(const uint8_t data1, const uint8_t data2, const uint8_t data3);
void usb_host_transmit_midi_note_off(const uint8_t channel, const uint8_t note, const uint8_t velocity);
void usb_host_transmit_midi_note_on(const uint8_t channel, const uint8_t note, const uint8_t velocity);
void usb_host_transmit_midi_aftertouch(const uint8_t channel, const uint8_t note, const uint8_t pressure);
void usb_host_transmit_midi_control_change(const uint8_t channel, const uint8_t command, const uint8_t value);
void usb_host_transmit_midi_program_change(const uint8_t channel, const uint8_t program);
void usb_host_transmit_midi_channel_pressure(const uint8_t channel, const uint8_t pressure);
void usb_host_transmit_midi_pitch_bend(const uint8_t channel, const int16_t value);
void usb_host_transmit_midi_sys_ex(const uint8_t type, const uint8_t data1, const uint8_t data2);

// TODO
// void usb_host_callback_midi_mtc_quarter_frame(CallbackType1_UI8 callback);
// void usb_host_callback_midi_song_position_pointer(CallbackType1_UI16 callback);
// void usb_host_callback_midi_song_select(CallbackType1_UI8 callback);
// void usb_host_callback_midi_tune_req(CallbackType0 callback);
// void usb_host_callback_midi_end_ex(CallbackType0 callback);
// void usb_host_callback_midi_clock_tick(CallbackType0 callback);
// void usb_host_callback_midi_clock_start(CallbackType0 callback);
// void usb_host_callback_midi_clock_continue(CallbackType0 callback);
// void usb_host_callback_midi_clock_stop(CallbackType0 callback);
// void usb_host_callback_midi_rt_sense(CallbackType0 callback);
// void usb_host_callback_midi_rt_reset(CallbackType0 callback);

#ifdef __cplusplus
}
#endif