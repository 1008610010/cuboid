#ifndef XTL__STATS_WRITER_SERVER_HPP__
#define XTL__STATS_WRITER_SERVER_HPP__ 1

#include <time.h>

#include <memory.h>
#include <string>

#include <xtl/linux/utils/UnixSocketServer.hpp>

namespace XTL
{
	class FileTransaction;
}

namespace XC3
{
	class StatsMessageBuffer;

	class StatsWriterServer : public XTL::UnixSocketServer
	{
		public:

			StatsWriterServer(const std::string & unixSocketPath, const std::string & outputDir);

			virtual ~StatsWriterServer() throw();

			virtual std::auto_ptr<UnixSocketServer::ClientHandler> CreateClientHandler(Client & client);

			virtual void OnTerminated();

			void WriteMessage(const StatsMessageBuffer & messageBuffer);

		private:

			void OpenOutputStream();

			const std::string outputDir_;
			std::string       outputFileName_;
			time_t            outputFileTime_;
			std::auto_ptr<XTL::FileTransaction> outputStream_;
	};
}

#endif

