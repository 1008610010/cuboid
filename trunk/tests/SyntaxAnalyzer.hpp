#ifndef XTL__SYNTAX_ANALYZER_HPP__
#define XTL__SYNTAX_ANALYZER_HPP__ 1

#include <xtl/utils/AutoPtrStack.hpp>

#include "Expression.hpp"

namespace XTL
{
	class SyntaxAnalyzer
	{
		public:

			struct Action
			{
				virtual ~Action() throw() { ;; }

				virtual bool Execute(SyntaxAnalyzer & syntaxAnalyzer, std::auto_ptr<Expression::Operator> & inputOperator) const = 0;
			};

			struct ActionNothing : public Action
			{
				ActionNothing() { ;; }

				virtual ~ActionNothing() throw() { ;; }

				virtual bool Execute(SyntaxAnalyzer & syntaxAnalyzer, std::auto_ptr<Expression::Operator> & inputOperator) const
				{
					return false;
				}
			};

			struct ActionPush : public Action
			{
				ActionPush() { ;; }

				virtual ~ActionPush() throw() { ;; }

				virtual bool Execute(SyntaxAnalyzer & syntaxAnalyzer, std::auto_ptr<Expression::Operator> & inputOperator) const
				{
					syntaxAnalyzer.Push(inputOperator);
					return false;
				}
			};

			struct ActionPop : public Action
			{
				ActionPop() { ;; }

				virtual ~ActionPop() throw() { ;; }

				virtual bool Execute(SyntaxAnalyzer & syntaxAnalyzer, std::auto_ptr<Expression::Operator> & inputOperator) const
				{
					syntaxAnalyzer.Pop();
					return true;
				}
			};

			class OperatorActions
			{
				public:

					virtual ~OperatorActions() throw() { ;; }

					virtual const Action & EmptyStackAction(const Expression::Operator & inputOperator) const = 0;

					virtual const Action & OperandAction(const Expression::Operator & inputOperator) const = 0;

					virtual const Action & OperatorAction(const Expression::Operator & inputOperator, const Expression::Operator & stackOperator) const = 0;

					static const ActionNothing NOTHING;
					static const ActionPush    PUSH;
					static const ActionPop     POP;
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

			void Push(std::auto_ptr<Expression::Operator> node)
			{
				operators_.Push(std::auto_ptr<Expression::Node>(node));
			}

			void Reduce()
			{
				operators_.Top().Reduce(operands_);
			}


		private:

			const Action & ResolveAction(const OperatorActions & operatorActions, const Expression::Operator & inputOperator);

			XTL::AutoPtrStack<Expression::Node> operands_;
			XTL::AutoPtrStack<Expression::Node> operators_;
	};
}

#endif

