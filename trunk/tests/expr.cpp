#include <stdio.h>

#include <memory>
#include <stdexcept>

#include <xtl/utils/AutoPtrVector.hpp>

template <typename T> class AutoPtrStack
{
	public:

		AutoPtrStack()
			: items_()
		{
			;;
		}

		bool IsEmpty() const
		{
			return items_.IsEmpty();
		}

		void Push(std::auto_ptr<T> ptr)
		{
			items_.PushBack(ptr);
		}

		std::auto_ptr<T> Pop()
		{
			return items_.PopBack();
		}

		T & Top()
		{
			return *(items_.Back());
		}

		const T & Top() const
		{
			return *(items_.Back());
		}

	private:

		XTL::AutoPtrVector<T> items_;

/*
		T & Top() const
		{
			return *(nodes_.back());
		}

		std::auto_ptr<T> Pop()
		{
			std::auto_ptr<T> result(nodes_.back());
			nodes_.pop_back();
			return result;
		}
*/
};

class Expression
{
	public:
	
		class Node
		{
			public:

				Node() { ;; }

				virtual ~Node() throw() { ;; }

				virtual bool IsOperand() const = 0;

				virtual void Reduce(AutoPtrStack<Node> & paramsStack) = 0;

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

				virtual void Reduce(AutoPtrStack<Node> & paramsStack)
				{
					// Do nothing.
				}
		};

		class Operator : public Node
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

				virtual unsigned int Priority() const = 0;

				enum OperatorType
				{
					TN,  // +x
					NT,  // x++
					TNT, // ( x )
					NTN  // x + y
				};

				virtual OperatorType Type() const = 0;

				virtual unsigned int NeedParamsCount() const = 0;

				virtual bool IsMultiParams() const
				{
					return false;
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
/*
				virtual void Reduce(AutoPtrStack<Node> & paramsStack)
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
							throw std::runtime_error("Lexic Error");
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
*/
			private:

				void AddParam(std::auto_ptr<Node> param)
				{
					if (!param->IsOperand())
					{
						throw std::runtime_error("Internal Error");
					}

					params_.push_back(param.release());
				}

				bool finished_;
				std::vector<Node *> params_;
		};
};

int main(int argc, const char * argv[])
{
	AutoPtrStack<int> s;
	for (int i = 0; i < 10; ++i)
	{
		s.Push(std::auto_ptr<int>(new int(i)));
	}
	while (!s.IsEmpty())
	{
		std::auto_ptr<int> pi = s.Pop();
		printf("%d\n", *pi);
	}
}

