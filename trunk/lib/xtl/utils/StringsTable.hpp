#ifndef XTL__STRINGS_TABLE_HPP__
#define XTL__STRINGS_TABLE_HPP__ 1

#include <string>
#include <vector>

#include "AutoPtrVector.hpp"

namespace XTL
{
	class StringAlignment
	{
		public:



		private:

			unsigned int id_;
	};

	class StringsVector
	{
		public:

			StringsVector()
				: values_()
			{
			}

			unsigned int Size() const
			{
				return values_.size();
			}

			void PushBack(const std::string & value)
			{
				values_.push_back(value);
			}

			StringsVector & Set(unsigned int columnIndex, const std::string & value)
			{
				if (columnIndex >= values_.size())
				{
					values_.resize(columnIndex + 1);
				}

				values_[columnIndex] = value;
				return *this;
			}

			const std::string & operator[] (unsigned int index) const
			{
				static const std::string EMPTY("");
				return index < values_.size() ? values_[index] : EMPTY;
			}

		private:

			std::vector<std::string> values_;
	};

	class StringsTable
	{
		public:

			enum Alignment
			{
				LEFT   = 0,
				CENTER = 1,
				RIGHT  = 2
			};

			struct ColumnDesc
			{
				ColumnDesc();

				void UpdateWidth(unsigned int newWidth);

				unsigned int alignment;
				unsigned int width;
				std::string  title;
			};

			class RowBuilder
			{
				public:

					RowBuilder(StringsTable & table, StringsVector & values);

					RowBuilder Row() const;

					RowBuilder & Column(const std::string & value);

				private:

					StringsTable  & table_;
					StringsVector & values_;
			};

			StringsTable()
				: rows_(),
				  columns_()
			{
				;;
			}

			StringsTable & SetColumn(unsigned int columnIndex, unsigned int alignment, const std::string & title)
			{
				ColumnDesc & columnDesc = CreateColumn(columnIndex);
				columnDesc.alignment = alignment;
				columnDesc.title = title;

				columnDesc.UpdateWidth(title.size());

				return *this;
			}

			RowBuilder Row()
			{
				rows_.PushBack(NewStringsVector());
				return RowBuilder(*this, *(rows_.Back()));
			}

			StringsTable & Set(unsigned int rowIndex, unsigned int columnIndex, const std::string & value)
			{
				StringsVector & values = CreateRow(rowIndex);

				values.Set(columnIndex, value);

				CreateColumn(columnIndex).UpdateWidth(value.size());

				return *this;
			}

			unsigned int RowsCount() const
			{
				return rows_.Size();
			}

			unsigned int ColumnsCount() const
			{
				return columns_.size();
			}

			const std::string & ColumnTitle(unsigned int columnIndex) const
			{
				return columns_[columnIndex].title;
			}

			unsigned int ColumnAlignment(unsigned int columnIndex) const
			{
				return columns_[columnIndex].alignment;
			}

			unsigned int ColumnWidth(unsigned int columnIndex) const
			{
				return columns_[columnIndex].width;
			}

			void UpdateColumnWidth(unsigned int columnIndex, unsigned int width)
			{
				CreateColumn(columnIndex).UpdateWidth(width);
			}

			const StringsVector & GetRow(unsigned int rowIndex) const
			{
				static const StringsVector EMPTY_ROW;

				if (rowIndex >= rows_.Size())
				{
					return EMPTY_ROW;
				}

				return rows_[rowIndex] != 0 ? *(rows_[rowIndex]) : EMPTY_ROW;
			}

			const StringsVector & operator[] (unsigned int rowIndex) const
			{
				return GetRow(rowIndex);
			}

		private:

			std::auto_ptr<StringsVector> NewStringsVector() const
			{
				return std::auto_ptr<StringsVector>(new StringsVector());
			}

			StringsVector & CreateRow(unsigned int rowIndex)
			{
				if (rowIndex < rows_.Size())
				{
					if (rows_[rowIndex] == 0)
					{
						rows_.Set(rowIndex, NewStringsVector());
					}
				}
				else
				{
					rows_.Set(rowIndex, NewStringsVector());
				}

				return *(rows_[rowIndex]);
			}

			ColumnDesc & CreateColumn(unsigned int columnIndex)
			{
				if (columnIndex >= columns_.size())
				{
					columns_.resize(columnIndex + 1);
				}

				return columns_[columnIndex];
			}


			XTL::AutoPtrVector<StringsVector> rows_;
			std::vector<ColumnDesc> columns_;

	};
}

#endif
