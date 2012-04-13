#ifndef _STXX__FILE_HPP__
#define _STXX__FILE_HPP__ 1

/**
 * @file File.hpp
 */

#include <sup/Types.h>
#include <unistd.h>

/**
 * @namespace STXX C++ file based storage
 */
namespace STXX
{
	/**
	 * @class File
	 * @brief Class, that represents file on disk
	 */
	class File
	{
		public:

			/**
			 * @enum Status
			 * @brief Enumeration of file status bits
			 */
			enum Status
			{
				STATUS_CAN_READ  = 0x0001, ///< file is readable
				STATUS_CAN_WRITE = 0x0002, ///< file is writable
				STATUS_LOCKED    = 0x0004  ///< file is locked
			};

			/**
			 * @enum OpeningMode
			 * @brief Enumeration of file opening modes
			 */
			enum OpeningMode
			{
				OPEN_READ       = 0, ///< open file for reading only
				OPEN_READ_WRITE = 1, ///< open file for reading and writing
				OPEN_OR_CREATE  = 2  ///< open file or create, if it does not exist for reading and writing
			};

			typedef UINT_64 AddressType;
			static const AddressType NIL = AddressType(-1);

			/**
			 * @brief Default constructor.
			 */
			File();

			/**
			 * @brief Destructor.
			 * @warning File do NOT close automatically during destruction.
			 */
			~File() throw();

			/**
			 * @brief Check, if file status have specified status flag.
			 * @param statusFlag - interesting flag
			 * @return true, if status have, false - otherwise.
			 */
			bool HasStatus(int statusFlag) const;

			/**
			 * @brief Check, if file is opened.
			 * @return true, if file is opened, false - otherwise.
			 */
			bool Opened() const;

			/**
			 * @brief Close file.
			 */
			void Close();

			/**
			 * @brief Create file with specified name or truncate if such already exists.
			 * @param fileName - name of file to create
			 * @return true, if file is created, false, if error occurs
			 * Current file will be closed regardless of this method result.
			 */
			bool Create(const CHAR_8 * fileName);

			/**
			 * @brief Open specified file using specified mode.
			 * @param fileName - name of file to open
			 * @param mode - mode of file opening, one of OpeningMode enumeration.
			 * @return true, if file is opened, false - otherwise
			 * Current file will be closed regardless of this method result.
			 */
			bool Open(const CHAR_8 * fileName, UINT_32 mode = OPEN_READ);

			/**
			 * @brief Lock or try to lock current file
			 * @param exclusive - if exclusive locking
			 * @param wait - wait for exclusive unlocking, if locked (maybe indefinitely)
			 * @return true, if file is now locked, false - otherwise
			 * Exclusive locking means lock for writing. Nonexclusive means lock for reading.
			 * Only one process can have write locking of specified file at the moment.
			 * Write locking can be set only if file is opened writable.
			 * This locking mechanism has 'per process' granularity not 'per thread'.
			 */
			bool Lock(bool exclusive, bool wait);

			/**
			 * @brief Unlock previously locked file
			 * @return true, if successfully unlocked, or false - otherwise
			 */
			bool Unlock();

			/**
			 * @brief Get current size of file.
			 * @return Current size of opened file, or zero, if file is closed.
			 */
			UINT_64 Size() const;

			/**
			 * @brief Get current position of file.
			 * @return Current position of opened file, or zero, if file is closed.
			 * Current position is the default position of the file, tha data is read from or
			 * is written to.
			 */
			UINT_64 Position() const;

			/**
			 * @brief Check if end of file is reached.
			 * @return true, if end of file is reached, false - otherwise.
			 */
			bool Eof() const;

			/**
			 * @brief Set new current position of file
			 * @param offset - number of bytes from beginning for new position
			 * @return true, if success, false - otherwise
			 */
			bool Seek(UINT_64 offset, int whence = SEEK_SET);

			/**
			 * @brief Truncate or enlarge file to specified size
			 * @param size - new size of file
			 * @return true, if success, false - otherwise
			 */
			bool Truncate(UINT_64 size);

			/**
			 * @brief Set new size of file
			 * @param size - new size of file
			 * @return true, if success, false - otherwise
			 * If size is smaller then current size - just truncating. If greater, then
			 * 'over-current-size' bytes will be actually write to the end of file.
			 */
			bool SetSize(UINT_64 size);

			/**
			 * @brief Read specified size of bytes from current position of the file.
			 * @param buffer - pointer to the destination, where read bytes will be placed
			 * @param size - number of bytes to read
			 * @return true, if success, false - otherwise
			 * Method fails in cases:
			 * - file is closed or unreadable;
			 * - there is not enough bytes to read from current position
			 * - some error occured, while reading
			 * After success execution current position of the file is incremented by specified size
			 */
			bool ReadBuffer(void * buffer, UINT_32 size);

			/**
			 * @brief Read specified size of bytes from specified offset of the file.
			 * @param buffer - pointer to the destination, where read bytes will be placed
			 * @param size - number of bytes to read
			 * @param offset - position from the beginning of the file, that reading starts from
			 * @return true, if success, false - otherwise
			 * @see ReadBuffer(void *, UINT_32)
			 * After success current position is offset + size
			 */
			bool ReadBuffer(void * buffer, UINT_32 size, UINT_64 offset);

			/**
			 * @brief Read value of specified type T from current position of the file.
			 * @param value - reference to the variable, where read value will be placed
			 * @return true, if success, false - otherwise
			 */
			template <typename T> bool Read(T & value);

			/**
			 * @brief Read value from the specified offset of the file.
			 * @param value - reference to the variable, where read value will be placed
			 * @param offset - position from the beginning of the file, that reading starts from
			 * @return true, if success, false - otherwise
			 * After success current position is offset + size
			 */
			template <typename T> bool Read(T & value, UINT_64 offset);

			/**
			 * @brief Write buffer with specified size to the current position of the file.
			 * @param buffer - pointer to the written data
			 * @param size - size in bytes of the written data
			 * @return true, if success, false - otherwise
			 * Method fails in cases:
			 * - file is closed or unwritable;
			 * - some error occured, while writing
			 * After success execution current position of the file is incremented by specified size.
			 */
			bool WriteBuffer(const void * buffer, UINT_32 size);

			/**
			 * @brief Write buffer with specified size to the specified offset of the file.
			 * @param buffer - pointer to the written data
			 * @param size - size in bytes of the written data
			 * @param offset - position from the beginning of the file, where buffer is written to
			 * @return true, if success, false - otherwise
			 * After success current position is offset + size
			 */
			bool WriteBuffer(const void * buffer, UINT_32 size, UINT_64 offset);

			/**
			 * @brief Append buffer with specified size to the end of the file.
			 * @param buffer - pointer to the appended buffer
			 * @param size - size in bytes of the appended buffer
			 * @return true, if success, false - otherwise
			 * After success current position points to the end of file
			 */
			bool AppendBuffer(const void * buffer, UINT_32 size);

			/**
			 * @brief Write value of the specified type to the current position of the file.
			 * @param value - constant reference to the written value
			 * @return Return value is the same as in method WriteBuffer(const void *, UINT_32)
			 * @see WriteBuffer(const void *, UINT_32)
			 */
			template <typename T> bool Write(const T & value);

			/**
			 * @brief Write value of the specified type to the specified offset of the file.
			 * @param value - constant reference to the written value
			 * @param offset - position from the beginning of the file, where value is written to
			 * @return Return value is the same as in method WriteBuffer(const void *, UINT_32, UINT_64)
			 * @see WriteBuffer(const void *, UINT_32, UINT_64)
			 */
			template <typename T> bool Write(const T & value, UINT_64 offset);

			/**
			 * @brief Append value to the end of the file.
			 * @param value - constant reference to the appended value
			 * @return Return value is the same as in method AppendBuffer(const void *, UINT_32)
			 * @see AppendBuffer(const void *, UINT_32)
			 */
			template <typename T> bool Append(const T & value);

			/**
			 * @brief Check, if file exists.
			 * @param fileName - name of checking file
			 * @return true, if file exists, false, if file doesn't exist or is forbidden
			 */
			static bool Exists(const CHAR_8 * fileName);

			/**
			 * @brief Remove file.
			 * @param fileName - name of file for removal
			 * @return true, if file was deleted, false, if some error occured
			 */
			static bool Remove(const CHAR_8 * fileName);

			/**
			 * @brief Get file size in bytes.
			 * @param fileName - name of file, which size is interesting
			 * @return Size of existing file, or zero, if error occured or such file does not exist
			 */
			static UINT_64 GetSize(const CHAR_8 * fileName);

			/**
			 * @brief Get last modified time as timestamp.
			 * @param fileName - name of file
			 * @return Last modified time in GMT format.
			 */
			static time_t GetModifiedTime(const CHAR_8 * fileName);

			/**
			 * @brief Rename file
			 * @param oldName - name of the renaming file
			 * @param newName - new name of the file
			 * @return true, if file is successfully renamed, false - otherwise
			 */
			static bool Rename(const CHAR_8 * oldName, const CHAR_8 * newName);

			/**
			 * @brief Flush file content on disk
			 * @return true on success
			 */
			bool Flush();

			/**
			 * @brief Get UNIX file descriptor
			 * @return file descriptor or -1 if file not opened
			 */
			int operator*() const { return desc_; }
		private:

			int     desc_;     ///< file descriptor, or -1 if closed
			int     status_;   ///< current status of the file @see Status
			UINT_64 size_;     ///< size of opened file, or 0 if closed
			UINT_64 position_; ///< position in opened file, or 0 if closed

			ssize_t SignalSafeRead(void *ptr, size_t size);
			ssize_t SignalSafeWrite(const void *ptr, size_t size);

			const static short int eintrIgnore_ = 5;

	};

	inline bool File::HasStatus(int statusFlag) const
	{
		return (status_ & statusFlag) != 0;
	}

	inline UINT_64 File::Size() const
	{
		return size_;
	}

	inline UINT_64 File::Position() const
	{
		return position_;
	}

	inline bool File::Eof() const
	{
		return position_ == size_;
	}

	inline bool File::ReadBuffer(void * buffer, UINT_32 size, UINT_64 offset)
	{
		return Seek(offset) && ReadBuffer(buffer, size);
	}

	inline bool File::WriteBuffer(const void * buffer, UINT_32 size, UINT_64 offset)
	{
		return Seek(offset) && WriteBuffer(buffer, size);
	}

	inline bool File::AppendBuffer(const void * buffer, UINT_32 size)
	{
		return WriteBuffer(buffer, size, Size());
	}

	template <typename T> inline bool File::Read(T & value)
	{
		return ReadBuffer(&value, sizeof(T));
	}

	template <typename T> inline bool File::Read(T & value, UINT_64 offset)
	{
		return ReadBuffer(&value, sizeof(T), offset);
	}

	template <typename T> inline bool File::Write(const T & value)
	{
		return WriteBuffer(&value, sizeof(T));
	}

	template <typename T> inline bool File::Write(const T & value, UINT_64 offset)
	{
		return WriteBuffer(&value, sizeof(T), offset);
	}

	template <typename T> inline bool File::Append(const T & value)
	{
		return Write(value, Size());
	}

}

#endif
