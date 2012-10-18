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

			void PrintSpaces(unsigned int count);
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

	template <char C, unsigned int CAPACITY = 256>
	class CharRepeater
	{
		public:

		static void Print(PrintStream & printStream, unsigned int count)
		{
			static const std::string SOURCE(CAPACITY, C);

			while (count > SOURCE.size())
			{
				printStream.Print(SOURCE);
				count -= SOURCE.size();
			}

			printStream.Print(SOURCE.data(), count);
		}
	};

	void PrintStringAligned(PrintStream & printStream, const std::string & value, unsigned int alignment, unsigned int width)
	{
		if (width <= value.size())
		{
			printStream.Print(value);
			return;
		}

		unsigned int spaces = width - value.size();

		if (alignment == XTL::ALIGNMENT::RIGHT)
		{
		}
		else if (alignment == XTL::ALIGNMENT::CENTER)
		{
		}
		else // alignment == XTL::ALIGNMENT::LEFT
		{
		}

		switch (alignment)
		{
			case RIGHT:
				printStream.PrintSpaces(spaces);
				printStream.Print(value);
				break;

			case CENTER:
				printStream.PrintSpaces(spaces / 2);
				printStream.Print(value);
				printStream.PrintSpaces(spaces - spaces / 2);
				break;

			default: // LEFT
				printStream.Print(value);
				printStream.PrintSpaces(spaces);
				break;
		}
	}

}

#endif
