#ifndef XTL__VARIANT_ARRAY_HPP__
#define XTL__VARIANT_ARRAY_HPP__ 1

#include <memory>
#include <vector>

#include "Variant.hpp"
#include "VariantPtr.hpp"

namespace XTL
{
	class PrintStream;

	class Variant::Array : public Variant
	{
		public:

			class ConstIterator;
			class Iterator;

			virtual ~Array() throw();

			virtual bool IsArray() const;

			virtual Variant::Array * ToArray();

			void Visit(Visitor & visitor) const;

			virtual void DebugPrintValue(PrintStream & stream, unsigned int indent) const;

			virtual bool IsEmpty() const = 0;

			virtual unsigned int Size() const = 0;

			virtual const VariantPtr & At(unsigned int index) const = 0;

			virtual VariantPtr & At(unsigned int index) = 0;

			virtual std::auto_ptr<ConstIterator> GetConstIterator() const = 0;

			virtual std::auto_ptr<Iterator> GetIterator() = 0;

			class ConstIterator
			{
				public:

					virtual ~ConstIterator() throw()
					{
						;;
					}

					virtual bool AtEnd() const = 0;

					virtual bool NotAtEnd() const
					{
						return !AtEnd();
					}

					virtual void Advance() = 0;

					virtual const VariantPtr & Current() const = 0;
			};

			class Iterator : public ConstIterator
			{
				public:

					virtual ~Iterator() throw() { ;; }

					virtual VariantPtr & Current() const = 0;
			};
	};

	class VariantVector : public Variant::Array
	{
		public:

			VariantVector();

			virtual ~VariantVector() throw();

			virtual bool IsEmpty() const;

			virtual unsigned int Size() const;

			virtual const VariantPtr & At(unsigned int index) const;

			virtual VariantPtr & At(unsigned int index);

			virtual std::auto_ptr<Variant::Array::ConstIterator> GetConstIterator() const;

			virtual std::auto_ptr<Variant::Array::Iterator> GetIterator();

			void PushBack(VariantPtr ptr);

		private:

			std::vector<VariantPtr> items_;
	};
}

#endif
