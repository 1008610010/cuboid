#ifndef XTL__STRINGS_TABLE_HPP__
#define XTL__STRINGS_TABLE_HPP__ 1

#include <string>
#include <vector>

#include "AutoPtrVector.hpp"

namespace XTL
{
	class StringsTable
	{
		public:

			class Row
			{
				public:

					Row();

					unsigned int Size() const;

					void PushBack(const std::string & value);

					Row & Set(unsigned int columnIndex, const std::string & value);

					const std::string & operator[] (unsigned int index) const;

				private:

					std::vector<std::string> values_;
			};

			class RowBuilder
			{
				public:

					RowBuilder(StringsTable & table, Row & rowRef);

					RowBuilder AddRow() const;

					RowBuilder & AddColumn(const std::string & value);

				private:

					StringsTable & table_;
					Row          & rowRef_;
			};

			StringsTable();

			RowBuilder AddRow();

			StringsTable & Set(unsigned int rowIndex, unsigned int columnIndex, const std::string & value);

			unsigned int RowsCount() const;

			unsigned int ColumnsCount() const;

			unsigned int ColumnWidth(unsigned int columnIndex) const;

			void UpdateColumnWidth(unsigned int columnIndex, unsigned int width);

			const Row & GetRow(unsigned int rowIndex) const;

			const Row & operator[] (unsigned int rowIndex) const;

		private:

			Row & CreateRow(unsigned int rowIndex);

			XTL::AutoPtrVector<Row>   rows_;
			std::vector<unsigned int> columnsWidths_;
	};
}

#endif
