#include "FileTransaction.hpp"

#include "../linux/fs/FileUtils.hpp"

namespace XTL
{
	FileTransaction::FileTransaction(const std::string & filePath, unsigned int bufferCapacity)
		: filePath_(filePath),
		  outputStream_(TempFilePath(filePath), bufferCapacity)
	{
		;;
	}

	FileTransaction::~FileTransaction() throw()
	{
		;;
	}

	void FileTransaction::Write(const void * buffer, unsigned int size)
	{
		outputStream_.Write(buffer, size);
	}

	const std::string & FileTransaction::FileName() const throw()
	{
		return filePath_;
	}

	FileSize FileTransaction::Position() const throw()
	{
		return outputStream_.Position();
	}

	void FileTransaction::Commit()
	{
		outputStream_.Flush();
		outputStream_.Close();

		FileUtils::Rename(TempFilePath(filePath_), filePath_);
	}

	const std::string FileTransaction::TempFilePath(const std::string & filePath) const
	{
		return filePath + ".tmp";
	}
}

