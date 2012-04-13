#ifndef _XC3__POSTGRESQL_QUERY_PARAMS_HPP__
#define _XC3__POSTGRESQL_QUERY_PARAMS_HPP__ 1

#include <libpq-fe.h>
#include <postgres.h>
#include <catalog/pg_type.h>
#include <string>
#include <vector>
#include <StringUtils.hpp>
#include "PostgreSqlUtils.hpp"

namespace XC3
{
	class PostgreSqlParam
	{
		public:

			PostgreSqlParam() { ;; }

			virtual ~PostgreSqlParam() throw() { ;; }

			virtual void Set(INT_64 iValueIn) = 0;

			virtual void Set(const std::string & sValueIn) = 0;

			virtual Oid Type() const = 0;

			virtual UINT_32 Length() const = 0;

			virtual const void * Value() const = 0;
	};

	template <typename T> class PostgreSqlParamMisc : public PostgreSqlParam
	{
		public:

			PostgreSqlParamMisc() : PostgreSqlParam(), value_()  { ;; }

			virtual ~PostgreSqlParamMisc() throw() { ;; }

			virtual void Set(INT_64 ivalue) { value_ = ReverseBytes(ivalue); }

			virtual void Set(const std::string & svalue) { value_ = ReverseBytes(StrToInt(svalue)); }

			virtual UINT_32 Length() const { return sizeof(T); }

			virtual const void * Value() const { return &value_; }

		private:

			T value_;
	};

	class PostgreSqlParamString : public PostgreSqlParam
	{
		public:

			PostgreSqlParamString() : PostgreSqlParam(), value_()  { ;; }

			virtual ~PostgreSqlParamString() throw() { ;; }

			virtual void Set(INT_64 ivalue) { IntToStr(ivalue, value_); }

			virtual void Set(const std::string & svalue) { value_ = svalue; }

			virtual Oid Type() const { return TEXTOID; }

			virtual UINT_32 Length() const { return value_.length(); }

			virtual const void * Value() const { return value_.data(); }

		private:

			std::string value_;
	};

	class PostgreSqlParamInt64 : public PostgreSqlParamMisc<INT_64>
	{
		public:

			virtual ~PostgreSqlParamInt64() throw() { ;; }

			virtual Oid Type() const { return INT8OID; }
	};

	class PostgreSqlQueryParams
	{
		public:

			PostgreSqlQueryParams();

			~PostgreSqlQueryParams() throw();

			void Create(const CHAR_8 * signature);

			void Destroy();

			UINT_32 Size() const { return params_.size(); }

			const int * Formats() const  { return formats_; }
			const Oid * Types() const  { return types_; }
			const int * Lengths() const  { return lengths_; }
			const char * const * Values() const { return values_; }

			void Set(UINT_32 index, INT_64 value);
			void Set(UINT_32 index, const std::string & value);

			void UpdateValues();

		private:

			std::vector<PostgreSqlParam *> params_;
			int  *  formats_;
			Oid  *  types_;
			int  *  lengths_;
			char ** values_;
	};
}

#endif
