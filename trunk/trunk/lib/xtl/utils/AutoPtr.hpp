#ifndef XTL__AUTO_PTR_HPP__
#define XTL__AUTO_PTR_HPP__ 1

#include <memory>

#include "AutoPtr.hpp"

namespace XTL
{
	template <typename T>
	std::auto_ptr<T> MakeAutoPtr(T * value)
	{
		return std::auto_ptr<T>(value);
	}
}

#endif
