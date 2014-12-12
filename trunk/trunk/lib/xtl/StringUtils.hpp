#ifndef XTL__STRING_UTILS_HPP__
#define XTL__STRING_UTILS_HPP__ 1

#include <string>

#include "TypeTraits.hpp"
#include "utils/Algorithm.hpp"

namespace XTL
{
	void ToLowerCase(std::string & s);

	void ToUpperCase(std::string & s);

	const std::string LowerCased(const std::string & s);

	const std::string UpperCased(const std::string & s);

	bool IsInteger(const char * s);

	bool IsInteger(const std::string & s);

	bool IsNumber(const char * s);

	bool IsNumber(const std::string & s);

	template <typename T>
	const char * StringToInteger(const char * src, T & result)
	{
		const char * p = src;
		result = 0;

		if (p == 0 || *p == '\0')
		{
			return p;
		}

		bool negative = false;
		if (*p == '-')
		{
			negative = true;
			++p;
		}

		for (; *p >= '0' && *p <= '9'; ++p)
		{
			result = 10 * result + (*p - '0');
		}

		if (negative)
		{
			result = -result;
		}

		return p;
	}

	template <typename T>
	T StringToInteger(const char * src)
	{
		T result = 0;
		StringToInteger(src, result);
		return result;
	}

	template <typename T>
	T StringToInteger(const std::string & src)
	{
		return StringToInteger<T>(src.c_str());
	}

	template <typename T>
	T StringToFloat(const char * src)
	{
		const char * p = src;

		if (p == 0 || *p == '\0')
		{
			return 0;
		}

		bool negative = false;
		if (*p == '-')
		{
			negative = true;
			++p;
		}

		// TODO: Look ahead of p and count number of digits. If it possible parse integer part to UINT_64
		T result = 0;
		for (; *p >= '0' && *p <= '9'; ++p)
		{
			result = 10 * result + (*p - '0');
		}

		if (*p == '.')
		{
			++p;

			double d = 1.0;
			for (; *p >= '0' && *p <= '9'; ++p)
			{
				d /= 10.0;
				result += (*p - '0') * d;
			}
		}

		if (*p == 'e' || *p == 'E')
		{
			++p;

			bool negativeExponent = false;
			if (*p == '+' || *p == '-')
			{
				negativeExponent = (*p == '-');
				++p;
			}

			long long e = 0;
			for (; *p >= '0' && *p <= '9'; ++p)
			{
				e = 10 * e + (*p - '0');
			}

			T exponent = Pow(static_cast<T>(10.0), e);

			if (negativeExponent)
			{
				result /= exponent;
			}
			else
			{
				result *= exponent;
			}
		}

		return negative ? -result : result;
	}

	template <typename T>
	T StringToFloat(const std::string & src)
	{
		return StringToFloat<T>(src.c_str());
	}

	/**
	 * Получить строковое представление целого числа без знака.
	 * @param value Исходное целое.
	 * @param ptr   Указатель на символ, следующий за последним символом принимающего буфера. Принимающий буфер необходимого размера должна выделить вызывающая сторона.
	 * @return Указатель на первый символ строкового представления.
	 */
	template <typename T>
	char * ReverseStringifyUnsignedInteger(T value, char * ptr)
	{
		if (value == 0)
		{
			--ptr;
			*ptr = '0';
			return ptr;
		}

		do
		{
			--ptr;
			*ptr = '0' + (value % 10);
			value /= 10;
		}
		while (value != 0);

		return ptr;
	}

	/**
	 * @see ReverseStringifyUnsignedInteger
	 */
	template <typename T>
	char * ReverseStringifySignedInteger(T value, char * ptr)
	{
		if (value == 0)
		{
			--ptr;
			*ptr = '0';
			return ptr;
		}
		else if (value > 0)
		{
			do
			{
				--ptr;
				*ptr = '0' + (value % 10);
				value /= 10;
			}
			while (value != 0);
		}
		else
		{
			do
			{
				--ptr;
				*ptr = '0' - (value % 10);
				value /= 10;
			}
			while (value != 0);
			--ptr;
			*ptr = '-';
		}
		return ptr;
	}

	namespace INTERNAL
	{
		template <bool IS_SIGNED> struct IntegerStringifierHelper
		{
			template <typename T>
			static char * Stringify(T value, char * ptr)
			{
				// Шаблон по умолчанию для знаковых целых
				return ReverseStringifySignedInteger(value, ptr);
			}
		};

		template <> struct IntegerStringifierHelper<false>
		{
			template <typename T>
			static char * Stringify(T value, char * ptr)
			{
				return ReverseStringifyUnsignedInteger(value, ptr);
			}
		};
	}

	template <typename T>
	class IntegerStringifier
	{
		public:

			static const unsigned int BUFFER_ALIGNMENT = sizeof(void *);
			static const unsigned int BUFFER_SIZE = AlignedCeil<TypeTraits<T>::MaxDecChars + 1, BUFFER_ALIGNMENT>::Value; // +1 - для нулевого символа

			explicit IntegerStringifier(const T & value)
				: ptr_(INTERNAL::IntegerStringifierHelper<TypeTraits<T>::IsSigned>::Stringify(value, buffer_ + BUFFER_SIZE - 1))
			{
				buffer_[BUFFER_SIZE - 1] = '\0';
			}

			unsigned int Size() const
			{
				return (buffer_ + BUFFER_SIZE - 1) - ptr_;
			}

			const char * const Chars() const
			{
				return ptr_;
			}

			const std::string ToString() const
			{
				return std::string(Chars(), Size());
			}

		private:

			// const T value_; // ??
			char buffer_[BUFFER_SIZE];
			const char * const ptr_;
	};

	// TODO: Refactor this bullshit. Add precision parameters.
	class FloatStringifier
	{
		public:

			static const unsigned int BUFFER_SIZE = 32;

			FloatStringifier(double value);

			unsigned int Size() const
			{
				return size_;
			}

			const char * Chars() const
			{
				return buffer_;
			}

			const std::string ToString() const
			{
				return std::string(Chars(), Size());
			}

		private:

			FloatStringifier(const FloatStringifier &);
			FloatStringifier & operator= (const FloatStringifier &);

			char buffer_[BUFFER_SIZE];
			unsigned int size_;
	};

	template <typename T>
	const std::string IntegerToString(const T & value)
	{
		return IntegerStringifier<T>(value).ToString();
	}

	/*
		const std::string source("abc,,xyzd,");

		XTL::StringSplitter splitter(source.c_str(), ',');
		std::string label;
		while (splitter.GetNext(label))
		{
			...
		}
	*/
	class StringSplitter
	{
		public:

			StringSplitter(const char * source, char delimiter);

			bool GetNext(std::string & result);

		private:

			const char * const source_;
			const char * begin_;
			const char delimiter_;
	};
}

#endif

