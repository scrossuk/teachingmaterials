#include "is_prime.hpp"

// Include a macro for performing checks at compile-time.
#include "static_assert.hpp"

STATIC_ASSERT(not is_prime<1>::value);

STATIC_ASSERT(is_prime<2>::value);

STATIC_ASSERT(is_prime<3>::value);

STATIC_ASSERT(not is_prime<4>::value);

STATIC_ASSERT(is_prime<5>::value);

STATIC_ASSERT(not is_prime<6>::value);

STATIC_ASSERT(is_prime<7>::value);

STATIC_ASSERT(not is_prime<8>::value);

STATIC_ASSERT(not is_prime<9>::value);

STATIC_ASSERT(not is_prime<10>::value);

STATIC_ASSERT(is_prime<11>::value);

// ...

STATIC_ASSERT(is_prime<103>::value);

// ...

// Unfortunately this usually goes beyond the maximum instantiation depth
// allowed by the compiler!
// STATIC_ASSERT(is_prime<7001>::value);


// Define a main() function to keep the linker happy.
int main() { return 0; }
