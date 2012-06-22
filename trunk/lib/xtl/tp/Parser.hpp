#ifndef XTL__PARSER_HPP__
#define XTL__PARSER_HPP__ 1

#include "../Types.hpp"
#include "CharSource.hpp"

namespace XTL
{
	class Parser
	{
		public:

			static const XTL::UINT_32 CHAR_NULL             = 0x00000001
			static const XTL::UINT_32 CHAR_DIGIT            = 0x00000002; // '0' .. '9'
			static const XTL::UINT_32 CHAR_LETTER_UPPERCASE = 0x00000004; // 'A' .. 'Z'
			static const XTL::UINT_32 CHAR_LETTER_LOWERCASE = 0x00000008; // 'a' .. 'z'
			static const XTL::UINT_32 CHAR_HEX_UPPERCASE    = 0x00000010; // 'A' .. 'F'
			static const XTL::UINT_32 CHAR_HEX_LOWERCASE    = 0x00000020; // 'a' .. 'f'
			static const XTL::UINT_32 CHAR_SPACE            = 0x00000040; // ' '
			static const XTL::UINT_32 CHAR_TAB              = 0x00000080; // '\t'
			static const XTL::UINT_32 CHAR_CARRIAGE_RETURN  = 0x00000100; // '\r'
			static const XTL::UINT_32 CHAR_LINE_FEED        = 0x00000200; // '\n'
			static const XTL::UINT_32 CHAR_PLUS             = 0x00000400; // '+'
			static const XTL::UINT_32 CHAR_MINUS            = 0x00000800; // '-'
			static const XTL::UINT_32 CHAR_UNDERSCORE       = 0x00001000; // '_'

			static const XTL::UINT_32 CHAR_LETTER           = CHAR_LETTER_UPPERCASE | CHAR_LETTER_LOWERCASE;

			class EndOfFile
			{
			};

		protected:

			explicit TextParser(CharSource & charSource);

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
			void SkipCharClass(const CharClass & charClass)
			{
				while (NotAtEnd())
				{
					if (!charClass.Contains(GetChar()))
					{
						return;
					}

					Advance();
				}

				throw EndOfFile();
			}

		private:

			CharSource & charSource_;





			static const CharClass DEFAULT_IDENTIFIER_HEAD;
			static const CharClass DEFAULT_IDENTIFIER_TAIL;

			const std::string ReadIdentifier(const CharClass & headClass = DEFAULT_IDENTIFIER_HEAD, const CharClass & tailClass = DEFAULT_IDENTIFIER_TAIL)
			{
				if (IsInClass(headClass))
				{
					Mark();
					Advance();
					SkipCharClass(tailClass);

					return ReleaseString();
				}

				return std::string();
			}

		protected:

			class BasicCharClassifier : public CharClassifier
			{
				public:

					static const BasicCharClassifier & Instance()
					{
						static BasicCharClassifier instance;

						return instance;
					}

				protected:

					BasicCharClassifier()
						: CharClassifier()
					{
						Add(CHAR_NULL,             '\0');
						Add(CHAR_DIGIT,            '0', '9');
						Add(CHAR_LETTER_UPPERCASE, 'A', 'Z');
						Add(CHAR_LETTER_LOWERCASE, 'a', 'z');
						Add(CHAR_HEX_UPPERCASE,    'A', 'F');
						Add(CHAR_HEX_LOWERCASE,    'a', 'f');
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

