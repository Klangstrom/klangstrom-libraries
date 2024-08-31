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

#include "KlangstromEnvironment.h"
#ifdef KLST_ARCH_IS_EMU

#ifdef KLST_PANDA_EMU
#define KLST_EMU_DISPLAY_WIDTH 480
#define KLST_EMU_DISPLAY_HEIGHT 272
#endif // KLST_PANDA_EMU

#include "Console.h"
#include "PVector.h"
#include "KlangstromEmulator.h"
#include "Display.h"
#include "DisplayDrawInterface.h"
#include "UmgebungFunctions.h"

class DrawableDisplay final : public Drawable {
public:
    const float width;
    const float height;

    DrawableDisplay(const uint16_t display_width,
                    const uint16_t display_height) : width(display_width),
                                                     height(display_height), mBuffers{} {
        mBuffers[0] = new PImage(display_width, display_height, 4);
        mBuffers[1] = new PImage(display_width, display_height, 4);
    }

    void draw(PGraphics* g_ptr) override {
        PGraphics& g      = *g_ptr;
        PImage&    buffer = *mBuffers[0];

        g.pushMatrix();
        g.translate(position.x, position.y);

        g.fill(1);
        g.textSize(KlangstromEmulator::DEFAULT_FONT_SIZE * 0.5f);
        g.text("DISPLAY", 0, 0);
        g.noFill();

        const uint32_t length = buffer.width * buffer.height * buffer.channels;
        for (uint32_t i = 0; i < length; i++) {
            buffer.color_data()[i] = 0xFF;
        }
        buffer.update();
        g.image(&buffer, 0, 0);

        g.stroke(1, 0.5f);
        g.rect(0, 0, width, height);

        g.popMatrix();
    }

    void set_position(const float x, const float y) {
        position.set(x, y);
    }

private:
            DrawableDisplay() = delete;
    PVector position;
    PImage* mBuffers[2];
};

#ifdef __cplusplus
extern "C" {
#endif

static DrawableDisplay* display     = nullptr;
static bool             initialized = false;

bool display_init_BSP(TouchPanelMode touch_panel_mode) {
    if (!initialized) {
        initialized = true;
        display     = new DrawableDisplay(KLST_EMU_DISPLAY_WIDTH, KLST_EMU_DISPLAY_HEIGHT);
        display->set_position(KlangstromEmulator::instance()->width - (KLST_EMU_DISPLAY_WIDTH + 20), 50);
        KlangstromEmulator::instance()->register_drawable(display);
    }
    return true;
}

void display_deinit() {
    if (initialized && display != nullptr) {
        KlangstromEmulator::instance()->unregister_drawable(display);
        delete display;
        display = nullptr;
    }
}

int16_t display_get_width() {
    // TODO make this platform specific
    if (display == nullptr) {
        return 0;
    }
    return display->width;
}

int16_t display_get_height() {
    // TODO make this platform specific
    if (display == nullptr) {
        return 0;
    }
    return display->height;
}

void display_set_backlight(const float brightness) { // TODO implement
    (void) brightness;
}

void display_enable_automatic_update(const bool sync_to_v_blank) { // TODO implement
    (void) sync_to_v_blank;
}

void display_swap_buffer() { // TODO implement
}

void display_switch_on() { // TODO implement
}

void display_switch_off() { // TODO implement
}

volatile uint32_t display_get_buffer_address() { return 0; }

volatile uint32_t* display_get_buffer() { return nullptr; }

void touch_init(TouchPanelMode touch_panel_mode) { // TODO implement
    (void) touch_panel_mode;
}

void touch_read(TouchEvent* touchevent) { // TODO implement
    (void) touchevent;
}

bool touch_has_event() { // TODO implement
    return false;
}

#ifdef __cplusplus
}
#endif

void display_clear_BSP(uint32_t color) {}

void display_set_pixel_BSP(uint16_t x, uint16_t y, uint32_t color) {}

void display_set_pixel_alpha_BSP(uint16_t x, uint16_t y, uint32_t color) {}

uint32_t display_get_pixel_BSP(uint16_t x, uint16_t y) { return 0; }

void display_rect_fill_BSP(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {}

void display_line_horizontal_BSP(uint16_t x, uint16_t y, uint16_t length, uint32_t color) {}

void display_line_vertical_BSP(uint16_t x, uint16_t y, uint16_t length, uint32_t color) {}

void display_image_BSP(uint32_t* data, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {}

void display_char_BSP(BitmapFont* font, uint16_t x, uint16_t y, uint8_t ascii_char, uint32_t color, uint32_t background_color) {}

#endif // KLST_ARCH_IS_EMU