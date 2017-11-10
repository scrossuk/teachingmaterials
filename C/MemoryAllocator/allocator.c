#include "assert.h"
#include "memory.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

// Size of a block, as used by mem_block_alloc().
#define MEM_BLOCK_SIZE 4096

// Set this to true in a test to simulate out of memory.
bool memory_exhausted = false;

// Returns a pointer to contiguous memory of size at least 'n' blocks.
// 'n' must be greater than 0. Returns NULL if no memory is available.
void* mem_block_alloc(size_t n) {
    assert(n > 0);
    if (memory_exhausted) {
        return NULL;
    }
    return malloc(n * MEM_BLOCK_SIZE);
}

// Releases memory allocated by mem_block_alloc(). 'ptr' MUST NOT be NULL.
void mem_block_free(void* ptr) {
    assert(ptr != NULL);
    free(ptr);
}

// Returns a pointer to contiguous memory of size at least 'n' bytes. Returns NULL
// if no memory is available or 'n' is zero.
void* mem_alloc(size_t n) {
    // FIXME: Add your implementation here.
}

// Releases memory allocated by mem_alloc(). Does nothing if 'ptr' is NULL.
void mem_free(void* ptr) {
    // FIXME: Add your implementation here.
}

/*
FIXME: Add test functions here.

void test_do_something(void) {
    // FIXME: Add test code here.
}

void test_do_something_else(void) {
    // FIXME: Add test code here.
}
*/

int main() {
    // FIXME: Add test calls here.
    // test_do_something();
    // test_do_something_else();
    // etc.
    return 0;
}
