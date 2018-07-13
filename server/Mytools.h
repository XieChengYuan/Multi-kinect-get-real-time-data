#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Kinect.h>
using namespace cv;

class Mytools
{
public:
	Mytools();
	~Mytools();
	Mat ConvertMat_8(const UINT16* pBuffer, int nWidth, int nHeight);
	Mat ConvertMat_8(const BYTE* pBuffer, int nWidth, int nHeight);
	Mat ConvertMat_16(const UINT16* pBuffer, int nWidth, int nHeight);
	Mat ConvertMat_16(const UINT16* pBuffer, int nWidth, int nHeight, USHORT nMinDepth, USHORT nMaxDepth);
	int numOfNonZeroPixels(const BYTE *pBuffer_bodyIndex, int nWidth, int nHeight);
	std::string int2str(const int &int_temp);
	void Reverse(Mat &mat);
};
