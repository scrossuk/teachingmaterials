# is_prime

`is_prime` is a template to determine if a number is prime or not at
compile-time. This is achieved using template metaprogramming.

Included here is both the implementation of `is_prime` in
[is_prime.hpp](is_prime.hpp) and then a small set of compile-time checks in
[IsPrimeTests.cpp](IsPrimeTests.cpp).

## Building

You'll need to run CMake; see [top level README](../../README.md) for more
information.

You can build only the compile-time checks (running just `make` will build
everything) using:

```
$ make isPrimeTests
```

## Running

There's no need to run the binary; the important stuff happens at compile-time.
