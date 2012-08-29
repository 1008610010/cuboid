#ifndef XTL__PARSER_HPP__
#define XTL__PARSER_HPP__ 1

#include "../Types.hpp"

#include "CharClass.hpp"
#include "CharClassifier.hpp"
#include "CharSource.hpp"

namespace XTL
{
	class Parser
	{
		public:

			static const CharClassBits CHAR_NULL             = 0x00000001; // '\0'
			static const CharClassBits CHAR_DIGIT            = 0x00000002; // '0' .. '9'
			static const CharClassBits CHAR_LETTER_UC        = 0x00000004; // 'A' .. 'Z'
			static const CharClassBits CHAR_LETTER_LC        = 0x00000008; // 'a' .. 'z'
			static const CharClassBits CHAR_HEX_UC           = 0x00000010; // 'A' .. 'F'
			static const CharClassBits CHAR_HEX_LC           = 0x00000020; // 'a' .. 'f'
			static const CharClassBits CHAR_SPACE            = 0x00000040; // ' '
			static const CharClassBits CHAR_TAB              = 0x00000080; // '\t'
			static const CharClassBits CHAR_CARRIAGE_RETURN  = 0x00000100; // '\r'
			static const CharClassBits CHAR_LINE_FEED        = 0x00000200; // '\n'
			static const CharClassBits CHAR_PLUS             = 0x00000400; // '+'
			static const CharClassBits CHAR_MINUS            = 0x00000800; // '-'
			static const CharClassBits CHAR_UNDERSCORE       = 0x00001000; // '_'

			static const CharClassBits CHAR_OCTAL            = 0x00002000; // '0' .. '7'
			static const CharClassBits CHAR_BINARY           = 0x00004000; // '0' .. '1'

			static const CharClassBits CHAR_LETTER           = CHAR_LETTER_UC | CHAR_LETTER_LC;

			class EndOfFile
			{
			};

			static const CharClass CreateCommonClass(CharClassBits classBits)
			{
				return CommonCharClassifier::Instance().CreateClass(classBits);
			}

		protected:

			explicit Parser(CharSource & charSource);

			bool NotAtEnd() const              { return charSource_.NotAtEnd(); }

			bool AtEnd() const                 { return charSource_.AtEnd(); }

			char GetChar() const               { return charSource_.GetChar(); }

			void Advance()                     { charSource_.Advance(); }

			void Mark()                        { charSource_.Mark(); }

			void Unmark()                      { charSource_.Unmark(); }

			const std::string ReleaseString()  { return charSource_.ReleaseString(); }

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

		private:

			CharSource & charSource_;

/*
			const std::string ReadIdentifier(const CharClass & headClass = IDENTIFIER_HEAD, const CharClass & tailClass = IDENTIFIER_TAIL)
			{
				if (headClass.Contains()IsInClass(headClass))
				{
					Mark();
					Advance();
					SkipCharClass(tailClass);

					return ReleaseString();
				}

				return std::string();
			}
*/
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
						Add(CHAR_DIGIT,            '0', '9');
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

