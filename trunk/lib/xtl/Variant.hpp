#ifndef XTL__VARIANT_HPP__
#define XTL__VARIANT_HPP__ 1

#include <stdio.h>

#include <string>

namespace XTL
{
	class PrintStream;

	class Variant
	{
		public:

			class BadCast
			{
			};

			class OutOfBounds
			{
			};

			class Scalar;
				class Null;
				class Boolean;
				class Number;
					class LongLongInt;
					class Double;
				class String;
			class Array;
			class Struct;

			class Visitor
			{
				public:

					virtual ~Visitor() throw() { ;; }

					virtual void Visit(const Null & v) = 0;

					virtual void Visit(const Boolean & v) = 0;

					virtual void Visit(const LongLongInt & v) = 0;

					virtual void Visit(const Double & v) = 0;

					virtual void Visit(const String & v) = 0;

					virtual void Visit(const Array & v) = 0;

					virtual void Visit(const Struct & v) = 0;
			};

			Variant()
				: refCount_(0) // WARNING: After construction refCount_ is zero!
			{
				;;
			}

			virtual ~Variant() throw();

			virtual bool IsNull() const
			{
				return false;
			}

			virtual bool IsScalar() const
			{
				return false;
			}

			virtual bool IsArray() const
			{
				return false;
			}

			virtual bool IsStruct() const
			{
				return false;
			}

			virtual LongLongInt * ToLongLongInt()
			{
				throw BadCast();
			}

			virtual Double * ToDouble()
			{
				throw BadCast();
			}

			virtual String * ToString()
			{
				throw BadCast();
			}

			virtual Array * ToArray()
			{
				throw BadCast();
			}

			virtual void Visit(Visitor & visitor) const = 0;

			void DebugPrint(PrintStream & stream) const;

			virtual void DebugPrintValue(PrintStream & stream, unsigned int indent) const = 0;

		protected:

			static void PrintIndent(PrintStream & stream, unsigned int indent);

		private:

			Variant(const Variant &);
			Variant & operator= (const Variant &);

			friend class VariantPtr;

			void AddRef()
			{
				++refCount_;
			}

			bool Release()
			{
				return --refCount_ == 0;
			}

			unsigned int refCount_;
	};
}

#endif
