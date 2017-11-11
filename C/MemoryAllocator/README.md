# Memory Allocator

As part of the development of a new operating system, you've been asked to build a memory allocator.

## What you need to implement

Your memory allocator should implement the following functions:

```c
// Returns a pointer to contiguous memory of size at least 'n' bytes. Returns NULL
// if no memory is available or 'n' is zero.
void* mem_alloc(size_t n);

// Releases memory allocated by mem_alloc(). Does nothing if 'ptr' is NULL.
void mem_free(void* ptr);
```

## What the kernel provides

To implement this the kernel provides the following functions that you can call:

```c
// Size of a block, as used by mem_block_alloc().
#define MEM_BLOCK_SIZE 4096

// Returns a pointer to contiguous memory of size at least 'n' blocks.
// 'n' must be greater than 0. Returns NULL if no memory is available.
void* mem_block_alloc(size_t n);

// Releases memory allocated by mem_block_alloc(). 'ptr' MUST NOT be NULL.
void mem_block_free(void* ptr);
```

## Implementation notes

There's a template you can use here: [allocator.c](allocator.c)

Note the following:

* Your code must compile!
* Write some test cases to show that your code works. These should be included with your answer.
* Add asserts as necessary to show invariants, pre-conditions, etc.
* Your implementation should only use `mem_block_alloc()` and `mem_block_free()` (i.e. **not** `malloc()`, `free()`, etc., except for testing).
* Your allocator must use memory efficiently, using a minimal amount of space per allocation - don't allocate entire blocks for small allocations.
* Memory released by `mem_free()` should be reusable by subsequent calls to `mem_alloc()` - we don't have infinite memory!
* You may use global variables to store state needed by the memory allocator.
* Don't worry about alignment, just make sure to return at least `n` contiguous bytes.
* Don't worry about thread safety.

## Questions on your implementation

**a)** Comment on the time cost of calling `mem_alloc()` and `mem_free()` in your implementation. What improvements could you make to reduce this?

**b)** How well does your memory allocator handle fragmentation after a long sequence of calls to `mem_alloc()` and `mem_free()`? What could you do to improve this?

**c)** What is the principle of locality? How well does your allocator support this? What improvements could be made?

**d)** The lead developer suggests an additional function `mem_realloc()`:

```c
// Returns a pointer to contiguous memory of size at least 'n' bytes.
// Attempts to grow space allocated in 'ptr', if not NULL. Returns NULL
// if no memory is available or 'n' is zero.
void* mem_realloc(void* ptr, size_t n);
```

In what use cases might such a function be useful? Describe how you might add this to your implementation to facilitate such use cases.
