#ifndef _XTL__JSON_PARSER_HPP__
#define _XTL__JSON_PARSER_HPP__ 1

#include "Json.hpp"
#include "Parser.hpp"

namespace XTL
{
	class JsonParser : protected Parser
	{
		public:

			using Parser::Error;

			JsonParser(const char * source);

			~JsonParser() throw();

			JsonValue * Release();

		protected:

			void SkipSpaces();

			JsonValue * ReadValue(JsonValue * parent);

			JsonValue * ReadNumber(JsonValue * parent);

			JsonValue * ReadString(JsonValue * parent);

			JsonValue * ReadArray(JsonValue * parent);

			JsonValue * ReadObject(JsonValue * parent);

			void Parse();

			JsonValue  * root_;
	};
}

#endif
