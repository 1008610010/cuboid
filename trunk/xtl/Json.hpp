#ifndef _XTL__JSON_HPP__
#define _XTL__JSON_HPP__ 1

#include <string>
#include <map>
#include <list>
#include <vector>

namespace XTL
{
	class JsonParseError
	{
		public:

			JsonParseError(int row, int column, const char * what)
				: row_(row), column_(column), what_(what) { ;; }

			int Row() const            { return row_; }

			int Column() const         { return column_; }

			const char * What() const  { return what_.c_str(); }

		protected:

			int         row_;
			int         column_;
			std::string what_;
	};

	class JsonValue
	{
		public:

			enum Type
			{
				NIL      = 0,
				INTEGER  = 1,
				FLOAT    = 2,
				STRING   = 3,
				BOOLEAN  = 4,
				ARRAY    = 5,
				OBJECT   = 6
			};

			JsonValue() { ;; }

 			virtual ~JsonValue() throw() { ;; }

			virtual int Type() const = 0;

			virtual void PrintPlain(FILE * stream) = 0;

			virtual void Print(FILE * stream, int indent, bool indentFirst);

			void PrintIndent(FILE * stream, int size);

		protected:

			JsonValue(const JsonValue &);
			JsonValue & operator= (const JsonValue &);
	};

	class JsonNullValue : public JsonValue
	{
		public:

			virtual ~JsonNullValue() throw() { ;; }

			virtual int Type() const  { return NIL; }

			virtual void PrintPlain(FILE * stream);
	};

	class JsonIntegerValue : public JsonValue
	{
		public:

			explicit JsonIntegerValue(const long long & value)
				: JsonValue(), value_(value) { ;; }

			virtual ~JsonIntegerValue() throw() { ;; }

			virtual int Type() const  { return INTEGER; }

			virtual void PrintPlain(FILE * stream);

		protected:

			long long value_;
	};

	class JsonFloatValue : public JsonValue
	{
		public:

			explicit JsonFloatValue(const double & value)
				: JsonValue(), value_(value) { ;; }

			virtual ~JsonFloatValue() throw() { ;; }

			virtual int Type() const  { return FLOAT; }

			virtual void PrintPlain(FILE * stream);

		protected:

			double value_;
	};

	class JsonStringValue : public JsonValue
	{
		public:

			explicit JsonStringValue(const std::string & value)
				: JsonValue(), value_(value) { ;; }

			virtual ~JsonStringValue() throw() { ;; }

			virtual int Type() const  { return STRING; }

			virtual void PrintPlain(FILE * stream);

		protected:

			std::string value_;
	};

	class JsonBooleanValue : public JsonValue
	{
		public:

			explicit JsonBooleanValue(bool value)
				: JsonValue(), value_(value) { ;; }

			virtual ~JsonBooleanValue() throw() { ;; }

			virtual int Type() const  { return BOOLEAN; }

			virtual void PrintPlain(FILE * stream);

		protected:

			bool value_;
	};

	class JsonArrayValue : public JsonValue
	{
		typedef std::vector<JsonValue *> ValuesVector;

		public:

			JsonArrayValue()
				: JsonValue(), values_() { ;; }

			virtual ~JsonArrayValue() throw();

			virtual int Type() const  { return ARRAY; }

			virtual void PrintPlain(FILE * stream);

			virtual void Print(FILE * stream, int indent, bool indentFirst);

			bool Empty() const;

			void Add(JsonValue * value);

		protected:

			void Free();

			ValuesVector values_;
	};

	class JsonObjectValue : public JsonValue
	{
		typedef std::list<std::pair<std::string, JsonValue *> > ValuesList;
		typedef std::map<std::string, ValuesList::iterator> IndexMap;

		public:

			JsonObjectValue()
				: JsonValue(), values_(), index_() { ;; }

			virtual ~JsonObjectValue() throw();

			virtual int Type() const  { return OBJECT; }

			virtual void PrintPlain(FILE * stream);

			virtual void Print(FILE * stream, int indent, bool indentFirst);

			bool Empty() const;

			JsonValue ** Set(const std::string & key, JsonValue * value);

		protected:

			void Free();

			ValuesList values_;
			IndexMap   index_;
	};

	class JsonParser
	{
		public:

			JsonParser(const char * source);

			~JsonParser() throw();

			JsonValue * Release();

		protected:

			bool IsSpace(char c)    { return c == ' ' || c == '\t' || c == '\r'; }

			bool IsNewLine(char c)  { return c == '\n'; }

			bool IsDigit(char c)    { return c >= '0' && c <= '9'; }

			bool IsAlpha(char c)    { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'; }

			void Advance()          { ++cur_; ++column_; }

			void AdvanceNewLine()   { ++cur_; ++row_; column_ = 0; }

			void SkipSpaces();

			void ReadKeyword(std::string & s);

			void ReadString(std::string & s);

			JsonValue * ReadNumber();

			JsonValue * ReadString();

			JsonValue * ReadValue();

			JsonValue * ReadArray();

			JsonValue * ReadObject();

			void Parse();

			JsonValue * root_;
			int row_;
			int column_;
			const char * cur_;
	};
}

#endif
