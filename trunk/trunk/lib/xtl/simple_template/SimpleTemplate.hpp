#ifndef XTL__SIMPLE_TEMPLATE_HPP__
#define XTL__SIMPLE_TEMPLATE_HPP__ 1

#include <list>
#include <string>
#include <vector>

#include "SimpleTemplateValueArray.hpp"
#include "../io/StringOutputStream.hpp"

namespace XTL
{
	class OutputStream;
	class SimpleTemplateItem;
	class SimpleTemplateItemVisitor;
	class SimpleTemplateParameter;
	class SimpleTemplateValueArray;

	class SimpleTemplate
	{
		public:

			explicit SimpleTemplate(const char * source);

			~SimpleTemplate() throw();

			unsigned int ParamsCount() const
			{
				return params_.size();
			}

			SimpleTemplateParameter * GetParameter(unsigned int paramIndex)
			{
				return paramIndex < params_.size() ? params_[paramIndex] : 0;
			}

			void Visit(SimpleTemplateItemVisitor & visitor) const;

		protected:

			void OnText(const char * ptr, unsigned int size);

			void OnParameter(unsigned int paramIndex);

			SimpleTemplateParameter & CreateParameter(unsigned int paramIndex);

		private:

			class ParserListener
			{
				public:

					explicit ParserListener(SimpleTemplate & tmpl)
						: tmpl_(tmpl) { ;; }

					void OnText(const char * ptr, unsigned int size)
					{
						tmpl_.OnText(ptr, size);
					}

					void OnParameter(unsigned int paramIndex)
					{
						tmpl_.OnParameter(paramIndex);
					}

					SimpleTemplate & tmpl_;
			};

			std::list<SimpleTemplateItem *> items_;
			std::vector<SimpleTemplateParameter *> params_;
	};


	void EvaluateTemplateWithValues(const SimpleTemplate & tmpl, OutputStream & outputStream, const SimpleTemplateValueArray & values);


	template <typename T1>
	void WriteTemplate(const SimpleTemplate & tmpl, OutputStream & outputStream, T1 t1)
	{
		SimpleTemplateValueArray values(1);
		values.SetReference(0, t1);
		EvaluateTemplateWithValues(tmpl, outputStream, values);
	}

	template <typename T1, typename T2>
	void WriteTemplate(const SimpleTemplate & tmpl, OutputStream & outputStream, T1 t1, T2 t2)
	{
		SimpleTemplateValueArray values(2);
		values.SetReference(0, t1);
		values.SetReference(1, t2);
		EvaluateTemplateWithValues(tmpl, outputStream, values);
	}


	template <typename T1>
	const std::string TemplateToString(const SimpleTemplate & tmpl, const T1 & t1)
	{
		StringOutputStream outputStream;
		WriteTemplate(tmpl, outputStream, t1);
		return outputStream.ToString();
	}

	template <typename T1, typename T2>
	const std::string TemplateToString(const SimpleTemplate & tmpl, const T1 & t1, const T2 & t2)
	{
		StringOutputStream outputStream;
		WriteTemplate(tmpl, outputStream, t1, t2);
		return outputStream.ToString();
	}
}

#endif

