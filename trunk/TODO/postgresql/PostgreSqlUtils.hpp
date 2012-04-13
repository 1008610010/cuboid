#ifndef _XC3__POSTGRESQL_UTILS_HPP__
#define _XC3__POSTGRESQL_UTILS_HPP__ 1

namespace XC3
{
	template <typename T> T ReverseBytes(T value)
	{
		T temp;
		for (unsigned int i = 0; i < sizeof(T); i++)
		{
			reinterpret_cast<char *>(&temp)[i] = ((CHAR_8*) &value)[sizeof(T) - 1 - i];
		}
		return temp;
	}

	template <typename T> void DestroyCollection(T & collection)
	{
		for (typename T::const_iterator itr = collection.begin(); itr != collection.end(); ++itr)
		{
			delete *itr;
		}
		collection.clear();
	}
}

#endif
