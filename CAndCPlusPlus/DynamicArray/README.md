# dynamic_array

`dynamic_array` is essentially a simplified version of `std::vector<>`.

Included here is both the implementation of `dynamic_array` (though with some
bugs left in; see the **FIXME** comments in the source) in
[dynamic_array.hpp](dynamic_array.hpp) and then a small set of unit tests in
[DynamicArrayTests.cpp](DynamicArrayTests.cpp).

## Building

You'll need to run CMake; see [top level README](../../README.md) for more
information.

You can build only the dynamic array unit tests (running just `make` will build
everything) using:

```
$ make dynamicArrayTests
```

## Running

You can run the unit tests for it (from the top level directory) like so:

```
$ ./CAndCPlusPlus/DynamicArray/dynamicArrayTests
```
