#ifndef CALLCOUNTER_HPP
#define CALLCOUNTER_HPP

/**
 * \brief Copy constructor and destructor call counter.
 *
 * Used for unit tests.
 */
class CallCounter {
public:
	CallCounter()
	: copyConstructorCallCount_(0), destructorCallCount_(0) { }
	
	size_t copyConstructorCallCount() {
		return copyConstructorCallCount_;
	}
	
	size_t destructorCallCount() {
		return destructorCallCount_;
	}
	
	void recordCopyConstructorCall() {
		copyConstructorCallCount_++;
	}
	
	void recordDestructorCall() {
		destructorCallCount_++;
	}
	
private:
	size_t copyConstructorCallCount_;
	size_t destructorCallCount_;
	
};

#endif
