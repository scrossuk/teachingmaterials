#include "block.h"

#include "blockmem.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

size_t block_alloc_size_for_data_size(const size_t data_size) {
    return sizeof(struct block) + blockmem_alloc_size_for_data_size(data_size);
}

struct blockmem *block_get_first_mem(struct block *block);

struct block *block_init(uint8_t *block_mem, const size_t alloc_size) {
    struct block *block = (struct block *)(block_mem + alloc_size) - 1;
    block->prev = NULL;
    block->next = NULL;
    
    blockmem_init(&(block->endmem), alloc_size);
    blockmem_set_end(&(block->endmem), true);
    
    const size_t mem_alloc_size = ((uint8_t *)block) - block_mem;
    blockmem_init(block_get_first_mem(block), mem_alloc_size);
    return block;
}

struct block *block_get_ptr_from_mem(struct blockmem *mem) {
    while (!blockmem_is_end(mem)) {
        mem = blockmem_next(mem);
    }
    return (struct block*)mem;
}

size_t block_get_alloc_size(struct block *block) {
    return blockmem_get_alloc_size(&(block->endmem));
}

void *block_get_alloc_ptr(struct block *block) {
    return (uint8_t *)(block + 1) - block_get_alloc_size(block);
}

struct blockmem *block_get_first_mem(struct block *block) {
    return (struct blockmem *)block_get_alloc_ptr(block);
}

struct blockmem *block_find_mem(struct block *block, size_t n) {
    struct blockmem *mem;
    
    for (mem = block_get_first_mem(block); !blockmem_is_end(mem); mem = blockmem_next(mem)) {
        assert(mem <= &(block->endmem));
        if (blockmem_is_allocated(mem)) { continue; }
        
        // Try to merge this with subsequent blockmems.
        blockmem_merge_with_next(mem);
        
        if (n > blockmem_get_data_size(mem)) { continue; }
        
        // Try to split blockmem so that rest of the space can be used.
        blockmem_split(mem, n);
        
        blockmem_set_allocated(mem, true);
        return mem;
    }
    
    assert(mem == &(block->endmem));
    return NULL;
}

bool block_has_allocations(struct block *block) {
    struct blockmem *mem = block_get_first_mem(block);
    while (!blockmem_is_end(mem)) {
        if (blockmem_is_allocated(mem)) {
            return true;
        }
        
        mem = blockmem_next(mem);
        assert(mem <= &(block->endmem));
    }
    
    assert(mem == &(block->endmem));
    return false;
}
