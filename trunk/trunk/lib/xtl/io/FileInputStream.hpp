#ifndef XTL__FILE_INPUT_STREAM_HPP__
#define XTL__FILE_INPUT_STREAM_HPP__ 1

#include <string>

#include "SeekableInputStream.hpp"
#include "../Types.hpp"
#include "../linux/fs/File.hpp"

namespace XTL
{
	class FileInputStream : public SeekableInputStream
	{
		public:

			explicit FileInputStream(const std::string & filePath);

			virtual ~FileInputStream() throw();

			virtual void Read(void * buffer, unsigned int size);

			virtual bool AtEnd() const;

			void Close();

			virtual void Seek(FileSize offset);

			virtual FileSize Size() const;

			virtual FileSize Position() const;

		private:

			FileInputStream(const FileInputStream &);
			FileInputStream & operator= (const FileInputStream &);

			File     file_;
			FileSize position_;
			FileSize size_;
	};
}

#endif

