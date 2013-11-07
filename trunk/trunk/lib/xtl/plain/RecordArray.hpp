#ifndef XTL__PLAIN__RECORD_ARRAY_HPP__
#define XTL__PLAIN__RECORD_ARRAY_HPP__ 1

#include <algorithm>
#include <deque>

#include "Record.hpp"
#include "../Logger.hpp"
#include "../io/Serializable.hpp"

namespace XTL
{
	class InputStream;
	class OutputStream;
}

namespace XTL
{
namespace PLAIN
{
	class MemoryBlock;

	class RecordArray : public XTL::Serializable
	{
		public:

			RecordArray(RecordPrototype::SharedConstPtr prototype, unsigned int chunkCapacity, unsigned int size = 0);

			virtual ~RecordArray() throw();

			void Clear();

			unsigned int Size() const;

			bool IsEmpty() const;

			unsigned int ChunksCount() const;

			void Resize(unsigned int newSize);

			RecordRef PushBack();

			RecordRef operator[] (unsigned int index);

			RecordConstRef operator[] (unsigned int index) const;

			const RecordPrototype * Prototype() const;

			RecordPrototype::SharedConstPtr SharedPrototype() const;

			virtual void Read(XTL::InputStream & stream);

			virtual void Write(XTL::OutputStream & stream) const;

		private:

			RecordArray(const RecordArray &);
			RecordArray & operator= (const RecordArray &);

			MemoryBlock * CreateMemoryBlock() const;

			void * ItemData(unsigned int index);

			const void * ItemData(unsigned int index) const;

			RecordPrototype::SharedConstPtr prototype_;
			unsigned int chunkCapacity_;
			unsigned int size_;

			// TODO: AutoPtrDeque
			mutable std::deque<MemoryBlock *> chunks_;
	};

	template <typename Comparator>
	RecordRef BinarySearch(RecordArray & array, Comparator comp)
	{
		unsigned int left = 0;
		unsigned int right = array.Size();

		while (left < right)
		{
			unsigned int middle = (left + right) / 2;

			RecordRef ref = array[middle];

			int result = comp(ref);

			if (result < 0)
			{
				right = middle;
			}
			else if (result > 0)
			{
				left = middle + 1;
			}
			else
			{
				return ref;
			}
		}

		return RecordRef(array.Prototype());
	}

	template <typename Comparator>
	RecordConstRef BinarySearch(const RecordArray & array, Comparator comp)
	{
		unsigned int left = 0;
		unsigned int right = array.Size();

		while (left < right)
		{
			unsigned int middle = (left + right) / 2;

			RecordConstRef ref = array[middle];

			int result = comp(ref);

			if (result < 0)
			{
				right = middle;
			}
			else if (result > 0)
			{
				left = middle + 1;
			}
			else
			{
				return ref;
			}
		}

		return RecordConstRef(array.Prototype());
	}


	template <typename RecordType_>
	class RecordFixedArray
	{
		public:

			typedef RecordType_ RecordType;

			RecordFixedArray(unsigned int capacity)
				: capacity_(capacity_),
				  data_(new char[ItemSize() * capacity])
			{
				::memset(data_, '\0', ItemSize() * capacity);
			}

			~RecordFixedArray() throw()
			{
				delete [] static_cast<char *>(data_);
			}

			static unsigned int ItemSize()
			{
				static const unsigned int itemSize = CalcItemSize();
				return itemSize;
			}

			static unsigned int Alignment()
			{
				return RecordType::Prototype()->Alignment();
			}

			unsigned int Capacity() const
			{
				return capacity_;
			}

			typename RecordType::ConstRef operator[] (unsigned int index) const
			{
				return static_cast<const char *>(data_) + index * ItemSize();
			}

			typename RecordType::Ref operator[] (unsigned int index)
			{
				return static_cast<char *>(data_) + index * ItemSize();
			}

			void Swap(RecordFixedArray & other)
			{
				std::swap(capacity_, other.capacity_);
				std::swap(data_, other.data_);
			}

		private:

			RecordFixedArray(const RecordFixedArray &);
			RecordFixedArray & operator= (const RecordFixedArray &);

			static unsigned int CalcItemSize()
			{
				if (RecordType::Prototype()->Size() % RecordType::Prototype()->Alignment())
				{
					Warn(
						"Using record prototype '%s' with size (%u), that is not a multiple of its alignment (%u), in RecordFixedArray",
						RecordType::Prototype()->Name(),
						RecordType::Prototype()->Size(),
						RecordType::Prototype()->Alignment()
					);
				}
				return RecordType::Prototype()->AlignedSize();
			}

			unsigned int capacity_;
			void * data_;
	};
}
}

#endif

