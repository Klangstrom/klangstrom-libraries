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

#include "DisplayDMA2DDrawLibrary.h"
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

// from `STM32DMA.cpp`

/**
* @fn void tearDown();
*
* @brief Waits until previous DMA drawing operation has finished
*/
void tearDown() {
    /* Wait for DMA2D to finish last run */
    while ((READ_REG(DMA2D->CR) & DMA2D_CR_START) != 0U)
        ;

    /* Clear transfer flags */
    WRITE_REG(DMA2D->IFCR, DMA2D_FLAG_TC | DMA2D_FLAG_CE | DMA2D_FLAG_TE);
}

/** Flushes a line of pixels in the data cache if used.
*
* @brief Flushes decoded RGB pixels when rendering compressed images
*/
void flushLine(uint32_t* addr, int sizebytes) {
    // This funciton is used when decompressing RGB images to flush
    // the currently decoded pixels in the cache to allow the DMA2D
    // to blend the pixels correcly.
    if (SCB->CCR & SCB_CCR_DC_Msk) {
        SCB_CleanDCache_by_Addr(addr, sizebytes);
    }
}

void InvalidateCache() {
    // Because DMA2D access main memory directly, the DCache must be invalidated
    // becuase it could hold a wrong image of the framebuffer. That's done
    // using the function SCB_CleanInvalidateDCache(). Remember to enable
    // "CPU Cache" in the "System Core" settings for "Cortex M7" in CubeMX
    // in order for this function call to work.
    if (SCB->CCR & SCB_CCR_DC_Msk) {
        SCB_CleanInvalidateDCache();
    }
}

void FlushCache() {
    // If the framebuffer is placed in Write-Back cached memory (e.g. SRAM) then
    // the DCache must be flushed prior to DMA2D accessing it. That's done
    // using the function SCB_CleanInvalidateDCache(). Remember to enable
    // "CPU Cache" in the "System Core" settings for "Cortex M7" in CubeMX in
    // order for this function call to work.
    if (SCB->CCR & SCB_CCR_DC_Msk) {
        SCB_CleanInvalidateDCache();
    }
}

void lineFromColor(uint32_t* const ptr,
                   const int16_t   count,
                   const uint32_t  color,
                   const uint8_t   alpha) {
    /* Wait for DMA2D to finish last run */
    while ((READ_REG(DMA2D->CR) & DMA2D_CR_START) != 0U)
        ;

    /* Clear transfer flags */
    WRITE_REG(DMA2D->IFCR, DMA2D_FLAG_TC | DMA2D_FLAG_CE | DMA2D_FLAG_TE);

    /* DMA2D OPFCCR register configuration (set output image color format) */
    WRITE_REG(DMA2D->OPFCCR, DMA2D_OUTPUT_ARGB8888);

    /* Configure DMA2D data size (pixels per line (PL)) */
    WRITE_REG(DMA2D->NLR, (1 | (count << DMA2D_NLR_PL_Pos)));

    // constexpr uint32_t Width  = 32;
    // constexpr uint32_t Height = 32;
    // MODIFY_REG(DMA2D->NLR, (DMA2D_NLR_NL | DMA2D_NLR_PL), (Height | (Width << DMA2D_NLR_PL_Pos)));

    /* Configure DMA2D destination address */
    WRITE_REG(DMA2D->OMAR, reinterpret_cast<uint32_t>(ptr));

    if (alpha < 0xFF) {
        /* Write DMA2D BGPFCCR (background control) register */
        WRITE_REG(DMA2D->BGPFCCR, DMA2D_OUTPUT_ARGB8888 | (DMA2D_NO_MODIF_ALPHA << DMA2D_BGPFCCR_AM_Pos));

        /* Write DMA2D FGPFCCR (foreground control) register */
        WRITE_REG(DMA2D->FGPFCCR, DMA2D_INPUT_A8 | (DMA2D_REPLACE_ALPHA << DMA2D_FGPFCCR_AM_Pos) | (alpha << DMA2D_FGPFCCR_ALPHA_Pos));

        /* DMA2D FGCOLR register configuration */
        WRITE_REG(DMA2D->FGCOLR, color & (DMA2D_FGCOLR_BLUE | DMA2D_FGCOLR_GREEN | DMA2D_FGCOLR_RED));

        /* Configure DMA2D Stream source2 address */
        WRITE_REG(DMA2D->BGMAR, (uint32_t) ptr);

        /* Configure DMA2D source address */
        WRITE_REG(DMA2D->FGMAR, (uint32_t) ptr);

        /* Enable the Peripheral and Enable the transfer complete interrupt */
        WRITE_REG(DMA2D->CR, (DMA2D_CR_START | DMA2D_M2M_BLEND));
    } else {
        /* Write DMA2D FGPFCCR register */
        WRITE_REG(DMA2D->FGPFCCR, DMA2D_OUTPUT_ARGB8888 | (DMA2D_NO_MODIF_ALPHA << DMA2D_FGPFCCR_AM_Pos));

        /* Set Output Color */
        WRITE_REG(DMA2D->OCOLR, color);

        /* Start DMA2D */
        WRITE_REG(DMA2D->CR, (DMA2D_CR_START | DMA2D_R2M));
    }
}

void lineFromARGB8888(uint8_t* const        ptr,
                      const uint32_t* const data,
                      const int16_t         length,
                      const uint8_t         alpha) {
    /* Wait for DMA2D to finish last run */
    while ((READ_REG(DMA2D->CR) & DMA2D_CR_START) != 0U)
        ;

    /* Clear transfer flags */
    WRITE_REG(DMA2D->IFCR, DMA2D_FLAG_TC | DMA2D_FLAG_CE | DMA2D_FLAG_TE);

    /* DMA2D OPFCCR register configuration */
    WRITE_REG(DMA2D->OPFCCR, DMA2D_OUTPUT_ARGB8888);

    /* Configure DMA2D data size */
    WRITE_REG(DMA2D->NLR, (1 | (length << DMA2D_NLR_PL_Pos)));

    /* Configure DMA2D destination address */
    WRITE_REG(DMA2D->OMAR, reinterpret_cast<uint32_t>(ptr));

    /* Configure DMA2D source address */
    WRITE_REG(DMA2D->FGMAR, reinterpret_cast<uint32_t>(data));

    if (alpha < 0xFF) {
        /* Set DMA2D color mode and alpha mode */
        WRITE_REG(DMA2D->FGPFCCR, DMA2D_INPUT_ARGB8888 | (DMA2D_COMBINE_ALPHA << DMA2D_FGPFCCR_AM_Pos) | (alpha << DMA2D_FGPFCCR_ALPHA_Pos));
    } else {
        /* Set DMA2D color mode and alpha mode */
        WRITE_REG(DMA2D->FGPFCCR, DMA2D_INPUT_ARGB8888 | (DMA2D_NO_MODIF_ALPHA << DMA2D_FGPFCCR_AM_Pos));
    }

    /* Write DMA2D BGPFCCR register */
    WRITE_REG(DMA2D->BGPFCCR, DMA2D_INPUT_ARGB8888 | (DMA2D_NO_MODIF_ALPHA << DMA2D_BGPFCCR_AM_Pos));

    /* Configure DMA2D Stream source2 address */
    WRITE_REG(DMA2D->BGMAR, reinterpret_cast<uint32_t>(ptr));

    /* Start DMA2D */
    WRITE_REG(DMA2D->CR, DMA2D_M2M_BLEND | DMA2D_CR_START);
}

/*
 * void STM32DMA::setupDataCopy(const BlitOp& blitOp) handles blit operation of
 * BLIT_OP_COPY
 * BLIT_OP_COPY_L8
 * BLIT_OP_COPY_16BIT
 * BLIT_OP_COPY_WITH_ALPHA
 * BLIT_OP_COPY_ARGB8888
 * BLIT_OP_COPY_ARGB8888_WITH_ALPHA
 * BLIT_OP_COPY_A4
 * BLIT_OP_COPY_A8
 */
void setupDataCopy(const BlitOp& blitOp) {
    uint32_t dma2dForegroundColorMode = blitOp.srcFormat;
    uint32_t dma2dBackgroundColorMode = blitOp.dstFormat;
    uint32_t dma2dOutputColorMode     = blitOp.dstFormat;
    // uint32_t dma2dForegroundColorMode = getChromARTInputFormat(static_cast<Bitmap::BitmapFormat>(blitOp.srcFormat));
    // uint32_t dma2dBackgroundColorMode = getChromARTInputFormat(static_cast<Bitmap::BitmapFormat>(blitOp.dstFormat));
    // uint32_t dma2dOutputColorMode     = getChromARTOutputFormat(static_cast<Bitmap::BitmapFormat>(blitOp.dstFormat));

    /* DMA2D OOR register configuration */
    WRITE_REG(DMA2D->OOR, blitOp.dstLoopStride - blitOp.nSteps);

    /* DMA2D BGOR register configuration -*/
    WRITE_REG(DMA2D->BGOR, blitOp.dstLoopStride - blitOp.nSteps);

    /* DMA2D FGOR register configuration -*/
    WRITE_REG(DMA2D->FGOR, blitOp.srcLoopStride - blitOp.nSteps);

    /* DMA2D OPFCCR register configuration */
    WRITE_REG(DMA2D->OPFCCR, dma2dOutputColorMode);

    /* Configure DMA2D data size */
    WRITE_REG(DMA2D->NLR, (blitOp.nLoops | (blitOp.nSteps << DMA2D_NLR_PL_Pos)));

    /* Configure DMA2D destination address */
    WRITE_REG(DMA2D->OMAR, reinterpret_cast<uint32_t>(blitOp.pDst));

    /* Configure DMA2D source address */
    WRITE_REG(DMA2D->FGMAR, reinterpret_cast<uint32_t>(blitOp.pSrc));

    switch (blitOp.operation) {
        case BLIT_OP_COPY_A4:
            /* Set DMA2D color mode and alpha mode */
            WRITE_REG(DMA2D->FGPFCCR, DMA2D_INPUT_A4 | (DMA2D_COMBINE_ALPHA << DMA2D_FGPFCCR_AM_Pos) | ((blitOp.alpha << DMA2D_FGPFCCR_ALPHA_Pos) & DMA2D_FGPFCCR_ALPHA));

            /* set DMA2D foreground color */
            WRITE_REG(DMA2D->FGCOLR, blitOp.color & (DMA2D_FGCOLR_BLUE | DMA2D_FGCOLR_GREEN | DMA2D_FGCOLR_RED));

            /* Write DMA2D BGPFCCR register */
            if (blitOp.replaceBgAlpha) {
                WRITE_REG(DMA2D->BGPFCCR, dma2dBackgroundColorMode | (DMA2D_REPLACE_ALPHA << DMA2D_BGPFCCR_AM_Pos) | ((0xFFUL << DMA2D_BGPFCCR_ALPHA_Pos) & DMA2D_BGPFCCR_ALPHA));
            } else {
                WRITE_REG(DMA2D->BGPFCCR, dma2dBackgroundColorMode | (DMA2D_NO_MODIF_ALPHA << DMA2D_BGPFCCR_AM_Pos));
            }

            /* Configure DMA2D Stream source2 address */
            WRITE_REG(DMA2D->BGMAR, reinterpret_cast<uint32_t>(blitOp.pDst));

            /* Set DMA2D mode */
            WRITE_REG(DMA2D->CR, DMA2D_M2M_BLEND | DMA2D_IT_TC | DMA2D_CR_START | DMA2D_IT_CE | DMA2D_IT_TE);
            break;
        case BLIT_OP_COPY_A8:
            /* Set DMA2D color mode and alpha mode */
            WRITE_REG(DMA2D->FGPFCCR, DMA2D_INPUT_A8 | (DMA2D_COMBINE_ALPHA << DMA2D_FGPFCCR_AM_Pos) | ((blitOp.alpha << DMA2D_FGPFCCR_ALPHA_Pos) & DMA2D_FGPFCCR_ALPHA));

            /* set DMA2D foreground color */
            WRITE_REG(DMA2D->FGCOLR, blitOp.color & (DMA2D_FGCOLR_BLUE | DMA2D_FGCOLR_GREEN | DMA2D_FGCOLR_RED));

            /* Write DMA2D BGPFCCR register */
            if (blitOp.replaceBgAlpha) {
                WRITE_REG(DMA2D->BGPFCCR, dma2dBackgroundColorMode | (DMA2D_REPLACE_ALPHA << DMA2D_BGPFCCR_AM_Pos) | ((0xFFUL << DMA2D_BGPFCCR_ALPHA_Pos) & DMA2D_BGPFCCR_ALPHA));
            } else {
                WRITE_REG(DMA2D->BGPFCCR, dma2dBackgroundColorMode | (DMA2D_NO_MODIF_ALPHA << DMA2D_BGPFCCR_AM_Pos));
            }

            /* Configure DMA2D Stream source2 address */
            WRITE_REG(DMA2D->BGMAR, reinterpret_cast<uint32_t>(blitOp.pDst));

            /* Set DMA2D mode */
            WRITE_REG(DMA2D->CR, DMA2D_M2M_BLEND | DMA2D_IT_TC | DMA2D_CR_START | DMA2D_IT_CE | DMA2D_IT_TE);
            break;
        case BLIT_OP_COPY_WITH_ALPHA:
            /* Set DMA2D color mode and alpha mode */
            WRITE_REG(DMA2D->FGPFCCR, dma2dForegroundColorMode | (DMA2D_COMBINE_ALPHA << DMA2D_FGPFCCR_AM_Pos) | ((blitOp.alpha << DMA2D_FGPFCCR_ALPHA_Pos) & DMA2D_FGPFCCR_ALPHA));

            /* Write DMA2D BGPFCCR register */
            if (blitOp.replaceBgAlpha) {
                WRITE_REG(DMA2D->BGPFCCR, dma2dBackgroundColorMode | (DMA2D_REPLACE_ALPHA << DMA2D_BGPFCCR_AM_Pos) | ((0xFFUL << DMA2D_BGPFCCR_ALPHA_Pos) & DMA2D_BGPFCCR_ALPHA));
            } else {
                WRITE_REG(DMA2D->BGPFCCR, dma2dBackgroundColorMode | (DMA2D_NO_MODIF_ALPHA << DMA2D_BGPFCCR_AM_Pos));
            }

            /* Configure DMA2D Stream source2 address */
            WRITE_REG(DMA2D->BGMAR, reinterpret_cast<uint32_t>(blitOp.pDst));

            /* Set DMA2D mode */
            WRITE_REG(DMA2D->CR, DMA2D_M2M_BLEND | DMA2D_IT_TC | DMA2D_CR_START | DMA2D_IT_CE | DMA2D_IT_TE);
            break;
        // case BLIT_OP_COPY_L8: {
        //     bool                    blend   = true;
        //     const clutData_t* const palette = reinterpret_cast<const clutData_t*>(blitOp.pClut);
        //
        //     /* Write foreground CLUT memory address */
        //     WRITE_REG(DMA2D->FGCMAR, reinterpret_cast<uint32_t>(&palette->data));
        //
        //     /* Set DMA2D color mode and alpha mode */
        //     WRITE_REG(DMA2D->FGPFCCR, dma2dForegroundColorMode | (DMA2D_COMBINE_ALPHA << DMA2D_FGPFCCR_AM_Pos) | ((blitOp.alpha << DMA2D_FGPFCCR_ALPHA_Pos) & DMA2D_FGPFCCR_ALPHA));
        //
        //     /* Write DMA2D BGPFCCR register */
        //     if (blitOp.replaceBgAlpha) {
        //         WRITE_REG(DMA2D->BGPFCCR, dma2dBackgroundColorMode | (DMA2D_REPLACE_ALPHA << DMA2D_BGPFCCR_AM_Pos) | ((0xFFUL << DMA2D_BGPFCCR_ALPHA_Pos) & DMA2D_BGPFCCR_ALPHA));
        //     } else {
        //         WRITE_REG(DMA2D->BGPFCCR, dma2dBackgroundColorMode | (DMA2D_NO_MODIF_ALPHA << DMA2D_BGPFCCR_AM_Pos));
        //     }
        //
        //     /* Configure DMA2D Stream source2 address */
        //     WRITE_REG(DMA2D->BGMAR, reinterpret_cast<uint32_t>(blitOp.pDst));
        //
        //     /* Configure CLUT */
        //     switch ((Bitmap::ClutFormat) palette->format) {
        //         case Bitmap::CLUT_FORMAT_L8_ARGB8888:
        //             /* Write foreground CLUT size and CLUT color mode */
        //             MODIFY_REG(DMA2D->FGPFCCR, (DMA2D_FGPFCCR_CS | DMA2D_FGPFCCR_CCM), (((palette->size - 1) << DMA2D_FGPFCCR_CS_Pos) | (DMA2D_CCM_ARGB8888 << DMA2D_FGPFCCR_CCM_Pos)));
        //             break;
        //         case Bitmap::CLUT_FORMAT_L8_RGB888:
        //             if (blitOp.alpha == 255) {
        //                 blend = false;
        //             }
        //             MODIFY_REG(DMA2D->FGPFCCR, (DMA2D_FGPFCCR_CS | DMA2D_FGPFCCR_CCM), (((palette->size - 1) << DMA2D_FGPFCCR_CS_Pos) | (DMA2D_CCM_RGB888 << DMA2D_FGPFCCR_CCM_Pos)));
        //             break;
        //         case Bitmap::CLUT_FORMAT_L8_RGB565:
        //         default:
        //             assert(0 && "Unsupported format");
        //             break;
        //     }
        //
        //     /* Enable the CLUT loading for the foreground */
        //     SET_BIT(DMA2D->FGPFCCR, DMA2D_FGPFCCR_START);
        //
        //     while ((READ_REG(DMA2D->FGPFCCR) & DMA2D_FGPFCCR_START) != 0U) {
        //     }
        //     DMA2D->IFCR = (DMA2D_FLAG_CTC);
        //
        //     /* Set DMA2D mode */
        //     if (blend) {
        //         WRITE_REG(DMA2D->CR, DMA2D_M2M_BLEND | DMA2D_IT_TC | DMA2D_CR_START | DMA2D_IT_CE | DMA2D_IT_TE);
        //     } else {
        //         WRITE_REG(DMA2D->CR, DMA2D_M2M_PFC | DMA2D_IT_TC | DMA2D_CR_START | DMA2D_IT_CE | DMA2D_IT_TE);
        //     }
        // } break;
        case BLIT_OP_COPY_ARGB8888:
            /* Write DMA2D FGPFCCR register */
            if (blitOp.replaceFgAlpha) {
                WRITE_REG(DMA2D->FGPFCCR, dma2dForegroundColorMode | (DMA2D_REPLACE_ALPHA << DMA2D_FGPFCCR_AM_Pos) | ((blitOp.alpha << DMA2D_FGPFCCR_ALPHA_Pos) & DMA2D_FGPFCCR_ALPHA));
            } else {
                WRITE_REG(DMA2D->FGPFCCR, dma2dForegroundColorMode | (DMA2D_NO_MODIF_ALPHA << DMA2D_FGPFCCR_AM_Pos));
            }

            /* Write DMA2D BGPFCCR register */
            if (blitOp.replaceBgAlpha) {
                WRITE_REG(DMA2D->BGPFCCR, dma2dBackgroundColorMode | (DMA2D_REPLACE_ALPHA << DMA2D_BGPFCCR_AM_Pos) | ((0xFFUL << DMA2D_BGPFCCR_ALPHA_Pos) & DMA2D_BGPFCCR_ALPHA));
            } else {
                WRITE_REG(DMA2D->BGPFCCR, dma2dBackgroundColorMode | (DMA2D_NO_MODIF_ALPHA << DMA2D_BGPFCCR_AM_Pos));
            }

            /* Configure DMA2D Stream source2 address */
            WRITE_REG(DMA2D->BGMAR, reinterpret_cast<uint32_t>(blitOp.pDst));

            /* Set DMA2D mode */
            WRITE_REG(DMA2D->CR, DMA2D_M2M_BLEND | DMA2D_IT_TC | DMA2D_CR_START | DMA2D_IT_CE | DMA2D_IT_TE);
            break;

        case BLIT_OP_COPY_ARGB8888_WITH_ALPHA:
            /* Set DMA2D color mode and alpha mode */
            WRITE_REG(DMA2D->FGPFCCR, dma2dForegroundColorMode | (DMA2D_COMBINE_ALPHA << DMA2D_FGPFCCR_AM_Pos) | ((blitOp.alpha << DMA2D_FGPFCCR_ALPHA_Pos) & DMA2D_FGPFCCR_ALPHA));

            /* Write DMA2D BGPFCCR register */
            if (blitOp.replaceBgAlpha) {
                WRITE_REG(DMA2D->BGPFCCR, dma2dBackgroundColorMode | (DMA2D_REPLACE_ALPHA << DMA2D_BGPFCCR_AM_Pos) | ((0xFFUL << DMA2D_BGPFCCR_ALPHA_Pos) & DMA2D_BGPFCCR_ALPHA));
            } else {
                WRITE_REG(DMA2D->BGPFCCR, dma2dBackgroundColorMode | (DMA2D_NO_MODIF_ALPHA << DMA2D_BGPFCCR_AM_Pos));
            }

            /* Configure DMA2D Stream source2 address */
            WRITE_REG(DMA2D->BGMAR, reinterpret_cast<uint32_t>(blitOp.pDst));

            /* Set DMA2D mode */
            WRITE_REG(DMA2D->CR, DMA2D_M2M_BLEND | DMA2D_IT_TC | DMA2D_CR_START | DMA2D_IT_CE | DMA2D_IT_TE);
            break;
        default: /* BLIT_OP_COPY & BLIT_OP_COPY_16BIT */
            /* Set DMA2D color mode and alpha mode */
            if (blitOp.replaceFgAlpha) {
                WRITE_REG(DMA2D->FGPFCCR, dma2dForegroundColorMode | (DMA2D_REPLACE_ALPHA << DMA2D_FGPFCCR_AM_Pos) | ((0xFFUL << DMA2D_BGPFCCR_ALPHA_Pos) & DMA2D_BGPFCCR_ALPHA));
            } else {
                WRITE_REG(DMA2D->FGPFCCR, dma2dForegroundColorMode | (DMA2D_NO_MODIF_ALPHA << DMA2D_FGPFCCR_AM_Pos));
            }

            /* Perform pixel-format-conversion (PFC) If Bitmap format is not same format as framebuffer format */
            if (blitOp.srcFormat != blitOp.dstFormat) {
                /* Configure DMA2D Stream source2 address */
                WRITE_REG(DMA2D->BGMAR, reinterpret_cast<uint32_t>(blitOp.pDst));

                /* Start DMA2D : PFC Mode */
                WRITE_REG(DMA2D->CR, DMA2D_M2M_PFC | DMA2D_IT_TC | DMA2D_CR_START | DMA2D_IT_CE | DMA2D_IT_TE);
            } else {
                /* Start DMA2D : M2M Mode */
                WRITE_REG(DMA2D->CR, DMA2D_M2M | DMA2D_IT_TC | DMA2D_CR_START | DMA2D_IT_CE | DMA2D_IT_TE);
            }
            break;
    }
}

/*
 * void STM32DMA::setupDataFill(const BlitOp& blitOp) handles blit operation of
 * BLIT_OP_FILL
 * BLIT_OP_FILL_16BIT
 * BLIT_OP_FILL_WITH_ALPHA
 */
void setupDataFill(const BlitOp& blitOp) {
    uint32_t dma2dOutputColorMode = blitOp.dstFormat;
    // uint32_t dma2dOutputColorMode = getChromARTOutputFormat(static_cast<Bitmap::BitmapFormat>(blitOp.dstFormat));

    /* DMA2D OPFCCR register configuration */
    WRITE_REG(DMA2D->OPFCCR, dma2dOutputColorMode);

    /* Configure DMA2D data size */
    WRITE_REG(DMA2D->NLR, (blitOp.nLoops | (blitOp.nSteps << DMA2D_NLR_PL_Pos))); // ?
    // MODIFY_REG(DMA2D->NLR, (DMA2D_NLR_NL | DMA2D_NLR_PL), (Height | (Width << DMA2D_NLR_PL_Pos)));

    /* Configure DMA2D destination address */
    WRITE_REG(DMA2D->OMAR, reinterpret_cast<uint32_t>(blitOp.pDst));

    /* DMA2D OOR register configuration */
    WRITE_REG(DMA2D->OOR, blitOp.dstLoopStride - blitOp.nSteps); // ???

    if (blitOp.operation == BLIT_OP_FILL_WITH_ALPHA) {
        /* DMA2D BGOR register configuration -*/
        WRITE_REG(DMA2D->BGOR, blitOp.dstLoopStride - blitOp.nSteps);

        /* DMA2D FGOR register configuration -*/
        WRITE_REG(DMA2D->FGOR, blitOp.dstLoopStride - blitOp.nSteps);

        /* Write DMA2D BGPFCCR register */
        if (blitOp.replaceBgAlpha) {
            WRITE_REG(DMA2D->BGPFCCR, dma2dOutputColorMode | (DMA2D_REPLACE_ALPHA << DMA2D_BGPFCCR_AM_Pos) | ((0xFFUL << DMA2D_BGPFCCR_ALPHA_Pos) & DMA2D_BGPFCCR_ALPHA));
        } else {
            WRITE_REG(DMA2D->BGPFCCR, dma2dOutputColorMode | (DMA2D_NO_MODIF_ALPHA << DMA2D_BGPFCCR_AM_Pos));
        }

        /* Write DMA2D FGPFCCR register */
        WRITE_REG(DMA2D->FGPFCCR, DMA2D_INPUT_A8 | (DMA2D_REPLACE_ALPHA << DMA2D_FGPFCCR_AM_Pos) | ((blitOp.alpha << DMA2D_FGPFCCR_ALPHA_Pos) & DMA2D_FGPFCCR_ALPHA));

        /* DMA2D FGCOLR register configuration -*/
        WRITE_REG(DMA2D->FGCOLR, blitOp.color & (DMA2D_FGCOLR_BLUE | DMA2D_FGCOLR_GREEN | DMA2D_FGCOLR_RED));

        /* Configure DMA2D Stream source2 address */
        WRITE_REG(DMA2D->BGMAR, reinterpret_cast<uint32_t>(blitOp.pDst));

        /* Configure DMA2D source address */
        WRITE_REG(DMA2D->FGMAR, reinterpret_cast<uint32_t>(blitOp.pDst));

        /* Enable the Peripheral and Enable the transfer complete interrupt */
        WRITE_REG(DMA2D->CR, (DMA2D_IT_TC | DMA2D_CR_START | DMA2D_M2M_BLEND | DMA2D_IT_CE | DMA2D_IT_TE));
    } else {
        /* Write DMA2D FGPFCCR register */
        WRITE_REG(DMA2D->FGPFCCR, dma2dOutputColorMode | (DMA2D_NO_MODIF_ALPHA << DMA2D_FGPFCCR_AM_Pos));

        /* DMA2D FGOR register configuration -*/
        WRITE_REG(DMA2D->FGOR, 0); // ???

        /* DMA2D OCOLR register configuration */
        if (blitOp.dstFormat == DMA2D_OUTPUT_RGB565) {
            // if (blitOp.dstFormat == Bitmap::RGB565) {
            /* Set color */
            WRITE_REG(DMA2D->OCOLR, blitOp.color & (DMA2D_OCOLR_BLUE_2 | DMA2D_OCOLR_GREEN_2 | DMA2D_OCOLR_RED_2));
        } else {
            /* Set color */
            WRITE_REG(DMA2D->OCOLR, ((blitOp.alpha << 24) & DMA2D_OCOLR_ALPHA_1) | ((blitOp.color) & (DMA2D_OCOLR_BLUE_1 | DMA2D_OCOLR_GREEN_1 | DMA2D_OCOLR_RED_1)));
        }

        /* Enable the Peripheral and Enable the transfer complete interrupt */
        WRITE_REG(DMA2D->CR, (DMA2D_IT_TC | DMA2D_CR_START | DMA2D_R2M | DMA2D_IT_CE | DMA2D_IT_TE));
        // WRITE_REG(DMA2D->CR, (              DMA2D_CR_START | DMA2D_R2M)); // as used in draw line
    }
}

#ifdef __cplusplus
}
#endif

#endif // KLST_PANDA_STM32
#endif // KLST_PERIPHERAL_ENABLE_DISPLAY