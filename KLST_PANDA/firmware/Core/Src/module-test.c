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
	printf("REGION: 0x%08" PRIX32 " - 0x%08" PRIX32 "\r\n", RAM_START_ADDR,
			RAM_END_ADDR);
	if (TestRAM(RAM_START_ADDR, RAM_END_ADDR) != 0) {
		println("        FAILED");
	} else {
		println("        OK");
	}
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
	for (pMem = (uint32_t*) RAM_START_ADDR; pMem <= (uint32_t*) RAM_END_ADDR;
			pMem++) {
		*pMem = (uint32_t) pMem;  // Example pattern: address of the cell itself
		i = *pMem;
		if ((i % 1024) == 0) {
			printf(".");
		}
	}
	printf("\r\n");

	// Verification phase: check the test pattern
	printf("READ  : ");
	for (pMem = (uint32_t*) RAM_START_ADDR; pMem <= (uint32_t*) RAM_END_ADDR;
			pMem++) {
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
/* Size of the HyperRAM */
#define OSPI_HYPERRAM_SIZE          24
#define OSPI_HYPERRAM_INCR_SIZE     256

/* End address of the OSPI memory */
#define OSPI_HYPERRAM_END_ADDR      (1 << OSPI_HYPERRAM_SIZE)

/* Buffer used for transmission */
uint8_t aTxBuffer[] =
		" ****Memory-mapped OSPI communication****   ****Memory-mapped OSPI communication****   ****Memory-mapped OSPI communication****   ****Memory-mapped OSPI communication****   ****Memory-mapped OSPI communication****  ****Memory-mapped OSPI communication**** ";

uint32_t address = 0;
__IO uint32_t *mem_addr;

/* Size of buffers */
#define BUFFERSIZE                  (COUNTOF(aTxBuffer) - 1)
/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)         (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

uint8_t CmdCplt, TxCplt, StatusMatch, RxCplt;

void external_memory_test_setup() {
	address = 0;
}

void external_memory_test_loop() {
	/* Intensive Access ----------------------------------------------- */
	mem_addr = (__IO uint32_t*) (LCD_FRAME_BUFFER + address);

	for (uint16_t index = 0; index < BUFFERSIZE; (index += 4)) {
		/* Writing Sequence --------------------------------------------------- */
		*mem_addr = *(uint32_t*) &aTxBuffer[index];

		/* Reading Sequence --------------------------------------------------- */
		if (*mem_addr != *(uint32_t*) &aTxBuffer[index]) {
			HAL_GPIO_TogglePin(_LED_01_GPIO_Port, _LED_01_Pin);
		}

		mem_addr++;
	}

	HAL_GPIO_TogglePin(_DISPLAY_ON_OFF_GPIO_Port, _DISPLAY_ON_OFF_Pin);
	HAL_Delay(100);

	address += OSPI_HYPERRAM_INCR_SIZE;
	if (address >= OSPI_HYPERRAM_END_ADDR) {
		address = 0;
	}

}
