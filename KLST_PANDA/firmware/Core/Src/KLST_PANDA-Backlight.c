#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#include "main.h"
#include "KLST_PANDA-backlight.h"

extern TIM_HandleTypeDef htim3;

static uint32_t frame_counter = 0;
static const uint32_t fPeriod = 32768;

void backlight_setup() {
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
}

void backlight_loop() {
	frame_counter++;
	const uint8_t mPhaseDivider = ((1 << (frame_counter % 5 + 2)));
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, fPeriod / mPhaseDivider);
	printf("LCD backlight divider: %i\r\n", mPhaseDivider);
}

void backlight_set_brightness(float brightness) {
	uint32_t mPhase = (uint32_t) (fPeriod * brightness);
	mPhase = MAX(1, MIN(fPeriod, mPhase));
	printf("%sbacklight brightness: %f %li/%li\r\n", DEBUG_PREFIX_SPACE, brightness, mPhase, fPeriod);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, mPhase > 0 ? mPhase : 1);
}
