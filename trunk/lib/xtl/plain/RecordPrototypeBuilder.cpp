#include "RecordPrototypeBuilder.hpp"

#include <stdio.h>

#include "../Logger.hpp"
#include "RecordPrototype.hpp"

namespace XTL
{
namespace PLAIN
{
	RecordPrototypeBuilder::RecordPrototypeBuilder(const std::string & prototypeName)
		: prototype_(new RecordPrototype(prototypeName))
	{
		;;
	}

	SharedPtr<const RecordPrototype> RecordPrototypeBuilder::Finish()
	{
		if (prototype_.IsNull())
		{
			Warn("Calling RecordPrototypeBuilder::Finish() method on already released prototype. No effect.\n");
			return prototype_;
		}

		if (prototype_->Size() % prototype_->Alignment())
		{
			Warn("Record prototype '%s' has size (%u), that is not a multiple of its alignment (%u)", prototype_->Name(), prototype_->Size(), prototype_->Alignment());
		}

		SharedPtr<const RecordPrototype> temp(prototype_);
		prototype_.Release();
		return temp;
	}

	RecordPrototypeBuilder & RecordPrototypeBuilder::AddField(const std::string & fieldName, SharedPtr<const FieldType> fieldType)
	{
		if (prototype_.IsNull())
		{
			Warn("Adding field '%s' to already released prototype. No effect.\n", fieldName);
			return *this;
		}

		prototype_->AddField(fieldName, fieldType);
		return *this;
	}
}
}
