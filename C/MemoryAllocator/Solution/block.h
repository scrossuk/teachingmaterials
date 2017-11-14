#ifndef BLOCK_H
#define BLOCK_H

#include "blockmem.h"

#include <stddef.h>
#include <stdint.h>

struct block {
    struct blockmem endmem;
    struct block *prev, *next;
};

// Get the size of memory that needs to be allocated in order to store the
// given amount of data.
size_t block_alloc_size_for_data_size(size_t data_size);

// Construct a block on top of the given memory.
struct block *block_init(uint8_t *block_mem, size_t alloc_size);

// Get block that corresponds to the given blockmem.
struct block *block_get_ptr_from_mem(struct blockmem *mem);

// Get the size of allocated memory occupied by the block.
size_t block_get_alloc_size(struct block *block);

// Get a pointer to the beginning of memory allocated by the block.
void *block_get_alloc_ptr(struct block *block);

// Get the first blockmem in the block.
struct blockmem *block_get_first_mem(struct block *block);

// Find a blockmem with at least n bytes for storing data.
struct blockmem *block_find_mem(struct block *block, size_t n);

// Query if the block has any allocated memory.
bool block_has_allocations(struct block *block);

#endif
