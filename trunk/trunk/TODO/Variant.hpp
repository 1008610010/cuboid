#ifndef _XTL__VARIANT_HPP__
#define _XTL__VARIANT_HPP__ 1

#include "StringUtils.hpp"
#include <string>
#include <memory>

namespace XTL
{
	/**
	 * @class VariantValue
	 * @brief Value of variant typed object.
	 *
	 * @retval 1
	 */
	class VariantValue
	{
		public:

			virtual ~VariantValue() throw() { ;; }

			virtual VariantValue * Clone() const = 0;

			virtual const long long AsInteger() const = 0;

			virtual const double AsFloat() const = 0;

			virtual const std::string AsString() const = 0;
	};

	template <typename _BaseType>
	class VariantTypedValue : public VariantValue
	{
		public:

			typedef _BaseType BaseType;

			VariantTypedValue() : VariantValue(), value_() { ;; }

			VariantTypedValue(const _BaseType & value) : VariantValue(), value_(value) { ;; }

			virtual ~VariantTypedValue() throw()
			{
				;;
			}

			operator BaseType & ()
			{
				return value_;
			}

			operator const BaseType & () const
			{
				return value_;
			}

		protected:

			BaseType value_;
	};

	class VariantInteger : public VariantTypedValue<long long>
	{
		typedef VariantTypedValue<long long> Super;

		public:

			VariantInteger() : Super() { ;; }

			VariantInteger(const long long & value) : Super(value) { ;; }

			virtual VariantInteger * Clone() const  { return new VariantInteger(value_); }

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
	};

	class VariantFloat : public VariantTypedValue<double>
	{
		typedef VariantTypedValue<double> Super;

		public:

			VariantFloat() : Super() { ;; }

			VariantFloat(const double & value) : Super(value) { ;; }

			virtual VariantFloat * Clone() const  { return new VariantFloat(value_); }

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
	};

	class VariantString : public VariantTypedValue<std::string>
	{
		typedef VariantTypedValue<std::string> Super;

		public:

			VariantString() : Super() { ;; }

			VariantString(const std::string & value) : Super(value) { ;; }

			VariantString(const char * value) : Super(value) { ;; }

			virtual VariantString * Clone() const  { return new VariantString(value_); }

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
	};

	class Variant
	{
		public:

			Variant()
				: value_(0) { ;; }

			Variant(const Variant & variant)
				: value_(variant.IsNull() ? 0 : variant.value_->Clone()) { ;; }

			Variant(const long long & value)
				: value_(new VariantInteger(value)) { ;; }

			Variant(const double & value)
				: value_(new VariantFloat(value)) { ;; }

			Variant(const char * value)
				: value_(new VariantString(value)) { ;; }

			Variant(const std::string & value)
				: value_(new VariantString(value)) { ;; }

			bool IsNull() const
			{
				return value_.get() == 0;
			}

			void Clear()
			{
				value_.reset(0);
			}

			const long long AsInteger() const
			{
				return value_.get() == 0 ? 0 : value_->AsInteger();
			}

			const double AsFloat() const
			{
				return value_.get() == 0 ? 0.0 : value_->AsFloat();
			}

			const std::string AsString() const
			{
				return value_.get() == 0 ? "" : value_->AsString();
			}

			Variant & operator= (const Variant & variant)
			{
				value_.reset(variant.IsNull() ? 0 : variant.value_->Clone());
				return *this;
			}

			Variant & operator= (const long long & value)
			{
				value_.reset(new VariantInteger(value));
				return *this;
			}

			Variant & operator= (const double & value)
			{
				value_.reset(new VariantFloat(value));
				return *this;
			}

			Variant & operator= (const char * value)
			{
				value_.reset(new VariantString(value));
				return *this;
			}

			Variant & operator= (const std::string & value)
			{
				value_.reset(new VariantString(value));
				return *this;
			}

			Variant & ToInteger()
			{
				return *this = AsInteger();
			}

			Variant & ToFloat()
			{
				return *this = AsFloat();
			}

			Variant & ToString()
			{
				return *this = AsString();
			}

		protected:

			std::auto_ptr<VariantValue> value_;
	};
}

#endif
