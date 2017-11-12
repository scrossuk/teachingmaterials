#ifndef BLOCK_H
#define BLOCK_H

#include "blockmem.h"

#include <stddef.h>
#include <stdint.h>

struct block {
    struct blockmem endmem;
    struct block *prev, *next;
};

size_t block_alloc_size_for_data_size(size_t data_size);

struct blockmem *block_get_first_mem(struct block *block);

struct block *block_init(uint8_t *block_mem, size_t alloc_size);

struct block *block_get_ptr_from_mem(struct blockmem *mem);

size_t block_get_alloc_size(struct block *block);

void *block_get_alloc_ptr(struct block *block);

struct blockmem *block_get_first_mem(struct block *block);

struct blockmem *block_find_mem(struct block *block, size_t n);

bool block_has_allocations(struct block *block);

#endif
