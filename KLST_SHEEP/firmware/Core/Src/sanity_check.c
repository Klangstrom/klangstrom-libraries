#include "main.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

extern DAC_HandleTypeDef hdac1;

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;

extern SAI_HandleTypeDef hsai_BlockA1;
extern SAI_HandleTypeDef hsai_BlockB1;
extern DMA_HandleTypeDef hdma_sai1_a;
extern DMA_HandleTypeDef hdma_sai1_b;

extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;
extern SPI_HandleTypeDef hspi4;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim13;
extern TIM_HandleTypeDef htim14;
extern TIM_HandleTypeDef htim15;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;

extern UART_HandleTypeDef huart7;
extern UART_HandleTypeDef huart8;
extern UART_HandleTypeDef huart2;

extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
extern HCD_HandleTypeDef hhcd_USB_OTG_HS;

extern int printf(const char*__restrict, ...) _ATTRIBUTE ((__format__ (__printf__, 1, 2)));
extern int abs (int);
void USR_USB_Host_Init();
void USR_USB_Device_Init();
void USR_USB_Device_Update();

int _write(int file, char *data, int len) {
	HAL_StatusTypeDef status = HAL_UART_Transmit(&huart2, (uint8_t*) data, len,
			0xFFFF);
	return (status == HAL_OK ? len : 0);
}

/* test */

uint8_t mCounter = 0;

void start_LED_timers() {
	static uint16_t mPrescaler = 7199;
	__HAL_TIM_SET_PRESCALER(&htim3, mPrescaler);
	__HAL_TIM_SET_PRESCALER(&htim4, mPrescaler);
	__HAL_TIM_SET_PRESCALER(&htim8, mPrescaler);
	__HAL_TIM_SET_PRESCALER(&htim13, mPrescaler);
	__HAL_TIM_SET_PRESCALER(&htim14, mPrescaler);
	__HAL_TIM_SET_PRESCALER(&htim15, mPrescaler);
	__HAL_TIM_SET_PRESCALER(&htim16, mPrescaler);

	static uint16_t mCounter_ = 499;
	__HAL_TIM_SET_COUNTER(&htim3, mCounter_);
	__HAL_TIM_SET_COUNTER(&htim4, mCounter_);
	__HAL_TIM_SET_COUNTER(&htim8, mCounter_);
	__HAL_TIM_SET_COUNTER(&htim13, mCounter_);
	__HAL_TIM_SET_COUNTER(&htim14, mCounter_);
	__HAL_TIM_SET_COUNTER(&htim15, mCounter_);
	__HAL_TIM_SET_COUNTER(&htim16, mCounter_);

	HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1); // LED_00 :: PB8
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);  // LED_01 :: PB5
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);  // LED_02 :: PB4
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);  // LED_03 :: PC9
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);  // LED_04 :: PC8
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);  // LED_05 :: PC7
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);  // LED_06 :: PC6
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);  // LED_07 :: PD15
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);  // LED_08 :: PD14
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);  // LED_09 :: PD13
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);  // LED_10 :: PD12
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);  // LED_11 :: PB1
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);  // LED_12 :: PB0
	HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1); // LED_13 :: PA7
	HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1); // LED_14 :: PA6
	HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1); // LED_15 :: PA2
}

void set_LED_pulse(uint8_t pValue) {
	uint8_t mOffset = 0;
	uint8_t mValue = 0;
	uint8_t mStep = 16;
	uint8_t mMax = 127;
	uint8_t v;

	mOffset += mStep; v = pValue + mOffset; mValue = abs(mMax - v);
	__HAL_TIM_SET_COMPARE(&htim16, TIM_CHANNEL_1, mValue);
	mOffset += mStep; v = pValue + mOffset; mValue = abs(mMax - v);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, mValue);
	mOffset += mStep; v = pValue + mOffset; mValue = abs(mMax - v);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, mValue);
	mOffset += mStep; v = pValue + mOffset; mValue = abs(mMax - v);
	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, mValue);
	mOffset += mStep; v = pValue + mOffset; mValue = abs(mMax - v);
	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, mValue);
	mOffset += mStep; v = pValue + mOffset; mValue = abs(mMax - v);
	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, mValue);
	mOffset += mStep; v = pValue + mOffset; mValue = abs(mMax - v);
	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, mValue);
	mOffset += mStep; v = pValue + mOffset; mValue = abs(mMax - v);
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, mValue);
	mOffset += mStep; v = pValue + mOffset; mValue = abs(mMax - v);
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, mValue);
	mOffset += mStep; v = pValue + mOffset; mValue = abs(mMax - v);
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, mValue);
	mOffset += mStep; v = pValue + mOffset; mValue = abs(mMax - v);
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, mValue);
	mOffset += mStep; v = pValue + mOffset; mValue = abs(mMax - v);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, mValue);
	mOffset += mStep; v = pValue + mOffset; mValue = abs(mMax - v);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, mValue);
	mOffset += mStep; v = pValue + mOffset; mValue = abs(mMax - v);
	__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, mValue);
	mOffset += mStep; v = pValue + mOffset; mValue = abs(mMax - v);
	__HAL_TIM_SET_COMPARE(&htim13, TIM_CHANNEL_1, mValue);
	mOffset += mStep; v = pValue + mOffset; mValue = abs(mMax - v);
	__HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_1, mValue);
}

void setup() {
	printf("+\r\n");
	printf("%li\r\n", HAL_RCC_GetSysClockFreq());
	start_LED_timers();
	HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	USR_USB_Host_Init();
	USR_USB_Device_Init();
}

void loop() {
	set_LED_pulse(mCounter);
	mCounter += 10;
#ifdef PRINT_ENCODER_VALUES
    printf("1: %li\r\n", TIM1->CNT);
    printf("2: %li\r\n", TIM2->CNT);
#endif
	HAL_Delay(50);
	USR_USB_Device_Update();
}
