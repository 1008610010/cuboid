#include <stdio.h>

#include <xtl/tp/Parser.hpp>

namespace XTL
{
	class HtmlParser : public Parser
	{
		public:

			HtmlParser(CharSource & charSource)
				: Parser(charSource)
			{
				Run();
			}

		private:

			void Run()
			{
				;;
			}
	};
}

int main(int argc, const char * argv[])
{
	printf("Ok!\n");
}

