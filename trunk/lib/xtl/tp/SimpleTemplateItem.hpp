#ifndef XTL__SIMPLE_TEMPLATE_ITEM_HPP__
#define XTL__SIMPLE_TEMPLATE_ITEM_HPP__ 1

#include <string>

namespace XTL
{
	class SimpleTemplateItemVisitor;
	class SimpleTemplateParameter;

	class SimpleTemplateItem
	{
		public:

			virtual ~SimpleTemplateItem() throw() { ;; }

			virtual void Visit(SimpleTemplateItemVisitor & visitor) const = 0;
	};

	class SimpleTemplateItemText : public SimpleTemplateItem
	{
		public:

			explicit SimpleTemplateItemText(const char * ptr, unsigned int size)
				: value_(ptr, size) { ;; }

			virtual ~SimpleTemplateItemText() throw() { ;; }

			virtual void Visit(SimpleTemplateItemVisitor & visitor) const;

			unsigned int Size() const
			{
				return value_.size();
			}

			const char * Chars() const
			{
				return value_.data();
			}

			const std::string & GetString() const
			{
				return value_;
			}

		private:

			const std::string value_;
	};

	class SimpleTemplateItemParameter : public SimpleTemplateItem
	{
		public:

			explicit SimpleTemplateItemParameter(const SimpleTemplateParameter & parameter)
				: parameter_(parameter) { ;; }

			virtual ~SimpleTemplateItemParameter() throw() { ;; }

			virtual void Visit(SimpleTemplateItemVisitor & visitor) const;

			const SimpleTemplateParameter & GetParameter() const
			{
				return parameter_;
			}

		private:

			const SimpleTemplateParameter & parameter_;
	};
}

#endif

