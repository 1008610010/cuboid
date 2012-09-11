#include "TemplateTreeBuilder.hpp"

namespace XTL
{
	void TemplateNodeList::DebugPrint(unsigned int indent) const
	{
		for (unsigned int i = 0; i < items_.Size(); ++i)
		{
			items_[i]->DebugPrint(indent);
		}
	}
}
