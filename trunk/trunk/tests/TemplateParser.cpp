#include "TemplateParser.hpp"

#include "../lib/xtl/StringUtils.hpp"
#include "../lib/xtl/tp/CharClass.hpp"

#include "TemplateTreeBuilder.hpp"

namespace XTL
{
	TemplateParser::TemplateParser(CharSource & charSource, TemplateTreeBuilder & builder)
		: Parser(charSource),
		  builder_(builder)
	{
		;;
	}

	void TemplateParser::Parse()
	{
		Mark();

		while (NotAtEnd())
		{
			if (GetChar() != '{')
			{
				Advance();
				continue;
			}

			builder_.AddText(ReleaseString());

			Mark();
			Advance();

			if (!SkipChar('{'))
			{
				Advance();
				continue;
			}

			Unmark();

			SkipWhitespaces();

			if (GetChar() == '$') // {{$VAR}}
			{
				Advance();
				if (AtEnd() || NotInClass(CharClass::IDENTIFIER_HEAD))
				{
					ThrowError("Syntax error");
				}

				const std::string variableName = ReadIdentifier();

				if (!SkipString("}}"))
				{
					ThrowError("Syntax error");
				}

				Mark();
			}
			else
			{
				bool closing = false;
				if (SkipChar('/'))
				{
					closing = true;
				}

				std::string tag = NeedIdentifier();
				ToLowerCase(tag);

				if (tag == "if")
				{
					if (closing)
					{
						builder_.CloseIf();
					}
					else
					{
						// ... {{if ...
						//       ---^
						SkipWhitespaces();

						builder_.OpenIf(NeedIdentifier());
					}
				}
				else if (tag == "else")
				{
					if (closing)
					{
						ThrowError("Syntax error");
					}
					else
					{
						builder_.OpenElse();
					}
				}
				else if (tag == "elseif")
				{
					if (closing)
					{
						ThrowError("Syntax error");
					}
					else
					{
						SkipWhitespaces();

						builder_.OpenElseIf(NeedIdentifier());
					}
				}
				else if (tag == "loop")
				{
					if (closing)
					{
						builder_.CloseLoop();
					}
					else
					{
						SkipWhitespaces();

						builder_.OpenLoop(NeedIdentifier());
					}
				}
				else if (tag == "else")
				{
					printf("{{ else }}\n");
				}
				else if (tag == "include")
				{
					printf("{{ include }}");
				}
				else
				{
					ThrowError("Syntax error");
				}

				SkipWhitespaces();

				if (!SkipString("}}"))
				{
					ThrowError("Syntax error");
				}

				Mark();
			}
		}

		builder_.AddText(ReleaseString());
	}

	void TemplateParser::SkipWhitespaces()
	{
		while (NotAtEnd() && InClass(CharClass::WHITESPACE))
		{
			Advance();
		}
	}

	const std::string TemplateParser::NeedIdentifier()
	{
		if (AtEnd() || NotInClass(CharClass::IDENTIFIER_HEAD))
		{
			ThrowError("Identifier was expected");
		}

		return ReadIdentifier();
	}
}
