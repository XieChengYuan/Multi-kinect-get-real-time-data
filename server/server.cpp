#include "server.h"
#include <iostream>
#include <WS2tcpip.h>
#include <process.h>
#include"Mytools.h"
#pragma comment(lib, "ws2_32.lib")  

using cv::Mat;
using cv::waitKey;
using cv::imshow;
using std::cerr;
using std::cout;
using std::endl;
using std::string;


#define SERVER_PORT 4999
#define MSG_BUF_SIZE 1024

__declspec(thread)Mytools mytools;
__declspec(thread)bool no_person;

struct RevInfo {
	int height;
	int width;
	UINT pBuffer_size;
	SYSTEMTIME st;
};

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
<<<<<<< HEAD
CRITICAL_SECTION cs;					//临界区对象，锁定theData
=======
>>>>>>> cb0e88a0044eb169dae298f1ab00698b2012325b
std::string sendInfo;
__declspec(thread)RevInfo rinfo;
__declspec(thread)UINT16 pBuffer[217088];
__declspec(thread)BYTE pBuffer_color[8294400];
__declspec(thread)BYTE pBuffer_bodyindex[217088];
__declspec(thread)Mat depthImg;
__declspec(thread)Mat colorImg;
__declspec(thread)Mat bodyindexImg;
__declspec(thread)Mat erodeelement;
__declspec(thread)Mat dilateelement;
__declspec(thread)BOOL resdepth;
__declspec(thread)BOOL rescolor;
__declspec(thread)BOOL resbodyindex;
__declspec(thread)int erosion_size;
__declspec(thread)int dilation_size;
__declspec(thread)int ret_val;



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

<<<<<<< HEAD
unsigned __stdcall CreateClientThread0(LPVOID lpParameter);
unsigned int* tid0;
unsigned __stdcall CreateClientThread1(LPVOID lpParameter);
unsigned int* tid1;
unsigned __stdcall CreateClientThread2(LPVOID lpParameter);
unsigned int* tid2;
void Server::WaitForClient()
{
	//InitializeCriticalSection(&cs);
=======
unsigned __stdcall CreateClientThread(LPVOID lpParameter);
void Server::WaitForClient()
{
>>>>>>> cb0e88a0044eb169dae298f1ab00698b2012325b
	cout << "***********************************************************" << endl;
	cout << "[帮助]：【D】深度【C】彩色【B】人体索引【BCD】此三种         " << endl;
	cout << "      【BD】深度+人体索引【CD】深度+彩色【BC】人体索引+彩色" << endl;
	cout << "***********************************************************" << endl;
	cout << "输入所需数据:";
	while (true) {
		std::cin >> sendInfo;
		if ((sendInfo == "D") | (sendInfo == "B") | (sendInfo == "C") |
			(sendInfo == "BCD") | (sendInfo == "BD") | (sendInfo == "CD") |
			(sendInfo == "BC"))
			break;
		else
			cout << "输入错误，请严格按照[帮助]中输入(区分大小写)" << endl;
	}
<<<<<<< HEAD
	int Flag = 0;
=======
	
>>>>>>> cb0e88a0044eb169dae298f1ab00698b2012325b
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
<<<<<<< HEAD
		if (Flag == 0)
=======
		::InetNtop(addr_clt.sin_family, &addr_clt, (PWSTR)buf_ip, IP_BUF_SIZE);
		cout << "A new client connected...IP address: " << buf_ip << ", port number: " << ::ntohs(addr_clt.sin_port) << endl;
		h_thread = (HANDLE)_beginthreadex(nullptr, 0, CreateClientThread, (LPVOID)sock_clt, 0, nullptr);
		if (h_thread == NULL)
>>>>>>> cb0e88a0044eb169dae298f1ab00698b2012325b
		{
			::InetNtop(addr_clt.sin_family, &addr_clt, (PWSTR)buf_ip, IP_BUF_SIZE);
			cout << "A new client connected...IP address: " << buf_ip << ", port number: " << ::ntohs(addr_clt.sin_port) << endl;
			h_thread0 = (HANDLE)_beginthreadex(nullptr, 0, CreateClientThread0, (LPVOID)sock_clt, 0,tid0);
			if (h_thread0 == NULL)
			{
				cerr << "Failed to create a new thread!Error code: " << ::WSAGetLastError() << "\n";
				::WSACleanup();
				system("pause");
				exit(1);
			}
			::CloseHandle(h_thread0);
			
		}
		else if (Flag == 1)
		{
			::InetNtop(addr_clt.sin_family, &addr_clt, (PWSTR)buf_ip, IP_BUF_SIZE);
			cout << "A new client connected...IP address: " << buf_ip << ", port number: " << ::ntohs(addr_clt.sin_port) << endl;
			h_thread1 = (HANDLE)_beginthreadex(nullptr, 0, CreateClientThread1, (LPVOID)sock_clt, 0, tid1);
			if (h_thread1 == NULL)
			{
				cerr << "Failed to create a new thread!Error code: " << ::WSAGetLastError() << "\n";
				::WSACleanup();
				system("pause");
				exit(1);
			}
			::CloseHandle(h_thread1);
			
		}
		else
		{
			::InetNtop(addr_clt.sin_family, &addr_clt, (PWSTR)buf_ip, IP_BUF_SIZE);
			cout << "A new client connected...IP address: " << buf_ip << ", port number: " << ::ntohs(addr_clt.sin_port) << endl;
			h_thread2 = (HANDLE)_beginthreadex(nullptr, 0, CreateClientThread2, (LPVOID)sock_clt, 0, tid2);
			if (h_thread2 == NULL)
			{
				cerr << "Failed to create a new thread!Error code: " << ::WSAGetLastError() << "\n";
				::WSACleanup();
				system("pause");
				exit(1);
			}
			::CloseHandle(h_thread2);
		}
		cout << Flag << endl;
		Flag++;
	}
}

<<<<<<< HEAD
unsigned __stdcall CreateClientThread0(LPVOID lpParameter)
{
	//EnterCriticalSection(&cs);	//进入临界区

	SOCKET sock_clt = (SOCKET)lpParameter;
	send(sock_clt, (char*)&sendInfo, sizeof(sendInfo), 0);
	string::size_type B_idx = sendInfo.find("B");
	string::size_type C_idx = sendInfo.find("C");
	string::size_type D_idx = sendInfo.find("D");
	//LeaveCriticalSection(&cs);
	while (true)
	{

#pragma region 接收深度数据
		if (D_idx != string::npos)
		{
			memset(&rinfo, 0, sizeof(rinfo));//清空结构体
			recv(sock_clt, (char*)&rinfo, sizeof(rinfo), 0); // 接收端的数据 
			std::cout << GetCurrentThreadId() << ":从客户端接收到：" << rinfo.height << " " << rinfo.width << " " << rinfo.pBuffer_size << std::endl;
			BOOL resdepth = RecvAll(sock_clt, (char*)pBuffer, rinfo.pBuffer_size);
			std::cout << "是否确实接收到（1为真）" << resdepth << std::endl;
			Mat depthImg = mytools.ConvertMat_8(pBuffer, rinfo.width, rinfo.height);
			cvNamedWindow("depth0");
			cv::imshow("depth0", depthImg);
			if (waitKey(33) == VK_ESCAPE) break;
			depthImg.release();
			memset(pBuffer, 0, sizeof(pBuffer));
		}
#pragma endregion



#pragma region 接收彩色数据
		if (C_idx != string::npos)
		{
			memset(&rinfo, 0, sizeof(rinfo));//清空结构体
			recv(sock_clt, (char*)&rinfo, sizeof(rinfo), 0); // 接收端的数据 
			std::cout << GetCurrentThreadId() << ":从客户端接收到：" << rinfo.height << " " << rinfo.width << " " << rinfo.pBuffer_size << std::endl;
			BOOL rescolor = RecvAll(sock_clt, (char*)pBuffer_color, rinfo.pBuffer_size);
			std::cout << "是否确实接收到（1为真）" << rescolor << std::endl;
			Mat colorImg(rinfo.height, rinfo.width, CV_8UC4);
			colorImg.data = pBuffer_color;
			colorImg.step = rinfo.pBuffer_size / colorImg.rows;
			std::cout << colorImg.step << std::endl;
			cvNamedWindow("color0");
			cv::imshow("color0", colorImg);
			if (waitKey(33) == VK_ESCAPE) break;
			colorImg.release();
			memset(pBuffer_color, 0, sizeof(pBuffer_color));
		}
#pragma endregion


#pragma region 接收人体索引数据
		if (B_idx != string::npos)
		{
			memset(&rinfo, 0, sizeof(rinfo));//清空结构体
			recv(sock_clt, (char*)&rinfo, sizeof(rinfo), 0); // 接收端的数据 
			std::cout << GetCurrentThreadId() << ":从客户端接收到：" << rinfo.height << " " << rinfo.width << " " << rinfo.pBuffer_size << std::endl;
			BOOL resbodyindex = RecvAll(sock_clt, (char*)pBuffer_bodyindex, rinfo.pBuffer_size);
			std::cout << "是否确实接收到（1为真）" << resbodyindex << std::endl;
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
			cvNamedWindow("bodyindex0");
			cv::imshow("bodyindex0", bodyindexImg);
			if (waitKey(33) == VK_ESCAPE) break;
			bodyindexImg.release();
			memset(pBuffer_bodyindex, 0, sizeof(pBuffer_bodyindex));
		}
#pragma endregion
	}


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
unsigned __stdcall CreateClientThread1(LPVOID lpParameter)
=======
unsigned __stdcall CreateClientThread(LPVOID lpParameter)
>>>>>>> cb0e88a0044eb169dae298f1ab00698b2012325b
{
	//EnterCriticalSection(&cs);	//进入临界区

	SOCKET sock_clt = (SOCKET)lpParameter;
	send(sock_clt, (char*)&sendInfo, sizeof(sendInfo), 0);
	string::size_type B_idx = sendInfo.find("B");
	string::size_type C_idx = sendInfo.find("C");
	string::size_type D_idx = sendInfo.find("D");
<<<<<<< HEAD
	//LeaveCriticalSection(&cs);
=======
>>>>>>> cb0e88a0044eb169dae298f1ab00698b2012325b
	while (true)
	{

#pragma region 接收深度数据
		if (D_idx != string::npos)
		{
			memset(&rinfo, 0, sizeof(rinfo));//清空结构体
			recv(sock_clt, (char*)&rinfo, sizeof(rinfo), 0); // 接收端的数据 
			std::cout << GetCurrentThreadId() << ":从客户端接收到：" << rinfo.height << " " << rinfo.width << " " << rinfo.pBuffer_size << std::endl;
			BOOL resdepth = RecvAll(sock_clt, (char*)pBuffer, rinfo.pBuffer_size);
<<<<<<< HEAD
			std::cout << "是否确实接收到（1为真）" << resdepth << std::endl;
			Mat depthImg = mytools.ConvertMat_8(pBuffer, rinfo.width, rinfo.height);
			cvNamedWindow("depth1");
			cv::imshow("depth1", depthImg);
=======
			std::cout << "是否确实接收到（1为真）"<< resdepth << std::endl;
			Mat depthImg = mytools.ConvertMat_8(pBuffer, rinfo.width, rinfo.height);
			cv::imshow("depth", depthImg);
>>>>>>> cb0e88a0044eb169dae298f1ab00698b2012325b
			if (waitKey(33) == VK_ESCAPE) break;
			depthImg.release();
			memset(pBuffer, 0, sizeof(pBuffer));
		}
#pragma endregion



#pragma region 接收彩色数据
		if (C_idx != string::npos)
		{
			memset(&rinfo, 0, sizeof(rinfo));//清空结构体
			recv(sock_clt, (char*)&rinfo, sizeof(rinfo), 0); // 接收端的数据 
			std::cout << GetCurrentThreadId() << ":从客户端接收到：" << rinfo.height << " " << rinfo.width << " " << rinfo.pBuffer_size << std::endl;
			BOOL rescolor = RecvAll(sock_clt, (char*)pBuffer_color, rinfo.pBuffer_size);
			std::cout << "是否确实接收到（1为真）" << rescolor << std::endl;
			Mat colorImg(rinfo.height, rinfo.width, CV_8UC4);
			colorImg.data = pBuffer_color;
			colorImg.step = rinfo.pBuffer_size / colorImg.rows;
			std::cout << colorImg.step << std::endl;
<<<<<<< HEAD
			cvNamedWindow("color1");
			cv::imshow("color1", colorImg);
=======
			cv::imshow("color", colorImg);
>>>>>>> cb0e88a0044eb169dae298f1ab00698b2012325b
			if (waitKey(33) == VK_ESCAPE) break;
			colorImg.release();
			memset(pBuffer_color, 0, sizeof(pBuffer_color));
		}
#pragma endregion


#pragma region 接收人体索引数据
<<<<<<< HEAD
		if (B_idx != string::npos)
=======
		if (B_idx != string::npos) 
>>>>>>> cb0e88a0044eb169dae298f1ab00698b2012325b
		{
			memset(&rinfo, 0, sizeof(rinfo));//清空结构体
			recv(sock_clt, (char*)&rinfo, sizeof(rinfo), 0); // 接收端的数据 
			std::cout << GetCurrentThreadId() << ":从客户端接收到：" << rinfo.height << " " << rinfo.width << " " << rinfo.pBuffer_size << std::endl;
			BOOL resbodyindex = RecvAll(sock_clt, (char*)pBuffer_bodyindex, rinfo.pBuffer_size);
			std::cout << "是否确实接收到（1为真）" << resbodyindex << std::endl;
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
<<<<<<< HEAD

				//pBodyIndexFrame->Release();
				//continue;

=======

				//pBodyIndexFrame->Release();
				//continue;

>>>>>>> cb0e88a0044eb169dae298f1ab00698b2012325b
			}
			else {
				no_person = 0;
			}
<<<<<<< HEAD
			cvNamedWindow("bodyindex1");
			cv::imshow("bodyindex1", bodyindexImg);
			if (waitKey(33) == VK_ESCAPE) break;
			bodyindexImg.release();
			memset(pBuffer_bodyindex, 0, sizeof(pBuffer_bodyindex));
		}
#pragma endregion
	}


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
unsigned __stdcall CreateClientThread2(LPVOID lpParameter)
{
	//EnterCriticalSection(&cs);	//进入临界区

	SOCKET sock_clt = (SOCKET)lpParameter;
	send(sock_clt, (char*)&sendInfo, sizeof(sendInfo), 0);
	string::size_type B_idx = sendInfo.find("B");
	string::size_type C_idx = sendInfo.find("C");
	string::size_type D_idx = sendInfo.find("D");
	//LeaveCriticalSection(&cs);
	while (true)
	{

#pragma region 接收深度数据
		if (D_idx != string::npos)
		{
			memset(&rinfo, 0, sizeof(rinfo));//清空结构体
			recv(sock_clt, (char*)&rinfo, sizeof(rinfo), 0); // 接收端的数据 
			std::cout << GetCurrentThreadId() << ":从客户端接收到：" << rinfo.height << " " << rinfo.width << " " << rinfo.pBuffer_size << std::endl;
			BOOL resdepth = RecvAll(sock_clt, (char*)pBuffer, rinfo.pBuffer_size);
			std::cout << "是否确实接收到（1为真）" << resdepth << std::endl;
			Mat depthImg = mytools.ConvertMat_8(pBuffer, rinfo.width, rinfo.height);
			cvNamedWindow("depth2");
			cv::imshow("depth2", depthImg);
			if (waitKey(33) == VK_ESCAPE) break;
			depthImg.release();
			memset(pBuffer, 0, sizeof(pBuffer));
		}
#pragma endregion



#pragma region 接收彩色数据
		if (C_idx != string::npos)
		{
			memset(&rinfo, 0, sizeof(rinfo));//清空结构体
			recv(sock_clt, (char*)&rinfo, sizeof(rinfo), 0); // 接收端的数据 
			std::cout << GetCurrentThreadId() << ":从客户端接收到：" << rinfo.height << " " << rinfo.width << " " << rinfo.pBuffer_size << std::endl;
			BOOL rescolor = RecvAll(sock_clt, (char*)pBuffer_color, rinfo.pBuffer_size);
			std::cout << "是否确实接收到（1为真）" << rescolor << std::endl;
			Mat colorImg(rinfo.height, rinfo.width, CV_8UC4);
			colorImg.data = pBuffer_color;
			colorImg.step = rinfo.pBuffer_size / colorImg.rows;
			std::cout << colorImg.step << std::endl;
			cvNamedWindow("color2");
			cv::imshow("color2", colorImg);
			if (waitKey(33) == VK_ESCAPE) break;
			colorImg.release();
			memset(pBuffer_color, 0, sizeof(pBuffer_color));
		}
#pragma endregion


#pragma region 接收人体索引数据
		if (B_idx != string::npos)
		{
			memset(&rinfo, 0, sizeof(rinfo));//清空结构体
			recv(sock_clt, (char*)&rinfo, sizeof(rinfo), 0); // 接收端的数据 
			std::cout << GetCurrentThreadId() << ":从客户端接收到：" << rinfo.height << " " << rinfo.width << " " << rinfo.pBuffer_size << std::endl;
			BOOL resbodyindex = RecvAll(sock_clt, (char*)pBuffer_bodyindex, rinfo.pBuffer_size);
			std::cout << "是否确实接收到（1为真）" << resbodyindex << std::endl;
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
			cvNamedWindow("bodyindex2");
			cv::imshow("bodyindex2", bodyindexImg);
			if (waitKey(33) == VK_ESCAPE) break;
			bodyindexImg.release();
			memset(pBuffer_bodyindex, 0, sizeof(pBuffer_bodyindex));
		}
#pragma endregion
=======

			cv::imshow("bodyindex", bodyindexImg);
			if (waitKey(33) == VK_ESCAPE) break;
			bodyindexImg.release();
			memset(pBuffer_bodyindex, 0, sizeof(pBuffer_bodyindex));
		}
#pragma endregion

>>>>>>> cb0e88a0044eb169dae298f1ab00698b2012325b
	}


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

