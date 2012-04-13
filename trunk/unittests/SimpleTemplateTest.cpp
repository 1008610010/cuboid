#include "SimpleTemplateTest.hpp"

#include <string>

#include <xtl/FormatString.hpp>
#include <xtl/tp/SimpleTemplate.hpp>
#include <xtl/tp/SimpleTemplateItemVisitor.hpp>
#include <xtl/tp/SimpleTemplateParameter.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION (SimpleTemplateTest);

namespace
{
	class TemplateSignatureBuilder : public XTL::SimpleTemplateItemVisitor
	{
		public:

			TemplateSignatureBuilder()
				: sign_() { ;; }

			virtual ~TemplateSignatureBuilder() throw() { ;; }

			virtual void Visit(const XTL::SimpleTemplateItemText & item)
			{
				sign_.append(XTL::FormatString("T%u", item.Size()));
			}

			virtual void Visit(const XTL::SimpleTemplateItemParameter & item)
			{
				sign_.append(XTL::FormatString("P%u", item.GetParameter().GetIndex()));
			}

			const std::string GetValue() const
			{
				return sign_;
			}

		private:

			std::string sign_;
	};

	const std::string GetTemplateSignature(const char * source)
	{
		const XTL::SimpleTemplate tmpl(source);
		TemplateSignatureBuilder builder;
		tmpl.Visit(builder);
		return builder.GetValue();
	}
}

void SimpleTemplateTest::TemplateParsingTest()
{
	CPPUNIT_ASSERT_EQUAL(GetTemplateSignature(0), std::string(""));
	CPPUNIT_ASSERT_EQUAL(GetTemplateSignature(""), std::string(""));
	CPPUNIT_ASSERT_EQUAL(GetTemplateSignature(" "), std::string("T1"));
	CPPUNIT_ASSERT_EQUAL(GetTemplateSignature("$_"), std::string("P0"));
	CPPUNIT_ASSERT_EQUAL(GetTemplateSignature("$$$$$$"), std::string("T1T1T1"));
	CPPUNIT_ASSERT_EQUAL(GetTemplateSignature("$1$_ $65536$_ ---"), std::string("P1P0T1P65536P1T4"));
}



