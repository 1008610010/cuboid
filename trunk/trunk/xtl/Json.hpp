#ifndef _XTL__JSON_HPP__
#define _XTL__JSON_HPP__ 1

#include <string>
#include <map>
#include <list>
#include <vector>
#include "StringUtils.hpp"

namespace XTL
{
	enum JsonTypes
	{
		JSON_UNDEFINED = 0,
		JSON_NULL      = 1,
		JSON_INTEGER   = 2,
		JSON_FLOAT     = 3,
		JSON_STRING    = 4,
		JSON_BOOLEAN   = 5,
		JSON_ARRAY     = 6,
		JSON_OBJECT    = 7
	};

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

			static const char * TypeNames[];

			explicit JsonValue(JsonValue * parent)
				: parent_(parent) { ;; }

 			virtual ~JsonValue() throw()   { ;; }

			JsonValue * Parent()           { return parent_; }

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

			explicit JsonNullValue(JsonValue * parent)
				: JsonValue(parent) { ;; }

			virtual ~JsonNullValue() throw() { ;; }

			virtual int Type() const  { return JSON_NULL; }

			virtual void PrintPlain(FILE * stream);

			virtual const long long AsInteger() const   { return 0ll; }

			virtual const double AsFloat() const        { return 0.0; }

			virtual const std::string AsString() const  { return ""; }
	};

	class JsonIntegerValue : public JsonValue
	{
		public:

			JsonIntegerValue(JsonValue * parent, const long long & value)
				: JsonValue(parent), value_(value) { ;; }

			virtual ~JsonIntegerValue() throw() { ;; }

			virtual int Type() const  { return JSON_INTEGER; }

			virtual void PrintPlain(FILE * stream);

			virtual const long long AsInteger() const   { return value_; }

			virtual const double AsFloat() const        { return IntegerToFloat(value_); }

			virtual const std::string AsString() const  { return IntegerToString(value_); }

			const long long Get() const
			{
				return value_;
			}

			void Set(const long long & value)
			{
				value_ = value;
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

			virtual int Type() const  { return JSON_FLOAT; }

			virtual void PrintPlain(FILE * stream);

			virtual const long long AsInteger() const   { return FloatToInteger(value_); }

			virtual const double AsFloat() const        { return value_; }

			virtual const std::string AsString() const  { return FloatToString(value_); }

			const double Get() const
			{
				return value_;
			}

			void Set(const double & value)
			{
				value_ = value;
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

			virtual int Type() const  { return JSON_STRING; }

			virtual void PrintPlain(FILE * stream);

			virtual const long long AsInteger() const   { return StringToInteger(value_); }

			virtual const double AsFloat() const        { return StringToFloat(value_); }

			virtual const std::string AsString() const  { return value_; }

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

		protected:

			std::string value_;
	};

	class JsonBooleanValue : public JsonValue
	{
		public:

			JsonBooleanValue(JsonValue * parent, bool value)
				: JsonValue(parent), value_(value) { ;; }

			virtual ~JsonBooleanValue() throw() { ;; }

			virtual int Type() const  { return JSON_BOOLEAN; }

			virtual void PrintPlain(FILE * stream);

			virtual const long long AsInteger() const   { return value_ ? 1 : 0; }

			virtual const double AsFloat() const        { return value_ ? 1.0 : 0.0; }

			virtual const std::string AsString() const  { return value_ ? "true" : "false"; }

			const bool Get() const
			{
				return value_;
			}

			void Set(const long long & value)
			{
				value_ = value;
			}

		protected:

			bool value_;
	};

	class JsonArrayValue : public JsonValue
	{
		typedef std::list<JsonValue *> ValuesList;

		public:

			class Iterator
			{
				public:

					Iterator(JsonArrayValue * array)
						: values_(array->values_), itr_(array->values_.begin()) { ;; }

					bool AtEnd() const
					{
						return itr_ == values_.end();
					}

					void Delete()
					{
						delete *itr_;
						itr_ = values_.erase(itr_);
					}

					const JsonValue * operator* () const
					{
						return *itr_;
					}

					JsonValue *& operator* ()
					{
						return *itr_;
					}

					Iterator & operator++ ()
					{
						++itr_;
						return *this;
					}

					Iterator operator++ (int)
					{
						Iterator temp(values_, itr_);
						++itr_;
						return temp;
					}

				protected:

					Iterator(ValuesList & values, ValuesList::iterator & itr)
						: values_(values), itr_(itr) { ;; }

					ValuesList & values_;
					ValuesList::iterator itr_;
			};


			explicit JsonArrayValue(JsonValue * parent)
				: JsonValue(parent), values_() { ;; }

			virtual ~JsonArrayValue() throw();

			virtual int Type() const  { return JSON_ARRAY; }

			virtual void PrintPlain(FILE * stream);

			virtual void Print(FILE * stream, int indent, bool indentFirst);

			bool Empty() const;

			JsonValue *& Front();

			JsonValue *& Back();

			JsonValue *& PushFront(JsonValue * value);

			JsonValue *& PushBack(JsonValue * value);

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

			virtual int Type() const  { return JSON_OBJECT; }

			virtual void PrintPlain(FILE * stream);

			virtual void Print(FILE * stream, int indent, bool indentFirst);

			bool Empty() const;
			
			void Delete(const std::string & key)
			{
				IndexMap::iterator itr = index_.find(key);
				if (itr != index_.end())
				{
					delete itr->second->second;
					values_.erase(itr->second);
					index_.erase(itr);
				}
			}

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
				return value_ == 0 ? JSON_UNDEFINED : value_->Type();
			}

			const char * TypeName() const
			{
				return value_ == 0 ? "Undefined" : value_->TypeName();
			}

			bool IsNull()    const  { return Type() == JSON_NULL; }
			bool IsInteger() const  { return Type() == JSON_INTEGER; }
			bool IsFloat()   const  { return Type() == JSON_FLOAT; }
			bool IsString()  const  { return Type() == JSON_STRING; }
			bool IsBoolean() const  { return Type() == JSON_BOOLEAN; }
			bool IsArray()   const  { return Type() == JSON_ARRAY; }
			bool IsObject()  const  { return Type() == JSON_OBJECT; }

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
							"Unsuccessful attempt of fetching value by key \"%s\" from object of type %s",
							key.c_str(),
							value_->TypeName()
						)
					);
				}
			}

			const JsonValue * value_;
	};

	template <typename ValueTypePtr>
	class JsonVariableBase
	{
		public:

			JsonVariableBase(ValueTypePtr value)
				: value_(value)
			{
				;;
			}

			int Type() const
			{
				return value_ == 0 ? JSON_UNDEFINED : value_->Type();
			}

			bool IsNull()    const  { return Type() == JSON_NULL; }

			bool IsInteger() const  { return Type() == JSON_INTEGER; }

			bool IsFloat()   const  { return Type() == JSON_FLOAT; }

			bool IsString()  const  { return Type() == JSON_STRING; }

			bool IsBoolean() const  { return Type() == JSON_BOOLEAN; }

			bool IsArray()   const  { return Type() == JSON_ARRAY; }

			bool IsObject()  const  { return Type() == JSON_OBJECT; }

			void Destroy()
			{
				delete value_;
				value_ = 0;
			}

			void Print(FILE * stream) const
			{
				if (value_ == 0)
				{
					fprintf(stream, "null");
				}
				else
				{
					value_->Print(stream, 0, false);
				}
			}

			JsonVariableBase<JsonIntegerValue *> Set(const long long & value)
			{
				if (IsInteger())
				{
					static_cast<JsonIntegerValue *>(value_)->Set(value);
				}
				else
				{
					Reset(new JsonIntegerValue(Parent(), value));
				}
				return static_cast<JsonIntegerValue *>(value_);
			}

			JsonVariableBase<JsonFloatValue *> Set(const double & value)
			{
				if (IsFloat())
				{
					static_cast<JsonFloatValue *>(value_)->Set(value);
				}
				else
				{
					Reset(new JsonFloatValue(Parent(), value));
				}
				return static_cast<JsonFloatValue *>(value_);
			}

			JsonVariableBase<JsonStringValue *> Set(const char * value)
			{
				if (IsString())
				{
					static_cast<JsonStringValue *>(value_)->Set(value);
				}
				else
				{
					Reset(new JsonStringValue(Parent(), value));
				}
				return static_cast<JsonStringValue *>(value_);
			}

			JsonVariableBase<JsonStringValue *> Set(const std::string & value)
			{
				if (IsString())
				{
					static_cast<JsonStringValue *>(value_)->Set(value);
				}
				else
				{
					Reset(new JsonStringValue(Parent(), value));
				}
				return static_cast<JsonStringValue *>(value_);
			}

			JsonVariableBase<JsonBooleanValue *> Set(bool value)
			{
				if (IsString())
				{
					static_cast<JsonBooleanValue *>(value_)->Set(value);
				}
				else
				{
					Reset(new JsonBooleanValue(Parent(), value));
				}
				return static_cast<JsonBooleanValue *>(value_);
			}
			
			JsonVariableBase<JsonIntegerValue *> operator= (int value)
			{
				return Set(static_cast<long long>(value));
			}

			JsonVariableBase<JsonIntegerValue *> operator= (const long long & value)
			{
				return Set(value);
			}

			JsonVariableBase<JsonFloatValue *> operator= (const double & value)
			{
				return Set(value);
			}

			JsonVariableBase<JsonStringValue *> operator= (const char * value)
			{
				return Set(value);
			}

			JsonVariableBase<JsonStringValue *> operator= (const std::string & value)
			{
				return Set(value);
			}

			JsonVariableBase<JsonBooleanValue *> operator= (bool value)
			{
				return Set(value);
			}

			JsonVariableBase<JsonArrayValue *> CreateArray()
			{
				Reset(new JsonArrayValue(Parent()));
				return static_cast<JsonArrayValue *>(value_);
			}

			JsonVariableBase<JsonObjectValue *> CreateObject()
			{
				Reset(new JsonObjectValue(Parent()));
				return static_cast<JsonObjectValue *>(value_);
			}
/*
			JsonVariableBase<JsonObjectValue *> AsObject()
			{
				// TODO: check Type()
				return static_cast<JsonObjectValue *>(value_);
			}
*/
		protected:

			JsonValue * Parent()
			{
				return value_ == 0 ? 0 : value_->Parent();
			}

			const JsonValue * Parent() const
			{
				return value_ == 0 ? 0 : value_->Parent();
			}

			void Reset(JsonValue * value)
			{
				delete value_;
				value_ = value;
			}

			ValueTypePtr value_;
	};

	class JsonVariable : protected JsonVariableBase<JsonValue *>
	{
		typedef JsonVariableBase<JsonValue *> Super;

		public:

			JsonVariable() : Super(0) { ;; }

			JsonVariable(JsonValue * value) : Super(value) { ;; }

			using Super::Type;
			using Super::IsNull;
			using Super::IsInteger;
			using Super::IsFloat;
			using Super::IsString;
			using Super::IsBoolean;
			using Super::IsArray;
			using Super::IsObject;
			using Super::Destroy;
			using Super::Print;
			using Super::Set;
			using Super::CreateArray;
			using Super::CreateObject;
			using Super::operator=;
//			using Super::AsObject;
	};

	class JsonVariableRef : protected JsonVariableBase<JsonValue *&>
	{
		typedef JsonVariableBase<JsonValue *&> Super;

		public:

			JsonVariableRef(JsonValue *& value) : Super(value) { ;; }

			using Super::Type;
			using Super::IsNull;
			using Super::IsInteger;
			using Super::IsFloat;
			using Super::IsString;
			using Super::IsBoolean;
			using Super::IsArray;
			using Super::IsObject;
			using Super::Destroy;
			using Super::Print;
			using Super::Set;
			using Super::CreateArray;
			using Super::CreateObject;
			using Super::operator=;

			bool IsScalar() const
			{
				switch (Type())
				{
					case JSON_NULL:
					case JSON_INTEGER:
					case JSON_FLOAT:
					case JSON_STRING:
					case JSON_BOOLEAN:
						return true;

					default:
						return false;
				}
			}

			const std::string AsString()
			{
				return IsScalar() ? value_->AsString() : "";
			}
			
			const long long ToInteger(long long defaultValue = 0)
			{
				if (!IsInteger())
				{
					Reset(new JsonIntegerValue(Parent(), IsScalar() ? value_->AsInteger() : defaultValue));
				}

				return static_cast<JsonIntegerValue *>(value_)->Get();
			}
			
			const std::string ToString()
			{
				if (!IsString())
				{
					Reset(new JsonStringValue(Parent(), IsScalar() ? value_->AsString() : ""));
				}

				return static_cast<JsonStringValue *>(value_)->Get();
			}
			
			JsonVariableBase<JsonArrayValue *> AsArray()
			{
				if (IsArray())
				{
					return static_cast<JsonArrayValue *>(value_);
				}
				else
				{
					return CreateArray();
				}
			}
			
			JsonVariableBase<JsonObjectValue *> AsObject()
			{
				if (IsObject())
				{
					return static_cast<JsonObjectValue *>(value_);
				}
				else
				{
					return CreateObject();
				}
			}
	};

	class JsonArray : protected JsonVariableBase<JsonArrayValue *>
	{
		typedef JsonVariableBase<JsonArrayValue *> Super;

		public:

			class Iterator
			{
				public:

					Iterator(JsonArray & array)
						: itr_(array.value_)
					{
						;;
					}

					bool AtEnd() const
					{
						return itr_.AtEnd();
					}

					JsonVariableRef operator* ()
					{
						return *itr_;
					}

					Iterator & operator++ ()
					{
						++itr_;
						return *this;
					}

					Iterator operator++ (int)
					{
						Iterator temp(itr_);
						++itr_;
						return temp;
					}

					void Delete()
					{
						itr_.Delete();
					}

				protected:

					Iterator(const JsonArrayValue::Iterator & itr) : itr_(itr) { ;; }

					JsonArrayValue::Iterator itr_;
			};

			JsonArray(Super value) : Super(value) { ;; }

			Iterator Begin()
			{
				return Iterator(*this);
			}

			JsonVariableRef PushBack()
			{
				return value_->PushBack(0);
			}

			JsonArray & PushBackNull()
			{
				value_->PushBack(new JsonNullValue(value_));
				return *this;
			}

			JsonArray & PushBack(int value)
			{
				value_->PushBack(new JsonIntegerValue(value_, value));
				return *this;
			}

			JsonArray & PushBack(const long long & value)
			{
				value_->PushBack(new JsonIntegerValue(value_, value));
				return *this;
			}

			JsonArray & PushBack(const char * value)
			{
				value_->PushBack(new JsonStringValue(value_, value));
				return *this;
			}

			JsonArray & PushBack(const std::string & value)
			{
				value_->PushBack(new JsonStringValue(value_, value));
				return *this;
			}

			JsonArray & PushBack(bool value)
			{
				value_->PushBack(new JsonBooleanValue(value_, value));
				return *this;
			}

			JsonArray PushBackArray()
			{
				return static_cast<JsonArrayValue *>(value_->PushBack(new JsonArrayValue(value_)));
			}

		protected:

			JsonArray(JsonArrayValue * value) : Super(value) { ;; }
	};

	class JsonObject : protected JsonVariableBase<JsonObjectValue *>
	{
		typedef JsonVariableBase<JsonObjectValue *> Super;

		public:

			JsonObject(Super value) : Super(value) { ;; }

			void Delete(const char * key)
			{
				value_->Delete(key);
			}

			JsonVariableRef operator[] (const char * key)
			{
				return value_->Get(key);
			}

		protected:

//			JsonObject(JsonObjectValue * value) : Super(value) { ;; }
	};
}

#endif
