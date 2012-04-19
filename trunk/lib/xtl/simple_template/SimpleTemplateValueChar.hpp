#ifndef XTL__SIMPLE_TEMPLATE_VALUE_CHAR_HPP__
#define XTL__SIMPLE_TEMPLATE_VALUE_CHAR_HPP__ 1

namespace XTL
{
	class SimpleTemplateValueChar : public SimpleTemplateValue
	{
		public:

			SimpleTemplateValueChar(char value)
				: value_(value) { ;; }

			virtual ~SimpleTemplateValueChar() throw() { ;; }

			virtual const char * Chars() const
			{
				return &value_;
			}

			virtual unsigned int Size() const
			{
				return 1;
			}

		private:

			const char value_;
	};

	template <>
	struct SimpleTemplateValueCreator<char>
	{
		static SimpleTemplateValue * CreateCopy(char value)
		{
			return new SimpleTemplateValueChar(value);
		}

		static SimpleTemplateValue * CreateReference(char value)
		{
			return CreateCopy(value);
		}
	};
}

#endif

