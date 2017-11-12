#include "blockmem.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

size_t blockmem_alloc_size_for_data_size(const size_t data_size) {
    return sizeof(struct blockmem) + data_size;
}

void blockmem_init(struct blockmem *mem, size_t alloc_size) {
    assert((alloc_size & 3) == 0);
    mem->size_field = alloc_size - sizeof(struct blockmem);
}

void *blockmem_get_data_ptr(struct blockmem *mem) {
    return &(mem->data[0]);
}

struct blockmem *blockmem_get_ptr_from_data_ptr(void *ptr) {
    return ((struct blockmem *)ptr) - 1;
}

size_t blockmem_get_data_size(const struct blockmem *mem) {
    return mem->size_field & ~3;
}

void blockmem_set_data_size(struct blockmem *mem, size_t size) {
    assert((size & 3) == 0);
    mem->size_field = size | (mem->size_field & 3);
}

size_t blockmem_get_alloc_size(const struct blockmem *mem) {
    return blockmem_get_data_size(mem) + sizeof(struct blockmem);
}

bool blockmem_is_end(const struct blockmem *mem) {
    return (mem->size_field & 2) != 0;
}

void blockmem_set_end(struct blockmem *mem, bool end) {
    if (end) {
        mem->size_field |= 2;
    } else {
        mem->size_field &= ~2;
    }
}

bool blockmem_is_allocated(const struct blockmem *mem) {
    return (mem->size_field & 1) != 0;
}

void blockmem_set_allocated(struct blockmem *mem, bool allocated) {
    assert(!blockmem_is_end(mem));
    if (allocated) {
        mem->size_field |= 1;
    } else {
        mem->size_field &= ~1;
    }
}

struct blockmem *blockmem_next(struct blockmem *mem) {
    assert(!blockmem_is_end(mem));
    return (struct blockmem *)&(mem->data[blockmem_get_data_size(mem)]);
}

void blockmem_split(struct blockmem *mem, size_t data_size) {
    assert(!blockmem_is_end(mem) && !blockmem_is_allocated(mem));
    
    const size_t available_data_size = blockmem_get_data_size(mem);
    assert(data_size <= available_data_size);
    
    if (available_data_size <= (data_size + blockmem_alloc_size_for_data_size(0))) {
        // Can't usefully split.
        return;
    }
    
    blockmem_set_data_size(mem, data_size);
    blockmem_init(blockmem_next(mem), available_data_size - data_size);
}

void blockmem_merge_with_next(struct blockmem *mem) {
    assert(!blockmem_is_end(mem) && !blockmem_is_allocated(mem));
    
    struct blockmem *next = blockmem_next(mem);
    while (!blockmem_is_end(next) && !blockmem_is_allocated(next)) {
        blockmem_set_data_size(mem, blockmem_get_data_size(mem) + blockmem_get_alloc_size(next));
        next = blockmem_next(mem);
    }
}
