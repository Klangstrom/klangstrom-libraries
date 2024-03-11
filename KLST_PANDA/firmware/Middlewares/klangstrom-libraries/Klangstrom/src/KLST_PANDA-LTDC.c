#include "main.h"
#include "KLST_PANDA-LTDC.h"
#include "KLST_PANDA-SerialDebug.h"

extern LTDC_HandleTypeDef hltdc;
extern DMA2D_HandleTypeDef hdma2d;

static uint32_t fVSYNCDuration = 0;
static uint32_t fVSYNCStart = 0;
static uint8_t active_framebuffer = FRAMEBUFFER1;
static uint32_t frame_counter = 0;

void display_switch_on() {
    HAL_GPIO_WritePin(_DISPLAY_ON_OFF_GPIO_Port, _DISPLAY_ON_OFF_Pin, GPIO_PIN_SET);
}

void display_switch_off() {
    HAL_GPIO_WritePin(_DISPLAY_ON_OFF_GPIO_Port, _DISPLAY_ON_OFF_Pin, GPIO_PIN_RESET);
}

void HAL_LTDC_ReloadEventCallback(LTDC_HandleTypeDef *hltdc_handle) {
    fVSYNCDuration = HAL_GetTick() - fVSYNCStart;
    HAL_GPIO_TogglePin(_LED_01_GPIO_Port, _LED_01_Pin);
    // switch and redraw framebuffer
    HAL_LTDC_Reload(hltdc_handle, LTDC_RELOAD_VERTICAL_BLANKING);
    fVSYNCStart = HAL_GetTick();
}

__IO uint32_t LTDC_get_backbuffer_address(void) {
    if (active_framebuffer == FRAMEBUFFER1)
        return (__IO uint32_t) FRAMEBUFFER2_ADDR;
    else return (__IO uint32_t) FRAMEBUFFER1_ADDR;
}

void LTDC_switch_framebuffer(void) {
//	LTDC->SRCR = LTDC_SRCR_VBR;
    if (active_framebuffer == FRAMEBUFFER1) {
        LTDC_Layer1->CFBAR = FRAMEBUFFER2_ADDR;
//		HAL_LTDC_SetAddress(&hltdc, FRAMEBUFFER2, 1);
        active_framebuffer = FRAMEBUFFER2;
    } else {
        LTDC_Layer1->CFBAR = FRAMEBUFFER1_ADDR;
//		HAL_LTDC_SetAddress(&hltdc, FRAMEBUFFER1, 1);
        active_framebuffer = FRAMEBUFFER1;
    }
//	HAL_LTDC_Reload(&hltdc, LTDC_RELOAD_VERTICAL_BLANKING);
    LTDC->SRCR = LTDC_SRCR_VBR; // not sure if it is better to first switch buffer and the reload or viceversa
//	while ((LTDC->CDSR & LTDC_CDSR_VSYNCS) == 0)
//		;
}

void DMA2D_FillRect(uint32_t color, uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    hdma2d.Instance = DMA2D;
    hdma2d.Init.Mode = DMA2D_R2M;
    hdma2d.Init.ColorMode = DMA2D_OUTPUT_ARGB8888;
    hdma2d.Init.OutputOffset = KLST_DISPLAY_WIDTH - width;
    HAL_DMA2D_Init(&hdma2d);
    HAL_DMA2D_ConfigLayer(&hdma2d, 0);
    HAL_DMA2D_ConfigLayer(&hdma2d, 1);
    HAL_DMA2D_Start(&hdma2d, color, LTDC_get_backbuffer_address() + (x + y * KLST_DISPLAY_WIDTH) * 4, width, height);
    HAL_DMA2D_PollForTransfer(&hdma2d, 10);
}

void DMA2D_XferCpltCallback(DMA2D_HandleTypeDef *handle) {
    /* USER CODE BEGIN DMA2D_XferCpltCallback */
    // If the framebuffer is placed in Write Through cached memory (e.g. SRAM) then we need
    // to flush the Dcache prior to letting DMA2D accessing it. That's done
    // using SCB_CleanInvalidateDCache().
//	SCB_CleanInvalidateDCache(); // todo is this necessary
    /* USER CODE END DMA2D_XferCpltCallback */
    HAL_GPIO_TogglePin(_LED_01_GPIO_Port, _LED_01_Pin);
    println("DMA2D_XferCpltCallback");
}

void DMA2D_XferErrorCallback(DMA2D_HandleTypeDef *handle) {
    println("DMA2D_XferErrorCallback");
    println(0);
}

void LTDC_setup() {
    /* fill framebuffers with black */
    for (int i = 0; i < KLST_DISPLAY_FRAMEBUFFER_SIZE; i++) {
        ((volatile uint8_t*) FRAMEBUFFER1_ADDR)[i] = 0x88;
        ((volatile uint8_t*) FRAMEBUFFER2_ADDR)[i] = 0x88;
    }

    hdma2d.XferCpltCallback = DMA2D_XferCpltCallback;
    hdma2d.XferErrorCallback = DMA2D_XferErrorCallback;

//	HAL_LTDC_Reload(&hltdc, LTDC_RELOAD_VERTICAL_BLANKING); // start reload look at 60Hz
}

void LTDC_loop() {
    frame_counter++;
//		const uint32_t mStartFillBuffer = HAL_GetTick();
//		for (uint32_t counter = 0x00; counter < KLST_DISPLAY_FRAMEBUFFER_SIZE; counter += 4) {
//			uint8_t rgb = (uint8_t) (rand());
//			*(__IO uint8_t*) (KLST_DISPLAY_FRAMEBUFFER_ADDRESS + counter + 0) = rgb; // B
//			*(__IO uint8_t*) (KLST_DISPLAY_FRAMEBUFFER_ADDRESS + counter + 1) = rgb; // G
//			*(__IO uint8_t*) (KLST_DISPLAY_FRAMEBUFFER_ADDRESS + counter + 2) = rgb; // R
//			*(__IO uint8_t*) (KLST_DISPLAY_FRAMEBUFFER_ADDRESS + counter + 3) = 0; // A
//		}
//		const uint32_t mFillBufferDuration = HAL_GetTick() - mStartFillBuffer;
//		printf("             frame fill duration    : %li\r\n", mFillBufferDuration);

    DMA2D_FillRect(0xFF000000, // ARGB
                   0,
                   0,
                   KLST_DISPLAY_WIDTH,
                   KLST_DISPLAY_HEIGHT);
    const uint32_t x = (frame_counter * 10) % KLST_DISPLAY_WIDTH;
    const uint32_t y = ((frame_counter * 10) / KLST_DISPLAY_WIDTH * 20) % KLST_DISPLAY_HEIGHT;
    DMA2D_FillRect(0xFFFFFF00, // ARGB
                   x,
                   y,
                   KLST_DISPLAY_WIDTH / 2,
                   KLST_DISPLAY_HEIGHT / 2);

    /* schedule redraw */
    LTDC_switch_framebuffer(); // manually trigger frame redraw

//	println("VSYNC duration: %li\r\n", fVSYNCDuration);
}
