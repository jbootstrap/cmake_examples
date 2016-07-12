#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <stdio.h>
#include <windows.h>
#include <atlimage.h>
#include <iostream>
#include <fstream>

#include "cvwin.h"
using namespace cvwin;

int main(void)
{
	HINSTANCE hInstance = ::GetModuleHandle(NULL);

	HICON hIcon = ExtractIcon(
		hInstance,                   // インスタンスのハンドル
		L"C:\\Windows\\notepad.exe",  // ファイル名
		0                            // アイコンのインデックス番号
	);
	printf("hIcon=0x%08x\n", hIcon);

	//cv::Mat img(cv::Size(width, height), CV_8UC3);
	cv::Mat img = cvwin::IconToMat(hIcon);

	DestroyIcon(hIcon);

	// 結果画像表示
	cv::namedWindow("Image", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::imshow("Image", img);
	cv::waitKey(0);
}
