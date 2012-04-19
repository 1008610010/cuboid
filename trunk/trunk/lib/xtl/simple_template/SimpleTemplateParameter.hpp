#ifndef XTL__SIMPLE_TEMPLATE_PARAMETER_HPP__
#define XTL__SIMPLE_TEMPLATE_PARAMETER_HPP__ 1

#include "SimpleTemplateValue.hpp"

namespace XTL
{
	class SimpleTemplateParameter
	{
		public:

			explicit SimpleTemplateParameter(unsigned int index)
				: index_(index),
				  value_(0)
			{
				;;
			}

			~SimpleTemplateParameter() throw()
			{
				delete value_;
			}

			unsigned int GetIndex() const
			{
				return index_;
			}

			const SimpleTemplateValue * GetValue() const
			{
				return value_;
			}

			void SetValue(SimpleTemplateValue * newValue)
			{
				if (value_ != newValue) {
					delete value_;
					value_ = newValue;
				}
			}

		private:

			unsigned int index_;
			SimpleTemplateValue * value_;
	};
}

#endif

