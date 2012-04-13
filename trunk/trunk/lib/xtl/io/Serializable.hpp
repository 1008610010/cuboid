#ifndef XTL__SERIALIZABLE_HPP__
#define XTL__SERIALIZABLE_HPP__ 1

#include "InputStream.hpp"
#include "OutputStream.hpp"

namespace XTL
{
	class Serializable
	{
		public:

			virtual ~Serializable() throw() { ;; }

			virtual void Read(InputStream & stream) = 0;

			virtual void Write(OutputStream & stream) = 0;
	};
}

#endif

