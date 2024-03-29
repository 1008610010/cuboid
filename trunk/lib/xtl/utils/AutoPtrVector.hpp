#ifndef XTL__AUTO_PTR_VECTOR_HPP__
#define XTL__AUTO_PTR_VECTOR_HPP__ 1

#include <algorithm>
#include <vector>

#include "AutoPtr.hpp"

namespace XTL
{
	template <typename T>
	class AutoPtrVector
	{
		public:

			typedef std::vector<T *> VectorType;

			AutoPtrVector()
				: items_()
			{
				;;
			}

			explicit AutoPtrVector(unsigned int size)
				: items_(size, 0)
			{
				;;
			}

			explicit AutoPtrVector(AutoPtrVector & other)
				: items_()
			{
				items_.swap(other.items_);
			}

			~AutoPtrVector() throw()
			{
				Clear();
			}

			void Clear()
			{
				const typename VectorType::iterator end = items_.end();
				for (typename VectorType::iterator itr = items_.begin(); itr != end; ++itr)
				{
					delete (*itr);
				}

				items_.clear();
			}

			bool IsEmpty() const
			{
				return items_.empty();
			}

			unsigned int Size() const
			{
				return items_.size();
			}

			void PushBack(std::auto_ptr<T> ptr)
			{
				items_.push_back(ptr.release());
			}

			template <typename U>
			void PushBack(std::auto_ptr<U> ptr)
			{
				items_.push_back(ptr.release());
			}

			std::auto_ptr<T> PopBack()
			{
				std::auto_ptr<T> result(items_.back());
				items_.pop_back();
				return result;
			}

			AutoPtrVector & Set(unsigned int index, std::auto_ptr<T> ptr)
			{
				if (index >= Size())
				{
					items_.resize(index + 1, 0);
				}
				else
				{
					delete items_[index];
				}

				items_[index] = ptr.release();

				return *this;
			}

			T * operator[] (unsigned int index) throw()
			{
				return items_[index];
			}

			const T * operator[] (unsigned int index) const throw()
			{
				return items_[index];
			}

			std::auto_ptr<T> Release(unsigned int index) const throw()
			{
				T * value = items_[index];
				items_[index] = 0;
				return std::auto_ptr<T>(value);
			}

			T * Back()
			{
				return items_.back();
			}

			const T * Back() const
			{
				return items_.back();
			}

			void Erase(unsigned int index)
			{
				delete items_[index];
				items_.erase(items_.begin() + index);
			}

			void Resize(unsigned int newSize)
			{
				if (newSize < items_.size())
				{
					for (unsigned int i = newSize; i < items_.size(); ++i)
					{
						delete items_[i];
						items_[i] = 0;
					}

					items_.resize(newSize);
				}
				else if (newSize > items_.size())
				{
					items_.resize(newSize, 0);
				}
			}

			/**
			 * bool comp(const T *, const T *)
			 */
			template <class Compare>
			void Sort(Compare comp)
			{
				std::sort(items_.begin(), items_.end(), comp);
			}

		private:

			AutoPtrVector(const AutoPtrVector &);
			AutoPtrVector & operator= (const AutoPtrVector &);

			VectorType items_;
	};
}

#endif

