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

// @TODO(fix temporary deprecation warning for `sprintf` ("-Wdeprecated-declarations"))

#include <cstdio>
#include <functional>
#include <string>

#include "MIDIParser.h"

#ifdef MIDI_PARSER_DISABLE_PRINT_MESSAGES
#warning "MIDI Parser messages are disabled."
#endif

// /* --- EVENT_TYPE (MIDI) --- */
typedef uint8_t EVENT_TYPE;
typedef uint8_t EVENT_DATA;

/* --- EVENT_TYPE (HID) --- */

static const EVENT_TYPE EVENT_RESERVED_01 = 0x00;
static const EVENT_TYPE EVENT_RESERVED_02 = 0x01;
static const EVENT_TYPE EVENT_RAW_DATA    = 0x02;
static const EVENT_TYPE EVENT_KEYBOARD    = 0x03;
static const EVENT_TYPE EVENT_MOUSE       = 0x04;
static const EVENT_TYPE EVENT_MIDI        = 0x05;
static const EVENT_TYPE EVENT_ENCODER     = 0x06;

static const EVENT_TYPE MIDI_NONE                  = 0x00;
static const EVENT_TYPE MIDI_NOTE_OFF              = 0x80;
static const EVENT_TYPE MIDI_NOTE_ON               = 0x90;
static const EVENT_TYPE MIDI_AFTERTOUCH            = 0xA0;
static const EVENT_TYPE MIDI_CONTROL_CHANGE        = 0xB0;
static const EVENT_TYPE MIDI_PROGRAM_CHANGE        = 0xC0;
static const EVENT_TYPE MIDI_CHANNEL_PRESSURE      = 0xD0;
static const EVENT_TYPE MIDI_PITCHBEND             = 0xE0;
static const EVENT_TYPE MIDI_SYS_EX                = 0xF0;
static const EVENT_TYPE MIDI_MTCQFRAME             = 0xF1; // dataB = quarter frame number
static const EVENT_TYPE MIDI_SONG_POSITION_POINTER = 0xF2;
static const EVENT_TYPE MIDI_SONG_SELECT           = 0xF3; // dataB = song#
static const EVENT_TYPE MIDI_UNDEF_1               = 0xF4;
static const EVENT_TYPE MIDI_UNDEF_2               = 0xF5;
static const EVENT_TYPE MIDI_TUNE_REQ              = 0xF6; // no data, return oscillators
static const EVENT_TYPE MIDI_ENDEX                 = 0xF7; // ends a SYSEX dump
static const EVENT_TYPE MIDI_CLOCK_TICK            = 0xF8;
static const EVENT_TYPE MIDI_RT_CLOCK              = 0xF8; // *
static const EVENT_TYPE MIDI_RT_UNDEF_1            = 0xF9;
static const EVENT_TYPE MIDI_CLOCK_START           = 0xFA;
static const EVENT_TYPE MIDI_RT_START              = 0xFA; // *
static const EVENT_TYPE MIDI_CLOCK_CONTINUE        = 0xFB;
static const EVENT_TYPE MIDI_RT_CONTINUE           = 0xFB; // *
static const EVENT_TYPE MIDI_CLOCK_STOP            = 0xFC;
static const EVENT_TYPE MIDI_RT_STOP               = 0xFC; // *
static const EVENT_TYPE MIDI_RT_UNDEF_2            = 0xFD;
static const EVENT_TYPE MIDI_RT_SENSE              = 0xFE; // Sent every 300ms on a live connection. Can ignore.
static const EVENT_TYPE MIDI_RT_RESET              = 0xFF; // Reset any parameters to their power up values
static const EVENT_TYPE MIDI_CC_MODWHEEL           = 1;    // 0-127
static const EVENT_TYPE MIDI_CC_BREATH             = 2;    // 0-127
static const EVENT_TYPE MIDI_CC_VOLUME             = 7;    // Channel mix volume
static const EVENT_TYPE MIDI_CC_PAN                = 10;   // 0-127; 64=centered
static const EVENT_TYPE MIDI_CC_EXPRESSION         = 11;   // playback dynamics
static const EVENT_TYPE MIDI_CC_SUSTAIN            = 64;   // 0=off; 127=on
static const EVENT_TYPE MIDI_CC_PORTAMENTO         = 65;   // 0=off; 127=on
static const EVENT_TYPE MIDI_CC_RESET              = 121;  // None. Also called "All Sound Off"
static const EVENT_TYPE MIDI_CC_MODE_LOCAL         = 122;  // 0=off; 127=on. Keyboard on/off
static const EVENT_TYPE MIDI_CC_ALL_NOTES_OFF      = 123;  // None
static const EVENT_TYPE MIDI_CC_OMNI_OFF           = 124;  // None
static const EVENT_TYPE MIDI_CC_OMNI_ON            = 125;  // None
static const EVENT_TYPE MIDI_CC_POLY_OFF           = 126;  // #channels (MONO ON)
static const EVENT_TYPE MIDI_CC_POLY_ON            = 127;  // None (MONO OFF)

typedef struct {
    uint8_t  channel;
    uint8_t  command;
    uint16_t a;
    uint8_t  b;
} EventMIDI;

class KlangstromMIDIParser {
private:
    typedef void (*CallbackType0)(void);
    typedef void (*CallbackType1_UI8)(uint8_t);
    typedef void (*CallbackType1_UI16)(uint16_t);
    typedef void (*CallbackType2_UI8)(uint8_t, uint8_t);
    typedef void (*CallbackType2_UI8_I16)(uint8_t, uint16_t);
    typedef void (*CallbackType2_UI8_VOIDPTR)(const uint8_t, const void*);
    typedef void (*CallbackType3_UI8)(uint8_t, uint8_t, uint8_t);

    CallbackType2_UI8_VOIDPTR callback_event_receive;
    CallbackType3_UI8         callback_note_off;
    CallbackType3_UI8         callback_note_on;
    CallbackType3_UI8         callback_aftertouch;
    CallbackType3_UI8         callback_control_change;
    CallbackType2_UI8         callback_program_change;
    CallbackType2_UI8         callback_channel_pressure;
    CallbackType2_UI8_I16     callback_pitch_bend;
    CallbackType0             callback_sys_ex;
    CallbackType1_UI8         callback_mtc_quarter_frame;
    CallbackType1_UI16        callback_song_position_pointer;
    CallbackType1_UI8         callback_song_select;
    CallbackType0             callback_tune_req;
    CallbackType0             callback_end_ex;
    CallbackType0             callback_clock_tick;
    CallbackType0             callback_clock_start;
    CallbackType0             callback_clock_continue;
    CallbackType0             callback_clock_stop;
    CallbackType0             callback_rt_sense;
    CallbackType0             callback_rt_reset;

    void handle_channel_message(uint8_t msg) {
        const uint8_t message_type = p.msg_value(msg);
        const uint8_t channel      = p.msg_channel(msg);
        EventMIDI     e;
        e.command = message_type;
        e.channel = channel;
        e.a       = p.get_dataA();
        e.b       = p.get_dataB();
        switch (message_type) {
            case MIDI_NOTE_OFF:
                if (callback_note_off) {
                    callback_note_off(channel, p.get_dataA(), p.get_dataB());
                }
                break;
            case MIDI_NOTE_ON:
                if (callback_note_on) {
                    callback_note_on(channel, p.get_dataA(), p.get_dataB());
                }
                break;
            case MIDI_AFTERTOUCH:
                if (callback_aftertouch) {
                    callback_aftertouch(channel, p.get_dataA(), p.get_dataB());
                }
                break;
            case MIDI_CONTROL_CHANGE:
                if (callback_control_change) {
                    callback_control_change(channel, p.get_dataA(), p.get_dataB());
                }
                break;
            case MIDI_PROGRAM_CHANGE:
                if (callback_program_change) {
                    callback_program_change(channel, p.get_dataB());
                }
                e.a = e.b;
                break;
            case MIDI_CHANNEL_PRESSURE:
                if (callback_channel_pressure) {
                    callback_channel_pressure(channel, p.get_dataB());
                }
                e.a = e.b;
                break;
            case MIDI_PITCHBEND:
                if (callback_pitch_bend) {
                    callback_pitch_bend(channel, p.get_int14() - 8192);
                }
                e.a = p.get_int14();
                e.b = 0;
                break;
        }
        if (callback_event_receive) {
            callback_event_receive(EVENT_MIDI, &e);
        }
    }

    void handle_sysex_message(uint8_t msg) {
        EventMIDI e;
        e.command = msg;
        e.channel = 0;
        e.b       = 0;
        e.a       = 0;
        switch (msg) {
            case MIDI_SYS_EX:
                if (callback_sys_ex) {
                    callback_sys_ex();
                }
                break;
            case MIDI_MTCQFRAME:
                if (callback_mtc_quarter_frame) {
                    callback_mtc_quarter_frame(p.get_dataB());
                }
                e.b = p.get_dataB();
                e.a = e.b;
                break;
            case MIDI_SONG_POSITION_POINTER:
                if (callback_song_position_pointer) {
                    callback_song_position_pointer(p.get_int14());
                }
                e.a = p.get_int14();
                e.b = 0;
                break;
            case MIDI_SONG_SELECT:
                if (callback_song_select) {
                    callback_song_select(p.get_dataB());
                }
                e.b = p.get_dataB();
                e.a = e.b;
                break;
            case MIDI_TUNE_REQ:
                if (callback_tune_req) {
                    callback_tune_req();
                }
                break;
            case MIDI_ENDEX:
                if (callback_end_ex) {
                    callback_end_ex();
                }
                break;
        }
        if (callback_event_receive) {
            callback_event_receive(EVENT_MIDI, &e);
        }
    }

    void handle_rt_message(uint8_t msg) {
        EventMIDI e;
        e.command = msg;
        e.channel = 0;
        e.b       = 0;
        e.a       = 0;
        switch (msg) {
            case MIDI_CLOCK_TICK:
                if (callback_clock_tick) {
                    callback_clock_tick();
                }
                break;
            case MIDI_CLOCK_START:
                if (callback_clock_start) {
                    callback_clock_start();
                }
                break;
            case MIDI_CLOCK_CONTINUE:
                if (callback_clock_continue) {
                    callback_clock_continue();
                }
                break;
            case MIDI_CLOCK_STOP:
                if (callback_clock_stop) {
                    callback_clock_stop();
                }
                break;
            case MIDI_RT_SENSE:
                if (callback_rt_sense) {
                    callback_rt_sense();
                }
                break;
            case MIDI_RT_RESET:
                if (callback_rt_reset) {
                    callback_rt_reset();
                }
                break;
        }
        if (callback_event_receive) {
            callback_event_receive(EVENT_MIDI, &e);
        }
    }

public:
    void register_event_receive(CallbackType2_UI8_VOIDPTR callback) { callback_event_receive = callback; }
    void register_note_off(CallbackType3_UI8 callback) { callback_note_off = callback; }
    void register_note_on(CallbackType3_UI8 callback) { callback_note_on = callback; }
    void register_aftertouch(CallbackType3_UI8 callback) { callback_aftertouch = callback; }
    void register_control_change(CallbackType3_UI8 callback) { callback_control_change = callback; }
    void register_program_change(CallbackType2_UI8 callback) { callback_program_change = callback; }
    void register_channel_pressure(CallbackType2_UI8 callback) { callback_channel_pressure = callback; }
    void register_pitch_bend(CallbackType2_UI8_I16 callback) { callback_pitch_bend = callback; }
    void register_sys_ex(CallbackType0 callback) { callback_sys_ex = callback; }
    void register_mtc_quarter_frame(CallbackType1_UI8 callback) { callback_mtc_quarter_frame = callback; }
    void register_song_position_pointer(CallbackType1_UI16 callback) { callback_song_position_pointer = callback; }
    void register_song_select(CallbackType1_UI8 callback) { callback_song_select = callback; }
    void register_tune_req(CallbackType0 callback) { callback_tune_req = callback; }
    void register_end_ex(CallbackType0 callback) { callback_end_ex = callback; }
    void register_clock_tick(CallbackType0 callback) { callback_clock_tick = callback; }
    void register_clock_start(CallbackType0 callback) { callback_clock_start = callback; }
    void register_clock_continue(CallbackType0 callback) { callback_clock_continue = callback; }
    void register_clock_stop(CallbackType0 callback) { callback_clock_stop = callback; }
    void register_rt_sense(CallbackType0 callback) { callback_rt_sense = callback; }
    void register_rt_reset(CallbackType0 callback) { callback_rt_reset = callback; }

    EVENT_TYPE parse(char midiByte) {
        const uint8_t msg = p.parse(midiByte);
        if (msg > MIDI_NONE) {
            if (p.is_rtmsg(msg)) {
                handle_rt_message(msg);
            } else if (p.is_sysmsg(msg)) {
                handle_sysex_message(msg);
            } else if (p.is_msg(msg)) {
                handle_channel_message(msg);
            }
        }
        return msg;
    }

    std::string message_str(uint8_t msg) {
        return message_str(msg, get_dataA(), get_dataB());
    }

    std::string message_str(uint8_t msg, uint8_t data_a, uint8_t data_b) {
        (void) data_a;
        (void) data_b;
#ifndef MIDI_PARSER_DISABLE_PRINT_MESSAGES
        char const midi_sysmsg_names[16][8] = {
            "SYSEX  ",
            "TMCODE ",
            "SNGPOS ",
            "SNGSEL ",
            "       ",
            "       ",
            "TUNE   ",
            "ENDEX  ",
            "CLOCK  ",
            "       ",
            "START  ",
            "CONT   ",
            "STOP   ",
            "       ",
            "SENSE  ",
            "RESET  ",
        };

        char const midi_msg_names[7][8] = {
            "NOTOFF ",
            "NOTEON ",
            "ATOUCH ",
            "CC CHG ",
            "PRGCHG ",
            "CTOUCH ",
            "P BEND ",
        };
        char buffer[48];
        if (msg != MIDI_NONE) {
            if (p.is_rtmsg(msg)) {
                sprintf(buffer, "RT  : %s 0x%02X", midi_sysmsg_names[msg & 0x0F], (int) msg);
            } else if (p.is_sysmsg(msg)) {
                sprintf(buffer, "SYS : %s 0x%02X", midi_sysmsg_names[msg & 0x0F], (int) msg);
            } else if (p.is_msg(msg)) {
                if (p.msg_value(msg) == MIDIParser::PITCH_BEND) {
                    sprintf(buffer, "%s(0x%02X) CH:0x%02X > 0x%04X",
                            midi_msg_names[(msg >> 4) & 0x07],
                            p.msg_value(msg),
                            p.msg_channel(msg),
                            p.get_int14());
                } else {
                    sprintf(buffer, "%s(0x%02X) CH:0x%02X > 0x%02X, 0x%02X",
                            midi_msg_names[(msg >> 4) & 0x07],
                            p.msg_value(msg),
                            p.msg_channel(msg),
                            p.get_dataA(),
                            p.get_dataB());
                }
            } else {
                sprintf(buffer, "ERR");
            }
        }
        return std::string(buffer).c_str();
#else
        return "";
#endif
    }

    void reset() {
        p.reset();
    }

    MIDIParser parser() {
        return p;
    }

    uint8_t get_message() const { return p.get_message(); }
    uint8_t get_dataA() const { return p.get_dataA(); }
    uint8_t get_dataB() const { return p.get_dataA(); }

private:
    MIDIParser p;
};
