#include "VariantScalar.hpp"

#include <stdio.h>

#include "StringUtils.hpp"
#include "PrintStream.hpp"

namespace XTL
{
	/*************************** Null ***************************/

	VariantPtr Variant::Null::Instance()
	{
		static VariantPtr instance(new Null());
		return instance;
	}

	Variant::Null::~Null() throw()
	{
		;;
	}

	bool Variant::Null::IsNull() const
	{
		return true;
	}

	void Variant::Null::DebugPrintValue(PrintStream & stream, unsigned int indent) const
	{
		stream.Print("null");
	}

	Variant::LongLongInt * Variant::Null::ToLongLongInt()
	{
		return new Variant::LongLongInt(0);
	}

	Variant::String * Variant::Null::ToString()
	{
		return new Variant::String("");
	}

	/*************************** Boolean ***************************/

	VariantPtr Variant::Boolean::True()
	{
		static VariantPtr instance(new Boolean(true));
		return instance;
	}

	VariantPtr Variant::Boolean::False()
	{
		static VariantPtr instance(new Boolean(false));
		return instance;
	}

	Variant::Boolean::Boolean(bool value)
		: Variant::Scalar(),
		  value_(value)
	{
		;;
	}

	Variant::Boolean::~Boolean() throw()
	{
		;;
	}

	Variant::LongLongInt * Variant::Boolean::ToLongLongInt()
	{
		return new Variant::LongLongInt(value_ ? 1 : 0);
	}

	Variant::String * Variant::Boolean::ToString()
	{
		return new Variant::String(value_ ? "true" : "false");
	}

	void Variant::Boolean::DebugPrintValue(PrintStream & stream, unsigned int indent) const
	{
		stream.Print(value_ ? "true" : "false");
	}

	/*************************** LongLongInt ***************************/

	Variant::LongLongInt::LongLongInt(const long long int & value)
		: Variant::Number(),
		  value_(value)
	{
		;;
	}

	Variant::LongLongInt::~LongLongInt() throw()
	{
		;;
	}

	Variant::LongLongInt * Variant::LongLongInt::ToLongLongInt()
	{
		return this;
	}

	Variant::String * Variant::LongLongInt::ToString()
	{
		return new Variant::String(IntegerToString(value_));
	}

	void Variant::LongLongInt::DebugPrintValue(PrintStream & stream, unsigned int indent) const
	{
		stream.Print("%lld", value_);
	}

	bool Variant::LongLongInt::IsInteger() const
	{
		return true;
	}

	/*************************** Double ***************************/

	Variant::Double::Double(const double & value)
		: Variant::Number(),
		  value_(value)
	{
		;;
	}

	Variant::Double::~Double() throw()
	{
		;;
	}

	Variant::LongLongInt * Variant::Double::ToLongLongInt()
	{
		return new Variant::LongLongInt(static_cast<long long int>(value_));
	}

	void Variant::Double::DebugPrintValue(PrintStream & stream, unsigned int indent) const
	{
		stream.Print("%g", value_);
	}

	bool Variant::Double::IsInteger() const
	{
		return false;
	}

	/*************************** String ***************************/

	Variant::String::String(const std::string & value)
		: Variant::Scalar(),
		  value_(value)
	{
		;;
	}

	Variant::String::String(const char * value)
		: Variant::Scalar(),
		  value_(value)
	{
		;;
	}

	Variant::String::~String() throw()
	{
		;;
	}

	Variant::LongLongInt * Variant::String::ToLongLongInt()
	{
		return new Variant::LongLongInt(StringToInteger<long long int>(value_));
	}

	Variant::String * Variant::String::ToString()
	{
		return this;
	}

	void Variant::String::DebugPrintValue(PrintStream & stream, unsigned int indent) const
	{
		stream.Print("\"");

		unsigned int begin = 0;
		const unsigned int end = value_.size();
		for (unsigned int i = 0; i < end; ++i)
		{
			if (value_[i] == '\\')
			{
				if (begin < i)
				{
					stream.Print(value_.substr(begin, i - begin));
				}
				stream.Print("\\\\");
				begin = i + 1;
			}
			else if (value_[i] == '"')
			{
				if (begin < i)
				{
					stream.Print(value_.substr(begin, i - begin));
				}
				stream.Print("\\\"");
				begin = i + 1;
			}
		}

		if (begin < end)
		{
			stream.Print(value_.substr(begin, end - begin));
		}

		stream.Print("\"");
	}
}
