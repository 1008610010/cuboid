#ifndef XTL__PID_FILE_HPP__
#define XTL__PID_FILE_HPP__ 1

#include <memory>
#include <string>

#include "../../FormatString.hpp"
#include "../AccessMode.hpp"
#include "../fs/File.hpp"

namespace XTL
{
	class PidFile
	{
		private:

			class Mode
			{
				public:

					explicit Mode(unsigned int id)
						: id_(id) { ;; }

					bool operator== (const Mode & other) const
					{
						return id_ == other.id_;
					}

				private:

					const unsigned int id_;
			};

		public:

			static const Mode REMOVE;
			static const Mode DO_NOT_REMOVE;

			// @throws XTL::UnixError
			explicit PidFile(const std::string & fileName, Mode mode = REMOVE);

			~PidFile() throw();

			void Remove();

		private:

			PidFile(const PidFile &);
			PidFile & operator= (const PidFile &);

			std::string fileName_;
			Mode        mode_;
	};
}

#endif

