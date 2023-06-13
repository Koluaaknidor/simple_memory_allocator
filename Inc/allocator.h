/*
 * allocator.h
 *
 *  Created on: Jun 13, 2023
 *      Author: USER
 */

#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_
#include <stdint.h>

#define HEAP_START 0x20001000
#define HEAP_SIZE 1024
#define HEAP_END (HEAP_START + HEAP_SIZE)

void allocator_init();
void* allocator_allocate(uint8_t size);
uint8_t allocator_free(void *chunk_pointer);

#endif /* ALLOCATOR_H_ */
