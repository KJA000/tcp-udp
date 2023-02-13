#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <stdlib.h>

#pragma comment (lib,"ws2_32.lib")
using namespace std;
enum Port { SERVER_PORT = 8080 };

char ipAddr[INET_ADDRSTRLEN];

int main()
{
	string msg;
	WSADATA wsaData;
	int iniR = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iniR != 0) cout << "fail to initialize" << endl;
	cout << "> echo-server is activated" << endl;

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	int isSocket = s;
	try {
		if (isSocket == INVALID_SOCKET)
		{
			throw isSocket;
		}
	}
	catch (SOCKET s)
	{
		cout << "> socket() failed and program terminated";
		return -1;
	}

	sockaddr_in a{};
	a.sin_family = AF_INET;
	a.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	a.sin_port = htons(SERVER_PORT);

	int bindresult = bind(s, reinterpret_cast<sockaddr*>(&a), sizeof(a));
	try {

		if (bindresult == SOCKET_ERROR)
		{
			throw bindresult;
		}
	}
	catch(int bindresult){

		cout << "> bind() failed and program terminated" << endl;
		closesocket(s);
		return -1;
	}
	
	int listenR = listen(s, SOMAXCONN);
	try {
		if (s == SOCKET_ERROR)
		{
			throw s;
		}
	}
	catch(int s){
			cout << "> listen() failed and program terminated" << endl;
			closesocket(s);
			return -1;
	}

	sockaddr_in ClientInfo;
	int Csize = sizeof(ClientInfo);
	SOCKET c = accept(s, reinterpret_cast<sockaddr*>(&ClientInfo), &Csize);
	inet_ntop(AF_INET, &(ClientInfo).sin_addr, ipAddr, INET6_ADDRSTRLEN);
	cout << "> client connected by IP address " << ipAddr << " with Port number " << (ClientInfo).sin_port << endl;
	while (true)
	{
		int closeR = closesocket(s);
		string host[NI_MAXHOST];
		string service[NI_MAXHOST];

		enum Bufsize { BUF_SIZE = 4096 };
		char buf[BUF_SIZE];
		memset(buf, 0, BUF_SIZE);

		int Received = recv(c, buf, BUF_SIZE, 0);

		cout << "> echoed: " << buf << endl;
		msg = buf;

		send(c, buf, Received + 1, 0);
		if (msg == "quit") {
			cout << "> echo-server is de-activated" << endl;
			break;
		}
	} //while (msg != "quit");

	closesocket(c);
	WSACleanup();

}
