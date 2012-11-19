#ifndef XTL_SQLITE__QUERY_RESULT_HPP__
#define XTL_SQLITE__QUERY_RESULT_HPP__ 1

namespace XTL
{
namespace SQLITE
{
	class Database;

	class QueryResult
	{
		private:

			struct Reference
			{
				Reference(Database * db, void * stmt)
					: db_(db), stmt_(stmt) { ;; }

				Database * db_;
				void     * stmt_;
			};

		public:

			QueryResult(QueryResult & other);

			QueryResult(Reference ref);

			~QueryResult() throw();

			QueryResult & operator= (QueryResult & other);

			QueryResult & operator= (Reference ref);

			operator Reference();

			int ColumnsCount() const;

			const char * ColumnName(unsigned int columnIndex) const;

			bool Fetch();

			const char * GetText(unsigned int columnIndex) const;

			void Finalize();

		private:

			friend class Database;

			QueryResult(Database * db, void * stmt);

			Database * db_;
			void     * stmt_;
	};
}
}

#endif
