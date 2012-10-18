#include "StringsTable.hpp"

namespace XTL
{
	/********** StringsTable::ColumnDesc **********/

	StringsTable::ColumnDesc::ColumnDesc()
		: alignment(LEFT),
		  width(0),
		  title()
	{
		;;
	}

	void StringsTable::ColumnDesc::UpdateWidth(unsigned int newWidth)
	{
		if (newWidth > width)
		{
			width = newWidth;
		}
	}

	/********** StringsTable::RowBuilder **********/

	StringsTable::RowBuilder::RowBuilder(StringsTable & table, StringsVector & values)
		: table_(table),
		  values_(values)
	{
		;;
	}

	StringsTable::RowBuilder StringsTable::RowBuilder::Row() const
	{
		return table_.Row();
	}

	StringsTable::RowBuilder & StringsTable::RowBuilder::Column(const std::string & value)
	{
		table_.UpdateColumnWidth(values_.Size(), value.length());
		values_.PushBack(value);
		return *this;
	}
}
