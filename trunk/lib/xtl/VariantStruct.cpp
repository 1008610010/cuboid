#include "VariantStruct.hpp"

#include "PrintStream.hpp"

namespace XTL
{
	/********** Variant::Struct **********/

	Variant::Struct::~Struct() throw()
	{
		;;
	}

	bool Variant::Struct::IsStruct() const
	{
		return true;
	}

	void Variant::Struct::DebugPrintValue(PrintStream & stream, unsigned int indent) const
	{
		stream.Print("{\n");

		std::auto_ptr<ConstIterator> itr(GetConstIterator());

		if (itr->NotAtEnd())
		{
			do
			{
				PrintIndent(stream, indent + 1);
				stream.PrintF("%s : ", itr->Key());
				itr->Value()->DebugPrintValue(stream, indent + 1);
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
		stream.Print("}");
	}

	/********** VariantStruct **********/

	namespace
	{
		class VariantMapConstIterator : public Variant::Struct::ConstIterator
		{
			public:

				explicit VariantMapConstIterator(const std::map<std::string, VariantPtr> & items)
					: end_(items.end()),
					  itr_(items.begin())
				{
					;;
				}

				virtual ~VariantMapConstIterator() throw() { ;; }

				virtual bool AtEnd() const                 { return itr_ == end_; }

				virtual bool NotAtEnd() const              { return itr_ != end_; }

				virtual void Advance()                     { ++itr_; }

				virtual const std::string & Key() const    { return itr_->first; }

				virtual const VariantPtr & Value() const   { return itr_->second; }

			private:

				const std::map<std::string, VariantPtr>::const_iterator end_;
				std::map<std::string, VariantPtr>::const_iterator itr_;
		};

		class VariantMapIterator : public Variant::Struct::Iterator
		{
			public:

				explicit VariantMapIterator(std::map<std::string, VariantPtr> & items)
					: end_(items.end()),
					  itr_(items.begin())
				{
					;;
				}

				virtual ~VariantMapIterator() throw() { ;; }

				virtual bool AtEnd() const              { return itr_ == end_; }

				virtual bool NotAtEnd() const           { return itr_ != end_; }

				virtual void Advance()                  { ++itr_; }

				virtual const std::string & Key() const { return itr_->first; }

				virtual VariantPtr & Value() const      { return itr_->second; }

			private:

				const std::map<std::string, VariantPtr>::iterator end_;
				std::map<std::string, VariantPtr>::iterator itr_;
		};
	}

	VariantMap::VariantMap()
		: Variant::Struct(),
		  items_()
	{
		;;
	}

	VariantMap::~VariantMap() throw()
	{
		;;
	}

	bool VariantMap::IsEmpty() const
	{
		return items_.empty();
	}

	unsigned int VariantMap::Size() const
	{
		return items_.size();
	}

	bool VariantMap::Exists(const std::string & key) const
	{
		return items_.count(key) > 0;
	}

	const VariantPtr * VariantMap::Get(const std::string & key) const
	{
		const std::map<std::string, VariantPtr>::const_iterator itr = items_.find(key);
		return itr != items_.end() ? &(itr->second) : 0;
	}

	VariantPtr * VariantMap::Get(const std::string & key)
	{
		const std::map<std::string, VariantPtr>::iterator itr = items_.find(key);
		return itr != items_.end() ? &(itr->second) : 0;
	}

	void VariantMap::Set(const std::string & key, VariantPtr value)
	{
		items_[key] = value;
	}

	std::auto_ptr<Variant::Struct::ConstIterator> VariantMap::GetConstIterator() const
	{
		return std::auto_ptr<Variant::Struct::ConstIterator>(new VariantMapConstIterator(items_));
	}

	std::auto_ptr<Variant::Struct::Iterator> VariantMap::GetIterator()
	{
		return std::auto_ptr<Variant::Struct::Iterator>(new VariantMapIterator(items_));
	}
}
