#include "VariantPtr.hpp"

#include "Variant.hpp"
#include "VariantScalar.hpp"
#include "VariantArray.hpp"
#include "VariantStruct.hpp"

namespace XTL
{
	void VariantPtr::AddRef()
	{
		if (ptr_ != 0)
		{
			ptr_->AddRef();
		}
	}

	void VariantPtr::Release()
	{
		if (ptr_ != 0)
		{
			if (ptr_->Release())
			{
				delete ptr_;
			}

			ptr_ = 0;
		}
	}

	long long int VariantPtr::ToLongLongInt()
	{
		Variant::LongLongInt * newPtr = ptr_->ToLongLongInt();
		*this = newPtr;
		return newPtr->GetValue();
	}

	const std::string & VariantPtr::ToString()
	{
		Variant::String * newPtr = ptr_->ToString();
		*this = newPtr;
		return newPtr->GetValue();
	}

	Variant::Array & VariantPtr::ToArray()
	{
		return *(ptr_->ToArray());
	}
}
