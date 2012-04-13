#ifndef XTL__INPUT_STREAM_HPP__
#define XTL__INPUT_STREAM_HPP__ 1

#include "../Types.hpp"

namespace XTL
{
	class InputStream
	{
		public:

			virtual ~InputStream() throw() { ;; }

			virtual void Read(void * buffer, unsigned int size) = 0;

			virtual bool AtEnd() const = 0;
	};
}

#endif

