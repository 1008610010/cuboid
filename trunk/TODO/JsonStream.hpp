#ifndef _XTL__JSON_STREAM_HPP__
#define _XTL__JSON_STREAM_HPP__ 1

#include <string.h>
#include <string>
#include <deque>
#include <stdexcept>
#include "StringUtils.hpp"

namespace json
{
	struct null {};

	struct array {};

	struct object {};
	
	struct plain_array {};
	
	struct plain_object {};
	
	struct end {};
	
	struct finish {};
}

namespace XTL
{
	class JsonStreamStack
	{
		public:

			JsonStreamStack() : items_() { ;; }
		
			const bool Empty() const
			{
				return items_.empty();
			}
		
			void Push(unsigned int value)
			{
				items_.push_back(value);
			}

			const unsigned int Top() const
			{
				return items_.back();
			}
		
			void SetTop(unsigned int value)
			{
				items_.back() = value;
			}

			void Pop()
			{
				items_.pop_back();
			}
	
		protected:

			std::deque<unsigned int> items_;
	};

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
				fprintf(file_, ptr);
			}
		
			void Write(const char * ptr, size_t size)
			{
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
		private:
		
			enum Level
			{
				JSON_ROOT,
				MULTILINE_ARRAY_BEGIN,
				MULTILINE_ARRAY_ITEM,
				MULTILINE_OBJECT_BEGIN,
				MULTILINE_OBJECT_KEY,
				MULTILINE_OBJECT_VALUE,
				PLAIN_ARRAY_BEGIN,
				PLAIN_ARRAY_ITEM,
				PLAIN_OBJECT_BEGIN,
				PLAIN_OBJECT_KEY,
				PLAIN_OBJECT_VALUE
			};
			
			class NullLiteral
			{
				public:
				
					enum { IsString = false };
				
					void Write(_Outputter & outputter) const
					{
						outputter.Write("null");
					}
			};
			
			class BooleanLiteral
			{
				public:
				
					enum { IsString = false };
				
					BooleanLiteral(bool value)
						: value_(value) { ;; }
						
					void Write(_Outputter & outputter) const
					{
						outputter.Write(value_ ? "true" : "false");
					}

				protected:
				
					const bool value_;
			};

			template <typename T>
			class IntegerLiteral
			{
				public:
				
					enum { IsString = false };

					IntegerLiteral(const T & value)
						: value_(value) { ;; }
						
					void Write(_Outputter & outputter) const
					{
						outputter.Write(IntegerDigits<T>(value_));
					}

				protected:

					const T value_;
			};
			
			class FloatLiteral
			{
				public:

					enum { IsString = false };
					
					FloatLiteral(const double & value)
						: value_(value) { ;; }
						
					void Write(_Outputter & outputter) const
					{
						outputter.Write(FloatDigits(value_));
					}
				
				protected:
				
					const double value_;
			};

			class StringLiteral
			{
				public:
				
					enum { IsString = true };

					StringLiteral(const std::string & value)
						: value_(value) { ;; }
						
					void Write(_Outputter & outputter) const
					{
						outputter.Write('"');

						// TODO: \uXXXX	
						const char * begin = value_.c_str();
						const char * ptr = begin;
						size_t size = value_.length();
						for (size_t i = 0; i < size; ++i, ++ptr)
						{
							char ch = JsonEscapeChar(*ptr);
							if (ch != 0)
							{
								if (begin < ptr)
								{
									outputter.Write(begin, ptr - begin);
									begin = ptr + 1;
								}
								outputter.Write('\\');
								outputter.Write(ch);
							}
						}

						if (begin < ptr)
						{
							outputter.Write(begin, ptr - begin);
						}

						outputter.Write('"');
					}

				protected:
				
					std::string value_;
			};

			class CStringLiteral
			{
				public:
				
					enum { IsString = true };
					
					CStringLiteral(const char * value)
						: value_(value) { ;; }
						
					void Write(_Outputter & outputter) const
					{
						outputter.Write('"');

						const char * begin = value_;
						const char * ptr = begin;
						for (; *ptr != '\0'; ++ptr)
						{
							char ch = JsonEscapeChar(*ptr);
							if (ch != 0)
							{
								if (begin < ptr)
								{
									outputter.Write(begin, ptr - begin);
								}
								outputter.Write('\\');
								outputter.Write(ch);
								begin = ptr + 1;
							}
						}

						if (begin < ptr)
						{
							outputter.Write(begin, ptr - begin);
						}

						outputter.Write('"');
					}

				protected:

					const char * value_;
			};

		public:
		
			JsonStream(_Outputter & outputter)
				: outputter_(outputter),
				  spacer_(),
				  stack_()
			{
				stack_.Push(JSON_ROOT);
			}

			void Finish();

			JsonStream & operator<< (json::plain_array);

			JsonStream & operator<< (json::plain_object);
			
			JsonStream & operator<< (json::array);

			JsonStream & operator<< (json::object);

			JsonStream & operator<< (json::end);

			void operator<< (json::finish)
			{
				Finish();
			}

			template <typename _Literal>
			JsonStream & WriteLiteral(const _Literal & literal);
			
			JsonStream & operator<< (const char * value)
			{
				return WriteLiteral(CStringLiteral(value));
			}
			
			JsonStream & operator<< (const std::string & value)
			{
				return WriteLiteral(StringLiteral(value));
			}
			
			JsonStream & operator<< (json::null)
			{
				return WriteLiteral(NullLiteral());
			}
			
			JsonStream & operator<< (bool value)
			{
				return WriteLiteral(BooleanLiteral(value));
			}
			
			JsonStream & operator<< (const int & value)
			{
				return WriteLiteral(IntegerLiteral<int>(value));
			}
			
			JsonStream & operator<< (const unsigned int & value)
			{
				return WriteLiteral(IntegerLiteral<unsigned int>(value));
			}
			
			JsonStream & operator<< (const long int & value)
			{
				return WriteLiteral(IntegerLiteral<long int>(value));
			}
			
			JsonStream & operator<< (const unsigned long int & value)
			{
				return WriteLiteral(IntegerLiteral<unsigned long int>(value));
			}
			
			JsonStream & operator<< (const long long int & value)
			{
				return WriteLiteral(IntegerLiteral<long long int>(value));
			}
			
			JsonStream & operator<< (const unsigned long long int & value)
			{
				return WriteLiteral(IntegerLiteral<unsigned long long int>(value));
			}
			
			JsonStream & operator<< (const double & value)
			{
				return WriteLiteral(FloatLiteral(value));
			}

		protected:
		
			void RaiseError(const std::string & what)
			{
				throw std::runtime_error(what);
			}
			
			void Space()
			{
				spacer_.WriteSpace(outputter_);
			}
			
			void NextLine()
			{
				spacer_.WriteNextLine(outputter_);
			}
			
			void Char(char ch)
			{
				outputter_.Write(ch);
			}
			
			_Outputter      & outputter_;
			_Spacer           spacer_;
			JsonStreamStack   stack_;
	};
}

#include "JsonStream.tcc"

#endif

