#include "server.h"
#include <iostream>
#include <WS2tcpip.h>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui.hpp"
#include <Kinect.h> 
#include <windows.h>  
#include"Mytools.h"
#pragma comment(lib, "ws2_32.lib")  

using cv::Mat;
using cv::waitKey;
using cv::imshow;
using std::cerr;
using std::cout;
using std::endl;

Mytools mytools;
bool no_person;

struct RevInfo {
	int height;
	int width;
	UINT pBuffer_size;
	SYSTEMTIME st;
}rinfo;

bool RecvAll(SOCKET &sock, char*buffer, int size)
{
	while (size>0)//剩余部分大于0
	{
		int RecvSize = recv(sock, buffer, size, 0);
		if (SOCKET_ERROR == RecvSize)
			return false;
		size = size - RecvSize;
		buffer += RecvSize;
	}
	return true;
}


#define SERVER_PORT 4999
#define MSG_BUF_SIZE 1024

Server::Server()
{
	cout << "Initializing server...\n";
	//
	winsock_ver = MAKEWORD(2, 2);
	addr_len = sizeof(SOCKADDR_IN);
	addr_svr.sin_family = AF_INET;
	addr_svr.sin_port = ::htons(SERVER_PORT);
	addr_svr.sin_addr.S_un.S_addr = ADDR_ANY;
	memset(buf_ip, 0, IP_BUF_SIZE);
	//
	ret_val = ::WSAStartup(winsock_ver, &wsa_data);
	if (ret_val != 0)
	{
		cerr << "WSA failed to start up!Error code: " << ::WSAGetLastError() << "\n";
		system("pause");
		exit(1);
	}
	cout << "WSA started up successfully...\n";
	//
	sock_svr = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock_svr == INVALID_SOCKET)
	{
		cerr << "Failed to create server socket!Error code: " << ::WSAGetLastError() << "\n";
		::WSACleanup();
		system("pause");
		exit(1);
	}
	cout << "Server socket created successfully...\n";
	//
	ret_val = ::bind(sock_svr, (SOCKADDR*)&addr_svr, addr_len);
	if (ret_val != 0)
	{
		cerr << "Failed to bind server socket!Error code: " << ::WSAGetLastError() << "\n";
		::WSACleanup();
		system("pause");
		exit(1);
	}
	cout << "Server socket bound successfully...\n";
	//
	ret_val = ::listen(sock_svr, SOMAXCONN);
	if (ret_val == SOCKET_ERROR)
	{
		cerr << "Server socket failed to listen!Error code: " << ::WSAGetLastError() << "\n";
		::WSACleanup();
		system("pause");
		exit(1);
	}
	cout << "Server socket started to listen...\n";
	//
	cout << "Server started successfully..." << endl;
}

Server::~Server()
{
	::closesocket(sock_svr);
	::closesocket(sock_clt);
	::WSACleanup();
	cout << "Socket closed..." << endl;
}

DWORD WINAPI CreateClientThread(LPVOID lpParameter);
void Server::WaitForClient()
{
	while (true)
	{
		sock_clt = ::accept(sock_svr, (SOCKADDR*)&addr_clt, &addr_len);
		if (sock_clt == INVALID_SOCKET)
		{
			cerr << "Failed to accept client!Error code: " << ::WSAGetLastError() << "\n";
			::WSACleanup();
			system("pause");
			exit(1);
		}
		::InetNtop(addr_clt.sin_family, &addr_clt, (PWSTR)buf_ip, IP_BUF_SIZE);
		cout << "A new client connected...IP address: " << buf_ip << ", port number: " << ::ntohs(addr_clt.sin_port) << endl;
		h_thread = ::CreateThread(nullptr, 0, CreateClientThread, (LPVOID)sock_clt, 0, nullptr);
		if (h_thread == NULL)
		{
			cerr << "Failed to create a new thread!Error code: " << ::WSAGetLastError() << "\n";
			::WSACleanup();
			system("pause");
			exit(1);
		}
		::CloseHandle(h_thread);
	}
}

DWORD WINAPI CreateClientThread(LPVOID lpParameter)
{
	UINT16* pBuffer = new UINT16[217088];
	BYTE* pBuffer_color = new BYTE[8294400];
	BYTE* pBuffer_bodyindex = new BYTE[217088];
	SOCKET sock_clt = (SOCKET)lpParameter;
	while (true)
	{

#pragma region 接收深度数据
		memset(&rinfo, 0, sizeof(rinfo));//清空结构体
		recv(sock_clt, (char*)&rinfo, sizeof(rinfo), 0); // 接收端的数据 
		std::cout << "从客户端接收到：" << rinfo.height << " " << rinfo.width << " " << rinfo.pBuffer_size << std::endl;
		BOOL resdepth = RecvAll(sock_clt, (char*)pBuffer, rinfo.pBuffer_size);
		std::cout << resdepth << std::endl;
		Mat depthImg = mytools.ConvertMat_8(pBuffer, rinfo.width, rinfo.height);
#pragma endregion



#pragma region 接收彩色数据
		memset(&rinfo, 0, sizeof(rinfo));//清空结构体
		recv(sock_clt, (char*)&rinfo, sizeof(rinfo), 0); // 接收端的数据 
		std::cout << "从客户端接收到：" << rinfo.height << " " << rinfo.width << " " << rinfo.pBuffer_size << std::endl;
		BOOL rescolor = RecvAll(sock_clt, (char*)pBuffer_color, rinfo.pBuffer_size);
		std::cout << rescolor << std::endl;
		Mat colorImg(rinfo.height, rinfo.width, CV_8UC4);
		colorImg.data = pBuffer_color;
		colorImg.step = rinfo.pBuffer_size / colorImg.rows;
		std::cout << colorImg.step << std::endl;
#pragma endregion


#pragma region 接收人体索引数据
		memset(&rinfo, 0, sizeof(rinfo));//清空结构体
		recv(sock_clt, (char*)&rinfo, sizeof(rinfo), 0); // 接收端的数据 
		std::cout << "从客户端接收到：" << rinfo.height << " " << rinfo.width << " " << rinfo.pBuffer_size << std::endl;
		BOOL resbodyindex = RecvAll(sock_clt, (char*)pBuffer_bodyindex, rinfo.pBuffer_size);
		std::cout << resbodyindex << std::endl;
		Mat bodyindexImg = mytools.ConvertMat_8(pBuffer_bodyindex, rinfo.width, rinfo.height);
		mytools.Reverse(bodyindexImg);
		int erosion_size = 3;
		Mat erodeelement = getStructuringElement(MORPH_ELLIPSE,
			Size(2 * erosion_size + 1, 2 * erosion_size + 1),
			Point(erosion_size, erosion_size));
		int dilation_size = 3;
		Mat dilateelement = getStructuringElement(MORPH_ELLIPSE,
			Size(2 * dilation_size + 1, 2 * dilation_size + 1),
			Point(dilation_size, dilation_size));
		cv::erode(bodyindexImg, bodyindexImg, erodeelement);
		cv::dilate(bodyindexImg, bodyindexImg, dilateelement);
		//printf("Begin Counting\n");
		if (mytools.numOfNonZeroPixels(pBuffer_bodyindex, rinfo.width, rinfo.height) < 100) {
			//没人就不算了
			no_person = 1;
			printf("No person!!\n");

			//pBodyIndexFrame->Release();
			//continue;

		}
		else {
			no_person = 0;
		}
#pragma endregion

#pragma	region 显示和清除
		cv::imshow("depth", depthImg);
		if (waitKey(33) == VK_ESCAPE) break;
		cv::imshow("color", colorImg);
		if (waitKey(33) == VK_ESCAPE) break;
		cv::imshow("bodyindex", bodyindexImg);
		if (waitKey(33) == VK_ESCAPE) break;
		depthImg.release();
		memset(pBuffer, 0, sizeof(pBuffer));
		colorImg.release();
		memset(pBuffer_color, 0, sizeof(pBuffer_color));
		bodyindexImg.release();
		memset(pBuffer_bodyindex, 0, sizeof(pBuffer_bodyindex));
#pragma endregion
		Sleep(300);
	}
	delete[] pBuffer;
	delete[] pBuffer_color;
	delete[] pBuffer_bodyindex;

	int ret_val = ::shutdown(sock_clt, SD_SEND);
	if (ret_val == SOCKET_ERROR)
	{
		cerr << "Failed to shutdown the client socket!Error code: " << ::GetLastError() << "\n";
		::closesocket(sock_clt);
		system("pause");
		return 1;
	}
	return 0;
}