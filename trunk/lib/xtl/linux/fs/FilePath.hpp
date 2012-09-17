#ifndef XTL__FILE_PATH_HPP__
#define XTL__FILE_PATH_HPP__ 1

#include <deque>
#include <string>

namespace XTL
{
	class FilePathTokenListener;

	class FilePath
	{
		// TODO: replace this with std::vector
		typedef std::deque<std::string> ListOfParts;

		public:

			explicit FilePath(const std::string & filePath);

			explicit FilePath(const char * filePath);

			bool IsAbsolute() const
			{
				return absolute_;
			}

			const std::string ToString() const;

			FilePath & Append(const FilePath & other);

			FilePath & Append(const std::string & filePath);

			FilePath & Append(const char * filePath);

			const std::string Remove();

			void ConvertToAbsolute();

			void ConvertToAbsolute(const std::string & baseDir);

			class Iterator
			{
				public:

					explicit Iterator(const FilePath & filePath)
						: itr_(filePath.parts_.begin()),
						  end_(filePath.parts_.end()),
						  currentPath_(filePath.IsAbsolute() ? "/" : "")
					{
						if (itr_ != end_)
						{
							currentPath_.append(*itr_);
						}
					}

					bool AtEnd() const
					{
						return itr_ == end_;
					}

					void Advance()
					{
						++itr_;
						if (itr_ != end_)
						{
							currentPath_.append("/").append(*itr_);
						}
					}

					const std::string & CurrentPath() const
					{
						return currentPath_;
					}

				private:

					ListOfParts::const_iterator itr_;
					const ListOfParts::const_iterator end_;
					std::string currentPath_;
			};

		private:

			friend class FilePathCreator;
			friend class FilePathAppender;

			void SetAbsolute();

			void AppendParentDir();

			void AppendPathPart(const std::string & part);

			void Tokenize(FilePathTokenListener & listener) const;

			ListOfParts parts_;
			bool        absolute_;
	};

	const std::string operator+ (const FilePath & left, const FilePath & right);

	const std::string operator+ (const FilePath & left, const std::string & right);
}

#endif

