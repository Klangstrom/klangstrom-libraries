#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

#include "main.h"
#include "KLST_PANDA-InternalMemory.h"
#include "KLST_PANDA-SerialDebug.h"

static int test_interal_ram(uint32_t RAM_START_ADDR, uint32_t RAM_END_ADDR);

static void run_test(uint32_t RAM_START_ADDR, uint32_t RAM_END_ADDR) {
	printf("REGION: 0x%08" PRIX32 " - 0x%08" PRIX32 "\r\n", RAM_START_ADDR, RAM_END_ADDR);
	if (test_interal_ram(RAM_START_ADDR, RAM_END_ADDR) != 0) {
		println("        FAILED");
	} else {
		println("        OK");
	}
}

/**
 * Simple memory test function
 * This function writes a test pattern to RAM and then verifies it.
 * @return 0 on success, non-zero on failure
 */
static int test_interal_ram(uint32_t RAM_START_ADDR, uint32_t RAM_END_ADDR) {
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

void internalmemory_test_all() {
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
