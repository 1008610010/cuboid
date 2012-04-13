#ifndef XTL__FILE_INPUT_STREAM_HPP__
#define XTL__FILE_INPUT_STREAM_HPP__ 1

#include <string>

#include "InputStream.hpp"
#include "../Types.hpp"
#include "../linux/fs/File.hpp"

namespace XTL
{
	class FileInputStream : public InputStream
	{
		public:

			explicit FileInputStream(const std::string & filePath);

			virtual ~FileInputStream() throw();

			virtual void Read(void * buffer, unsigned int size);

			virtual bool AtEnd() const;

			void Close();

			void Seek(FileSize offset);

			FileSize Size() const;

			FileSize Position() const;

			FileSize AvailableSize() const;

		private:

			FileInputStream(const FileInputStream &);
			FileInputStream & operator= (const FileInputStream &);

			File     file_;
			FileSize position_;
			FileSize size_;
	};
}

#endif

