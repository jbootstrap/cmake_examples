#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <stdio.h>
#include <windows.h>
#include <atlimage.h>

#include "cvwin.hpp"

int main(void)
{
	HINSTANCE hInstance = ::GetModuleHandle(NULL);

	HICON hIcon = ExtractIcon(
		hInstance,                   // �C���X�^���X�̃n���h��
		L"C:\\Windows\\notepad.exe",  // �t�@�C����
		0                            // �A�C�R���̃C���f�b�N�X�ԍ�
	);
	printf("hIcon=0x%08x\n", (unsigned int)hIcon);

	std::vector<uchar> png = cvwin::IconToPng(hIcon);
	printf("png.size()=%u\n", png.size());
	std::string str;
	str.assign(png.begin(), png.end());
	printf("str.size()=%u\n", str.size());
	FILE *fp = fopen("a.png", "wb");
	fwrite(str.c_str(), str.size(), 1, fp);
	fclose(fp);

	//cv::Mat img(cv::Size(width, height), CV_8UC3);
	cv::Mat img = cvwin::IconToMat(hIcon);

	DestroyIcon(hIcon);

	// ���ʉ摜�\��
	cv::namedWindow("Image", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::imshow("Image", img);
	cv::waitKey(0);
}
