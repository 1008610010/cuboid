#ifndef XTL_PLAIN__RECORD_ARRAY_HPP__
#define XTL_PLAIN__RECORD_ARRAY_HPP__ 1

#include <deque>

#include "Record.hpp"
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
}
}

#endif

