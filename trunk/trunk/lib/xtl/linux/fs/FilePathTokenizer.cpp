#include "FilePathTokenizer.hpp"

namespace XTL
{
	FilePathTokenizer::FilePathTokenizer(FilePathTokenListener & listener, const char * filePath, char delimiter)
		: listener_(listener),
		  ptr_(filePath),
		  begin_(filePath),
		  delimiter_(delimiter)
	{
		if (ptr_ == 0 || *ptr_ == '\0')
		{
			return;
		}

		// TODO: Заменить строку filePath на объект класса CharSourceText
		if (IsDelimiter())
		{
			listener_.OnRootDir();

			SkipDelimiters();
			begin_ = ptr_;
		}

		while (*ptr_ != '\0')
		{
			// HERE: *ptr != delimiter_ && *ptr != '\0'
			do
			{
				++ptr_;
			}
			while (!IsDelimiter() && *ptr_ != '\0');

			ProcessToken(begin_, ptr_);

			if (IsDelimiter())
			{
				SkipDelimiters();
			}

			begin_ = ptr_;
		}
	}

	bool FilePathTokenizer::IsDelimiter()
	{
		return *ptr_ == delimiter_;
	}

	void FilePathTokenizer::SkipDelimiters()
	{
		do
		{
			++ptr_;
		}
		while (IsDelimiter());
	}

	// [ begin .. end )
	void FilePathTokenizer::ProcessToken(const char * begin, const char * end)
	{
		if (*begin == '.')
		{
			++begin;
			if (begin == end) // "."
			{
				listener_.OnCurrentDir();
				return;
			}
			else if (*begin == '.' && begin + 1 == end) // ".."
			{
				listener_.OnParentDir();
				return;
			}
			--begin;
		}

		listener_.OnPathPart(std::string(begin, end - begin));
	}

	void TokenizeFilePath(FilePathTokenListener & listener, const char * filePath, char delimiter)
	{
		FilePathTokenizer tokenizer(listener, filePath, delimiter);
	}
}

