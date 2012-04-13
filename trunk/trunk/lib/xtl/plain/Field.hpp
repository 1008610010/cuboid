#ifndef XTL_PLAIN__FIELD_HPP__
#define XTL_PLAIN__FIELD_HPP__ 1

#include <string>

#include "../SharedPtr.hpp"
#include "FieldType.hpp"

namespace XTL
{
namespace PLAIN
{
	class Field
	{
		public:

			Field(const std::string & name, SharedPtr<const FieldType> type, unsigned int offset)
				: name_(name),
				  type_(type),
				  offset_(offset)
			{
				;;
			}

			const std::string Name() const
			{
				return name_;
			}

			unsigned int Offset() const
			{
				return offset_;
			}

			const FieldType * Type() const
			{
				return type_.Get();
			}

			unsigned int Size() const
			{
				return type_->Size();
			}

			void * Translate(void * ptr) const
			{
				return static_cast<char *>(ptr) + offset_;
			}

			const void * Translate(const void * ptr) const
			{
				return static_cast<const char *>(ptr) + offset_;
			}

		private:

			Field(const Field &);
			Field & operator= (const Field &);

			const std::string name_;
			const SharedPtr<const FieldType> type_;
			const unsigned int offset_;
	};
}
}

#endif

