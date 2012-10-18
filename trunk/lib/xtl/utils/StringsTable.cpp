#include "StringsTable.hpp"

namespace XTL
{
	/********** StringsTable::Row **********/

	StringsTable::Row::Row()
		: values_()
	{
		;;
	}

	unsigned int StringsTable::Row::Size() const
	{
		return values_.size();
	}

	void StringsTable::Row::PushBack(const std::string & value)
	{
		values_.push_back(value);
	}

	StringsTable::Row & StringsTable::Row::Set(unsigned int columnIndex, const std::string & value)
	{
		if (columnIndex >= values_.size())
		{
			values_.resize(columnIndex + 1);
		}

		values_[columnIndex] = value;
		return *this;
	}

	const std::string & StringsTable::Row::operator[] (unsigned int index) const
	{
		static const std::string EMPTY("");

		return index < values_.size() ? values_[index] : EMPTY;
	}

	/********** StringsTable::RowBuilder **********/

	StringsTable::RowBuilder::RowBuilder(StringsTable & table, Row & rowRef)
		: table_(table),
		  rowRef_(rowRef)
	{
		;;
	}

	StringsTable::RowBuilder StringsTable::RowBuilder::AddRow() const
	{
		return table_.AddRow();
	}

	StringsTable::RowBuilder & StringsTable::RowBuilder::AddColumn(const std::string & value)
	{
		table_.UpdateColumnWidth(rowRef_.Size(), value.length());
		rowRef_.PushBack(value);
		return *this;
	}

	/********** StringsTable::RowBuilder **********/

	namespace
	{
		std::auto_ptr<StringsTable::Row> NewRow()
		{
			return std::auto_ptr<StringsTable::Row>(new StringsTable::Row());
		}
	}

	StringsTable::StringsTable()
		: rows_(),
		  columnsWidths_()
	{
		;;
	}

	StringsTable::RowBuilder StringsTable::AddRow()
	{
		rows_.PushBack(NewRow());
		return RowBuilder(*this, *(rows_.Back()));
	}

	StringsTable & StringsTable::Set(unsigned int rowIndex, unsigned int columnIndex, const std::string & value)
	{
		Row & row = CreateRow(rowIndex);

		row.Set(columnIndex, value);

		UpdateColumnWidth(columnIndex, value.size());

		return *this;
	}

	unsigned int StringsTable::RowsCount() const
	{
		return rows_.Size();
	}

	unsigned int StringsTable::ColumnsCount() const
	{
		return columnsWidths_.size();
	}

	unsigned int StringsTable::ColumnWidth(unsigned int columnIndex) const
	{
		return columnsWidths_[columnIndex];
	}

	void StringsTable::UpdateColumnWidth(unsigned int columnIndex, unsigned int width)
	{
		if (columnIndex >= columnsWidths_.size())
		{
			columnsWidths_.resize(columnIndex + 1);
		}

		if (columnsWidths_[columnIndex] < width)
		{
			columnsWidths_[columnIndex] = width;
		}
	}

	const StringsTable::Row & StringsTable::GetRow(unsigned int rowIndex) const
	{
		static const Row EMPTY_ROW;

		if (rowIndex >= rows_.Size())
		{
			return EMPTY_ROW;
		}

		return rows_[rowIndex] != 0 ? *(rows_[rowIndex]) : EMPTY_ROW;
	}

	const StringsTable::Row & StringsTable::operator[] (unsigned int rowIndex) const
	{
		return GetRow(rowIndex);
	}

	StringsTable::Row & StringsTable::CreateRow(unsigned int rowIndex)
	{
		if (rowIndex < rows_.Size())
		{
			if (rows_[rowIndex] == 0)
			{
				rows_.Set(rowIndex, NewRow());
			}
		}
		else
		{
			rows_.Set(rowIndex, NewRow());
		}

		return *(rows_[rowIndex]);
	}
}
