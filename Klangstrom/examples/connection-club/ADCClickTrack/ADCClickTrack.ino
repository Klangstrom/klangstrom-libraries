/**
 * this example demonstrates how to use the analog-digital-converter (ADC) and the digital-analog-converter (DAC).
 */

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "DACADC.h"
#include "Display.h"
#include "Draw.h"
#include "KLSTFont_5x8.h"
#include "Key.h"

Key* key_left;
Key* key_right;

void setup() {
    system_init();

    key_init();
    key_left  = key_create(KEY_LEFT);
    key_right = key_create(KEY_RIGHT);

    dac_init();

    display_init(true, NO_TOUCHPANEL);
    display_enable_automatic_update(true);
    draw_set_text(&Font_5x8);
}

void loop() {
    console_println("...");
    delay(500);
}

void display_update_event() {
    draw_clear(GRAY(0x00));

    constexpr uint8_t scale = 16;
        std::string message = "XX";
        draw_fill(color(0xFF));
        draw_set_background_color(color(0xFF, 0x00));
        draw_text(display_get_width() / 2 - message.size() * draw_get_text()->width * scale / 2,
                  display_get_height() / 2 - draw_get_text()->height * scale / 2,
                  message.c_str(),
                  scale);
}

void key_event(const Key* key) {
    if (key->device_id == key_right->device_id) {
    }
    if (key->device_id == key_left->device_id) {
    }
}
