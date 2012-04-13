#ifndef XTL__STRING_OUTPUT_STREAM_HPP__
#define XTL__STRING_OUTPUT_STREAM_HPP__ 1

#include <string>
#include <deque>

#include "OutputStream.hpp"

namespace XTL
{
	class StringOutputStream : public OutputStream
	{
		public:

			StringOutputStream();

			virtual ~StringOutputStream() throw() { ;; }

			virtual void Write(const void * buffer, unsigned int size);

			const std::string ToString() const;

		private:

			std::deque<char> chars_;
	};
}

#endif

