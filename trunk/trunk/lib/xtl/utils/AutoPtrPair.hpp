#ifndef XTL__AUTO_PTR_PAIR_HPP__
#define XTL__AUTO_PTR_PAIR_HPP__ 1

#include <memory>

namespace XTL
{
	template <typename KeyType, typename ValueType>
	class AutoPtrPair
	{
		private:

			struct Reference
			{
				KeyType     key;
				ValueType * value;

				Reference(const KeyType & key_, ValueType * value_)
					: key(key_),
					  value(value_)
				{
					;;
				}
			};

		public:

			explicit AutoPtrPair(const KeyType & key) throw()
				: key_(key),
				  value_()
			{
				;;
			}

			AutoPtrPair(AutoPtrPair & other) throw()
				: key_(other.key_),
				  value_(other.value_)
			{
				;;
			}

			AutoPtrPair(const KeyType & key, std::auto_ptr<ValueType> value) throw()
				: key_(key),
				  value_(value)
			{
				;;
			}

			template <typename U>
			AutoPtrPair(const KeyType & key, std::auto_ptr<U> value) throw()
				: key_(key),
				  value_(value)
			{
				;;
			}

			const KeyType & Key() const
			{
				return key_;
			}

			std::auto_ptr<ValueType> ReleaseValue()
			{
				return value_;
			}

			AutoPtrPair & operator= (AutoPtrPair & other)
			{
				if (this != &other)
				{
					key_ = other.key_;
					value_ = other.value_;
				}

				return *this;
			}

			AutoPtrPair(Reference ref) throw()
				: key_(ref.key),
				  value_(ref.value)
			{
				;;
			}

			AutoPtrPair & operator= (Reference ref) throw()
			{
				key_ = ref.key;
				value_.reset(ref.value);

				return *this;
			}

			operator Reference() throw()
			{
				return Reference(key_, value_.release());
			}

		private:

			KeyType key_;
			std::auto_ptr<ValueType> value_;
	};

	template <typename KeyType, typename ValueType>
	AutoPtrPair<KeyType, ValueType> MakeAutoPtrPair(const KeyType & key, std::auto_ptr<ValueType> value)
	{
		return AutoPtrPair<KeyType, ValueType>(key, value);
	}
}

#endif

