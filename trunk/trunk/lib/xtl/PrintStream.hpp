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

			virtual void Print(const std::string & s) = 0;

			template <typename T1>
			void Print(const char * format, const T1 & t1)
			{
				Print(FormatString(format, t1));
			}

			template <typename T1, typename T2>
			void Print(const char * format, const T1 & t1, const T2 & t2)
			{
				Print(FormatString(format, t1, t2));
			}
	};

	class StandardPrintStream : public PrintStream
	{
		public:

			static PrintStream & Out();

			static PrintStream & Err();

			virtual ~StandardPrintStream() throw();

			virtual void Print(const char * s);

			virtual void Print(const std::string & s);

		private:

			explicit StandardPrintStream(FILE * stream);

			FILE * stream_;
	};
}

#endif
