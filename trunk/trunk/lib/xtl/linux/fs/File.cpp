#include "File.hpp"

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>

#include "FileStats.hpp"
#include "FileUtils.hpp"
#include "../UnixError.hpp"

namespace XTL
{
	static const char * const ERROR_FILE_IS_CLOSED       = "File is closed";
	static const char * const ERROR_ALREADY_OPENED       = "File is already opened";
	static const char * const ERROR_FILE_IS_NOT_READABLE = "File is not readable";
	static const char * const ERROR_FILE_IS_NOT_WRITABLE = "File is not writable";
	static const char * const ERROR_FILE_IS_NOT_SEEKABLE = "File is not seekable";

	const File::CreateMode File::CREATE_READ_WRITE (
		O_CREAT | O_RDWR | O_LARGEFILE,
		READABLE | WRITEABLE | SEEKABLE
	);

	const File::CreateMode File::CREATE_APPEND (
		O_CREAT | O_WRONLY | O_APPEND | O_LARGEFILE,
		WRITEABLE
	);

	const File::CreateMode File::CREATE_NEW_READ_WRITE (
		O_CREAT | O_EXCL | O_RDWR | O_TRUNC | O_LARGEFILE,
		READABLE | WRITEABLE | SEEKABLE
	);

	const File::CreateMode File::CREATE_NEW_APPEND (
		O_CREAT | O_EXCL | O_WRONLY | O_APPEND | O_TRUNC | O_LARGEFILE,
		WRITEABLE
	);

	const File::CreateMode File::RECREATE_READ_WRITE (
		O_CREAT | O_RDWR | O_TRUNC | O_LARGEFILE,
		READABLE | WRITEABLE | SEEKABLE
	);

	const File::CreateMode File::RECREATE_APPEND (
		O_CREAT | O_WRONLY | O_APPEND | O_TRUNC | O_LARGEFILE,
		WRITEABLE
	);

	const File::OpenMode File::OPEN_READ_ONLY (
		O_RDONLY | O_LARGEFILE,
		READABLE | SEEKABLE
	);

	const File::OpenMode File::OPEN_READ_WRITE (
		O_RDWR | O_LARGEFILE,
		READABLE | WRITEABLE | SEEKABLE
	);

	const File::OpenMode File::OPEN_APPEND (
		O_WRONLY | O_APPEND | O_LARGEFILE,
		WRITEABLE
	);

	const File::SeekMode File::SEEK_FROM_BEGIN   (SEEK_SET);
	const File::SeekMode File::SEEK_FROM_END     (SEEK_END);
	const File::SeekMode File::SEEK_FROM_CURRENT (SEEK_CUR);

	File::File(const std::string & filePath)
		: state_(CLOSED),
		  desc_(-1),
		  filePath_(filePath)
	{
		if (filePath_.empty())
		{
			throw ILLEGAL_ARGUMENT_ERROR("fileName");
		}
	}

	void File::Create(CreateMode mode, AccessMode accessMode)
	{
		if (IsOpened())
		{
			throw ILLEGAL_OPERATION_ERROR(ERROR_ALREADY_OPENED);
		}

		desc_ = ::open(filePath_.c_str(), mode.Flags(), accessMode.Get());

		if (desc_ < 0)
		{
			state_ = CLOSED;
			desc_ = -1;
			if (errno == EEXIST)
			{
				throw UnixError::AlreadyExists();
			}
			else
			{
				throw UnixError();
			}
		}

		state_ = mode.State();
	}

	void File::Open(OpenMode mode)
	{
		if (IsOpened())
		{
			throw ILLEGAL_OPERATION_ERROR(ERROR_ALREADY_OPENED);
		}

		desc_ = ::open(filePath_.c_str(), mode.Flags());

		if (desc_ < 0)
		{
			state_ = CLOSED;
			desc_ = -1;
			throw UnixError();
		}

		state_ = mode.State();
	}

	void File::Close()
	{
		if (IsClosed())
		{
			return;
		}

		/* MAN: close()
		Закрывает файловый дескриптор, который после этого не ссылается
		ни на один файл и может быть использован повторно.
		Все блокировки, находящиеся на соответствующем файле, снимаются
		(независимо от того, был ли использован для установки блокировки
		именно этот файловый дескриптор).

		Если desc_ является последней копией какого-либо файлового
		дескриптора, то ресурсы, связанные с ним, освобождаются.
		Если дескриптор был последней ссылкой на файл, удаленный
		с помощью unlink(2), то файл окончательно удаляется.

		Проверка на успешное закрытие необходима, так как вполне
		возможно, что ошибка в предыдущей операции write(2) впервые даст
		о себе знать при выполнении close.

		Успешное  выполнение не гарантирует, что данные успешно записаны
		на диск, потому что ядро откладывает запись. Обычно файловые
		системы не сбрасывают буфера на диск при закрытии потока.
		Если вам нужно удостовериться, что данные физически сохранены на
		диске, используйте fsync(2). (В этом случае всё будет зависить
		от железа и самого диска.)
		*/

		if (::close(desc_) != 0)
		{
			state_ = CLOSED;
			desc_ = -1;
			throw UnixError();
		}

		state_ = CLOSED;
		desc_ = -1;
	}

	void File::Flush()
	{
		if (IsClosed())
		{
			throw ILLEGAL_OPERATION_ERROR(ERROR_FILE_IS_CLOSED);
		}

		unsigned int interruptsCount = MAX_INTERRUPTS_IGNORES;

		while (true)
		{
			if (::fsync(desc_) == 0)
			{
				break;
			}

			if (errno != EINTR)
			{
				throw UnixError();
			}

			if (interruptsCount-- == 0)
			{
				throw UnixError::Interrupted();
			}
		}
	}

	bool File::Unlink()
	{
		Close();

		if (::unlink(filePath_.c_str()) == 0)
		{
			return true;
		}

		if (errno == ENOENT)
		{
			return false;
		}
		else
		{
			throw UnixError();
		}
	}

	unsigned int File::Read(void * buffer, unsigned int size)
	{
		if (IsClosed())
		{
			throw ILLEGAL_OPERATION_ERROR(ERROR_FILE_IS_CLOSED);
		}

		if (!IsReadable())
		{
			throw ILLEGAL_OPERATION_ERROR(ERROR_FILE_IS_NOT_READABLE);
		}

		char * ptr = static_cast<char *>(buffer);
		unsigned int total = 0;
		unsigned int interruptsCount = MAX_INTERRUPTS_IGNORES;

		while (size > 0)
		{
			/* MAN: ssize_t read(int fd, void *buf, size_t count);
			Если count равен нулю, то read() возвращает ноль и ничего
			не делает. Если count больше, чем SSIZE_MAX, то результат
			неопределен.
			При успешном завершении возвращается количество байт,
			которые были прочитаны (ноль означает конец файла), а
			позиция в файле увеличивается на это значение.
			Если количество прочитанных байт меньше, чем количество
			запрошенных, то это не считается ошибкой: например, мы могли
			читать вблизи конца файла, или же из трубы, или с терминала,
			или же read() был прерван сигналом.
			В случае ошибки возвращается -1, а errno устанавливается
			должным образом. В этом случае позиция файла неопределена.

			If a read() is interrupted by a signal before it reads any
			data, it shall return -1 with errno set to [EINTR].
			If a read() is interrupted by a signal after it has
			successfully read some data, it shall return the number of
			bytes read.
			*/

			ssize_t count = ::read(desc_, ptr, size > SSIZE_MAX ? SSIZE_MAX : size);

			if (count > 0)
			{
				ptr += count;
				total += count;
				size -= count;
			}
			else if (count == 0)
			{
				return total;
			}
			else if (errno != EINTR)
			{
				throw UnixError();
			}
			else if (interruptsCount-- == 0)
			{
				if (total > 0)
				{
					return total;
				}
				else
				{
					throw UnixError::Interrupted();
				}
			}
		}

		return total;
	}

	unsigned int File::Write(const void * buffer, unsigned int size)
	{
		if (IsClosed())
		{
			throw ILLEGAL_OPERATION_ERROR(ERROR_FILE_IS_CLOSED);
		}

		if (!IsWriteable())
		{
			throw ILLEGAL_OPERATION_ERROR(ERROR_FILE_IS_NOT_WRITABLE);
		}

		if (buffer == 0)
		{
			throw ILLEGAL_ARGUMENT_ERROR("buffer");
		}

		const char * ptr = static_cast<const char *>(buffer);
		unsigned int total = 0;
		unsigned int interruptsCount = MAX_INTERRUPTS_IGNORES;

		while (size > 0)
		{
			/* MAN: write()
			В случае успеха, возвращается количество записанных байт
			(ноль говорит о том, что ничего записано не было).
			В случае ошибки, возвращается -1 и значение errno
			устанавливается соответствующим образом. Если count равен
			нулю и файловый дескриптор указывает на обычный файл, будет
			возвращаться 0 без каких-либо других последствий.
			*/

			ssize_t count = ::write(desc_, ptr, size);
			if (count >= 0)
			{
				ptr += count;
				total += count;
				size -= count;
			}
			else if (errno != EINTR)
			{
				throw UnixError();
			}
			else if (interruptsCount-- == 0)
			{
				return total;
			}
		}

		return total;
	}

	unsigned int File::Write(const std::string & buffer)
	{
		return Write(buffer.data(), buffer.size());
	}

	FileSize File::Seek(FileSize offset, SeekMode mode)
	{
		if (IsClosed())
		{
			throw ILLEGAL_OPERATION_ERROR(ERROR_FILE_IS_CLOSED);
		}

		if (!IsSeekable())
		{
			throw ILLEGAL_OPERATION_ERROR(ERROR_FILE_IS_NOT_SEEKABLE);
		}

		const off64_t result = ::lseek64(desc_, offset, mode.Whence());
		if (result < 0)
		{
			throw UnixError();
		}

		return result;
	}

	void File::Rename(const std::string & newFilePath)
	{
		FileUtils::Rename(filePath_, newFilePath);
		filePath_ = newFilePath;
	}

	void File::GetStats(FileStats & stats)
	{
		if (IsClosed())
		{
			throw ILLEGAL_OPERATION_ERROR(ERROR_FILE_IS_CLOSED);
		}

		return stats.Init(desc_);
	}
}

