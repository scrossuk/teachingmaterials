#ifndef ISPRIME_HPP
#define ISPRIME_HPP

/**
 * \brief Check if Number can be divided exactly by any of 2..Divisor.
 */
template <int Number, int Divisor>
struct can_be_divided {
	static const bool value =
	    // Check if number can be divided exactly for this divisor.
	    ((Number % Divisor) == 0) ||
	    // If not, check the next lower divisor.
	    can_be_divided<Number, Divisor - 1>::value;
};

// Specialise for Divisor=1 to infinite recursion.
template <int Number>
struct can_be_divided<Number, 1> {
	static const bool value = false;
};

/**
 * \brief Check if Number is prime.
 */
template <int Number>
struct is_prime {
	// A number is prime if it can't be divided by any of numbers smaller
	// than it (except 1).
	static const bool value = not can_be_divided<Number, Number-1>::value;
	
	// Static const variables are evaluated at compile-time, hence this
	// works. An alternative is to use enums, such as:
	//     enum { value = not can_be_divided<Number, Number-1>::value };
};

// Special case for one.
// (We don't handle cases of zero or less.)
template <>
struct is_prime<1> {
	static const bool value = false;
};

#endif
