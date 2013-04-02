#ifndef XTL__FILE_HPP__
#define XTL__FILE_HPP__ 1

#include <sys/types.h>

#include <string>

#include "../AccessMode.hpp"
#include "../../Types.hpp"

namespace XTL
{
	class FileStats;

	/**
	 * @class File
	 * @brief Класс, представляющий объект файл. Является оберткой над системными вызовами файловых операций ввода/вывода.
	 *
	 * Конструктор копирования и оператор присваивания объектов класса разрешены.
	 * В деструкторе автоматическое закрытие дескриптора НЕ ВЫЗЫВАЕТСЯ!
	 */
	class File
	{
		private:

			/**
			 * @class OpenMode
			 * @brief Режим открытия файла.
			 *
			 * Объявлен в закрытой части класса File, поэтому объекты этого класса создавать запрещено.
			 */
			class OpenMode
			{
				public:

					OpenMode(int flags, int state)
						: flags_(flags), state_(state) { ;; }

					int Flags() const  { return flags_; }

					int State() const  { return state_; }

				private:

					const int flags_;
					const int state_;
			};

			class CreateMode
			{
				public:

					CreateMode(int flags, int state)
						: flags_(flags), state_(state) { ;; }

					int Flags() const  { return flags_; }

					int State() const  { return state_; }

				private:

					const int flags_;
					const int state_;
			};

			class SeekMode
			{
				public:

					explicit SeekMode(int whence)
						: whence_(whence) { ;; }

					int Whence() const  { return whence_; }

				private:

					const int whence_;
			};

		public:

			/**
			 * Открыть существующий файл для чтения и записи, в случае отсутствия создается новый файл.
			 * Разрешены операции: Read, Write, Seek.
			 */
			static const CreateMode CREATE_READ_WRITE;

			/**
			 * Открыть существующий файл для добавления в конец, в случае отстутсвия создается новый файл.
			 * Разрешены операции: Write.
			 */
			static const CreateMode CREATE_APPEND;

			/**
			 * Создать новый файл для чтения и записи. Если файл с указанным именем уже существует, то генерируется исключение.
			 * Разрешены операции: Read, Write, Seek.
			 */
			static const CreateMode CREATE_NEW_READ_WRITE;

			/**
			 * Создать новый файл для добавления в конец. Если файл с указанным именем уже существует, то генерируется исключение.
			 * Разрешены операции: Write.
			 */
			static const CreateMode CREATE_NEW_APPEND;

			/**
			 * Создать новый файл для чтения и записи. Если файл с указанным именем уже существует, то он будет удален.
			 * Разрешены операции: Read, Write, Seek.
			 */
			static const CreateMode RECREATE_READ_WRITE;

			/**
			 * Создать новый файл для добавления в конец. Если файл с указанным именем уже существует, то он будет удален.
			 * Разрешены операции: Write.
			 */
			static const CreateMode RECREATE_APPEND;

			/**
			 * Открыть существующий файл только для чтения.
			 * Разрешены операции: Read, Seek.
			 */
			static const OpenMode OPEN_READ_ONLY;

			/**
			 * Открыть существующий файл для чтения и записи.
			 * Разрешены операции: Read, Write, Seek.
			 */
			static const OpenMode OPEN_READ_WRITE;

			/**
			 * Открыть существующий файл для добавления в конец.
			 * Разрешены операции: Write.
			 */
			static const OpenMode OPEN_APPEND;

			static const SeekMode SEEK_FROM_BEGIN;
			static const SeekMode SEEK_FROM_END;
			static const SeekMode SEEK_FROM_CURRENT;

			/**
			 * @enum State
			 * @brief Состояния в которых может находиться файловый объект.
			 */
			enum State
			{
				CLOSED    = 0x0000, ///< Файл закрыт
				READABLE  = 0x0001, ///< Из файла разрешено читать данные
				WRITEABLE = 0x0002, ///< В файл разрешено записывать данные
				SEEKABLE  = 0x0004, ///< Разрешено изменять позицию чтения/записи
				LOCKED    = 0x0008  ///< Файл заблокирован
			};

			static const unsigned int MAX_INTERRUPTS_IGNORES = 4;

			/**
			 * @brief Конструктор по умолчанию.
			 * После создания файл закрыт.
			 */
			explicit File(const std::string & filePath);

			int GetState() const
			{
				return state_;
			}

			const std::string & Path() const
			{
				return filePath_;
			}

			/**
			 * @brief Проверка, открыт ли файл в текущий момент.
			 * @return true - если открыт, false - в противном случае.
			 */
			bool IsOpened() const
			{
				return desc_ != -1;
			}

			/**
			 * @brief Проверка, закрыт ли файл в текущий момент.
			 * @return true - если закрыт, false - в противном случае.
			 */
			bool IsClosed() const
			{
				return desc_ == -1;
			}

			/**
			 * @throw IllegalOperationError - файл уже открыт.
			 * @throw AlreadyExistsError    - файл уже существует (в режиме CREATE_NEW_*).
			 * @throw UnixError             - системная ошибка.
			 */
			void Create(CreateMode mode, AccessMode accessMode = AccessMode::DEFAULT_FILE);

			/**
			 * @brief Открыть файл в заданном режиме.
			 * @param fileName  name of file to open
			 * @param mode      Режим открытия файла. Одна из констант
			 *                  перечисления OpeningMode.
			 * @return true  - если файл успешно открыт/создан,
			 *         false - в противном случае.
			 * @throw IllegalOperationError - файл уже открыт.
			 * @throw UnixError             - системная ошибка.
			 */
			void Open(OpenMode mode);

			/**
			 * @throw UnixError - системная ошибка.
			 * Если файл уже закрыт, то ничего не происходит.
			 */
			void Close();

			/**
			 * @throw IllegalOperationError  Файл не открыт.
			 * @throw UnixError              Системная ошибка.
			 * @throw InterruptedError       Системный вызов прерван по сигналу более, чем MAX_INTERRUPTS_IGNORES раз.
			 */
			void Flush();

			/**
			 * @return true  - Файл успешно удален.
			 * @return false - Файл не существует.
			 * @throw UnixError Системная ошибка.
			 */
			bool Unlink();

			bool IsReadable() const
			{
				return (state_ & READABLE) != 0;
			}

			bool IsWriteable() const
			{
				return (state_ & WRITEABLE) != 0;
			}

			bool IsSeekable() const
			{
				return (state_ & SEEKABLE) != 0;
			}

			/**
			 * @brief Прочитать из текущей позиции в файле указанное количество байт.
			 * @param buffer
			 * @param size
			 * @return true - если открыт, false - в противном случае.
			 * @throw IllegalOperationError  Файл не открыт или чтение из файла запрещено (например, файл открыт только для добавления в конец).
			 * @throw UnixError              Системная ошибка.
			 * @throw InterruptedError       Системный вызов прерван по сигналу более, чем MAX_INTERRUPTS_IGNORES раз, и из файла не было прочитано ни одного байта.
			 */
			unsigned int Read(void * buffer, unsigned int size);

			/**
			 * @throw IllegalOperationError  Файл не открыт или запись в файл запрещена (например, файл открыт только для чтения).
			 * @throw UnixError              Системная ошибка.
			 * В случае, когда системный вызов прерван по сигналу более, чем MAX_INTERRUPTS_IGNORES раз, возвращается число записанных в файл байт, возможно равное нулю.
			 */
			unsigned int Write(const void * buffer, unsigned int size);

			unsigned int Write(const std::string & buffer);

			/**
			 * @throw IllegalOperationError  Файл не открыт или изменение позиции в файле запрещено (например, файл открыт для добавления в конец).
			 * @throw UnixError              Системная ошибка.
			 */
			FileSize Seek(FileSize offset, SeekMode mode = SEEK_FROM_BEGIN);

			FileSize SeekFromBegin(FileSize offset)
			{
				return Seek(offset, SEEK_FROM_BEGIN);
			}

			FileSize SeekFromEnd(FileSize offset)
			{
				return Seek(offset, SEEK_FROM_END);
			}

			FileSize SeekFromCurrent(FileSize offset)
			{
				return Seek(offset, SEEK_FROM_CURRENT);
			}

			void Rename(const std::string & newFilePath);

			/**
			 * @throw UnixError              Системная ошибка.
			 */
			void GetStats(FileStats & stats) const;

			FileSize Size() const;

		protected:

			int state_;
			int desc_;
			/*
				TODO: Путь к файлу необходим для указания его в выбрасываемых исключениях, типа UnixError.
			*/
			std::string filePath_; // Должен быть модификатор const, но убран, чтобы работал оператор присваивания. Нафига нам оператор присваивания?
	};

	class FileCloseSentinel
	{
		public:

			explicit FileCloseSentinel(File & file)
				: file_(file)
			{
				;;
			}

			~FileCloseSentinel() throw()
			{
				file_.Close();
			}

		private:

			FileCloseSentinel(const FileCloseSentinel &);
			FileCloseSentinel & operator= (const FileCloseSentinel &);

			File & file_;
	};
}

#endif
