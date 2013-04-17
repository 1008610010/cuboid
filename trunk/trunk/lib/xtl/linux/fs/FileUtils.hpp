#ifndef XTL__FILE_UTILS_HPP__
#define XTL__FILE_UTILS_HPP__ 1

#include <sys/stat.h>

#include <string>
#include <vector>

#include "../AccessMode.hpp"

namespace XTL
{
	class FilePath;

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

		/**
		 * @throw UnixError::AlreadyExists - если символическая ссылка уже существует.
		 * @throw UnixError - Системная ошибка.
		 */
		static void CreateSymlink(const std::string & linkPath, const std::string & destination);

		/**
		 * Создать символическую ссылку. Перезаписать, если она уже существует.
		 * @throw UnixError - Системная ошибка.
		 */
		static void RecreateSymlink(const std::string & linkPath, const std::string & destination);

		/**
		 * Получить цель символической ссылки, т.е. файл, на который она указывает.
		 */
		static const std::string ReadSymlink(const std::string & linkPath);

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
		static bool CreateDirectory(const std::string & directoryPath, AccessMode accessMode = AccessMode::DEFAULT_DIRECTORY);

		/**
		 * @return true     - Директория успешно создана.
		 * @return false    - Директория уже существует.
		 * @throw UnixError - Системная ошибка.
		 */
		static bool CreatePath(const FilePath & filePath, AccessMode accessMode = AccessMode::DEFAULT_DIRECTORY);

		/**
		 * @return true     - Директория успешно создана.
		 * @return false    - Директория уже существует.
		 * @throw UnixError - Системная ошибка.
		 */
		static bool CreatePath(const std::string & dirPath, AccessMode accessMode = AccessMode::DEFAULT_DIRECTORY);

		/**
		 * @return true     - Директория успешно создана.
		 * @return false    - Директория уже существует.
		 * @throw UnixError - Системная ошибка.
		 */
		static bool CreatePathForFile(const std::string & filePath, AccessMode accessMode = AccessMode::DEFAULT_DIRECTORY);

		static void SlurpFile(const std::string & filePath, std::vector<char> & content);
	};
}

#endif

