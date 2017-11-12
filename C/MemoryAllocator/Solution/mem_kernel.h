#ifndef MEM_KERNEL_H
#define MEM_KERNEL_H

#include <stddef.h>

// Size of a block, as used by mem_block_alloc().
#define MEM_BLOCK_SIZE 4096

// Returns a pointer to contiguous memory of size at least 'n' blocks.
// 'n' must be greater than 0. Returns NULL if no memory is available.
void* mem_block_alloc(size_t n);

// Releases memory allocated by mem_block_alloc(). 'ptr' MUST NOT be NULL.
void mem_block_free(void* ptr);

#endif
