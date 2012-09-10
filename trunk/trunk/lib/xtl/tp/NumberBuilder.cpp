#include "NumberBuilder.hpp"

#include <math.h>

namespace XTL
{
	/*** IntegerBuilder ***/

	IntegerBuilder::IntegerBuilder()
		: value_(0),
		  isNegative_(false)
	{
		;;
	}

	const Number IntegerBuilder::Release() const
	{
		if (isNegative_)
		{
			return Number(-static_cast<INT_64>(value_));
		}
		else
		{
			return Number(value_);
		}
	}

	void IntegerBuilder::SetNegative()
	{
		if (isNegative_)
		{
			return;
		}

		if (value_ > MAX_SIGNED)
		{
			throw OverflowError();
		}

		isNegative_ = true;
	}

	void IntegerBuilder::OnDecimalDigit(unsigned int digit)
	{
		if (CanAppendIntegerDigit(digit))
		{
			value_ = 10 * value_ + digit;
		}
		else
		{
			throw OverflowError();
		}
	}

	void IntegerBuilder::OnBinaryDigit(unsigned int digit)
	{
		if (value_ > (MAX_UNSIGNED >> 1) + 1)
		{
			throw OverflowError();
		}

		value_ = (value_ << 1) + digit;
	}

	void IntegerBuilder::OnOctalDigit(unsigned int digit)
	{
		if (value_ > (MAX_UNSIGNED >> 3) + 1)
		{
			throw OverflowError();
		}

		value_ = (value_ << 3) + digit;
	}

	void IntegerBuilder::OnHexadecimalDigit(unsigned int digit)
	{
		if (value_ > (MAX_UNSIGNED >> 4) + 1)
		{
			throw OverflowError();
		}

		value_ = (value_ << 4) + digit;
	}

	bool IntegerBuilder::CanAppendIntegerDigit(unsigned int digit) const
	{
		if (isNegative_)
		{
			return value_ < MAX_SIGNED / 10 || (value_ == MAX_SIGNED / 10 && digit <= MAX_SIGNED % 10);
		}
		else
		{
			return value_ < MAX_UNSIGNED / 10 || (value_ == MAX_UNSIGNED / 10 && digit <= MAX_UNSIGNED % 10);
		}
	}

	/*** NumberBuilder ***/

	const Number NumberBuilder::Release() const
	{
		if (isFloat_)
		{
			INT_32 e = (isNegativeExponent_ ? -exponent_ : exponent_) + exponentDelta_;
			return Number((isNegative_ ? -f_ : f_) * ::pow(10.0, static_cast<double>(e)));
		}
		else if (isNegative_)
		{
			return Number(-static_cast<INT_64>(i_));
		}
		else
		{
			return Number(i_);
		}
	}

	void NumberBuilder::SetNegative()
	{
		if (isNegative_)
		{
			return;
		}

		isNegative_ = true;

		if (!isFloat_ && i_ > MAX_SIGNED)
		{
			SetFloat();
		}
	}

	void NumberBuilder::OnIntegerDigit(unsigned int digit)
	{
		if (!isFloat_)
		{
			if (CanAppendIntegerDigit(digit))
			{
				i_ = 10 * i_ + digit;
				return;
			}

			SetFloat();
		}

		f_ = 10 * f_ + digit;
	}

	void NumberBuilder::OnFractionalDigit(int digit)
	{
		SetFloat();
		f_ = 10 * f_ + digit;
		--exponentDelta_;
	}

	void NumberBuilder::SetNegativeExponent()
	{
		SetFloat();
		isNegativeExponent_ = true;
	}

	void NumberBuilder::OnExponentDigit(int digit)
	{
		if (exponent_ < MAX_EXPONENT)
		{
			exponent_ = 10 * exponent_ + digit;
		}
	}

	void NumberBuilder::SetFloat()
	{
		if (!isFloat_)
		{
			f_ = static_cast<double>(i_);
			isFloat_ = true;
		}
	}

	bool NumberBuilder::CanAppendIntegerDigit(unsigned int digit) const
	{
		if (isNegative_)
		{
			return i_ < MAX_SIGNED / 10 || (i_ == MAX_SIGNED / 10 && digit <= MAX_SIGNED % 10);
		}
		else
		{
			return i_ < MAX_UNSIGNED / 10 || (i_ == MAX_UNSIGNED / 10 && digit <= MAX_UNSIGNED % 10);
		}
	}
}
