#pragma once
#include <Winsock2.h>
#include <windows.h>
#include <iostream>
#include<string>

#pragma comment (lib, "ws2_32.lib")

#define IP_BUF_SIZE 129

class Server
{

public:
	Server();
	~Server();
	Server(const Server &) = delete;
	Server & operator=(const Server &) = delete;
	void WaitForClient(std::string data);
	std::string inData;
private:
	WORD winsock_ver;
	WSADATA wsa_data;
	SOCKET sock_svr;
	SOCKET sock_clt;
	HANDLE h_thread;
	SOCKADDR_IN addr_svr;
	SOCKADDR_IN addr_clt;
	int ret_val;
	int addr_len;
	char buf_ip[IP_BUF_SIZE];
};
