#include "Base64Test.hpp"

#include <string>

#include <xtl/CharBuffer.hpp>
#include <xtl/FormatString.hpp>
#include <xtl/utils/Base64.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION (Base64Test);

namespace
{
	void FillSetByString(std::set<char> & result, const char * s)
	{
		for (const char * p = s; *p != '\0'; ++p)
		{
			result.insert(*p);
		}
	}
}

namespace
{
	const char * const TEST_STRINGS[] = {
		"",
		"\0",
		"12",
		"xyz",
		"\xfc",
		"\xfb",
		"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c\x7d\x7e\x7f\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8d\x8e\x8f\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9a\x9b\x9c\x9d\x9e\x9f\xa0\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\xaf\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8\xc9\xca\xcb\xcc\xcd\xce\xcf\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8\xd9\xda\xdb\xdc\xdd\xde\xdf\xe0\xe1\xe2\xe3\xe4\xe5\xe6\xe7\xe8\xe9\xea\xeb\xec\xed\xee\xef\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff",
		"Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure."
	};

	struct TwoStrings
	{
		const char * source;
		const char * encoded;
	};

	const TwoStrings TEST_PAIRS[] = {
		{
			"Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.",
			"TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlzIHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGludWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRoZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4="
		},
		{
			"So?<p>This 4, 5, 6, 7, 8, 9, z, {, |, } tests Base64 encoder. Show me: @, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, [, \\, ], ^, _, `, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s.",
			"U28/PHA+VGhpcyA0LCA1LCA2LCA3LCA4LCA5LCB6LCB7LCB8LCB9IHRlc3RzIEJhc2U2NCBlbmNvZGVyLiBTaG93IG1lOiBALCBBLCBCLCBDLCBELCBFLCBGLCBHLCBILCBJLCBKLCBLLCBMLCBNLCBOLCBPLCBQLCBRLCBSLCBTLCBULCBVLCBWLCBXLCBYLCBZLCBaLCBbLCBcLCBdLCBeLCBfLCBgLCBhLCBiLCBjLCBkLCBlLCBmLCBnLCBoLCBpLCBqLCBrLCBsLCBtLCBuLCBvLCBwLCBxLCByLCBzLg=="
		}
	};
}

Base64Test::Base64Test()
	: allowedChars_(),
	  allowedCharsUrl_()
{
	;;
}

void Base64Test::setUp()
{
	FillSetByString(allowedChars_,    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=");
	FillSetByString(allowedCharsUrl_, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_=");
}

void Base64Test::Test1()
{
	for (unsigned int i = 0; i < sizeof(TEST_STRINGS) / sizeof(TEST_STRINGS[0]); ++i)
	{
		TestEncodeDecode(TEST_STRINGS[i]);
		TestEncodeDecodeUrl(TEST_STRINGS[i]);
	}

	for (unsigned int i = 0; i < sizeof(TEST_PAIRS) / sizeof(TEST_PAIRS[0]); ++i)
	{
		TestEncode(TEST_PAIRS[i].source, TEST_PAIRS[i].encoded);
		TestDecode(TEST_PAIRS[i].encoded, TEST_PAIRS[i].source);
	}

	try
	{
		XTL::CharBuffer bufferDecoded;
		XTL::Base64::Decode("1234567", bufferDecoded);
		CPPUNIT_FAIL("Successfully decoded invalid string");
	}
	catch (...) { ;; }

	try
	{
		XTL::CharBuffer bufferDecoded;
		XTL::Base64::Decode("1234#678", bufferDecoded);
		CPPUNIT_FAIL("Successfully decoded invalid string");
	}
	catch (...) { ;; }

	{
		const char * LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		std::string allCharsEncoded;
		for (unsigned i = 0; i < 64; ++i)
		{
			allCharsEncoded.append(1, LETTERS[i]);
		}

		XTL::CharBuffer allCharsDecoded;

		try
		{
			XTL::Base64::Decode(allCharsEncoded, allCharsDecoded);

			TestEncodeDecode(allCharsDecoded.ToString());
			TestEncodeDecodeUrl(allCharsDecoded.ToString());
		}
		catch (...)
		{
			CPPUNIT_FAIL("Exception in XTL::Base64::Decode");
		}
	}
}

namespace
{
	void TestAllowedChars(const std::string & encoded, const std::set<char> & allowed)
	{
		for (unsigned int i = 0; i < encoded.size(); ++i)
		{
			if (allowed.count(encoded[i]) == 0)
			{
				CPPUNIT_FAIL(XTL::FormatString("Unallowed char '%c' in encoded string", encoded[i]));
			}
		}
	}
}

void Base64Test::TestAllowedChars(const std::string & encoded) const
{
	::TestAllowedChars(encoded, allowedChars_);
}

void Base64Test::TestAllowedCharsUrl(const std::string & encoded) const
{
	::TestAllowedChars(encoded, allowedCharsUrl_);
}

void Base64Test::TestEncode(const std::string & source, const std::string & encoded) const
{
	XTL::CharBuffer bufferEncoded;

	XTL::Base64::Encode(source, bufferEncoded);
	TestAllowedChars(bufferEncoded.ToString());

	CPPUNIT_ASSERT_EQUAL(encoded, bufferEncoded.ToString());
}

void Base64Test::TestDecode(const std::string & encoded, const std::string & source) const
{
	TestAllowedChars(encoded);

	XTL::CharBuffer bufferDecoded;

	try
	{
		XTL::Base64::Decode(encoded, bufferDecoded);

		CPPUNIT_ASSERT_EQUAL(source, bufferDecoded.ToString());
	}
	catch (...)
	{
		CPPUNIT_FAIL("Exception in XTL::Base64::Decode");
	}
}

void Base64Test::TestEncodeDecode(const std::string & source) const
{
	XTL::CharBuffer bufferEncoded;
	XTL::CharBuffer bufferDecoded;

	XTL::Base64::Encode(source, bufferEncoded);
	TestAllowedChars(bufferEncoded.ToString());

	try
	{
		XTL::Base64::Decode(bufferEncoded.Data(), bufferEncoded.Size(), bufferDecoded);

		CPPUNIT_ASSERT_EQUAL(source, bufferDecoded.ToString());
	}
	catch (...)
	{
		CPPUNIT_FAIL("Exception in XTL::Base64::Decode");
	}
}

void Base64Test::TestEncodeDecodeUrl(const std::string & source) const
{
	XTL::CharBuffer bufferEncoded;
	XTL::CharBuffer bufferDecoded;

	XTL::Base64::EncodeUrl(source, bufferEncoded);
	TestAllowedCharsUrl(bufferEncoded.ToString());

	try
	{
		XTL::Base64::DecodeUrl(bufferEncoded.Data(), bufferEncoded.Size(), bufferDecoded);

		CPPUNIT_ASSERT_EQUAL(source, bufferDecoded.ToString());
	}
	catch (...)
	{
		CPPUNIT_FAIL("Exception in XTL::Base64::Decode");
	}
}
