#include "VariantArray.hpp"

#include "PrintStream.hpp"

namespace XTL
{
	/********** Variant::Array **********/

	Variant::Array::~Array() throw()
	{
		;;
	}

	bool Variant::Array::IsArray() const
	{
		return true;
	}

	Variant::Array * Variant::Array::ToArray()
	{
		return this;
	}

	void Variant::Array::DebugPrintValue(PrintStream & stream, unsigned int indent) const
	{
		stream.Print("[\n");

		std::auto_ptr<Variant::Array::ConstIterator> itr(GetConstIterator());

		if (itr->NotAtEnd())
		{
			do
			{
				PrintIndent(stream, indent + 1);
				itr->Current()->DebugPrintValue(stream, indent + 1);
				itr->Advance();
				if (itr->NotAtEnd())
				{
					stream.Print(",");
				}
				stream.Print("\n");
			}
			while (itr->NotAtEnd());
		}

		PrintIndent(stream, indent);
		stream.Print("]");
	}

	/********** VariantVector **********/

	namespace
	{
		class VariantVectorConstIterator : public Variant::Array::ConstIterator
		{
			public:

				explicit VariantVectorConstIterator(const std::vector<VariantPtr> & items)
					: end_(items.end()),
					  itr_(items.begin())
				{
					;;
				}

				virtual ~VariantVectorConstIterator() throw() { ;; }

				virtual bool AtEnd() const                 { return itr_ == end_; }

				virtual bool NotAtEnd() const              { return itr_ != end_; }

				virtual void Advance()                     { ++itr_; }

				virtual const VariantPtr & Current() const { return *itr_; }

			private:

				const std::vector<VariantPtr>::const_iterator end_;
				std::vector<VariantPtr>::const_iterator itr_;
		};

		class VariantVectorIterator : public Variant::Array::Iterator
		{
			public:

				explicit VariantVectorIterator(std::vector<VariantPtr> & items)
					: end_(items.end()),
					  itr_(items.begin())
				{
					;;
				}

				virtual ~VariantVectorIterator() throw() { ;; }

				virtual bool AtEnd() const           { return itr_ == end_; }

				virtual bool NotAtEnd() const        { return itr_ != end_; }

				virtual void Advance()               { ++itr_; }

				virtual VariantPtr & Current() const { return *itr_; }

			private:

				const std::vector<VariantPtr>::iterator end_;
				std::vector<VariantPtr>::iterator itr_;
		};
	}

	VariantVector::VariantVector()
		: Variant::Array(),
		  items_()
	{
		;;
	}

	VariantVector::~VariantVector() throw()
	{
		;;
	}

	bool VariantVector::IsEmpty() const
	{
		return items_.empty();
	}

	unsigned int VariantVector::Size() const
	{
		return items_.size();
	}

	const VariantPtr & VariantVector::At(unsigned int index) const
	{
		if (index >= items_.size())
		{
			throw OutOfBounds();
		}

		return items_[index];
	}

	VariantPtr & VariantVector::At(unsigned int index)
	{
		if (index >= items_.size())
		{
			throw OutOfBounds();
		}

		return items_[index];
	}

	void VariantVector::PushBack(VariantPtr ptr)
	{
		items_.push_back(ptr);
	}

	std::auto_ptr<Variant::Array::ConstIterator> VariantVector::GetConstIterator() const
	{
		return std::auto_ptr<Variant::Array::ConstIterator>(new VariantVectorConstIterator(items_));
	}

	std::auto_ptr<Variant::Array::Iterator> VariantVector::GetIterator()
	{
		return std::auto_ptr<Variant::Array::Iterator>(new VariantVectorIterator(items_));
	}
}
