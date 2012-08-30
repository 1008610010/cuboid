#ifndef XTL__STRING_LITERAL_PARSER_HPP__
#define XTL__STRING_LITERAL_PARSER_HPP__ 1

#include "../CharBuffer.hpp"
#include "Parser.hpp"

namespace XTL
{
	class StringLiteralParser : protected Parser
	{
		public:

			StringLiteralParser(char boundingChar, char escapeSequenceChar)
				: boundingChar_(boundingChar),
				  escapeSequenceChar_(escapeSequenceChar)
			{
				;;
			}

			virtual ~StringLiteralParser() throw()
			{
				;;
			}

			const std::string Parse() const
			{
				// Assert( NeedChar() == boundingChar_ )

				CharBuffer result;

				Advance();
				Mark();

				char c;
				while ((c = NeedChar()) != boundingChar_)
				{
					if (c == escapeSequenceChar_)
					{
						ReleaseString(result);

						Advance();

						ParseEscapeSequence
					}
					else
					{
						Advance();
					}
				}

				ReleaseString(result);
				Advance();

				return result;
			}

					if (charSource.AtEnd())
					{
						throw std::runtime_error("Error");
					}
					else if (c == escapeSequenceChar_)
					{

						charSource.Advance();

						if (GetEscapeSequenceParser().Parse(charSource, result))
						{
							charSource.Mark();
						}
						else
						{
							throw std::runtime_error(XTL::FormatString("Invalid escape sequence in string literal: %c%c", escapeSequenceChar_, charSource.GetChar()));
						}
					}
					else
					{
						charSource.Advance();
					}
				}

				result.append(charSource.ReleaseString());
				charSource.Unmark();

				return result;
			}

		protected:

			virtual const EscapeSequenceParser & GetEscapeSequenceParser() const = 0;

		private:

			const char boundingChar_;
			const char escapeSequenceChar_;
	};
}

#endif
