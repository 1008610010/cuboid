#ifndef XTL_PLAIN__RECORD_ARRAY_READER_HPP__
#define XTL_PLAIN__RECORD_ARRAY_READER_HPP__ 1

#include "../Types.hpp"
#include "../io/InputStream.hpp"

namespace XTL
{
namespace PLAIN
{
	template <typename ItemType_>
	class RecordArrayReader
	{
		public:

			RecordArrayReader(XTL::InputStream & inputStream)
				: inputStream_(inputStream),
				  size_(0),
				  index_(0),
				  item_()
			{
				;;
			}

			void Init(XTL::UINT_32 newSize)
			{
				size_ = newSize;
				index_ = 0;
			}

			bool AtEnd() const
			{
				return index_ >= size_;
			}

			void Read() const
			{
				item_.Read(inputStream_);
				++index_;
			}

			const ItemType_ * operator->() const
			{
				return &item_;
			}

			void SkipAll() const
			{
				// TODO: optimize it
				while (!AtEnd())
				{
					Read();
				}
			}

		private:

			XTL::InputStream     & inputStream_;
			XTL::UINT_32           size_;
			mutable XTL::UINT_32   index_;
			mutable ItemType_      item_;
	};
}
}

#endif

