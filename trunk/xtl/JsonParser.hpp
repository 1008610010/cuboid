#ifndef _XTL__JSON_PARSER_HPP__
#define _XTL__JSON_PARSER_HPP__ 1

#include "Json.hpp"

namespace XTL
{
	class JsonParser
	{
		public:

			class Error
			{
				public:
					Error(int row, int column, const char * what)
						: row_(row), column_(column), what_(what) { ;; }

					int Row() const            { return row_; }
					int Column() const         { return column_; }
					const char * What() const  { return what_.c_str(); }

				protected:
					int         row_;
					int         column_;
					std::string what_;
			};

			JsonParser(const char * source);

			~JsonParser() throw();

			JsonValue * Release();

		protected:

			bool IsSpace(char c)    { return c == ' ' || c == '\t' || c == '\r'; }

			bool IsNewLine(char c)  { return c == '\n'; }

			bool IsDigit(char c)    { return c >= '0' && c <= '9'; }

			bool IsAlpha(char c)    { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'; }

			void Advance()          { ++cur_; ++column_; }

			void AdvanceNewLine()   { ++cur_; ++row_; column_ = 0; }

			void SkipSpaces();

			void ReadKeyword(std::string & s);

			void ReadString(std::string & s);

			JsonValue * ReadNumber(JsonValue * parent);

			JsonValue * ReadString(JsonValue * parent);

			JsonValue * ReadValue(JsonValue * parent);

			JsonValue * ReadArray(JsonValue * parent);

			JsonValue * ReadObject(JsonValue * parent);

			void Parse();

			JsonValue * root_;
			int row_;
			int column_;
			const char * cur_;
	};
}

#endif
