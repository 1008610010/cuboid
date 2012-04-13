#ifndef XTL__VARIANT_HPP__
#define XTL__VARIANT_HPP__ 1

#include <string>

#include "Types.hpp"

namespace XTL
{
	class VariantValue;

	class VariantValuePtr
	{
		public:

			VariantValuePtr();

			explicit VariantValuePtr(VariantValue * ptr);

			VariantValuePtr(const VariantValuePtr & other);

			~VariantValuePtr() throw();

			VariantValuePtr & operator= (const VariantValuePtr & other);

			bool IsNull() const                 { return ptr_ == 0; }

			VariantValue * Get()                { return ptr_; }

			VariantValue * Get() const          { return ptr_; }

			VariantValue * operator-> ()        { return ptr_; }

			VariantValue * operator-> () const  { return ptr_; }

			VariantValue & operator* ()         { return *ptr_; }

			VariantValue & operator* () const   { return *ptr_; }

		private:

			void AddRef();

			void Release();

			VariantValue * ptr_;
	};

	class Variant
	{
		public:

			typedef XTL::INT_64 IntegerType;
			typedef double      FloatType;

			Variant();

			Variant(const Variant & other);

			Variant & operator= (const Variant & other);

			Variant & operator= (IntegerType & value);

			bool IsNull() const;

			void SetNull();

		private:

			VariantValuePtr valuePtr_;
	};

	class VariantValue
	{
		public:

			virtual ~VariantValue() throw() { ;; }

			virtual VariantValuePtr Clone() const = 0;

			virtual bool IsNull() const
			{
				return false;
			}

			virtual VariantValuePtr ToInteger() const;

			virtual VariantValuePtr ToFloat()   const;

			virtual VariantValuePtr ToString()  const;

			virtual VariantValuePtr Assign(Variant::IntegerType data);

			virtual VariantValuePtr Assign(Variant::FloatType data);

			virtual VariantValuePtr Assign(const std::string & data);

		protected:

			VariantValue()
				: refCount_(0)
			{
				;;
			}

			unsigned int RefCount() const
			{
				return refCount_;
			}

		private:

			friend class VariantValuePtr;

			VariantValue(const VariantValue &);
			VariantValue & operator= (const VariantValue &);

			void AddRef()
			{
				++refCount_;
			}

			bool Release()
			{
				return --refCount_ == 0;
			}

			unsigned int refCount_;
	};

	class VariantValue_Null : public VariantValue
	{
		public:

			virtual ~VariantValue_Null() throw() { ;; }

			static VariantValuePtr Create();

			virtual VariantValuePtr Clone() const;

			virtual bool IsNull() const;

		private:

			VariantValue_Null()
				: VariantValue() { ;; }
	};

	class VariantValue_Integer : public VariantValue
	{
		public:

			virtual ~VariantValue_Integer() throw() { ;; }

			static VariantValuePtr Create(Variant::IntegerType data);

			virtual VariantValuePtr Clone() const;

			virtual VariantValuePtr ToInteger();

			virtual VariantValuePtr ToFloat();

			virtual VariantValuePtr ToString();

			virtual VariantValuePtr Assign(Variant::IntegerType data);

		private:

			explicit VariantValue_Integer(Variant::IntegerType data)
				: VariantValue(),
				  data_(data)
			{
				;;
			}

			Variant::IntegerType data_;
	};

	class VariantValue_Float : public VariantValue
	{
		public:

			virtual ~VariantValue_Float() throw() { ;; }

			static VariantValuePtr Create(Variant::FloatType data);

			virtual VariantValuePtr Clone() const;

			virtual VariantValuePtr ToInteger();

			virtual VariantValuePtr ToFloat();

			virtual VariantValuePtr ToString();

			virtual VariantValuePtr Assign(Variant::IntegerType data);


		private:

			explicit VariantValue_Float(Variant::FloatType data)
				: VariantValue(),
				  data_(data)
			{
				;;
			}

			Variant::FloatType data_;
	};

	class VariantValue_String : public VariantValue
	{
		public:

			virtual ~VariantValue_String() throw() { ;; }

			static VariantValuePtr Create(const std::string & data)
			{
				return VariantValuePtr(new VariantValue_String(data));
			}

			virtual VariantValuePtr Clone() const
			{
				return Create(data_);
			}

		private:

			explicit VariantValue_String(const std::string & data)
				: VariantValue(),
				  data_(data)
			{
				;;
			}

			std::string data_;
	};
}

#endif

