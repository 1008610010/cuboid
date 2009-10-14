#include "JsonParser.hpp"
#include "StringUtils.hpp"
#include <string>
#include <memory>

namespace XTL
{
	JsonParser::JsonParser(const char * source)
		: Parser(source),
		  root_(0)
	{
		Parse();
	}

	JsonParser::~JsonParser() throw()
	{
		delete root_;
	}

	JsonValue * JsonParser::Release()
	{
		JsonValue * temp = root_;
		root_ = 0;
		return temp;
	}

	void JsonParser::SkipSpaces()
	{
		while (true)
		{
			if (IsSpace(*cur_))
			{
				Advance();
			}
			else if (IsNewLine(*cur_))
			{
				AdvanceNewLine();
			}
			else
			{
				return;
			}
		}
	}

	JsonValue * JsonParser::ReadValue(JsonValue * parent)
	{
		if (*cur_ == '"')
		{
			return ReadString(parent);
		}
		else if (IsDigit(*cur_) || *cur_ == '-')
		{
			return ReadNumber(parent);
		}
		else if (IsAlpha(*cur_))
		{
			std::string keyword;
			ParseIdentifier(keyword);

			if (keyword == "null")
			{
				return new JsonNullValue(parent);
			}
			if (keyword == "true")
			{
				return new JsonBooleanValue(parent, true);
			}
			else if (keyword == "false")
			{
				return new JsonBooleanValue(parent, false);
			}
			else
			{
				throw Error(
					row_,
					column_ - keyword.length(),
					FormatString("Invalid bareword \"%s\", while value expected", keyword.c_str())
				);
			}
		}
		else if (*cur_ == '[')
		{
			return ReadArray(parent);
		}
		else if (*cur_ == '{')
		{
			return ReadObject(parent);
		}
		else
		{
			RaiseError("Invalid symbol, while value expected");
			return 0;
		}
	}

	/*
		IsDigit(*cur_) || *cur_ == '-'
	*/
	JsonValue * JsonParser::ReadNumber(JsonValue * parent)
	{
		long long integer = 0;
		double real = 0.0;

		if (ParseNumberLiteral(integer, real))
		{
			return new JsonIntegerValue(parent, integer);
		}
		else
		{
			return new JsonFloatValue(parent, real);
		}
	}

	// *cur == '"'
	JsonValue * JsonParser::ReadString(JsonValue * parent)
	{
		std::string value;
		ParseStringLiteral(value);
		return new JsonStringValue(parent, value);
	}

	// *cur_ == '['
	JsonValue * JsonParser::ReadArray(JsonValue * parent)
	{
		std::auto_ptr<JsonArrayValue> array(new JsonArrayValue(parent));

		Advance();
		SkipSpaces();
		if (*cur_ == ']')
		{
			Advance();
			return array.release();
		}

		while (true)
		{
			array->PushBack(ReadValue(parent));

			SkipSpaces();

			if (*cur_ == ',')
			{
				Advance();
				SkipSpaces();
				continue;
			}
			else if (*cur_ == ']')
			{
				Advance();
				return array.release();
			}
			else
			{
				RaiseError("Comma or end of array expected");
			}
		}
	}

	// *cur_ == '{'
	JsonValue * JsonParser::ReadObject(JsonValue * parent)
	{
		std::auto_ptr<JsonObjectValue> object(new JsonObjectValue(parent));
		std::string key;

		Advance();
		SkipSpaces();
		if (*cur_ == '}')
		{
			Advance();
			return object.release();
		}

		while (true)
		{
			if (*cur_ == '"')
			{
				ParseStringLiteral(key);
			}
			else
			{
				RaiseError("Unexpected symbol, while waiting key of object");
			}

			SkipSpaces();
			if (*cur_ != ':')
			{
				RaiseError("Colon expected");
			}
			Advance();
			SkipSpaces();

			object->Set(key, ReadValue(parent));

			SkipSpaces();
			if (*cur_ == ',')
			{
				Advance();
				SkipSpaces();
			}
			else if (*cur_ == '}')
			{
				Advance();
				return object.release();
			}
			else
			{
				RaiseError("Comma or end of object expected");
			}
		}
	}

	void JsonParser::Parse()
	{
		SkipSpaces();

		std::auto_ptr<JsonValue> value(ReadValue(0));

		SkipSpaces();
		if (*cur_ != '\0')
		{
			RaiseError("Unexpected symbol(s) at the end of file");
		}

		root_ = value.release();
	}
}
