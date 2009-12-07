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

template <typename _KeyType,
          typename _ValueType,
          unsigned int CAPACITY = (1 << (sizeof(_KeyType) << 3))>
class ArrayMap
{
	public:

		typedef _KeyType   KeyType;
		typedef _ValueType ValueType;

		ValueType & Get(const KeyType & key)
		{
			return array_[AsUint(key)];
		}

		const ValueType & Get(const KeyType & key) const
		{
			return array_[AsUint(key)];
		}

		ValueType & operator[] (const KeyType & key)
		{
			return Get(key);
		}

		const ValueType & operator[] (const KeyType & key) const
		{
			return Get(key);
		}

		ArrayMap & Set(const KeyType & key, const ValueType & value)
		{
			array_[AsUint(key)] = value;
			return *this;
		}

		ArrayMap & Set(const KeyType & keyFrom, const KeyType & keyTo, const ValueType & value)
		{
			ValueType * end = array_ + AsUint(keyTo);
			for (ValueType * p = array_ + AsUint(keyFrom); p <= end; ++p)
			{
				*p = value;
			}
			return *this;
		}

		ArrayMap & SetAll(const ValueType & value)
		{
			ValueType * end = array_ + CAPACITY;
			for (ValueType * p = array_; p < end; ++p)
			{
				*p = value;
			}
			return *this;
		}

	protected:

		static unsigned int AsUint(const KeyType & key)
		{
			return static_cast<unsigned int>(
				static_cast<typename UnsignedInt<sizeof(key)>::Type>(
					key
				)
			);
		}

		ValueType array_[CAPACITY];
};

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
			map_.SetAll(-1);

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

		ArrayMap<char, signed char> map_;
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
			}

			CharFilter & Remove(char ch)
			{
				set_.Clear(static_cast<unsigned char>(ch));
			}

			

		protected:

			StaticBitSet<32> set_;
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

		ArrayMap<char, unsigned int> map_;
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

class IntegerToken
{
	static bool Digit(char ch)
	{
		return ch >= '0' && ch <= '9';
	}

	public:

		IntegerToken()
			: value_(0) { ;; }

		template <typename _CharSource>
		void Read(_CharSource & source)
		{
			value_ = 0;
			bool negative = false;
			if (source.Read('-'))
			{
				negative = true;
			}
			else
			{
				while (source.Read(Digit))
				{
					value_ = (value_ << 3) + (value_ << 1) + (source.Char() - '0');
				}
			}

		}

		const long long int & Value() const
		{
			return value_;
		}

	protected:

		long long int value_;
};

class Lexer
{
	public:

		enum
		{
			TOKEN_EOF  = 0x0000,
			TOKEN_BOF  = 0x0001,
			TOKEN_CHAR = 0x0002
		};
};

#include <sys/time.h>

int main(int argc, const char * argv[])
{
	XTL::StaticBitSet<16> bs;

	for (unsigned int i = 0; i < 64; ++i)
	{
		bs.Invert(i, 127 - i);
	}

	for (unsigned int i = 0; i < bs.Capacity(); ++i)
	{
		fprintf(stderr, "%d ", bs.Get(i) ? 1 : 0);
	}

	fprintf(stderr, "\n");

	return 0;

	timeval t;

	gettimeofday(&t, NULL);
	double value = (double) t.tv_sec + (double) t.tv_usec / 1000000.0;

	gettimeofday(&t, NULL);
	value = (double) t.tv_sec + (double) t.tv_usec / 1000000.0 - value;
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

