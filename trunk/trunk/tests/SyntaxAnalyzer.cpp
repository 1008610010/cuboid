#include "SyntaxAnalyzer.hpp"

#include "ExpressionOperator.hpp"

namespace XTL
{
	void SyntaxAnalyzer::Process(std::auto_ptr<Expression::Node> node)
	{
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

				const Action & action = ResolveAction(operatorActions);

				if (!action.Execute(*this, inputOperator))
				{
					break;
				}
			}
		}
	}

	const SyntaxAnalyzer::Action & SyntaxAnalyzer::ResolveAction(const OperatorActions & operatorActions)
	{
		if (operators_.IsEmpty())
		{
			return operatorActions.EmptyStackAction();
		}
		else if (operators_.Top().IsOperand())
		{
			return operatorActions.OperandAction();
		}
		else
		{
			return operatorActions.OperatorAction(static_cast<Expression::Operator &>(operators_.Top()));
		}
	}

}

