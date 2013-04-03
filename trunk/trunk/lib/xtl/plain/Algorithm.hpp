#ifndef XTL_PLAIN__ALGORITHM_HPP__
#define XTL_PLAIN__ALGORITHM_HPP__ 1

#include <stdexcept>

#include <xtl/Exception.hpp>
#include <xtl/FormatString.hpp>
#include <xtl/io/SeekableInputStream.hpp>

#include "Record.hpp"
#include "RecordArray.hpp"

namespace XTL
{
namespace PLAIN
{
	void Swap(RecordRef & left, RecordRef & right, RecordRef & temp);

	/**
	 * int RecordComparator_::operator() (RecordRef ref);
	 */
	template <typename RecordComparator_>
	bool BinarySearch(XTL::SeekableInputStream & inputStream, RecordRef ref, const RecordComparator_ & comp)
	{
		unsigned int recordSize = ref.Size();

		if (recordSize == 0)
		{
			throw ILLEGAL_OPERATION_ERROR("Record size is zero");
		}

		if (inputStream.Size() % recordSize != 0)
		{
			throw std::runtime_error(XTL::FormatString("Input stream size (%u) is not a multiple of the record size (%u)", inputStream.Size(), recordSize));
		}

		unsigned int left = 0;
		unsigned int right = inputStream.Size() / recordSize;

		while (left < right)
		{
			unsigned int middle = (left + right) / 2;

			inputStream.Seek(middle * recordSize);
			ref.Read(inputStream);

			int result = comp(ref);

			if (result < 0)
			{
				right = middle;
			}
			else if (result > 0)
			{
				left = middle + 1;
			}
			else
			{
				return true;
			}
		}

		return false;
	}

	template <typename RecordComparator>
	void QuickSort(RecordArray & array, std::vector<int> & index, int low, int high, const RecordComparator & comp)
	{
		int i = low;
		int j = high;

		RecordConstRef x = array[index[(low + high) / 2]];

		do
		{
			while (comp(array[index[i]], x))
			{
				++i;
			}

			while (comp(x, array[index[j]]))
			{
				--j;
			}

			if (i < j)
			{
				std::swap(index[i], index[j]);
				++i;
				--j;
			}
			else if (i == j)
			{
				++i;
				--j;
			}
		}
		while (i <= j);

		if (low < j)
		{
			QuickSort(array, index, low, j, comp);
		}

		if (i < high)
		{
			QuickSort(array, index, i, high, comp);
		}
	}

	template <typename RecordComparator_>
	void BuildIndex(RecordArray & array, std::vector<int> & index, const RecordComparator_ & comp)
	{
		index.resize(array.Size(), 0);

		for (int i = 0; i < static_cast<int>(array.Size()); ++i)
		{
			index[i] = i;
		}

		if (array.Size() <= 1)
		{
			return;
		}

		QuickSort(array, index, 0, array.Size() - 1, comp);
	}

	void ArrangeByIndex(RecordArray & array, std::vector<int> & index);

	template <typename RecordComparator_>
	void Sort(RecordArray & array, const RecordComparator_ & comp)
	{
		std::vector<int> index(array.Size(), 0);

		BuildIndex(array, index, comp);

		ArrangeByIndex(array, index);
	}
}
}

#endif

