#ifndef XTL__SIMPLE_TEMPLATE_WRITER_HPP__
#define XTL__SIMPLE_TEMPLATE_WRITER_HPP__ 1

#include "SimpleTemplateItemVisitor.hpp"

namespace XTL
{
	class OutputStream;
	class SimpleTemplateItemParameter;
	class SimpleTemplateItemText;
	class SimpleTemplateValueArray;

	class SimpleTemplateOutputStreamWriter : public SimpleTemplateItemVisitor
	{
		public:

			SimpleTemplateOutputStreamWriter(OutputStream & outputStream, const SimpleTemplateValueArray & values);

			virtual void Visit(const SimpleTemplateItemText & item);

			virtual void Visit(const SimpleTemplateItemParameter & item);

		private:

			OutputStream & outputStream_;
			const SimpleTemplateValueArray & values_;
	};
}

#endif

