#include <iostream>
#include <string>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
using namespace std;
int main()
{
	const char* serverIpAddr = "127.0.0.1";
	enum ePort { PORT = 8080 };

	WSADATA wsData;
	int wsResult = WSAStartup(MAKEWORD(2, 2), &wsData);

	cout << "> echo-client is activated" << endl;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);


	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(PORT);
	int convResult = inet_pton(AF_INET, serverIpAddr, &hint.sin_addr);

	int connResult = connect(sock, reinterpret_cast<sockaddr*>(&hint), sizeof(hint));

	enum eBufSize { BUF_SIZE = 4096 };
	char buf[BUF_SIZE];
	std::string userMsg;

	while (true)
	{
		std::cout << "> ";
		std::getline(std::cin, userMsg);

		if (userMsg.size() > 0)
		{
			int sendResult = send(sock, userMsg.c_str(), userMsg.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				ZeroMemory(buf, BUF_SIZE);
				int bytesReceived = recv(sock, buf, BUF_SIZE, 0);
				if (bytesReceived > 0)
				{
					std::cout << "> received: " << buf << std::endl;
				}
			}
		}
		if (userMsg == "quit") break;

	}
	cout << "> echo-client is de-activated" << endl;
	closesocket(sock);
	WSACleanup();

	return 0;
}