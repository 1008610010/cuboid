#include "RecordPrototype.hpp"

#include <stdio.h>
#include <string.h>

#include <stdexcept>

#include "../Logger.hpp"
#include "Field.hpp"
#include "FieldType.hpp"

namespace XTL
{
namespace PLAIN
{
	RecordPrototype::RecordPrototype(const std::string & name)
		: name_(name),
		  size_(0),
		  alignment_(1),
		  fields_(),
		  defaultValue_()
	{
		;;
	}

	void RecordPrototype::AddField(const std::string & fieldName, SharedPtr<const FieldType> fieldType)
	{
		if (size_ % fieldType->Alignment() != 0)
		{
			Warn(
				"Adding field '%s' with alignment %u to record prototype '%s' at not divisible offset %u\n",
				fieldName,
				fieldType->Alignment(),
				name_,
				size_
			);
		}
		else if (alignment_ < fieldType->Alignment())
		{
			alignment_ = fieldType->Alignment();
		}

		std::auto_ptr<Field> newField(new Field(fieldName, fieldType, size_));

		fields_.PushBack(newField);

		size_ += fieldType->Size();
		defaultValue_.resize(size_, '\0');
	}

	void RecordPrototype::Initialize(void * data) const
	{
		::memcpy(data, &(defaultValue_[0]), size_);
	}

	const Field * RecordPrototype::GetField(unsigned int fieldIndex) const
	{
		return fields_[fieldIndex];
	}

	unsigned int RecordPrototype::GetFieldIndex(const char * fieldName, unsigned int fieldSize) const
	{
		for (unsigned int i = 0; i < fields_.Size(); ++i)
		{
			if (fields_[i]->Name() == fieldName)
			{
				if (fields_[i]->Size() != fieldSize)
				{
					throw std::runtime_error(FormatString("RecordPrototype '%s' has field '%s' with size %u, but requested size is %u", name_.c_str(), fieldName, fields_[i]->Size(), fieldSize));
				}

				return i;
			}
		}

		throw std::runtime_error(FormatString("RecordPrototype '%s' has not field '%s'", name_.c_str(), fieldName));
	}
}
}

