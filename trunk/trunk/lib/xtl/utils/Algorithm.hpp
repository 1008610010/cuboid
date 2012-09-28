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

	template <typename T1>
	bool CascadeCompare(const T1 & t1left, const T1 & t1right)
	{
		return t1left < t1right;
	}

	template <typename T1, typename T2>
	bool CascadeCompare(const T1 & t1left, const T1 & t1right,
	                    const T2 & t2left, const T2 & t2right)
	{
		if (CascadeCompare(t1left, t1right))
		{
			return true;
		}
		else if (CascadeCompare(t1right, t1left))
		{
			return false;
		}

		return CascadeCompare(t2left, t2right);
	}

	template <typename T1, typename T2, typename T3>
	bool CascadeCompare(const T1 & t1left, const T1 & t1right,
	                    const T2 & t2left, const T2 & t2right,
	                    const T3 & t3left, const T3 & t3right)
	{
		if (CascadeCompare(t1left, t1right))
		{
			return true;
		}
		else if (CascadeCompare(t1right, t1left))
		{
			return false;
		}

		return CascadeCompare(t2left, t2right, t3left, t3right);
	}

	template <typename T1, typename T2, typename T3, typename T4>
	bool CascadeCompare(const T1 & t1left, const T1 & t1right,
	                    const T2 & t2left, const T2 & t2right,
	                    const T3 & t3left, const T3 & t3right,
	                    const T4 & t4left, const T4 & t4right)
	{
		if (CascadeCompare(t1left, t1right))
		{
			return true;
		}
		else if (CascadeCompare(t1right, t1left))
		{
			return false;
		}

		return CascadeCompare(t2left, t2right, t3left, t3right, t4left, t4right);
	}
}

#endif
