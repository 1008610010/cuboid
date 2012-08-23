#ifndef XTL__SYNTAX_ANALYZER_HPP__
#define XTL__SYNTAX_ANALYZER_HPP__ 1

#include <xtl/utils/AutoPtrStack.hpp>

#include "Expression.hpp"

namespace XTL
{
	class SyntaxAnalyzer
	{
		public:

			class Action
			{
				public:

					virtual ~Action() throw() { ;; }

					virtual bool Execute(SyntaxAnalyzer & syntaxAnalyzer, std::auto_ptr<Expression::Operator> & inputOperator) const = 0;
			};

			class OperatorActions
			{
				public:

					virtual ~OperatorActions() throw() { ;; }

					virtual const Action & EmptyStackAction() const = 0;

					virtual const Action & OperandAction() const = 0;

					virtual const Action & OperatorAction(const Expression::Operator & stackOperator) const = 0;
			};

			SyntaxAnalyzer()
				: operands_(),
				  operators_()
			{
				;;
			}

			void Process(std::auto_ptr<Expression::Node> node);

			void Pop()
			{
				std::auto_ptr<Expression::Node> node = operators_.Pop();
				node->Reduce(operands_);
				operands_.Push(node);
			}

			void Push(std::auto_ptr<Expression::Node> node)
			{
				operators_.Push(node);
			}
/*
			void Push(std::auto_ptr<Expression::Operator> node)
			{
				operators_.Push(std::auto_ptr<Expression::Node>(node));
			}
*/
			void Reduce()
			{
				operators_.Top().Reduce(operands_);
			}


		private:

			const Action & ResolveAction(const OperatorActions & operatorActions);

			XTL::AutoPtrStack<Expression::Node> operands_;
			XTL::AutoPtrStack<Expression::Node> operators_;
	};
}

#endif

