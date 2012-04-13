#include <cppunit/CompilerOutputter.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

int main(int argc, char* argv[])
{
	CppUnit::TestResult result;

	CppUnit::TestResultCollector collectedResults;
	result.addListener(&collectedResults);

	CppUnit::BriefTestProgressListener progress;
	result.addListener(&progress);

	CppUnit::TestRunner runner;
	CppUnit::TestFactoryRegistry & registry = CppUnit::TestFactoryRegistry::getRegistry();
	runner.addTest(registry.makeTest());
	runner.run(result);

	CppUnit::CompilerOutputter outputter(&collectedResults, std::cerr);
	outputter.write();

	return collectedResults.wasSuccessful () ? 0 : 1;
}

