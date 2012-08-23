#ifndef XTL__AUTO_PTR_STACK_HPP__
#define XTL__AUTO_PTR_STACK_HPP__ 1

#include "AutoPtrVector.hpp"

namespace XTL
{
	template <typename T>
	class AutoPtrStack
	{
		public:

			AutoPtrStack()
				: items_()
			{
				;;
			}

			bool IsEmpty() const
			{
				return items_.IsEmpty();
			}

			void Push(std::auto_ptr<T> ptr)
			{
				items_.PushBack(ptr);
			}

			std::auto_ptr<T> Pop()
			{
				return items_.PopBack();
			}

			T & Top()
			{
				return *(items_.Back());
			}

			const T & Top() const
			{
				return *(items_.Back());
			}

		private:

			AutoPtrStack(const AutoPtrStack &);
			AutoPtrStack & operator= (const AutoPtrStack &);

			XTL::AutoPtrVector<T> items_;
	};
}

#endif

