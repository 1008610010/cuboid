#ifndef XTL__EXPRESSION_HPP__
#define XTL__EXPRESSION_HPP__ 1

#include <xtl/PrintStream.hpp>
#include <xtl/utils/AutoPtrStack.hpp>

namespace XTL
{
	class Expression
	{
		public:

			class Node
			{
				public:

					Node() { ;; }

					virtual ~Node() throw() { ;; }

					virtual bool IsOperand() const = 0;

					virtual void Reduce(XTL::AutoPtrStack<Node> & paramsStack) = 0;

					virtual const std::string ToString() const = 0;

					virtual void DebugPrint(XTL::PrintStream & stream) = 0;

				private:

					Node(const Node &);
					Node & operator= (const Node &);
			};

			class Operand : public Node
			{
				public:

					virtual ~Operand() throw() { ;; }

					virtual bool IsOperand() const
					{
						return true;
					}

					virtual void Reduce(XTL::AutoPtrStack<Node> & paramsStack)
					{
						// Do nothing.
					}
			};

			class Operator;
	};
}

#endif

