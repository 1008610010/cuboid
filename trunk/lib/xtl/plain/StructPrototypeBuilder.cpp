#include "StructPrototypeBuilder.hpp"

#include "StructPrototype.hpp"

namespace XTL
{
namespace PLAIN
{
	StructPrototypeBuilder::StructPrototypeBuilder(const std::string & prototypeName)
		: prototype_(new StructPrototype(prototypeName))
	{
		;;
	}

	SharedPtr<const StructPrototype> StructPrototypeBuilder::Finish()
	{
		if (prototype_.IsNull())
		{
			// TODO: throw exception here
			return prototype_;
		}

		prototype_->AdjustSize(prototype_->Alignment());

		SharedPtr<const StructPrototype> temp(prototype_);
		prototype_.Release();
		return temp;
	}

	StructPrototypeBuilder & StructPrototypeBuilder::AddField(const std::string & fieldName, SharedPtr<const FieldType> fieldType)
	{
		if (prototype_.IsNull())
		{
			// TODO: throw exception here
			return *this;
		}

		prototype_->AddField(fieldName, fieldType);
		return *this;
	}
}
}

