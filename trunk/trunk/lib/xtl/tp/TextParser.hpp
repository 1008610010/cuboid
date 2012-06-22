#ifndef XTL__TEXT_PARSER_HPP__
#define XTL__TEXT_PARSER_HPP__ 1

#include "../Types.hpp"
#include "TextCharSource.hpp"

/*
    NUL SOH STX ETX EOT ENQ ACK BEL BS  TAB LF  VT  FF  CR  SO  SI
    DLE DC1 DC2 DC3 DC4 NAK SYN ETB CAN EM  SUB ESC FS  GS  RS  US
        !   "   #   $   %   &   '   (   )   *   +   ,   —   .   /
    0   1   2   3   4   5   6   7   8   9   :   ;   <   =   >   ?
    @   A   B   C   D   E   F   G   H   I   J   K   L   M   N   O
    P   Q   R   S   T   U   V   W   X   Y   Z   [   \   ]   ^   _
    `   a   b   c   d   e   f   g   h   i   j   k   l   m   n   o
    p   q   r   s   t   u   v   w   x   y   z   {   |   }   ~   DEL
*/

namespace XTL
{
	class TextParser
	{
		public:

			static const XTL::UINT_32 CHAR_DIGIT            = 0x00000001; // '0' .. '9'
			static const XTL::UINT_32 CHAR_LETTER_UPPERCASE = 0x00000002; // 'A' .. 'Z'
			static const XTL::UINT_32 CHAR_LETTER_LOWERCASE = 0x00000004; // 'a' .. 'z'
			static const XTL::UINT_32 CHAR_HEX_UPPERCASE    = 0x00000008; // 'A' .. 'F'
			static const XTL::UINT_32 CHAR_HEX_LOWERCASE    = 0x00000010; // 'a' .. 'f'
			static const XTL::UINT_32 CHAR_SPACE            = 0x00000020; // ' '
			static const XTL::UINT_32 CHAR_TAB              = 0x00000040; // '\t'
			static const XTL::UINT_32 CHAR_CARRIAGE_RETURN  = 0x00000080; // '\r'
			static const XTL::UINT_32 CHAR_LINE_FEED        = 0x00000100; // '\n'
			static const XTL::UINT_32 CHAR_PLUS             = 0x00000200; // '+'
			static const XTL::UINT_32 CHAR_MINUS            = 0x00000400; // '-'
			static const XTL::UINT_32 CHAR_UNDERSCORE       = 0x00000800; // '_'

			static const XTL::UINT_32 CHAR_LETTER           = CHAR_LETTER_UPPERCASE | CHAR_LETTER_LOWERCASE;

			explicit TextParser(TextCharSource & charSource);

			bool NotAtEnd() const              { return charSource_.NotAtEnd(); }

			bool AtEnd() const                 { return charSource_.AtEnd(); }

			char GetChar() const               { return charSource_.GetChar(); }

			void Advance()                     { charSource_.Advance(); }

			TextCursor GetCursor() const       { return charSource_.GetCursor(); }

			void Mark()                        { charSource_.Mark(); }

			void Unmark()                      { charSource_.Unmark(); }

			const std::string ReleaseString()  { return charSource_.ReleaseString(); }


			bool IsInClass(const CharClass & charClass) const
			{
				return charClass.Contains(GetChar());
			}

			bool WaitChar(char c)
			{
				while (NotAtEnd())
				{
					if (GetChar() == c)
					{
						return true;
					}

					Advance();
				}

				return false;
			}

			bool ReadChar(char c)
			{
				if (GetChar() == c)
				{
					Advance();
					return true;
				}
				else
				{
					return false;
				}
			}

			void SkipChars(char c)
			{
				while (NotAtEnd() && GetChar() == c)
				{
					Advance();
				}
			}

			void SkipCharClass(const CharClass & charClass)
			{
				while (NotAtEnd() && IsInClass(charClass))
				{
					Advance();
				}
			}

			static const CharClass DEFAULT_IDENTIFIER_HEAD;

			static const CharClass DEFAULT_IDENTIFIER_TAIL;

			const std::string ReadIdentifier(const CharClass & headClass = DEFAULT_IDENTIFIER_HEAD, const CharClass & tailClass = DEFAULT_IDENTIFIER_TAIL);

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

					CommonCharClassifier();
			};

		private:

			TextCharSource & charSource_;
	};
}

#endif

