#ifndef XTL__PLAIN__RECORD_PROTOTYPE_BUILDER_HPP__
#define XTL__PLAIN__RECORD_PROTOTYPE_BUILDER_HPP__ 1

#include <string>

#include "../SharedPtr.hpp"
#include "FieldType.hpp"

namespace XTL
{
namespace PLAIN
{
	class RecordPrototype;

	class RecordPrototypeBuilder
	{
		public:

			RecordPrototypeBuilder(const std::string & prototypeName);

			SharedPtr<const RecordPrototype> Finish();

			RecordPrototypeBuilder & AddField_CHAR(const std::string & fieldName)
			{
				return AddField(fieldName, FieldType_CHAR::Instance());
			}

			RecordPrototypeBuilder & AddField_INT_8(const std::string & fieldName)
			{
				return AddField(fieldName, FieldType_INT_8::Instance());
			}

			RecordPrototypeBuilder & AddField_UINT_8(const std::string & fieldName)
			{
				return AddField(fieldName, FieldType_UINT_8::Instance());
			}

			RecordPrototypeBuilder & AddField_INT_16(const std::string & fieldName)
			{
				return AddField(fieldName, FieldType_INT_16::Instance());
			}

			RecordPrototypeBuilder & AddField_UINT_16(const std::string & fieldName)
			{
				return AddField(fieldName, FieldType_UINT_16::Instance());
			}

			RecordPrototypeBuilder & AddField_INT_32(const std::string & fieldName)
			{
				return AddField(fieldName, FieldType_INT_32::Instance());
			}

			RecordPrototypeBuilder & AddField_UINT_32(const std::string & fieldName)
			{
				return AddField(fieldName, FieldType_UINT_32::Instance());
			}

			RecordPrototypeBuilder & AddField_INT_64(const std::string & fieldName)
			{
				return AddField(fieldName, FieldType_INT_64::Instance());
			}

			RecordPrototypeBuilder & AddField_UINT_64(const std::string & fieldName)
			{
				return AddField(fieldName, FieldType_UINT_64::Instance());
			}

			RecordPrototypeBuilder & AddField_FLOAT(const std::string & fieldName)
			{
				return AddField(fieldName, FieldType_FLOAT::Instance());
			}

			RecordPrototypeBuilder & AddField_DOUBLE(const std::string & fieldName)
			{
				return AddField(fieldName, FieldType_DOUBLE::Instance());
			}

			RecordPrototypeBuilder & AddField_CHARS(const std::string & fieldName, unsigned int capacity)
			{
				return AddField(fieldName, FieldType_CHARS::Create(capacity));
			}

			template <typename RecordType_>
			RecordPrototypeBuilder & AddField_STRUCT(const std::string & fieldName)
			{
				return AddField(fieldName, FieldType_STRUCT<RecordType_>::Instance());
			}

			template <typename RecordType_>
			RecordPrototypeBuilder & AddField_STRUCT_TUPLE(const std::string & fieldName, unsigned int capacity)
			{
				return AddField(fieldName, FieldType_STRUCT_TUPLE<RecordType_>::Create(capacity));
			}

		private:

			RecordPrototypeBuilder & AddField(const std::string & fieldName, SharedPtr<const FieldType> fieldType);

			SharedPtr<RecordPrototype> prototype_;
	};
}
}

#endif

