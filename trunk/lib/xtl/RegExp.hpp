#ifndef XTL__REG_EXP_HPP__
#define XTL__REG_EXP_HPP__ 1

#include <string>

namespace XTL
{
	class RegExpSubmatch;

	class RegExp
	{
		public:

			explicit RegExp(const std::string & s);

			~RegExp() throw();

			template <typename T1>
			bool Search(const std::string & source, T1 & t1) const;

			template <typename T1, typename T2>
			bool Search(const std::string & source, T1 & t1, T2 & t2) const;

			template <typename T1, typename T2, typename T3>
			bool Search(const std::string & source, T1 & t1, T2 & t2, T3 & t3) const;

			template <typename T1, typename T2, typename T3, typename T4>
			bool Search(const std::string & source, T1 & t1, T2 & t2, T3 & t3, T4 & t4) const;

		private:

			RegExp(const RegExp &);
			RegExp & operator= (const RegExp &);

			bool MakeSearch(const std::string & source, unsigned int needSubmatch, RegExpSubmatch & submatch) const;

			void * re_;
	};

	class RegExpSubmatch
	{
		public:

			RegExpSubmatch();

			~RegExpSubmatch() throw();

			unsigned int Size() const;

			const std::string operator[] (unsigned int index) const;

		private:

			friend class RegExp;

			RegExpSubmatch(const RegExpSubmatch &);
			RegExpSubmatch & operator= (const RegExpSubmatch &);

			void * smatch_;
	};
}

namespace XTL
{
	void UnpackSubmatch(const std::string & s, int & result);

	void UnpackSubmatch(const std::string & s, unsigned int & result);

	void UnpackSubmatch(const std::string & s, char & result);

	void UnpackSubmatch(const std::string & s, std::string & result);

	template <typename T1>
	bool RegExp::Search(const std::string & source, T1 & t1) const
	{
		RegExpSubmatch submatch;
		if (MakeSearch(source, 1, submatch))
		{
			UnpackSubmatch(submatch[1], t1);
			return true;
		}
		else
		{
			return false;
		}
	}

	template <typename T1, typename T2>
	bool RegExp::Search(const std::string & source, T1 & t1, T2 & t2) const
	{
		RegExpSubmatch submatch;
		if (MakeSearch(source, 2, submatch))
		{
			UnpackSubmatch(submatch[1], t1);
			UnpackSubmatch(submatch[2], t2);
			return true;
		}
		else
		{
			return false;
		}
	}

	template <typename T1, typename T2, typename T3>
	bool RegExp::Search(const std::string & source, T1 & t1, T2 & t2, T3 & t3) const
	{
		RegExpSubmatch submatch;
		if (MakeSearch(source, 3, submatch))
		{
			UnpackSubmatch(submatch[1], t1);
			UnpackSubmatch(submatch[2], t2);
			UnpackSubmatch(submatch[3], t3);
			return true;
		}
		else
		{
			return false;
		}
	}

	template <typename T1, typename T2, typename T3, typename T4>
	bool RegExp::Search(const std::string & source, T1 & t1, T2 & t2, T3 & t3, T4 & t4) const
	{
		RegExpSubmatch submatch;
		if (MakeSearch(source, 4, submatch))
		{
			UnpackSubmatch(submatch[1], t1);
			UnpackSubmatch(submatch[2], t2);
			UnpackSubmatch(submatch[3], t3);
			UnpackSubmatch(submatch[4], t4);
			return true;
		}
		else
		{
			return false;
		}
	}
}

#endif
