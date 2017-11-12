#ifndef MEM_H
#define MEM_H

#include <stddef.h>

// Returns a pointer to contiguous memory of size at least 'n' bytes. Returns NULL
// if no memory is available or 'n' is zero.
void* mem_alloc(size_t n);

// Releases memory allocated by mem_alloc(). Does nothing if 'ptr' is NULL.
void mem_free(void* ptr);

#endif
