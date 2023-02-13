#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <thread>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

std::string userMsg;
SOCKET sock;

enum eBufSize { BUF_SIZE = 4096 };
char buf[BUF_SIZE];

void proc_recv() {

	string cmd; //string 생성
	while (!WSAGetLastError()) {

		ZeroMemory(buf, BUF_SIZE);
		int bytesReceived = recv(sock, buf, BUF_SIZE, 0);
		if (bytesReceived > 0)
		{
			std::cout << "> received: " << buf << std::endl;
		}
		if (userMsg == "quit") return;
	}
	
}

int main() {
	const char* serverIpAddr = "127.0.0.1";
	enum ePort { PORT = 8080 };

	WSADATA wsData;
	int wsResult = WSAStartup(MAKEWORD(2, 2), &wsData);

	cout << "> echo-client is activated" << endl;

	sock = socket(AF_INET, SOCK_STREAM, 0);


	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(PORT);
	int convResult = inet_pton(AF_INET, serverIpAddr, &hint.sin_addr);

	int connResult = connect(sock, reinterpret_cast<sockaddr*>(&hint), sizeof(hint));
	do {
		if (connResult == -1)
		{
			connResult = connect(sock, reinterpret_cast<sockaddr*>(&hint), sizeof(hint));
		}
	} while (connResult == -1);
	thread proc1(proc_recv);

	while (!WSAGetLastError()) {
		std::cout << "> *** ";
		std::getline(std::cin, userMsg);

		if (userMsg.size() > 0)
		{
			int sendResult = send(sock, userMsg.c_str(), userMsg.size() + 1, 0);
		}
	}
	cout << "조인으로 넘어감";
	proc1.join();
	cout << proc1.joinable();
	cout << "> echo-client is de-activated" << endl;
	closesocket(sock);
	WSACleanup();
}