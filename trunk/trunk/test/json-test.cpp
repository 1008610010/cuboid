#include <stdio.h>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <memory>

class JsonParseError
{
	public:
	
		JsonParseError(int row, int column, const char * what)
			: row_(row), column_(column), what_(what)
		{
			;;
		}
		
		int Row() const
		{
			return row_;
		}
		
		int Column() const
		{
			return column_;
		}
		
		const char * What() const
		{
			return what_.c_str();
		}
	
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
			STRING   = 0,
			INTEGER  = 1,
			FLOAT    = 2,
			LIST     = 3,
			MAP      = 4
		};

 		virtual ~JsonValue() throw() { ;; }

		virtual int Type() const = 0;
};

class JsonString : public JsonValue
{
	public:
	
		JsonString(const std::string & value)
			: JsonValue(), value_(value) { ;; }
	
		virtual ~JsonString() throw()
		{
			;;
		}
		
		virtual int Type() const
		{
			return STRING;
		}
		
	protected:
	
		std::string value_;
};

class JsonInteger : public JsonValue
{
	public:
	
		JsonInteger(const long long & value)
			: JsonValue(), value_(value) { ;; }
	
		virtual ~JsonInteger() throw()
		{
			;;
		}
		
		virtual int Type() const
		{
			return INTEGER;
		}
		
	protected:
	
		long long value_;
};

class JsonFloat : public JsonValue
{
	public:
	
		JsonFloat(const double & value)
			: JsonValue(), value_(value) { ;; }
	
		virtual ~JsonFloat() throw()
		{
			;;
		}
		
		virtual int Type() const
		{
			return FLOAT;
		}
		
	protected:
	
		double value_;
};

class JsonContainer : public JsonValue
{
	public:
	
		JsonContainer(JsonContainer * parent)
			: parent_(parent) { ;; }
			
		JsonContainer * Parent() const
		{
			return parent_;
		}

	protected:
	
		JsonContainer * parent_;
};

class JsonList : public JsonContainer
{
	typedef std::vector<JsonValue *> ValuesVector;
	
	public:
	
		JsonList(JsonContainer * parent)
			: JsonContainer(parent), values_() { ;; }
			
		virtual ~JsonList() throw()
		{
			Free();
		}
			
		virtual int Type() const
		{
			return LIST;
		}
		
		JsonValue * Parent()
		{
			return parent_;
		}

		void Add(JsonValue * value)
		{
			values_.push_back(value);
		}

	protected:
	
		void Free()
		{
			for (ValuesVector::iterator itr = values_.begin(); itr != values_.end(); ++itr)
			{
				delete *itr;
			}
		}
	
		JsonValue    * parent_;
		ValuesVector   values_;
};

class JsonMap : public JsonContainer
{
	typedef std::list<JsonValue *> ValuesList;
	typedef std::map<std::string, ValuesList::iterator> IndexMap;

	public:

		JsonMap(JsonContainer * parent)
			: JsonContainer(parent), values_(), index_() { ;; }
			
		virtual ~JsonMap() throw()
		{
			Free();
		}
			
		virtual int Type() const
		{
			return MAP;
		}
		
		JsonValue * Parent()
		{
			return parent_;
		}
		
		void Set(const std::string & key, JsonValue * value)
		{
			IndexMap::iterator itr = index_.find(key);
			if (itr != index_.end())
			{
				if (*(itr->second) != value)
				{
					delete *(itr->second);
					*(itr->second) = value;
				}
			}
			else
			{
				values_.push_back(value);
				ValuesList::iterator last = values_.end();
				--last;
				index_[key] = last;
			}
		}
			
	protected:
	
		void Free()
		{
			for (ValuesList::iterator itr = values_.begin(); itr != values_.end(); ++itr)
			{
				delete *itr;
			}
		}
	
		JsonValue * parent_;
		ValuesList values_;
		IndexMap index_;
};

class JsonParser
{
	public:

		JsonParser(const char * source)
			: root_(0),
			  row_(0),
			  column_(0),
			  cur_(source)
		{
			Parse();
		}
		
		~JsonParser() throw()
		{
			delete root_;
		}
			
	protected:
	
		bool IsSpace(char c)
		{
			return c == ' ' || c == '\t' || c == '\r';
		}
		
		bool IsNewLine(char c)
		{
			return c == '\n';
		}
		
		bool IsDigit(char c)
		{
			return c >= '0' && c <= '9';
		}
		
		void SkipSpaces()
		{
			for (const char * p = cur_; ; ++p)
			{
				if (IsSpace(*p))
				{
					++column_;
				}
				else if (IsNewLine(*p))
				{
					++row_;
					column_ = 0;
				}
				else
				{
					// TODO: check (p - cur_) for space count here
					cur_ = p;
					return;
				}
			}
		}
		
		/*
			IsDigit(*cur_) || *cur_ == '-' || *cur_ == '+'
		*/
		JsonValue * ReadNumber()
		{
			bool isNegative = false;

			if (*cur_ == '-' || *cur_ == '+')
			{
				isNegative = (*cur_ == '-');
				++cur_;
				++column_;
				SkipSpaces();
				if (!IsDigit(*cur_))
				{
					throw JsonParseError(row_, column_, "Decimal digit expected");
				}
			}

			long long integer = 0;

			for (; IsDigit(*cur_); ++cur_, ++column_)
			{
				integer = 10 * integer + (*cur_ - '0');
			}
			
			if (*cur_ == '.')
			{
				++cur_;
				++column_;
				double real = integer;
				double divisor = 1.0;
				for (; IsDigit(*cur_); ++cur_, ++column_)
				{
					divisor /= 10.0;
					real += (*cur_ - '0') * divisor;
				}
				return new JsonFloat(isNegative ? -real : real);
			}
			else
			{
				return new JsonInteger(isNegative ? -integer : integer);
			}
			
			// TODO: check (*cur_ == 'E' || *cur_ == 'e') for 1.0e2
		}
		
		/*
			*cur_ == '"'
		*/
		void ReadString(std::string & s)
		{
			const char * begin = cur_;
			++cur_;
			++column_;
			
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
					++cur_;
					++column_;
					return;
				}
				else if (*cur_ == '\\')
				{
					if (begin < cur_)
					{
						s.append(begin, cur_ - begin);
					}
					++cur_;
					++column_;
					if (*cur_ == '\0')
					{
						throw JsonParseError(row_, column_, "Unexpected end of file in double quoted string");
					}
					else if (*cur_ == '\\')
					{
						++cur_;
						++column_;
						s.append(1, '\\');
						begin = cur_;
					}
					else if (*cur_ == '"')
					{
						++cur_;
						++column_;
						s.append(1, '"');
						begin = cur_;
					}
					else if (*cur_ == 'n')
					{
						++cur_;
						++column_;
						s.append(1, '\n');
						begin = cur_;
					}
					else
					{
						// TODO: print sequence in JsonParseError
						throw JsonParseError(row_, column_, "Unknown escape sequence");
					}
				}
				else if (*cur_ == '\n')
				{
					++cur_;
					++row_;
					column_ = 0;
				}
				else
				{
					++cur_;
					++column_;
				}
			}
		}
		
		JsonValue * ReadString()
		{
			std::string value;
			ReadString(value);
			return new JsonString(value);
		}
		
		// *cur_ == '['
		JsonValue * ReadList(JsonContainer * parent)
		{
			std::auto_ptr<JsonList> list(new JsonList(parent));

			++cur_;
			++column_;
			
			SkipSpaces();
			if (*cur_ == ']')
			{
				++cur_;
				++column_;
				return list.release();
			}

			while (true)
			{
				if (*cur_ == '"')
				{
					list->Add(ReadString());
				}
				else if (IsDigit(*cur_) || *cur_ == '+' || *cur_ == '-')
				{
					list->Add(ReadNumber());
				}
				else if (*cur_ == '[')
				{
					list->Add(ReadList(list.get()));
				}
				else if (*cur_ == '{')
				{
					list->Add(ReadMap(list.get()));
				}
				else
				{
					throw JsonParseError(row_, column_, "Invalid symbol, when value expected");
				}

				SkipSpaces();

				if (*cur_ == ',')
				{
					++cur_;
					++column_;
					SkipSpaces();
					continue;
				}
				else if (*cur_ == ']')
				{
					++cur_;
					++column_;
					return list.release();
				}
				else
				{
					throw JsonParseError(row_, column_, "Comma or end of list expected");
				}
			}
		}
	
		// *cur_ == '{'
		JsonValue * ReadMap(JsonContainer * parent)
		{
			std::auto_ptr<JsonMap> map(new JsonMap(parent));
			std::string key;
			JsonValue * value;

			++cur_;
			++column_;
			
			SkipSpaces();
			if (*cur_ == '}')
			{
				++cur_;
				++column_;
				return map.release();
			}

			while (true)
			{
				if (*cur_ == '"')
				{
					ReadString(key);
				}
				else
				{
					throw JsonParseError(row_, column_, "Unexpected symbol, while waiting key of map");
				}

				SkipSpaces();
				if (*cur_ != ':')
				{
					throw JsonParseError(row_, column_, "Colon expected");
				}
				++cur_;
				++column_;
				SkipSpaces();

				if (*cur_ == '"')
				{
					value = ReadString();
				}
				else if (IsDigit(*cur_) || *cur_ == '+' || *cur_ == '-')
				{
					value = ReadNumber();
				}
				else if (*cur_ == '{')
				{
					value = ReadMap(map.get());
				}
				else if (*cur_ == '[')
				{
					value = ReadList(map.get());
				}
				else
				{
					throw JsonParseError(row_, column_, "Invalid symbol, when value expected");
				}
						
				map->Set(key, value);

				SkipSpaces();
				if (*cur_ == ',')
				{
					++cur_;
					++column_;
					SkipSpaces();
					continue;
				}
				else if (*cur_ == '}')
				{
					++cur_;
					++column_;
					return map.release();
				}
				else
				{
					throw JsonParseError(row_, column_, "Comma or end of map expected");
				}
			}
		}
	
		void Parse()
		{
			SkipSpaces();
			if (*cur_ == '[')
			{
				root_ = ReadList(0);
			}
			else if (*cur_ == '{')
			{
				root_ = ReadMap(0);
			}
			else
			{
				throw JsonParseError(row_, column_, "List or map expected at the root level");
			}
		}
	
		JsonValue * root_;
		int row_;
		int column_;
		const char * cur_;
};

int main(int argc, const char * argv[])
{
	printf("JSON test.\n");
	
	const char * source =
	" { \n"
	"\"a\" : \"b\" , \"\\\\fucky\" : 12345.6789\n"
	" }";
	
	fprintf(stderr, "%s\n", source);
	
	try
	{
		JsonParser parser(source);
	}
	catch (const JsonParseError & error)
	{
		fprintf(stderr, "[ERROR] Line %d, col %d: %s\n", error.Row(), error.Column(), error.What());
	}
	
	return 0;
}

