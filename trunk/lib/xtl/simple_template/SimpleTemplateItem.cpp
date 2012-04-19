#include "SimpleTemplateItem.hpp"

#include "SimpleTemplateItemVisitor.hpp"
#include "SimpleTemplateParameter.hpp"

namespace XTL
{
	void SimpleTemplateItemText::Visit(SimpleTemplateItemVisitor & visitor) const
	{
		visitor.Visit(*this);
	}

	void SimpleTemplateItemParameter::Visit(SimpleTemplateItemVisitor & visitor) const
	{
		visitor.Visit(*this);
	}
}

