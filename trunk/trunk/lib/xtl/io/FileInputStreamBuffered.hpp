#ifndef XTL__FILE_INPUT_STREAM_BUFFERED_HPP__
#define XTL__FILE_INPUT_STREAM_BUFFERED_HPP__ 1

#include <string>

#include "FileInputStream.hpp"

namespace XTL
{
	class FileInputStreamBuffered : public InputStream
	{
		public:

			static const unsigned int DEFAULT_BUFFER_CAPACITY = 65536;

			FileInputStreamBuffered(const std::string & filePath, unsigned int bufferCapacity = DEFAULT_BUFFER_CAPACITY);

			virtual ~FileInputStreamBuffered() throw();

			virtual bool AtEnd() const;

			virtual void Read(void * buffer, unsigned int size);

			FileSize Size() const;

			FileSize Position() const;

			FileSize AvailableSize() const;

			const std::string & FilePath() const throw();

		private:

			class Buffer
			{
				public:

					explicit Buffer(unsigned int capacity);

					~Buffer() throw();

					bool AtEnd() const;

					unsigned int Capacity() const;

					unsigned int AvailableSize() const;

					unsigned int Read(void * buffer, unsigned int size);

					void Update(FileInputStream & inputStream, unsigned int minimumSize);

				private:

					Buffer(const Buffer &);
					Buffer & operator= (const Buffer &);

					char * const data_;
					const unsigned int capacity_;
					unsigned int size_;
					unsigned int position_;
			};

			FileInputStreamBuffered(const FileInputStreamBuffered &);
			FileInputStreamBuffered & operator= (const FileInputStreamBuffered &);

			FileInputStream inputStream_;
			Buffer buffer_;
	};
}

#endif

