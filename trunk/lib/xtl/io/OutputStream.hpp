#ifndef XTL__OUTPUT_STREAM_HPP__
#define XTL__OUTPUT_STREAM_HPP__ 1

namespace XTL
{
	class OutputStream
	{
		public:

			virtual ~OutputStream() throw() { ;; }

			virtual void Write(const void * buffer, unsigned int size) = 0;
	};
}

#endif

