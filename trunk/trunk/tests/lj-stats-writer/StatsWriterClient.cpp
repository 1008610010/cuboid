#include "StatsWriterClient.hpp"

#include "StatsMessage.hpp"

namespace XC3
{
	StatsWriterClient::StatsWriterClient(const std::string & unixSocketPath)
		: client_(unixSocketPath)
	{
		client_.Connect();
	}

	void StatsWriterClient::Send(const std::string & message)
	{
		StatsMessageHeader header(message.size());

		client_.Send(&header, sizeof(header));
		client_.Send(message.data(), message.size());
	}
}

