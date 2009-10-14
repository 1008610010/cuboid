#include "Json.hpp"
#include <string.h>
#include <memory>

namespace XTL
{
	const char * JsonValue::TypeNames[] = {
		"Undefined",
		"Null",
		"Integer",
		"Float",
		"String",
		"Boolean",
		"Array",
		"Object"
	};

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
			::fprintf(stream, "%s", space);
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
		for (ValuesList::iterator itr = values_.begin(); itr != values_.end(); ++itr)
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
		ValuesList::iterator last = values_.end();
		--last;
		for (ValuesList::iterator itr = values_.begin(); itr != values_.end(); ++itr)
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
		ValuesList::iterator last = values_.end();
		--last;
		for (ValuesList::iterator itr = values_.begin(); itr != values_.end(); ++itr)
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

	JsonValue *& JsonArrayValue::Front()
	{
		return values_.front();
	}

	JsonValue *& JsonArrayValue::Back()
	{
		return values_.back();
	}

	JsonValue *& JsonArrayValue::PushFront(JsonValue * value)
	{
		values_.push_back(value);
		return Front();
	}

	JsonValue *& JsonArrayValue::PushBack(JsonValue * value)
	{
		values_.push_back(value);
		return Back();
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

	JsonValue *& JsonObjectValue::Set(const std::string & key, JsonValue * value)
	{
		IndexMap::iterator itr = index_.find(key);
		if (itr != index_.end())
		{
			if (itr->second->second != value)
			{
				delete itr->second->second;
				itr->second->second = value;
			}
			return itr->second->second;
		}
		else
		{
			values_.push_back(std::make_pair(key, value));
			ValuesList::iterator last = values_.end();
			--last;
			index_[key] = last;
			return last->second;
		}
	}
}
