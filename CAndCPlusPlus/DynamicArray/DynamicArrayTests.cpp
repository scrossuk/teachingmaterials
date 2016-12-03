// The code we are testing. By putting it first we ensure it #includes its own
// dependencies correctly.
#include "dynamic_array.hpp"

#include <cstdio>
#include <vector>

// Macros like 'CHECK_EQ' to check for specific conditions in unit tests and
// terminate if those conditions are false.
#include "Check.hpp"

// A 'fake' element type that we use for unit testing.
#include "FakeElementType.hpp"

// Typedef a function pointer so it is easier to use.
typedef void (*TestFunctionType)();

// A test is a pair of its name and the function to be called to run it.
typedef std::pair<const char*, TestFunctionType> TestType;

void runTests(const std::vector<TestType>& tests) {
	// In C++11 onwards you can use 'for(auto t: tests)'! We're sticking to
	// C++03 in this code but you can use C++11 in your own code/answers.
	std::vector<TestType>::const_iterator it;
	for (it = tests.begin(); it != tests.end(); ++it) {
		const TestType& t = *it;
		printf("Running test '%s'...\n", t.first);
		t.second();
	}
}

void testEmptyConstructor() {
	{
	        dynamic_array<FakeElementType> array;
		CHECK_EQ(array.size(), 0);
	}
}

void testPushBack() {
	CallCounter counter;
	
	{
	        dynamic_array<FakeElementType> array;
		
		array.push_back(FakeElementType(counter, 10));
		
		CHECK_EQ(array.size(), 1);
		CHECK_EQ(array[0].id(), 10);
		
		CHECK_EQ(counter.copyConstructorCallCount(), 1);
		CHECK_EQ(counter.destructorCallCount(), 0);
	}
	
	CHECK_EQ(counter.copyConstructorCallCount(), 1);
	CHECK_EQ(counter.destructorCallCount(), 1);
}

void testPopBack() {
	CallCounter counter;
	
	{
	        dynamic_array<FakeElementType> array;
		
		array.push_back(FakeElementType(counter, 10));
		
		array.pop_back();
		
		CHECK_EQ(array.size(), 0);
		
		CHECK_EQ(counter.copyConstructorCallCount(), 1);
		CHECK_EQ(counter.destructorCallCount(), 1);
	}
	
	CHECK_EQ(counter.copyConstructorCallCount(), 1);
	CHECK_EQ(counter.destructorCallCount(), 1);
}

void testResize() {
	CallCounter counter;
	
	{
	        dynamic_array<FakeElementType> array;
		
		// Grow array.
		array.resize(10, FakeElementType(counter, 10));
		
		CHECK_EQ(array.size(), 10);
		for (size_t i = 0; i < 10; i++) CHECK_EQ(array[i].id(), 10);
		
		// 10 copies created from one original (now destroyed).
		CHECK_EQ(counter.copyConstructorCallCount(), 10);
		CHECK_EQ(counter.destructorCallCount(), 0);
		
		// Shrink array.
		array.resize(5, FakeElementType(counter, 20));
		
		CHECK_EQ(array.size(), 5);
		for (size_t i = 0; i < 5; i++) CHECK_EQ(array[i].id(), 10);
		
		CHECK_EQ(counter.copyConstructorCallCount(), 10);
		CHECK_EQ(counter.destructorCallCount(), 5);
	}
	
	CHECK_EQ(counter.copyConstructorCallCount(), 10);
	CHECK_EQ(counter.destructorCallCount(), 10);
}

void testSelfAssign() {
	CallCounter counter;
	
	{
	        dynamic_array<FakeElementType> array;
		array.push_back(FakeElementType(counter, 10));
		array.push_back(FakeElementType(counter, 20));
		
		CHECK_EQ(array.size(), 2);
		CHECK_EQ(array[0].id(), 10);
		CHECK_EQ(array[1].id(), 20);
		
		CHECK_EQ(counter.copyConstructorCallCount(), 2);
		CHECK_EQ(counter.destructorCallCount(), 0);
		
		array = array;
		
		CHECK_EQ(array.size(), 2);
		CHECK_EQ(array[0].id(), 10);
		CHECK_EQ(array[1].id(), 20);
		
		// An oddity of our self-assign implementation is that we
		// unnecessarily invoke copy constructors and destructors;
		// that's OK as long as it works.
		CHECK_EQ(counter.copyConstructorCallCount(), 4);
		CHECK_EQ(counter.destructorCallCount(), 2);
	}
	
	CHECK_EQ(counter.copyConstructorCallCount(), 4);
	CHECK_EQ(counter.destructorCallCount(), 4);
}

void testCopy() {
	CallCounter counter;
	
	{
	        dynamic_array<FakeElementType> array;
		array.push_back(FakeElementType(counter, 10));
		array.push_back(FakeElementType(counter, 20));
		
	        dynamic_array<FakeElementType> arrayCopy(array);
		
		// Check our original first.
		CHECK_EQ(array.size(), 2);
		CHECK_EQ(array[0].id(), 10);
		CHECK_EQ(array[1].id(), 20);
		
		// Check the copy.
		CHECK_EQ(arrayCopy.size(), 2);
		CHECK_EQ(arrayCopy[0].id(), 10);
		CHECK_EQ(arrayCopy[1].id(), 20);
		
		CHECK_EQ(counter.copyConstructorCallCount(), 4);
		CHECK_EQ(counter.destructorCallCount(), 0);
	}
	
	CHECK_EQ(counter.copyConstructorCallCount(), 4);
	CHECK_EQ(counter.destructorCallCount(), 4);
}

void testHugeArray() {
	CallCounter counter;
	
	const size_t NUM_ELEMENTS = 10000;
	
	// We'll get lots of copies as the array grows.
	const size_t NUM_EXPECTED_COPIES = 26356;
	
	{
	        dynamic_array<FakeElementType> array;
		
		for (size_t i = 0; i < NUM_ELEMENTS; i++) {
			array.push_back(FakeElementType(counter, i));
			
			// Check immediately after adding.
			CHECK_EQ(array[i].id(), i);
		}
		
		CHECK_EQ(array.size(), NUM_ELEMENTS);
		
		for (size_t i = 0; i < NUM_ELEMENTS; i++) {
			CHECK_EQ(array[i].id(), i);
		}
		
		CHECK_EQ(counter.copyConstructorCallCount(), NUM_EXPECTED_COPIES);
		CHECK_EQ(counter.destructorCallCount(),
		         NUM_EXPECTED_COPIES - NUM_ELEMENTS);
	}
	
	CHECK_EQ(counter.copyConstructorCallCount(), NUM_EXPECTED_COPIES);
	CHECK_EQ(counter.destructorCallCount(), NUM_EXPECTED_COPIES);
}

void testHugeArrayReserve() {
	CallCounter counter;
	
	const size_t NUM_ELEMENTS = 10000;
	
	{
	        dynamic_array<FakeElementType> array;
		
		// Prevent extra copies by reserving correct space at start.
		array.reserve(NUM_ELEMENTS);
		
		for (size_t i = 0; i < NUM_ELEMENTS; i++) {
			array.push_back(FakeElementType(counter, i));
			
			// Check immediately after adding.
			CHECK_EQ(array[i].id(), i);
		}
		
		CHECK_EQ(array.size(), NUM_ELEMENTS);
		
		for (size_t i = 0; i < NUM_ELEMENTS; i++) {
			CHECK_EQ(array[i].id(), i);
		}
		
		// reserve() prevents any extra copies since we never have to
		// grow.
		CHECK_EQ(counter.copyConstructorCallCount(), NUM_ELEMENTS);
		CHECK_EQ(counter.destructorCallCount(), 0);
	}
	
	CHECK_EQ(counter.copyConstructorCallCount(), NUM_ELEMENTS);
	CHECK_EQ(counter.destructorCallCount(), NUM_ELEMENTS);
}

int main() {
	std::vector<TestType> tests;
	tests.push_back(TestType("empty", testEmptyConstructor));
	tests.push_back(TestType("push_back()", testPushBack));
	tests.push_back(TestType("pop_back()", testPopBack));
	tests.push_back(TestType("resize()", testResize));
	tests.push_back(TestType("self assign", testSelfAssign));
	tests.push_back(TestType("copy", testCopy));
	tests.push_back(TestType("huge array", testHugeArray));
	tests.push_back(TestType("huge array reserve()", testHugeArrayReserve));
	
	runTests(tests);
	return 0;
}
