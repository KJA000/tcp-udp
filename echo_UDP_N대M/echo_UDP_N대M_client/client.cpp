#include <iostream>
#include <winsock2.h>
using namespace std;

#pragma comment(lib,"ws2_32.lib") 
#pragma warning(disable:4996) 
   
#define SERVER "127.0.0.1"  
#define BUFLEN 512  
#define PORT 8888  

int main()
{
    
    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
    {
        cout << "Failed. Error Code: %d", WSAGetLastError();
        return 1;
    }

    sockaddr_in server;
    int client_socket;
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) // <<< UDP socket
    {
        cout << "socket() failed with error code: %d", WSAGetLastError();
        return 2;
    }
    cout << "> echo-client is activated" << endl;


    memset((char*)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.S_un.S_addr = inet_addr(SERVER);

    while (true)
    {
        char message[BUFLEN];
        cout << ">> ";
        cin.getline(message, BUFLEN);

        if (sendto(client_socket, message, strlen(message), 0, (sockaddr*)&server, sizeof(sockaddr_in)) == SOCKET_ERROR)
        {
            cout << "sendto() failed with error code: %d", WSAGetLastError();
            return -1;
        }

        char answer[BUFLEN] = {};

        int slen = sizeof(sockaddr_in);
        int answer_length;
        if (answer_length = recvfrom(client_socket, answer, BUFLEN, 0, (sockaddr*)&server, &slen) == SOCKET_ERROR)
        {
            cout << "recvfrom() failed with error code: %d", WSAGetLastError();
            exit(0);
        }

        cout << "> received: " << answer << "\n";
        if (string(message) == "quit") break;
        recvfrom(client_socket, answer, BUFLEN, 0, (sockaddr*)&server, &slen);
    }

    closesocket(client_socket);
    WSACleanup();
}