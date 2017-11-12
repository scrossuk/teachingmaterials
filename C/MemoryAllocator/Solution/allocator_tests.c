#include "mem.h"
#include "mem_kernel.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Set this to true in a test to simulate out of memory.
bool memory_exhausted = false;

void* mem_block_alloc(size_t n) {
    assert(n > 0);
    if (memory_exhausted) {
        return NULL;
    }
    return malloc(n * MEM_BLOCK_SIZE);
}

void mem_block_free(void* ptr) {
    assert(ptr != NULL);
    free(ptr);
}

void test_alloc_zero(void) {
    void *ptr = mem_alloc(0);
    assert(ptr == NULL);
}

void test_free_zero(void) {
    mem_free(NULL);
}

void test_alloc_huge(void) {
    const size_t alloc_size = 100000;
    uint8_t *ptr = mem_alloc(alloc_size);
    for (size_t i = 0; i < alloc_size; i++) {
        ptr[i] = 100;
    }
    
    mem_free(ptr);
}

void test_alloc_and_free(void) {
    for (size_t i = 0; i < 1000; i++) {
        void *ptr = mem_alloc(i);
        mem_free(ptr);
    }
}

void test_store_and_check(void) {
    size_t *ptrs[100];
    for (size_t i = 0; i < 100; i++) {
        ptrs[i] = mem_alloc(sizeof(size_t));
    }
    for (size_t i = 0; i < 100; i++) {
        *(ptrs[i]) = i;
    }
    for (size_t i = 0; i < 100; i++) {
        assert(*(ptrs[i]) == i);
    }
    for (size_t i = 0; i < 100; i++) {
        mem_free(ptrs[i]);
    }
}

void test_alloc_reuse(void) {
    void *ptrs[100];
    for (size_t i = 0; i < 100; i++) {
        ptrs[i] = mem_alloc(1);
    }
    
    void *p = ptrs[42];
    mem_free(p);
    void *new_p = mem_alloc(1);
    assert(p == new_p);
    ptrs[42] = new_p;
    
    for (size_t i = 0; i < 100; i++) {
        mem_free(ptrs[i]);
    }
}

void test_alloc_grow(void) {
    void *p0 = mem_alloc(10);
    mem_free(p0);
    
    void *p1 = mem_alloc(20);
    mem_free(p1);
    
    void *p2 = mem_alloc(30);
    mem_free(p2);
    
    assert(p0 == p1 && p1 == p2);
}

void test_stable_ptr(void) {
    int *ptr = mem_alloc(sizeof(int));
    *ptr = 42;
    
    for (size_t i = 0; i < 1234; i++) {
        int *ptr1 = mem_alloc(sizeof(int));
        *ptr1 = 43;
        mem_free(ptr1);
    }
    
    assert(*ptr == 42);
    mem_free(ptr);
}

void test_stress(void) {
    const size_t alloc_count = 10000;
    void **allocs = mem_alloc(sizeof(void*) * alloc_count);
    
    for (size_t i = 0; i < alloc_count; i++) {
        allocs[i] = mem_alloc((i % 77) + 1);
    }
    
    for (size_t i = 0; i < alloc_count; i++) {
        if ((i % 3) != 0) { continue; }
        
        mem_free(allocs[i]);
        allocs[i] = mem_alloc((i % 99) + 3);
    }
    
    for (size_t i = 0; i < alloc_count; i++) {
        mem_free(allocs[i]);
    }
    
    mem_free(allocs);
}

int main() {
    test_alloc_zero();
    test_free_zero();
    test_alloc_huge();
    test_alloc_and_free();
    test_store_and_check();
    test_alloc_reuse();
    test_alloc_grow();
    test_stable_ptr();
    test_stress();
    
    printf("Tests complete\n");
    return 0;
}
