#ifndef _XTL__PARSER_HPP__
#define _XTL__PARSER_HPP__ 1

#include <string>

namespace XTL
{
	class Parser
	{
		public:

			class Error
			{
				public:
					Error(int row, int column, const std::string & what)
						: row_(row), column_(column), what_(what) { ;; }

					int Row() const            { return row_; }
					int Column() const         { return column_; }
					const char * What() const  { return what_.c_str(); }

				protected:
					int         row_;
					int         column_;
					std::string what_;
			};

			explicit Parser(const char * source);

		protected:

			bool IsSpace(char c)    { return c == ' ' || c == '\t' || c == '\r'; }

			bool IsNewLine(char c)  { return c == '\n'; }

			bool IsDigit(char c)    { return c >= '0' && c <= '9'; }

			bool IsAlpha(char c)    { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'; }

			void Advance()          { ++cur_; ++column_; }

			void AdvanceNewLine()   { ++cur_; ++row_; column_ = 0; }

			void RaiseError(const std::string & what)
			{
				throw Error(row_, column_, what);
			}

			void ParseIdentifier(std::string & s);

			void ParseIntegerLiteral(long long & i);

			// true, if integer literal; false, if real
			bool ParseNumberLiteral(long long & i, double & f);

			void ParseStringLiteral(std::string & s);

			const char * cur_;
			int          row_;
			int          column_;
	};
}

#endif
