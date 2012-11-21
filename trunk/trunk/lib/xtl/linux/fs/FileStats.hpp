#ifndef XTL__FILE_STATS_HPP__
#define XTL__FILE_STATS_HPP__ 1

#include <sys/stat.h>

#include <string>

#include "../../Types.hpp"
#include "../AccessMode.hpp"

namespace XTL
{
	class FileType
	{
		public:

			static const FileType NOT_EXISTS;
			static const FileType REGULAR;
			static const FileType DIRECTORY;
			static const FileType LINK;
			static const FileType UNKNOWN;

			const bool operator== (const FileType & other) const
			{
				return id_ == other.id_;
			}

			const bool operator!= (const FileType & other) const
			{
				return id_ != other.id_;
			}

		private:

			friend class FileStats;

			static FileType FromStats(const struct ::stat64 & stats);

			explicit FileType(int id)
				: id_(id) { ;; }

			const int id_;
	};

	class FileStats
	{
		public:

			FileStats();

			explicit FileStats(const std::string & fileName);

			explicit FileStats(int fd);

			void Init(const std::string & fileName);

			void Init(int fd);

			FileSize Size() const;

			const FileType Type() const;

			const AccessMode Mode() const;

			bool IsRegular() const;

			bool IsDirectory() const;

			/**
			 * Время последнего доступа. Изменяется при доступе к файлу, например, при exec(2), mknod(2), pipe(2), utime(2)
			 * и read(2) (если прочитано больше нуля байт). Другие функции, например, mmap(2), могут изменять, а могу и не изменять st_atime.
			 */
			time_t LastAccessTime() const;

			/**
			 * Время последней модификации. Изменяется при модификациях файла, например, при mknod(2), truncate(2), utime(2)
			 * и write(2) (если записано больше нуля байт).
			 * Для каталога изменяется при создании и удалении файлов в этом каталоге.
			 * Не изменяется при изменении владельца, группы, количества жестких ссылок или режима доступа.
			 */
			time_t ModificationTime() const;

			/**
			 * Время последнего изменения. Изменяется при записи или установке информации об inode (владельце, группе, количеству ссылок, режиме и т. д.).
			 */
			time_t StatusChangeTime() const;

			static const FileType Type(const std::string & filePath);

		private:

			struct ::stat64 stats_;
	};
}

#endif
