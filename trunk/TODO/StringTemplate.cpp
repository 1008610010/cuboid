#include "StringTemplate.hpp"
#include "CharStream.hpp"
#include "Exception.hpp"
#include "StringUtils.hpp"

namespace XTL
{
	namespace
	{
		bool IsDigit(char ch)
		{
			return ch >= '0' && ch <= '9';
		}
	}

	/***** StringTemplateContext *****/

	StringTemplateContext::StringTemplateContext(unsigned int varsCount)
		: vars_(varsCount)
	{
		;;
	}

	StringTemplateVar & StringTemplateContext::operator[] (unsigned int index)
	{
		if (index >= vars_.size())
		{
			vars_.resize(index + 1);
		}

		return vars_[index];
	}

	void StringTemplateContext::Write(CharStream & stream, unsigned int index) const
	{
		if (index < vars_.size())
		{
			const StringTemplateVar & var(vars_[index]);

			if (!stream.Write(var.Chars(), var.Size()))
			{
				ThrowRuntimeError("Stream write error");
			}
		}
	}

	/***** StringTemplate *****/

	StringTemplate::StringTemplate(const char * format)
		: root_(0)
	{
		Parse(format);
	}

	StringTemplate::~StringTemplate() throw()
	{
		delete root_;
	}

	void StringTemplate::Parse(const char * format)
	{
		delete root_;
		root_ = new List();

		if (format == 0)
		{
			return;
		}

		CharStream constant;
		const char * current = format;
		const char * constantBegin = current;

		while (*current != '\0')
		{
			if (*current == '$')
			{
				// dollar escape '...$$...'
				if (*(current + 1) == '$')
				{
					++current;
					constant.Write(constantBegin, current - constantBegin);
					++current;
					constantBegin = current;
					continue;
				}

				const char * constantEnd = current;

				++current;

				bool brackets = (*current == '{');
				if (brackets)
				{
					++current;
				}

				// variable '...$[0-9]+...' or '...${[0-9]+}...'
				if (IsDigit(*current))
				{
					if (constant.Size() > 0)
					{
						if (constantBegin < constantEnd)
						{
							constant.Write(constantBegin, constantEnd - constantBegin);
						}
						root_->Add(new Constant(constant.Chars(), constant.Size()));
						constant.Clear();
					}
					else if (constantBegin < constantEnd)
					{
						root_->Add(new Constant(constantBegin, constantEnd - constantBegin));
					}

					unsigned long int varIndex = 0;

					do
					{
						varIndex = 10 * varIndex + (*current - '0');
						++current;
					}
					while (IsDigit(*current));

					root_->Add(new Variable(varIndex));

					if (brackets && *current == '}')
					{
						++current;
					}

					constantBegin = current;
					continue;
				}
			}
			++current;
		}

		if (constant.Size() > 0)
		{
			if (constantBegin < current)
			{
				constant.Write(constantBegin, current - constantBegin);
			}
			root_->Add(new Constant(constant.Chars(), constant.Size()));
			constant.Clear();
		}
		else if (constantBegin < current)
		{
			root_->Add(new Constant(constantBegin, current - constantBegin));
		}
	}

	void StringTemplate::Evaluate(CharStream & output, const StringTemplateContext & context) const
	{
		if (root_ != 0)
		{
			root_->Write(output, context);
		}
	}

	const std::string StringTemplate::Evaluate(const StringTemplateContext & context) const
	{
		CharStream stream;
		Evaluate(stream, context);
		return std::string(stream.Chars(), stream.Size());
	}

	/***** StringTemplate::Constant *****/

	void StringTemplate::Constant::Write(CharStream & stream, const StringTemplateContext & context) const
	{
		if (!stream.Write(value_.data(), value_.size()))
		{
			ThrowRuntimeError("Stream write error");
		};
	}

	/***** StringTemplate::Variable *****/

	void StringTemplate::Variable::Write(CharStream & stream, const StringTemplateContext & context) const
	{
		context.Write(stream, index_);
	}

	/***** StringTemplate::List *****/

	StringTemplate::List::~List() throw()
	{
		typedef std::list<Node *>::const_iterator Iterator;

		const Iterator end(items_.end());
		for (Iterator itr(items_.begin()); itr != end; ++itr)
		{
			delete *itr;
		}
	}

	void StringTemplate::List::Write(CharStream & stream, const StringTemplateContext & context) const
	{
		typedef std::list<Node *>::const_iterator Iterator;

		const Iterator end(items_.end());
		for (Iterator itr(items_.begin()); itr != end; ++itr)
		{
			(*itr)->Write(stream, context);
		}
	}


	void operator<< (StringTemplateVar & var, const char * value)
	{
		var.Set(value, ::strlen(value));
	}

	void operator<< (StringTemplateVar & var, const std::string & value)
	{
		var.Set(value.data(), value.size());
	}

	void operator<< (StringTemplateVar & var, int value)
	{
		XTL::IntegerDigits<int> digits(value);
		var.Set(digits.Chars(), digits.Size());
	}

	void operator<< (StringTemplateVar & var, unsigned int value)
	{
		XTL::IntegerDigits<unsigned int> digits(value);
		var.Set(digits.Chars(), digits.Size());
	}

	void operator<< (StringTemplateVar & var, long long int value)
	{
		XTL::IntegerDigits<long long int> digits(value);
		var.Set(digits.Chars(), digits.Size());
	}

	void operator<< (StringTemplateVar & var, unsigned long long int value)
	{
		XTL::IntegerDigits<unsigned long long int> digits(value);
		var.Set(digits.Chars(), digits.Size());
	}
}
