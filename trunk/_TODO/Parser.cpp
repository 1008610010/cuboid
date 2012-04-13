#include "Parser.hpp"
#include "StringUtils.hpp"

namespace XTL
{
	Parser::Parser(const char * source)
		: cur_(source),
		  row_(0),
		  column_(0)
	{
		;;
	}

	// IsAlpha(*cur)
	void Parser::ParseIdentifier(std::string & s)
	{
		const char * begin = cur_;
		Advance();
		for (; IsAlpha(*cur_) || IsDigit(*cur_); Advance()) { ;; }
		s.assign(begin, cur_ - begin);
	}

	// IsDigit(*cur_) || cur_ == '-'
	void Parser::ParseIntegerLiteral(long long & i)
	{
		i = 0;
		bool neg = false;

		if (*cur_ == '-')
		{
			neg = true;
			Advance();
			if (!IsDigit(*cur_))
			{
				RaiseError("Decimal digit expected");
			}
		}

		for (; IsDigit(*cur_); Advance())
		{
			i *= 10;
			i += (*cur_ - '0');
		}

		if (neg)
		{
			i = -i;
		}
	}

	// IsDigit(*cur_) || cur_ == ''
	bool Parser::ParseNumberLiteral(long long & i, double & f)
	{
		bool integer = true;

		ParseIntegerLiteral(i);

		if (*cur_ == '.')
		{
			Advance();

			integer = false;
			f = i;

			double d = 1.0;
			if (i >= 0)
			{
				for (; IsDigit(*cur_); Advance())
				{
					d /= 10.0;
					f += (*cur_ - '0') * d;
				}
			}
			else
			{
				for (; IsDigit(*cur_); Advance())
				{
					d /= 10.0;
					f -= (*cur_ - '0') * d;
				}
			}
		}

		if (*cur_ == 'e' || *cur_ == 'E')
		{
			Advance();

			long long exponent;
			ParseIntegerLiteral(exponent);

			if (integer && exponent > 0)
			{
				for (; exponent > 0; --exponent, i *= 10) { ;; }
			}
			else
			{
				if (integer)
				{
					integer = false;
					f = i;
				}

				if (exponent > 0)
				{
					for (; exponent > 0; --exponent, f *= 10.0) { ;; }
				}
				else
				{
					for (; exponent < 0; ++exponent, f /= 10.0) { ;; }
				}
			}
		}

		return integer;
	}

	/*
		*cur_ == '"'
	*/
	void Parser::ParseStringLiteral(std::string & s)
	{
		Advance();
		const char * begin = cur_;
		s.clear();

		while (true)
		{
			if (*cur_ == '\0')
			{
				RaiseError("Unexpected end of file in double quoted string");
			}
			else if (*cur_ == '\n')
			{
				RaiseError("Multiline string literal");
			}
			else if (*cur_ == '"')
			{
				if (begin < cur_)
				{
					s.append(begin, cur_ - begin);
					begin = cur_;
				}
				Advance();
				return;
			}
			else if (*cur_ == '\\')
			{
				if (begin < cur_)
				{
					s.append(begin, cur_ - begin);
				}
				Advance();
				switch (*cur_)
				{
					case '\0':
						RaiseError("Unexpected end of file in double quoted string");
					case '\\':
					case '/':
					case '"':
						s.append(1, *cur_); Advance(); begin = cur_; break;
					case 'b':
						s.append(1, '\b'); Advance(); begin = cur_; break;
					case 'f':
						s.append(1, '\f'); Advance(); begin = cur_; break;
					case 'n':
						s.append(1, '\n'); Advance(); begin = cur_; break;
					case 'r':
						s.append(1, '\r'); Advance(); begin = cur_; break;
					case 't':
						s.append(1, '\t'); Advance(); begin = cur_; break;
					default:
						// TODO: \uXXXX - char in hexademical digit

						RaiseError(FormatString("Unknown escape sequence: \\%c", *cur_));
				}
			}
			else
			{
				Advance();
			}
		}
	}
}
