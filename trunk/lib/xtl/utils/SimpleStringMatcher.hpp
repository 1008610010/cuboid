#ifndef XTL__SIMPLE_STRING_MATCHER_HPP__
#define XTL__SIMPLE_STRING_MATCHER_HPP__ 1

#include <memory>
#include <string>

#include "../StringUtils.hpp"
#include "AutoPtrVector.hpp"

namespace XTL
{
	class CharSource;

	/*
	 * TODO: add properties: minMatchedLength, maxMatchedLength;
	 */
	class SimpleStringMatcher
	{
		public:

			class ParseError
			{
				public:

					ParseError(const std::string & what)
						: what_(what)
					{
						;;
					}

					const char * What() const throw()
					{
						return what_.c_str();
					}

				private:

					const std::string what_;
			};

			class Submatch
			{
				public:

					Submatch();

					void SetBegin(int begin);

					void SetEnd(int end);

					bool Extract(const std::string & source, std::string & result) const;

				private:

					int begin_;
					int end_;
			};

			class Context;

			class Result
			{
				struct Reference
				{
					bool matched;
					const Context * context;

					Reference(bool matched_, const Context * context_)
						: matched(matched_), context(context_) { ;; }
				};

				public:

					Result();

					explicit Result(std::auto_ptr<const Context> context);

					Result(Reference reference);

					~Result() throw();

					operator Reference();

					Result & operator= (Reference reference);

					operator bool () const;

					bool GetSubmatch(const std::string & source, unsigned int index, std::string & result) const;

					const std::string GetSubmatch(const std::string & source, unsigned int index) const;

					template <typename T>
					bool GetSubmatch(const std::string & source, unsigned int index, T & value) const
					{
						std::string temp;
						if (GetSubmatch(source, index, temp))
						{
							value = XTL::StringToInteger<T>(temp);
							return true;
						}
						else
						{
							return false;
						}
					}

					template <typename T>
					const T GetSubmatchInteger(const std::string & source, unsigned int index) const
					{
						return XTL::StringToInteger<T>(GetSubmatch(source, index));
					}

					template <typename T1,
					          typename T2>
					void GetSubmatches(const std::string & source, T1 & t1, T2 & t2)
					{
						GetSubmatch(source, 0, t1);
						GetSubmatch(source, 1, t2);
					}

					template <typename T1,
					          typename T2,
					          typename T3>
					void GetSubmatches(const std::string & source, T1 & t1, T2 & t2, T3 & t3)
					{
						GetSubmatch(source, 0, t1);
						GetSubmatch(source, 1, t2);
						GetSubmatch(source, 2, t3);
					}

					template <typename T1,
					          typename T2,
					          typename T3,
					          typename T4>
					void GetSubmatches(const std::string & source, T1 & t1, T2 & t2, T3 & t3, T4 & t4)
					{
						GetSubmatch(source, 0, t1);
						GetSubmatch(source, 1, t2);
						GetSubmatch(source, 2, t3);
						GetSubmatch(source, 3, t4);
					}

					template <typename T1,
					          typename T2,
					          typename T3,
					          typename T4,
					          typename T5>
					void GetSubmatches(const std::string & source, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5)
					{
						GetSubmatch(source, 0, t1);
						GetSubmatch(source, 1, t2);
						GetSubmatch(source, 2, t3);
						GetSubmatch(source, 3, t4);
						GetSubmatch(source, 4, t5);
					}

				private:

					bool matched_;
					std::auto_ptr<const Context> context_;
			};

			class Node
			{
				public:

					virtual ~Node() throw() { ;; }

					virtual bool Process(CharSource & charSource, Context & context) const = 0;
			};

			explicit SimpleStringMatcher(const char * tmpl);

			Result Match(CharSource & charSource) const;

			Result Match(const std::string & source) const;

		private:

			SimpleStringMatcher(const SimpleStringMatcher &);
			SimpleStringMatcher & operator= (const SimpleStringMatcher &);

			void Parse();

			void AddStringNode(CharSource & charSource);

			void AddStringNode(const std::string & s);

			const std::string template_;
			AutoPtrVector<const Node> nodes_;
			unsigned int submatchesCount_;
	};
}

#endif
