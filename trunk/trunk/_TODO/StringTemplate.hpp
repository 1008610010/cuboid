#ifndef __XTL__STRING_TEMPLATE_HPP__
#define __XTL__STRING_TEMPLATE_HPP__ 1

#include <string>
#include <vector>
#include <list>

namespace XTL
{
	class CharStream;

	class StringTemplateVar
	{
		public:

			StringTemplateVar()
				: value_()
			{
				;;
			}

			const char * Chars() const
			{
				return value_.data();
			}

			const unsigned int Size() const
			{
				return value_.size();
			}

			void Set(const char * ptr, unsigned int size)
			{
				value_.assign(ptr, size);
			}

			void Set(const std::string & value)
			{
				value_.assign(value);
			}

		protected:

			std::string value_;
	};

	class StringTemplateContext
	{
		public:

			StringTemplateContext(unsigned int varsCount);

			StringTemplateVar & operator[] (unsigned int index);

		protected:

			friend class StringTemplate;

			void Write(CharStream & stream, unsigned int index) const;

			std::vector<StringTemplateVar> vars_;
	};

	class StringTemplate
	{
		public:

			StringTemplate(const char * format);

			~StringTemplate() throw();

			void Parse(const char * format);

			void Evaluate(CharStream & output, const StringTemplateContext & context) const;

			const std::string Evaluate(const StringTemplateContext & context) const;

			const std::string operator() () const
			{
				StringTemplateContext context(0);
				return Evaluate(context);
			}

			template <typename T1>
			const std::string operator() (const T1 & t1) const
			{
				StringTemplateContext context(1);
				context[0] << t1;
				return Evaluate(context);
			}

			template <typename T1,
			          typename T2>
			const std::string operator() (const T1 & t1,
			                              const T2 & t2) const
			{
				StringTemplateContext context(2);
				context[0] << t1;
				context[1] << t2;
				return Evaluate(context);
			}

			template <typename T1,
			          typename T2,
			          typename T3>
			const std::string operator() (const T1 & t1,
			                              const T2 & t2,
			                              const T3 & t3) const
			{
				StringTemplateContext context(3);
				context[0] << t1;
				context[1] << t2;
				context[2] << t3;
				return Evaluate(context);
			}

			template <typename T1,
			          typename T2,
			          typename T3,
			          typename T4>
			const std::string operator() (const T1 & t1,
			                              const T2 & t2,
			                              const T3 & t3,
			                              const T4 & t4) const
			{
				StringTemplateContext context(4);
				context[0] << t1;
				context[1] << t2;
				context[2] << t3;
				context[3] << t4;
				return Evaluate(context);
			}

			template <typename T1,
			          typename T2,
			          typename T3,
			          typename T4,
			          typename T5>
			const std::string operator() (const T1 & t1,
			                              const T2 & t2,
			                              const T3 & t3,
			                              const T4 & t4,
			                              const T5 & t5) const
			{
				StringTemplateContext context(4);
				context[0] << t1;
				context[1] << t2;
				context[2] << t3;
				context[3] << t4;
				context[4] << t5;
				return Evaluate(context);
			}

			template <typename T1,
			          typename T2,
			          typename T3,
			          typename T4,
			          typename T5,
			          typename T6>
			const std::string operator() (const T1 & t1,
			                              const T2 & t2,
			                              const T3 & t3,
			                              const T4 & t4,
			                              const T5 & t5,
			                              const T6 & t6) const
			{
				StringTemplateContext context(4);
				context[0] << t1;
				context[1] << t2;
				context[2] << t3;
				context[3] << t4;
				context[4] << t5;
				context[5] << t6;
				return Evaluate(context);
			}

		protected:

			class Node
			{
				public:

					Node() { ;; }

					virtual ~Node() throw() { ;; }

					virtual void Write(CharStream & stream, const StringTemplateContext & context) const = 0;

				protected:

					Node(const Node &);
					Node & operator= (const Node &);
			};

			class Constant : public Node
			{
				public:

					Constant(const char * ptr, unsigned int size)
						: Node(), value_(ptr, size) { ;; }

					virtual ~Constant() throw() { ;; }

					virtual void Write(CharStream & stream, const StringTemplateContext & context) const;

				protected:

					const std::string value_;
			};

			class Variable : public Node
			{
				public:

					Variable(unsigned int index)
						: Node(), index_(index) { ;; }

					virtual ~Variable() throw() { ;; }

					virtual void Write(CharStream & stream, const StringTemplateContext & context) const;

				protected:

					unsigned int index_;
			};

			class List : public Node
			{
				public:

					List()
						: Node(), items_() { ;; }

					virtual ~List() throw();

					virtual void Write(CharStream & stream, const StringTemplateContext & context) const;

					void Add(Node * node)
					{
						items_.push_back(node);
					}

				protected:

					std::list<Node *> items_;
			};

		protected:

			List * root_;
	};

	void operator<< (StringTemplateVar & var, const char * value);

	void operator<< (StringTemplateVar & var, const std::string & value);

	void operator<< (StringTemplateVar & var, int value);

	void operator<< (StringTemplateVar & var, unsigned int value);

	void operator<< (StringTemplateVar & var, long long int value);

	void operator<< (StringTemplateVar & var, unsigned long long int value);
}

#define STRING_TEMPLATE(Name, Format) static const XTL::StringTemplate Name(Format)

#endif
