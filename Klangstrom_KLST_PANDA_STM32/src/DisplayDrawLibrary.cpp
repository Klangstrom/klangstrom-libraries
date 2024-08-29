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

#include "Klangstrom.h"
#ifdef KLST_PERIPHERAL_ENABLE_DISPLAY
#ifdef KLST_PANDA_STM32

#include <vector>

#include "main.h"
#include "Console.h"
#include "Display.h"

/* NOTE these functions are deliberately not placed in `extern "C" {}` block to allow overloading */

void display_pixel(const uint16_t x, const uint16_t y, const uint32_t color) {
    if (x < KLST_DISPLAY_WIDTH && y < KLST_DISPLAY_HEIGHT) {
        const uint32_t offset                                                       = x + y * KLST_DISPLAY_WIDTH;
        reinterpret_cast<volatile uint32_t*>(LTDC_get_backbuffer_address())[offset] = color;
    }
}

uint32_t display_get_pixel(const uint16_t x, const uint16_t y) {
    if (x < KLST_DISPLAY_WIDTH && y < KLST_DISPLAY_HEIGHT) {
        const uint32_t offset = x + y * KLST_DISPLAY_WIDTH;
        return reinterpret_cast<volatile uint32_t*>(LTDC_get_backbuffer_address())[offset];
    }
    return 0;
}

#define ABS(X) ((X) > 0 ? (X) : -(X))

void display_circle_stroke(const uint16_t x, const uint16_t y, const uint16_t radius, const uint32_t color) {
    int32_t  decision  = 3 - (radius << 1);
    uint32_t current_x = 0;
    uint32_t current_y = radius;

    while (current_x <= current_y) {
        display_pixel((x + current_x), (y - current_y), color);
        display_pixel((x - current_x), (y - current_y), color);
        display_pixel((x + current_y), (y - current_x), color);
        display_pixel((x - current_y), (y - current_x), color);
        display_pixel((x + current_x), (y + current_y), color);
        display_pixel((x - current_x), (y + current_y), color);
        display_pixel((x + current_y), (y + current_x), color);
        display_pixel((x - current_y), (y + current_x), color);
        if (decision < 0) {
            decision += (current_x << 2) + 6;
        } else {
            decision += ((current_x - current_y) << 2) + 10;
            current_y--;
        }
        current_x++;
    }
}

void display_line(const uint16_t x0,
                  const uint16_t y0,
                  const uint16_t x1,
                  const uint16_t y1,
                  const uint32_t color) {
    int16_t xinc1;
    int16_t xinc2;
    int16_t yinc1;
    int16_t yinc2;
    int16_t den;
    int16_t num;
    int16_t num_add;
    int16_t num_pixels;

    const int16_t deltax = ABS(x1 - x0); /* The difference between the x's */
    const int16_t deltay = ABS(y1 - y0); /* The difference between the y's */
    int16_t       x      = x0;           /* Start x off at the first pixel */
    int16_t       y      = y0;           /* Start y off at the first pixel */

    /* The x-values are increasing */
    if (x1 >= x0) {
        xinc1 = 1;
        xinc2 = 1;
    } else {
        /* The x-values are decreasing */
        xinc1 = -1;
        xinc2 = -1;
    }
    /* The y-values are increasing */
    if (y1 >= y0) {
        yinc1 = 1;
        yinc2 = 1;
    } else {
        /* The y-values are decreasing */
        yinc1 = -1;
        yinc2 = -1;
    }

    /* There is at least one x-value for every y-value */
    if (deltax >= deltay) {
        xinc1      = 0; /* Don't change the x when numerator >= denominator */
        yinc2      = 0; /* Don't change the y for every iteration */
        den        = deltax;
        num        = deltax / 2;
        num_add    = deltay;
        num_pixels = deltax; /* There are more x-values than y-values */
    } else {
        /* There is at least one y-value for every x-value */
        xinc2      = 0; /* Don't change the x for every iteration */
        yinc1      = 0; /* Don't change the y when numerator >= denominator */
        den        = deltay;
        num        = deltay / 2;
        num_add    = deltax;
        num_pixels = deltay; /* There are more y-values than x-values */
    }

    for (int16_t curpixel = 0; curpixel <= num_pixels; curpixel++) {
        display_pixel(x, y, color); /* Draw the current pixel */
        num += num_add;             /* Increase the numerator by the top of the fraction */
        /* Check if numerator >= denominator */
        if (num >= den) {
            num -= den; /* Calculate the new numerator value */
            x += xinc1; /* Change the x as appropriate */
            y += yinc1; /* Change the y as appropriate */
        }
        x += xinc2; /* Change the x as appropriate */
        y += yinc2; /* Change the y as appropriate */
    }
}

void display_rect(const uint16_t x,
                  const uint16_t y,
                  const uint16_t width,
                  const uint16_t height,
                  const uint32_t color,
                  const bool     filled) {
    if (filled) {
        display_rect_fill(x, y, width, height, color);
    } else {
        display_rect_stroke(x, y, width, height, color);
    }
}

void display_rect_stroke(const uint16_t x,
                         const uint16_t y,
                         const uint16_t width,
                         const uint16_t height,
                         const uint32_t color) {
    /* Draw horizontal lines */
    display_line_horizontal(x, y, width, color);
    display_line_horizontal(x, (y + height), width, color);

    /* Draw vertical lines */
    display_line_vertical(x, y, height, color);
    display_line_vertical((x + width), y, height, color);
}

void display_triangle_fill(const uint16_t x0,
                           const uint16_t x1,
                           const uint16_t x2,
                           const uint16_t y0,
                           const uint16_t y1,
                           const uint16_t y2,
                           const uint32_t color) {
    int16_t xinc1;
    int16_t xinc2;
    int16_t yinc1;
    int16_t yinc2;
    int16_t den;
    int16_t num;
    int16_t num_add;
    int16_t num_pixels;

    const int16_t deltax = ABS(x1 - x0); /* The difference between the x's */
    const int16_t deltay = ABS(y1 - y0); /* The difference between the y's */
    int16_t       x      = x0;           /* Start x off at the first pixel */
    int16_t       y      = y0;           /* Start y off at the first pixel */

    /* The x-values are increasing */
    if (x1 >= x0) {
        xinc1 = 1;
        xinc2 = 1;
    } else {
        /* The x-values are decreasing */
        xinc1 = -1;
        xinc2 = -1;
    }

    /* The y-values are increasing */
    if (y1 >= y0) {
        yinc1 = 1;
        yinc2 = 1;
    } else {
        /* The y-values are decreasing */
        yinc1 = -1;
        yinc2 = -1;
    }

    /* There is at least one x-value for every y-value */
    if (deltax >= deltay) {
        xinc1      = 0; /* Don't change the x when numerator >= denominator */
        yinc2      = 0; /* Don't change the y for every iteration */
        den        = deltax;
        num        = deltax / 2;
        num_add    = deltay;
        num_pixels = deltax; /* There are more x-values than y-values */
    } else {
        /* There is at least one y-value for every x-value */
        xinc2      = 0; /* Don't change the x for every iteration */
        yinc1      = 0; /* Don't change the y when numerator >= denominator */
        den        = deltay;
        num        = deltay / 2;
        num_add    = deltax;
        num_pixels = deltay; /* There are more y-values than x-values */
    }

    for (int16_t curpixel = 0; curpixel <= num_pixels; curpixel++) {
        display_line(x, y, x2, y2, color);

        num += num_add; /* Increase the numerator by the top of the fraction */
        if (num >= den) /* Check if numerator >= denominator */
        {
            num -= den; /* Calculate the new numerator value */
            x += xinc1; /* Change the x as appropriate */
            y += yinc1; /* Change the y as appropriate */
        }
        x += xinc2; /* Change the x as appropriate */
        y += yinc2; /* Change the y as appropriate */
    }
}

void display_polygon_stroke(const std::vector<Point>& points, const uint32_t color) {
    if (points.size() < 2) {
        return;
    }
    for (size_t i = 0; i < points.size() - 1; i++) {
        display_line(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, color);
    }
    display_line(points[0].x, points[0].y, points[points.size() - 1].x, points[points.size() - 1].y, color);
}

void display_polygon_fill(const std::vector<Point>& points, const uint32_t color) {
    int16_t  x1 = 0;
    int16_t  y1 = 0;
    uint16_t image_right;
    uint16_t image_bottom;
    uint16_t image_left = image_right = points[0].x;
    uint16_t image_top = image_bottom = points[0].y;

    for (int16_t counter = 1; counter < points.size(); counter++) {
        const int16_t pixelX = points[counter].x;
        if (pixelX < image_left) {
            image_left = pixelX;
        }
        if (pixelX > image_right) {
            image_right = pixelX;
        }

        const int16_t pixelY = points[counter].y;
        if (pixelY < image_top) {
            image_top = pixelY;
        }
        if (pixelY > image_bottom) {
            image_bottom = pixelY;
        }
    }

    if (points.size() < 2) {
        return;
    }

    const int16_t X_center = (image_left + image_right) / 2;
    const int16_t Y_center = (image_bottom + image_top) / 2;

    const int16_t X_first = points[0].x;
    const int16_t Y_first = points[0].y;

    for (size_t i = 0; i < points.size() - 1; i++) {
        const int16_t X = points[i].x;
        const int16_t Y = points[i].y;
        x1              = points[i + 1].x;
        y1              = points[i + 1].y;

        display_triangle_fill(X, x1, X_center, Y, y1, Y_center, color);
        display_triangle_fill(X, X_center, x1, Y, Y_center, y1, color);
        display_triangle_fill(X_center, x1, X, Y_center, y1, Y, color);
    }

    display_triangle_fill(X_first, x1, X_center, Y_first, y1, Y_center, color);
    display_triangle_fill(X_first, X_center, x1, Y_first, Y_center, y1, color);
    display_triangle_fill(X_center, x1, X_first, Y_center, y1, Y_first, color);
}

void display_ellipse_fill(const int      x,
                          const int      y,
                          const int      radius_x,
                          const int      radius_y,
                          const uint32_t color) {
    int         xx   = 0;
    int         yy   = -radius_y;
    int         err  = 2 - 2 * radius_x;
    const float rad1 = radius_x;
    const float rad2 = radius_y;
    const float k    = rad2 / rad1;

    do {
        display_line_horizontal(x - static_cast<uint16_t>(xx / k), y + yy, (2 * static_cast<uint16_t>(xx / k) + 1), color);
        display_line_horizontal(x - static_cast<uint16_t>(xx / k), y - yy, 2 * static_cast<uint16_t>(xx / k) + 1, color);

        int e2 = err;
        if (e2 <= xx) {
            err += ++xx * 2 + 1;
            if (-yy == xx && e2 <= yy) {
                e2 = 0;
            }
        }
        if (e2 > yy) {
            err += ++yy * 2 + 1;
        }
    } while (yy <= 0);
}

void display_ellipse_stroke(const int      x,
                            const int      y,
                            const int      radius_x,
                            const int      radius_y,
                            const uint32_t color) {
    int         xx   = 0;
    int         yy   = -radius_y;
    int         err  = 2 - 2 * radius_x;
    const float rad1 = radius_x;
    const float rad2 = radius_y;
    const float k    = rad2 / rad1;

    do {
        display_pixel(x - static_cast<uint16_t>(xx / k), y + yy, color);
        display_pixel(x + static_cast<uint16_t>(xx / k), y + yy, color);
        display_pixel(x + static_cast<uint16_t>(xx / k), y - yy, color);
        display_pixel(x - static_cast<uint16_t>(xx / k), y - yy, color);

        int e2 = err;
        if (e2 <= xx) {
            err += ++xx * 2 + 1;
            if (-yy == xx && e2 <= yy) {
                e2 = 0;
            }
        }
        if (e2 > yy) {
            err += ++yy * 2 + 1;
        }
    } while (yy <= 0);
}

static void DrawChar(BitmapFont*    font,
                     const uint16_t x,
                     uint16_t       y,
                     const uint8_t* c,
                     const uint32_t color,
                     const uint32_t background_color) {
    uint32_t line;

    const uint16_t height = font->Height;
    const uint16_t width  = font->Width;
    const uint8_t  offset = 8 * ((width + 7) / 8) - width;

    for (uint16_t i = 0; i < height; i++) {
        const uint8_t* pchar = const_cast<uint8_t*>(c) + (width + 7) / 8 * i;
        switch ((width + 7) / 8) {
            case 1:
                line = pchar[0];
                break;
            case 2:
                line = pchar[0] << 8 | pchar[1];
                break;
            case 3:
            default:
                line = pchar[0] << 16 | (pchar[1] << 8) | pchar[2];
                break;
        }

        for (uint16_t j = 0; j < width; j++) {
            if (line & (1 << (width - j + offset - 1))) {
                display_pixel(x + j, y, color);
            } else {
                if (GET_ALPHA(background_color) > 0x00) {
                    display_pixel((x + j), y, background_color);
                }
            }
        }
        y++;
    }
}

void display_char(BitmapFont*    font,
                  const uint16_t x,
                  const uint16_t y,
                  const uint8_t  ascii_char,
                  const uint32_t color,
                  const uint32_t background_color) {
    DrawChar(font,
             x,
             y,
             &font->table[(ascii_char - ' ') * font->Height * ((font->Width + 7) / 8)],
             color,
             background_color);
}

void display_text(BitmapFont*     font,
                  const uint16_t  x,
                  const uint16_t  y,
                  const char*     text,
                  const TextAlign align,
                  const uint32_t  color,
                  const uint32_t  background_color) {
    uint16_t ref_column;
    uint16_t i    = 0;
    uint32_t size = 0;
    auto     ptr  = text;

    /* Get the text size */
    while (*ptr++) {
        size++;
    }

    /* Characters number per line */
    const uint32_t xsize = display_get_width() / font->Width;

    switch (align) {
        case CENTERED: {
            ref_column = x + (xsize - size) * font->Width / 2;
            break;
        }
        case LEFT: {
            ref_column = x;
            break;
        }
        case RIGHT: {
            ref_column = -x + (xsize - size) * font->Width;
            break;
        }
        default: {
            ref_column = x;
            break;
        }
    }

    /* Check that the Start column is located in the screen */
    if ((ref_column < 1) || (ref_column >= 0x8000)) {
        ref_column = 1;
    }

    /* Send the string character by character on LCD */
    while (*text != 0 & (display_get_width() - i * font->Width & 0xFFFF) >= font->Width) {
        /* Display one character on LCD */
        display_char(font, ref_column, y, *text, color, background_color);
        /* Decrement the column position by 16 */
        ref_column += font->Width;
        /* Point on the next character */
        text++;
        i++;
    }
}

#endif // KLST_PANDA_STM32
#endif // KLST_PERIPHERAL_ENABLE_DISPLAY
