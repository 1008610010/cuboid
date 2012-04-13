#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <deque>
#include <list>
#include <map>

template <typename _DataType, int BUCKET_CAPACITY> class Bucket
{
	public:

		typedef std::list<_DataType> ListOfOverflows;

		Bucket()
			: size_(0),
			  slots_(static_cast<_DataType*>(malloc(BUCKET_CAPACITY * sizeof(_DataType)))),
			  overflows_()
		{
			;;
		}

		~Bucket() throw()
		{
			free(slots_);
		}

		int Size() const
		{
			return size_;
		}

		void SetSize(int size)
		{
			size_ = size;
		}

		bool Full() const
		{
			return size_ == BUCKET_CAPACITY;
		}

		void Add(const _DataType & data)
		{
			if (Full())
			{
				overflows_.push_back(data);
			}
			else
			{
				slots_[size_++] = data;
			}
		}

		ListOfOverflows & Overflows()
		{
			return overflows_;
		}

		int Search(const _DataType & data) const
		{
			for (int i = 0; i < size_; ++i)
			{
				if (slots_[i] == data)
				{
					return i;
				}
			}
			return -1;
		}

		_DataType & operator[] (int index)
		{
			return slots_[index];
		}
		
	protected:

		int size_;
		_DataType * slots_;
		ListOfOverflows overflows_;
};

template <typename _DataType, int BUCKET_CAPACITY> class LinearHash
{
	typedef Bucket<_DataType, BUCKET_CAPACITY> BucketType;
	typedef typename BucketType::ListOfOverflows ListOfOverflows;
	typedef std::deque<BucketType*> VectorOfBuckets;
	

	public:

		LinearHash()
			: capacity_(BUCKET_CAPACITY),
			  size_(0),
			  level_(0),
			  splitPos_(0)
		{
			buckets_.push_back(new BucketType());
		}

		~LinearHash() throw()
		{
			for (typename VectorOfBuckets::iterator itr = buckets_.begin(); itr != buckets_.end(); ++itr)
			{
				delete *itr;
			}
		}

		int Size() const
		{
			return size_;
		}

		int Capacity() const
		{
			return capacity_;
		}

		bool Insert(const _DataType & data)
		{
			int hash = HashFunc(data);
			int slot = HashLevel(hash, level_);
			if (slot < splitPos_)
			{
				slot = HashLevel(hash, level_ + 1);
			}
			//if (buckets_[slot]->Full())
			if ((float) size_ / capacity_ >= 0.8)
			{
				Split();
				slot = HashLevel(hash, level_);
				if (slot < splitPos_)
				{
					slot = HashLevel(hash, level_ + 1);
				}
			}
			++size_;
			buckets_[slot]->Add(data);
			return true;
		}

		void Split()
		{
			//printf("split\n");
			buckets_.push_back(new BucketType());
			BucketType & oldBucket = *(buckets_[splitPos_]);
			BucketType & newBucket = *(buckets_[(1 << level_) + splitPos_]);
			int j = 0;
			for (int i = 0; i < oldBucket.Size(); ++i)
			{
				int slot = HashLevel(HashFunc(oldBucket[i]), level_ + 1);
				if (slot == splitPos_)
				{
					if (j < i)
					{
						oldBucket[j] = oldBucket[i];
					}
					++j;
				}
				else
				{
					newBucket.Add(oldBucket[i]);
				}
			}
			for (typename ListOfOverflows::iterator itr = oldBucket.Overflows().begin(); itr != oldBucket.Overflows().end(); )
			{
				int slot = HashLevel(HashFunc(*itr), level_ + 1);
				if (slot == splitPos_)
				{
					if (j == BUCKET_CAPACITY)
					{
						++itr;
					}
					else
					{
						oldBucket[j++] = *itr;
						oldBucket.Overflows().erase(itr++);
					}
				}
				else
				{
					newBucket.Add(*itr);
					oldBucket.Overflows().erase(itr++);
				}
			}
			oldBucket.SetSize(j);
			
			capacity_ += BUCKET_CAPACITY;
			++splitPos_;
			if (splitPos_ == (1 << level_))
			{
				splitPos_ = 0;
				++level_;
			}
		}

		bool SelfTest() const
		{
			for (int i = 0; i < (1 << level_) + splitPos_; ++i)
			{
				BucketType * bucket = buckets_[i];
				if (!bucket->Full() && !bucket->Overflows().empty())
				{
					return false;
				}
			}
			for (int i = 0; i < splitPos_; ++i)
			{
				BucketType * bucket = buckets_[i];
				for (int j = 0; j < bucket->Size(); ++j)
				{
					if (HashLevel(HashFunc((*bucket)[j]), level_ + 1) != i)
					{
						return false;
					}
				}
				bucket = buckets_[(1 << level_) + i];
				for (int j = 0; j < bucket->Size(); ++j)
				{
					if (HashLevel(HashFunc((*bucket)[j]), level_ + 1) != (1 << level_) + i)
					{
						return false;
					}
				}
			}
			for (int i = splitPos_; i < (1 << level_); ++i)
			{
				BucketType * bucket = buckets_[i];
				for (int j = 0; j < bucket->Size(); ++j)
				{
					if (HashLevel(HashFunc((*bucket)[j]), level_) != i)
					{
						return false;
					}
				}
			}
			int s = 0;
			for (int i = 0; i < (1 << level_) + splitPos_; ++i)
			{
				s += buckets_[i]->Size() + buckets_[i]->Overflows().size();
			}
			if (s != size_)
			{
				return false;
			}
			if (BUCKET_CAPACITY * ((1 << level_) + splitPos_) != capacity_)
			{
				return false;
			}
			return true;
		}

		void DebugPrint() const
		{
			std::map<int, int> m;
			int s1 = 0;
			int s2 = 0;
			int c = 0;
			for (int i = 0; i < (1 << level_) + splitPos_; ++i)
			{
				s1 += buckets_[i]->Size();
				s2 += buckets_[i]->Overflows().size();
				++m[buckets_[i]->Overflows().size()];
				c += (1 + buckets_[i]->Overflows().size());
			}
			printf("primary  = %d\n", s1);
			printf("overflow = %d\n", s2);
			for (std::map<int, int>::const_iterator itr = m.begin(); itr != m.end(); ++itr)
			{
				printf("%d;%d\n", itr->first, itr->second);
			}
			printf("avg=%0.6f\n", (float) c / ((1 << level_) + splitPos_));
			printf("%d/%d=%0.3f%%\n", size_, capacity_, 100.0 * size_ / capacity_);
		}

	protected:

		static int HashFunc(const _DataType & data)
		{
			return (int) data;
		}

		static int HashLevel(int hash, int level)
		{
			return hash & ((1 << level) - 1);
		}

		VectorOfBuckets buckets_;
		int capacity_;
		int size_;
		int level_;
		int splitPos_;
};

int main(int argc, const char * argv[])
{
	printf("TIME=%d\n", (int) time(NULL));
	srand(time(NULL));

	for (int i = 0; i < 10; i++)
	{
		printf("%d ", rand());
	}
	printf("\n");

	static const int COUNT = 10000;
	int success = 0;

	LinearHash<int, 256> lh;
	for (int i = 0; i < COUNT; ++i)
	{
		if (lh.Insert(rand()))
		{
			++success;
		}
	}
	printf("%d - %0.3f\n", COUNT, 100.0 * (float) lh.Size() / (float) lh.Capacity());

	lh.DebugPrint();
	printf("Selftest...%d\n", lh.SelfTest());
//	printf("success=%d / count=%d\n", success, COUNT);
}
