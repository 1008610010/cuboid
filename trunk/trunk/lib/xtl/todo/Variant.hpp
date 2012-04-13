#ifndef XTL__VARIANT_HPP__
#define XTL__VARIANT_HPP__ 1

namespace XTL
{
	// TODO: realize using SharedPtr<VariantValue>

	class Variant
	{
		public:

			typedef XTL::INT_64 IntegerType;
			typedef double      FloatType;
	};

	class VariantValue
	{
		public:

			typedef IntegerType

			virtual ~VariantValue() throw () { ;; }

			virtual VariantValue * Clone() = 0;

			class Null;
			class Boolean;
			class Integer;
			class Float;
			class ConstString;
	};

	class VariantValue::Null : public VariantValue
	{
		public:

			static SharedPtr<VariantValue> Create()
			{
				static SharedPtr<VariantValue> instance(new Null());
				return instance;
			}

			virtual ShardPtr<VariantValue> Clone()
			{
				return Create();
			}

		private:

			Null() { ;; }
			Null(const Null &);
			Null & operator= (const Null &);
	};

	class VariantValue::Integer : public VariantValue
	{
	};

	class VariantValue::Float : public VariantValue
	{
	};

	class VariantValue::ConstString : public VariantValue
	{
	};



	class Variant
	{
		public:

			Variant()
				: value_(VariantValue::Null::Instance())
			{
				;;
			}

			Variant(const Variant & other)
				: value_(other.value_->Clone())
			{
				;;
			}

			~Variant() throw()
			{
				Destroy();
			}

			Variant & operator= (const Variant & other)
			{
				if (this != &other)
				{
					Destroy();
					value_ = other.value_->Clone();
				}
				return *this;
			}

			void Clear();
			Variant & operator= (bool value);
			Variant & operator= (char value);
			Variant & operator= (int value);
			Variant & operator= (long long int value);
			Variant & operator= (float value);
			Variant & operator= (double value);
			Variant & operator= (const char * value);
			Variant & operator= (const std::string & value);

			void ToInteger();
			void ToFloat();
			void ToString();

		private:

			void Destroy()
			{
				if (value_ != VariantValue::Null::Instance())
				{
					delete value_;
				}
			}

			VariantValue * value_;
	};
}

#endif

