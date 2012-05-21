#ifndef XTL__SEEKABLE_INPUT_STREAM_HPP__
#define XTL__SEEKABLE_INPUT_STREAM_HPP__ 1

#include "InputStream.hpp"
#include "../Types.hpp"

namespace XTL
{
	class SeekableInputStream : public InputStream
	{
		public:

			virtual ~SeekableInputStream() throw() { ;; }

			virtual void Seek(FileSize offset) = 0;

			virtual FileSize Size() const = 0;

			virtual FileSize Position() const = 0;

			FileSize AvailableSize() const
			{
				return Size() - Position();
			}
	};
}

#endif

