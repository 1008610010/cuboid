#include "IniConfig.hpp"

namespace XTL
{
	/********** IniConfig::Section **********/

	IniConfig::Section::Section(const std::string & sectionName)
		: sectionName_(sectionName),
		  pairs_()
	{
		;;
	}

}
