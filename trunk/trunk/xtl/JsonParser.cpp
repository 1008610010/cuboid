#include "JsonParser.hpp"
#include "StringUtils.hpp"
#include <string>
#include <memory>

namespace XTL
{
	JsonParser::JsonParser(const char * source)
		: root_(0),
		  row_(0),
		  column_(0),
		  cur_(source)
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

	// IsAlpha(*cur)
	void JsonParser::ReadKeyword(std::string & s)
	{
		const char * begin = cur_;
		Advance();
		for (; IsAlpha(*cur_) || IsDigit(*cur_); Advance()) { ;; }
		s.assign(begin, cur_ - begin);
	}

	/*
		*cur_ == '"'
	*/
	void JsonParser::ReadString(std::string & s)
	{
		Advance();
		const char * begin = cur_;
		s.clear();

		while (true)
		{
			if (*cur_ == '\0')
			{
				throw Error(row_, column_, "Unexpected end of file in double quoted string");
			}
			else if (*cur_ == '\n')
			{
				throw Error(row_, column_, "Multiline string");
			}
			else if (*cur_ == '"')
			{
				if (begin < cur_)
				{
					s.append(begin, cur_ - begin);
					begin = cur_;
				}
				Advance();
				return;
			}
			else if (*cur_ == '\\')
			{
				if (begin < cur_)
				{
					s.append(begin, cur_ - begin);
				}
				Advance();
				switch (*cur_)
				{
					case '\0':
						throw Error(row_, column_, "Unexpected end of file in double quoted string");
					case '\\':
					case '/':
					case '"':
						s.append(1, *cur_); Advance(); begin = cur_; break;
					case 'b':
						s.append(1, '\b'); Advance(); begin = cur_; break;
					case 'f':
						s.append(1, '\f'); Advance(); begin = cur_; break;
					case 'n':
						s.append(1, '\n'); Advance(); begin = cur_; break;
					case 'r':
						s.append(1, '\r'); Advance(); begin = cur_; break;
					case 't':
						s.append(1, '\t'); Advance(); begin = cur_; break;
					default:
						// TODO: \uXXXX - char in hexademical digit

						throw Error(
							row_,
							column_,
							FormatString("Unknown escape sequence: \\%c", *cur_).c_str()
						);
				}
			}
			else
			{
				Advance();
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
			ReadKeyword(keyword);

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
					FormatString("Invalid bareword \"%s\", while value expected", keyword.c_str()).c_str()
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
			throw Error(row_, column_, "Invalid symbol, while value expected");
		}
	}

	/*
		IsDigit(*cur_) || *cur_ == '-'
	*/
	JsonValue * JsonParser::ReadNumber(JsonValue * parent)
	{
		long long integer = 0;
		double real = 0.0;
		bool isNegative = false;
		bool isInteger = true;

		if (*cur_ == '-')
		{
			isNegative = true;
			Advance();
			SkipSpaces();
			if (!IsDigit(*cur_))
			{
				throw Error(row_, column_, "Decimal digit expected");
			}
		}

		for (; IsDigit(*cur_); Advance())
		{
			integer = 10 * integer + (*cur_ - '0');
		}

		if (*cur_ == '.')
		{
			Advance();

			isInteger = false;
			real = integer;

			double divisor = 1.0;
			for (; IsDigit(*cur_); Advance())
			{
				divisor /= 10.0;
				real += (*cur_ - '0') * divisor;
			}
		}

		if (*cur_ == 'e' || *cur_ == 'E')
		{
			Advance();

			bool isExponentPositive = true;
			if (*cur_ == '+' || *cur_ == '-')
			{
				isExponentPositive = (*cur_ == '+');
				Advance();
			}

			if (!IsDigit(*cur_))
			{
				throw Error(row_, column_, "Exponent expected");
			}

			long long exponent = 0;
			for (; IsDigit(*cur_); Advance())
			{
				exponent = 10 * exponent + (*cur_ - '0');
			}
/*
			if (isInteger)
			{
				isInteger = false;
				real = integer;
			}
*/

			if (isInteger && isExponentPositive)
			{
				for (; exponent > 0; --exponent, integer *= 10) { ;; }
			}
			else
			{
				if (isInteger)
				{
					isInteger = false;
					real = integer;
				}

				if (isExponentPositive)
				{
					for (; exponent > 0; --exponent, real *= 10.0) { ;; }
				}
				else
				{
					for (; exponent > 0; --exponent, real /= 10.0) { ;; }
				}
			}
		}

		if (isInteger)
		{
			return new JsonIntegerValue(parent, isNegative ? -integer : integer);
		}
		else
		{
			return new JsonFloatValue(parent, isNegative ? -real : real);
		}
	}

	JsonValue * JsonParser::ReadString(JsonValue * parent)
	{
		std::string value;
		ReadString(value);
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
			array->Add(ReadValue(parent));

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
				throw Error(row_, column_, "Comma or end of array expected");
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
				ReadString(key);
			}
			else
			{
				throw Error(row_, column_, "Unexpected symbol, while waiting key of object");
			}

			SkipSpaces();
			if (*cur_ != ':')
			{
				throw Error(row_, column_, "Colon expected");
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
				throw Error(row_, column_, "Comma or end of object expected");
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
			throw Error(row_, column_, "Unexpected symbol(s) at the end of file");
		}

		root_ = value.release();
	}
}
