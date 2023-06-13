#include "list.h"
#include <stdint.h>
#include <stddef.h>
#include "main.h"
#include "allocator.h"

uint8_t is_first(LIST_POINTER_t* current_pointer)
{
	if ((uint32_t)current_pointer == HEAP_START)
		return 1;
	return 0;
}

uint8_t is_last(LIST_POINTER_t* current_pointer)
{
	if (current_pointer->size == STOP_SIZE)
		return 1;
	return 0;
}

LIST_POINTER_t* navigate_next(LIST_POINTER_t* current_pointer)
{
	if (is_last(current_pointer))
		return NULL;
	return current_pointer + (current_pointer->size + CHUNK_SIZE_NO_DATA);
}

LIST_POINTER_t* navigate_prev(LIST_POINTER_t* current_pointer)
{
	if (is_first(current_pointer))
		return NULL;
	current_pointer -= 1;
	return current_pointer - (current_pointer->size + CHUNK_SIZE_NO_DATA - 1);
}

uint8_t next_free(LIST_POINTER_t* current_pointer)
{
	current_pointer = navigate_next(current_pointer);
	if (current_pointer == NULL)
		return 0;
	return current_pointer->free;
}

uint8_t prev_free(LIST_POINTER_t* current_pointer)
{
	current_pointer = navigate_prev(current_pointer);
	if (current_pointer == NULL)
		return 0;
	return current_pointer->free;
}
