#include <stdio.h>

#include <map>

#include <xtl/Types.hpp>
#include <xtl/mysql/Connection.hpp>
#include <xtl/mysql/Exception.hpp>
#include <xtl/mysql/QueryResult.hpp>
#include <xtl/mysql/QueryResultRow.hpp>

class RatingPropsPosts
{
	typedef std::map<std::pair<XTL::UINT_32, XTL::UINT_32>, XTL::UINT_32> PostFlags;

	public:

		RatingPropsPosts(XTL::MYSQL::Connection & dbc)
			: postFlags_()
		{
			XTL::MYSQL::QueryResult result = dbc.Select("SELECT journal_id, post_id, flags FROM rating_props_posts");
			if (result.IsNull())
			{
				fprintf(stderr, "%s\n", dbc.LastError());
				return;
			}

			std::vector<std::string> columns;
			result.GetColumnsNames(columns);
			for (std::vector<std::string>::const_iterator itr = columns.begin(); itr != columns.end(); ++itr)
			{
				printf("%s\n", (*itr).c_str());
			}

			XTL::MYSQL::QueryResultRow row;
			while (result.Fetch(row))
			{
				XTL::UINT_32 journalId = row.GetInteger(0);
				XTL::UINT_32 ditemId   = row.GetInteger(1);
				XTL::UINT_32 flags     = row.GetInteger(2);

				postFlags_[std::make_pair(journalId, ditemId)] = flags;
			}
		}

		XTL::UINT_32 GetFlags(XTL::UINT_32 journalId, XTL::UINT_32 ditemId) const
		{
			PostFlags::const_iterator itr = postFlags_.find(std::make_pair(journalId, ditemId));

			return itr == postFlags_.end() ? 0 : itr->second;
		}

	private:

		PostFlags postFlags_;
};

int main(int argc, const char * argv[])
{
	XTL::MYSQL::ConnectionConfig dbcConfig;
	dbcConfig.host = "172.16.10.101";
	dbcConfig.user = "root";
	dbcConfig.database = "voting";

	XTL::MYSQL::Connection dbc(dbcConfig);

	try
	{
		dbc.Open();

		RatingPropsPosts ratingPropsPosts(dbc);

		printf("%u\n", ratingPropsPosts.GetFlags(6566132, 115160));

		dbc.Close();
	}
	catch (const XTL::MYSQL::QueryError & e)
	{
		fprintf(stderr, "[%u] %s (%s)\n", e.Code(), e.What().c_str(), e.Query().c_str());
	}
	catch (const XTL::MYSQL::Exception & e)
	{
		fprintf(stderr, "[%u] %s\n", e.Code(), e.What().c_str());
	}

	return 0;
}

