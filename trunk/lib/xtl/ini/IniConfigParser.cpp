#include "IniConfigParser.hpp"

#include "../VariantScalar.hpp"
#include "../tp/NumberLiteralParser.hpp"
#include "../tp/StringLiteralParser.hpp"

namespace XTL
{
	IniConfigParser::IniConfigParser(CharSource & charSource, IniConfig & config)
		: Parser(charSource),
		  config_(config),
		  currentSection_()
	{
		while (NotAtEnd())
		{
			ParseLine();
		}
	}

	void IniConfigParser::ParseLine()
	{
		SkipLinearSpaces();
		if (AtEnd())
		{
			return;
		}

		char c = GetChar();
		if (c == '[')
		{
			ReadSection();
		}
		else if (CharClass::IDENTIFIER_HEAD.Contains(c))
		{
			ReadKeyValue();
		}
		/*
		else if (c == '{')
		{
			ReadCommand();
		}
		*/

		SkipLinearSpaces();
		if (AtEnd())
		{
			return;
		}

		if (GetChar() == ';' || GetChar() == '#')
		{
			SkipComments();
		}

		SkipNewLine();
	}

	void IniConfigParser::ReadSection()
	{
		// Assert( NeedChar() == '[' )

		try
		{
			Advance();
			SkipLinearSpaces();
			if (!CharClass::IDENTIFIER_HEAD.Contains(NeedChar()))
			{
				ThrowError("Section name was expected");
			}
			currentSection_ == ReadIdentifier();
			SkipLinearSpaces();
			if (NeedChar() != ']')
			{
				ThrowError("Closing square bracket was expected");
			}
			Advance();
		}
		catch (const EndOfFile &)
		{
			ThrowError("Unexpected end of file");
		}
	}

	void IniConfigParser::ReadKeyValue()
	{
		// Assert( CharClass.IDENTIFIER_HEAD.Contains(NeedChar()) )

		const std::string key = ReadIdentifier();

		bool wasDivider = false;
		if (NotAtEnd() && InClass(CharClass::LINEAR_SPACE))
		{
			wasDivider = true;
			Advance();
			SkipLinearSpaces();
		}

		if (NotAtEnd())
		{
			char c = GetChar();
			if (c == ':' || c == '=')
			{
				wasDivider = true;
				Advance();
				SkipLinearSpaces();
			}
		}

		if (!wasDivider)
		{
			ThrowError("Key-value divider was expected (':', '=' or SPACE)");
		}

		if (AtEnd())
		{
			ThrowError("Value expected");
		}

		config_.Set(currentSection_, key, ReadValue());
	}

	namespace
	{
		class SingleQuotedStringParser : public XTL::StringLiteralParser
		{
			public:

				explicit SingleQuotedStringParser(XTL::CharSource & charSource)
					: StringLiteralParser(charSource, '\'', '\\', false)
				{
					;;
				}

				virtual ~SingleQuotedStringParser() throw() { ;; }

			protected:

				class ES : public StringLiteralParser::EscapeSequenceSet<ES>
				{
					public:

						ES()
							: XTL::StringLiteralParser::EscapeSequenceSet<ES>()
						{
							Add('\'', '\'');
							Add('\\', '\\');
						}
				};

				virtual void ParseEscapeSequence(XTL::CharBuffer & result)
				{
					ES::Instance().Parse(GetCharSource(), result);
				}
		};

		class DoubleQuotedStringParser : public XTL::StringLiteralParser
		{
			public:

				explicit DoubleQuotedStringParser(XTL::CharSource & charSource)
					: XTL::StringLiteralParser(charSource, '"', '\\', false)
				{
					;;
				}

				virtual ~DoubleQuotedStringParser() throw() { ;; }

			protected:

				class ES : public XTL::StringLiteralParser::EscapeSequenceSet<ES>
				{
					public:

						ES()
							: XTL::StringLiteralParser::EscapeSequenceSet<ES>()
						{
							Add('"', '"');
							Add('\\', '\\');
							Add('r', '\r');
							Add('n', '\n');
							Add('t', '\t');
						}
				};

				virtual void ParseEscapeSequence(XTL::CharBuffer & result)
				{
					ES::Instance().Parse(GetCharSource(), result);
				}
		};
	}

	/*
	 * " ( [^\\"] | '\\' [rnt\\"]  )* "
	 * ' ( [^\\'] | '\\' [\\'] )* '
	 * -? [0-9]+ ( '.' [0-9]+ )? ( [eE] [+-] [0-9]+ )
	 * '0' ( ('x' [0-9A-Fa-f]+) | ('b' [0-1]+) | ([0-7]*) )
	 */
	VariantPtr IniConfigParser::ReadValue()
	{
		// Assert( NotAtEnd() && NotInClass(CharClass::LINEAR_SPACE) )

		char c = GetChar();

		if (c == '\'')
		{
			return VariantPtr(new Variant::String(SingleQuotedStringParser(GetCharSource()).Parse()));
		}
		else if (c == '"')
		{
			return VariantPtr(new Variant::String(DoubleQuotedStringParser(GetCharSource()).Parse()));
		}
		else if (CharClass::NUMBER_HEAD.Contains(c))
		{
			const Number n = FloatLiteralParser(GetCharSource()).Parse();

			if (n.IsFloat())
			{
				return VariantPtr(new Variant::Double(n.ToFloat()));
			}
			else
			{
				return VariantPtr(new Variant::LongLongInt(n.ToSignedInteger()));
			}
		}
		else
		{
			ThrowError("Value was expected");
		}
		/*
		else if (CharClass::IDENTIFIER_HEAD.Contains(c))
		{
		}
		*/
	}

	void IniConfigParser::SkipComments()
	{
		// Assert( NeedChar() == ';' || NeedChar() == '#' )

		do
		{
			Advance();
		}
		while (NotAtEnd() && GetChar() != '\n');
	}

	void IniConfigParser::SkipNewLine()
	{
		if (AtEnd())
		{
			return;
		}

		if (GetChar() == '\r')
		{
			Advance();
			if (AtEnd())
			{
				return;
			}
		}

		if (GetChar() == '\n')
		{
			Advance();
		}
		else
		{
			ThrowError("New line expected");
		}
	}
}
