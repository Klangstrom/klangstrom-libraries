/**
 * this example demonstrates how to use the display with touch panel
 */

#include <vector>

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "Display.h"

#include "KLST--ICON--128x128.h"

class PVector {
public:
    PVector(uint16_t x, uint16_t y)
        : x(x),
          y(y) {}

    PVector()
        : x(0),
          y(0) {}

    uint16_t x;
    uint16_t y;
};

// std::vector<PVector> points;
constexpr uint16_t MAX_POINTS     = 64;
uint16_t           points_counter = 0;
PVector            points[MAX_POINTS];

uint16_t x = 0;
uint16_t y = 0;

void setup() {
    system_init();

    display_init(true);
    display_set_backlight(0.5f);
    display_enable_automatic_update(true);
}

void loop() {
    console_println("...");
    delay(1000);
}

void display_update_event() {
    display_clear(BRIGHTNESS(0x00));
    display_clear(RGBA(0x00, 0x00, 0x00, 0xFF));
    display_rect(30, 30, 20, 20, RGB(0xFF, 0x00, 0x00));
    display_rect(50, 30, 20, 20, RGB(0x00, 0xFF, 0x00));
    display_rect(70, 30, 20, 20, RGB(0x00, 0x00, 0xFF));
    display_rect(x, y, 20, 20, RGBA(0xFF, 0xFF, 0xFF, 0x80));
    // TODO images is currently broken
    // display_image(IMAGE_DATA,
    //               (display_get_width() - IMAGE_WIDTH) / 2,
    //               (display_get_height() - IMAGE_HEIGHT) / 2,
    //               IMAGE_WIDTH,
    //               IMAGE_HEIGHT);

    for (int i = 0; i < MAX_POINTS; ++i) {
        display_set_pixel(points[i].x, points[i].y, BRIGHTNESS(0xFF));
    }
}

void display_touch_event(TouchEvent* touchevent) {
    console_clear();
    console_println("TOUCH EVENT: %d", touchevent->number_of_touches);
    for (int i = 0; i < touchevent->number_of_touches; ++i) {
        console_println("TOUCH %d    : %d,%d", i, touchevent->x[i], touchevent->y[i]);
        points[points_counter].x = touchevent->x[i];
        points[points_counter].y = touchevent->y[i];
        points_counter++;
        points_counter %= MAX_POINTS;
        if (i == 0) {
            x = touchevent->x[i];
            y = touchevent->y[i];
        }
        if (i == 1) {
            const float brightness = static_cast<float>(touchevent->x[i]) / 480.0f;
            display_set_backlight(brightness);
        }
    }
}
