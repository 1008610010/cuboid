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

			class Spacer
			{
				public:

					virtual ~Spacer() throw() { ;; }

					virtual void Space(PrintStream & printStream) = 0;

					virtual void NextLine(PrintStream & printStream) = 0;

					virtual void IncIndent() = 0;

					virtual void DecIndent() = 0;
			};

			class Literal
			{
				public:

					virtual ~Literal() { ;; }

					virtual bool CanBeObjectKey() const { return false; }

					virtual void Print(PrintStream & stream) const = 0;
			};

			JsonPrinter(PrintStream & printStream, Spacer & spacer);

			~JsonPrinter() throw();

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

			void operator<< (JSON::LinearFinish);

			void operator<< (JSON::Finish);

		private:

			JsonPrinter & operator<< (const Literal & literal);

			void Char(char c)
			{
				printStream_.Print(&c, 1);
			}

			void Space()
			{
				spacer_.Space(printStream_);
			}

			void NextLine()
			{
				spacer_.NextLine(printStream_);
			}

			void IncIndent()
			{
				spacer_.IncIndent();
			}

			void DecIndent()
			{
				spacer_.DecIndent();
			}

			void RaiseError(const char * what);

			PrintStream & printStream_;
			Spacer      & spacer_;
			std::auto_ptr<JsonPrinterStack> stack_;
	};
}

#endif
