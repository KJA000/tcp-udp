#include <iostream>
#include <winsock2.h>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

#pragma comment(lib,"ws2_32.lib") 
#pragma warning(disable:4996) 

#define BUFLEN 512
#define PORT 8888

vector<string> client_vec;
vector<int> port_cli;
string port_client;

int main()
{
    
    sockaddr_in server, client;
    vector<sockaddr_in> allcli;

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
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(server_socket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
    {
        cout << "Bind failed with error code: %d", WSAGetLastError();
        exit(EXIT_FAILURE);
    }
    cout << "> echo-server is activated" << endl;

    while (true)
    {
        fflush(stdout);
        char message[BUFLEN] = {};

        int i = 0;

        int message_len;
        int slen = sizeof(sockaddr_in);
        if (message_len = recvfrom(server_socket, message, BUFLEN, 0, (sockaddr*)&client, &slen) == SOCKET_ERROR)
        {
            cout << "recvfrom() failed with error code: %d", WSAGetLastError();
            exit(0);
        }

        cout << "> client connected by IP address " << inet_ntoa(client.sin_addr) << " with Port number " << ntohs(client.sin_port) << endl;

        allcli.push_back(client);
        
        //sendto 로 보내기
        if (sendto(server_socket, message, strlen(message), 0, (sockaddr*)&client, sizeof(sockaddr_in)) == SOCKET_ERROR)
        {
            cout << "sendto() failed with error code: %d", WSAGetLastError();
            return 3;
        }
        if ((sockaddr*)(&allcli[i % 2]) != (sockaddr*)(&client))
        {
            sendto(server_socket, message, strlen(message), 0, (sockaddr*)&allcli[i % 2], sizeof(sockaddr_in));
        }

        if (string(message) == "#REG")
        {
            cout << "> client registered " << inet_ntoa(client.sin_addr) << endl;
            client_vec.push_back(inet_ntoa(client.sin_addr));
            port_cli.push_back(ntohs(client.sin_port));

        }
        else if (string(message) == "#DEREG"&&find(port_cli.begin(),port_cli.end(), ntohs(client.sin_port))!=port_cli.end())
        {
            cout << "> client de-registered " << inet_ntoa(client.sin_addr) << endl;
            client_vec.erase(remove(client_vec.begin(),client_vec.end(),inet_ntoa(client.sin_addr)),client_vec.end());
        }
        else {

            if (client_vec.empty() == true)
            {
                cout << "> no clients to echo" << endl;
            }
            else if (find(port_cli.begin(), port_cli.end(), ntohs(client.sin_port)) == port_cli.end())
            {
                cout << "> ignores a message from un-registered client" << endl;
            }
            else
            {
               cout << "> echoed:  " << message << " to " << client_vec.size() << " clients. " << endl;
               for (int i = 0; i < allcli.size(); i++)
               {
                   if ((sockaddr*)(&allcli[i]) != (sockaddr*)(&client))
                   {
                       sendto(server_socket, message, strlen(message), 0, (sockaddr*)&allcli[i], sizeof(sockaddr_in));
                   }
               } 
            }
        }
        i++;
        //sendto(server_socket, message, strlen(message), 0, (sockaddr*)&client, sizeof(sockaddr_in))
        //cin.getline(message, BUFLEN);

    }

    closesocket(server_socket);
    WSACleanup();
}