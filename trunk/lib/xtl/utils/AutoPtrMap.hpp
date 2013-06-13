#ifndef XTL__AUTO_PTR_MAP_HPP__
#define XTL__AUTO_PTR_MAP_HPP__ 1

#include <map>
#include <memory>

namespace XTL
{
	template <typename KeyType_, typename ValueType_>
	class AutoPtrMap
	{
		public:

			typedef KeyType_   KeyType;
			typedef ValueType_ ValueType;

			typedef typename std::map<KeyType, ValueType *>::iterator       map_iterator;
			typedef typename std::map<KeyType, ValueType *>::const_iterator map_const_iterator;

			class ConstIterator
			{
				public:

					ConstIterator & operator++ ()
					{
						++itr_;
						return *this;
					}

					ConstIterator & operator++ (int)
					{
						ConstIterator temp(*this);
						++(*this);
						return temp;
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

					bool operator== (ConstIterator other) const
					{
						return itr_ == other.itr_;
					}

					bool operator!= (ConstIterator other) const
					{
						return itr_ != other.itr_;
					}

				private:

					friend class AutoPtrMap;
					friend class Iterator;

					explicit ConstIterator(map_const_iterator itr)
						: itr_(itr)
					{
						;;
					}

					map_const_iterator itr_;
			};

			class Iterator
			{
				public:

					operator ConstIterator () const
					{
						return ConstIterator(itr_);
					}

					Iterator & operator++ ()
					{
						++itr_;
						return *this;
					}

					Iterator & operator++ (int)
					{
						Iterator temp(*this);
						++(*this);
						return temp;
					}

					const KeyType & Key() const
					{
						return itr_->first;
					}

					ValueType * operator-> () const
					{
						return itr_->second;
					}

					ValueType * Value()
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

					bool operator== (Iterator other) const
					{
						return itr_ == other.itr_;
					}

					bool operator!= (Iterator other) const
					{
						return itr_ != other.itr_;
					}

				private:

					friend class AutoPtrMap;

					explicit Iterator(map_iterator itr)
						: itr_(itr)
					{
						;;
					}

					map_iterator itr_;
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
				const map_iterator end = map_.end();
				for (map_iterator itr = map_.begin(); itr != end; ++itr)
				{
					delete itr->second;
				}

				map_.clear();
			}

			Iterator Set(const KeyType & key, std::auto_ptr<ValueType> value)
			{
				map_iterator itr = map_.find(key);

				if (itr != map_.end())
				{
					delete itr->second;
				}
				else
				{
					itr = map_.insert(std::pair<KeyType, ValueType *>(key, 0)).first;
				}

				itr->second = value.release();

				return Iterator(itr);
			}

			ValueType * const operator[] (const KeyType & key) const
			{
				map_const_iterator itr = map_.find(key);

				return itr == map_.end() ? 0 : itr->second;
			}

			ConstIterator Begin() const
			{
				return ConstIterator(map_.begin());
			}

			ConstIterator End() const
			{
				return ConstIterator(map_.end());
			}

			Iterator Begin()
			{
				return Iterator(map_.begin());
			}

			Iterator End()
			{
				return Iterator(map_.end());
			}

			ConstIterator Find(const KeyType & key) const
			{
				return ConstIterator(map_.find(key));
			}

			Iterator Find(const KeyType & key)
			{
				return Iterator(map_.find(key));
			}

			void Erase(Iterator itr)
			{
				delete itr.itr_->second;
				map_.erase(itr.itr_);
			}

			bool Erase(const KeyType & key)
			{
				map_iterator itr = map_.find(key);

				if (itr == map_.end())
				{
					return false;
				}

				delete itr->second;
				map_.erase(itr);

				return true;
			}

		private:

			AutoPtrMap(const AutoPtrMap &);
			AutoPtrMap & operator= (const AutoPtrMap &);

			std::map<KeyType, ValueType *> map_;
	};
}

#endif

