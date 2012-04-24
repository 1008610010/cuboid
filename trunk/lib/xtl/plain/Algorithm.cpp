#include "Algorithm.hpp"

namespace XTL
{
namespace PLAIN
{
	void Swap(RecordRef & left, RecordRef & right, RecordRef & temp)
	{
		temp = left;
		left = right;
		right = temp;
	}

	void ArrangeByIndex(RecordArray & array, std::vector<int> & index)
	{
		Record temp(array.SharedPrototype());

		for (int i = 0; i < static_cast<int>(array.Size()); ++i)
		{
			if (i == index[i] || index[i] == -1)
			{
				continue;
			}

			temp.Assign(array[i]);
			int to = i;
			int from = index[to];

			do
			{
				array[to].Assign(array[from]);
				index[to] = -1;
				to = from;
				from = index[from];
			}
			while (from != i);

			array[to].Assign(temp);
			index[to] = -1;
		}
	}
}
}

