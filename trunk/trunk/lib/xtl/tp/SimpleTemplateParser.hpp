#ifndef XTL__SIMPLE_TEMPLATE_PARSER_HPP__
#define XTL__SIMPLE_TEMPLATE_PARSER_HPP__ 1

namespace XTL
{
	/*
		Listener - класс, должен поддерживать интерфейс:
			void OnText(const char * ptr, int size);
			void OnParameter(int paramIndex);
	*/
	template <class Listener>
	class SimpleTemplateParser
	{
		public:

			static const char DEFAULT_CONTROL_CHAR = '$';

			// TODO: заменить "const char *" на объект класса CharSourceText
			SimpleTemplateParser(const char * source, Listener & listener, char controlChar = DEFAULT_CONTROL_CHAR)
				: ptr_(source),
				  textBegin_(source),
				  currentParamIndex_(0),
				  listener_(listener),
				  controlChar_(controlChar)
			{
				Parse();
			}

		private:

			void Parse()
			{
				if (ptr_ == 0) {
					return;
				}

				while (*ptr_ != '\0')
				{
					if (*ptr_ == controlChar_) {
						++ptr_;
						ParseControlChar();
					}
					else {
						++ptr_;
					}
				}
				OnText(ptr_);
			}

			void OnText(const char * textEnd)
			{
				if (textBegin_ < textEnd) {
					listener_.OnText(textBegin_, textEnd - textBegin_);
				}
			}

			void OnParameter(int paramIndex)
			{
				listener_.OnParameter(paramIndex);
			}

			void ParseControlChar()
			{
				if (*ptr_ == controlChar_) { // $$
					OnText(ptr_ - 1);
					textBegin_ = ptr_;
					++ptr_;
				}
				else if (*ptr_ == '_') { // $_
					OnText(ptr_ - 1);
					OnParameter(currentParamIndex_++);
					++ptr_;
					textBegin_ = ptr_;
				}
				else if (IsDigitDec(*ptr_)) { // $1
					OnText(ptr_ - 1);
					OnParameter(UnpackInteger());
					textBegin_ = ptr_;
				}
				else {
					// just skip it
					++ptr_;
				}
			}

			// COND: IsDigitDec(*ptr_) == true
			int UnpackInteger()
			{
				int result = 0;

				do {
					result = 10 * result + UnpackDigitDec(*ptr_);
					++ptr_;
				}
				while (IsDigitDec(*ptr_));

				return result;
			}

			bool IsDigitDec(char c) const
			{
				return c >= '0' && c <= '9';
			}

			int UnpackDigitDec(char c) const
			{
				return c - '0';
			}

			const char * ptr_;
			const char * textBegin_;
			int          currentParamIndex_;
			Listener   & listener_;
			const char   controlChar_;
	};

	template <typename Listener>
	void ParseSimpleTemplate(const char * source, Listener & listener)
	{
		SimpleTemplateParser<Listener>(source, listener);
	}
}

#endif

