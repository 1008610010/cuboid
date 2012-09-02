#ifndef XTL__STRING_LITERAL_PARSER_HPP__
#define XTL__STRING_LITERAL_PARSER_HPP__ 1

#include "../CharBuffer.hpp"
#include "../utils/AutoPtrMap.hpp"
#include "Parser.hpp"

namespace XTL
{
	class StringLiteralParser : protected Parser
	{
		public:

			StringLiteralParser(CharSource & charSource, char boundingChar, char escapeSequenceChar)
				: Parser(charSource),
				  boundingChar_(boundingChar),
				  escapeSequenceChar_(escapeSequenceChar)
			{
				;;
			}

			virtual ~StringLiteralParser() throw()
			{
				;;
			}

			const std::string Parse();

			virtual void ParseEscapeSequence(CharBuffer & buffer) = 0;

			class EscapeSequence
			{
				public:

					virtual ~EscapeSequence() throw() { ;; }

					virtual void Parse(CharSource & charSource, CharBuffer & result) = 0;

					class Char;
			};

			template <class Subclass>
			class EscapeSequenceSet
			{
				public:

					static const EscapeSequenceSet & Instance()
					{
						static Subclass instance;
						return instance;
					}

					void Parse(CharSource & charSource, CharBuffer & result) const
					{
						EscapeSequence * seq = sequences_[charSource.GetChar()];
						if (seq == 0)
						{
							throw Parser::Error(charSource.GetCursor(), "Invalid escape sequence");
						}

						seq->Parse(charSource, result);
					}

				protected:

					EscapeSequenceSet()
						: sequences_()
					{
						;;
					}

					void Add(char c, std::auto_ptr<EscapeSequence> seq);

					void Add(char from, char to);

				private:

					AutoPtrMap<char, EscapeSequence> sequences_;
			};

		private:

			const char boundingChar_;
			const char escapeSequenceChar_;
	};

	class StringLiteralParser::EscapeSequence::Char : public StringLiteralParser::EscapeSequence
	{
		public:

			explicit Char(char outputChar)
				: outputChar_(outputChar) { ;; }

			virtual ~Char() throw() { ;; }

			virtual void Parse(CharSource & charSource, CharBuffer & result)
			{
				result.Append(outputChar_);
				charSource.Advance();
			}

		private:

			char outputChar_;
	};

	template <class Subclass>
	void StringLiteralParser::EscapeSequenceSet<Subclass>::Add(char c, std::auto_ptr<EscapeSequence> seq)
	{
		sequences_.Set(c, seq);
	}

	template <class Subclass>
	void StringLiteralParser::EscapeSequenceSet<Subclass>::Add(char from, char to)
	{
		Add(from, std::auto_ptr<EscapeSequence>(new EscapeSequence::Char(to)));
	}
}

#endif
