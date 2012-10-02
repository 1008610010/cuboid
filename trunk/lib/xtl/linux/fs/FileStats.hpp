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

			static const FileType Type(const std::string & filePath);

		private:

			struct ::stat64 stats_;
	};
}

#endif
