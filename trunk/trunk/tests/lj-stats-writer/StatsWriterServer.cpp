#include "StatsWriterServer.hpp"

#include <stdio.h>

#include <xtl/FormatString.hpp>
#include <xtl/io/FileTransaction.hpp>

#include "StatsMessage.hpp"

namespace XC3
{
	StatsWriterServer::StatsWriterServer(const std::string & unixSocketPath, const std::string & outputDir)
		: XTL::UnixSocketServer(unixSocketPath),
		  outputDir_(outputDir),
		  outputFileName_(),
		  outputFileTime_(0),
		  outputStream_()
	{
		;;
	}

	StatsWriterServer::~StatsWriterServer() throw()
	{
		;;
	}

	void StatsWriterServer::OnTerminated()
	{
		if (outputStream_.get() != 0)
		{
			outputStream_->Commit();
			outputStream_.reset();
		}
	}

	namespace
	{
		const std::string GetFileName(time_t ts)
		{
			struct tm * t = ::gmtime(&ts);

			return XTL::FormatString("%04d-%02d-%02d-%02d-%02d-%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
		}
	}

	void StatsWriterServer::OpenOutputStream()
	{
		outputFileTime_ = ::time(0);
		outputFileName_ = GetFileName(outputFileTime_);

		const std::string filePath = XTL::FormatString("%s/binlog.%s", outputDir_, outputFileName_);
		fprintf(stderr, "Creating %s\n", filePath.c_str());
		outputStream_.reset(new XTL::FileTransaction(filePath));
	}

	namespace
	{
		class WriterHandler : public XTL::UnixSocketServer::ClientHandler
		{
			public:

				explicit WriterHandler(StatsWriterServer & server, XTL::UnixSocketServer::Client & client)
					: XTL::UnixSocketServer::ClientHandler(client),
					  server_(server),
					  messageBuffer_()
				{
					;;
				}

				virtual ~WriterHandler() throw()
				{
					;;
				}

				virtual void OnClientConnected()
				{
					fprintf(stderr, "OnClientConnected()\n");
				}

				virtual void OnClientDisconnected()
				{
					fprintf(stderr, "OnClientDisconnected()\n");
				}

				virtual void OnReceiveError()
				{
					fprintf(stderr, "OnReceiveError()\n");
				}

				virtual void OnDataReceived(const void * buffer, unsigned int size)
				{
					fprintf(stderr, "OnDataReceived(%u)\n", size);

					messageBuffer_.Write(buffer, size);

					while (messageBuffer_.IsComplete())
					{
						if (!messageBuffer_.Header()->IsMagicValid())
						{
							fprintf(stderr, "Message magic is invalid");
							Disconnect();
						}

						server_.WriteMessage(messageBuffer_);
						messageBuffer_.Remove();
					}
				}

			private:

				StatsWriterServer & server_;
				StatsMessageBuffer messageBuffer_;
		};
	}

	std::auto_ptr<XTL::UnixSocketServer::ClientHandler> StatsWriterServer::CreateClientHandler(Client & client)
	{
		return std::auto_ptr<XTL::UnixSocketServer::ClientHandler>(new WriterHandler(*this, client));
	}

	void StatsWriterServer::WriteMessage(const StatsMessageBuffer & messageBuffer)
	{
		if (outputStream_.get() == 0)
		{
			OpenOutputStream();
		}

		outputStream_->Write(messageBuffer.Header(), messageBuffer.TotalSize());

		if (outputStream_->Position() >= 120 && ::time(0) > outputFileTime_)
		{
			outputStream_->Commit();
			outputStream_.reset();
		}
	}
}

