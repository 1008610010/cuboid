#ifndef XTL__INI_CONFIG_PARSER_HPP__
#define XTL__INI_CONFIG_PARSER_HPP__ 1

#include <string>

#include "IniConfig.hpp"
#include "../tp/Parser.hpp"

namespace XTL
{
	class IniConfigParser : public Parser
	{
		public:

			IniConfigParser(CharSource & charSource, IniConfig & config);

		private:

			void ParseLine();

			/*
			 * ... '[' \s* [A-Za-z_][A-Za-z0-9_]* \s* ']' ...
			 *   ---^                                     ^---
			 */
			void ReadSection();

			void ReadKeyValue();

			/*
			 * " ( [^\\"] | '\\' [rnt\\"]  )* "
			 * ' ( [^\\'] | '\\' [\\'] )* '
			 * -? [0-9]+ ( '.' [0-9]+ )? ( [eE] [+-] [0-9]+ )
			 * '0' ( ('x' [0-9A-Fa-f]+) | ('b' [0-1]+) | ([0-7]*) )
			 */
			VariantPtr ReadValue();

			void SkipComments();

			void SkipNewLine();

			IniConfig   & config_;
			std::string   currentSection_;
	};
}

#endif
