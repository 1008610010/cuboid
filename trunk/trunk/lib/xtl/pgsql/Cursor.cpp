#include "Cursor.hpp"

#include "Connection.hpp"
#include "QueryResult.hpp"
#include "../Exception.hpp"
#include "../tp/SimpleTemplate.hpp"
#include "../tp/SimpleTemplateValueInteger.hpp"
#include "../tp/SimpleTemplateValueString.hpp"

namespace XTL
{
namespace PGSQL
{
	namespace
	{
		static const XTL::SimpleTemplate TEMPLATE_DECLARE_CURSOR ("DECLARE $_ CURSOR FOR $_;");
		static const XTL::SimpleTemplate TEMPLATE_CLOSE_CURSOR   ("CLOSE $_;");
		static const XTL::SimpleTemplate TEMPLATE_FETCH_CURSOR   ("FETCH $_ IN $_;");
	}

	Cursor::Cursor(Connection & dbc)
		: dbc_(dbc),
		  name_()
	{
		;;
	}

	void Cursor::Open(const std::string & name, const std::string & query)
	{
		Close();
		if (name.empty())
		{
			throw ILLEGAL_ARGUMENT_ERROR("Empty name of cursor");
		}
		// TODO: check, if name is empty()

		dbc_.Execute(XTL::TemplateToString(TEMPLATE_DECLARE_CURSOR, name, query));
		name_ = name;
	}

	void Cursor::Close()
	{
		if (name_.empty()) {
			return;
		}

		std::string cursorQuery = XTL::TemplateToString(TEMPLATE_CLOSE_CURSOR, name_);
		name_.clear();
		dbc_.Execute(cursorQuery);
	}

	QueryResult Cursor::Fetch(int count)
	{
		return dbc_.Execute(XTL::TemplateToString(TEMPLATE_FETCH_CURSOR, count, name_));
	}
}
}

