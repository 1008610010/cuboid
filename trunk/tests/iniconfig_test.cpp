#include <cstdio>

#include <xtl/ini/IniConfig.hpp>
#include <xtl/linux/UnixError.hpp>

int main(int argc, char * argv[])
{
	XTL::IniConfig config;

	try
	{
		config.LoadFromFile("../tests/data/main.ini");
		long long int v = config.GetSectionRequired("my_location").GetLongLongInt("name");
		fprintf(stderr, "%lld\n", v);
	}
	catch (const XTL::UnixError & e)
	{
		fprintf(stderr, "%s\n", e.What().c_str());
	}
}
