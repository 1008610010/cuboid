#ifndef XTL__AUTO_PTR_MAP_CACHE_HPP__
#define XTL__AUTO_PTR_MAP_CACHE_HPP__ 1

#include <list>
#include <map>

namespace XTL
{
	template <typename ValueType_>
	class AutoPtrRef
	{
		typedef ValueType_   ValueType;
		typedef ValueType_ * ValueTypePtr;

		public:

			explicit AutoPtrRef(ValueTypePtr & ptrRef)
				: ptrRef_(ptrRef)
			{
				;;
			}

			AutoPtrRef & operator= (ValueType * ptr)
			{
				if (ptrRef_ == ptr)
				{
					return *this;
				}

				if (ptrRef_ != 0)
				{
					delete ptrRef_;
				}

				ptrRef_ = ptr;

				return *this;
			}

			operator ValueType * ()
			{
				return ptrRef_;
			}

			ValueType * operator->()
			{
				return ptrRef_;
			}

			template <typename ValueCreator_>
			AutoPtrRef & SetIfNull(ValueCreator_ creator)
			{
				if (ptrRef_ == 0)
				{
					ptrRef_ = creator();
				}

				return *this;
			}

		private:

			ValueTypePtr & ptrRef_;
	};

	/*
	 * @class AutoPtrMapCache
	 * @brief Класс, представляющий ограниченные по размеру кэш с методом вытеснения Least Recently Used.
	 */
	template <typename KeyType_, typename ValueType_>
	class AutoPtrMapCache
	{
		public:

			typedef KeyType_   KeyType;
			typedef ValueType_ ValueType;

		private:

			struct MapEntry;
			struct ListEntry;

			typedef std::list<ListEntry> List;
			typedef std::map<KeyType, MapEntry> Map;

			struct ListEntry
			{
				typename Map::iterator mapItr;

				explicit ListEntry(typename Map::iterator itr)
					: mapItr(itr)
				{
					;;
				}
			};

			struct MapEntry
			{
				ValueType * value;
				typename List::iterator listItr;

				MapEntry()
					: value(0),
					  listItr()
				{
					;;
				}
			};

		public:

			explicit AutoPtrMapCache(int capacity)
				: capacity_(capacity),
				  size_(0),
				  map_(),
				  list_()
			{
				;;
			}

			~AutoPtrMapCache() throw()
			{
				Clear();
			}

			unsigned int Size() const
			{
				return size_;
			}

			AutoPtrRef<ValueType> Get(const KeyType & key)
			{
				typename Map::iterator mapItr = map_.find(key);
				if (mapItr != map_.end())
				{
					list_.erase(mapItr->second.listItr);
				}
				else
				{
					if (size_ > 0 && size_ == capacity_)
					{
						DiscardLeastRecentlyUsed();
					}

					mapItr = map_.insert(std::make_pair(key, MapEntry())).first;
					++size_;
				}

				list_.push_front(ListEntry(mapItr));
				mapItr->second.listItr = list_.begin();

				return AutoPtrRef<ValueType>(mapItr->second.value);
			}

			void Clear()
			{
				for (typename Map::iterator mapItr = map_.begin(); mapItr != map_.end(); ++mapItr)
				{
					delete mapItr->second.value;
				}
				size_ = 0;
				map_.clear();
				list_.clear();
			}

		private:

			AutoPtrMapCache(const AutoPtrMapCache &);
			AutoPtrMapCache & operator= (const AutoPtrMapCache &);

			void DiscardLeastRecentlyUsed()
			{
				const typename Map::iterator mapItr = list_.back().mapItr;
				list_.erase(mapItr->second.listItr);
				delete mapItr->second.value;
				map_.erase(mapItr);
				--size_;
			}

			const unsigned int capacity_;
			unsigned int       size_;
			Map                map_;
			List               list_;
	};
}

#endif
