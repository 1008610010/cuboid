#include "FilePath.hpp"

#include "../../Exception.hpp"
#include "FilePathTokenizer.hpp"
#include "FileUtils.hpp"

namespace XTL
{
		class FilePathCreator : public FilePathTokenListener
		{
			public:

				explicit FilePathCreator(FilePath & target)
					: target_(target) { ;; }

				virtual ~FilePathCreator() throw()
				{
					;;
				}

				virtual void OnRootDir()
				{
					target_.SetAbsolute();
				}

				virtual void OnCurrentDir()
				{
					;;
				}

				virtual void OnParentDir()
				{
					target_.AppendParentDir();
				}

				virtual void OnPathPart(const std::string & part)
				{

					target_.AppendPathPart(part);
				}

			private:

				FilePath & target_;
		};

		class FilePathAppender : public FilePathCreator
		{
			public:

				explicit FilePathAppender(FilePath & target)
					: FilePathCreator(target) { ;; }

				virtual ~FilePathAppender() throw()
				{
					;;
				}

				virtual void OnRootDir()
				{
					// Do nothing or throw exception?
				}
		};

	FilePath::FilePath(const std::string & filePath)
		: parts_(),
		  absolute_(false)
	{
		FilePathCreator creator(*this);
		TokenizeFilePath(creator, filePath.c_str());
	}

	FilePath::FilePath(const char * filePath)
		: parts_(),
		  absolute_(false)
	{
		FilePathCreator creator(*this);
		TokenizeFilePath(creator, filePath);
	}

	const std::string FilePath::ToString() const
	{
		if (parts_.empty())
		{
			return absolute_ ? "/" : ".";
		}

		std::string buffer(absolute_ ? "/" : "");

		ListOfParts::const_iterator itr(parts_.begin());
		const ListOfParts::const_iterator end(parts_.end());

		buffer.append(*itr);

		for (++itr; itr != end; ++itr)
		{
			buffer.append("/").append(*itr);
		}

		return buffer;
	}

	FilePath & FilePath::Append(const FilePath & other)
	{
		FilePathAppender appender(*this);
		other.Tokenize(appender);
		return *this;
	}

	FilePath & FilePath::Append(const std::string & filePath)
	{
		return Append(filePath.c_str());
	}

	FilePath & FilePath::Append(const char * filePath)
	{
		FilePathAppender appender(*this);
		TokenizeFilePath(appender, filePath);
		return *this;
	}

	const std::string FilePath::Remove()
	{
		if (parts_.empty())
		{
			return std::string();
		}

		const std::string result = parts_.back();
		parts_.pop_back();
		return result;
	}

	void FilePath::ConvertToAbsolute()
	{
		if (IsAbsolute())
		{
			return;
		}

		ConvertToAbsolute(FileUtils::GetCurrentDirectory());
	}

	void FilePath::ConvertToAbsolute(const std::string & baseDir)
	{
		if (IsAbsolute())
		{
			return;
		}

		FilePath tempFilePath(baseDir);
		if (!tempFilePath.IsAbsolute())
		{
			throw ILLEGAL_ARGUMENT_ERROR("FilePath::ConvertToAbsolute() - baseDir is not absolute");
		}

		tempFilePath.Append(*this);
		std::swap(parts_, tempFilePath.parts_);

		SetAbsolute();
	}

	void FilePath::SetAbsolute()
	{
		absolute_ = true;
	}

	void FilePath::AppendParentDir()
	{
		if (parts_.empty())
		{
			if (!absolute_)
			{
				parts_.push_back("..");
			}
		}
		else if (parts_.back() == "..")
		{
			parts_.push_back("..");
		}
		else
		{
			parts_.pop_back();
		}
	}

	void FilePath::AppendPathPart(const std::string & part)
	{
		parts_.push_back(part);
	}

	void FilePath::Tokenize(FilePathTokenListener & listener) const
	{
		const ListOfParts::const_iterator end(parts_.end());

		if (IsAbsolute())
		{
			listener.OnRootDir();
		}

		for (ListOfParts::const_iterator itr(parts_.begin()); itr != end; ++itr)
		{
			if (*itr == "..")
			{
				listener.OnParentDir();
			}
			else
			{
				listener.OnPathPart(*itr);
			}
		}
	}

	const std::string operator+ (const FilePath & left, const FilePath & right)
	{
		return FilePath(left).Append(right).ToString();
	}

	const std::string operator+ (const FilePath & left, const std::string & right)
	{
		return FilePath(left).Append(right).ToString();
	}
}

