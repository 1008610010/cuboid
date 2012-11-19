#ifndef XTL__VARIANT_SCALAR_HPP__
#define XTL__VARIANT_SCALAR_HPP__ 1

#include "Variant.hpp"
#include "VariantPtr.hpp"

namespace XTL
{
	// TODO: replace subclasses of Variant:
	// Variant::Scalar to VariantScalar

	class Variant::Scalar : public Variant
	{
		public:

			virtual ~Scalar() throw()
			{
				;;
			}

			virtual bool IsScalar() const
			{
				return true;
			}
	};

	class Variant::Null : public Variant::Scalar
	{
		public:

			static VariantPtr Instance();

			virtual ~Null() throw();

			virtual bool IsNull() const;

			virtual void DebugPrintValue(PrintStream & stream, unsigned int indent) const;

			virtual LongLongInt * ToLongLongInt();

			virtual Double * ToDouble();

			virtual Variant::String * ToString();

			virtual void Visit(Visitor & visitor) const;

		private:

			Null()
				: Variant::Scalar() { ;; }
	};

	class Variant::Boolean : public Variant::Scalar
	{
		public:

			static VariantPtr True();

			static VariantPtr False();

			virtual ~Boolean() throw();

			virtual LongLongInt * ToLongLongInt();

			virtual Double * ToDouble();

			virtual Variant::String * ToString();

			virtual void Visit(Visitor & visitor) const;

			virtual void DebugPrintValue(PrintStream & stream, unsigned int indent) const;

			bool GetValue() const;

		private:

			explicit Boolean(bool value);

			const bool value_;
	};

	class Variant::Number : public Variant::Scalar
	{
		public:

			virtual ~Number() throw() { ;; }

			virtual bool IsInteger() const = 0;
	};

	class Variant::LongLongInt : public Variant::Number
	{
		public:

			explicit LongLongInt(const long long int & value);

			virtual ~LongLongInt() throw();

			virtual LongLongInt * ToLongLongInt();

			virtual Double * ToDouble();

			virtual String * ToString();

			virtual bool IsInteger() const;

			virtual void Visit(Visitor & visitor) const;

			virtual void DebugPrintValue(PrintStream & stream, unsigned int indent) const;

			const long long int GetValue() const { return value_; }

		private:

			const long long int value_;
	};

	class Variant::Double : public Variant::Number
	{
		public:

			explicit Double(const double & value);

			virtual ~Double() throw();

			virtual LongLongInt * ToLongLongInt();

			virtual Double * ToDouble();

			virtual String * ToString();

			virtual bool IsInteger() const;

			virtual void Visit(Visitor & visitor) const;

			virtual void DebugPrintValue(PrintStream & stream, unsigned int indent) const;

			const double GetValue() const { return value_; }

		private:

			const double value_;
	};

	class Variant::String : public Variant::Scalar
	{
		public:

			explicit String(const std::string & value);

			explicit String(const char * value);

			virtual ~String() throw();

			virtual LongLongInt * ToLongLongInt();

			virtual Double * ToDouble();

			virtual Variant::String * ToString();

			virtual void Visit(Visitor & visitor) const;

			virtual void DebugPrintValue(PrintStream & stream, unsigned int indent) const;

			const std::string & GetValue() const { return value_; }

		private:

			const std::string value_;
	};
}

#endif
