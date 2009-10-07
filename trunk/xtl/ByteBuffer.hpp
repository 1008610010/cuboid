#ifndef _XTL__BYTE_BUFFER_HPP__
#define _XTL__BYTE_BUFFER_HPP__ 1

#include <string>

/**
 * @namespace XTL Extended Template Library
 */
namespace XTL
{
	/**
	 * @class ByteBuffer
	 * @brief Realize autogrowing buffer of bytes.
	 *
	 * While need growing, capacity of buffer doubles.
	 */
	class ByteBuffer
	{
		/**
		 * @var Minimal size of allocated memory.
		 */
		static const int MIN_CAPACITY = 16;

		public:

			/**
			 * @brief Default constructor. Does not allocate memory.
			 */
			ByteBuffer();

			/**
			 * @brief Constructor with preallocating memory.
			 * @param capacity Initial capacity of the creating buffer.
			 * @exception OutOfMemoryException
			 */
			explicit ByteBuffer(int capacity);

			/**
			 * @brief Copy constructor
			 * @param buffer Another buffer object.
			 * @exception OutOfMemoryException
			 */
			ByteBuffer(const ByteBuffer & buffer);

			/**
			 * @brief Destructor
			 */
			~ByteBuffer() throw();

			/**
			 * @brief Get current capacity of the buffer.
			 * @return Current capacity of the buffer.
			 */
			int Capacity() const;

			/**
			 * @brief Get current size of the buffer.
			 * @return Current size of the buffer.
			 */
			int Size() const;

			/**
			 * @brief Get pointer to the buffer data.
			 * @return Pointer to the buffer data.
			 */
			const char * Data() const;

			/**
			 * @brief Check, if the buffer is empty.
			 * @return true, if the buffer is empty;
			 *        false, otherwise.
			 */
			bool Empty() const;

			/**
			 * @brief Reserve buffer memory for appending 'size' bytes.
			 * @param size Size of additional memory to reserve.
			 * @exception OutOfMemoryException
			 */
			void Reserve(int size);

			/**
			 * @brief Free buffer memory.
			 */
			void Free();

			/**
			 * @brief Clear buffer.
			 */
			void Clear();

			/**
			 * @brief Assignment operator.
			 * @exception OutOfMemoryException
			 * @return Reference to self.
			 */
			ByteBuffer & operator= (const ByteBuffer & buffer);

			/**
			 * @brief Swap contents of the buffer with another one.
			 * @param buffer Another buffer object
			 */
			void Swap(ByteBuffer & buffer);

			ByteBuffer & Append(const void * value, int size);

			ByteBuffer & Append(const ByteBuffer & buffer);

			ByteBuffer & Append(const char * value);

			ByteBuffer & Append(const std::string & value);

			template <typename T> ByteBuffer & Append(const T & value);

			ByteBuffer & operator<< (const ByteBuffer & buffer);

			ByteBuffer & operator<< (const char * value);

			ByteBuffer & operator<< (const std::string & value);

			template <typename T> ByteBuffer & operator<< (const T & value);

			/**
			 * @brief Erase first 'size' bytes from the beginning of buffer and shift tail.
			 * @param size Size of data to erase.
			 */
			void Shift(int size);

		protected:

			int    capacity_; /// Size of allocated memory, pointed by data_.
			int    size_;     /// Size of used memory.
			char * data_;     /// Pointer to allocated memory.
	};

	template <typename T> ByteBuffer & ByteBuffer::Append(const T & value)
	{
		return Append(&value, sizeof(T));
	}

	template <typename T> ByteBuffer & ByteBuffer::operator<< (const T & value)
	{
		return Append(value);
	}
}

#endif

