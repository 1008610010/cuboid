#ifndef _XTL__JSON_HPP__
#define _XTL__JSON_HPP__ 1

#include <string>
#include <map>
#include <list>
#include <vector>
#include "StringUtils.hpp"

namespace XTL
{
	class JsonException
	{
		public:

			explicit JsonException(const char * what) : what_(what) { ;; }

			explicit JsonException(const std::string & what) : what_(what) { ;; }

			const char * What() const { return what_.c_str(); }

		protected:

			const std::string what_;
	};

	class JsonValue
	{
		public:

			enum Type
			{
				UNDEFINED = -1,
				NIL       =  0,
				INTEGER   =  1,
				FLOAT     =  2,
				STRING    =  3,
				BOOLEAN   =  4,
				ARRAY     =  5,
				OBJECT    =  6
			};

			static const char * TypeNames[];

			explicit JsonValue(JsonValue * parent) : parent_(parent) { ;; }

 			virtual ~JsonValue() throw() { ;; }

			JsonValue * Parent()  { return parent_; }

			virtual int Type() const = 0;

			const char * TypeName() const  { return TypeNames[Type()]; }

			virtual void PrintPlain(FILE * stream) = 0;

			virtual void Print(FILE * stream, int indent, bool indentFirst);

			void PrintIndent(FILE * stream, int size);

			virtual const long long   AsInteger() const = 0;
			virtual const double      AsFloat()   const = 0;
			virtual const std::string AsString()  const = 0;

		protected:

			JsonValue(const JsonValue &);
			JsonValue & operator= (const JsonValue &);

			JsonValue * parent_;
	};

	class JsonNullValue : public JsonValue
	{
		public:

			explicit JsonNullValue(JsonValue * parent) : JsonValue(parent) { ;; }

			virtual ~JsonNullValue() throw() { ;; }

			virtual int Type() const  { return NIL; }

			virtual void PrintPlain(FILE * stream);

			virtual const long long AsInteger() const
			{
				return 0ll;
			}

			virtual const double AsFloat() const
			{
				return 0.0;
			}

			virtual const std::string AsString() const
			{
				return std::string("");
			}
	};

	class JsonIntegerValue : public JsonValue
	{
		public:

			JsonIntegerValue(JsonValue * parent, const long long & value)
				: JsonValue(parent), value_(value) { ;; }

			virtual ~JsonIntegerValue() throw() { ;; }

			virtual int Type() const  { return INTEGER; }

			virtual void PrintPlain(FILE * stream);

			void Set(const long long & value)
			{
				value_ = value;
			}

			virtual const long long AsInteger() const
			{
				return value_;
			}

			virtual const double AsFloat() const
			{
				return IntegerToFloat(value_);
			}

			virtual const std::string AsString() const
			{
				return IntegerToString(value_);
			}

		protected:

			long long value_;
	};

	class JsonFloatValue : public JsonValue
	{
		public:

			JsonFloatValue(JsonValue * parent, const double & value)
				: JsonValue(parent), value_(value) { ;; }

			virtual ~JsonFloatValue() throw() { ;; }

			virtual int Type() const  { return FLOAT; }

			virtual void PrintPlain(FILE * stream);

			virtual const long long AsInteger() const
			{
				return FloatToInteger(value_);
			}

			virtual const double AsFloat() const
			{
				return value_;
			}

			virtual const std::string AsString() const
			{
				return FloatToString(value_);
			}

		protected:

			double value_;
	};

	class JsonStringValue : public JsonValue
	{
		public:

			JsonStringValue(JsonValue * parent, const std::string & value)
				: JsonValue(parent), value_(value) { ;; }

			virtual ~JsonStringValue() throw() { ;; }

			virtual int Type() const  { return STRING; }

			virtual void PrintPlain(FILE * stream);

			const std::string Get() const
			{
				return value_;
			}

			void Set(const std::string & value)
			{
				value_ = value;
			}

			void Set(const char * value)
			{
				value_.assign(value);
			}

			virtual const long long AsInteger() const
			{
				return StringToInteger(value_);
			}

			virtual const double AsFloat() const
			{
				return StringToFloat(value_);
			}

			virtual const std::string AsString() const
			{
				return value_;
			}

		protected:

			std::string value_;
	};

	class JsonBooleanValue : public JsonValue
	{
		public:

			JsonBooleanValue(JsonValue * parent, bool value)
				: JsonValue(parent), value_(value) { ;; }

			virtual ~JsonBooleanValue() throw() { ;; }

			virtual int Type() const  { return BOOLEAN; }

			virtual void PrintPlain(FILE * stream);

			virtual const long long AsInteger() const
			{
				return value_ ? 1 : 0;
			}

			virtual const double AsFloat() const
			{
				return value_ ? 1.0 : 0.0;
			}

			virtual const std::string AsString() const
			{
				return std::string(value_ ? "true" : "false");
			}

		protected:

			bool value_;
	};

	class JsonArrayValue : public JsonValue
	{
		typedef std::list<JsonValue *> ValuesList;

		public:

			explicit JsonArrayValue(JsonValue * parent)
				: JsonValue(parent), values_() { ;; }

			virtual ~JsonArrayValue() throw();

			virtual int Type() const  { return ARRAY; }

			virtual void PrintPlain(FILE * stream);

			virtual void Print(FILE * stream, int indent, bool indentFirst);

			bool Empty() const;

			void Add(JsonValue * value);

			virtual const long long AsInteger() const
			{
				throw JsonException("Invalid using Array as Integer");
			}

			virtual const double AsFloat() const
			{
				throw JsonException("Invalid using Array as Float");
			}

			virtual const std::string AsString() const
			{
				throw JsonException("Invalid using Array as String");
			}

		protected:

			void Free();

			ValuesList values_;
	};

	class JsonObjectValue : public JsonValue
	{
		typedef std::list<std::pair<std::string, JsonValue *> > ValuesList;
		typedef std::map<std::string, ValuesList::iterator> IndexMap;

		public:

			explicit JsonObjectValue(JsonValue * parent)
				: JsonValue(parent), values_(), index_() { ;; }

			virtual ~JsonObjectValue() throw();

			virtual int Type() const  { return OBJECT; }

			virtual void PrintPlain(FILE * stream);

			virtual void Print(FILE * stream, int indent, bool indentFirst);

			bool Empty() const;

			const JsonValue * Get(const std::string & key) const
			{
				IndexMap::const_iterator itr = index_.find(key);
				return itr == index_.end() ? 0 : itr->second->second;
			}

			JsonValue *& Get(const std::string & key)
			{
				IndexMap::iterator itr = index_.find(key);
				if (itr != index_.end())
				{
					return itr->second->second;
				}
				else
				{
					values_.push_back(std::make_pair(key, static_cast<JsonValue *>(0)));
					ValuesList::iterator last = values_.end();
					--last;
					index_[key] = last;
					return last->second;
				}
			}

			JsonValue *& Set(const std::string & key, JsonValue * value);

			virtual const long long AsInteger() const
			{
				throw JsonException("Invalid using Object as Integer");
			}

			virtual const double AsFloat() const
			{
				throw JsonException("Invalid using Object as Float");
			}

			virtual const std::string AsString() const
			{
				throw JsonException("Invalid using Object as String");
			}

		protected:

			void Free();

			ValuesList values_;
			IndexMap   index_;
	};

	class JsonConstant
	{
		public:

			JsonConstant(const JsonValue * value)
				: value_(value) { ;; }

			int Type() const
			{
				return value_ == 0 ? JsonValue::NIL : value_->Type();
			}

			bool IsNull()    const  { return Type() == JsonValue::NIL; }
			bool IsInteger() const  { return Type() == JsonValue::INTEGER; }
			bool IsFloat()   const  { return Type() == JsonValue::FLOAT; }
			bool IsString()  const  { return Type() == JsonValue::STRING; }
			bool IsBoolean() const  { return Type() == JsonValue::BOOLEAN; }
			bool IsArray()   const  { return Type() == JsonValue::ARRAY; }
			bool IsObject()  const  { return Type() == JsonValue::OBJECT; }

			const long long AsInteger() const
			{
				return value_ == 0 ? 0 : value_->AsInteger();
			}

			const double AsFloat() const
			{
				return value_ == 0 ? 0.0 : value_->AsFloat();
			}

			const std::string AsString() const
			{
				return value_ == 0 ? "" : value_->AsString();
			}

			JsonConstant Get(const std::string & key) const
			{
				NeedObject(key);
				const JsonValue * value = static_cast<const JsonObjectValue *>(value_)->Get(key);
				if (value == 0)
				{
					throw JsonException(FormatString("Key \"%s\" not found", key.c_str()));
				}
				else
				{
					return JsonConstant(value);
				}
			}

			bool Get(const std::string & key, std::string value) const
			{
				NeedObject(key);
				return false;
			}

		protected:

			void NeedObject(const std::string & key) const
			{
				if (!IsObject())
				{
					throw JsonException(
						FormatString(
							"Unhappy attempt of fetching value by key \"%s\" from object of type %s",
							key.c_str(),
							value_->TypeName()
						)
					);
				}
			}

			const JsonValue * value_;
	};

	class JsonVariable
	{
		public:

			JsonVariable(JsonValue *& value)
				: value_(value) { ;; }

			int Type() const
			{
				return value_ == 0 ? JsonValue::NIL : value_->Type();
			}

			bool IsNull()    const  { return Type() == JsonValue::NIL; }
			bool IsInteger() const  { return Type() == JsonValue::INTEGER; }
			bool IsFloat()   const  { return Type() == JsonValue::FLOAT; }
			bool IsString()  const  { return Type() == JsonValue::STRING; }
			bool IsBoolean() const  { return Type() == JsonValue::BOOLEAN; }
			bool IsArray()   const  { return Type() == JsonValue::ARRAY; }
			bool IsObject()  const  { return Type() == JsonValue::OBJECT; }

			const std::string AsString() const
			{
				if (IsString())
				{
					return static_cast<JsonStringValue *>(value_)->Get();
				}
				else
				{
					throw JsonException("");
				}
			}

			JsonConstant operator[] (const std::string & key) const
			{
				if (IsObject())
				{
					return static_cast<const JsonObjectValue *>(value_)->Get(key);
				}
				else
				{
					throw JsonException("");
				}
			}

			JsonVariable operator[] (const std::string & key)
			{
				if (IsObject())
				{
					return static_cast<JsonObjectValue *>(value_)->Get(key);
				}
				else
				{
					throw JsonException("");
				}
			}

			JsonValue *& CreateObject()
			{
				Reset(new JsonObjectValue(value_->Parent()));
				return value_;
			}

			JsonVariable & operator= (const long long & value)
			{
				if (IsInteger())
				{
					static_cast<JsonIntegerValue *>(value_)->Set(value);
				}
				else
				{
					Reset(new JsonIntegerValue(value_->Parent(), value));
				}
				return *this;
			}

			JsonVariable & operator= (const char * value)
			{
				if (IsString())
				{
					static_cast<JsonStringValue *>(value_)->Set(value);
				}
				else
				{
					Reset(new JsonStringValue(value_->Parent(), value));
				}
				return *this;
			}

			JsonVariable & operator= (const std::string & value)
			{
				if (Type() == JsonValue::STRING)
				{
					static_cast<JsonStringValue *>(value_)->Set(value);
				}
				else
				{
					Reset(new JsonStringValue(value_->Parent(), value));
				}
				return *this;
			}

		protected:

			void Reset(JsonValue * newValue)
			{
				delete value_;
				value_ = newValue;
			}

			void Destroy()
			{
				delete value_;
				value_ = 0;
			}

			JsonValue *& value_;
	};

	class JsonString
	{
		public:

			JsonString(JsonValue ** value)
				: value_(reinterpret_cast<JsonStringValue **>(value))
			{
			}

		protected:

			JsonStringValue ** Cast(JsonValue ** value)
			{
				return 0;
			}

			JsonStringValue ** value_;
	};

	class JsonObject
	{
		public:

			JsonObject(JsonValue * value)
				: value_(reinterpret_cast<JsonObjectValue *>(value))
			{
				// TODO: check (*value_)->Type() == OBJECT
			}

			JsonVariable operator[] (const char * key)
			{
				return value_->Set(key, 0);
			}

		protected:

			JsonObjectValue * value_;
	};

}

#endif
