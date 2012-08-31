#ifndef XTL__STRING_LITERAL_PARSER_HPP__
#define XTL__STRING_LITERAL_PARSER_HPP__ 1

#include "../CharBuffer.hpp"
#include "Parser.hpp"

namespace XTL
{
	class StringLiteralParser : protected Parser
	{
		public:

			StringLiteralParser(CharSource & charSource, char boundingChar, char escapeSequenceChar)
				: Parser(charSource),
				  boundingChar_(boundingChar),
				  escapeSequenceChar_(escapeSequenceChar)
			{
				;;
			}

			virtual ~StringLiteralParser() throw()
			{
				;;
			}

			const std::string Parse()
			{
				// Assert( NeedChar() == boundingChar_ )

				Mark();

				CharBuffer result;

				Advance();
				Mark();

				while (true)
				{
					if (AtEnd())
					{
						Unmark();
						ThrowError(ReleaseCursor(), "Unclosed string literal");
					}

					char c = GetChar();
					if (c == boundingChar_)
					{
						break;
					}
					else if (c == escapeSequenceChar_)
					{
						ReleaseString(result);

						Advance();

						try
						{
							ParseEscapeSequence(result);
						}
						catch (const Error & e)
						{
							Unmark();
							throw;
						}

						Mark();
					}
					else
					{
						Advance();
					}
				}

				ReleaseString(result);
				Advance();

				Unmark();

				return result.ToString();
			}

			virtual void ParseEscapeSequence(CharBuffer & buffer) = 0;

		private:

			const char boundingChar_;
			const char escapeSequenceChar_;
	};
}

#endif
