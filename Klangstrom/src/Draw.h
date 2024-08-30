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

#include "Display.h"
#include "BitmapFont.h"

/* internal color format is fixed to ARGB */
/* internal point type is int16_t */

typedef enum {
    CENTERED = 0x01,
    RIGHT    = 0x02,
    LEFT     = 0x03
} TextAlign;

/* NOTE these functions are deliberately NOT placed in `extern "C" {}` block to allow overloading */

// TODO remove color parameter and use global color variables
// TODO consider getting rid of all `*_stroke` and `*_fill` functions and evaluate color flags instead ( processing.org style )

void     draw_clear(uint32_t color);
void     draw_fill(uint32_t color);
void     draw_no_fill();
void     draw_stroke(uint32_t color);
void     draw_no_stroke();
void     draw_set_pixel(int16_t x, int16_t y, uint32_t color);
uint32_t draw_get_pixel(int16_t x, int16_t y);
void     draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color);
void     draw_line_arbitrary(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color);
void     draw_line_horizontal(int16_t x, int16_t y, uint16_t length, uint32_t color);
void     draw_line_vertical(int16_t x, int16_t y, uint16_t length, uint32_t color);
void     draw_rect(int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color, bool filled);
void     draw_rect_stroke(int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color);
void     draw_rect_fill(int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color);
void     draw_circle(int16_t x, int16_t y, int16_t radius);                        // TODO
void     draw_circle_stroke(int16_t x, int16_t y, int16_t radius, uint32_t color); // TODO
void     draw_circle_fill(int16_t x, int16_t y, int16_t radius);                   // TODO
void     draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color);
void     draw_triangle_stroke(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color);
void     draw_triangle_fill(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color);
void     draw_polygon(const std::vector<Point>& points); // TODO
void     draw_polygon_stroke(const std::vector<Point>& points, uint32_t color);
void     draw_polygon_fill(const std::vector<Point>& points, uint32_t color); // TODO WIP
void     draw_ellipse(int16_t x, int16_t y, int16_t radius_x, int16_t radius_y);
void     draw_ellipse_stroke(int16_t x, int16_t y, int16_t radius_x, int16_t radius_y, uint32_t color);
void     draw_ellipse_fill(int16_t x, int16_t y, int16_t radius_x, int16_t radius_y, uint32_t color);
void     draw_image(uint32_t* data, int16_t x, int16_t y, uint16_t width, uint16_t height);
void     draw_set_text(BitmapFont* font);
void     draw_set_text_background_color(uint32_t color);
void     draw_set_text_align(TextAlign align); // TODO
void     draw_char(BitmapFont* font, int16_t x, int16_t y, uint8_t ascii_char, uint32_t color, uint32_t background_color);
void     draw_text(BitmapFont* font, int16_t x, int16_t y, const char* text, TextAlign align, uint32_t color, uint32_t background_color);
