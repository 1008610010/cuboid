#include "SimpleTemplateWriter.hpp"
#include "SimpleTemplateParameter.hpp"
#include "SimpleTemplateValueArray.hpp"
#include "../io/OutputStream.hpp"

namespace XTL
{
	SimpleTemplateOutputStreamWriter::SimpleTemplateOutputStreamWriter(OutputStream & outputStream, const SimpleTemplateValueArray & values)
		: outputStream_(outputStream),
		  values_(values)
	{
		;;
	}

	void SimpleTemplateOutputStreamWriter::Visit(const SimpleTemplateItemText & item)
	{
		outputStream_.Write(item.Chars(), item.Size());
	}

	void SimpleTemplateOutputStreamWriter::Visit(const SimpleTemplateItemParameter & item)
	{
		unsigned int paramIndex = item.GetParameter().GetIndex();
		if (paramIndex >= values_.Size()) {
			// TODO: throw exception here or write UNDEFINED value to output stream
			return;
		}

		const SimpleTemplateValue * value = values_[paramIndex];
		if (value == 0) {
			// TODO: throw exception here or write UNDEFINED value to output stream
			return;
		}

		outputStream_.Write(value->Chars(), value->Size());
	}
}

