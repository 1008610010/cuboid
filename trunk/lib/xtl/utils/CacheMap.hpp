#ifndef XTL__CACHE_MAP_HPP__
#define XTL__CACHE_MAP_HPP__

#include <list>
#include <map>

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
}

#endif
