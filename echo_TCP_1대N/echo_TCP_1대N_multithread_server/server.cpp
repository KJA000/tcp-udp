#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <string>]
#include <signal.h>
#include <thread>
#include <io.h>
#include <vector>

#pragma comment (lib,"ws2_32.lib")
using namespace std;
enum Port { SERVER_PORT = 8080 };
char ipAddr[INET_ADDRSTRLEN];
SOCKET s;
SOCKET c;
const int MAXCON = 5;
string msg;
thread proc2;
int Csize;
sockaddr_in ClientInfo;

#define PACKET_SIZE 1024
SOCKET skt, client_sock;
sockaddr_in a;

void proc_recvs() {
	
	s = socket(AF_INET, SOCK_STREAM, 0);

	int bindresult = bind(s, reinterpret_cast<sockaddr*>(&a), sizeof(a));

	listen(s, MAXCON);
	sockaddr_in ClientInfo;
	Csize = sizeof(ClientInfo);
	c = accept(s, reinterpret_cast<sockaddr*>(&ClientInfo), &Csize);
	do {

		s = socket(AF_INET, SOCK_STREAM, 0);

		int bindresult = bind(s, reinterpret_cast<sockaddr*>(&a), sizeof(a));

		listen(s, MAXCON);
		sockaddr_in ClientInfo;
		Csize = sizeof(ClientInfo);
		c = accept(s, reinterpret_cast<sockaddr*>(&ClientInfo), &Csize);
		//cout << c << "************************\n";

	} while (c == INVALID_SOCKET);
	
	if(c != INVALID_SOCKET) {
		inet_ntop(AF_INET, &(ClientInfo).sin_addr, ipAddr, INET_ADDRSTRLEN);
		cout << "> client connected by IP address " << ipAddr << " with Port number " << (ClientInfo).sin_port << endl;
	}

      while (true) {
		char buffer[PACKET_SIZE] = { 0 };

		int closeR = closesocket(s);
		string host[NI_MAXHOST];
		string service[NI_MAXHOST];

		enum Bufsize { BUF_SIZE = 4096 };
		char buf[BUF_SIZE];
		memset(buf, 0, BUF_SIZE);

		int Received = recv(c, buf, BUF_SIZE, 0);
		if (buf != NULL) {
			cout << "^^1";
			cout << "> echoed: " << buf << endl;
			msg = buf;
		}
		else break;

		send(c, buf, Received + 1, 0);
		if (msg == "quit") {
			cout << "^^3";
			cout << "> echo-server is de-activated" << endl;
			return;
		}
	}
}

void main() {
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);


	WSADATA wsaData;
	int iniR = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iniR != 0) cout << "fail to initialize" << endl;

	cout << "> echo-server is activated" << endl;

	sockaddr_in a{};
	a.sin_family = AF_INET;
	a.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	a.sin_port = htons(SERVER_PORT);

	proc_recvs();
	cout << "함수호출성공\n";
	proc2 = thread(proc_recvs);
	cout << "스레드도 성공\n";
	proc2.join();
	cout << "조인까지 성공\n";
	closesocket(client_sock);
	closesocket(skt);
	WSACleanup();
	
}