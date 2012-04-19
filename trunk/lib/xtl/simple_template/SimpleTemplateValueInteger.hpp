#ifndef XTL__SIMPLE_TEMPLATE_VALUE_INTEGER_HPP__
#define XTL__SIMPLE_TEMPLATE_VALUE_INTEGER_HPP__ 1

#include "../StringUtils.hpp"

namespace XTL
{
	template <typename T>
	class SimpleTemplateValueInteger : public SimpleTemplateValue
	{
		public:

			SimpleTemplateValueInteger(T value)
				: stringifier_(value) { ;; }

			virtual ~SimpleTemplateValueInteger() throw() { ;; }

			virtual const char * Chars() const
			{
				return stringifier_.Chars();
			}

			virtual unsigned int Size() const
			{
				return stringifier_.Size();
			}

		private:

			const IntegerStringifier<T> stringifier_;
	};

#define DECLARE_SIMPLE_TEMPLATE_INTEGER_CREATOR(TYPE) \
	template <> \
	struct SimpleTemplateValueCreator<TYPE> \
	{ \
		static SimpleTemplateValue * CreateCopy(const TYPE & obj) \
		{ \
			return new SimpleTemplateValueInteger<TYPE>(obj); \
		} \
		static SimpleTemplateValue * CreateReference(const TYPE & obj) \
		{ \
			return CreateCopy(obj); \
		} \
	};

	DECLARE_SIMPLE_TEMPLATE_INTEGER_CREATOR(short int)
	DECLARE_SIMPLE_TEMPLATE_INTEGER_CREATOR(int)
	DECLARE_SIMPLE_TEMPLATE_INTEGER_CREATOR(long int)
	DECLARE_SIMPLE_TEMPLATE_INTEGER_CREATOR(long long int)

	DECLARE_SIMPLE_TEMPLATE_INTEGER_CREATOR(unsigned short int)
	DECLARE_SIMPLE_TEMPLATE_INTEGER_CREATOR(unsigned int)
	DECLARE_SIMPLE_TEMPLATE_INTEGER_CREATOR(unsigned long int)
	DECLARE_SIMPLE_TEMPLATE_INTEGER_CREATOR(unsigned long long int)
}

#endif

