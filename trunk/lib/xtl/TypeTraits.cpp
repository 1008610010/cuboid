#include "TypeTraits.hpp"

namespace XTL
{
	const char * const TypeTraits<int>::DecFormat                    = "%d";
	const char * const TypeTraits<long int>::DecFormat               = "%ld";
	const char * const TypeTraits<long long int>::DecFormat          = "%lld";
	const char * const TypeTraits<unsigned int>::DecFormat           = "%u";
	const char * const TypeTraits<unsigned long int>::DecFormat      = "%lu";
	const char * const TypeTraits<unsigned long long int>::DecFormat = "%llu";
}
