#include "SimpleStringMatcher.hpp"

#include "../tp/CharClass.hpp"
#include "../tp/CharSource.hpp"

namespace XTL
{
	SimpleStringMatcher::Submatch::Submatch()
		: begin_(-1),
		  end_(-1)
	{
		;;
	}

	void SimpleStringMatcher::Submatch::SetBegin(int begin)
	{
		begin_ = begin;
	}

	void SimpleStringMatcher::Submatch::SetEnd(int end)
	{
		end_ = end;
	}

	bool SimpleStringMatcher::Submatch::Extract(const std::string & source, std::string & result) const
	{
		if (begin_ < 0 || end_ < 0)
		{
			return false;
		}

		result.assign(source.data() + begin_, source.data() + end_);
		return true;
	}


	/********** SimpleStringMatcher::Context **********/

	class SimpleStringMatcher::Context
	{
		public:

			explicit Context(unsigned int submatchesCount)
				: submatches_(submatchesCount)
			{
				;;
			}

			void OpenSubmatch(unsigned int index, int position)
			{
				submatches_[index].SetBegin(position);
			}

			void CloseSubmatch(unsigned int index, int position)
			{
				submatches_[index].SetEnd(position);
			}

			bool GetSubmatch(const std::string & source, unsigned int index, std::string & result) const
			{
				if (index >= submatches_.size())
				{
					return false;
				}

				return submatches_[index].Extract(source, result);
			}

		private:

			std::vector<Submatch> submatches_;
	};

	namespace
	{
		class NodeString : public SimpleStringMatcher::Node
		{
			public:

				explicit NodeString(const std::string & s)
					: Node(),
					  s_(s)
				{
					;;
				}

				virtual ~NodeString() throw()
				{
					;;
				}

				virtual bool Process(CharSource & charSource, SimpleStringMatcher::Context & context) const
				{
					for (unsigned int i = 0; i < s_.size(); ++i)
					{
						if (charSource.AtEnd() || charSource.GetChar() != s_[i])
						{
							return false;
						}
						charSource.Advance();
					}

					return true;
				}

			private:

				const std::string s_;
		};

		class NodeAnyChar : public SimpleStringMatcher::Node
		{
			public:

				virtual ~NodeAnyChar() throw() { ;; }

				virtual bool Process(CharSource & charSource, SimpleStringMatcher::Context & context) const
				{
					if (charSource.AtEnd())
					{
						return false;
					}

					charSource.Advance();
					return true;
				}
		};

		class NodeDigit : public SimpleStringMatcher::Node
		{
			public:

				virtual ~NodeDigit() throw() { ;; }

				virtual bool Process(CharSource & charSource, SimpleStringMatcher::Context & context) const
				{
					if (charSource.AtEnd() || !CharClass::DECIMAL.Contains(charSource.GetChar()))
					{
						return false;
					}

					charSource.Advance();
					return true;
				}
		};

		class NodeSubmatchOpen : public SimpleStringMatcher::Node
		{
			public:

				explicit NodeSubmatchOpen(unsigned int submatchIndex)
					: submatchIndex_(submatchIndex)
				{
					;;
				}

				virtual ~NodeSubmatchOpen() throw() { ;; }

				virtual bool Process(CharSource & charSource, SimpleStringMatcher::Context & context) const
				{
					context.OpenSubmatch(submatchIndex_, charSource.GetCharIndex());
					return true;
				}

			private:

				const unsigned int submatchIndex_;
		};

		class NodeSubmatchClose : public SimpleStringMatcher::Node
		{
			public:

				explicit NodeSubmatchClose(unsigned int submatchIndex)
					: submatchIndex_(submatchIndex)
				{
					;;
				}

				virtual ~NodeSubmatchClose() throw() { ;; }

				virtual bool Process(CharSource & charSource, SimpleStringMatcher::Context & context) const
				{
					context.CloseSubmatch(submatchIndex_, charSource.GetCharIndex());
					return true;
				}

			private:

				const unsigned int submatchIndex_;
		};

		class NodeEnd : public SimpleStringMatcher::Node
		{
			public:

				virtual ~NodeEnd() throw() { ;; }

				virtual bool Process(CharSource & charSource, SimpleStringMatcher::Context & context) const
				{
					return charSource.AtEnd();
				}
		};
	}

	/********** SimpleStringMatcher::Result **********/

	SimpleStringMatcher::Result::Result()
		: matched_(false),
		  context_()
	{
		;;
	}

	SimpleStringMatcher::Result::Result(std::auto_ptr<const Context> context)
		: matched_(true),
		  context_(context.release())
	{
		;;
	}

	SimpleStringMatcher::Result::Result(Reference reference)
		: matched_(reference.matched),
		  context_(reference.context)
	{
		;;
	}

	SimpleStringMatcher::Result::~Result() throw()
	{
		;;
	}

	SimpleStringMatcher::Result::operator Reference()
	{
		return Reference(matched_, context_.release());
	}

	SimpleStringMatcher::Result & SimpleStringMatcher::Result::operator= (Reference reference)
	{
		matched_ = reference.matched;
		context_.reset(reference.context);
		return *this;
	}

	SimpleStringMatcher::Result::operator bool () const
	{
		return matched_;
	}

	bool SimpleStringMatcher::Result::GetSubmatch(const std::string & source, unsigned int index, std::string & result) const
	{
		return context_->GetSubmatch(source, index, result);
	}

	const std::string SimpleStringMatcher::Result::GetSubmatch(const std::string & source, unsigned int index) const
	{
		std::string temp;
		if (GetSubmatch(source, index, temp))
		{
			return temp;
		}
		else
		{
			return std::string("");
		}
	}


	/********** SimpleStringMatcher **********/

	SimpleStringMatcher::SimpleStringMatcher(const char * tmpl)
		: template_(tmpl),
		  nodes_(),
		  submatchesCount_(0)
	{
		Parse();
	}

	void SimpleStringMatcher::Parse()
	{
		nodes_.Clear();
		submatchesCount_ = 0;

		CharSource::ConstCharPtr charSource(template_.data(), template_.size());
		std::stack<unsigned int> submatches;

		charSource.Mark();
		while (charSource.NotAtEnd())
		{
			switch (charSource.GetChar())
			{
				case '(':
					AddStringNode(charSource);
					nodes_.PushBack(std::auto_ptr<const Node>(new NodeSubmatchOpen(submatchesCount_)));
					submatches.push(submatchesCount_);
					++submatchesCount_;
					charSource.Advance();
					charSource.Mark();
					break;

				case ')':
					AddStringNode(charSource);
					if (submatches.empty())
					{
						throw ParseError("Invalid submatches balance");
					}
					nodes_.PushBack(std::auto_ptr<const Node>(new NodeSubmatchClose(submatches.top())));
					submatches.pop();
					charSource.Advance();
					charSource.Mark();
					break;

				case '?':
					AddStringNode(charSource);
					nodes_.PushBack(std::auto_ptr<const Node>(new NodeAnyChar()));
					charSource.Advance();
					charSource.Mark();
					break;

				case '$':
					AddStringNode(charSource);
					nodes_.PushBack(std::auto_ptr<const Node>(new NodeEnd()));
					charSource.Advance();
					charSource.Mark();
					break;

				case '%':
					AddStringNode(charSource);
					charSource.Advance();
					if (charSource.AtEnd())
					{
						throw ParseError("Invalid escape sequence");
					}
					switch (charSource.GetChar())
					{
						case '%': AddStringNode("%"); break;
						case '$': AddStringNode("$"); break;
						case '(': AddStringNode("("); break;
						case ')': AddStringNode(")"); break;
						case '?': AddStringNode("?"); break;
						case 'd': nodes_.PushBack(std::auto_ptr<const Node>(new NodeDigit())); break;
						default:
							throw std::runtime_error("Invalid escape sequence");
					}
					charSource.Advance();
					charSource.Mark();
					break;

				default:
					charSource.Advance();
			}
		}

		AddStringNode(charSource);
	}

	void SimpleStringMatcher::AddStringNode(CharSource & charSource)
	{
		AddStringNode(charSource.ReleaseString());
	}

	void SimpleStringMatcher::AddStringNode(const std::string & s)
	{
		if (!s.empty())
		{
			nodes_.PushBack(std::auto_ptr<const Node>(new NodeString(s)));
		}
	}

	SimpleStringMatcher::Result SimpleStringMatcher::Match(CharSource & charSource) const
	{
		std::auto_ptr<Context> context(new Context(submatchesCount_));

		for (unsigned int i = 0; i < nodes_.Size(); ++i)
		{
			if (!nodes_[i]->Process(charSource, *context))
			{
				return Result();
			}
		}

		return Result(std::auto_ptr<const Context>(context.release()));
	}

	SimpleStringMatcher::Result SimpleStringMatcher::Match(const std::string & source) const
	{
		CharSource::ConstCharPtr charSource(source.data(), source.size());
		return Match(charSource);
	}

}
