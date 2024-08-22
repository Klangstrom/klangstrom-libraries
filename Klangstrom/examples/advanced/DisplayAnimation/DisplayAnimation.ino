/**
 * this example demonstrates how to use the display with touch panel
 */

#include <vector>

#include "main.h"
#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "Display.h"

#include "KLST--ICON--128x128.h"
#include "DisplayDrawLibrary_DMA2D_STM32.h"

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

uint16_t x = 100;
uint16_t y = 100;

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

extern DMA2D_HandleTypeDef hdma2d;
uint8_t move = 0;
void display_update_event() {
    display_clear(BRIGHTNESS(0x00));

    display_rect_fill(30, 30, 20, 20, RGB(0xFF, 0x00, 0x00));
    display_rect_fill(50, 30, 20, 20, RGB(0x00, 0xFF, 0x00));
    display_rect_fill(70, 30, 20, 20, RGB(0x00, 0x00, 0xFF));
    display_rect_fill(display_get_width() - 40, display_get_height() - 40, 20, 20, BRIGHTNESS(0xFF));
    display_rect_fill(x, y, 20, 20, RGBA(0xFF, 0xFF, 0xFF, 0x80));
    display_rect_fill(x + 5, y + 5, 40, 40, 0x80FF8000);

    // display_line_horizontal(x, y, 1024, RGBA(0xFF, 0xFF, 0x00, 0x80));

    for (int i = 0; i < MAX_POINTS; ++i) {
        display_pixel(points[i].x, points[i].y, BRIGHTNESS(0xFF));
    }

    display_image(IMAGE_DATA, move++, y - IMAGE_HEIGHT / 2, IMAGE_WIDTH, IMAGE_HEIGHT);
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
