#ifndef XTL_PLAIN__RECORD_PROTOTYPE_HPP__
#define XTL_PLAIN__RECORD_PROTOTYPE_HPP__ 1

#include <string>
#include <vector>

#include "Field.hpp"
#include "../SharedPtr.hpp"
#include "../utils/AutoPtrVector.hpp"

namespace XTL
{
namespace PLAIN
{
	class FieldType;

	class RecordPrototype
	{
		public:

			typedef SharedPtr<const RecordPrototype> SharedConstPtr;

			explicit RecordPrototype(const std::string & name);

			const std::string & Name() const
			{
				return name_;
			}

			unsigned int Size() const
			{
				return size_;
			}

			unsigned int Alignment() const
			{
				return alignment_;
			}

			unsigned int FieldsCount() const
			{
				return fields_.Size();
			}

			void Initialize(void * data) const;

			const Field * GetField(unsigned int fieldIndex) const;

			unsigned int GetFieldIndex(const char * fieldName, unsigned int fieldSize) const;

		private:

			friend class RecordPrototypeBuilder;

			RecordPrototype(const RecordPrototype &);
			RecordPrototype & operator= (const RecordPrototype &);

			void AddField(const std::string & fieldName, SharedPtr<const FieldType> fieldType);

			const std::string name_;
			unsigned int size_;
			unsigned int alignment_;

			AutoPtrVector<Field> fields_;
			std::vector<char> defaultValue_;
	};
}
}

#endif

