#ifndef XTL__AUTO_PTR_MAP_HPP__
#define XTL__AUTO_PTR_MAP_HPP__ 1

#include <map>

#include "AutoPtr.hpp"

namespace XTL
{
	template <typename KeyType_, typename ValueType_>
	class AutoPtrMap
	{
		public:

			typedef KeyType_   KeyType;
			typedef ValueType_ ValueType;

			typedef std::map<KeyType, ValueType *> Map;

			class const_iterator
			{
				public:

					const_iterator & operator++ ()
					{
						++itr_;
						return *this;
					}

					const_iterator & operator++ (int)
					{
						const_iterator temp(*this);
						++(*this);
						return temp;
					}

					const std::pair<KeyType, const ValueType *> operator* () const
					{
						return *itr_;
					}

					const KeyType & Key() const
					{
						return itr_->first;
					}

					const ValueType * Value() const
					{
						return itr_->second;
					}

					const ValueType * operator-> () const
					{
						return itr_->second;
					}

					bool operator== (const_iterator other) const
					{
						return itr_ == other.itr_;
					}

					bool operator!= (const_iterator other) const
					{
						return itr_ != other.itr_;
					}

				private:

					friend class AutoPtrMap;
					friend class iterator;

					explicit const_iterator(typename Map::const_iterator itr)
						: itr_(itr)
					{
						;;
					}

					typename Map::const_iterator itr_;
			};

			class iterator
			{
				public:

					operator const_iterator () const
					{
						return const_iterator(itr_);
					}

					iterator & operator++ ()
					{
						++itr_;
						return *this;
					}

					iterator & operator++ (int)
					{
						iterator temp(*this);
						++(*this);
						return temp;
					}

					std::pair<const KeyType &, ValueType * const> operator* () const
					{
						return std::pair<const KeyType &, ValueType * const>(itr_->first, itr_->second);
					}

					const KeyType & Key() const
					{
						return itr_->first;
					}

					const ValueType * Value() const
					{
						return itr_->second;
					}

					ValueType * Value()
					{
						return itr_->second;
					}

					ValueType * operator-> () const
					{
						return itr_->second;
					}

					std::auto_ptr<ValueType> ReleaseValue()
					{
						std::auto_ptr<ValueType> value(itr_->second);
						itr_->second = 0;
						return value;
					}

					void SetValue(std::auto_ptr<ValueType> value)
					{
						itr_->second = value;
					}

					bool operator== (iterator other) const
					{
						return itr_ == other.itr_;
					}

					bool operator!= (iterator other) const
					{
						return itr_ != other.itr_;
					}

				private:

					friend class AutoPtrMap;

					explicit iterator(typename Map::iterator itr)
						: itr_(itr)
					{
						;;
					}

					typename Map::iterator itr_;
			};

			AutoPtrMap()
				: map_()
			{
				;;
			}

			~AutoPtrMap() throw()
			{
				Clear();
			}

			unsigned int Size() const
			{
				return map_.size();
			}

			void Clear()
			{
				const typename Map::iterator end = map_.end();
				for (typename Map::iterator itr = map_.begin(); itr != end; ++itr)
				{
					delete itr->second;
				}

				map_.clear();
			}

			iterator Set(const KeyType & key, std::auto_ptr<ValueType> value)
			{
				typename Map::iterator itr = map_.find(key);

				if (itr != map_.end())
				{
					delete itr->second;
				}
				else
				{
					itr = map_.insert(std::pair<KeyType, ValueType *>(key, 0)).first;
				}

				itr->second = value.release();

				return iterator(itr);
			}

			ValueType * const operator[] (const KeyType & key) const
			{
				const typename Map::const_iterator itr = map_.find(key);

				return itr == map_.end() ? 0 : itr->second;
			}

			const_iterator begin() const
			{
				return const_iterator(map_.begin());
			}

			const_iterator end() const
			{
				return const_iterator(map_.end());
			}

			iterator begin()
			{
				return iterator(map_.begin());
			}

			iterator end()
			{
				return iterator(map_.end());
			}

			const_iterator Find(const KeyType & key) const
			{
				return const_iterator(map_.find(key));
			}

			iterator Find(const KeyType & key)
			{
				return iterator(map_.find(key));
			}

			template <typename Creator_>
			iterator FindOrCreate(const KeyType & key, const Creator_ & creator)
			{
				iterator itr = Find(key);
				if (itr == end())
				{
					itr = Set(key, creator());
				}

				return itr;
			}

			iterator Erase(iterator itr)
			{
				delete itr.itr_->second;
				map_.erase(itr.itr_++);
				return itr;
			}

			bool Erase(const KeyType & key)
			{
				typename Map::iterator itr = map_.find(key);

				if (itr == map_.end())
				{
					return false;
				}

				delete itr->second;
				map_.erase(itr);

				return true;
			}

			template <typename Func_>
			void ForEach(Func_ func) const
			{
				const typename Map::const_iterator end = map_.end();
				for (typename Map::const_iterator itr = map_.begin(); itr != end; ++itr)
				{
					func(itr->first, itr->second);
				}
			}

		private:

			AutoPtrMap(const AutoPtrMap &);
			AutoPtrMap & operator= (const AutoPtrMap &);

			Map map_;
	};
}

#endif

