# Teaching Materials

This repository contains a collection of code for teaching purposes.

You'll need CMake 2.8+ to build this code. To install CMake on Ubuntu run:

```
$ sudo apt-get install cmake
```

If you're on a Linux machine, you can build all of the code by running:

```
$ cmake .
$ make
```

## C&C++

### dynamic_array

`dynamic_array` is essentially a simplified version of `std::vector<>`.

See the [dynamic_array README](CAndCPlusPlus/DynamicArray/README.md).

### is_prime

`is_prime` is a template to determine if a number is prime or not at
compile-time. This is achieved using template metaprogramming.

See the [is_prime README](CAndCPlusPlus/IsPrime/README.md).
