#ifndef XTL__CACHE_MAP_HPP__
#define XTL__CACHE_MAP_HPP__

#include <list>
#include <map>
#include <memory>

namespace XTL
{
	/**
	 * Реализация вытесняющего кэша. Если размер кэша достигает capacity элементов,
	 * то при вставке нового элемента "самый старый" будет удален.
	 * Вызов метода operator[](KEY) делает элемент с этим ключом "самым новым".
	 */
	template <typename Key, typename T>
	class CacheMap
	{
		struct ListEntry;
		struct MapEntry;

		typedef std::list<ListEntry> List;
		typedef std::map<Key, MapEntry> Map;

		struct ListEntry
		{
			typename Map::iterator itrInMap;

			ListEntry(typename Map::iterator itr)
				: itrInMap(itr) { ;; }
		};

		struct MapEntry
		{
			T value;
			typename List::iterator itrInList;
		};

		public:

			explicit CacheMap(unsigned int capacity)
				: capacity_(capacity),
				  map_(),
				  list_()
			{
				;;
			}

			unsigned int Capacity() const
			{
				return capacity_;
			}

			unsigned int Size() const
			{
				return map_.size();
			}

			void Clear()
			{
				map_.clear();
				list_.clear();
			}

			T & operator[] (const Key & key)
			{
				typename Map::iterator mi = map_.find(key);

				if (mi != map_.end())
				{
					list_.erase(mi->second.itrInList);
				}
				else
				{
					if (map_.size() >= capacity_)
					{
						typename Map::iterator deleting = list_.back().itrInMap;
						list_.erase(deleting->second.itrInList);
						map_.erase(deleting);
					}

					mi = map_.insert(std::make_pair(key, MapEntry())).first;
				}

				list_.push_front(mi);
				mi->second.itrInList = list_.begin();

				return mi->second.value;
			}

		private:

			unsigned int capacity_;
			Map          map_;
			List         list_;
	};

	template <typename Key, typename T>
	class CachePtrMap
	{
		struct ListEntry;
		struct MapEntry;

		typedef std::list<ListEntry> List;
		typedef std::map<Key, MapEntry> Map;

		struct ListEntry
		{
			typename Map::iterator itrInMap;

			ListEntry(typename Map::iterator itr)
				: itrInMap(itr) { ;; }
		};

		struct MapEntry
		{
			T * value;
			typename List::iterator itrInList;
		};

		public:

			explicit CachePtrMap(unsigned int capacity)
				: capacity_(capacity),
				  map_(),
				  list_()
			{
				;;
			}

			~CachePtrMap() throw()
			{
				Clear();
			}

			unsigned int Capacity() const
			{
				return capacity_;
			}

			unsigned int Size() const
			{
				return map_.size();
			}

			void Clear()
			{
				const typename Map::iterator end = map_.end();
				for (typename Map::iterator itr = map_.begin(); itr != map_.end(); ++itr)
				{
					delete itr->second.value;
				}

				map_.clear();
				list_.clear();
			}

			T * Find(const Key & key)
			{
				typename Map::iterator mi = map_.find(key);

				if (mi == map_.end())
				{
					return 0;
				}

				Touch(mi);

				return mi->second.value;
			}

			T * Push(const Key & key, std::auto_ptr<T> value)
			{
				typename Map::iterator mi = map_.find(key);

				if (mi == map_.end())
				{
					if (map_.size() >= capacity_)
					{
						DeleteOldest();
					}

					MapEntry entry;
					entry.value = 0;
					entry.itrInList = list_.end();

					mi = map_.insert(std::make_pair(key, entry)).first;
				}

				delete mi->second.value;
				mi->second.value = value.release();

				Touch(mi);

				return mi->second.value;
			}

		private:

			void DeleteOldest()
			{
				typename Map::iterator deleting = list_.back().itrInMap;

				delete deleting->second.value;

				list_.erase(deleting->second.itrInList);
				map_.erase(deleting);
			}

			void Touch(typename Map::iterator mi)
			{
				if (mi->second.itrInList != list_.end())
				{
					list_.erase(mi->second.itrInList);
				}

				list_.push_front(mi);
				mi->second.itrInList = list_.begin();
			}

			unsigned int capacity_;
			Map          map_;
			List         list_;
	};
}

#endif
