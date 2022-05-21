#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment (lib, "ws2_32.lib") 
#include <winsock2.h>
#include <string>
#include <iostream>
#include "Converter.cpp"
#include <string.h>

class SocketUtility
{
	WSADATA wsaData;
	SOCKET servSock;
	sockaddr_in sockAddr;
	SOCKADDR clntAddr;
	int nSize;
	char packet[24 * 24];
	SOCKET clntSock;
public:
	SocketUtility()
	{
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		memset(&sockAddr, 0, sizeof(sockAddr));
		sockAddr.sin_family = PF_INET; 
		sockAddr.sin_addr.s_addr = inet_addr("0.0.0.0"); 
		sockAddr.sin_port = htons(502); 
		bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
		listen(servSock, 20);
	}

	void Listen()
	{
		nSize = sizeof(SOCKADDR);
		clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
	}

	int GetData(char *packet)
	{
		return recv(clntSock, packet, 24 * 24, NULL);
	}

	void SendData(std::string str)
	{
		send(clntSock, str.c_str(), strlen(str.c_str()) + sizeof(char), NULL);
	}

	~SocketUtility()
	{
		closesocket(servSock);
		WSACleanup();
	}
};