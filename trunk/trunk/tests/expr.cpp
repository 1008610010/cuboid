#include <stdio.h>

#include "Expression.hpp"
#include "ExpressionOperator.hpp"
#include "SyntaxAnalyzer.hpp"

#include <xtl/PrintStream.hpp>

template <typename T>
class OperatorActionsSingleton : public XTL::SyntaxAnalyzer::OperatorActions
{
	public:

		static XTL::SyntaxAnalyzer::OperatorActions & Instance()
		{
			static T instance;
			return instance;
		}

		virtual ~OperatorActionsSingleton() throw()
		{
			;;
		}

	protected:

		OperatorActionsSingleton() { ;; }
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

		virtual const std::string ToString() const
		{
			return name_;
		}

		virtual void DebugPrint(XTL::PrintStream & stream)
		{
			stream.Print(name_);
		}

	private:

		const std::string name_;
};

class BinaryOperator : public XTL::Expression::Operator
{
	public:

		virtual unsigned int NeedParamsCount() const
		{
			return 2;
		}

		virtual bool IsComposite() const
		{
			return false;
		}

		virtual const XTL::SyntaxAnalyzer::OperatorActions & GetActions() const
		{
			return OperatorActions::Instance();
		}

		class OperatorActions : public OperatorActionsSingleton<OperatorActions>
		{
			public:

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
					if (stackOperator.IsComposite() || stackOperator.StackPriority() <= inputOperator.InputPriority())
					{
						return PUSH;
					}
					else
					{
						return POP;
					}
				}
		};
};

class InputOperator : public XTL::Expression::Operator
{
	public:

		virtual ~InputOperator() throw()
		{
			;;
		}

		virtual unsigned int NeedParamsCount() const
		{
			throw std::runtime_error("InputOperator has not NeedParamsCount");
		}

		virtual unsigned int StackPriority() const
		{
			throw std::runtime_error("InputOperator has not StackPriority");
		}
};

enum OPERATORS
{
	TERMINATOR,
	ADDITION,
	MULTIPLICATION,
	ROUND_BRACKETS
};

static const unsigned int MIN_PRIORITY = 0;
static const unsigned int MAX_PRIORITY = 65536;


class Addition : public BinaryOperator
{
	public:

		virtual ~Addition() throw() { ;; }

		virtual unsigned int Id() const { return ADDITION; }

		virtual unsigned int InputPriority() const { return 1; }

		virtual unsigned int StackPriority() const { return 2; }

		virtual const std::string ToString() const { return "+"; }

	protected:

		virtual const char * ToConstCharPtr() const
		{
			return "+";
		}
};

class OpenBracket : public XTL::Expression::Operator
{
	public:

		virtual unsigned int NeedParamsCount() const
		{
			return 1;
		}

		virtual bool IsComposite() const
		{
			return true;
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
					return PUSH;
				}

			private:

				OperatorActions() { ;; }
		};
};

class CloseBracket : public InputOperator
{
	public:

		virtual const XTL::SyntaxAnalyzer::OperatorActions & GetActions() const
		{
			return OperatorActions::Instance();
		}

		class OperatorActions : public OperatorActionsSingleton<OperatorActions>
		{
			public:

				virtual ~OperatorActions() throw() { ;; }

				virtual const XTL::SyntaxAnalyzer::Action & EmptyStackAction(const XTL::Expression::Operator & inputOperator) const
				{
					throw std::runtime_error("FUCK!");
				}

				virtual const XTL::SyntaxAnalyzer::Action & OperandAction(const XTL::Expression::Operator & inputOperator) const
				{
					return POP;
				}

				virtual const XTL::SyntaxAnalyzer::Action & OperatorAction(const XTL::Expression::Operator & inputOperator, const XTL::Expression::Operator & stackOperator) const
				{
					if (stackOperator.IsComposite())
					{
						if (stackOperator.Id() == inputOperator.Id())
						{
							return REDUCE;
						}
						else
						{
							throw std::runtime_error("Syntax Error");
						}
					}
					else
					{
						return POP;
					}
				}
		};
};

class Multiplication : public BinaryOperator
{
	public:

		virtual ~Multiplication() throw() { ;; }

		virtual unsigned int Id() const { return MULTIPLICATION; }

		virtual unsigned int InputPriority() const { return 3; }

		virtual unsigned int StackPriority() const { return 4; }

		virtual const std::string ToString() const
		{
			return "*";
		}

	protected:

		virtual const char * ToConstCharPtr() const
		{
			return "*";
		}
};

class Terminator : public InputOperator
{
	public:

		virtual ~Terminator() throw() { ;; }

		virtual unsigned int Id() const { return TERMINATOR; }

		virtual unsigned int InputPriority() const { return 0; }

		virtual bool IsComposite() const
		{
			return false;
		}

		virtual const XTL::SyntaxAnalyzer::OperatorActions & GetActions() const
		{
			return OperatorActions::Instance();
		}

		class OperatorActions : public OperatorActionsSingleton<OperatorActions>
		{
			public:

				virtual ~OperatorActions() throw() { ;; }

				virtual const XTL::SyntaxAnalyzer::Action & EmptyStackAction(const XTL::Expression::Operator & inputOperator) const
				{
					// TODO: Pop result expression tree
					return NOTHING;
				}

				virtual const XTL::SyntaxAnalyzer::Action & OperandAction(const XTL::Expression::Operator & inputOperator) const
				{
					return POP;
				}

				virtual const XTL::SyntaxAnalyzer::Action & OperatorAction(const XTL::Expression::Operator & inputOperator, const XTL::Expression::Operator & stackOperator) const
				{
					if (stackOperator.IsComposite())
					{
						throw std::runtime_error("Syntax Error");
					}
					else
					{
						return POP;
					}
				}
		};

		virtual const std::string ToString() const { return "#"; }

	protected:

		virtual const char * ToConstCharPtr() const { return "#"; }

	private:
};

class RoundBrackets : public OpenBracket
{
	public:

		virtual unsigned int Id() const
		{
			return ROUND_BRACKETS;
		}

		virtual unsigned int InputPriority() const
		{
			return MIN_PRIORITY;
		}

		virtual unsigned int StackPriority() const
		{
			return MAX_PRIORITY;
		}

		virtual const char * ToConstCharPtr() const
		{
			return "R";
		}
};

class RoundBracketClose : public CloseBracket
{
	public:

		virtual unsigned int Id() const
		{
			return ROUND_BRACKETS;
		}

		virtual unsigned int InputPriority() const
		{
			return MAX_PRIORITY;
		}

		virtual unsigned int StackPriority() const
		{
			throw std::runtime_error("InputOperator have not StackPriority");
		}

		virtual const char * ToConstCharPtr() const
		{
			return "R";
		}

		virtual bool IsComposite() const
		{
			return false;
		}
};

/*
struct A {};

struct B : public A {};

struct C : public A {};

void F(const A & a)
{
	printf("F(A)\n");
}

void F(const B & b)
{
	printf("F(B)\n");
}

void F(const C & c)
{
	printf("F(C)\n");
}

struct Test : public B {};


	Test t;
	F(t);
	return 0;
*/

int main(int argc, const char * argv[])
{
	std::auto_ptr<XTL::Expression::Node> node;

	const std::string s = "(x+y)*(a+b)";

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
			else if (s[i] == '(')
			{
				node.reset(new RoundBrackets());
			}
			else if (s[i] == ')')
			{
				node.reset(new RoundBracketClose());
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

		// TODO: move this shit to the Terminator::EmptyStackAction() method
		std::auto_ptr<XTL::Expression::Node> expressionRoot = sa.Release();

		expressionRoot->DebugPrint(XTL::StdOut());
		fprintf(stdout, "\n");
	}
	catch (const std::runtime_error & e)
	{
		fprintf(stderr, "ERROR: %s\n", e.what());
	}
}

