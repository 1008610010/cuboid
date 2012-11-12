#include "IniConfig.hpp"

#include "lib/xtl/FormatString.hpp"

namespace XTL
{
	/********** IniConfig::Error::SectionDoesNotExist **********/

	IniConfig::Error::SectionDoesNotExist::SectionDoesNotExist(const std::string & sectionName)
		: sectionName_(sectionName)
	{
		;;
	}

	virtual IniConfig::Error::SectionDoesNotExist::~SectionDoesNotExist() throw()
	{
		;;
	}

	virtual const std::string IniConfig::Error::SectionDoesNotExist::What() const
	{
		return FormatString("Section '%s' does not exist", sectionName_);
	}

	const std::string & IniConfig::Error::SectionDoesNotExist::SectionName() const
	{
		return sectionName_;
	}

	/********** IniConfig::Error::KeyWasNotFound **********/

	IniConfig::Error::KeyWasNotFound::KeyWasNotFound(const std::string & sectionName, const std::string & key)
		: sectionName_(sectionName),
		  key_(key)
	{
		;;
	}

	IniConfig::Error::KeyWasNotFound::~KeyWasNotFound() throw()
	{
		;;
	}

	const std::string IniConfig::Error::KeyWasNotFound::What() const
	{
		return FormatString("Section '%s' does not contain key '%s'", sectionName_, key_);
	}

	const std::string & IniConfig::Error::KeyWasNotFound::SectionName() const
	{
		return sectionName_;
	}

	const std::string & IniConfig::Error::KeyWasNotFound::Key() const
	{
		return key_;
	}

	/********** IniConfig::Section **********/

	IniConfig::Section::Section(const std::string & sectionName)
		: sectionName_(sectionName),
		  pairs_()
	{
		;;
	}

	bool IniConfig::Section::Contains(const std::string & key) const
	{
		return pairs_.count(key) > 0;
	}

	void IniConfig::Section::Set(const std::string & key, const VariantPtr & value)
	{
		pairs_[key] = value;
	}

	VariantPtr IniConfig::Section::Get(const std::string & key) const
	{
		std::map<std::string, VariantPtr>::const_iterator itr = pairs_.find(key);

		if (itr == pairs_.end())
		{
			throw Error::KeyWasNotFound(sectionName_, key);
		}

		return itr->second;
	}

	const long long int IniConfig::Section::GetLongLongInt(const std::string & key, const long long int & defaultValue) const
	{
		std::map<std::string, VariantPtr>::iterator itr = pairs_.find(key);

		if (itr == pairs_.end())
		{
			return defaultValue;
		}

		return itr->second.ToLongLongInt();
	}

	const double IniConfig::Section::GetDouble(const std::string & key, const double & defaultValue) const
	{
		std::map<std::string, VariantPtr>::iterator itr = pairs_.find(key);

		if (itr == pairs_.end())
		{
			return defaultValue;
		}

		return itr->second.ToDouble();
	}

	const std::string IniConfig::Section::GetString(const std::string & key, const std::string & defaultValue) const
	{
		std::map<std::string, VariantPtr>::iterator itr = pairs_.find(key);

		if (itr == pairs_.end())
		{
			return defaultValue;
		}

		return itr->second.ToString();
	}

	/********** IniConfig **********/

	Section & IniConfig::CreateSection(const std::string & sectionName)
	{
		Section * section = sections_[sectionName];
		if (section == 0)
		{
			std::auto_ptr<Section> newSection(new Section(sectionName));
			section = newSection.get();
			sections_.Set(sectionName, newSection);
		}

		return *section;
	}

	void IniConfig::Set(const std::string & sectionName, const std::string & key, VariantPtr value)
	{
		CreateSection(sectionName).Set(key, value);
	}

	const Section & IniConfig::GetSectionRequired(const std::string & sectionName) const
	{
		Section * section = sections_[sectionName];

		if (section == 0)
		{
			throw Error::SectionDoesNotExist(sectionName);
		}

		return *section;
	}

	const Section & IniConfig::GetSectionOptional(const std::string & sectionName) const
	{
		static const Section EMPTY_SECTION("");

		Section * section = sections_[sectionName];

		return section != 0 ? section : EMPTY_SECTION;
	}


}
