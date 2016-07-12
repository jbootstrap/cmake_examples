#include "cvwin.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <windows.h>
#include <atlimage.h>

HBITMAP cvwin::IconToBitmap(HICON hIcon)
{
	BITMAP bm;
	ICONINFO iconInfo;
	GetIconInfo(hIcon, &iconInfo);
	GetObject(iconInfo.hbmColor, sizeof(BITMAP), &bm);
	int width = bm.bmWidth;
	int height = bm.bmHeight;
	HDC hDC = GetDC(NULL);
	HDC hMemDC = CreateCompatibleDC(hDC);
	HBITMAP hMemBmp = CreateCompatibleBitmap(hDC, width, height);
	HBITMAP hResultBmp = NULL;
	HGDIOBJ hOrgBMP = SelectObject(hMemDC, hMemBmp);
	DrawIconEx(hMemDC, 0, 0, hIcon, width, height, 0, NULL, DI_NORMAL);
	hResultBmp = hMemBmp;
	hMemBmp = NULL;
	SelectObject(hMemDC, hOrgBMP);
	DeleteDC(hMemDC);
	ReleaseDC(NULL, hDC);
	return hResultBmp;
}

std::vector<uchar> cvwin::IconToPng(HICON hIcon)
{
	HBITMAP hBitmap = cvwin::IconToBitmap(hIcon);
	std::vector<uchar> png = cvwin::BitmapToPng(hBitmap);
	DeleteObject(hBitmap);
	return png;
}

cv::Mat cvwin::IconToMat(HICON hIcon)
{
	std::vector<uchar> png = cvwin::IconToPng(hIcon);
	cv::Mat mat = cv::imdecode(cv::Mat(png), 1);
	return mat;
}

std::vector<uchar> cvwin::BitmapToPng(HBITMAP hBitmap)
{
	std::vector<uchar> buff;
	CImage cimg;
	cimg.Attach(hBitmap);
	IStream* pIStream = NULL;
	if (CreateStreamOnHGlobal(NULL, TRUE, (LPSTREAM*)&pIStream) != S_OK)
	{
		DeleteObject(hBitmap);
		return buff;
	}
	HRESULT SaveStatus = cimg.Save(pIStream, Gdiplus::ImageFormatPNG);
	if (SaveStatus != S_OK)
	{
		pIStream->Release();
		DeleteObject(hBitmap);
		return buff;
	}
	cimg.Detach();
	DeleteObject(hBitmap);
	ULARGE_INTEGER ulnSize;
	LARGE_INTEGER lnOffset;
	lnOffset.QuadPart = 0;
	if (pIStream->Seek(lnOffset, STREAM_SEEK_END, &ulnSize) != S_OK)
	{
		pIStream->Release();
		return buff;
	}
	if (pIStream->Seek(lnOffset, STREAM_SEEK_SET, NULL) != S_OK)
	{
		pIStream->Release();
		return buff;
	}
	buff.resize((unsigned int)ulnSize.QuadPart);
	ULONG ulBytesRead;
	if (pIStream->Read(&buff[0], (ULONG)ulnSize.QuadPart, &ulBytesRead) != S_OK)
	{
		pIStream->Release();
		return buff;
	}
	pIStream->Release();
	return buff;
}

cv::Mat cvwin::BitmapToMat(HBITMAP hBitmap)
{
	std::vector<uchar> png = cvwin::BitmapToPng(hBitmap);
	cv::Mat mat = cv::imdecode(cv::Mat(png), 1);
	return mat;
}
