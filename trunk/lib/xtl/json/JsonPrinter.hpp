#ifndef XTL__JSON_PRINTER_HPP__
#define XTL__JSON_PRINTER_HPP__ 1

#include <memory>

#include <xtl/PrintStream.hpp>

namespace XTL
{
	namespace JSON
	{
		struct Null {};

		struct Array {};

		struct Object {};

		struct End {};

		struct Finish {};

		struct LinearArray {};

		struct LinearObject {};

		struct LinearFinish {};
	}

	class JsonPrinterStack;

	class JsonPrinter
	{
		public:

			JsonPrinter(PrintStream & printStream);

			JsonPrinter & operator<< (JSON::Null);

			JsonPrinter & operator<< (bool value);

			JsonPrinter & operator<< (const char * value);

			JsonPrinter & operator<< (const std::string & value);

			JsonPrinter & operator<< (char value);

			JsonPrinter & operator<< (int value);

			JsonPrinter & operator<< (unsigned int value);

			JsonPrinter & operator<< (long int value);

			JsonPrinter & operator<< (unsigned long int value);

			JsonPrinter & operator<< (long long int value);

			JsonPrinter & operator<< (unsigned long long int value);

			JsonPrinter & operator<< (double value);

			JsonPrinter & operator<< (JSON::LinearArray);

			JsonPrinter & operator<< (JSON::LinearObject);

			JsonPrinter & operator<< (JSON::Array);

			JsonPrinter & operator<< (JSON::Object);

			JsonPrinter & operator<< (JSON::End);

			JsonPrinter & operator<< (JSON::LinearFinish);

			JsonPrinter & operator<< (JSON::Finish);

		private:

			void Char(char c)
			{
				printStream_.Print(&c, 1);
			}

			void Space()
			{
				// spacer_.WriteSpace(printStream_);
			}

			void NextLine()
			{
				// spacer_.WriteNextLine(printStream_);
			}

			void IncIndent()
			{
				// spacer_.IncIndent();
			}

			void RaiseError(const char * what);

			PrintStream & printStream_;
			std::auto_ptr<JsonPrinterStack> stack_;
	};
}

#endif
