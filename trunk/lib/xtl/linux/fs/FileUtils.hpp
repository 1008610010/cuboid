#ifndef XTL__FILE_UTILS_HPP__
#define XTL__FILE_UTILS_HPP__ 1

#include <sys/stat.h>

#include <string>

namespace XTL
{
	struct FileUtils
	{
		/**
		 * Обертка для системного вызова rename.
		 * Атомарное переименование файла, т.е. если newPath уже существует, то он будет атомарно перезаписан (если не возникнет ошибок),
		 * т.е. нет ни одной точки, когда другой процесс, пытающийся обратиться к newPath, не обнаружит его.
		 * @throw UnixError - Системная ошибка.
		 */
		static void Rename(const std::string & oldPath, const std::string & newPath);

		/**
		 * Обертка для системного вызова unlink.
		 * @return true - если файл успешно удален, false - файл не существует.
		 * @throw UnixError - Системная ошибка.
		 */
		static bool Unlink(const std::string & filePath);

		static const std::string GetCurrentDirectory();

		static const std::string NormalizeFilePath(const std::string & filePath);

		static const std::string AbsoluteFilePath(const std::string & filePath, const std::string & baseDir);

		static const std::string AbsoluteFilePath(const std::string & filePath);

		static const std::string JoinFilePath(const std::string & left, const std::string & right);

		/**
		 * @return true     - Директория успешно создана.
		 * @return false    - Директория уже существует.
		 * @throw UnixError - Системная ошибка.
		 */
		static bool CreateDirectory(const std::string & directoryPath);
	};
}

#endif

