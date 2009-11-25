#ifndef _XTL__PROGRAM_OPTIONS_PRINTER_HPP__
#define _XTL__PROGRAM_OPTIONS_PRINTER_HPP__ 1

#include "ProgramOptions.hpp"

namespace XTL
{
	class ProgramOptionsPrinter
	{
		public:

			ProgramOptionsPrinter(FILE * file, int columnSize);

			void OnModeSelector(const OptionDesc * desc, const Option * option, int depth);

			void OnModeValue(const OptionDesc * desc, const Option * option, int depth);

			void OnOption(const OptionDesc * desc, const Option * option, int depth);

		protected:

			void Indent(int depth);

			const std::string Space(int size) const;

			void Print(const std::string & first, const std::string & second);

			FILE * f_;
			std::string column_;
	};
}

#endif
