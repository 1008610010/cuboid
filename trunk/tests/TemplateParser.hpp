#ifndef XTL__TEMPLATE_PARSER_HPP__
#define XTL__TEMPLATE_PARSER_HPP__ 1

#include <string>

#include "../lib/xtl/tp/CharSource.hpp"
#include "../lib/xtl/tp/Parser.hpp"

namespace XTL
{
	class TemplateTreeBuilder;

	class TemplateParser : protected Parser
	{
		public:

			TemplateParser(CharSource & charSource, TemplateTreeBuilder & builder);

			void Parse();

		private:

			void SkipWhitespaces();

			const std::string NeedIdentifier();

			TemplateTreeBuilder & builder_;
	};
}

#endif
