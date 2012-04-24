#include "RecordArray.hpp"

#include <string.h>

#include "RecordPrototype.hpp"
#include "../io/InputStream.hpp"
#include "../io/OutputStream.hpp"
#include "../utils/Algorithm.hpp"

namespace XTL
{
namespace PLAIN
{
	class MemoryBlock
	{
		public:

			MemoryBlock(unsigned int capacity, unsigned int itemSize)
				: data_(new char[capacity * itemSize]),
				  itemSize_(itemSize)
			{
				// TODO: ASSERT(size_ <= capacity_)
				::memset(data_, '\0', capacity * itemSize);
			}

			~MemoryBlock() throw()
			{
				delete[] data_;
			}

			void * ItemData(unsigned int index)
			{
				return data_ + index * itemSize_;
			}

			const void * ItemData(unsigned int index) const
			{
				return data_ + index * itemSize_;
			}

			void Read(XTL::InputStream & stream, unsigned int itemsCount)
			{
				stream.Read(data_, itemsCount * itemSize_);
			}

			void Write(XTL::OutputStream & stream, unsigned int itemsCount) const
			{
				stream.Write(data_, itemsCount * itemSize_);
			}

		private:

			MemoryBlock(const MemoryBlock &);
			MemoryBlock & operator= (const MemoryBlock &);

			char         * data_;
			unsigned int   itemSize_;
	};

	RecordArray::RecordArray(RecordPrototype::SharedConstPtr prototype, unsigned int chunkCapacity, unsigned int size)
		: prototype_(prototype),
		  chunkCapacity_(chunkCapacity),
		  size_(0),
		  chunks_()
	{
		if (size > 0)
		{
			Resize(size);
		}
	}

	RecordArray::~RecordArray() throw()
	{
		Clear();
	}

	void RecordArray::Clear()
	{
		std::deque<MemoryBlock *>::iterator end = chunks_.end();
		for (std::deque<MemoryBlock *>::iterator itr = chunks_.begin(); itr != end; ++itr)
		{
			delete *itr;
		}

		chunks_.clear();
		size_ = 0;
	}

	unsigned int RecordArray::Size() const
	{
		return size_;
	}

	bool RecordArray::IsEmpty() const
	{
		return size_ == 0;
	}

	unsigned int RecordArray::ChunksCount() const
	{
		return chunks_.size();
	}

	void RecordArray::Resize(unsigned int newSize)
	{
		if (size_ == newSize)
		{
			return;
		}
		else if (newSize == 0)
		{
			Clear();
		}
		else if (newSize > size_)
		{
			unsigned int newChunksCount = (newSize - 1) / chunkCapacity_ + 1;

			if (newChunksCount > chunks_.size())
			{
				chunks_.resize(newChunksCount, 0);
			}

			size_ = newSize;
		}
		else // size_ > newSize
		{
			unsigned int newChunksCount = (newSize - 1) / chunkCapacity_ + 1;

			for (unsigned int i = newChunksCount; i < chunks_.size(); ++i)
			{
				delete chunks_[i];
				chunks_[i] = 0;
			}
			chunks_.resize(newChunksCount);

			size_ = newSize;
		}
	}

	RecordRef RecordArray::PushBack()
	{
		Resize(Size() + 1);
		return (*this)[Size() - 1];
	}

	RecordRef RecordArray::operator[] (unsigned int index)
	{
		return RecordRef(prototype_.Get(), ItemData(index));
	}

	RecordConstRef RecordArray::operator[] (unsigned int index) const
	{
		return RecordConstRef(prototype_.Get(), ItemData(index));
	}

	const RecordPrototype * RecordArray::Prototype() const
	{
		return prototype_.Get();
	}

	RecordPrototype::SharedConstPtr RecordArray::SharedPrototype() const
	{
		return prototype_;
	}

	void RecordArray::Read(XTL::InputStream & stream)
	{
		unsigned int leftItems = size_;
		std::deque<MemoryBlock *>::iterator end = chunks_.end();
		for (std::deque<MemoryBlock *>::iterator itr = chunks_.begin(); itr != end; ++itr)
		{
			if (*itr == 0)
			{
				*itr = CreateMemoryBlock();
			}

			unsigned int itemsToRead = Min(leftItems, chunkCapacity_);
			(*itr)->Read(stream, itemsToRead);
			leftItems -= itemsToRead;
		}
	}

	void RecordArray::Write(XTL::OutputStream & stream) const
	{
		unsigned int leftItems = size_;
		std::deque<MemoryBlock *>::iterator end = chunks_.end();
		for (std::deque<MemoryBlock *>::iterator itr = chunks_.begin(); itr != end; ++itr)
		{
			if (*itr == 0)
			{
				// TODO: create empty Record and write it chunkCapacity_ times.
				*itr = CreateMemoryBlock();
			}

			unsigned int itemsToWrite = Min(leftItems, chunkCapacity_);
			(*itr)->Write(stream, itemsToWrite);
			leftItems -= itemsToWrite;
		}
	}

	MemoryBlock * RecordArray::CreateMemoryBlock() const
	{
		return new MemoryBlock(chunkCapacity_, prototype_->Size());
	}

	void * RecordArray::ItemData(unsigned int index)
	{
		// TODO: ASSERT(index < size_)
		unsigned int chunkIndex = index / chunkCapacity_;
		if (chunks_[chunkIndex] == 0)
		{
			chunks_[chunkIndex] = CreateMemoryBlock();
		}

		return chunks_[chunkIndex]->ItemData(index % chunkCapacity_);
	}

	const void * RecordArray::ItemData(unsigned int index) const
	{
		unsigned int chunkIndex = index / chunkCapacity_;
		if (chunks_[chunkIndex] == 0)
		{
			chunks_[chunkIndex] = CreateMemoryBlock();
		}

		return chunks_[chunkIndex]->ItemData(index % chunkCapacity_);
	}


}
}

