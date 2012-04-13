#ifndef XTL_PLAIN__STRUCT_PROTOTYPE_BUILDER_HPP__
#define XTL_PLAIN__STRUCT_PROTOTYPE_BUILDER_HPP__ 1

#include <string>

#include "../SharedPtr.hpp"
#include "FieldType.hpp"

namespace XTL
{
namespace PLAIN
{
	class StructPrototype;

	class StructPrototypeBuilder
	{
		public:

			StructPrototypeBuilder(const std::string & prototypeName);

			SharedPtr<const StructPrototype> Finish();

			StructPrototypeBuilder & AddField_CHAR(const std::string & fieldName)
			{
				return AddField(fieldName, FieldType_CHAR::Instance());
			}

			StructPrototypeBuilder & AddField_INT_16(const std::string & fieldName)
			{
				return AddField(fieldName, FieldType_INT_16::Instance());
			}

			StructPrototypeBuilder & AddField_UINT_16(const std::string & fieldName)
			{
				return AddField(fieldName, FieldType_UINT_16::Instance());
			}

			StructPrototypeBuilder & AddField_INT_32(const std::string & fieldName)
			{
				return AddField(fieldName, FieldType_INT_32::Instance());
			}

			StructPrototypeBuilder & AddField_UINT_32(const std::string & fieldName)
			{
				return AddField(fieldName, FieldType_UINT_32::Instance());
			}

			StructPrototypeBuilder & AddField_INT_64(const std::string & fieldName)
			{
				return AddField(fieldName, FieldType_INT_64::Instance());
			}

			StructPrototypeBuilder & AddField_UINT_64(const std::string & fieldName)
			{
				return AddField(fieldName, FieldType_UINT_64::Instance());
			}

			StructPrototypeBuilder & AddField_FLOAT(const std::string & fieldName)
			{
				return AddField(fieldName, FieldType_FLOAT::Instance());
			}

			StructPrototypeBuilder & AddField_DOUBLE(const std::string & fieldName)
			{
				return AddField(fieldName, FieldType_DOUBLE::Instance());
			}

			StructPrototypeBuilder & AddField_CHARS(const std::string & fieldName, unsigned int capacity)
			{
				return AddField(fieldName, FieldType_CHARS::Create(capacity));
			}

		private:

			StructPrototypeBuilder & AddField(const std::string & fieldName, SharedPtr<const FieldType> fieldType);

			SharedPtr<StructPrototype> prototype_;
	};
}
}

#endif

