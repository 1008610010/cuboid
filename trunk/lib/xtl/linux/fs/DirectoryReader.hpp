#ifndef XTL__DIRECTORY_READER_HPP__
#define XTL__DIRECTORY_READER_HPP__ 1

#include <dirent.h>

#include <string>

namespace XTL
{
	class DirectoryReader
	{
		public:

			class Entry
			{
				public:

					bool IsNull() const
					{
						return result_ == 0;
					}

					/*
					 * WARN: На XFS d_type не работает!
					 * Мож этот метод выпилить к ибиням?
					 */
					bool IsRegular1() const
					{
						return (result_->d_type & DT_REG) != 0;
					}

					bool IsDirectory() const
					{
						return (result_->d_type & DT_DIR) != 0;
					}

					bool IsUnknownType() const
					{
						return (result_->d_type & DT_UNKNOWN) != 0;
					}

					const char * Name() const
					{
						return result_->d_name;
					}

				private:

					friend class DirectoryReader;

					Entry()
						: entry_(),
						  result_(&entry_)
					{
						;;
					}

					struct dirent * EntryPtr()
					{
						return &entry_;
					}

					struct dirent ** ResultPtr()
					{
						return &result_;
					}

					struct dirent   entry_;
					struct dirent * result_;
			};

			explicit DirectoryReader(const std::string & directory);

			DirectoryReader(const std::string & directory, const std::string & pattern);

			~DirectoryReader() throw();

			bool Read();

			const Entry & Current() const
			{
				return entry_;
			}

		private:

			void Init();

			void GetNext();

			const std::string directory_;
			const std::string pattern_;
			DIR   * dir_;
			Entry   entry_;
	};
}

#endif

