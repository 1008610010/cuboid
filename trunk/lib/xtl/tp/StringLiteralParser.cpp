#include "StringLiteralParser.hpp"

namespace XTL
{
	const std::string StringLiteralParser::Parse()
	{
		// Assert( NeedChar() == boundingChar_ )

		Mark(); // Save begin position of string literal

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
			else if (c == '\n' && !multiline_)
			{
				Unmark();
				Unmark();
				ThrowError("Multiline string literal");
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
		Unmark();

		Advance();

		return result.ToString();
	}
}
