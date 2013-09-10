#ifndef XTL__SQLITE__STATEMENT_HPP__
#define XTL__SQLITE__STATEMENT_HPP__ 1

#include <string>

#include <xtl/Types.hpp>

namespace XTL
{
namespace SQLITE
{
	class Database;

	class Statement
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

			Statement(Statement & other);

			Statement(Reference ref);

			~Statement() throw();

			Statement & operator= (Statement & other);

			Statement & operator= (Reference ref);

			operator Reference();

			int ColumnsCount() const;

			const char * ColumnName(unsigned int columnIndex) const;

			/**
			 * Нумерация параметров начинается с 1.
			 */
			void BindNull(int paramIndex);

			void Bind(int paramIndex, int value);

			/**
			 * Ugly Hack: вызов метода Bind(int, int);
			 */
			void Bind(int paramIndex, unsigned int value);

			void Bind(int paramIndex, XTL::INT_64 value);

			void Bind(int paramIndex, double value);

			void Bind(int paramIndex, const std::string & value);

			void Reset();

			int Execute();

			template <typename T1, typename T2, typename T3, typename T4, typename T5>
			int Execute(const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5)
			{
				Bind(1, t1);
				Bind(2, t2);
				Bind(3, t3);
				Bind(4, t4);
				Bind(5, t5);
				return Execute();
			}

			bool Fetch();

			const char * GetText(unsigned int columnIndex) const;

			int GetInteger(unsigned int columnIndex) const;

			const std::string FetchString(const char * defaultValue = "");

		private:

			friend class Database;

			Statement(Database * db, void * stmt);

			void Finalize();

			Database * db_;
			void     * stmt_;
	};
}
}

#endif
