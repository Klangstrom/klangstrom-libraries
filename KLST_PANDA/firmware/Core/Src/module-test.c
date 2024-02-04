/*
 * module-test.c
 *
 *  Created on: Jan 31, 2024
 *      Author: dennisppaul
 */

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "main.h"
#include "module-tests.h"

void print_debug(char *text) {
	printf("[%010" PRIu32 "] %s\r\n", (uint32_t) HAL_GetTick(), text);
}

void println(char *text) {
	printf("%s\r\n", text);
}

static void run_test(uint32_t RAM_START_ADDR, uint32_t RAM_END_ADDR) {
	printf("REGION: 0x%08" PRIX32 " - 0x%08" PRIX32 "\r\n", RAM_START_ADDR, RAM_END_ADDR);
	if (TestRAM(RAM_START_ADDR, RAM_END_ADDR) != 0) {
		println("        FAILED");
	} else {
		println("        OK");
	}
	/*
	 [0000000018] test external RAM
	 OK
	 */
	//	uint32_t a = 0;
	//	void *b = (void*) &a;
	//	printf("%p\r\n", b);
}

void test_all_ram() {
	// REGION  :: RAM_START_ADDR , RAM_END_ADDR , SIZE
	// ITCMRAM :: 0x00000000     , 0x0000FFFF   , 64KB
	// DTCMRAM :: 0x20000000     , 0x2001FFFF   , 128KB
	// RAM_D1  :: 0x24000000     , 0x2404FFFF   , 320KB
	// RAM_D2  :: 0x30000000     , 0x30007FFF   , 32KB
	// RAM_D3  :: 0x38000000     , 0x38003FFF   , 16KB
	// FLASH   :: 0x08000000     , 0x080FFFFF   , 1024KB
	run_test(0x00000000, 0x0000FFFF);
	run_test(0x20000000, 0x2001FFFF);
//	run_test(0x24000000 + 0x1000, 0x2404FFFF - 0x400);
	run_test(0x30000000, 0x30007FFF);
	run_test(0x38000000, 0x38003FFF);
}

/**
 * Simple memory test function
 * This function writes a test pattern to RAM and then verifies it.
 * @return 0 on success, non-zero on failure
 */

int TestRAM(uint32_t RAM_START_ADDR, uint32_t RAM_END_ADDR) {
	volatile uint32_t *pMem;
	uint32_t testVal = 0;
	int errorFlag = 0;
	uint32_t i;

	// Write phase: fill RAM with a test pattern
	printf("WRITE : ");
	for (pMem = (uint32_t*) RAM_START_ADDR; pMem <= (uint32_t*) RAM_END_ADDR; pMem++) {
		*pMem = (uint32_t) pMem;  // Example pattern: address of the cell itself
		i = *pMem;
		if ((i % 1024) == 0) {
			printf(".");
		}
	}
	printf("\r\n");

	// Verification phase: check the test pattern
	printf("READ  : ");
	for (pMem = (uint32_t*) RAM_START_ADDR; pMem <= (uint32_t*) RAM_END_ADDR; pMem++) {
		testVal = *pMem;
		if (testVal != (uint32_t) pMem) {
			errorFlag = 1;  // Memory error detected
			printf(" at %p", pMem);
			break;
		}
		i = *pMem;
		if ((i % 1024) == 0) {
			printf(".");
		}
	}
	println("");

	return errorFlag; // Return 0 if the RAM is okay, 1 if there is an error
}

/* S70KL1281 memory */

#define SDRAM_ADDRESS_START 0x90000000
#define SDRAM_SIZE           0x1000000

void external_memory_test() {
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

	// fill with visual pattern
	const uint32_t M_FRAMEBUFFER_SIZE = 480 * 272 * 4;
	for (counter = 0x00; counter < M_FRAMEBUFFER_SIZE; counter++) {
		*(__IO uint8_t*) (SDRAM_ADDRESS_START + (counter + 0)) = 0;
	}
	for (counter = 0x00; counter < M_FRAMEBUFFER_SIZE; counter += 4) {
		const uint8_t rgb = (uint8_t) (counter / (480 * 4));
		*(__IO uint8_t*) (SDRAM_ADDRESS_START + M_FRAMEBUFFER_SIZE + (counter + 0)) = 255;
		*(__IO uint8_t*) (SDRAM_ADDRESS_START + M_FRAMEBUFFER_SIZE + (counter + 1)) = rgb;
		*(__IO uint8_t*) (SDRAM_ADDRESS_START + M_FRAMEBUFFER_SIZE + (counter + 2)) = rgb;
		*(__IO uint8_t*) (SDRAM_ADDRESS_START + M_FRAMEBUFFER_SIZE + (counter + 3)) = 255;
	}
}

