#ifndef XTL__ALGORITHM_HPP__
#define XTL__ALGORITHM_HPP__ 1

namespace XTL
{
	template <typename T>
	const T Min(T x, T y)
	{
		return x < y ? x : y;
	}

	template <typename T>
	const T Max(T x, T y)
	{
		return x < y ? y : x;
	}

	template <typename T>
	T Pow(T x, unsigned int n)
	{
		T result = 1;

		while (n != 0)
		{
			if (n % 2 == 0)
			{
				n /= 2;
				x *= x;
			}
			else
			{
				--n;
				result *= x;
			}
		}

		return result;
	}
}

#endif

