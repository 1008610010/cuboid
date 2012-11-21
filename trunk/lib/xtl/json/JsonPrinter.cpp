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
		class NullLiteral : public JsonPrinter::Literal
		{
			public:

				virtual void Print(PrintStream & stream) const
				{
					stream.Print("null");
				}
		};

		class BooleanLiteral : public JsonPrinter::Literal
		{
			public:

				BooleanLiteral(bool value)
					: value_(value)
				{
					;;
				}

				virtual void Print(PrintStream & stream) const
				{
					stream.Print(value_ ? "true" : "false");
				}

			private:

				const bool value_;
		};

		template <typename T>
		class IntegerLiteral : public JsonPrinter::Literal
		{
			public:

				IntegerLiteral(const T & value)
					: value_(value)
				{
					;;
				}

				virtual void Print(PrintStream & stream) const
				{
					IntegerStringifier<T> stringifier(value_);

					stream.Print(stringifier.Chars(), stringifier.Size());
				}

			private:

				const T value_;
		};

		class DoubleLiteral : public JsonPrinter::Literal
		{
			public:

				DoubleLiteral(double value)
					: value_(value)
				{
					;;
				}

				virtual void Print(PrintStream & stream) const
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

		class ConstCharPtrLiteral : public JsonPrinter::Literal
		{
			public:

				ConstCharPtrLiteral(const char * value)
					: value_(value)
				{
					;;
				}

				virtual bool CanBeObjectKey() const
				{
					return true;
				}

				virtual void Print(PrintStream & stream) const
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

	JsonPrinter::JsonPrinter(PrintStream & printStream, Spacer & spacer)
		: printStream_(printStream),
		  spacer_(spacer),
		  stack_(new JsonPrinterStack())
	{
		stack_->Push(JSON_ROOT);
	}

	// Определяем деструктор явно, т.к. класс содержит auto_ptr-указатель
	// объект класса, объявленного как forward declaration.
	JsonPrinter::~JsonPrinter() throw()
	{
		;;
	}

	JsonPrinter & JsonPrinter::operator<< (JSON::Null)
	{
		return *this << NullLiteral();
	}

	JsonPrinter & JsonPrinter::operator<< (bool value)
	{
		return *this << BooleanLiteral(value);
	}

	JsonPrinter & JsonPrinter::operator<< (const char * value)
	{
		return *this << ConstCharPtrLiteral(value);
	}

	JsonPrinter & JsonPrinter::operator<< (const std::string & value)
	{
		return *this << ConstCharPtrLiteral(value.c_str());
	}

	JsonPrinter & JsonPrinter::operator<< (char value)
	{
		char temp[] = { value, '\0' };

		return *this << ConstCharPtrLiteral(temp);
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
		return *this << MakeIntegerLiteral(value);
	}

	JsonPrinter & JsonPrinter::operator<< (unsigned int value)
	{
		return *this << MakeIntegerLiteral(value);
	}

	JsonPrinter & JsonPrinter::operator<< (long int value)
	{
		return *this << MakeIntegerLiteral(value);
	}

	JsonPrinter & JsonPrinter::operator<< (unsigned long int value)
	{
		return *this << MakeIntegerLiteral(value);
	}

	JsonPrinter & JsonPrinter::operator<< (long long int value)
	{
		return *this << MakeIntegerLiteral(value);
	}

	JsonPrinter & JsonPrinter::operator<< (unsigned long long int value)
	{
		return *this << MakeIntegerLiteral(value);
	}

	JsonPrinter & JsonPrinter::operator<< (double value)
	{
		return *this << DoubleLiteral(value);
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

	JsonPrinter & JsonPrinter::operator<< (JSON::Object)
	{
		switch (stack_->Top())
		{
			case JSON_ROOT:
				break;

			case LINEAR_ARRAY_BEGIN:
			case LINEAR_ARRAY_ITEM:
			case LINEAR_OBJECT_KEY:
				return *this << JSON::LinearObject();

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
				RaiseError("Begin of object received, while string key expected.");

			default:
				RaiseError("Internal error");
		}

		Char('{');
		IncIndent();
		stack_->Push(MULTILINE_OBJECT_BEGIN);

		return *this;
	}

	JsonPrinter & JsonPrinter::operator<< (JSON::End)
	{
		switch (stack_->Top())
		{
			case JSON_ROOT:
				RaiseError("Unexpected 'end' tag");

			case LINEAR_ARRAY_BEGIN:
				Char(']');
				break;

			case LINEAR_ARRAY_ITEM:
				Space();
				Char(']');
				break;

			case LINEAR_OBJECT_BEGIN:
				Char('}');
				break;

			case LINEAR_OBJECT_VALUE:
				Space();
				Char('}');
				break;

			case MULTILINE_ARRAY_BEGIN:
				// DecIndent();
				// NextLine();
				Char(']');
				break;

			case MULTILINE_ARRAY_ITEM:
				DecIndent();
				NextLine();
				Char(']');
				break;

			case MULTILINE_OBJECT_BEGIN:
				// DecIndent();
				// NextLine();
				Char('}');
				break;

			case MULTILINE_OBJECT_VALUE:
				DecIndent();
				NextLine();
				Char('}');
				break;

			case LINEAR_OBJECT_KEY:
			case MULTILINE_OBJECT_KEY:
				RaiseError("End of object received, while string key expected.");

			default:
				RaiseError("Internal error");
		}

		stack_->Pop();

		return *this;
	}

	void JsonPrinter::operator<< (JSON::LinearFinish)
	{
		while (stack_->Top() != JSON_ROOT)
		{
			*this << JSON::End();
		}
	}

	void JsonPrinter::operator<< (JSON::Finish)
	{
		while (stack_->Top() != JSON_ROOT)
		{
			*this << JSON::End();
		}

		NextLine();
	}

	JsonPrinter & JsonPrinter::operator<< (const Literal & literal)
	{
		switch (stack_->Top())
		{
			case JSON_ROOT:
				RaiseError("Unexpected literal at root level");

			case LINEAR_ARRAY_BEGIN:
				Space();
				literal.Print(printStream_);
				stack_->SetTop(LINEAR_ARRAY_ITEM);
				break;

			case LINEAR_ARRAY_ITEM:
				Char(',');
				Space();
				literal.Print(printStream_);
				break;

			case LINEAR_OBJECT_BEGIN:
				if (!literal.CanBeObjectKey())
				{
					RaiseError("Object key as string literal expected");
				}
				Space();
				literal.Print(printStream_);
				Space();
				Char(':');
				stack_->SetTop(LINEAR_OBJECT_KEY);
				break;

			case LINEAR_OBJECT_KEY:
				Space();
				literal.Print(printStream_);
				stack_->SetTop(LINEAR_OBJECT_VALUE);
				break;

			case LINEAR_OBJECT_VALUE:
				if (!literal.CanBeObjectKey())
				{
					RaiseError("Object key as string literal expected");
				}
				Char(',');
				Space();
				literal.Print(printStream_);
				Space();
				Char(':');
				stack_->SetTop(LINEAR_OBJECT_KEY);
				break;

			case MULTILINE_ARRAY_BEGIN:
				NextLine();
				literal.Print(printStream_);
				stack_->SetTop(MULTILINE_ARRAY_ITEM);
				break;

			case MULTILINE_ARRAY_ITEM:
				Char(',');
				NextLine();
				literal.Print(printStream_);
				break;

			case MULTILINE_OBJECT_BEGIN:
				if (!literal.CanBeObjectKey())
				{
					RaiseError("Object key as string literal expected");
				}
				NextLine();
				literal.Print(printStream_);
				Space();
				Char(':');
				stack_->SetTop(MULTILINE_OBJECT_KEY);
				break;

			case MULTILINE_OBJECT_KEY:
				Space();
				literal.Print(printStream_);
				stack_->SetTop(MULTILINE_OBJECT_VALUE);
				break;

			case MULTILINE_OBJECT_VALUE:
				if (!literal.CanBeObjectKey())
				{
					RaiseError("Object key as string literal expected");
				}
				Char(',');
				NextLine();
				literal.Print(printStream_);
				Space();
				Char(':');
				stack_->SetTop(MULTILINE_OBJECT_KEY);
				break;
		}

		return *this;
	}

	void JsonPrinter::RaiseError(const char * what)
	{
		throw std::runtime_error(what);
	}
}

/*
namespace XC3
{

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
*/
