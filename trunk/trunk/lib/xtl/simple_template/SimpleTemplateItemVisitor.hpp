#ifndef XTL__SIMPLE_TEMPLATE_ITEM_VISITOR_HPP__
#define XTL__SIMPLE_TEMPLATE_ITEM_VISITOR_HPP__

#include "SimpleTemplateItem.hpp"

namespace XTL
{
	class SimpleTemplateItemVisitor
	{
		public:

			virtual ~SimpleTemplateItemVisitor() throw() { ;; }

			virtual void Visit(const SimpleTemplateItemText & item) = 0;

			virtual void Visit(const SimpleTemplateItemParameter & item) = 0;
	};
}

#endif

