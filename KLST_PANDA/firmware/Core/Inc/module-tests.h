/*
 * module-tests.h
 *
 *  Created on: Jan 31, 2024
 *      Author: dennisppaul
 */

#ifndef INC_MODULE_TESTS_H_
#define INC_MODULE_TESTS_H_

#include <stdint.h>

void print_debug(char *text);
void println(char *text);
void test_all_ram();
int TestRAM(uint32_t RAM_START_ADDR, uint32_t RAM_END_ADDR);
void external_memory_test_setup();
void external_memory_test_loop();

#endif /* INC_MODULE_TESTS_H_ */
