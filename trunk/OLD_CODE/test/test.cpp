#include "xtl/TcpClient.hpp"
#include "xtl/ByteBuffer.hpp"
#include <netdb.h>

class NameResolveException
{
};

class IpAddress
{
	public:

		IpAddress()
		{
			address_.s_addr = INADDR_NONE;
		}

		IpAddress(const char * host)
		{
			Initialize(host);
		}

		static IpAddress None()
		{
			return IpAddress((unsigned long int) INADDR_NONE);
		}

		static IpAddress Any()
		{
			return IpAddress((unsigned long int) INADDR_ANY);
		}

		const std::string AsString() const
		{
			char buffer[INET_ADDRSTRLEN];
			return ::inet_ntop(AF_INET, &(address_.s_addr), buffer, sizeof(buffer)) == NULL
			       ? std::string()
			       : std::string(buffer);
		}

	protected:

		explicit IpAddress(unsigned long int address)
		{
			address_.s_addr = address;
		}

		void Initialize(const char * host)
		{
			/*
#if XTL_USE_GETADDRINFO
			struct ::addrinfo hints;
			struct ::addrinfo * result;

			::memset(&hints, 0, sizeof(hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = 0;
			hints.ai_protocol = 0;
			hints.ai_flags = 0;

			const char * portString[16];
			::sprintf(portString, "%d", port);

			int error = ::getaddrinfo(host, portString, &hints, &result);
			if (error != 0)
			{
				throw SocketAddressError(error, std::string("getaddrinfo(): ") + ::gai_strerror(error));
			}

			for (struct ::addrinfo * current = result; current != NULL; current = current->ai_next)
			{
				if (current->ai_family == AF_INET)
				{
					::memcpy(&address_, current->ai_addr, current->ai_addrlen);
					::freeaddrinfo(result);
					return;
				}
			}
			::freeaddrinfo(result);

			throw SocketAddressError("Host not found");
#else
			*/

			struct ::hostent * hostEntry = NULL;
#ifdef XTL_USE_GETHOSTBYNAME_R
			struct ::hostent hostData;
			char buffer[2048];
			int error = 0;
			if (::gethostbyname_r(host, &hostData, buffer, sizeof(buffer), &hostEntry, &error) != 0)
			{
				// throw SocketAddressError(error, std::string("gethostbyname_r(): ") + ::hstrerror(error));
				throw NameResolveException();
			}
#else
			hostEntry = ::gethostbyname(host);
			if (hostEntry == NULL || hostEntry->h_addrtype != AF_INET)
			{
				// throw SocketAddressError(h_errno, std::string("gethostbyname(): ") + ::hstrerror(h_errno));
				throw NameResolveException();
			}
#endif
			::memcpy(&address_, hostEntry->h_addr_list[0], hostEntry->h_length);

/*
			for (int i = 0; hostEntry->h_addr_list[i] != NULL; ++i)
			{
				printf("%08x\n", *((unsigned int *) (hostEntry->h_addr_list[i])));
			}
*/
//#endif
		}

		struct ::in_addr address_;
};


#include <xtl/BitSet.hpp>
#include <xtl/BitUtils.hpp>
#include <xtl/Types.hpp>

namespace XTL
{
	template <typename _KeyType, typename _ValueType>
	class StaticArrayMap
	{
		static const unsigned int CAPACITY = (1 << (sizeof(_KeyType) << 3));

		public:

			typedef _KeyType   KeyType;
			typedef _ValueType ValueType;

			static const unsigned int Capacity()
			{
				return CAPACITY;
			}

			const ValueType & Get(const KeyType & key) const
			{
				return values_[Index(key)];
			}

			ValueType & Get(const KeyType & key)
			{
				return values_[Index(key)];
			}

			const ValueType & operator[] (const KeyType & key) const
			{
				return Get(key);
			}

			ValueType & operator[] (const KeyType & key)
			{
				return Get(key);
			}

			StaticArrayMap & Init(const ValueType & value)
			{
				ValueType * end = values_ + Capacity();
				for (ValueType * p = values_; p < end; ++p)
				{
					*p = value;
				}
				return *this;
			}

			StaticArrayMap & Set(const KeyType & key, const ValueType & value)
			{
				values_[Index(key)] = value;
				return *this;
			}

			StaticArrayMap & Set(const KeyType & keyFrom, const KeyType & keyTo, const ValueType & value)
			{
				ValueType * end = values_ + Index(keyTo);
				for (ValueType * p = values_ + Index(keyFrom); p <= end; ++p)
				{
					*p = value;
				}
				return *this;
			}

		protected:

			static const unsigned int Index(const KeyType & key)
			{
				return static_cast<unsigned int>(
					static_cast<typename UnsignedInt<sizeof(KeyType)>::Type>(
						key
					)
				);
			}

			_ValueType values_[CAPACITY];
	};

	class DigitClassifier : protected StaticArrayMap<char, signed char>
	{
		typedef StaticArrayMap<char, signed char> Super;

		public:

			DigitClassifier(bool upperHex, bool lowerHex)
				: Super()
			{
				Init(-1);
				Set('0', 0); Set('1', 1); Set('2', 2); Set('3', 3);
				Set('4', 4); Set('5', 5); Set('6', 6); Set('7', 7);
				Set('8', 8); Set('9', 9);
				if (upperHex)
				{
					Set('A', 10); Set('B', 11); Set('C', 12);
					Set('D', 13); Set('E', 14); Set('F', 15);
				}
				if (lowerHex)
				{
					Set('a', 10); Set('b', 11); Set('c', 12);
					Set('d', 13); Set('e', 14); Set('f', 15);
				}
			}

			const int operator() (char ch) const
			{
				return Super::Get(ch);
			}

		protected:
	};

	DigitClassifier & DecDigitClassifier()
	{
		static DigitClassifier instance(false, false);
		return instance;
	}

	DigitClassifier & HexDigitClassifier()
	{
		static DigitClassifier instance(true, true);
		return instance;
	}
}

class HexCharClassifier
{
	public:

		static const HexCharClassifier & Instance()
		{
			static HexCharClassifier instance;
			return instance;
		}

		const int Get(char ch) const
		{
			return map_[ch];
		}

	protected:

		HexCharClassifier()
		{
			map_.Init(-1);

			for (char ch = '0'; ch <= '9'; ++ch)
			{
				map_.Set(ch, ch - '0');
			}
			for (char ch = 'a'; ch <= 'f'; ++ch)
			{
				map_.Set(ch, ch - 'a' + 10);
			}
			for (char ch = 'A'; ch <= 'F'; ++ch)
			{
				map_.Set(ch, ch - 'A' + 10);
			}
		};

		XTL::StaticArrayMap<char, signed char> map_;
};

namespace XTL
{
	class CharFilter
	{
		public:

			CharFilter() : set_() { ;; }

			CharFilter & Add(char ch)
			{
				set_.Set(static_cast<unsigned char>(ch));
				return *this;
			}

			CharFilter & Remove(char ch)
			{
				set_.Clear(static_cast<unsigned char>(ch));
				return *this;
			}

			const bool operator() (char ch) const
			{
				return set_.Get(ch);
			}

		protected:

			StaticBitSet<32> set_;
	};

	class CharClassifier
	{
	};
	
}

class CharClassifier
{
	public:
/*
		'\x04' - CTRL_D
		'\0x0c' - FORM_FEED
		'\x1a' - CTRL_Z
*/
/*
#$&
=@\^`|~

BRACKETS    ()[]<>{}
UNDERSCORE  _
QUOTE       '"
PUNCTUATION .,:;?! QUOTE
ARITHMETIC  +-*%/
*/

		enum CharClass
		{
			CHAR_UNDEFINED       = 0x0000,
			CHAR_NULL            = 0x0000, // '\x00' == '\0'
			CHAR_BELL            = 0x0000, // '\x07' == '\a'
			CHAR_BACKSPACE       = 0x0000, // '\x08' == '\b'
			CHAR_TAB             = 0x0000, // '\x09' == '\t'
			CHAR_LINE_FEED       = 0x0000, // '\x0a' == '\n'
			CHAR_VTAB            = 0x0000, // '\x0b' == '\v'
			CHAR_CARRIAGE_RETURN = 0x0000, // '\x0d' == '\r'
			CHAR_ESCAPE          = 0x0000, // '\x1b'
			CHAR_SPACE           = 0x0000, // '\x20' == ' '

//"#$%&'(),./:;<=>@[\]^_`{|}~

			CHAR_DIGIT           = 0x0000, // '0'..'9'
			CHAR_ALPHA_UPPER     = 0x0000, // 'A'...'Z'
			CHAR_ALPHA_LOWER     = 0x0000, // 'a'...'z'
/*
CHAR_QUESTION // '?'
CHAR_EXCLAMATION // '!'
CHAR_MINUS // '-'
CHAR_PLUS // '+'
CHAR_STAR // '*'
*/
			CHAR_NEWLINE         = 0x0020,
			CHAR_HEX_LOWER       = 0x0000, // 'a'...'f'
			CHAR_HEX_UPPER       = 0x0000, // 'A'...'F'
			CHAR_UNDERSCORE      = 0x0000, // '_'
			CHAR_BRACKET         = 0x0000, // '(', ')', '[', ']', '<', '>', '{', '}'

			CHAR_HEX_DIGIT       = CHAR_DIGIT | CHAR_HEX_LOWER | CHAR_HEX_UPPER,
			CHAR_ALPHA           = CHAR_ALPHA_LOWER | CHAR_ALPHA_UPPER,
			CHAR_BLANK           = CHAR_SPACE | CHAR_TAB | CHAR_VTAB |
			                       CHAR_CARRIAGE_RETURN | CHAR_LINE_FEED
		};

		enum CharSubclass
		{
			CHAR_0 = 0x0000,
			CHAR_1 = 0x0001,
			CHAR_2 = 0x0002,
			CHAR_3 = 0x0003,
			CHAR_4 = 0x0004,
			CHAR_5 = 0x0005,
			CHAR_6 = 0x0006,
			CHAR_7 = 0x0007,
			CHAR_8 = 0x0008,
			CHAR_9 = 0x0009,
			CHAR_A = 0x0010,
			CHAR_B = 0x0011,
			CHAR_C = 0x0012,
			CHAR_D = 0x0013,
			CHAR_E = 0x0014,
			CHAR_F = 0x0015
		};

		CharClassifier()
		{
			/*
			map_.Set('\0', CHAR_EOF);
			map_.Set('0', '9', CHAR_DIGIT);
			map_.Set('a', 'z', CHAR_ALPHA_LOWER);
			map_.Set('A', 'Z', CHAR_ALPHA_UPPER);

			map_.Set('_', CHAR_UNDERSCORE);
			map_.Set(' ', CHAR_SPACE);
			map_.Set('\t', CHAR_SPACE);
			map_.Set('\v', CHAR_SPACE);
			map_.Set('\r', CHAR_SPACE);
			map_.Set('\n', CHAR_SPACE);

			map_.Set('\n', CHAR_NEWLINE);

			map_.Set('a', 'f', CHAR_HEX_LOWER);
			map_.Set('A', 'F', CHAR_HEX_UPPER);
			*/
		}

		XTL::StaticArrayMap<char, unsigned int> map_;
};


class CharSource
{
	public:

		typedef char CharType;

		CharSource(const void * ptr)
			: ptr_(static_cast<const char *>(ptr))
		{
			;;
		}

		void Advance()
		{
			++ptr_;
		}

		unsigned int Read()
		{
			if (*ptr_ == '\0')
			{
				return false;
			}
			Advance();
		}

		unsigned int CharClass() const
		{
			return 0;
		}

		const char Char() const
		{
			return *ptr_;
		}

	protected:

		const char * ptr_;
//		CharClassMap
};

namespace XTL
{
	class NullTerminatedCharSource
	{
		public:

			NullTerminatedCharSource(const void * ptr)
				: ptr_(static_cast<const char *>(ptr)) { ;; }

			const bool AtEnd() const
			{
				return *ptr_ == '\0';
			}

			const bool NotAtEnd() const
			{
				return *ptr_ != '\0';
			}

			void Advance()
			{
				++ptr_;
			}

			const char Char() const
			{
				return *ptr_;
			}

			const bool Skip(char ch)
			{
				if (AtEnd())
				{
					return false;
				}

				if (Char() == ch)
				{
					Advance();
					return true;
				}

				return false;
			}

		protected:

			const char * ptr_;
	};

	class BoundedCharSource : protected NullTerminatedCharSource
	{
		typedef NullTerminatedCharSource Super;

		public:

			BoundedCharSource(const void * begin, unsigned int length)
				: Super(begin),
				  end_(static_cast<const char *>(begin) + length)
			{
				;;
			}

			BoundedCharSource(const void * begin, const void * end)
				: Super(begin),
				  end_(static_cast<const char *>(end))
			{
				;;
			}

			const bool AtEnd() const
			{
				return ptr_ >= end_;
			}

			const bool NotAtEnd() const
			{
				return ptr_ < end_;
			}

			using Super::Advance;
			using Super::Char;
			using Super::Skip;

		protected:

			const char * end_;
	};

	class TextPosition
	{
		public:

			TextPosition()
				: row_(0), column_(0) { ;; }

			TextPosition(unsigned int row)
				: row_(row), column_(0) { ;; }

			TextPosition(unsigned int row, unsigned int column)
				: row_(row), column_(column) { ;; }

			const unsigned int Row() const
			{
				return row_;
			}

			const unsigned int Column() const
			{
				return column_;
			}

			void NextRow()
			{
				++row_;
				column_ = 0;
			}

			void NextColumn()
			{
				++column_;
			}

		protected:

			unsigned int row_;
			unsigned int column_;
	};

	template <typename _CharSource>
	class TextSource
	{
		public:

			typedef _CharSource CharSource;

			TextSource(const CharSource & charSource)
				: charSource_(charSource), position_()
			{
				;;
			}

			TextSource(const CharSource & charSource, const TextPosition & position)
				: charSource_(charSource), position_(position)
			{
				;;
			}

			const bool AtEnd() const
			{
				return charSource_.AtEnd();
			}

			const bool NotAtEnd() const
			{
				return charSource_.NotAtEnd();
			}

			void Advance()
			{
				// TODO: may be add IsNewLine() method to char source
				if (Char() == '\n')
				{
					position_.NextRow();
				}
				else
				{
					position_.NextColumn();
				}

				charSource_.Advance();
			}

			const char Char() const
			{
				return charSource_.Char();
			}

			const CharSource & Source() const
			{
				return charSource_;
			}

			const TextPosition & Position() const
			{
				return position_;
			}

			const unsigned int Row() const
			{
				return position_.Row();
			}

			const unsigned int Column() const
			{
				return position_.Column();
			}

			const bool Skip(char ch)
			{
				if (AtEnd())
				{
					return false;
				}

				if (Char() == ch)
				{
					Advance();
					return true;
				}

				return false;
			}

		protected:

			CharSource   charSource_;
			TextPosition position_;
	};

	class NullTerminatedTextSource : public TextSource<NullTerminatedCharSource>
	{
		typedef TextSource<NullTerminatedCharSource> Super;

		public:

			NullTerminatedTextSource(const void * ptr)
				: Super(NullTerminatedCharSource(ptr))
			{
				;;
			}

			NullTerminatedTextSource(const void * ptr, const TextPosition & position)
				: Super(NullTerminatedCharSource(ptr), position)
			{
				;;
			}
	};
}

#include <sys/time.h>

template <typename _CharSource, typename _CharFilter>
bool GobbleChars(_CharSource & charSource, _CharFilter filter, unsigned int & count)
{
	for (; charSource.NotAtEnd(); charSource.Advance())
	{
		if (!filter(charSource.Char()))
		{
			return true;
		}
		++count;
	}
	return false;
}

class CharGobbler
{
	public:

		CharGobbler()
			: count_(0) { ;; }

		template <typename _CharSource, typename _CharFilter>
		bool operator() (_CharSource & charSource, _CharFilter filter)
		{
			count_ = 0;

			return GobbleChars(charSource, filter, count_);
		}

		const unsigned int Count() const
		{
			return count_;
		}

	protected:

		unsigned int count_;
};

namespace XTL
{
	class DecimalClassifier : protected StaticArrayMap<char, signed char>
	{
		typedef StaticArrayMap<char, signed char> Super;

		public:

			DecimalClassifier()
				: Super()
			{
				Init(-1);
				Set('0', 0); Set('1', 1); Set('2', 2); Set('3', 3);
				Set('4', 4); Set('5', 5); Set('6', 6); Set('7', 7);
				Set('8', 8); Set('9', 9);
			}

			const int operator() (char ch) const
			{
				return Get(ch);
			}

			static DecimalClassifier & Instance()
			{
				static DecimalClassifier instance;
				return instance;
			}
	};

	class HexadecimalClassifier : protected DecimalClassifier
	{
		public:

			HexadecimalClassifier()
				: DecimalClassifier()
			{
				Set('A', 10); Set('a', 10);
				Set('B', 11); Set('b', 11);
				Set('C', 12); Set('c', 12);
				Set('D', 13); Set('d', 13);
				Set('E', 14); Set('e', 14);
				Set('F', 15); Set('f', 15);
			}
			
			const int operator() (char ch) const
			{
				return Get(ch);
			}
	};

	class UnsignedIntegerParser
	{
		public:

			UnsignedIntegerParser()
				: value_(0) { ;; }

			template <typename _CharSource>
			bool operator() (_CharSource & charSource)
			{
				value_ = 0;

				for (; charSource.NotAtEnd(); charSource.Advance())
				{
					int digit = DecimalClassifier::Instance()(charSource.Char());
					if (digit < 0)
					{
						return true;
					}

					value_ = (value_ << 3) + (value_ << 1) + digit;
				}

				return false;
			}

			const unsigned long long int Value() const
			{
				return value_;
			}

		protected:

			unsigned long long int value_;
	};

	class IntegerParser
	{
		public:

			IntegerParser()
				: value_(0) { ;; }

			template <typename _CharSource>
			bool operator() (_CharSource & charSource)
			{
				value_ = 0;

				bool negative = false;

				if (charSource.Skip('-'))
				{
					negative = true;
				}
				else
				{
					charSource.Skip('+');
				}

				UnsignedIntegerParser parser;
				bool result = parser(charSource);

				value_ = (negative ? -parser.Value() : parser.Value());

				return result;
			}

		protected:

			signed long long int value_;
	};
}

int main(int argc, const char * argv[])
{
	const char * const s = "123456\nasd\n\nqwer";

	XTL::NullTerminatedTextSource src(s);

	for (; src.NotAtEnd(); src.Advance())
	{
		fprintf(stderr, "%02x (%u, %u)\n", src.Char(), src.Row(), src.Column());
	}

	return 0;

	timeval t;

	char buffer[1000000];

	gettimeofday(&t, NULL);
	double value = (double) t.tv_sec + (double) t.tv_usec / 1000000.0;

	for (int i = 0; i < 10000; ++i)
	{
		memset(buffer, '\xff', sizeof(buffer));
		//SetChars(buffer, '\xff', sizeof(buffer));
	}

	gettimeofday(&t, NULL);
	value = (double) t.tv_sec + (double) t.tv_usec / 1000000.0 - value;

	fprintf(stderr, "%0.3f\n", value);

	return 0;

	for (int i = 0; i < 256; ++i)
	{
		fprintf(stderr, "%d ", HexCharClassifier::Instance().Get(i));
		if ((i + 1) % 16 == 0)
		{
			fprintf(stderr, "\n");
		}
	}

/*
	int i = 0;
	for (unsigned char c = '\x20'; c <= '\x7f'; ++c)
	{
		printf("%c", c);
		if (++i % 16 == 0)
		{
			printf("\n");
		}
	}
*/
	return 0;

	XTL::ByteBuffer bb(600000);
	bb << 1;
	XTL::ByteBuffer a = bb;
	return 0;

	const char * remoteHost = "www.microsoft.com";
	int remotePort = 80;

	XTL::TcpClient client;

	if (!client.Connect(remoteHost, remotePort))
	{
		fprintf(stderr, "Unable to connect to %s:%d\n", remoteHost, remotePort);
		return 1;
	}

	fprintf(stdout, "Status: %d\n", client.Status());

	return 0;

	fprintf(stderr, "Connected to %s:%d\n", remoteHost, remotePort);

	client.Push(
		"GET / HTTP/1.1\r\n"
		"Host: www.yandex.ru\r\n"
		"User-Agent: Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5\r\n"
		"Accept: text/html\r\n"
		"Connection: close\r\n\r\n"
	);

	while (client.HasSendingData())
	{
		if (!client.DoSend())
		{
			fprintf(stderr, "Error, while sending request\n");
			return 1;
		}
	}

	while (client.DoReceive())
	{
		;;
	}

	printf("%s\n", client.ReceivedData());

	client.Close();

	return 0;

//	TcpSocket cs;

//	int result = cs.connect(host.c_str(), port, true);
//	printf("connect = %d\n", result);

	/*
	const char * const q =
	"GET / HTTP/1.1\r\n"
	"Host: yandex.ru\r\n"
	"User-Agent: Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5\r\n"
	"Accept: text/html\r\n"
	"Connection: close\r\n\r\n";

	printf("sending\n");
	cs.sendAll(q, strlen(q));
	printf("ok!\n");

	char buffer[4096];
	int received;

	while (cs.opened())
	{
		received = cs.recv(buffer, sizeof(buffer) - 1);
		if (received > 0)
		{
			buffer[received] = '\0';
			printf("received: %d\n", received);
		}
	}
	*/

//	cs.close();
}

