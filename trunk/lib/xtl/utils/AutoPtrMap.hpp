#ifndef XTL__AUTO_PTR_MAP_HPP__
#define XTL__AUTO_PTR_MAP_HPP__ 1

#include <map>

namespace XTL
{
	template <typename KeyType_, typename ValueType_>
	class AutoPtrMap
	{
		public:

			typedef KeyType_   KeyType;
			typedef ValueType_ ValueType;

			AutoPtrMap()
				: map_()
			{
				;;
			}

			~AutoPtrMap() throw()
			{
				const typename std::map<KeyType, ValueType *>::iterator end = map_.end();
				for (typename std::map<KeyType, ValueType *>::iterator itr = map_.begin(); itr != end; ++itr)
				{
					delete itr->second;
				}
			}

			AutoPtrMap & Set(const KeyType & key, std::auto_ptr<ValueType> value)
			{
				typename std::map<KeyType, ValueType *>::iterator itr = map_.find(key);

				if (itr != map_.end())
				{
					delete itr->second;
					itr->second = value.release();
				}
				else
				{
					map_[key] = value.release();
				}

				return *this;
			}

			ValueType * operator[] (const KeyType & key) const
			{
				typename std::map<KeyType, ValueType *>::const_iterator itr = map_.find(key);

				return itr == map_.end() ? 0 : itr->second;
			}

		private:

			AutoPtrMap(const AutoPtrMap &);
			AutoPtrMap & operator= (const AutoPtrMap &);

			std::map<KeyType, ValueType *> map_;
	};
}

#endif

