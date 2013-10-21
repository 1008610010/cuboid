#ifndef XTL_PLAIN__FIELD_TYPE_HPP__
#define XTL_PLAIN__FIELD_TYPE_HPP__ 1

#include "../SharedPtr.hpp"
#include "../Types.hpp"

namespace XTL
{
namespace PLAIN
{
	class FieldType
	{
		public:

			virtual ~FieldType() throw() { ;; }

			virtual unsigned int Size() const = 0;

			virtual unsigned int Alignment() const = 0;
	};

	template <typename F, typename T>
	class FieldType_Simple : public FieldType
	{
		public:

			static SharedPtr<const F> Instance()
			{
				static SharedPtr<const F> instance(new F());

				return instance;
			};

			virtual ~FieldType_Simple() throw()
			{
				;;
			}

			virtual unsigned int Size() const
			{
				return sizeof(T);
			}

			virtual unsigned int Alignment() const
			{
				return sizeof(T);
			}

		protected:

			FieldType_Simple()
			{
				;;
			}

		private:

			FieldType_Simple(const FieldType_Simple &);
			FieldType_Simple & operator= (const FieldType_Simple &);
	};

	class FieldType_CHAR : public FieldType_Simple<FieldType_CHAR, char>
	{
	};

	class FieldType_INT_8 : public FieldType_Simple<FieldType_INT_8, INT_8>
	{
	};

	class FieldType_UINT_8 : public FieldType_Simple<FieldType_UINT_8, UINT_8>
	{
	};

	class FieldType_INT_16 : public FieldType_Simple<FieldType_INT_16, INT_16>
	{
	};

	class FieldType_UINT_16 : public FieldType_Simple<FieldType_UINT_16, UINT_16>
	{
	};

	class FieldType_INT_32 : public FieldType_Simple<FieldType_INT_32, INT_32>
	{
	};

	class FieldType_UINT_32 : public FieldType_Simple<FieldType_UINT_32, UINT_32>
	{
	};

	class FieldType_INT_64 : public FieldType_Simple<FieldType_INT_64, INT_64>
	{
	};

	class FieldType_UINT_64 : public FieldType_Simple<FieldType_UINT_64, UINT_64>
	{
	};

	class FieldType_FLOAT : public FieldType_Simple<FieldType_FLOAT, float>
	{
	};

	class FieldType_DOUBLE : public FieldType_Simple<FieldType_DOUBLE, double>
	{
	};

	class FieldType_CHARS : public FieldType
	{
		public:

			static SharedPtr<const FieldType_CHARS> Create(unsigned int capacity)
			{
				return SharedPtr<const FieldType_CHARS>(new FieldType_CHARS(capacity));
			}

			virtual ~FieldType_CHARS() throw() { ;; }

			virtual unsigned int Size() const
			{
				return capacity_ * sizeof(char);
			}

			virtual unsigned int Alignment() const
			{
				return sizeof(char);
			}

		private:

			explicit FieldType_CHARS(unsigned int capacity)
				: capacity_(capacity)
			{
				;;
			}

			FieldType_CHARS(const FieldType_CHARS &);
			FieldType_CHARS & operator= (const FieldType_CHARS &);

			const unsigned int capacity_;
	};

	template <typename RecordType_>
	class FieldType_STRUCT : public FieldType
	{
		public:

			static SharedPtr<FieldType_STRUCT> Instance()
			{
				static SharedPtr<const FieldType_STRUCT> instance(new FieldType_STRUCT());
				return instance;
			};

			virtual ~FieldType_STRUCT() throw()
			{
				;;
			}

			virtual unsigned int Size() const
			{
				return RecordType_::Prototype()->Size();
			}

			virtual unsigned int Alignment() const
			{
				return RecordType_::Prototype()->Alignment();
			}

		private:

			FieldType_STRUCT()
			{
				;;
			}

			FieldType_STRUCT(const FieldType_STRUCT &);
			FieldType_STRUCT & operator= (const FieldType_STRUCT &);
	};

	template <typename RecordType_>
	class FieldType_STRUCT_TUPLE : public FieldType
	{
		public:

			static SharedPtr<const FieldType_STRUCT_TUPLE<RecordType_> > Create(unsigned int capacity)
			{
				return SharedPtr<const FieldType_STRUCT_TUPLE<RecordType_> >(new FieldType_STRUCT_TUPLE<RecordType_>(capacity));
			}

			virtual unsigned int Size() const
			{
				return size_;
			}

			virtual unsigned int Alignment() const
			{
				return RecordType_::Prototype()->Alignment();
			}

		private:

			explicit FieldType_STRUCT_TUPLE(unsigned int capacity)
				: capacity_(capacity),
				  size_(capacity_ * RecordType_::Prototype()->AlignedSize())
			{
				;;
			}

			FieldType_STRUCT_TUPLE(const FieldType_STRUCT_TUPLE &);
			FieldType_STRUCT_TUPLE & operator= (const FieldType_STRUCT_TUPLE &);

			const unsigned int capacity_;
			const unsigned int size_;
	};
}
}

#endif

