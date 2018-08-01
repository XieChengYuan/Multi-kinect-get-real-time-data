#pragma once
#include <Winsock2.h>
#include <windows.h>

#pragma comment (lib, "ws2_32.lib")

#define IP_BUF_SIZE 129

class Server
{
public:
	Server();
	~Server();
	Server(const Server &) = delete;
	Server & operator=(const Server &) = delete;
	void WaitForClient();
private:
	WORD winsock_ver;
	WSADATA wsa_data;
	SOCKET sock_svr;
	SOCKET sock_clt;
	HANDLE h_thread0;
	HANDLE h_thread1;
	HANDLE h_thread2;
	SOCKADDR_IN addr_svr;
	SOCKADDR_IN addr_clt;
	int ret_val;
	int addr_len;
	char buf_ip[IP_BUF_SIZE];
};

