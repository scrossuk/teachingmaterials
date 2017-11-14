#include "mem.h"

#include "block.h"
#include "mem_kernel.h"

#include <assert.h>

static struct block* first_block;

static size_t div_round_up(size_t a, size_t b) {
    return (a + (b - 1)) / b;
}

void* mem_alloc(size_t n) {
    if (n == 0) { return NULL; }
    
    // Round up to nearest multiple of 8.
    n = (n + 7) & ~7;
    
    // Try to find space in existing blocks.
    for (struct block *b = first_block; b != NULL; b = b->next) {
        struct blockmem *mem = block_find_mem(b, n);
        if (mem != NULL) { return blockmem_get_data_ptr(mem); }
    }
    
    // No space available, so allocate a new block.
    const size_t block_min_alloc_size = block_alloc_size_for_data_size(n);
    const size_t mem_block_count = div_round_up(block_min_alloc_size, MEM_BLOCK_SIZE);
    void *block_mem = mem_block_alloc(mem_block_count);
    if (block_mem == NULL) { return NULL; }
    
    struct block *block = block_init(block_mem, mem_block_count * MEM_BLOCK_SIZE);
    assert(block != NULL);
    
    // Add new block to front of list.
    block->next = first_block;
    first_block = block;
    
    struct blockmem *mem = block_find_mem(block, n);
    assert(mem != NULL);
    
    return blockmem_get_data_ptr(mem);
}

void mem_free(void* ptr) {
    if (ptr == NULL) { return; }
    
    struct blockmem *mem = blockmem_get_ptr_from_data_ptr(ptr);
    assert(blockmem_is_allocated(mem) && "Already freed");
    blockmem_set_allocated(mem, false);
    
    // Try to merge this with subsequent blocks.
    blockmem_merge_with_next(mem);
    
    // Free the block if nothing is allocated in it.
    struct block *block = block_get_ptr_from_mem(mem);
    if (!block_has_allocations(block)) {
        if (block->prev != NULL) { block->prev->next = block->next; }
        if (block->next != NULL) { block->next->prev = block->prev; }
        if (block == first_block) { first_block = block->next; }
        mem_block_free(block_get_alloc_ptr(block));
    }
}
