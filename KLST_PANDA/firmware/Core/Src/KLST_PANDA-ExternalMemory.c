#include "stm32h7xx_hal.h"
#include "KLST_PANDA-ExternalMemory.h"

extern OSPI_HandleTypeDef hospi1;

/* allocate external memory */
//#define EXT_MEM __attribute__((section(".external_memory")))
//#define NUM_SAMPLES (48000 * 20)
//EXT_MEM float m_array[NUM_SAMPLES];
void externalmemory_setup() {
	OSPI_HyperbusCmdTypeDef sCommand;
	OSPI_MemoryMappedTypeDef sMemMappedCfg;

	/* Memory-mapped mode configuration */
	sCommand.AddressSpace = HAL_OSPI_MEMORY_ADDRESS_SPACE;
	sCommand.AddressSize = HAL_OSPI_ADDRESS_32_BITS;
	sCommand.DQSMode = HAL_OSPI_DQS_ENABLE;
	sCommand.Address = 0;
	sCommand.NbData = 1;

	if (HAL_OSPI_HyperbusCmd(&hospi1, &sCommand,
	HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}

	sMemMappedCfg.TimeOutActivation = HAL_OSPI_TIMEOUT_COUNTER_DISABLE;

	if (HAL_OSPI_MemoryMapped(&hospi1, &sMemMappedCfg) != HAL_OK) {
		Error_Handler();
	}
}


/* S70KL1281 memory */

#define SDRAM_ADDRESS_START 0x90000000
#define SDRAM_SIZE           0x1000000

void externalmemory_test() {
	uint32_t counter;
	uint32_t start_time;
	uint32_t stop_time;

	start_time = HAL_GetTick();
	for (counter = 0x00; counter < SDRAM_SIZE; counter++) {
		*(__IO uint8_t*) (SDRAM_ADDRESS_START + counter) = (uint8_t) 0x0;
	}
	stop_time = HAL_GetTick() - start_time;
	printf("             writing 0s   : %li\r\n", stop_time);

	HAL_Delay(50);

	uint8_t testByte = 0x00;
	start_time = HAL_GetTick();
	for (counter = 0x00; counter < SDRAM_SIZE; counter++) {
		*(__IO uint8_t*) (SDRAM_ADDRESS_START + counter) = (uint8_t) (testByte + counter);
	}
	stop_time = HAL_GetTick() - start_time;
	printf("             writing byte : %li\r\n", stop_time);

	HAL_Delay(50);

	uint32_t error_counter = 0;
	start_time = HAL_GetTick();
	for (counter = 0x00; counter < SDRAM_SIZE; counter++) {
		uint8_t testValue = counter;
		uint8_t readValue = *(__IO uint8_t*) (SDRAM_ADDRESS_START + counter);

		if (testValue != readValue) {
			error_counter++;
		}
	}
	stop_time = HAL_GetTick() - start_time;
	printf("             reading byte : %li\r\n", stop_time);
	printf("                   errors : %li\r\n", error_counter);

	HAL_Delay(50);

//	// fill with visual pattern
//	const uint32_t M_FRAMEBUFFER_SIZE = 480 * 272 * 4;
//	for (counter = 0x00; counter < M_FRAMEBUFFER_SIZE; counter++) {
//		*(__IO uint8_t*) (SDRAM_ADDRESS_START + (counter + 0)) = 0;
//	}
//	for (counter = 0x00; counter < M_FRAMEBUFFER_SIZE; counter += 4) {
//		const uint8_t rgb = (uint8_t) (counter / (480 * 4));
//		*(__IO uint8_t*) (SDRAM_ADDRESS_START + M_FRAMEBUFFER_SIZE + (counter + 0)) = 255;
//		*(__IO uint8_t*) (SDRAM_ADDRESS_START + M_FRAMEBUFFER_SIZE + (counter + 1)) = rgb;
//		*(__IO uint8_t*) (SDRAM_ADDRESS_START + M_FRAMEBUFFER_SIZE + (counter + 2)) = rgb;
//		*(__IO uint8_t*) (SDRAM_ADDRESS_START + M_FRAMEBUFFER_SIZE + (counter + 3)) = 255;
//	}
}
