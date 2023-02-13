#include <iostream>
#include <winsock2.h>
using namespace std;

#pragma comment(lib,"ws2_32.lib") 
#pragma warning(disable:4996) 

#define BUFLEN 512
#define PORT 8888

int main()
{
    sockaddr_in server, client;


    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        cout << "Failed. Error Code: %d", WSAGetLastError();
        exit(0);
    }

    SOCKET server_socket;
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
    {
        cout << "Could not create socket: %d", WSAGetLastError();
    }

    cout << "> echo-server is activated" << endl;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    int current_port;

    if (bind(server_socket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
    {
        cout << "Bind failed with error code: %d", WSAGetLastError();
        exit(EXIT_FAILURE);
    }

    while (true)
    {
        fflush(stdout);
        char message[BUFLEN] = {};

        int message_len;
        int slen = sizeof(sockaddr_in);
        if (message_len = recvfrom(server_socket, message, BUFLEN, 0, (sockaddr*)&client, &slen) == SOCKET_ERROR)
        {
            cout << "recvfrom() failed with error code: %d", WSAGetLastError();
            exit(0);
        }

        cout << "> client connected by IP address " << inet_ntoa(client.sin_addr) << " with Port number " << ntohs(client.sin_port) << endl;

        cout << "> echoed: " << message << endl;



        if (sendto(server_socket, message, strlen(message), 0, (sockaddr*)&client, sizeof(sockaddr_in)) == SOCKET_ERROR)
        {
            cout << "sendto() failed with error code: %d", WSAGetLastError();
            return 3;
        }
    }

    closesocket(server_socket);
    WSACleanup();
}