#include "SyntaxAnalyzer.hpp"

#include <stdio.h>

#include "ExpressionOperator.hpp"

namespace XTL
{
	void SyntaxAnalyzer::Process(std::auto_ptr<Expression::Node> node)
	{
		fprintf(stdout, "<<< ");
		XTL::StandardPrintStream::Out().Print(node->ToString());
		fprintf(stdout, "\n");

		if (node->IsOperand())
		{
			operators_.Push(node);
		}
		else
		{
			std::auto_ptr<Expression::Operator> inputOperator(static_cast<Expression::Operator *>(node.release()));

			while (true)
			{
				const OperatorActions & operatorActions = inputOperator->GetActions();

				const Action & action = ResolveAction(operatorActions, *inputOperator);

				if (!action.Execute(*this, inputOperator))
				{
					break;
				}
			}
		}
	}

	const SyntaxAnalyzer::Action & SyntaxAnalyzer::ResolveAction(const OperatorActions & operatorActions, const Expression::Operator & inputOperator)
	{
		if (operators_.IsEmpty())
		{
			return operatorActions.EmptyStackAction(inputOperator);
		}
		else if (operators_.Top().IsOperand())
		{
			return operatorActions.OperandAction(inputOperator);
		}
		else
		{
			return operatorActions.OperatorAction(inputOperator, static_cast<Expression::Operator &>(operators_.Top()));
		}
	}

	const SyntaxAnalyzer::ActionNothing SyntaxAnalyzer::OperatorActions::NOTHING;
	const SyntaxAnalyzer::ActionPush    SyntaxAnalyzer::OperatorActions::PUSH;
	const SyntaxAnalyzer::ActionPop     SyntaxAnalyzer::OperatorActions::POP;
}

