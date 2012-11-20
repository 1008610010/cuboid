#include "JsonPrinter.hpp"

#include <list>
#include <stdexcept>

#include <xtl/StringUtils.hpp>

namespace XTL
{
	namespace
	{
		enum StackItemType
		{
			JSON_ROOT,
			LINEAR_ARRAY_BEGIN,
			LINEAR_ARRAY_ITEM,
			LINEAR_OBJECT_BEGIN,
			LINEAR_OBJECT_KEY,
			LINEAR_OBJECT_VALUE,
			MULTILINE_ARRAY_BEGIN,
			MULTILINE_ARRAY_ITEM,
			MULTILINE_OBJECT_BEGIN,
			MULTILINE_OBJECT_KEY,
			MULTILINE_OBJECT_VALUE
		};
	}

	class JsonPrinterStack
	{
		public:

			JsonPrinterStack()
				: items_()
			{
				;;
			}

			void Push(StackItemType value)
			{
				items_.push_back(value);
			}

			StackItemType Top() const
			{
				return items_.back();
			}

			void SetTop(StackItemType value)
			{
				items_.back() = value;
			}

			void Pop()
			{
				items_.pop_back();
			}

		private:

			std::list<StackItemType> items_;
	};

	namespace
	{
		class NullLiteral
		{
			public:

				enum { CanBeObjectKey = false };

				void Print(PrintStream & stream) const
				{
					stream.Print("null");
				}
		};

		class BooleanLiteral
		{
			public:

				enum { CanBeObjectKey = false };

				BooleanLiteral(bool value)
					: value_(value)
				{
					;;
				}

				void Print(PrintStream & stream) const
				{
					stream.Print(value_ ? "true" : "false");
				}

			private:

				const bool value_;
		};

		template <typename T>
		class IntegerLiteral
		{
			public:

				enum { CanBeObjectKey = false };

				IntegerLiteral(const T & value)
					: value_(value)
				{
					;;
				}

				void Print(PrintStream & stream) const
				{
					IntegerStringifier<T> stringifier(value_);

					stream.Print(stringifier.Chars(), stringifier.Size());
				}

			private:

				const T value_;
		};

		class DoubleLiteral
		{
			public:

				enum { CanBeObjectKey = false };

				DoubleLiteral(double value)
					: value_(value)
				{
					;;
				}

				void Print(PrintStream & stream) const
				{
					FloatStringifier stringifier(value_);

					stream.Print(stringifier.Chars(), stringifier.Size());

					if (IsInteger(stringifier.Chars()))
					{
						stream.Print(".0");
					}
				}

			private:

				const double value_;
		};

		class ConstCharPtrLiteral
		{
			public:

				enum { IsString = true };

				ConstCharPtrLiteral(const char * value)
					: value_(value)
				{
					;;
				}

				void Print(PrintStream & stream) const
				{
					stream.Print("\"");

					const char * begin = value_;
					const char * ptr = begin;
					for (; *ptr != '\0'; ++ptr)
					{
						char ch = EscapeChar(*ptr);
						if (ch != 0)
						{
							if (begin < ptr)
							{
								stream.Print(begin, ptr - begin);
							}

							char escape[] = { '\\', ch };
							stream.Print(escape, 2);
							begin = ptr + 1;
						}
					}

					if (begin < ptr)
					{
						stream.Print(begin, ptr - begin);
					}

					stream.Print("\"");
				}

			private:

				static char EscapeChar(char ch)
				{
					static const char chars[] = {
						0, 0, 0, 0, 0, 0, 0, 0, 'b', 't', 'n', 0, 'f', 'r', 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, '"', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '/',
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '\\', 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
					};

					return chars[static_cast<unsigned char>(ch)];
				}

				const char * const value_;
		};

	}

	JsonPrinter::JsonPrinter(PrintStream & printStream)
		: printStream_(printStream),
		  stack_(new JsonPrinterStack())
	{
		stack_->Push(JSON_ROOT);
	}

	template <typename LiteralType>
	inline void PrintLiteral(PrintStream & printStream, const LiteralType & literal)
	{
		literal.Print(printStream);
	}

	JsonPrinter & JsonPrinter::operator<< (JSON::Null)
	{
		PrintLiteral(printStream_, NullLiteral());
		return *this;
	}

	JsonPrinter & JsonPrinter::operator<< (bool value)
	{
		PrintLiteral(printStream_, BooleanLiteral(value));
		return *this;
	}

	JsonPrinter & JsonPrinter::operator<< (const char * value)
	{
		PrintLiteral(printStream_, ConstCharPtrLiteral(value));
		return *this;
	}

	JsonPrinter & JsonPrinter::operator<< (const std::string & value)
	{
		PrintLiteral(printStream_, ConstCharPtrLiteral(value.c_str()));
		return *this;
	}

	JsonPrinter & JsonPrinter::operator<< (char value)
	{
		char temp[] = { value, '\0' };

		PrintLiteral(printStream_, ConstCharPtrLiteral(temp));
		return *this;
	}

	namespace
	{
		template <typename T>
		inline IntegerLiteral<T> MakeIntegerLiteral(T value)
		{
			return IntegerLiteral<T>(value);
		}
	}

	JsonPrinter & JsonPrinter::operator<< (int value)
	{
		PrintLiteral(printStream_, MakeIntegerLiteral(value));
		return *this;
	}

	JsonPrinter & JsonPrinter::operator<< (unsigned int value)
	{
		PrintLiteral(printStream_, MakeIntegerLiteral(value));
		return *this;
	}

	JsonPrinter & JsonPrinter::operator<< (long int value)
	{
		PrintLiteral(printStream_, MakeIntegerLiteral(value));
		return *this;
	}

	JsonPrinter & JsonPrinter::operator<< (unsigned long int value)
	{
		PrintLiteral(printStream_, MakeIntegerLiteral(value));
		return *this;
	}

	JsonPrinter & JsonPrinter::operator<< (long long int value)
	{
		PrintLiteral(printStream_, MakeIntegerLiteral(value));
		return *this;
	}

	JsonPrinter & JsonPrinter::operator<< (unsigned long long int value)
	{
		PrintLiteral(printStream_, MakeIntegerLiteral(value));
		return *this;
	}

	JsonPrinter & JsonPrinter::operator<< (double value)
	{
		PrintLiteral(printStream_, DoubleLiteral(value));
		return *this;
	}

	JsonPrinter & JsonPrinter::operator<< (JSON::LinearArray)
	{
		switch (stack_->Top())
		{
			case JSON_ROOT:
				break;

			case LINEAR_ARRAY_BEGIN:
				Space();
				stack_->SetTop(LINEAR_ARRAY_ITEM);
				break;

			case MULTILINE_ARRAY_BEGIN:
				NextLine();
				stack_->SetTop(MULTILINE_ARRAY_ITEM);
				break;

			case LINEAR_ARRAY_ITEM:
				Char(',');
				Space();
				break;

			case MULTILINE_ARRAY_ITEM:
				Char(',');
				NextLine();
				break;

			case LINEAR_OBJECT_KEY:
				Space();
				stack_->SetTop(LINEAR_OBJECT_VALUE);
				break;

			case MULTILINE_OBJECT_KEY:
				Space();
				stack_->SetTop(MULTILINE_OBJECT_VALUE);
				break;

			case LINEAR_OBJECT_BEGIN:
			case LINEAR_OBJECT_VALUE:
			case MULTILINE_OBJECT_BEGIN:
			case MULTILINE_OBJECT_VALUE:
				RaiseError("Begin of array received, while string key expected.");

			default:
				RaiseError("Internal error");
		}

		Char('[');
		stack_->Push(LINEAR_ARRAY_BEGIN);

		return *this;
	}

	JsonPrinter & JsonPrinter::operator<< (JSON::LinearObject)
	{
		switch (stack_->Top())
		{
			case JSON_ROOT:
				break;

			case LINEAR_ARRAY_BEGIN:
				Space();
				stack_->SetTop(LINEAR_ARRAY_ITEM);
				break;

			case MULTILINE_ARRAY_BEGIN:
				NextLine();
				stack_->SetTop(MULTILINE_ARRAY_ITEM);
				break;

			case LINEAR_ARRAY_ITEM:
				Char(',');
				Space();
				break;

			case MULTILINE_ARRAY_ITEM:
				Char(',');
				NextLine();
				break;

			case LINEAR_OBJECT_KEY:
				Space();
				stack_->SetTop(LINEAR_OBJECT_VALUE);
				break;

			case MULTILINE_OBJECT_KEY:
				Space();
				stack_->SetTop(MULTILINE_OBJECT_VALUE);
				break;

			case LINEAR_OBJECT_BEGIN:
			case LINEAR_OBJECT_VALUE:
			case MULTILINE_OBJECT_BEGIN:
			case MULTILINE_OBJECT_VALUE:
				RaiseError("Begin of object received, while string key expected.");

			default:
				RaiseError("Internal error");
		}

		Char('{');
		stack_->Push(LINEAR_OBJECT_BEGIN);

		return *this;
	}

	JsonPrinter & JsonPrinter::operator<< (JSON::Array)
	{
		switch (stack_->Top())
		{
			case JSON_ROOT:
				break;

			case LINEAR_ARRAY_BEGIN:
			case LINEAR_ARRAY_ITEM:
			case LINEAR_OBJECT_KEY:
				return *this << JSON::LinearArray();

			case MULTILINE_ARRAY_BEGIN:
				NextLine();
				stack_->SetTop(MULTILINE_ARRAY_ITEM);
				break;

			case MULTILINE_ARRAY_ITEM:
				Char(',');
				NextLine();
				break;

			case MULTILINE_OBJECT_KEY:
				Space();
				stack_->SetTop(MULTILINE_OBJECT_VALUE);
				break;

			case LINEAR_OBJECT_BEGIN:
			case LINEAR_OBJECT_VALUE:
			case MULTILINE_OBJECT_BEGIN:
			case MULTILINE_OBJECT_VALUE:
				RaiseError("Begin of array received, while string key expected.");

			default:
				RaiseError("Internal error");
		}

		Char('[');
		IncIndent();
		stack_->Push(MULTILINE_ARRAY_BEGIN);

		return *this;
	}

/*


			JsonPrinter & operator<< (JSON::Object);

			JsonPrinter & operator<< (JSON::End);

			JsonPrinter & operator<< (JSON::LinearFinish);

			JsonPrinter & operator<< (JSON::Finish);
*/

	void JsonPrinter::RaiseError(const char * what)
	{
		throw std::runtime_error(what);
	}
}

/*
namespace XC3
{
	class JsonSpacer
	{
		public:

			JsonSpacer()
				: indent_(0) { ;; }

			template <typename _Outputter>
			void WriteSpace(_Outputter & outputter)
			{
				outputter.Write(' ');
			}

			template <typename _Outputter>
			void WriteNewLine(_Outputter & outputter)
			{
				outputter.Write('\n');
			}

			template <typename _Outputter>
			void WriteIndent(_Outputter & outputter)
			{
				for (unsigned int i = 0; i < indent_; ++i)
				{
					outputter.Write("    ", 4);
				}
			}

			template <typename _Outputter>
			void WriteNextLine(_Outputter & outputter)
			{
				WriteNewLine(outputter);
				WriteIndent(outputter);
			}

			void IncIndent()
			{
				++indent_;
			}

			void DecIndent()
			{
				--indent_;
			}

		protected:

			unsigned int indent_;
	};

	struct JsonNullSpacer
	{
		template <typename _Outputter>
		void WriteSpace(_Outputter & outputter) { ;; }

		template <typename _Outputter>
		void WriteNewLine(_Outputter & outputter) { ;; }

		template <typename _Outputter>
		void WriteIndent(_Outputter & outputter) { ;; }

		template <typename _Outputter>
		void WriteNextLine(_Outputter & outputter) { ;; }

		void IncIndent() { ;; }

		void DecIndent() { ;; }
	};

	class JsonFileOutputter
	{
		public:

			JsonFileOutputter(FILE * file)
				: file_(file) { ;; }

			void Write(char ch)
			{
				fprintf(file_, "%c", ch);
			}

			void Write(const char * ptr)
			{
				fprintf(file_, "%s", ptr);
			}

			void Write(const char * ptr, size_t size)
			{
				// size_t result =
				fwrite(ptr, 1, size, file_);
			}

		protected:

			FILE * file_;
	};

	class JsonStringOutputter
	{
		public:

			JsonStringOutputter(std::string & s)
				: s_(s) { ;; }

			void Write(char ch)
			{
				s_.append(1, ch);
			}

			void Write(const char * ptr)
			{
				s_.append(ptr);
			}

			void Write(const char * ptr, size_t size)
			{
				s_.append(ptr, size);
			}

		protected:

			std::string & s_;
	};

	char JsonEscapeChar(char ch);

	template <typename _Outputter, typename _Spacer = JsonSpacer>
	class JsonStream
	{
		public:

			JsonStream(_Outputter & outputter)
				: outputter_(outputter),
				  spacer_(),
				  stack_()
			{
				stack_.Push(JSON_ROOT);
			}

			void Finish(bool newLine);

			JsonStream & operator<< (json::plain_array);

			JsonStream & operator<< (json::plain_object);

			JsonStream & operator<< (json::array);

			JsonStream & operator<< (json::object);

			JsonStream & operator<< (json::end);

			void operator<< (json::plain_finish)
			{
				Finish(false);
			}

			void operator<< (json::finish)
			{
				Finish(true);
			}

		protected:

			void RaiseError(const std::string & what)
			{
				throw std::runtime_error(what);
			}

			_Outputter      & outputter_;
			_Spacer           spacer_;
			JsonStreamStack   stack_;
	};
}

namespace XC3
{
	template <typename _Outputter, typename _Spacer>
	void JsonStream<_Outputter, _Spacer>::Finish(bool newLine)
	{
		while (stack_.Top() != JSON_ROOT)
		{
			*this << json::end();
		}

		if (newLine)
		{
			NextLine();
		}
	}

	template <typename _Outputter, typename _Spacer>
	JsonStream<_Outputter, _Spacer> & JsonStream<_Outputter, _Spacer>::operator<< (json::object)
	{
		switch (stack_.Top())
		{
			case JSON_ROOT:
				break;

			case PLAIN_ARRAY_BEGIN:
			case PLAIN_ARRAY_ITEM:
			case PLAIN_OBJECT_KEY:
				return *this << json::plain_object();

			case MULTILINE_ARRAY_BEGIN:
				NextLine();
				stack_.SetTop(MULTILINE_ARRAY_ITEM);
				break;

			case MULTILINE_ARRAY_ITEM:
				Char(',');
				NextLine();
				break;

			case MULTILINE_OBJECT_KEY:
				Space();
				stack_.SetTop(MULTILINE_OBJECT_VALUE);
				break;

			case PLAIN_OBJECT_BEGIN:
			case PLAIN_OBJECT_VALUE:
			case MULTILINE_OBJECT_BEGIN:
			case MULTILINE_OBJECT_VALUE:
				RaiseError("Begin of object received, while string key expected.");

			default:
				RaiseError("Internal error");
		}

		Char('{');
		spacer_.IncIndent();
		stack_.Push(MULTILINE_OBJECT_BEGIN);

		return *this;
	}

	template <typename _Outputter, typename _Spacer>
	JsonStream<_Outputter, _Spacer> & JsonStream<_Outputter, _Spacer>::operator<< (json::end)
	{
		switch (stack_.Top())
		{
			case JSON_ROOT:
				RaiseError("Unexpected 'end' tag");

			case PLAIN_ARRAY_BEGIN:
				Char(']');
				break;

			case MULTILINE_ARRAY_BEGIN:
				spacer_.DecIndent();
				NextLine();
				Char(']');
				break;

			case PLAIN_ARRAY_ITEM:
				Space();
				Char(']');
				break;

			case MULTILINE_ARRAY_ITEM:
				spacer_.DecIndent();
				NextLine();
				Char(']');
				break;

			case PLAIN_OBJECT_BEGIN:
				Char('}');
				break;

			case MULTILINE_OBJECT_BEGIN:
				spacer_.DecIndent();
				NextLine();
				Char('}');
				break;

			case PLAIN_OBJECT_VALUE:
				Space();
				Char('}');
				break;

			case MULTILINE_OBJECT_VALUE:
				spacer_.DecIndent();
				NextLine();
				Char('}');
				break;

			case PLAIN_OBJECT_KEY:
			case MULTILINE_OBJECT_KEY:
				RaiseError("End of object received, while string key expected.");

			default:
				RaiseError("Internal error");
		}

		stack_.Pop();

		return *this;
	}

	template <typename _Outputter, typename _Spacer>
	template <typename _Literal>
	JsonStream<_Outputter, _Spacer> & JsonStream<_Outputter, _Spacer>::WriteLiteral(const _Literal & literal)
	{
		switch (stack_.Top())
		{
			case JSON_ROOT:
				RaiseError("Unexpected literal at root level");

			case PLAIN_ARRAY_BEGIN:
				Space();
				literal.Write(outputter_);
				stack_.SetTop(PLAIN_ARRAY_ITEM);
				break;

			case PLAIN_ARRAY_ITEM:
				Char(',');
				Space();
				literal.Write(outputter_);
				break;

			case PLAIN_OBJECT_BEGIN:
				if (!_Literal::IsString)
				{
					RaiseError("Object key as string literal expected");
				}
				Space();
				literal.Write(outputter_);
				Space();
				Char(':');
				stack_.SetTop(PLAIN_OBJECT_KEY);
				break;

			case PLAIN_OBJECT_KEY:
				Space();
				literal.Write(outputter_);
				stack_.SetTop(PLAIN_OBJECT_VALUE);
				break;

			case PLAIN_OBJECT_VALUE:
				if (!_Literal::IsString)
				{
					RaiseError("Object key as string literal expected");
				}
				Char(',');
				Space();
				literal.Write(outputter_);
				Space();
				Char(':');
				stack_.SetTop(PLAIN_OBJECT_KEY);
				break;

			case MULTILINE_ARRAY_BEGIN:
				NextLine();
				literal.Write(outputter_);
				stack_.SetTop(MULTILINE_ARRAY_ITEM);
				break;

			case MULTILINE_ARRAY_ITEM:
				Char(',');
				NextLine();
				literal.Write(outputter_);
				break;

			case MULTILINE_OBJECT_BEGIN:
				if (!_Literal::IsString)
				{
					RaiseError("Object key as string literal expected");
				}
				NextLine();
				literal.Write(outputter_);
				Space();
				Char(':');
				stack_.SetTop(MULTILINE_OBJECT_KEY);
				break;

			case MULTILINE_OBJECT_KEY:
				Space();
				literal.Write(outputter_);
				stack_.SetTop(MULTILINE_OBJECT_VALUE);
				break;

			case MULTILINE_OBJECT_VALUE:
				if (!_Literal::IsString)
				{
					RaiseError("Object key as string literal expected");
				}
				Char(',');
				NextLine();
				literal.Write(outputter_);
				Space();
				Char(':');
				stack_.SetTop(MULTILINE_OBJECT_KEY);
				break;
		}

		return *this;
	}
}

*/




