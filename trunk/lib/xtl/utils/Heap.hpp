#ifndef XTL__HEAP_HPP__
#define XTL__HEAP_HPP__ 1

#include <algorithm>
#include <vector>

namespace XTL
{
	/*
	 * Инверсия порядка, задаваемого функцией Compare.
	 */
	template <typename Compare>
	class Inverse
	{
		public:

			template <typename T>
			bool operator() (const T & left, const T & right) const
			{
				return comp_(right, left);
			}

		private:

			const Compare comp_;
	};

	/**
	 * Класс-обертка над STL функциями работы с пирамидой.
	 */
	template <typename T, typename Compare>
	class Heap
	{
		public:

			Heap()
				: items_(),
				  comp_(),
				  heapified_(true)
			{
				;;
			}

			bool Empty() const
			{
				return items_.empty();
			}

			unsigned int Size() const
			{
				return items_.size();
			}

			const T & operator[] (unsigned int index) const
			{
				return items_[index];
			}

			Heap & PushBack(const T & value)
			{
				items_.push_back(value);
				heapified_ = false;
				return *this;
			}

			Heap & Push(const T & value)
			{
				items_.push_back(value);

				if (heapified_)
				{
					std::push_heap(items_.begin(), items_.end(), comp_);
				}

				return *this;
			}

			const T & Top()
			{
				Heapify();

				return items_.front();
			}

			const T Pop()
			{
				Heapify();

				const T temp = items_.front();
				std::pop_heap(items_.begin(), items_.end(), comp_);
				items_.pop_back();

				return temp;
			}

			void Heapify()
			{
				if (heapified_)
				{
					return;
				}

				std::make_heap(items_.begin(), items_.end(), comp_);
				heapified_ = true;
			}

		private:

			std::vector<T> items_;
			Compare        comp_;
			bool           heapified_;
	};
}

#endif

