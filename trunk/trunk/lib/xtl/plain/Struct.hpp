#ifndef XTL_PLAIN__STRUCT_HPP__
#define XTL_PLAIN__STRUCT_HPP__ 1

#include "../SharedPtr.hpp"
#include "../io/InputStream.hpp"
#include "../io/OutputStream.hpp"

namespace XTL
{
namespace PLAIN
{
	class StructPrototype;

	class Struct
	{
		public:

			Struct(SharedPtr<const StructPrototype> prototype);

			~Struct() throw();

			template <typename T>
			T & TypedField(unsigned int fieldIndex)
			{
				return *static_cast<T *>(FieldData(fieldIndex));
			}

			template <typename T>
			const T & TypedField(unsigned int fieldIndex) const
			{
				return *static_cast<const T *>(FieldData(fieldIndex));
			}

			void Read(InputStream & stream);

			void Write(OutputStream & stream) const;

			/*
			operator StructRef ()
			{
				return StructRef(prototype_, data_);
			}

			operator StructConstRef () const
			{
				return StructConstRef(prototype, data_);
			}
			*/

		protected:

			const StructPrototype * Prototype() const
			{
				return prototype_.Get();
			}

			char * Data()
			{
				return data_;
			}

			const char * Data() const
			{
				return data_;
			}

		private:

			Struct(const Struct &);
			Struct & operator= (const Struct &);

			void * FieldData(unsigned int fieldIndex);

			const void * FieldData(unsigned int fieldIndex) const;

			SharedPtr<const StructPrototype> prototype_;
			char * const data_;
	};
}
}

#endif

