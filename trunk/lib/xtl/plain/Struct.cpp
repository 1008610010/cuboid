#include "Struct.hpp"

#include "StructPrototype.hpp"

namespace XTL
{
namespace PLAIN
{
	Struct::Struct(SharedPtr<const StructPrototype> prototype)
		: prototype_(prototype),
		  data_(new char[prototype->Size()]) // TODO: create StructDataAllocator, that allocate aligned by prototype->Alignment() memory blocks
	{
		prototype_->Initialize(data_);
	}

	Struct::~Struct() throw()
	{
		prototype_->Finalize(data_);
		delete [] data_;
	}

	void * Struct::FieldData(unsigned int fieldIndex)
	{
		return prototype_->GetField(fieldIndex)->Translate(data_);
	}

	const void * Struct::FieldData(unsigned int fieldIndex) const
	{
		return prototype_->GetField(fieldIndex)->Translate(data_);
	}

	void Struct::Read(InputStream & stream)
	{
		stream.Read(data_, prototype_->Size());
	}

	void Struct::Write(OutputStream & stream) const
	{
		stream.Write(data_, prototype_->Size());
	}
}
}

