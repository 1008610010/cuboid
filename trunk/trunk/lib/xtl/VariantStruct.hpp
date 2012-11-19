#ifndef XTL__VARIANT_STRUCT_HPP__
#define XTL__VARIANT_STRUCT_HPP__ 1

#include <memory>
#include <map>
#include <string>

#include "Variant.hpp"
#include "VariantPtr.hpp"

namespace XTL
{
	class PrintStream;

	class Variant::Struct : public Variant
	{
		public:

			class ConstIterator;
			class Iterator;

			virtual ~Struct() throw();

			virtual bool IsStruct() const;

			virtual void Visit(Visitor & visitor) const;

			virtual void DebugPrintValue(PrintStream & stream, unsigned int indent) const;

			virtual bool IsEmpty() const = 0;

			virtual unsigned int Size() const = 0;

			virtual bool Exists(const std::string & key) const = 0;

			virtual const VariantPtr * Get(const std::string & key) const = 0;

			virtual VariantPtr * Get(const std::string & key) = 0;

			virtual void Set(const std::string & key, VariantPtr value) = 0;

			virtual std::auto_ptr<ConstIterator> GetConstIterator() const = 0;

			virtual std::auto_ptr<Iterator> GetIterator() = 0;

			class ConstIterator
			{
				public:

					virtual ~ConstIterator() throw() { ;; }

					virtual bool AtEnd() const = 0;

					virtual bool NotAtEnd() const
					{
						return !AtEnd();
					}

					virtual void Advance() = 0;

					virtual const std::string & Key() const = 0;

					virtual const VariantPtr & Value() const = 0;
			};

			class Iterator : public ConstIterator
			{
				public:

					virtual ~Iterator() throw() { ;; }

					virtual VariantPtr & Value() const = 0;
			};
	};

	class VariantMap : public Variant::Struct
	{
		public:

			VariantMap();

			virtual ~VariantMap() throw();

			virtual bool IsEmpty() const;

			virtual unsigned int Size() const;

			virtual bool Exists(const std::string & key) const;

			virtual const VariantPtr * Get(const std::string & key) const;

			virtual VariantPtr * Get(const std::string & key);

			virtual void Set(const std::string & key, VariantPtr value);

			virtual std::auto_ptr<Variant::Struct::ConstIterator> GetConstIterator() const;

			virtual std::auto_ptr<Variant::Struct::Iterator> GetIterator();

		private:

			std::map<std::string, VariantPtr> items_;
	};
}

#endif
