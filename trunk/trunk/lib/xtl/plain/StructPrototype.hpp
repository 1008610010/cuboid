#ifndef XTL_PLAIN__STRUCT_PROTOTYPE_HPP__
#define XTL_PLAIN__STRUCT_PROTOTYPE_HPP__ 1

#include "../SharedPtr.hpp"
#include "../FormatString.hpp"
#include "../utils/AutoPtrVector.hpp"
#include "Field.hpp"

#include <stdexcept>
#include <string>

namespace XTL
{
namespace PLAIN
{
	class FieldType;

	class StructPrototype
	{
		public:

			typedef SharedPtr<const StructPrototype> SharedConstPtr;

			explicit StructPrototype(const std::string & name);

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

			void Finalize(void * data) const;

			const Field * GetField(unsigned int fieldIndex) const
			{
				return fields_[fieldIndex];
			}

			unsigned int GetFieldIndex(const char * fieldName, unsigned int fieldSize) const
			{
				for (unsigned int i = 0; i < fields_.Size(); ++i)
				{
					if (fields_[i]->Name() == fieldName)
					{
						if (fields_[i]->Size() != fieldSize)
						{
							throw std::runtime_error(FormatString("Prototype '%s' has field '%s' with size %u, but requested size is %u", name_.c_str(), fieldName, fields_[i]->Size(), fieldSize));
						}

						return i;
					}
				}

				throw std::runtime_error(FormatString("Prototype '%s' has not field '%s'", name_.c_str(), fieldName));
			}

		private:

			friend class StructPrototypeBuilder;

			StructPrototype(const StructPrototype &);
			StructPrototype & operator= (const StructPrototype &);

			void AdjustSize(unsigned int alignment);

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

