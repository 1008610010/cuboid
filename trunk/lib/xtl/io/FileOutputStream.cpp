#include "FileOutputStream.hpp"

namespace XTL
{
	FileOutputStream::FileOutputStream(const std::string & filePath, AccessMode accessMode)
		: file_(filePath),
		  position_(0)
	{
		file_.Create(File::RECREATE_READ_WRITE, accessMode);
	}

	FileOutputStream::~FileOutputStream() throw()
	{
		;;
	}

	void FileOutputStream::Write(const void * buffer, unsigned int size)
	{
		file_.Write(buffer, size);
		position_ += size;
	}
}

