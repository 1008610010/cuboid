#ifndef XTL__STRINGS_TABLE_PRINTER_HPP__
#define XTL__STRINGS_TABLE_PRINTER_HPP__ 1

#include "../PrintStream.hpp"
#include "StringsTable.hpp"

namespace XTL
{
	class CellAlignment
	{
		public:

			static const CellAlignment LEFT;
			static const CellAlignment CENTER;
			static const CellAlignment RIGHT;

			/**
			 * По умолчанию CellAlignment::LEFT
			 */
			CellAlignment();

			bool operator== (const CellAlignment & other) const;

		private:

			explicit CellAlignment(unsigned int id);

			unsigned int id_;
	};

	class StringsTablePrinter
	{
		public:

			explicit StringsTablePrinter(PrintStream & printStream);

			virtual ~StringsTablePrinter() throw();

			CellAlignment GetAlignment(unsigned int columnIndex) const;

			StringsTablePrinter & SetAlignment(unsigned int columnIndex, CellAlignment alignment);

			void Print(const StringsTable & table);

			/**
			 * Вывести строки из интервала [rowFrom, rowTo).
			 */
			virtual void Print(const StringsTable & table, unsigned int rowFrom, unsigned int rowTo);

		protected:

			bool PrepareBeforePrint(const StringsTable & table);

			void PrintString(const char * value);

			void PrintString(const std::string & value);

			void PrintStringAligned(const std::string & value, CellAlignment alignment, unsigned int width);

		private:

			PrintStream & printStream_;
			std::vector<CellAlignment> columns_;
	};
}

#endif
