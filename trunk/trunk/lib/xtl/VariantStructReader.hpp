#ifndef XTL__VARIANT_STRUCT_READER_HPP__
#define XTL__VARIANT_STRUCT_READER_HPP__ 1

#include <string>

#include "VariantStruct.hpp"
#include "VariantPtr.hpp"

namespace XTL
{
	class VariantStructReader
	{
		public:

			explicit VariantStructReader(Variant::Struct & structRef)
				: structRef_(structRef)
			{
				;;
			}

			long long int GetLongLongInt(const char * key, long long int defaultValue)
			{
				VariantPtr * ptr = structRef_.Get(key);
				return ptr != 0 ? ptr->ToLongLongInt() : defaultValue;
			}

			const std::string GetString(const char * key, const std::string & defaultValue)
			{
				VariantPtr * ptr = structRef_.Get(key);
				return ptr != 0 ? ptr->ToString() : defaultValue;
			}

			VariantPtr * Get(const char * key)
			{
				return structRef_.Get(key);
			}

		private:

			Variant::Struct & structRef_;
	};
}

#endif

