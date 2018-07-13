#include "Mytools.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include <cstdio>
#include <string>
#include <Kinect.h>



Mytools::Mytools()
{
}


Mytools::~Mytools()
{
}


std::string Mytools::int2str(const int &int_temp)
{
	std::stringstream stream;
	stream << int_temp;
	std::string string_temp = stream.str();   //此处也可以用 stream>>string_temp  
	return string_temp;
}

// 转换depth图像到cv::Mat
Mat Mytools::ConvertMat_8(const UINT16* pBuffer, int nWidth, int nHeight)
{
	Mat img(nHeight, nWidth, CV_8UC1);
	//指向头指针
	uchar* p_mat = img.data;
	//指向最后一个元素的指针
	const UINT16* pBufferEnd = pBuffer + (nWidth * nHeight);
	while (pBuffer < pBufferEnd)  //16位最大值为65536
	{
		//将16位数据转换到8位
		*p_mat = *pBuffer / 65536.0 * 256;
		pBuffer++;
		p_mat++;
	}
	return img;
}

// 转换bodyindex图像到cv::Mat
Mat Mytools::ConvertMat_8(const BYTE* pBuffer, int nWidth, int nHeight)
{
	Mat img(nHeight, nWidth, CV_8UC1);
	//指向头指针
	uchar* p_mat = img.data;
	//指向最后一个元素的指针
	const BYTE* pBufferEnd = pBuffer + (nWidth * nHeight);
	while (pBuffer < pBufferEnd)
	{
		*p_mat = *pBuffer;
		pBuffer++;
		p_mat++;
	}
	return img;
}

/// TIFF: 把 16位深度数据 从 UINT16型的数组 转换到 CV_16UC3的Mat，即16位无符号整型三通道Mat中
Mat Mytools::ConvertMat_16(const UINT16* pBuffer, int nWidth, int nHeight)
{
	Mat img(nHeight, nWidth, CV_16UC1); // rows = nHeight cols = nWidth

	for (int i = 0; i < nHeight; i++)
	{
		UINT16* data = img.ptr<UINT16>(i);
		for (int j = 0; j < nWidth;)
		{
			USHORT depth = *pBuffer << 3;
			data[j++] = depth;
			pBuffer++;
		}
	}

	return img;
}

/// TIFF: 把 16位深度数据 从 UINT16型的数组 转换到 CV_16UC3的Mat，即16位无符号整型三通道Mat中
Mat Mytools::ConvertMat_16(const UINT16* pBuffer, int nWidth, int nHeight, USHORT nMinDepth, USHORT nMaxDepth)
{
	Mat img(nHeight, nWidth, CV_16UC3); // rows = nHeight cols = nWidth

	for (int i = 0; i < nHeight; i++)
	{
		UINT16* data = img.ptr<UINT16>(i);
		for (int j = 0; j < nWidth * 3;)
		{
			USHORT depth = (*pBuffer >= nMinDepth) && (*pBuffer <= nMaxDepth) ? *pBuffer << 3 : 0;
			data[j++] = depth;
			data[j++] = depth;
			data[j++] = depth;
			pBuffer++;
		}
	}

	return img;
}

int Mytools::numOfNonZeroPixels(const BYTE *pBuffer, int nWidth, int nHeight) {
	//指向最后一个元素的指针
	const BYTE* pBufferEnd = pBuffer + (nWidth * nHeight);
	int num = 0;
	while (pBuffer < pBufferEnd)
	{
		if (*pBuffer)num++;
		pBuffer++;
	}
	return num;
}

void Mytools::Reverse(Mat &mat) {
	uchar* p_mat = mat.data;
	for (int i = 0; i < mat.rows*mat.cols; i++) {
		*p_mat = 255 - *p_mat;
		p_mat++;
	}
}