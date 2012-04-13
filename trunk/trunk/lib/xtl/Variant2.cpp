#include "Variant2.hpp"

#include <stdexcept>

#include "FormatString.hpp"
#include "StringUtils.hpp"

namespace XTL
{
	/********** VariantValuePtr **********/

	VariantValuePtr::VariantValuePtr()
		: ptr_(0)
	{
		;;
	}

	VariantValuePtr::VariantValuePtr(VariantValue * ptr)
		: ptr_(ptr)
	{
		AddRef();
	}

	VariantValuePtr::VariantValuePtr(const VariantValuePtr & other)
		: ptr_(other.ptr_)
	{
		AddRef();
	}

	VariantValuePtr::~VariantValuePtr() throw()
	{
		Release();
	}

	VariantValuePtr & VariantValuePtr::operator= (const VariantValuePtr & other)
	{
		if (ptr_ != other.ptr_)
		{
			Release();
			ptr_ = other.ptr_;
			AddRef();
		}

		return *this;
	}

	void VariantValuePtr::AddRef()
	{
		if (ptr_ != 0)
		{
			ptr_->AddRef();
		}
	}

	void VariantValuePtr::Release()
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

	/********** Variant **********/

	Variant::Variant()
		: valuePtr_(VariantValue_Null::Create())
	{
		;;
	}

	Variant::Variant(const Variant & other)
		: valuePtr_(other.valuePtr_)
	{
		;;
	}

	Variant & Variant::operator= (const Variant & other)
	{
		valuePtr_ = other.valuePtr_;
		return *this;
	}

	Variant & Variant::operator= (IntegerType & value)
	{
		valuePtr_ = valuePtr_->Assign(value);
		return *this;
	}

	bool Variant::IsNull() const
	{
		return valuePtr_->IsNull();
	}

	void Variant::SetNull()
	{
		valuePtr_ = VariantValue_Null::Create();
	}

	/********** VariantValue **********/

	VariantValuePtr VariantValue::ToInteger() const
	{
		throw std::runtime_error("Variant bad cast");
	}

	VariantValuePtr VariantValue::ToFloat() const
	{
		throw std::runtime_error("Variant bad cast");
	}

	VariantValuePtr VariantValue::ToString() const
	{
		throw std::runtime_error("Variant bad cast");
	}

	VariantValuePtr VariantValue::Assign(Variant::IntegerType data)
	{
		return VariantValue_Integer::Create(data);
	}

	VariantValuePtr VariantValue::Assign(Variant::FloatType data)
	{
		return VariantValue_Float::Create(data);
	}

	VariantValuePtr VariantValue::Assign(const std::string & data)
	{
		return VariantValue_String::Create(data);
	}


	VariantValuePtr VariantValue_Null::Create()
	{
		static VariantValuePtr instance(new VariantValue_Null());

		return instance;
	}

	VariantValuePtr VariantValue_Null::Clone() const
	{
		return Create();
	}

	bool VariantValue_Null::IsNull() const
	{
		return true;
	}


	VariantValuePtr VariantValue_Integer::Create(Variant::IntegerType data)
	{
		return VariantValuePtr(new VariantValue_Integer(data));
	}

	VariantValuePtr VariantValue_Integer::Clone() const
	{
		return Create(data_);
	}

	VariantValuePtr VariantValue_Integer::ToInteger()
	{
		return VariantValuePtr(this);
	}

	VariantValuePtr VariantValue_Integer::ToFloat()
	{
		throw std::runtime_error("VariantValue_Integer::ToFloat() is not implemented");
	}

	VariantValuePtr VariantValue_Integer::ToString()
	{
		return VariantValue_String::Create(IntegerToString(data_));
	}

	VariantValuePtr VariantValue_Integer::Assign(Variant::IntegerType data)
	{
		if (RefCount() == 1)
		{
			data_ = data;
			return VariantValuePtr(this);
		}
		else
		{
			return Create(data);
		}
	}


	VariantValuePtr VariantValue_Float::Create(Variant::FloatType data)
	{
		return VariantValuePtr(new VariantValue_Float(data));
	}

	VariantValuePtr VariantValue_Float::Clone() const
	{
		return Create(data_);
	}

	VariantValuePtr VariantValue_Float::ToInteger()
	{
		return VariantValue_Integer::Create(static_cast<Variant::IntegerType>(data_));
	}

	VariantValuePtr VariantValue_Float::ToFloat()
	{
		return VariantValuePtr(this);
	}

	VariantValuePtr VariantValue_Float::ToString()
	{
		return VariantValue_String::Create(FormatString("%g", data_));
	}


	VariantValuePtr VariantValue_Float::Assign(Variant::IntegerType data)
	{
		return VariantValue_Integer::Create(data);
	}

}

