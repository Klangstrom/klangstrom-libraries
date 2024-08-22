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
#include "DisplayDMA2DDrawLibrary.h"

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
    // display_line(x, y, 1024, RGBA(0xFF, 0xFF, 0x00, 0x80));
    // TODO images is currently broken
    // display_image(IMAGE_DATA,
    //               (display_get_width() - IMAGE_WIDTH) / 2,
    //               (display_get_height() - IMAGE_HEIGHT) / 2,
    //               IMAGE_WIDTH,
    //               IMAGE_HEIGHT);

    for (int i = 0; i < MAX_POINTS; ++i) {
        display_set_pixel(points[i].x, points[i].y, BRIGHTNESS(0xFF));
    }

    constexpr uint8_t  BYTES_PER_PIXEL = 4;
    const uint16_t     xx              = x + 20;
    const uint16_t     yy              = y + 20;
    const uint32_t     offset          = (xx + yy * KLST_DISPLAY_WIDTH) * BYTES_PER_PIXEL;
    uint32_t*          destination     = reinterpret_cast<uint32_t*>(LTDC_get_backbuffer_address() + offset);
    constexpr uint32_t width           = 40;
    constexpr uint32_t height          = 40;

    BlitOp op_fill;
    op_fill.operation      = BLIT_OP_FILL;          // (used)
    op_fill.pSrc           = nullptr;               // -
    op_fill.pClut          = nullptr;               // -
    op_fill.pDst           = destination;           // (used) framebuffer address
    op_fill.nSteps         = width;                 // (used) width
    op_fill.nLoops         = height;                // (used) height
    op_fill.srcLoopStride  = 0;                     // -
    op_fill.dstLoopStride  = KLST_DISPLAY_WIDTH;    // (used) buffer width
    op_fill.color          = 0xFFFF8000;            // color
    op_fill.alpha          = 0xFF;                  // (used)
    op_fill.srcFormat      = 0;                     // -
    op_fill.dstFormat      = DMA2D_OUTPUT_ARGB8888; // i.e `output` ( currently fixed to DMA2D_OUTPUT_ARGB8888 )
    op_fill.replaceBgAlpha = false;                 // (used) only in `BLIT_OP_FILL_WITH_ALPHA`
    op_fill.replaceFgAlpha = false;                 // -

    /* Clear transfer flags */
    WRITE_REG(DMA2D->IFCR, DMA2D_FLAG_TC | DMA2D_FLAG_CE | DMA2D_FLAG_TE);
    while ((READ_REG(DMA2D->CR) & DMA2D_CR_START) != 0U)
        ;
    /* Clear transfer flags */
    setupDataFill(op_fill);

    BlitOp op_data;
    op_data.operation      = BLIT_OP_COPY_ARGB8888; // (used)
    op_data.pSrc           = nullptr;               // image data address
    op_data.pClut          = nullptr;               // -
    op_data.pDst           = 0;                     // framebuffer address
    op_data.nSteps         = 0;                     // (used)
    op_data.nLoops         = 0;                     // (used)
    op_data.srcLoopStride  = 0;                     // (used)
    op_data.dstLoopStride  = 0;                     // (used)
    op_data.color          = 0xFFFFFFFF;            // color
    op_data.alpha          = 0xFF;                  // (used)*
    op_data.srcFormat      = DMA2D_INPUT_ARGB8888;  // (used) i.e `input`
    op_data.dstFormat      = DMA2D_OUTPUT_ARGB8888; // (used) i.e `output`
    op_data.replaceBgAlpha = false;                 // (used)
    op_data.replaceFgAlpha = false;                 // (used)
    // setupDataCopy(op_data);
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
