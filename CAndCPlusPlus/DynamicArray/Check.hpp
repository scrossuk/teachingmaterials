#ifndef CHECK_HPP
#define CHECK_HPP

// A function for printing a value. You add other overloads for different types
// and CHECK_*() will work for those types.
void printValue(size_t value) {
	printf("%zu", value);
}

// Not using assert() as it is disabled when NDEBUG is defined; we can rely on
// this function to always be enabled.
template <typename A, typename B>
void checkEqFunction(const char* fileName, int line, const A& a, const B& b,
                     const char* aStr, const char* bStr) {
	// Early return for when a==b means the rest of the code doesn't have to
	// be indented.
	if (a == b) return;
	
	// Tries to print a helpful message if a check fails.
	printf("%s:%d: check failed: ", fileName, line);
	printf("%s (", aStr);
	printValue(a);
	printf(") != %s (", bStr);
	printValue(b);
	printf(")\n");
	std::terminate();
}

// '__FILE__' and '__LINE__' are replaced by the compiler to the current
// file/line, which will the location of the generated code for this macro.
//
// '(a)' and '(b)' have parentheses to prevent issues with their evaluation if
// they use inappropriate operators (e.g. the comma operator). This is good
// practice when using macros.
//
// '#a' and '#b' convert 'a' and 'b' text arguments (which is how the
// preprocessor sees them) to C strings.
//
// Note that the template arguments to checkEqFunction() will be inferred by the
// compiler based on the argument types.
#define CHECK_EQ(a, b) checkEqFunction(__FILE__, __LINE__, (a), (b), #a, #b)

#endif
