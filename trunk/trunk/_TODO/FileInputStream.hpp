#ifndef _STXX__FILE_INPUT_STREAM_HPP__
#define _STXX__FILE_INPUT_STREAM_HPP__ 1

/**
 * @file FileInputStream
 */

#include <sup/Types.h>
#include "File.hpp"

/**
 * @namespace STXX C++ file based storage
 */
namespace STXX
{
	/**
	 * @class FileInputStream
	 * @brief File input stream class.
	 */
	class FileInputStream
	{
		public:

			/**
			 * @class Buffer
			 * @brief Auxiliary class of buffered data, that is read from file stream.
			 */
			class Buffer
			{
				public:

					Buffer(UINT_64 capacity);

					~Buffer() throw();

					void Clear();

					UINT_64 Size() const     { return size_; }

					UINT_64 Position() const { return position_; }

					bool Read(File & file, void * buffer, UINT_64 size);

					template <typename T>
					bool Read(File & file, T & value)
					{
						if (sizeof(T) <= size_ - position_)
						{
							value = *reinterpret_cast<const T *>(data_ + position_);
							position_ += sizeof(T);
							return true;
						}
						else
						{
							return Read(file, &value, sizeof(T));
						}
					}

				private:

					bool Load(File & file);

					UINT_64   capacity_;
					UINT_64   size_;
					UINT_64   position_;
					CHAR_8  * data_;
			};

			FileInputStream(UINT_64 bufferCapacity)
				: file_(), buffer_(bufferCapacity) { ;; }

			~FileInputStream() throw() { ;; }

			bool Open(const CHAR_8 * name)
			{
				buffer_.Clear();
				return file_.Open(name) && file_.Seek(0);
			}

			bool Close()
			{
				file_.Close();
				return true;
			}

			void Reset()
			{
				buffer_.Clear();
				file_.Seek(0);
			}

			bool Eof() const
			{
				return Position() == file_.Size();
			}

			UINT_64 Position() const
			{
				return file_.Position() + buffer_.Position() - buffer_.Size();
			}

			bool Seek(UINT_64 position)
			{
				buffer_.Clear();
				return file_.Seek(position);
			}

			bool Read(void * buffer, UINT_64 size) const
			{
				return buffer_.Read(file_, buffer, size);
			}

			template <typename T>
			bool Read(T & value) const
			{
				return buffer_.Read(file_, value);
			}

		private:

			mutable File   file_;
			mutable Buffer buffer_;
	};
}

#endif
