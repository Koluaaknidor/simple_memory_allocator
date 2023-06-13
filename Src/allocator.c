/*
 * allocator.c
 *
 *  Created on: Jun 13, 2023
 *      Author: USER
 */
#include "allocator.h"
#include "list.h"
#include <stdint.h>
#include <stddef.h>

static void set_chunk(LIST_POINTER_t* chunk, LIST_POINTER_t value);
static void free_chunk(LIST_POINTER_t* chunk);
//Combines anchor with the chunk on the right if its free
static void combine_chunks(LIST_POINTER_t* anchor);

void allocator_init()
{
	//TODO: move filling with 0s to startup code
	uint8_t *byte = (uint8_t*)HEAP_START;
	for (int i=0; i<HEAP_SIZE; i++)
	{
		byte[i] = 0;
	}
}

static void set_chunk(LIST_POINTER_t* chunk, LIST_POINTER_t value)
{
	chunk->size = value.size;
	chunk->free = value.free;
	chunk = navigate_next(chunk) - 1;
	chunk->size = value.size;
	chunk->free = value.free;
}

void* allocator_allocate(uint8_t size)
{
	LIST_POINTER_t *heap_pointer = (LIST_POINTER_t*)HEAP_START;

	while (((uint32_t)heap_pointer + CHUNK_SIZE_NO_DATA + size) <= (HEAP_END - CHUNK_SIZE_NO_DATA))
	{
		if (heap_pointer->size == 0 || heap_pointer->free == 1)
		{
			if (heap_pointer->size >= size)
			{
				uint8_t bytes_left = heap_pointer->size - size;
				LIST_POINTER_t chunk_new_value;
				chunk_new_value.free = 0;
				chunk_new_value.size = size;

				//Occupy rest of the bytes to not leave a chunk that is unable to hold data
				if (bytes_left > 0 && bytes_left <= CHUNK_SIZE_NO_DATA)
					chunk_new_value.size += bytes_left;

				set_chunk(heap_pointer, chunk_new_value);

				if (bytes_left > CHUNK_SIZE_NO_DATA)
				{
					LIST_POINTER_t *next_chunk_pointer = navigate_next(heap_pointer);
					chunk_new_value.free = 1;
					chunk_new_value.size = bytes_left - CHUNK_SIZE_NO_DATA;
					set_chunk(next_chunk_pointer, chunk_new_value);
				}

				return heap_pointer;
			}
			else if (heap_pointer->size == 0)
			{
				LIST_POINTER_t chunk_new_value;
				chunk_new_value.free = 0;
				chunk_new_value.size = size;
				set_chunk(heap_pointer, chunk_new_value);

				return heap_pointer;
			}
			else
				;
		}
		heap_pointer = navigate_next(heap_pointer);
	}
	//No right spot found
	return NULL;
}

static void free_chunk(LIST_POINTER_t* chunk)
{
	chunk->free = 1;
	chunk = navigate_next(chunk) - 1;
	chunk->free = 1;
}

static void combine_chunks(LIST_POINTER_t* anchor)
{
	if (next_free(anchor) && anchor->free)
	{
		LIST_POINTER_t new_chunk_value;
		new_chunk_value.size = anchor->size + navigate_next(anchor)->size + CHUNK_SIZE_NO_DATA;
		new_chunk_value.free = 1;

		set_chunk(anchor, new_chunk_value);
	}
}

uint8_t allocator_free(void *chunk_pointer)
{
	chunk_pointer = (LIST_POINTER_t*)chunk_pointer;
	LIST_POINTER_t *heap_pointer = (LIST_POINTER_t*)HEAP_START;
	while ((uint32_t)chunk_pointer >= (uint32_t)heap_pointer)
	{
		if ((uint32_t)chunk_pointer == (uint32_t)heap_pointer)
		{
			free_chunk(chunk_pointer);
			if (!is_last(chunk_pointer))
				combine_chunks(chunk_pointer);
			if (!is_first(chunk_pointer))
			{
				combine_chunks(navigate_prev(chunk_pointer));
			}
			return 1;
		}
		heap_pointer = navigate_next(heap_pointer);
	}
	//No right spot found
	return 0;
}

