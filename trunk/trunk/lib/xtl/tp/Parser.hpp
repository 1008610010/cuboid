#ifndef XTL__PARSER_HPP__
#define XTL__PARSER_HPP__ 1

#include "../Types.hpp"
#include "../FormatString.hpp"

#include "CharClass.hpp"
#include "CharClassifier.hpp"
#include "CharSource.hpp"

namespace XTL
{
	class Parser
	{
		public:

			static const CharClassBits CHAR_NULL             = 0x00000001; // '\0'
			static const CharClassBits CHAR_DECIMAL          = 0x00000002; // '0' .. '9'
			static const CharClassBits CHAR_LETTER_UC        = 0x00000004; // 'A' .. 'Z'
			static const CharClassBits CHAR_LETTER_LC        = 0x00000008; // 'a' .. 'z'
			static const CharClassBits CHAR_HEX_UC           = 0x00000010; // 'A' .. 'F'
			static const CharClassBits CHAR_HEX_LC           = 0x00000020; // 'a' .. 'f'
			static const CharClassBits CHAR_OCTAL            = 0x00000040; // '0' .. '7'
			static const CharClassBits CHAR_BINARY           = 0x00000080; // '0' .. '1'
			static const CharClassBits CHAR_SPACE            = 0x00000100; // ' '
			static const CharClassBits CHAR_TAB              = 0x00000200; // '\t'
			static const CharClassBits CHAR_CARRIAGE_RETURN  = 0x00000400; // '\r'
			static const CharClassBits CHAR_LINE_FEED        = 0x00000800; // '\n'
			static const CharClassBits CHAR_PLUS             = 0x00001000; // '+'
			static const CharClassBits CHAR_MINUS            = 0x00002000; // '-'
			static const CharClassBits CHAR_UNDERSCORE       = 0x00004000; // '_'

			static const CharClassBits CHAR_LETTER           = CHAR_LETTER_UC | CHAR_LETTER_LC;

			class Error
			{
				public:

					Error(const TextCursor & cursor, const std::string & what)
						: cursor_(cursor),
						  what_(what)
					{
						;;
					}

					const TextCursor & Cursor() const throw()
					{
						return cursor_;
					}

					const char * What() const throw()
					{
						return what_.c_str();
					}

				private:

					const TextCursor  cursor_;
					const std::string what_;
			};

			class EndOfFile
			{
			};

			static const CharClass CreateCommonClass(CharClassBits classBits)
			{
				return CommonCharClassifier::Instance().CreateClass(classBits);
			}

		protected:

			explicit Parser(CharSource & charSource);

			CharSource & GetCharSource() const      { return charSource_; }

			bool AtEnd() const                      { return charSource_.AtEnd(); }

			bool NotAtEnd() const                   { return charSource_.NotAtEnd(); }

			char GetChar() const                    { return charSource_.GetChar(); }

			void Advance()                          { charSource_.Advance(); }

			unsigned int Mark()                     { return charSource_.Mark(); }

			void Unmark()                           { charSource_.Unmark(); }

			const std::string ReleaseString()       { return charSource_.ReleaseString(); }

			void ReleaseString(CharBuffer & buffer) { charSource_.ReleaseString(buffer); }

			const TextCursor GetCursor() const      { return charSource_.GetCursor(); }

			const TextCursor ReleaseCursor()        { return charSource_.ReleaseCursor(); }

			bool InClass(const CharClass & charClass) const    { return charClass.Contains(GetChar()); }

			bool NotInClass(const CharClass & charClass) const { return !charClass.Contains(GetChar()); }

			/**
			 * @return Current character
			 * @throws EndOfFile - if char source is at end;
			 */
			char NeedChar() const;

			/**
			 * @throws EndOfFile - if end of char source was reached;
			 */
			void WaitChar(char c);

			/**
			 * @throws EndOfFile - if end of char source was reached;
			 */
			void SkipChars(char c);

			/**
			 * @throws EndOfFile - if end of char source was reached;
			 */
			void SkipCharClass(const CharClass & charClass);

			/**
			 * Если текущий символ c, то сдвигаем позицию.
			 * @return true  если текущий символ c
			 * @return false и противном случае
			 */
			bool ReadChar(char c)
			{
				if (AtEnd() || GetChar() != c)
				{
					return false;
				}

				Advance();
				return true;
			}

			bool ReadString(const char * s)
			{
				for (; *s != '\0'; ++s)
				{
					if (AtEnd() || GetChar() != *s)
					{
						return false;
					}

					Advance();
				}

				return true;
			}

			/**
			 * Precondition: NotAtEnd() && CharClass::IDENTIFIER_HEAD.Contains(GetChar())
			 */
			const std::string ReadIdentifier();

			void ThrowError(const TextCursor & cursor, const char * what)
			{
				throw Error(cursor, what);
			}

			template <typename T1>
			void ThrowError(const TextCursor & cursor, const char * format, const T1 & t1)
			{
				throw Error(cursor, XTL::FormatString(format, t1));
			}

			template <typename T1, typename T2>
			void ThrowError(const TextCursor & cursor, const char * format, const T1 & t1, const T2 & t2)
			{
				throw Error(cursor, XTL::FormatString(format, t1, t2));
			}

			void ThrowError(const char * what)
			{
				ThrowError(GetCursor(), what);
			}

			template <typename T1>
			void ThrowError(const char * format, const T1 & t1)
			{
				ThrowError(GetCursor(), format, t1);
			}

			template <typename T1, typename T2>
			void ThrowError(const char * format, const T1 & t1, const T2 & t2)
			{
				throw Error(GetCursor(), XTL::FormatString(format, t1, t2));
			}

		private:

			CharSource & charSource_;

		protected:

			class CommonCharClassifier : public CharClassifier
			{
				public:

					static const CommonCharClassifier & Instance()
					{
						static CommonCharClassifier instance;

						return instance;
					}

				protected:

					CommonCharClassifier()
						: CharClassifier()
					{
						Add(CHAR_NULL,             '\0');
						Add(CHAR_DECIMAL,          '0', '9');
						Add(CHAR_OCTAL,            '0', '7');
						Add(CHAR_BINARY,           '0', '1');
						Add(CHAR_LETTER_UC,        'A', 'Z');
						Add(CHAR_LETTER_LC,        'a', 'z');
						Add(CHAR_HEX_UC,           'A', 'F');
						Add(CHAR_HEX_LC,           'a', 'f');
						Add(CHAR_SPACE,            ' ');
						Add(CHAR_TAB,              '\t');
						Add(CHAR_CARRIAGE_RETURN,  '\r');
						Add(CHAR_LINE_FEED,        '\n');
						Add(CHAR_PLUS,             '+');
						Add(CHAR_MINUS,            '-');
						Add(CHAR_UNDERSCORE,       '_');
					}
			};
	};
}

#endif

