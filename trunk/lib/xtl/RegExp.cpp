#include "RegExp.hpp"

#include <stdexcept>
#include <boost/regex.hpp>

#include "FormatString.hpp"
#include "StringUtils.hpp"

namespace XTL
{
	RegExpSubmatch::RegExpSubmatch()
		: smatch_(new boost::smatch())
	{
		;;
	}

	RegExpSubmatch::~RegExpSubmatch() throw()
	{
		delete static_cast<boost::smatch *>(smatch_);
	}

	unsigned int RegExpSubmatch::Size() const
	{
		return static_cast<boost::smatch *>(smatch_)->size() - 1;
	}

	const std::string RegExpSubmatch::operator[] (unsigned int index) const
	{
		return static_cast<boost::smatch *>(smatch_)->str(index);
	}


	RegExp::RegExp(const std::string & s)
		: re_(new boost::regex(s))
	{
		;;
	}

	RegExp::~RegExp() throw()
	{
		delete static_cast<boost::regex *>(re_);
	}

	bool RegExp::MakeSearch(const std::string & source, unsigned int needSubmatch, RegExpSubmatch & submatch) const
	{
		bool found = boost::regex_search(
			source,
			*static_cast<boost::smatch *>(submatch.smatch_),
			*static_cast<const boost::regex *>(re_)
		);

		if (found)
		{
			if (needSubmatch != submatch.Size())
			{
				throw std::runtime_error(XTL::FormatString("Invalid submatch count: have %i, requested: %u", submatch.Size(), needSubmatch));
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	void UnpackSubmatch(const std::string & s, int & result)
	{
		result = XTL::StringToInteger<int>(s);
	}

	void UnpackSubmatch(const std::string & s, unsigned int & result)
	{
		result = XTL::StringToInteger<unsigned int>(s);
	}

	void UnpackSubmatch(const std::string & s, char & result)
	{
		if (s.empty() > 0)
		{
			result = '\0';
		}
		else
		{
			result = s[0];
		}
	}

	void UnpackSubmatch(const std::string & s, std::string & result)
	{
		result = s;
	}
}
