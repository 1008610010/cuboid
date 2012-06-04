#ifndef XC3__STATS_WRITER_CLIENT_HPP__
#define XC3__STATS_WRITER_CLIENT_HPP__ 1

#include <string>

#include <xtl/linux/utils/UnixSocketClient.hpp>

namespace XC3
{
	class StatsWriterClient
	{
		public:

			explicit StatsWriterClient(const std::string & unixSocketPath);

			void Send(const std::string & message);

		private:

			XTL::UnixSocketClient client_;
	};
}

#endif

