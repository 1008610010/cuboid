#include "IniConfig.hpp"

#include <vector>

#include "IniConfigParser.hpp"
#include "../FormatString.hpp"
#include "../Types.hpp"
#include "../VariantScalar.hpp"
#include "../linux/fs/File.hpp"

namespace XTL
{
	/********** IniConfig::Error:: **********/

	IniConfig::Error::Error(const std::string & what)
		: what_(what)
	{
		;;
	}

	IniConfig::Error::~Error() throw()
	{
		;;
	}

	const std::string IniConfig::Error::What() const
	{
		return what_;
	}

	/********** IniConfig::Error::SectionDoesNotExist **********/

	IniConfig::Error::SectionDoesNotExist::SectionDoesNotExist(const std::string & sectionName)
		: Error(FormatString("Section '%s' does not exist", sectionName)),
		  sectionName_(sectionName)
	{
		;;
	}

	IniConfig::Error::SectionDoesNotExist::~SectionDoesNotExist() throw()
	{
		;;
	}

	const std::string & IniConfig::Error::SectionDoesNotExist::SectionName() const
	{
		return sectionName_;
	}

	/********** IniConfig::Error::KeyDoesNotExist **********/

	IniConfig::Error::KeyDoesNotExist::KeyDoesNotExist(const std::string & sectionName, const std::string & key)
		: Error(FormatString("Section '%s' does not contain key '%s'", sectionName_, key_)),
		  sectionName_(sectionName),
		  key_(key)
	{
		;;
	}

	IniConfig::Error::KeyDoesNotExist::~KeyDoesNotExist() throw()
	{
		;;
	}

	const std::string & IniConfig::Error::KeyDoesNotExist::SectionName() const
	{
		return sectionName_;
	}

	const std::string & IniConfig::Error::KeyDoesNotExist::Key() const
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

	namespace
	{
		class AppendTo
		{
			public:

				explicit AppendTo(IniConfig::Section & section)
					: section_(section)
				{
					;;
				}

				void operator() (const std::pair<std::string, VariantPtr> & pair)
				{
					section_.Set(pair.first, pair.second);
				}

			private:

				IniConfig::Section & section_;
		};
	}

	void IniConfig::Section::Append(const Section & section)
	{
		std::for_each(section.pairs_.begin(), section.pairs_.end(), AppendTo(*this));
	}

	VariantPtr IniConfig::Section::Get(const std::string & key) const
	{
		std::map<std::string, VariantPtr>::const_iterator itr = pairs_.find(key);

		if (itr == pairs_.end())
		{
			throw Error::KeyDoesNotExist(sectionName_, key);
		}

		return itr->second;
	}

	const long long int IniConfig::Section::GetLongLongInt(const std::string & key) const
	{
		{
			return Get(key).ToLongLongInt();
		}
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

	IniConfig::Section & IniConfig::CreateSection(const std::string & sectionName)
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

	void IniConfig::Append(const IniConfig & config)
	{
		for (AutoPtrMap<std::string, Section>::const_iterator itr = config.sections_.begin(); itr != config.sections_.end(); ++itr)
		{
			CreateSection((*itr).first).Append(*((*itr).second));
		}
	}

	const IniConfig::Section & IniConfig::GetSectionRequired(const std::string & sectionName) const
	{
		Section * section = sections_[sectionName];
		if (section == 0)
		{
			throw Error::SectionDoesNotExist(sectionName);
		}

		return *section;
	}

	const IniConfig::Section & IniConfig::GetSectionOptional(const std::string & sectionName) const
	{
		static const Section EMPTY_SECTION("");

		Section * section = sections_[sectionName];

		return section != 0 ? *section : EMPTY_SECTION;
	}

	void IniConfig::LoadFromFile(const std::string & filePath)
	{
		sections_.Clear();

		File file(filePath);
		file.Open(File::OPEN_READ_ONLY);
		FileCloseSentinel fileCloser(file);

		const FileSize fileSize = file.Size();
		if (fileSize > MAX_INI_CONFIG_SIZE)
		{
			throw Error(XTL::FormatString("File '%s' is too big", filePath));
		}

		std::vector<char> fileContent(fileSize, '\0');
		if (file.Read(&fileContent[0], fileSize) != fileSize)
		{
			throw Error(XTL::FormatString("Read error from file '%s'", filePath));
		}

		CharSource::ConstCharPtr charSource(&fileContent[0], fileContent.size());
		IniConfigParser(charSource, *this);
	}
}
