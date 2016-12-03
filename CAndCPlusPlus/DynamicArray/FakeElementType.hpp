#ifndef FAKEELEMENTTYPE_HPP
#define FAKEELEMENTTYPE_HPP

#include "CallCounter.hpp"

/**
 * \brief Fake element object used for testing purposes.
 */
class FakeElementType {
public:
	// NOTE: By defining this constructor C++ will NOT create a default
	//       (no argument) constructor. This is useful to prevent
	//       dynamic_array calling the default constructor as we need to
	//       pass the CallCounter reference to this object.
	FakeElementType(CallCounter& counter, size_t id)
	: counter_(counter), id_(id), isCopy_(false) { }
	
	FakeElementType(const FakeElementType& other)
	: counter_(other.counter_), id_(other.id()), isCopy_(true) {
		counter_.recordCopyConstructorCall();
	}
	
	~FakeElementType() {
		// Only count destructor calls of copies, since originals will
		// be created by the tests.
		if (isCopy_) counter_.recordDestructorCall();
	}
	
	size_t id() const {
		return id_;
	}
	
private:
	// We don't expect dynamic_array to ever use assignment, hence make it
	// private to trigger compile time errors.
	FakeElementType& operator=(const FakeElementType&);
	
	CallCounter& counter_;
	size_t id_;
	bool isCopy_;
	
};

#endif
