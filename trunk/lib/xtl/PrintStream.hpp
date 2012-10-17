#ifndef XTL__PRINT_STREAM_HPP__
#define XTL__PRINT_STREAM_HPP__ 1

#include <string>

#include "FormatString.hpp"

namespace XTL
{
	class PrintStream
	{
		public:

			virtual ~PrintStream() throw()
			{
				;;
			}

			virtual void Print(const char * s) = 0;

			virtual void Print(const char * s, unsigned int size) = 0;

			void Print(const std::string & s)
			{
				Print(s.data(), static_cast<unsigned int>(s.size()));
			}

			template <typename T1>
			void PrintF(const char * format, const T1 & t1)
			{
				Print(FormatString(format, t1));
			}

			template <typename T1, typename T2>
			void PrintF(const char * format, const T1 & t1, const T2 & t2)
			{
				Print(FormatString(format, t1, t2));
			}
	};

	PrintStream & StdOut();

	PrintStream & StdErr();

	class StandardPrintStream : public PrintStream
	{
		public:

			explicit StandardPrintStream(FILE * stream);

			virtual ~StandardPrintStream() throw();

			virtual void Print(const char * s);

			virtual void Print(const char * s, unsigned int size);

		private:

			FILE * stream_;
	};
}

#endif
