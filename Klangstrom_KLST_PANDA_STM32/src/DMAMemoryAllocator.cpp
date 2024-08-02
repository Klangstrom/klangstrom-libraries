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

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define POOL_SIZE 1024 * 10 // Example size, adjust as needed

// Define the memory pool in the desired section
__attribute__((section(".dma_buffer")))
uint8_t dma_memory_pool[POOL_SIZE]; // Define a large enough pool size

//extern uint8_t dma_memory_pool[];

typedef struct {
    uint8_t* base;
    uint8_t* next;
    size_t   size;
} MemoryPool;

static MemoryPool dma_pool = {
    .base = dma_memory_pool,
    .next = dma_memory_pool,
    .size = POOL_SIZE};

void* dma_malloc(size_t size) {
    if ((dma_pool.next + size) > (dma_pool.base + dma_pool.size)) {
        // Not enough memory in the pool
        return NULL;
    }
    void* ptr = dma_pool.next;
    dma_pool.next += size;
    return ptr;
}

void dma_free_all() {
    dma_pool.next = dma_pool.base;
}


#ifdef __cplusplus
}
#endif

//// EXAMPLE:
//
//#define I2S_BUFFER_SIZE 256 // Example size
//
//uint32_t* dma_TX_buffer;
//
//void init_dma_buffer() {
//    dma_TX_buffer = (uint32_t*) dma_malloc(I2S_BUFFER_SIZE * sizeof(uint32_t));
//    if (dma_TX_buffer == NULL) {
//        // Handle allocation failure
//    }
//}
//
//int main() {
//    init_dma_buffer();
//    // Use dma_TX_buffer as needed
//
//    // reset the memory pool if needed
//    dma_free_all();
//
//    return 0;
//}
