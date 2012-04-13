#include <stdio.h>
#include <xtl/ProgramOptions.hpp>
#include <xtl/ProgramOptionsPrinter.hpp>

void PrintVersion(const XTL::OptionDesc * desc, const char * s);
void ShowHelp(const XTL::OptionDesc * desc, const char * s);

/*
ARGUMENTS_BEGIN
	ARGUMENT_TRIGGER (Help,   "h", "help", 0, OPTIONAL, "Show this help message", ShowHelp)
//	ARGUMENT_STRING  (Config, "c", "config", "FILE", REQUIRED | PASSWORD, "Config file name")
//	ARGUMENT_FLAG    (Turbo,  "t", "turbo", "Turbo calculation")
//	ARGUMENT_INTEGER (Power,  "p", "power", "#", REQUIRED, "Power of calculation")
//	ARGUMENT_INTEGER (Size,   "s", "size", "#",  OPTIONAL, "Size of array")

	ARGUMENT_MODES_BEGIN(Color, REQUIRED, "Color value")
		ARGUMENT_MODE_STRING(Color, "c", "color", "NAME", "Color by the name (red, green, blue, yellow, white, black, cyan, magenta)")
		ARGUMENT_MODE_FLAG(Red,   "r",   "red",   "Red color")
		ARGUMENT_MODE_FLAG(Green, "g",   "green", "Green color")
		ARGUMENT_MODE_FLAG(Blue,  "b",   "blue",  "Blue color")
		ARGUMENT_MODE_STRING(RGB, "rgb", "rgb", "RRGGBB", "Red-green-blue color representation")
		ARGUMENT_MODE_INTEGER_BEGIN(Grayscale, "gs", "grayscale", "#", "Grayscale color")
			ARGUMENT_INTEGER(Scale, "s", "scale", "#", OPTIONAL, "Luminosity value of white color")
		ARGUMENT_MODE_INTEGER_END(Grayscale)
	ARGUMENT_MODES_END(Color)

//		ARGUMENT_MODE_FLAG(Black, "b", "black", OPTIONAL, "Black color")
	ARGUMENT_STRING(Log, 0, "log", "FILE", OPTIONAL, "Path to log file")
ARGUMENTS_END
*/

/*
#define ARGUMENT_FLAG(ID, LABEL1, LABEL2, TEXT) \
#define ARGUMENT_STRING(ID, LABEL1, LABEL2, VALUE2, FLAGS, TEXT) \
#define ARGUMENT_INTEGER(ID, LABEL1, LABEL2, VALUE2, FLAGS, TEXT) \
#define ARGUMENT_MODE_FLAG(ID, LABEL1, LABEL2, TEXT) \
#define ARGUMENT_MODE_INTEGER(ID, LABEL1, LABEL2, VALUE2, FLAGS, TEXT) \
#define ARGUMENT_MODE_FLAG_BEGIN(ID, LABEL1, LABEL2, TEXT) \
#define ARGUMENT_MODE_FLAG_END(ID) \
#define ARGUMENT_MODE_STRING_BEGIN(ID, LABEL1, LABEL2, VALUE2, FLAGS, TEXT) \
#define ARGUMENT_MODE_STRING_END(ID) \
#define ARGUMENT_MODE_INTEGER_BEGIN(ID, LABEL1, LABEL2, VALUE2, FLAGS, TEXT) \
#define ARGUMENT_MODE_INTEGER_END(ID) \
#define ARGUMENT_GROUP_BEGIN(ID, FLAGS, TEXT) \
#define ARGUMENT_GROUP_END(ID) \
#define ARGUMENT_GROUP_STRING(ID, LABEL1, LABEL2, VALUE2, FLAGS, TEXT) \
*/

ARGUMENTS_BEGIN
	ARGUMENT_FLAG(Flag, "f", "flag", "Simple flag")
	ARGUMENT_TRIGGER (Help, "h", "help", "Show this help message", ShowHelp)

	ARGUMENT_MODES_BEGIN(Color, REQUIRED, "Color value")
		ARGUMENT_MODE_FLAG(Black, 0, "black", "Black color")
		ARGUMENT_MODE_FLAG(White, 0, "white", "White color")
		ARGUMENT_MODE_FLAG(Red,   0, "red",   "Red color")
		ARGUMENT_MODE_FLAG(Green, 0, "green", "Green color")
		ARGUMENT_MODE_FLAG(Blue,  0, "blue",  "Blue color")
		ARGUMENT_MODE_STRING(Name, "n", "name", "NAME", OPTIONAL, "Color by the name (red, green, blue, yellow, white, black, cyan, magenta)")
		ARGUMENT_MODE_GROUP_BEGIN(RGB, "RGB")
			ARGUMENT_GROUP_INTEGER (Red,   "r", 0, "#", REQUIRED, "Red component")
			ARGUMENT_GROUP_INTEGER (Green, "g", 0, "#", REQUIRED, "Green component")
			ARGUMENT_GROUP_INTEGER (Blue,  "b", 0, "#", REQUIRED, "Blue component")
			ARGUMENT_GROUP_FLAG    (Short, 0, "short", "Scale color component by 16, not by 256")
		ARGUMENT_MODE_GROUP_END(RGB)
		ARGUMENT_MODE_INTEGER_BEGIN(Grayscale, "gs", "grayscale", "#", OPTIONAL, "Grayscale color")
			ARGUMENT_INTEGER(Scale, 0, "max", "#", OPTIONAL, "Luminosity value of white color")
		ARGUMENT_MODE_INTEGER_END(Grayscale)
/*
		ARGUMENT_MODE_STRING(RGB, "rgb", "rgb", "RRGGBB", "Red-green-blue color representation")
		ARGUMENT_MODE_INTEGER_BEGIN(Grayscale, "gs", "grayscale", "#", "Grayscale color")
			ARGUMENT_INTEGER(Scale, "s", "scale", "#", OPTIONAL, "Luminosity value of white color")
		ARGUMENT_MODE_INTEGER_END(Grayscale)
*/
	ARGUMENT_MODES_END(Color)
/*
	ARGUMENT_GROUP_BEGIN (Color, OPTIONAL, "Color")
		ARGUMENT_GROUP_INTEGER(Red,   "r", "red",   "#", REQUIRED, "Red component")
		ARGUMENT_GROUP_INTEGER(Green, "g", "green", "#", REQUIRED, "Green component")
		ARGUMENT_GROUP_INTEGER(Blue,  "b", "blue",  "#", REQUIRED, "Blue component")
		ARGUMENT_GROUP_FLAG   (BlackWhite, "bw", "black-and-white", "Convert color to black-and-white")
	ARGUMENT_GROUP_END(Color)
*/
ARGUMENTS_END

int main(int argc, const char * argv[])
{
	ARGUMENTS_PARSE(argc, argv)

/*
	if (Args->IsColor())
	{
		fprintf(stderr, "red = %d\n", (int) Args->Color().GetRed());
		if (Args->Color().HasGreen())
		{
			fprintf(stderr, "green = %d\n", (int) Args->Color().GetGreen());
		}
	}
	else
	{
		fprintf(stderr, "No color\n");
	}
*/
}

void PrintVersion(const XTL::OptionDesc * desc, const char * s)
{
	fprintf(stderr, "program-option-test v1.0\n");
}

void ShowHelp(const XTL::OptionDesc * desc, const char * s)
{
	fprintf(stderr, "Usage: %s OPTIONS\n\n", Args->ProgramName().c_str());
	Args->Iterate(XTL::ProgramOptionsPrinter(stderr, 30));
	throw XTL::ProgramOptionsExit(0);
}
