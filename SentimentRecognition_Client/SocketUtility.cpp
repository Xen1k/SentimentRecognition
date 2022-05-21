#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <string>
#include <iostream>

#pragma comment (lib, "ws2_32.lib") // Загружаем ws2_32.dll

using std::cout;
using std::endl;

class SocketUtility 
{
private:
    WSADATA m_WsaData;
    SOCKET m_Sock;
    sockaddr_in m_SockAddr;
    char* m_SzBuffer;
    int m_HasConnetcion = -1;
    const std::string m_Ip;
    const int m_packetSize;
public:
	SocketUtility(std::string ip, int packetSize) : m_Ip(ip), m_packetSize(packetSize)
	{
        WSAStartup(MAKEWORD(2, 2), &m_WsaData); //0000001000000010
        m_Sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        memset(&m_SockAddr, 0, sizeof(m_SockAddr)); 
        m_SockAddr.sin_family = PF_INET;
        m_SockAddr.sin_addr.s_addr = inet_addr(ip.c_str()); 
        m_SockAddr.sin_port = htons(502);
        m_HasConnetcion = connect(m_Sock, (SOCKADDR*)&m_SockAddr, sizeof(SOCKADDR));
        m_SzBuffer = new char[m_packetSize];
	}

    inline void SendData(char* packet) {
        send(m_Sock, packet, m_packetSize, NULL);
    }

  

    std::string GetData()
    {
        recv(m_Sock, m_SzBuffer, m_packetSize, NULL);
        return m_SzBuffer;
    }

    bool IsConnected()
    {
        return m_HasConnetcion != -1;
    }

    ~SocketUtility()
    {
        closesocket(m_Sock);
        WSACleanup();
    }
};