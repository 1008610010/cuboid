#ifndef XTL__PROGRAM_OPTIONS_HPP__
#define XTL__PROGRAM_OPTIONS_HPP__ 1

#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Exception.hpp"   // for TerminateProgram
#include "PrintStream.hpp"
#include "utils/AutoPtrVector.hpp"

namespace XTL
{
	class ProgramOption;
	class ProgramOptionsPool;

	ProgramOptionsPool & ProgramOptions();

	namespace PO
	{
		enum ProgramOptionFlags
		{
			REQUIRED = 0x0001,
			PASSWORD = 0x0002
		};

		std::auto_ptr<XTL::ProgramOption> Flag(const char * label, const char * text, bool & result);

		std::auto_ptr<XTL::ProgramOption> String(const char * label, const char * text, std::string & result, unsigned int flags = 0);

		std::auto_ptr<XTL::ProgramOption> Integer(const char * label, const char * text, int & result, unsigned int flags = 0);

		std::auto_ptr<XTL::ProgramOption> Integer(const char * label, const char * text, unsigned int & result, unsigned int flags = 0);

		std::auto_ptr<XTL::ProgramOption> LongLong(const char * label, const char * text, long long int & result, unsigned int flags = 0);

		std::auto_ptr<XTL::ProgramOption> LongLong(const char * label, const char * text, unsigned long long int & result, unsigned int flags = 0);

		std::auto_ptr<XTL::ProgramOption> Choice(const char * label, const char * text, unsigned int & result, const char * const * options, unsigned int flags = 0);

		std::auto_ptr<XTL::ProgramOption> Array(const char * label, const char * text, std::vector<std::string> & result, unsigned int flags = 0);

		typedef void (*Handler) (const char *);

		std::auto_ptr<XTL::ProgramOption> Function(const char * label, const char * text, Handler handler, unsigned int flags = 0);

		void ProgramOptionsPrintUsage(const char * value);

		std::auto_ptr<XTL::ProgramOption> PrintUsage(const char * label, const char * text);
	}

	/******************************************************************/

	class StringsTable;

	class ProgramOption
	{
		public:

			ProgramOption(const char * label, const char * text, unsigned int flags);

			virtual ~ProgramOption() throw();

			const char * GetLabel() const;

			const char * GetText() const;

			const std::string GetTextWithRequired() const;

			bool IsRequired() const;

			bool Exists() const;

			void Set(char * value);

			virtual bool IsArray() const = 0;

			virtual bool NeedValue() const = 0;

		protected:

			virtual void SetValue(const char * value) = 0;

		private:

			const char * const label_;
			const char * const text_;
			const unsigned int flags_;
			bool               exists_;
	};

	class ProgramOptionsList
	{
		public:

			ProgramOptionsList();

			bool IsEmpty() const;

			unsigned int Size() const;

			ProgramOptionsList & operator<< (std::auto_ptr<ProgramOption> option);

			void Print(PrintStream & printStream);

			void PrintToTable(StringsTable & table) const;

		private:

			friend class ProgramOptionsPool;

			bool IsLastArray() const;

			bool IsLastOptional() const;

			void SetOptionValue(char * value);

			void CheckRequired(std::list<const ProgramOption *> & missingRequired);

			XTL::AutoPtrVector<ProgramOption> optionsList_;
			unsigned int currentIndex_;
	};

	class ProgramOptionsMap
	{
		public:

			ProgramOptionsMap();

			bool IsEmpty() const;

/*
			bool Contains(const std::string & key) const
			{
				return optionsMap_.count(key) > 0;
			}
*/
			ProgramOptionsMap & operator<< (std::auto_ptr<ProgramOption> option);

			void PrintToTable(StringsTable & table) const;

		private:

			friend class ProgramOptionsPool;

			ProgramOption * FindOption(const std::string & key) const;

			void SetOptionValue(const std::string & key, char * value);

			void SetOptionValue(const std::string & key, ProgramOption * option, char * value);

			void CheckRequired(std::list<const ProgramOption *> & missingRequired);

			XTL::AutoPtrVector<ProgramOption>      optionsList_;
			std::map<std::string, ProgramOption *> optionsMap_;
			std::vector<const ProgramOption *>     optionsRequired_;
	};

	class ProgramOptionsPool
	{
		public:

			ProgramOptionsList & List();

			ProgramOptionsMap & Map();

			void Parse(int argc, char * argv[]);

			void PrintUsage(PrintStream & printStream);

		private:

			friend ProgramOptionsPool & ProgramOptions();

			ProgramOptionsPool();

			ProgramOptionsPool(const ProgramOptionsPool &);
			ProgramOptionsPool & operator= (const ProgramOptionsPool &);

			std::string        programName_;
			ProgramOptionsList optionsList_;
			ProgramOptionsMap  optionsMap_;
	};
}

#endif
