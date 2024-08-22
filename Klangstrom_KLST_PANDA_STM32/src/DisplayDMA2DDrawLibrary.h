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

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct colortype Types.hpp touchgfx/hal/Types.hpp
 *
 * @brief Type for representing a color.
 *
 *        This type can contain a color value. Note that in order to maintain backwards
 *        compatibility, casting this type to an integral value will yield a 16-bit value. To
 *        extract a 24/32-bit color from this type, use the getColor32 function.
 */
struct colortype {
    /**
     * @fn colortype()
     *
     * @brief Default constructor.
     *
     *        Default constructor. Creates a black (0) color.
     */
    colortype() : color(0) {}

    /**
     * @fn colortype(uint32_t col)
     *
     * @brief Constructor.
     *
     *        Contructor which creates a colortype with the given color. Use
     *        Color::getColorFrom24BitRGB() to create a color that will work on both 16 bit LCD
     *        and 24 bit LCD.
     *
     * @param col The color.
     *
     * @see Color::getColorFrom24BitRGB()
     */
    colortype(uint32_t col) {
        color = col;
    }

    /**
     * @fn inline uint32_t getColor32() const
     *
     * @brief Gets color 32 as a 32bit value suitable for passing to Color::getRedColor(),
     *        Color::getGreenColor() and Color::getBlueColor() which will handle both 16 bit
     *        colortypes and 24 bit colortypes.
     *
     * @return The color 32.
     *
     * @see Color::getRedColor()
     * @see Color::getGreenColor()
     * @see Color::getBlueColor()
     */
    inline uint32_t getColor32() const {
        return color;
    }

    /**
     * @fn operator uint16_t() const
     *
     * @brief Cast that converts the given colortype to an uint16_t.
     *
     *        Cast that converts the given colortype to an uint16_t. Provided only for backward
     *        compatibility. Not recommended to use.
     *
     * @return The result of the operation.
     */
    operator uint16_t() const {
        return (uint16_t) color;
    }

    uint32_t color; ///< The color
};

/**
 * @enum BlitOperations
 * @brief The BlitOp operations.
 */
enum BlitOperations {
    BLIT_OP_COPY                         = 1 << 0, ///< Copy the source to the destination
    BLIT_OP_FILL                         = 1 << 1, ///< Fill the destination with color
    BLIT_OP_COPY_WITH_ALPHA              = 1 << 2, ///< Copy the source to the destination using the given alpha
    BLIT_OP_FILL_WITH_ALPHA              = 1 << 3, ///< Fill the destination with color using the given alpha
    BLIT_OP_COPY_WITH_TRANSPARENT_PIXELS = 1 << 4, ///< Deprecated, ignored. (Copy the source to the destination, but not the transparent pixels)
    BLIT_OP_COPY_ARGB8888                = 1 << 5, ///< Copy the source to the destination, performing per-pixel alpha blending
    BLIT_OP_COPY_ARGB8888_WITH_ALPHA     = 1 << 6, ///< Copy the source to the destination, performing per-pixel alpha blending and blending the result with an image-wide alpha
    BLIT_OP_COPY_A4                      = 1 << 7, ///< Copy 4-bit source text to destination, performing per-pixel alpha blending
    BLIT_OP_COPY_A8                      = 1 << 8  ///< Copy 8-bit source text to destination, performing per-pixel alpha blending
};

/**
 * @struct BlitOp BlitOp.hpp touchgfx/hal/BlitOp.hpp
 * @brief BlitOp instances carry the required information for performing operations on the LCD
 *        (frame buffer) using DMA.
 */
struct BlitOp {
    uint32_t        operation; ///< The operation to perform @see BlitOperations
    const uint16_t* pSrc;      ///< Pointer to the source (pixels or indexes)
    const uint8_t*  pClut;     ///< Pointer to the source CLUT entires
    uint32_t*       pDst;      ///< Pointer to the destination
    // uint16_t*       pDst;           ///< Pointer to the destination
    uint16_t  nSteps;         ///< The number of pixels in a line
    uint16_t  nLoops;         ///< The number of lines
    uint16_t  srcLoopStride;  ///< The number of bytes to stride the source after every loop
    uint16_t  dstLoopStride;  ///< The number of bytes to stride the destination after every loop
    colortype color;          ///< Color to fill
    uint8_t   alpha;          ///< The alpha to use
    uint8_t   srcFormat;      ///< The source format @see BitmapFormat
    uint8_t   dstFormat;      ///< The destination format @see BitmapFormat
    bool      replaceBgAlpha; ///< Replace the background per pixel alpha value with 255 = solid
    bool      replaceFgAlpha; ///< Replace the fourground per pixel alpha value with 255 = solid
};

void tearDown();
void flushLine(uint32_t* addr, int sizebytes);
void InvalidateCache();
void FlushCache();

/**
 * Draw a horizontal line (one pixel high) using the given color.
 *
 * @param [in] ptr   The pointer to the position in the framebuffer.
 * @param      count Number of pixels to draw.
 * @param      color The color.
 * @param      alpha The alpha.
 */
void lineFromColor(uint32_t* const ptr,
                   const int16_t   count,
                   const uint32_t  color,
                   const uint8_t   alpha);

/**
 * Draw a horizontal line (one pixel high) using pixels from the given data pointer (ARGB8888
 * data).
 *
 * @param [in] ptr   The pointer to the position in the framebuffer.
 * @param      data  The ARGB8888 data.
 * @param      length Number of pixels to draw.
 * @param      alpha The alpha.
 */
void lineFromARGB8888(uint8_t* const        ptr,
                      const uint32_t* const data,
                      const int16_t         length,
                      const uint8_t         alpha);
// lineFromRGB888(framebuffer, bitmapPointer, length, alpha);

void setupDataCopy(const BlitOp& blitOp);
void setupDataFill(const BlitOp& blitOp);

#ifdef __cplusplus
}
#endif
