#include <iostream>
#include<atlstr.h>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include "opencv2/highgui.hpp"
#include <Kinect.h>  
#include <windows.h> 
#pragma comment(lib, "ws2_32.lib")
#pragma comment ( lib, "kinect20.lib" ) 

#pragma region using 
using std::cin;
using std::cerr;
using std::cout;
using std::endl;
using std::flush;
using cv::Mat;
using cv::waitKey;
using std::string;
#pragma endregion

const char DEFAULT_PORT[] = "4999";
const int SEND_BUF_SIZE = 256;

struct SendInfo {
	int height;
	int width;
	UINT pBuffer_size;
	SYSTEMTIME st;
}sinfo;


int main() {

	system("title 客户端2");//设置cmd窗口标题
	system("color 0B");
#pragma region 初始化kinect
	IKinectSensor*          m_pKinectSensor;
	IDepthFrameSource*      pDepthFrameSource;
	IDepthFrameReader*      m_pDepthFrameReader;
	IColorFrameSource*      pColorFrameSource;
	IColorFrameReader*      m_pColorFrameReader;
	IFrameDescription*      depthFrameDescription;
	IFrameDescription*      colorFrameDescription;
	IFrameDescription*      bodyIndexFrameDescription;
	ColorImageFormat        imageFormat = ColorImageFormat_None;
	ICoordinateMapper*      coordinateMapper;
	IBodyIndexFrameSource*		pBodyIndexFrameSource;
	IBodyIndexFrameReader*		m_pBodyIndexFrameReader;
	GetDefaultKinectSensor(&m_pKinectSensor);      //获取默认kinect传感器
	printf("打开kinect传感器成功\n");
	//打开传感器
	m_pKinectSensor->Open();
	//获得深度信息传感器
	m_pKinectSensor->get_DepthFrameSource(&pDepthFrameSource);
	//获得彩色信息传感器  
	m_pKinectSensor->get_ColorFrameSource(&pColorFrameSource);
	//人体索引信息传感器
	m_pKinectSensor->get_BodyIndexFrameSource(&pBodyIndexFrameSource);
	//打开深度信息帧读取器
	pDepthFrameSource->OpenReader(&m_pDepthFrameReader);
	//打开彩色信息帧读取器  
	pColorFrameSource->OpenReader(&m_pColorFrameReader);
	//打开人体索引
	pBodyIndexFrameSource->OpenReader(&m_pBodyIndexFrameReader);
#pragma endregion

#pragma region 设置客户端
	WSADATA wsa_data; //WSADATA变量,包含windows socket执行的信息
	int i_result = 0; //接收返回值
	SOCKET sock_client = INVALID_SOCKET;
	addrinfo *result = nullptr, hints;
	//初始化winsock动态库(ws2_32.dll),MAKEWORD(2, 2)用于请求使用winsock2.2版本
	i_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (i_result != 0) {
		cerr << "WSAStartup() function failed: " << i_result << "\n";
		system("pause");
		return 1;
	}
	SecureZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	i_result = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
	if (i_result != 0) {
		cerr << "getaddrinfo() function failed with error: " << WSAGetLastError() << "\n";
		WSACleanup();
		system("pause");
		return 1;
	}
	//创建套接字
	sock_client = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (sock_client == INVALID_SOCKET) {
		cerr << "socket() function failed with error: " << WSAGetLastError() << "\n";
		WSACleanup();
		system("pause");
		return 1;
	}
	//连接服务器
	i_result = connect(sock_client, result->ai_addr, result->ai_addrlen);
	if (i_result == SOCKET_ERROR) {
		cerr << "connect() function failed with error: " << WSAGetLastError() << "\n";
		WSACleanup();
		system("pause");
		return 1;
	}
	cout << "connect server successfully..." << endl;
	cout << "start to send data..." << endl;
	freeaddrinfo(result);
	std::string Recv;
	memset(&Recv, 0, sizeof(Recv)); 
	recv(sock_client, (char*)&Recv, sizeof(Recv), 0);
	cout << "接收到指令：" << Recv << endl;
	cout << "开始发送数据" << endl;
#pragma endregion
	SYSTEMTIME local_time = { 0 };
	while (true)
	{
#pragma region 获取图像
		IDepthFrame*       pDepthFrame = NULL;
		TIMESPAN*         Depth_relativeTime = NULL;
		IColorFrame*       pColorFrame = NULL;
		TIMESPAN*          Color_relativeTime = NULL;
		IBodyIndexFrame*   pBodyIndexFrame = NULL;
		TIMESPAN*          BodyIndex_relativeTime = NULL;
		string::size_type B_idx = Recv.find("B");
		string::size_type C_idx = Recv.find("C");
		string::size_type D_idx = Recv.find("D");
#pragma endregion

#pragma region 发送深度数据
		if (D_idx != string::npos)
		{
			//获取深度图像
			while (pDepthFrame == NULL) {
				m_pDepthFrameReader->AcquireLatestFrame(&pDepthFrame);
			}
			GetLocalTime(&local_time);
			_tprintf(_T("The local time is\t: %02d:%02d:%02d.%03d"), local_time.wHour, local_time.wMinute, local_time.wSecond, local_time.wMilliseconds);
			//pDepthFrame->get_RelativeTime(Depth_relativeTime);
			//cout << Depth_relativeTime;
			pDepthFrame->get_FrameDescription(&depthFrameDescription); //获取帧的像素信息（宽和高）
			memset(&sinfo, 0, sizeof(sinfo));
			depthFrameDescription->get_Width(&sinfo.width);
			depthFrameDescription->get_Height(&sinfo.height);
			UINT16 *pBuffer_depth = NULL;
			//获取图像像素个数和指向图像的指针
			pDepthFrame->AccessUnderlyingBuffer(&sinfo.pBuffer_size, &pBuffer_depth);
			send(sock_client, (char*)&sinfo, sizeof(sinfo), 0);
			int resdepth = send(sock_client, (char*)pBuffer_depth, sinfo.pBuffer_size, 0);
			std::cout << "已发送：" << sinfo.height << " " << sinfo.width << " " << resdepth << std::endl;
			if (waitKey(33) == VK_ESCAPE) break;
			pDepthFrame->Release();
		}
#pragma endregion


#pragma region 发送彩色数据
		if (C_idx != string::npos)
		{
			//获取彩色图像
			while (pColorFrame == NULL) {
				m_pColorFrameReader->AcquireLatestFrame(&pColorFrame);
			}
			GetLocalTime(&local_time);
			_tprintf(_T("The local time is\t: %02d:%02d:%02d.%03d"), local_time.wHour, local_time.wMinute, local_time.wSecond, local_time.wMilliseconds);
			//pColorFrame->get_RelativeTime(Color_relativeTime);
			//cout << Color_relativeTime;
			pColorFrame->get_FrameDescription(&colorFrameDescription);
			memset(&sinfo, 0, sizeof(sinfo)); 
			colorFrameDescription->get_Width(&sinfo.width);
			colorFrameDescription->get_Height(&sinfo.height);
			uchar *pBuffer_color = NULL;
			//获取图像像素个数和指向图像的指针
			pColorFrame->get_RawColorImageFormat(&imageFormat);
			Mat colorImg(sinfo.height, sinfo.width, CV_8UC4);
			pBuffer_color = colorImg.data;
			sinfo.pBuffer_size = colorImg.rows*colorImg.step;
			pColorFrame->CopyConvertedFrameDataToArray(sinfo.pBuffer_size, reinterpret_cast<BYTE*>(pBuffer_color), ColorImageFormat_Bgra);
			send(sock_client, (char*)&sinfo, sizeof(sinfo), 0);
			int rescolor = send(sock_client, (char*)pBuffer_color, sinfo.pBuffer_size, 0);
			std::cout << "已发送：" << sinfo.height << " " << sinfo.width << " " << rescolor << std::endl;
			if (waitKey(33) == VK_ESCAPE) break;
			pColorFrame->Release();
		}
#pragma endregion

#pragma region 发送人体索引数据
		if (B_idx != string::npos)
		{
			//获取人体索引
			while (pBodyIndexFrame == NULL) {
				m_pBodyIndexFrameReader->AcquireLatestFrame(&pBodyIndexFrame);
			}
			GetLocalTime(&local_time);
			_tprintf(_T("The local time is\t: %02d:%02d:%02d.%03d"), local_time.wHour, local_time.wMinute, local_time.wSecond, local_time.wMilliseconds);
			//pBodyIndexFrame->get_RelativeTime(BodyIndex_relativeTime);
			//cout << BodyIndex_relativeTime;
			pBodyIndexFrame->get_FrameDescription(&bodyIndexFrameDescription);
			memset(&sinfo, 0, sizeof(sinfo)); 
			bodyIndexFrameDescription->get_Width(&sinfo.width);
			bodyIndexFrameDescription->get_Height(&sinfo.height);
			BYTE *pBuffer_bodyIndex = NULL;
			pBodyIndexFrame->AccessUnderlyingBuffer(&sinfo.pBuffer_size, &pBuffer_bodyIndex);
			send(sock_client, (char*)&sinfo, sizeof(sinfo), 0);
			int resbodyindex = send(sock_client, (char*)pBuffer_bodyIndex, sinfo.pBuffer_size, 0);
			std::cout << "已发送：" << sinfo.height << " " << sinfo.width << " " << resbodyindex << std::endl;
			if (waitKey(33) == VK_ESCAPE) break;
			pBodyIndexFrame->Release();
		}
#pragma endregion
	}
	if (m_pKinectSensor)
	{
		m_pKinectSensor->Close();
	}
	closesocket(sock_client);   //关闭套接字  
	WSACleanup();           //释放套接字资源;  
	system("pause");
	return 0;
}