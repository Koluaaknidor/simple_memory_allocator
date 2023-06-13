/*
 * Two-way list for heap navigation
 */

#ifndef LIST_H_
#define LIST_H_

#include <stdint.h>

typedef struct
{
	uint8_t size		:7;
	uint8_t free		:1;
} LIST_POINTER_t;

#define STOP_SIZE 0
#define CHUNK_SIZE_NO_DATA 2

uint8_t is_first(LIST_POINTER_t* current_pointer);
uint8_t is_last(LIST_POINTER_t* current_pointer);
/*
 * Updates current pointer to the address of next pointer.
 * If current pointer`s size is equal to STOP_SIZE, return NULL, otherwise return new pointer
 * */
LIST_POINTER_t* navigate_next(LIST_POINTER_t* current_pointer);

/*
 * Updates current pointer to the address of previous pointer.
 * If current pointer`s located at heap_start, return NULL, otherwise return new pointer
 * */
LIST_POINTER_t* navigate_prev(LIST_POINTER_t* current_pointer);


/*
 * Returns 1 if the next chunk is free
 */
uint8_t next_free(LIST_POINTER_t* current_pointer);

/*
 * Returns 1 if the previous chunk is free
 */
uint8_t prev_free(LIST_POINTER_t* current_pointer);

#endif /* LIST_H_ */
