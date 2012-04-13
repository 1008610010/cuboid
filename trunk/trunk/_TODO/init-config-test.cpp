#include <xtl/InitConfig.hpp>

int main(int argc, const char * argv[])
{
	XTL::InitConfig config;

	try
	{
		if (!config.Load("proba.ini"))
		{
			fprintf(stderr, "Unable to load config: file not found\n");
			return 1;
		}

		const XTL::InitConfigSection * section = config.GetSection("Counter");
		if (section != 0)
		{
			fprintf(stderr, "%d\n", section->Get("FlushPeriod").IsNull());
			fprintf(stderr, "%d\n", section->Get("FlushPeriod1").IsNull());
			fprintf(stderr, "%d\n", section->Get("DumpDirectory").IsNull());
			XTL::Variant tmp = section->Get("DumpDirectory");
			if (!tmp.IsNull())
			{
				fprintf(stderr, "%s\n", tmp.AsString().c_str());
			}
			return 0;
		}
	}
	catch (const XTL::InitConfigParser::Error & e)
	{
		fprintf(stderr, "Config parser error (row %d, column %d): %s\n", e.Row() + 1, e.Column() + 1, e.What());
		return 1;
	}
	catch (const XTL::InitConfig::Error & e)
	{
		fprintf(stderr, "%s\n", e.What());
		return 1;
	}

	return 0;
}
