#ifndef XTL__FILE_OUTPUT_STREAM_BUFFERED_HPP__
#define XTL__FILE_OUTPUT_STREAM_BUFFERED_HPP__

#include <string>

#include "FileOutputStream.hpp"

namespace XTL
{
	class FileOutputStreamBuffered : public OutputStream
	{
		public:

			static const unsigned int DEFAULT_BUFFER_CAPACITY = 65536;

			FileOutputStreamBuffered(const std::string & filePath, unsigned int bufferCapacity = DEFAULT_BUFFER_CAPACITY);

			virtual ~FileOutputStreamBuffered() throw();

			virtual void Write(const void * buffer, unsigned int size);

			FileSize Position() const throw();

			const std::string & FilePath() const throw()
			{
				return outputStream_.FilePath();
			}

			void Flush();

			void Close();

		private:

			class Buffer
			{
				public:

					explicit Buffer(unsigned int capacity);

					~Buffer() throw();

					unsigned int Capacity() const;

					unsigned int Size() const;

					unsigned int FreeSize() const;

					void Write(const void * buffer, unsigned int size);

					void Flush(FileOutputStream & outputStream);

				private:

					Buffer(const Buffer &);
					Buffer & operator= (const Buffer &);

					char * const data_;
					const unsigned int capacity_;
					unsigned int size_;
			};

			FileOutputStreamBuffered(const FileOutputStreamBuffered &);
			FileOutputStreamBuffered & operator= (const FileOutputStreamBuffered &);

			FileOutputStream outputStream_;
			Buffer           buffer_;
			FileSize         position_;
	};
}

#endif

