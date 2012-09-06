#include "FileInputStream.hpp"

#include <stdio.h>

#include <stdexcept>

#include "../linux/fs/FileStats.hpp"

namespace XTL
{
	FileInputStream::FileInputStream(const std::string & filePath)
		: file_(filePath),
		  position_(0),
		  size_(0)
	{
		file_.Open(File::OPEN_READ_ONLY);

		XTL::FileStats fileStats;
		file_.GetStats(fileStats);
		size_ = fileStats.Size();
	}

	FileInputStream::~FileInputStream() throw()
	{
		Close();
	}

	void FileInputStream::Read(void * buffer, unsigned int size)
	{
		unsigned int wasRead = file_.Read(buffer, size);
		if (wasRead != size)
		{
			throw ReadError();
		}

		position_ += size;
	}

	bool FileInputStream::AtEnd() const
	{
		return position_ >= size_;
	}

	void FileInputStream::Close()
	{
		file_.Close();
	}

	void FileInputStream::Seek(FileSize offset)
	{
		file_.Seek(offset);
	}

	FileSize FileInputStream::Size() const
	{
		return size_;
	}

	FileSize FileInputStream::Position() const
	{
		return position_;
	}

	const std::string & FileInputStream::FilePath() const throw()
	{
		return file_.Path();
	}
}

