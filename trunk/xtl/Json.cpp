#include "Json.hpp"
#include <string.h>
#include <memory>

namespace XTL
{
	const std::string EscapeString(const std::string & s)
	{
		std::string es;
		es.reserve(s.length());

		const char * cur = s.c_str();
		const char * begin = cur;

		while (*cur != '\0')
		{
			switch (*cur)
			{
				case '"':
				case '\\':
				case '/':
					if (begin < cur) es.append(begin, cur - begin);
					es.append(1, '\\');
					es.append(1, *cur);
					++cur;
					begin = cur;
					break;
				case '\b':
					if (begin < cur) es.append(begin, cur - begin);
					es.append("\\b");
					++cur;
					begin = cur;
					break;
				case '\f':
					if (begin < cur) es.append(begin, cur - begin);
					es.append("\\f");
					++cur;
					begin = cur;
					break;
				case '\n':
					if (begin < cur) es.append(begin, cur - begin);
					es.append("\\n");
					++cur;
					begin = cur;
					break;
				case '\r':
					if (begin < cur) es.append(begin, cur - begin);
					es.append("\\r");
					++cur;
					begin = cur;
					break;
				case '\t':
					if (begin < cur) es.append(begin, cur - begin);
					es.append("\\t");
					++cur;
					begin = cur;
					break;
				default:
					++cur;
			}
		}

		if (begin < cur) es.append(begin, cur - begin);

		return es;
	}

	void JsonValue::Print(FILE * stream, int indent, bool indentFirst)
	{
		if (indentFirst)
		{
			PrintIndent(stream, indent);
		}
		PrintPlain(stream);
	}

	void JsonValue::PrintIndent(FILE * stream, int size)
	{
		static const char * space = "    ";
		for (; size > 0; --size)
		{
			::fprintf(stream, space);
		}
	}

	void JsonNullValue::PrintPlain(FILE * stream)
	{
		::fprintf(stream, "null");
	}

	void JsonIntegerValue::PrintPlain(FILE * stream)
	{
		::fprintf(stream, "%lld", value_);
	}

	void JsonFloatValue::PrintPlain(FILE * stream)
	{
		::fprintf(stream, "%.16g", value_);
	}

	void JsonStringValue::PrintPlain(FILE * stream)
	{
		// TODO: escape some characters
		::fprintf(stream, "\"%s\"", EscapeString(value_).c_str());
	}

	void JsonBooleanValue::PrintPlain(FILE * stream)
	{
		::fprintf(stream, value_ ? "true" : "false");
	}

	JsonArrayValue::~JsonArrayValue() throw()
	{
		Free();
	}

	void JsonArrayValue::Free()
	{
		for (ValuesVector::iterator itr = values_.begin(); itr != values_.end(); ++itr)
		{
			delete *itr;
		}
	}

	void JsonArrayValue::PrintPlain(FILE * stream)
	{
		if (Empty())
		{
			::fprintf(stream, "[]");
			return;
		}
		::fprintf(stream, "[ ");
		ValuesVector::iterator last = values_.end() - 1;
		for (ValuesVector::iterator itr = values_.begin(); itr != values_.end(); ++itr)
		{
			(*itr)->PrintPlain(stream);
			if (itr != last)
			{
				::fprintf(stream, ", ");
			}
		}
		::fprintf(stream, " ]");
	}

	void JsonArrayValue::Print(FILE * stream, int indent, bool indentFirst)
	{
		if (indentFirst)
		{
			PrintIndent(stream, indent);
		}
		::fprintf(stream, "[\n");
		ValuesVector::iterator last = values_.end() - 1;
		for (ValuesVector::iterator itr = values_.begin(); itr != values_.end(); ++itr)
		{
			if ((*itr) == 0)
			{
				::fprintf(stream, "null");
			}
			else
			{
				(*itr)->Print(stream, indent + 1, true);
			}
			if (itr != last)
			{
				::fprintf(stream, ",");
			}
			::fprintf(stream, "\n");
		}
		PrintIndent(stream, indent);
		::fprintf(stream, "]");
	}

	bool JsonArrayValue::Empty() const
	{
		return values_.empty();
	}

	void JsonArrayValue::Add(JsonValue * value)
	{
		values_.push_back(value);
	}

	JsonObjectValue::~JsonObjectValue() throw()
	{
		Free();
	}

	void JsonObjectValue::Free()
	{
		for (ValuesList::iterator itr = values_.begin(); itr != values_.end(); ++itr)
		{
			delete itr->second;
		}
	}

	void JsonObjectValue::PrintPlain(FILE * stream)
	{
		if (Empty())
		{
			::fprintf(stream, "{}");
			return;
		}
		::fprintf(stream, "{ ");
		ValuesList::iterator last = values_.end();
		--last;
		for (ValuesList::iterator itr = values_.begin(); itr != values_.end(); ++itr)
		{
			::fprintf(stream, "\"%s\" : ", EscapeString(itr->first).c_str());
			itr->second->PrintPlain(stream);
			if (itr != last)
			{
				::fprintf(stream, ", ");
			}
		}
		::fprintf(stream, " }");
	}

	void JsonObjectValue::Print(FILE * stream, int indent, bool indentFirst)
	{
		if (indentFirst)
		{
			PrintIndent(stream, indent);
		}
		::fprintf(stream, "{\n");
		ValuesList::iterator last = values_.end();
		--last;
		for (ValuesList::iterator itr = values_.begin(); itr != values_.end(); ++itr)
		{
			PrintIndent(stream, indent + 1);
			::fprintf(stream, "\"%s\" : ", EscapeString(itr->first).c_str());
			if (itr->second == 0)
			{
				::fprintf(stream, "null");
			}
			else
			{
				itr->second->Print(stream, indent + 1, false);
			}
			if (itr != last)
			{
				::fprintf(stream, ",");
			}
			::fprintf(stream, "\n");
		}
		PrintIndent(stream, indent);
		::fprintf(stream, "}");
	}

	bool JsonObjectValue::Empty() const
	{
		return values_.empty();
	}

	JsonValue ** JsonObjectValue::Set(const std::string & key, JsonValue * value)
	{
		IndexMap::iterator itr = index_.find(key);
		if (itr != index_.end())
		{
			if (itr->second->second != value)
			{
				delete itr->second->second;
				itr->second->second = value;
			}
			return &(itr->second->second);
		}
		else
		{
			values_.push_back(std::make_pair(key, value));
			ValuesList::iterator last = values_.end();
			--last;
			index_[key] = last;
			return &(last->second);
		}
	}

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
		for (; IsAlpha(*cur_); Advance()) { ;; }
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
				throw JsonParseError(row_, column_, "Unexpected end of file in double quoted string");
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
						throw JsonParseError(row_, column_, "Unexpected end of file in double quoted string");
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
						// TODO: print sequence in JsonParseError
						throw JsonParseError(row_, column_, "Unknown escape sequence");
				}
			}
			else
			{
				Advance();
			}
		}
	}

	/*
		IsDigit(*cur_) || *cur_ == '-'
	*/
	JsonValue * JsonParser::ReadNumber()
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
				throw JsonParseError(row_, column_, "Decimal digit expected");
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
				throw JsonParseError(row_, column_, "Exponent expected");
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
			return new JsonIntegerValue(isNegative ? -integer : integer);
		}
		else
		{
			return new JsonFloatValue(isNegative ? -real : real);
		}
	}

	JsonValue * JsonParser::ReadString()
	{
		std::string value;
		ReadString(value);
		return new JsonStringValue(value);
	}

	JsonValue * JsonParser::ReadValue()
	{
		if (*cur_ == '"')
		{
			return ReadString();
		}
		else if (IsDigit(*cur_) || *cur_ == '-')
		{
			return ReadNumber();
		}
		else if (IsAlpha(*cur_))
		{
			std::string keyword;
			ReadKeyword(keyword);

			if (keyword == "null")
			{
				return new JsonNullValue();
			}
			if (keyword == "true")
			{
				return new JsonBooleanValue(true);
			}
			else if (keyword == "false")
			{
				return new JsonBooleanValue(false);
			}
			else
			{
				throw JsonParseError(row_, column_, "Invalid keyword, while value expected");
			}
		}
		else if (*cur_ == '[')
		{
			return ReadArray();
		}
		else if (*cur_ == '{')
		{
			return ReadObject();
		}
		else
		{
			throw JsonParseError(row_, column_, "Invalid symbol, while value expected");
		}
	}

	// *cur_ == '['
	JsonValue * JsonParser::ReadArray()
	{
		std::auto_ptr<JsonArrayValue> array(new JsonArrayValue());

		Advance();
		SkipSpaces();
		if (*cur_ == ']')
		{
			Advance();
			return array.release();
		}

		while (true)
		{
			array->Add(ReadValue());

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
				throw JsonParseError(row_, column_, "Comma or end of array expected");
			}
		}
	}

	// *cur_ == '{'
	JsonValue * JsonParser::ReadObject()
	{
		std::auto_ptr<JsonObjectValue> object(new JsonObjectValue());
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
				throw JsonParseError(row_, column_, "Unexpected symbol, while waiting key of object");
			}

			SkipSpaces();
			if (*cur_ != ':')
			{
				throw JsonParseError(row_, column_, "Colon expected");
			}
			Advance();
			SkipSpaces();

			object->Set(key, ReadValue());

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
				throw JsonParseError(row_, column_, "Comma or end of object expected");
			}
		}
	}

	void JsonParser::Parse()
	{
		SkipSpaces();
		if (*cur_ == '[')
		{
			root_ = ReadArray();
		}
		else if (*cur_ == '{')
		{
			root_ = ReadObject();
		}
		else
		{
			throw JsonParseError(row_, column_, "Array or object expected at the root level");
		}

		SkipSpaces();
		if (*cur_ != '\0')
		{
			throw JsonParseError(row_, column_, "Unexpected symbol(s) at the end of file");
		}
	}

}
