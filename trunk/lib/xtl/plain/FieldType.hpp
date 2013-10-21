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

			unsigned int capacity_;
	};

	class FieldType_STRUCT : public FieldType
	{
		public:



		private:

			FieldType_STRUCT(const FieldType_STRUCT &);
			FieldType_STRUCT & operator= (const FieldType_STRUCT &);
	};
}
}

#endif

