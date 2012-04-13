#ifndef XTL__SIMPLE_TEMPLATE_VALUE_ARRAY_HPP__
#define XTL__SIMPLE_TEMPLATE_VALUE_ARRAY_HPP__ 1

#include <memory>

#include "SimpleTemplateValue.hpp"

namespace XTL
{
	class SimpleTemplateValueArray
	{
		public:

			explicit SimpleTemplateValueArray(unsigned int size)
				: size_(size),
				  values_(new std::auto_ptr<SimpleTemplateValue>[size])
			{
				;;
			}

			~SimpleTemplateValueArray() throw()
			{
				delete [] values_;
			}

			unsigned int Size() const
			{
				return size_;
			}

			const SimpleTemplateValue * const operator[] (unsigned int index) const
			{
				return values_[index].get();
			}

			template <typename T>
			void SetCopy(unsigned int index, const T & value)
			{
				values_[index].reset(CreateSimpleTemplateValueCopy(value));
			}

			template <typename T>
			void SetReference(unsigned int index, const T & value)
			{
				values_[index].reset(CreateSimpleTemplateValueReference(value));
			}

		private:

			const unsigned int size_;
			std::auto_ptr<SimpleTemplateValue> * const values_;
	};
}

#endif

