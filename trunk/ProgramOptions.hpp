+<<<<<<< .mine
+
+/*
+	-f
+	-f value
+	--flag
+	--flag=value
+	...
+ */
+class ProgramOptions
+{
+	public:
+
+		class ParseError
+		{
+			public:
+
+				explicit ParseError(const std::string & key)
+					: key_(key)
+				{
+					;;
+				}
+
+			private:
+
+				const std::string key_;
+		};
+
+		class UnknownOption : public ParseError
+		{
+			public:
+
+				explicit UnknownOption(const std::string & key)
+					: ParseError(key)
+				{
+					;;
+				}
+		};
+
+		class NeedOptionValue : public ParseError
+		{
+			public:
+
+				explicit NeedOptionValue(const std::string & key)
+					: ParseError(key)
+				{
+					;;
+				}
+		};
+
+		enum Flags
+		{
+			OPTIONAL = 0x0000,
+			REQUIRED = 0x0001,
+			BOOLEAN  = 0x0002,
+			PASSWORD = 0x0004
+		};
+
+		void Parse(int argc, char * argv[])
+		{
+			int i = 0;
+			while (i < argc)
+			{
+				char * arg = argv[i];
+				if (arg[0] == '-')
+				{
+					if (arg[1] == '-')
+					{
+						char * v = ::strchr(arg + 2, '=');
+
+						if (v == 0)
+						{
+							OnOption(arg, 0);
+						}
+						else
+						{
+							OnOption(std::string(arg, v), v + 1);
+						}
+
+						++i;
+					}
+					else
+					{
+						const ProgramOption * option = FindOption(arg);
+						if (option == 0)
+						{
+							throw UnknownOption(arg);
+							// TODO: or
+						}
+
+						++i;
+
+						if (option->NeedValue())
+						{
+							if (i == argc)
+							{
+								throw NeedOptionValue(arg);
+							}
+
+							OnOption(arg, argv[i]);
+							++i;
+						}
+						else
+						{
+							OnOption(arg, 0);
+						}
+					}
+				}
+				else
+				{
+					++i;
+				}
+			}
+		}
+
+		void OnOption(const std::string & key, char * value)
+		{
+		}
+
+	private:
+
+};
+
+
+=======
+#include <xtl/utils/SimpleStringMatcher.hpp>
+
+>>>>>>> .r206
 int main(int argc, const char * argv[])
 {
 	{
+		XTL::SimpleStringMatcher ssm("abc-(%d%d))");
+
+		if (XTL::SimpleStringMatcher::Result result = ssm.Match("abc-23"))
+		{
+			printf("Matched '%s'!\n", result.GetSubmatch("abc-23", 0).c_str());
+		}
+
+		return 0;
+	}
+
+	{
 		time_t ts = ::time(0);
 		struct tm * t = ::gmtime(&ts);

Select: (p) postpone, (df) diff-full, (e) edit, (r) resolved,
        (mc) mine-conflict, (tc) theirs-conflict,
        (s) show all options: tc
G    trunk/tests/forge.cpp
A    trunk/lib/xtl/utils/SimpleStringMatcher.cpp
A    trunk/lib/xtl/utils/SimpleStringMatcher.hpp
U    trunk/lib/xtl/plain/RecordArray.cpp
U    trunk/lib/CMakeLists.txt

