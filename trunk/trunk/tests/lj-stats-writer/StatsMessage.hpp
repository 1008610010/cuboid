#ifndef XC3__STATS_MESSAGE_HPP__
#define XC3__STATS_MESSAGE_HPP__ 1

#include <string.h>

#include <vector>

#include <xtl/Types.hpp>

namespace XC3
{
	class StatsMessageHeader
	{
		public:

			static const XTL::UINT_32 MAGIC = 0x2a2a2a2a; // "****"

			explicit StatsMessageHeader(XTL::UINT_32 size)
				: magic_(MAGIC),
				  size_(size)
			{
				;;
			}

			bool IsMagicValid() const
			{
				return magic_ == MAGIC;
			}

			XTL::UINT_32 Size() const
			{
				return size_;
			}

		private:

			XTL::UINT_32 magic_;
			XTL::UINT_32 size_;
	};

	class StatsMessageBuffer
	{
		public:

			StatsMessageBuffer()
				: buffer_()
			{
				;;
			}

			void Write(const void * ptr, unsigned int size)
			{
				unsigned int oldSize = buffer_.size();
				buffer_.resize(oldSize + size);
				::memcpy(&(buffer_[oldSize]), ptr, size);
			}

			bool HasHeader() const
			{
				return buffer_.size() >= sizeof(StatsMessageHeader);
			}

			const StatsMessageHeader * Header() const
			{
				return reinterpret_cast<const StatsMessageHeader *>(&(buffer_[0]));
			}

			unsigned int TotalSize() const
			{
				return sizeof(StatsMessageHeader) + Header()->Size();
			}

			bool IsMagicValid() const
			{
				return Header()->IsMagicValid();
			}

			bool IsComplete() const
			{
				return HasHeader() && buffer_.size() >= TotalSize();
			}

			void Remove()
			{
				buffer_.erase(buffer_.begin(), buffer_.begin() + TotalSize());
			}

		private:

			std::vector<char> buffer_;
	};
}

#endif

