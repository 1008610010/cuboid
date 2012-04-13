#ifndef _STXX__OUTPUT_STREAM_HPP__
#define _STXX__OUTPUT_STREAM_HPP__ 1

#include <sup/Types.h>
#include "File.hpp"

namespace STXX
{
	class FileOutputStream
	{
		public:

			class Buffer
			{
				public:

					Buffer(UINT_64 capacity);

					~Buffer() throw();

					void Clear();

					UINT_64 Size() const { return size_; }

					bool Write(File & file, const void * buffer, UINT_64 size);

					bool Flush(File & file);

				private:

					UINT_64   capacity_;
					UINT_64   size_;
					CHAR_8  * data_;
			};

			FileOutputStream(INT_64 bufferCapacity)
				: file_(), buffer_(bufferCapacity)
			{
				;;
			}

			bool Open(const CHAR_8 * name)
			{
				buffer_.Clear();
				return file_.Create(name);
			}

			bool Close()
			{
				if (!buffer_.Flush(file_))
				{
					return false;
				}
				file_.Close();
				return true;
			}

			UINT_64 Size() const
			{
				return file_.Size();
			}

			UINT_64 Position() const
			{
				return file_.Position() + buffer_.Size();
			}

			bool Write(const void * buffer, UINT_64 size)
			{
				return buffer_.Write(file_, buffer, size);
			}

			template <typename T>
			bool Write(const T & value)
			{
				return Write(&value, sizeof(value));
			}

		private:

			File   file_;
			Buffer buffer_;
	};
}

#endif
