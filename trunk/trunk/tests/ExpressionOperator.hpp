#ifndef XTL__EXPRESSION_OPERATOR_HPP__
#define XTL__EXPRESSION_OPERATOR_HPP__ 1

#include <stdexcept>

#include "Expression.hpp"
#include "SyntaxAnalyzer.hpp"

namespace XTL
{
	class Expression::Operator : public Expression::Node
	{
		public:

			Operator()
				: finished_(false),
				  params_()
			{
				;;
			}

			virtual ~Operator() throw() { ;; }

			virtual bool IsOperand() const
			{
				return IsFinished();
			}

			bool IsFinished() const
			{
				return finished_;
			}

			virtual unsigned int Id() const = 0;

			virtual unsigned int InputPriority() const = 0;

			virtual unsigned int StackPriority() const = 0;

			virtual unsigned int NeedParamsCount() const = 0;

			virtual bool IsMultiParams() const
			{
				return false;
			}

			virtual const SyntaxAnalyzer::OperatorActions & GetActions() const = 0;

			virtual void Reduce(XTL::AutoPtrStack<Node> & paramsStack)
			{
				if (IsFinished())
				{
					return;
				}

				AutoPtrStack<Expression::Node> tempStack;
				for (unsigned int i = 0; i < NeedParamsCount(); ++i)
				{
					if (paramsStack.IsEmpty())
					{
						throw std::runtime_error("Syntax Error");
					}

					tempStack.Push(paramsStack.Pop());
				}

				for (unsigned int i = 0; i < NeedParamsCount(); ++i)
				{
					// TODO: pop all paramsCount_ items from paramsStack
					AddParam(tempStack.Pop());
				}

				finished_ = true;
			}

/*
				void IncParamsCount()
				{
					if (IsFinished())
					{
						throw std::runtime_error("Internal Error");
					}

					++paramsCount_;
				}
*/

			virtual const std::string ToString() const
			{
				return "OPERATOR";
			}

			virtual void DebugPrint(XTL::PrintStream & stream)
			{
				stream.Print("(");

				for (unsigned int i = 0; i < params_.Size(); ++i)
				{
					params_[i]->DebugPrint(stream);
					stream.Print(" ");
				}

				stream.Print(ToConstCharPtr());
				stream.Print(")");
			}
			
		protected:

			virtual const char * ToConstCharPtr() const = 0;

		private:

			void AddParam(std::auto_ptr<Node> param)
			{
				if (!param->IsOperand())
				{
					throw std::runtime_error("Internal Error");
				}

				params_.PushBack(param);
			}

			bool finished_;
			AutoPtrVector<Node> params_;
	};
}

#endif

