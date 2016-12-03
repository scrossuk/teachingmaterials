#ifndef STATIC_ASSERT_HPP
#define STATIC_ASSERT_HPP

// A macro for concatenating strings.
// (See http://stackoverflow.com/q/14396139/142088 for explanation.)
#define CONCAT_(a, b) a##b
#define CONCAT(a, b) CONCAT_(a, b)

/**
 * \brief A struct for performing checks at compile-time.
 *
 * This is the generic case of the template; it is unimplemented and therefore
 * does not have a size. This would mean that both of the following are
 * compile-time errors:
 * 
 * - sizeof(static_assert_check<true>)
 * - sizeof(static_assert_check<false>)
 *
 * However we subsequently implement static_assert_check<true> in a
 * specialization, meaning that only sizeof(static_assert_check<false>) is an
 * error. We can then use this in STATIC_ASSERT().
 *
 * (C++11 defines a static_assert() function; we're sticking to C++03 here but
 * you can use that function yourself.)
 */
template<bool>
struct static_assert_check;

// Only implement the case where the boolean is true.
template<>
struct static_assert_check<true>{};

/**
 * \brief Macro to perform check at compile-time.
 *
 * This will generate an enum value that attempts to resolve the size of
 * static_assert_check<cond> at compile-time, which will result in a
 * compile-time error if cond is false.
 */
#define STATIC_ASSERT(cond) \
	enum { CONCAT(static_assert_, __LINE__) = \
		sizeof(static_assert_check<(cond)>) };

// Uncomment for a sanity check of STATIC_ASSERT().
// STATIC_ASSERT(false);

#endif
