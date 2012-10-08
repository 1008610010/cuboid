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

+
+		enum Flags
+		{
+			OPTIONAL = 0x0000,
+			REQUIRED = 0x0001,
+			BOOLEAN  = 0x0002,
+			PASSWORD = 0x0004
+		};
+
+

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

