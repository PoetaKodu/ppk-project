#pragma once

#include <utility>

template <typename T>
class UniquePtr
{
public:

	UniquePtr(T * ptr_ = nullptr)
		: ptr(ptr_)
	{
	}

	UniquePtr(UniquePtr const&) = delete;
	UniquePtr(UniquePtr && rhs_)
		: ptr(rhs_.ptr)
	{
		rhs_.ptr = nullptr;
	}

	UniquePtr& operator=(UniquePtr && rhs_)
	{
		std::swap(ptr, rhs_.ptr);
		return *this;
	}

	void reset(T * newValue_ = nullptr)
	{
		if (ptr)
			delete ptr;
		ptr = newValue_;
	}

	T& operator*() const{
		return *ptr;
	}

	~UniquePtr() {
		if (ptr)
			delete ptr;
	}
	T* ptr;
};