#include <stdio.h>

#include "Expression.hpp"
#include "ExpressionOperator.hpp"
#include "SyntaxAnalyzer.hpp"

class BinaryOperator : public XTL::Expression::Operator
{
	public:

		virtual unsigned int NeedParamsCount() const
		{
			return 2;
		}

		virtual const XTL::SyntaxAnalyzer::OperatorActions & GetActions() const
		{
			return OperatorActions::Instance();
		}

		class OperatorActions : public XTL::SyntaxAnalyzer::OperatorActions
		{
			public:

				static XTL::SyntaxAnalyzer::OperatorActions & Instance()
				{
					static OperatorActions instance;
					return instance;
				}

				virtual ~OperatorActions() throw() { ;; }

				virtual const XTL::SyntaxAnalyzer::Action & EmptyStackAction(const XTL::Expression::Operator & inputOperator) const
				{
					return PUSH;
				}

				virtual const XTL::SyntaxAnalyzer::Action & OperandAction(const XTL::Expression::Operator & inputOperator) const
				{
					return POP;
				}

				virtual const XTL::SyntaxAnalyzer::Action & OperatorAction(const XTL::Expression::Operator & inputOperator, const XTL::Expression::Operator & stackOperator) const
				{
					if (inputOperator.InputPriority() < stackOperator.StackPriority())
					{
						return POP;
					}
					else
					{
						return PUSH;
					}
				}

			private:

				OperatorActions() { ;; }
		};

	private:
};

class Identifier : public XTL::Expression::Operand
{
	public:

		explicit Identifier(const std::string & name)
			: name_(name)
		{
			;;
		}

		virtual ~Identifier() throw() { ;; }

	private:

		const std::string name_;
};

class Addition : public BinaryOperator
{
	public:

		virtual ~Addition() throw() { ;; }

		virtual unsigned int Id() const { return 0; }

		virtual unsigned int InputPriority() const { return 1; }

		virtual unsigned int StackPriority() const { return 2; }
};

class Multiplication : public BinaryOperator
{
	public:

		virtual ~Multiplication() throw() { ;; }

		virtual unsigned int Id() const { return 0; }

		virtual unsigned int InputPriority() const { return 3; }

		virtual unsigned int StackPriority() const { return 4; }
};

class Terminator : public XTL::Expression::Operator
{
	public:

		virtual unsigned int NeedParamsCount() const
		{
			return 0;
		}

		virtual unsigned int Id() const { return 0; }

		virtual unsigned int InputPriority() const { return 0; }

		virtual unsigned int StackPriority() const { return 0; }

		virtual const XTL::SyntaxAnalyzer::OperatorActions & GetActions() const
		{
			return OperatorActions::Instance();
		}

		class OperatorActions : public XTL::SyntaxAnalyzer::OperatorActions
		{
			public:

				static XTL::SyntaxAnalyzer::OperatorActions & Instance()
				{
					static OperatorActions instance;
					return instance;
				}

				virtual ~OperatorActions() throw() { ;; }

				virtual const XTL::SyntaxAnalyzer::Action & EmptyStackAction(const XTL::Expression::Operator & inputOperator) const
				{
					return NOTHING;
				}

				virtual const XTL::SyntaxAnalyzer::Action & OperandAction(const XTL::Expression::Operator & inputOperator) const
				{
					return POP;
				}

				virtual const XTL::SyntaxAnalyzer::Action & OperatorAction(const XTL::Expression::Operator & inputOperator, const XTL::Expression::Operator & stackOperator) const
				{
					return POP;
				}

			private:

				OperatorActions() { ;; }
		};

	private:
};


int main(int argc, const char * argv[])
{
	std::auto_ptr<XTL::Expression::Node> node;

	const std::string s = "x+y+z+z+";

	XTL::SyntaxAnalyzer sa;

	try
	{
		for (unsigned int i = 0; i < s.size(); ++i)
		{
			if (s[i] == '+')
			{
				node.reset(new Addition());
			}
			else if (s[i] == '*')
			{
				node.reset(new Multiplication());
			}
			else if (s[i] >= 'a' && s[i] <= 'z')
			{
				node.reset(new Identifier(std::string(1, s[i])));
			}
			else
			{
				fprintf(stderr, "Invalid char: '%c'\n", s[i]);
			}

			sa.Process(node);
		}

		node.reset(new Terminator());
		sa.Process(node);
	}
	catch (const std::runtime_error & e)
	{
		fprintf(stderr, "ERROR: %s\n", e.what());
	}
}

