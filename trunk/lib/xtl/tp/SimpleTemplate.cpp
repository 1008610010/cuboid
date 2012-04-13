#include "SimpleTemplate.hpp"

#include "SimpleTemplateItem.hpp"
#include "SimpleTemplateItemVisitor.hpp"
#include "SimpleTemplateParameter.hpp"
#include "SimpleTemplateParser.hpp"
#include "SimpleTemplateWriter.hpp"

namespace XTL
{
	SimpleTemplate::SimpleTemplate(const char * source)
		: items_(),
		  params_()
	{
		ParserListener parserListener(*this);
		ParseSimpleTemplate(source, parserListener);
	}

	SimpleTemplate::~SimpleTemplate() throw()
	{
		const std::list<SimpleTemplateItem *>::iterator itemsEnd = items_.end();
		for (std::list<SimpleTemplateItem *>::iterator itr = items_.begin(); itr != itemsEnd; ++itr)
		{
			delete *itr;
		}

		const std::vector<SimpleTemplateParameter *>::iterator paramsEnd = params_.end();
		for (std::vector<SimpleTemplateParameter *>::iterator itr = params_.begin(); itr != paramsEnd; ++itr)
		{
			delete *itr;
		}
	}

	void SimpleTemplate::Visit(SimpleTemplateItemVisitor & visitor) const
	{
		const std::list<SimpleTemplateItem *>::const_iterator itemsEnd = items_.end();
		for (std::list<SimpleTemplateItem *>::const_iterator itr = items_.begin(); itr != itemsEnd; ++itr)
		{
			(*itr)->Visit(visitor);
		}
	}

	void SimpleTemplate::OnText(const char * ptr, unsigned int size)
	{
		items_.push_back(new SimpleTemplateItemText(ptr, size));
	}

	void SimpleTemplate::OnParameter(unsigned int paramIndex)
	{
		items_.push_back(new SimpleTemplateItemParameter(CreateParameter(paramIndex)));
	}

	SimpleTemplateParameter & SimpleTemplate::CreateParameter(unsigned int paramIndex)
	{
		SimpleTemplateParameter * param = 0;

		if (paramIndex == params_.size())
		{
			param = new SimpleTemplateParameter(paramIndex);
			params_.push_back(param);
		}
		else if (paramIndex < params_.size())
		{
			param = params_[paramIndex];
			if (param == 0)
			{
				param = new SimpleTemplateParameter(paramIndex);
				params_[paramIndex] = param;
			}
		}
		else
		{
			params_.resize(paramIndex + 1, 0);
			param = new SimpleTemplateParameter(paramIndex);
			params_[paramIndex] = param;
		}

		return *param;
	}

	/**********************************************************************************************************/

	void EvaluateTemplateWithValues(const SimpleTemplate & tmpl, OutputStream & outputStream, const SimpleTemplateValueArray & values)
	{
		SimpleTemplateOutputStreamWriter writer(outputStream, values);
		tmpl.Visit(writer);
	}
}

