#ifndef _XTL__INIT_CONFIG_PARSER_HPP__
#define _XTL__INIT_CONFIG_PARSER_HPP__ 1

#include "Parser.hpp"

namespace XTL
{
	class InitConfig;

	class InitConfigParser : protected Parser
	{
		public:

			using Parser::Error;

			InitConfigParser(const char * source, InitConfig & config);

		protected:

			void SkipSpaces()
			{
				for (; IsSpace(*cur_); Advance()) { ;; }
			}

			// *cur_ == ';'
			void SkipComments();

			void GotoNextLine();

			// before: *cur_ == '['
			// after:  begin of line or end of file
			void ReadSectionName(std::string & s);

			// IsAlpha(*cur)
			void ReadParameterPair(std::string & section);

			void Parse();

		protected:

			InitConfig & config_;
	};
}

#endif
