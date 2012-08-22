#ifndef XTL__VARIANT_PTR_HPP__
#define XTL__VARIANT_PTR_HPP__ 1

#include <string>

#include "Variant.hpp"

namespace XTL
{
	class VariantPtr
	{
		public:

			VariantPtr()
				: ptr_(0)
			{
				;;
			}

			explicit VariantPtr(Variant * ptr)
				: ptr_(ptr)
			{
				AddRef();
			}

			VariantPtr(const VariantPtr & other)
				: ptr_(other.ptr_)
			{
				AddRef();
			}

			~VariantPtr() throw()
			{
				Release();
			}

			VariantPtr & operator= (Variant * ptr)
			{
				if (ptr_ != ptr)
				{
					Release();
					ptr_ = ptr;
					AddRef();
				}

				return *this;
			}

			VariantPtr & operator= (const VariantPtr & other)
			{
				return (*this = other.ptr_);
			}

			bool IsNull() const { return ptr_ == 0; }

			Variant * operator-> ()       { return ptr_; }
			Variant * operator-> () const { return ptr_; }
			Variant & operator*  ()       { return *ptr_; }
			Variant & operator*  () const { return *ptr_; }

			long long int ToLongLongInt();

			const std::string & ToString();

			Variant::Array & ToArray();

		private:

			void AddRef();

			void Release();

			Variant * ptr_;
	};
}

#endif
