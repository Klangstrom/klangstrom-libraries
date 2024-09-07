/**
 * this example demonstrates how to use the analog-digital-converter (ADC) and the digital-analog-converter (DAC).
 */

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "DACADC.h"
#include "Beat.h"
#include "Display.h"
#include "Draw.h"
#include "KLSTFont_5x8.h"
#include "Key.h"
#include "Encoder.h"

Beat beat;
Key* key_left;
Key* key_right;

bool     click_toggle   = false;
uint16_t m_beat_counter = 0;
int      bpm            = 480;

Encoder* encoder_LEFT;
int32_t  previous_rotation_LEFT = 0;


void setup() {
    system_init();

    key_init();
    key_left  = key_create(KEY_LEFT);
    key_right = key_create(KEY_RIGHT);

    dac_init();

    display_init(true, NO_TOUCHPANEL);
    display_enable_automatic_update(true);
    draw_set_text(&Font_5x8);

    beat.init(7);
    beat.set_bpm(bpm);
    beat.pause();

    encoder_LEFT = encoder_create(ENCODER_LEFT);
}

void loop() {
    console_println("...");
    delay(500);
}

void beat_event(const uint8_t beat_id, const uint16_t beat_counter) {
    m_beat_counter          = beat_counter;
    const float click_value = click_toggle ? 1.0f : 0.0f;
    dac_write(click_value);
    click_toggle = !click_toggle;
}

void display_update_event() {
    draw_clear(GRAY(0x00));

    constexpr uint8_t scale = 16;
    if (beat.is_running()) {
        std::string message;
        if (m_beat_counter % 4 == 0) {
            message = "B" + std::to_string(m_beat_counter % 16 / 4);
        } else {
            message = "--";
        }
        draw_fill(color(0xFF));
        draw_set_background_color(color(0xFF, 0x00));
        draw_text(display_get_width() / 2 - message.size() * draw_get_text()->width * scale / 2,
                  display_get_height() / 2 - draw_get_text()->height * scale / 2,
                  message.c_str(),
                  scale);
    } else {
        draw_fill(color(0xFF));
        draw_set_background_color(color(0xFF, 0x00));
        const std::string message = "XX";
        constexpr uint8_t scale   = 16;
        draw_text(display_get_width() / 2 - message.size() * draw_get_text()->width * scale / 2,
                  display_get_height() / 2 - draw_get_text()->height * scale / 2,
                  message.c_str(),
                  scale);
    }
    const std::string bpm_message = std::to_string(bpm / 4) + " BPM";
    draw_text(display_get_width() / 2 - bpm_message.size() * draw_get_text()->width / 2,
              display_get_height() / 2 + draw_get_text()->height * scale / 2,
              bpm_message.c_str());
}

void key_event(const Key* key) {
    if (key->device_id == key_right->device_id) {
        beat.start();
    }
    if (key->device_id == key_left->device_id) {
        beat.pause();
    }
}

void encoder_event(Encoder* encoder, uint8_t event) {
    if (event == ENCODER_EVENT_ROTATION) {
        const int32_t rotation_delta = encoder->rotation - previous_rotation_LEFT;
        previous_rotation_LEFT       = encoder->rotation;
        bpm += rotation_delta * 4;
        if (bpm < 0) {
            bpm = 0;
        }
        if (bpm > 960) {
            bpm = 960;
        }
        beat.set_bpm(bpm);
    }
}
