#ifndef BLOCKMEM_H
#define BLOCKMEM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct blockmem {
    size_t size_field;
    uint8_t data[0];
};

size_t blockmem_alloc_size_for_data_size(size_t data_size);

void blockmem_init(struct blockmem *mem, size_t alloc_size);

void *blockmem_get_data_ptr(struct blockmem *mem);

struct blockmem *blockmem_get_ptr_from_data_ptr(void *ptr);

size_t blockmem_get_data_size(const struct blockmem *mem);

void blockmem_set_data_size(struct blockmem *mem, size_t size);

size_t blockmem_get_alloc_size(const struct blockmem *mem);

bool blockmem_is_end(const struct blockmem *mem);

void blockmem_set_end(struct blockmem *mem, bool end);

bool blockmem_is_allocated(const struct blockmem *mem);

void blockmem_set_allocated(struct blockmem *mem, bool allocated);

struct blockmem *blockmem_next(struct blockmem *mem);

void blockmem_split(struct blockmem *mem, size_t data_size);

void blockmem_merge_with_next(struct blockmem *mem);

#endif
