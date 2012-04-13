#ifndef _XTL__MYSQL_LIBRARY_HPP__
#define _XTL__MYSQL_LIBRARY_HPP__ 1

#include <list>
#include <string>

namespace XTL
{
	class CStringArray
	{
		public:

			CStringArray(char ** values);

			~CStringArray() throw();

			operator char ** ()  { return values_; }

		protected:

			CStringArray(const CStringArray &);
			CStringArray & operator= (const CStringArray &);

			char ** values_;
	};

	class MySqlOptionList
	{
		public:

			MySqlOptionList() { ;; }

			~MySqlOptionList() throw() { ;; }

			unsigned int Size() const;

			MySqlOptionList & operator<< (const char * v);

			MySqlOptionList & operator<< (const std::string & v);

			char ** ToArray() const;

		private:

			std::list<std::string> values_;
	};

	class MySqlLibrary
	{
		public:

			MySqlLibrary(const std::string & appName, const std::string & configFile);

			~MySqlLibrary() throw();
	};
}

#endif
