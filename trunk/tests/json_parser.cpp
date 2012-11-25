#include <stdio.h>

#include <xtl/tp/CharSource.hpp>
#include <xtl/tp/Parser.hpp>

#include <xtl/Variant.hpp>
#include <xtl/VariantPtr.hpp>
#include <xtl/VariantScalar.hpp>
#include <xtl/VariantArray.hpp>
#include <xtl/VariantStruct.hpp>

namespace XTL
{
	bool operator<= (char c, const CharClass & charClass)
	{
		return charClass.Contains(c);
	}


	class JsonParser : public Parser
	{
		public:

			JsonParser(CharSource & charSource)
				: Parser(charSource)
			{
				Parse();
			}

		private:

			void Parse()
			{
				SkipSpaces();

				ReadValue();
			}

			void SkipSpaces()
			{
				while (NotAtEnd() && GetChar() <= CharClass::WHITESPACE)
				{
					Advance();
				}
			}

			static VariantPtr KeywordToValue(const std::string & keyword)
			{
				if (keyword == "null")
				{
					return Variant::Null();
				}
				else if (keyword == "true")
				{
					return Variant::Boolean::True();
				}
				else if (keyword == "false")
				{
					return Variant::Boolean::False();
				}
				else
				{
					/*
					throw Error(
						row_, column_ - keyword.length(),
						FormatString("Invalid bareword \"%s\", while value expected", keyword.c_str())
					);
					*/
					return Variant::Null();
				}
			}

			VariantPtr ReadValue()
			{
				char c = NeedChar();

				if (c == '"')
				{
					return ReadStringDoubleQuoted();
				}
				else if (c <= CharClass::DECIMAL || c == '-')
				{
					return ReadNumber();
				}
				else if (c <= CharClass::IDENTIFIER_HEAD)
				{
					std::string keyword = ReadIdentifier();

					// TODO: check config CASE_INSENSITIVE_KEYWORDS

					return KeywordToValue(keyword);
				}
				else if (c == '[')
				{
					return ReadArray();
				}
				else if (c == '{')
				{
					return ReadObject();
				}
				else
				{
					// TODO: throw Error("Invalid symbol, while value expected");
					return Variant::Null();
				}
			}

			VariantPtr ReadStringDoubleQuoted()
			{
				return VariantPtr(new Variant::String(""));
			}

			VariantPtr ReadNumber()
			{
				return VariantPtr(new Variant::LongLongInt(0));
			}

			VariantPtr ReadArray()
			{
				std::auto_ptr<Variant::Array> result(new VariantVector());

				return VariantPtr(result.release());
			}

			VariantPtr ReadObject()
			{
				std::auto_ptr<Variant::Struct> result(new VariantMap());

				return VariantPtr(result.release());
			}
	};
}

int main(int argc, char * argv[])
{
	const std::string jsonSource("[ [ ] ]");
	XTL::CharSource::ConstCharPtr charSource(jsonSource.data(), jsonSource.size());
	XTL::JsonParser jsonParser(charSource);
	printf("Ok!\n");
}
