#ifndef INC_KLST_PANDA_LTDC_H_
#define INC_KLST_PANDA_LTDC_H_

#include "stm32h7xx_hal.h"

#define KLST_DISPLAY_FRAMEBUFFER_SIZE    (KLST_DISPLAY_WIDTH * KLST_DISPLAY_HEIGHT * 4)
#define FRAMEBUFFER1_ADDR                (KLST_DISPLAY_FRAMEBUFFER_ADDRESS)
#define FRAMEBUFFER2_ADDR                (KLST_DISPLAY_FRAMEBUFFER_ADDRESS + KLST_DISPLAY_FRAMEBUFFER_SIZE)
#define FRAMEBUFFER1                     0
#define FRAMEBUFFER2                     1

/* FB1 :: 0x90000000
 * FB2 :: 0x9007F800
 */

void display_switch_on();
void display_switch_off();

void LTDC_loop();
void LTDC_setup();
void HAL_LTDC_ReloadEventCallback(LTDC_HandleTypeDef *hltdc_handle);
__IO uint32_t LTDC_get_backbuffer_address(void);
void LTDC_switch_framebuffer(void);
void DMA2D_FillRect(uint32_t color, uint32_t x, uint32_t y, uint32_t width, uint32_t height);
void DMA2D_XferCpltCallback(DMA2D_HandleTypeDef *handle);
void DMA2D_XferErrorCallback(DMA2D_HandleTypeDef *handle);

#endif /* INC_KLST_PANDA_LTDC_H_ */
//  values for ER-TFT043A2-3 display with ST7282 driver
//
//	LTDC_CLOCK_RATE          9.5   // ( TotalWidth * TotalHeigh * RefreshRate ) / 1000000 = 9.45 MHz at ( 534 * 295 * 60Hz )
//	DISPLAY_HSYNC            3     /* Horizontal synchronization */
//	DISPLAY_HBP              43    /* Horizontal back porch      */
//	DISPLAY_HFP              8     /* Horizontal front porch     */
//	DISPLAY_VSYNC            3     /* Vertical synchronization   */
//	DISPLAY_VBP              12    /* Vertical back porch        */
//	DISPLAY_VFP              8     /* Vertical front porch       */
//	DISPLAY_WIDTH            480
//	DISPLAY_HEIGHT           272
//	DISPLAY_FRAMEBUFFER_SIZE (DISPLAY_WIDTH * DISPLAY_HEIGHT * 4) // 32bit TODO(check if this 32bit or 24bit)
//	HorizontalSync     = (DISPLAY_HSYNC - 1);
//	VerticalSync       = (DISPLAY_VSYNC - 1);
//	AccumulatedHBP     = (DISPLAY_HSYNC + DISPLAY_HBP - 1);
//	AccumulatedVBP     = (DISPLAY_VSYNC + DISPLAY_VBP - 1);
//	AccumulatedActiveW = (DISPLAY_WIDTH + DISPLAY_HSYNC + DISPLAY_HBP - 1);
//	AccumulatedActiveH = (DISPLAY_HEIGHT + DISPLAY_VSYNC + DISPLAY_VBP - 1);
//	TotalWidth         = (DISPLAY_WIDTH + DISPLAY_HSYNC + DISPLAY_HBP + DISPLAY_HFP - 1);
//	TotalHeigh         = (DISPLAY_HEIGHT + DISPLAY_VSYNC + DISPLAY_VBP + DISPLAY_VFP - 1);
