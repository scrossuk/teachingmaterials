#ifndef DYNAMICARRAY_HPP
#define DYNAMICARRAY_HPP

#include <algorithm>
#include <cassert>
#include <cstdlib>

/**
 * \brief Dynamically resizable array.
 *
 * This is essentially a very simply version of std::vector<> (and uses the same
 * underscore-separated style as the standard library).
 *
 * There are various comments starting 'FIXME:'; these point to issues with the
 * implementation (generally left un-fixed for brevity) which you should
 * consider how to address.
 *
 * NOTE: This class is designed for pre-C++11 and as such doesn't provide move
 *       operations.
 */
template <typename T>
class dynamic_array {
public:
	/**
	 * \brief Create an empty dynamic array.
	 */
	dynamic_array()
	: size_(0),
	capacity_(0),
	data_(NULL) { }
	
	/**
	 * \brief Copy from another dynamic array instance.
	 */
	dynamic_array(const dynamic_array<T>& array)
	: size_(array.size()),
	capacity_(array.size()) {
		// FIXME: Doesn't check if malloc() returns NULL.
		// TODO: How could a caller pass a custom allocator?
		T* const ptr = static_cast<T*>(malloc(sizeof(T) * capacity_));
		for (size_t i = 0; i < array.size(); i++) {
			// Uses placement new to call copy constructor.
			// FIXME: Doesn't handle copy constructors throwing!
			new(&ptr[i]) T(array[i]);
		}
		data_ = ptr;
	}
	
	/**
	 * \brief Assign from another dynamic array instance.
	 */
	dynamic_array<T>& operator=(const dynamic_array<T>& array) {
		// Copy and then swap.
		//
		// This both safely handles self-assignment and provides strong
		// exception safety (means if this method throws we guarantee
		// the dynamic_array object remains in its original state).
		dynamic_array<T> arrayCopy(array);
		swap(arrayCopy);
		return *this;
	}
	
	/**
	 * \brief Swap fields with another dynamic array instance.
	 */
	void swap(dynamic_array<T>& array) {
		std::swap(size_, array.size_);
		std::swap(capacity_, array.capacity_);
		std::swap(data_, array.data_);
	}
	
	/**
	 * \brief Destroy this array instance.
	 */
	~dynamic_array() {
		// Call destructors.
		// We do this in **reverse** order of construction.
		for (size_t i = 0; i < size(); i++) {
			const size_t revPosition = size() - i - 1;
			// We can rely on destructors NOT throwing, so this is
			// OK.
			data_[revPosition].~T();
		}
		// TODO: How could a caller pass a custom allocator?
		free(data_);
	}
	
	/**
	 * \brief Get the current array size.
	 */
	size_t size() const {
		return size_;
	}
	
	/**
	 * \brief Access an element by index.
	 */
	T& operator[](size_t index) {
		assert(index < size());
		return data_[index];
	}
	
	/**
	 * \brief Access an element by index (const overload).
	 */
	const T& operator[](size_t index) const {
		assert(index < size());
		return data_[index];
	}
	
	/**
	 * \brief Increase capacity of array.
	 */
	void reserve(const size_t newCapacity) {
		if (newCapacity <= capacity_) {
			return;
		}
		
		// Allocate a larger array.
		capacity_ = newCapacity * 2;
		
		// FIXME: Doesn't check if malloc() returns NULL.
		// TODO: How could a caller pass a custom allocator?
		T* const newData = static_cast<T*>(malloc(sizeof(T) * capacity_));
		for (size_t i = 0; i < size(); i++) {
			// Uses placement new to call copy constructor.
			// FIXME: Doesn't handle copy constructors throwing!
			new(&newData[i]) T(data_[i]);
		}
		
		// Destroy existing array.
		// We do this in **reverse** order of construction.
		for (size_t i = 0; i < size(); i++) {
			const size_t revPosition = size() - i - 1;
			// We can rely on destructors NOT throwing, so this is
			// OK.
			data_[revPosition].~T();
		}
		// TODO: How could a caller pass a custom allocator?
		free(data_);
		
		data_ = newData;
	}
	
	/**
	 * \brief Resize the array to contain 'newSize' elements.
	 *
	 * If the new array size is larger fill the new slots with copies of
	 * 'value'.
	 */
	void resize(size_t newSize, const T& value = T()) {
		// Increase capacity (if we're expanding the array).
		reserve(newSize);
		
		// Call copy constructors (if we're expanding the array).
		for (size_t i = size(); i < newSize; i++) {
			// Uses placement new to call copy constructor.
			// FIXME: Doesn't handle copy constructors throwing!
			new(&data_[i]) T(value);
		}
		
		// Call destructors (if we're shrinking the array).
		// We do this in **reverse** order of construction.
		for (size_t i = newSize; i < size(); i++) {
			const size_t revPosition = size() - i - 1;
			// We can rely on destructors NOT throwing, so this is
			// OK.
			data_[revPosition].~T();
		}
		
		size_ = newSize;
	}
       
	/**
	 * \brief Append element to back of array.
	 */
	void push_back(const T& element) {
		resize(size() + 1, element);
	}
       
	/**
	 * \brief Remove last element.
	 */
	void pop_back() {
		assert(size() > 0);
		size_--;
		// We can rely on destructors NOT throwing, so this is OK.
		data_[size_].~T();
	}
	
	/**
	 * \brief Dynamic Array Iterator Type
	 */
	typedef T* iterator;
	
	/**
	 * \brief Get iterator referring to beginning of array.
	 */
	iterator begin() {
		return data_;
	}
	
	/**
	 * \brief Get iterator referring to end of array.
	 */
	iterator end() {
		return data_ + size();
	}
	
private:
	// Separate size and capacity fields mean we can avoid having to
	// re-allocate the underlying storage when each element is added.
	size_t size_, capacity_;
	T* data_;
	
};
 
#endif