#include "StructPrototype.hpp"

#include "FieldType.hpp"

#include <string.h>

namespace XTL
{
namespace PLAIN
{
	StructPrototype::StructPrototype(const std::string & name)
		: name_(name),
		  size_(0),
		  alignment_(1),
		  fields_(),
		  defaultValue_()
	{
		;;
	}

	void StructPrototype::AdjustSize(unsigned int alignment)
	{
		if (size_ % alignment != 0)
		{
			size_ = alignment * (size_ / alignment + 1);
			defaultValue_.resize(size_, '\0');
		}
	}

	void StructPrototype::AddField(const std::string & fieldName, SharedPtr<const FieldType> fieldType)
	{
		AdjustSize(fieldType->Alignment());

		std::auto_ptr<Field> newField(new Field(fieldName, fieldType, size_));

		size_ += fieldType->Size();
		if (alignment_ < fieldType->Alignment())
		{
			alignment_ = fieldType->Alignment();
		}

		fields_.PushBack(newField);
		defaultValue_.resize(size_, '\0');

		/*
		if (fieldType->IsObject())
		{
			objectFields_.push_back(fields_.Size() - 1);
		}
		*/
	}

	void StructPrototype::Initialize(void * data) const
	{
		::memcpy(data, &(defaultValue_[0]), size_);
	}

	void StructPrototype::Finalize(void * data) const
	{
		/*
		for (unsigned int i = 0; i < objectFields_.size(); ++i)
		{
			unsigned int fieldIndex = objectFields_[i];
			const Field & field = *(fields_[fieldIndex]);
			field.Type()->Finalize(static_cast<char *>(raw) + field.Offset());
		}
		*/
	}
}
}

