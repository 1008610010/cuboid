#include "TemplateTreeBuilder.hpp"

namespace XTL
{
	void TemplateNodeList::Evaluate(const TemplateContext & context, CharBuffer & output) const
	{
		for (unsigned int i = 0; i < items_.Size(); ++i)
		{
			items_[i]->Evaluate(context, output);
		}
	}

	void TemplateNodeList::DebugPrint(unsigned int indent) const
	{
		for (unsigned int i = 0; i < items_.Size(); ++i)
		{
			items_[i]->DebugPrint(indent);
		}
	}
}
