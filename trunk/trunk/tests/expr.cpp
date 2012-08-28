#include <stdio.h>

#include "Expression.hpp"
#include "ExpressionOperator.hpp"
#include "SyntaxAnalyzer.hpp"

#include <xtl/PrintStream.hpp>

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

class Addition : public BinaryOperator
{
	public:

		virtual ~Addition() throw() { ;; }

		virtual unsigned int Id() const { return 0; }

		virtual unsigned int InputPriority() const { return 1; }

		virtual unsigned int StackPriority() const { return 2; }

		virtual const std::string ToString() const { return "+"; }

	protected:

		virtual const char * ToConstCharPtr() const
		{
			return "+";
		}
};

class StackOperator;

class InputOperator
{
	class InputOperatorAction
	{
		
	};

	class InputOperatorActions
	{
		virtual const InputOperatorAction & OnEmptyStack() const = 0;

		virtual const InputOperatorAction & OnOperand() const = 0;

		virtual const InputOperatorAction & OnOperator() const = 0;
	};

	virtual StackOperator * Convert() = 0;
};

class StackOperator
{
	class StackOperatorAction
	{
	};

	class StackOperatorActions
	{
		virtual const StackOperatorAction & OnCompositeOperator() const = 0;
	};
};

class CompositeOperator : public XTL::Expression::Operator
{
	public:

//		virtual const XTL::SyntaxAnalyzer::OperatorActions & GetActions() const
};

class CompositeOperatorHead : public XTL::Expression::Operator
{
	public:

		
};

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



class CompositeOperatorTail : public XTL::Expression::Operator
{
	public:

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
					return PUSH;
				}

			private:

		};
		
};

class OpenBracket : public XTL::Expression::Operator
{
	public:

		virtual unsigned int NeedParamsCount() const
		{
			return 1;
		}

		virtual unsigned int OperatorId() const = 0;

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

class CloseBracket : public XTL::Expression::Operator
{
	public:

		virtual unsigned int NeedParamsCount() const
		{
			return 0;
		}

		virtual unsigned int OperatorId() const = 0;

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
					// return stackOperator.GetActions().CompositeOperatorAction();
					return REDUCE;
					/*
					if (inputOperator.OperatorId() == stackOperator.OperatorId())
					{
						return REDUCE;
					}
					else
					{
						return POP;
					}
					*/
				}
		};
};

class Multiplication : public BinaryOperator
{
	public:

		virtual ~Multiplication() throw() { ;; }

		virtual unsigned int Id() const { return 0; }

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

		class OperatorActions : public OperatorActionsSingleton<OperatorActions>
		{
			public:

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
		};

		virtual const std::string ToString() const { return "#"; }

	protected:

		virtual const char * ToConstCharPtr() const { return "#"; }

	private:
};

struct A
{
};

struct B : public A
{
};

void F(const A & a)
{
	printf("F(A)\n");
}

void F(const B & a)
{
	printf("F(B)\n");
}

struct C
{
	
};



int main(int argc, const char * argv[])
{
	std::auto_ptr<XTL::Expression::Node> node;

	const std::string s = "x+y*z*z";

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

		// TODO: move this shit to the Terminator::EmptyStackAction() method
		std::auto_ptr<XTL::Expression::Node> expressionRoot = sa.Release();

		expressionRoot->DebugPrint(XTL::StandardPrintStream::Out());
		fprintf(stdout, "\n");
	}
	catch (const std::runtime_error & e)
	{
		fprintf(stderr, "ERROR: %s\n", e.what());
	}
}

