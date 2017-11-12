project(MemoryAllocator)

add_executable(allocatorTests allocator_tests.c block.c blockmem.c mem.c)
