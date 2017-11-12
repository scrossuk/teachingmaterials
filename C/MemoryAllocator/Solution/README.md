# Memory Allocator

## How it works

### Blocks

This memory allocator stores blocks in a linked list. Block information (`struct block`) is stored **at the end** of each allocated block:

```
struct block {
    struct blockmem endmem;
    struct block *prev, *next;
};
```

### Block slots

Within each block there are one or more slots (`struct blockmem`), which are a `size_t` field followed by the memory:

```
struct blockmem {
    size_t size_field;
    uint8_t data[0];
};
```

The `size_t` field provides the size, as well as whether this slot is the last in the block, and whether it is allocated.

### Finding block for a slot

The block information is stored at the end of a block to allow iterating from any slot to the block information. The code to do this is simply:

```
struct block *block_get_ptr_from_mem(struct blockmem *mem) {
    while (!blockmem_is_end(mem)) {
        mem = blockmem_next(mem);
    }
    return (struct block*)mem;
}
```

## Questions on your implementation

> **a)** Comment on the time cost of calling `mem_alloc()` and `mem_free()` in your implementation.

`mem_alloc()` takes `O(n)` time, where `n` is the total number of slots, since it simply iterates through all blocks to find space.

`mem_free()` takes `O(m)` time, where `m` is the number of slots in the same block as the allocation being freed, since it must check to see if the block can be freed.

> What improvements could you make to reduce this?

Store a list of available slots for particular sizes, which can reduce `mem_alloc()` to constant time.

> **b)** How well does your memory allocator handle fragmentation after a long sequence of calls to `mem_alloc()` and `mem_free()`?

Blocks will tend to be divided into inconveniently sized free space, hence external fragmentation.

> What could you do to improve this?

Use pools for different common sizes of allocations (i.e. internal fragmentation rather than external fragmentation). Could also use buddy memory allocation.

> **c)** What is the principle of locality?

Locality means that locations close to each other in memory tend also to be accessed close in time. Here are some specific kinds:

* Temporal locality - when a memory location is accessed it is likely to be accessed again soon.
* Spatial locality - when a memory location is accessed nearby locations are likely to be accessed soon.

> How well does your allocator support this?

A sequence of `mem_alloc()` calls will tend to return nearby memory allocations, unless re-using previous freed allocations. Spatial locality support is undermined by the `sizeof(size_t)` gap between allocations.

> What improvements could be made?

Remove the `sizeof(size_t)` gap and put this information elsewhere. When outside the allocator this will improve cache performance, and furthermore is also likely to improve cache performance inside the allocator.

> **d)** The lead developer suggests an additional function `mem_realloc()`:
> [...]
> In what use cases might such a function be useful?

A `realloc()` function is useful for growing arrays, since it allows avoiding copying the elements if there is space available at the end of the allocated memory.

> Describe how you might add this to your implementation to facilitate such use cases.

The code can query the next `blockmem` to see if it is unallocated, not the end of the block and has sufficient size to support the expansion. Otherwise, it calls `mem_alloc()` to get a new allocation, copies from the old into the new, calls `mem_free()` on the old allocation and then returns the new allocation.
