#include "xtl/TcpClient.hpp"
#include "xtl/ByteBuffer.hpp"

int main(int argc, const char * argv[])
{
	XTL::ByteBuffer bb(600000);
	bb << 1;
	XTL::ByteBuffer a = bb;
	return 0;

	const char * remoteHost = "www.microsoft.com";
	int remotePort = 80;

	XTL::TcpClient client;

	if (!client.Connect(remoteHost, remotePort))
	{
		fprintf(stderr, "Unable to connect to %s:%d\n", remoteHost, remotePort);
		return 1;
	}

	fprintf(stdout, "Status: %d\n", client.Status());

	return 0;

	fprintf(stderr, "Connected to %s:%d\n", remoteHost, remotePort);

	client.Push(
		"GET / HTTP/1.1\r\n"
		"Host: www.yandex.ru\r\n"
		"User-Agent: Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5\r\n"
		"Accept: text/html\r\n"
		"Connection: close\r\n\r\n"
	);

	while (client.HasSendingData())
	{
		if (!client.DoSend())
		{
			fprintf(stderr, "Error, while sending request\n");
			return 1;
		}
	}

	while (client.DoReceive())
	{
		;;
	}

	printf("%s\n", client.ReceivedData());

	client.Close();

	return 0;

//	TcpSocket cs;

//	int result = cs.connect(host.c_str(), port, true);
//	printf("connect = %d\n", result);

	/*
	const char * const q =
	"GET / HTTP/1.1\r\n"
	"Host: yandex.ru\r\n"
	"User-Agent: Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5\r\n"
	"Accept: text/html\r\n"
	"Connection: close\r\n\r\n";

	printf("sending\n");
	cs.sendAll(q, strlen(q));
	printf("ok!\n");

	char buffer[4096];
	int received;

	while (cs.opened())
	{
		received = cs.recv(buffer, sizeof(buffer) - 1);
		if (received > 0)
		{
			buffer[received] = '\0';
			printf("received: %d\n", received);
		}
	}
	*/

//	cs.close();
}

