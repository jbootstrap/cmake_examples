#include "cvwin.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <vector>
#include <windows.h>
#include <atlimage.h>
#include <gdiplus.h>

std::vector<uchar> cvwin::BitmapToPng(HBITMAP hBitmap)
{
	std::vector<uchar> png_data;
	CImage cimg;
	cimg.Attach(hBitmap);
	IStream* pIStream = NULL;
	if (CreateStreamOnHGlobal(NULL, TRUE, (LPSTREAM*)&pIStream) != S_OK)
	{
		cimg.Detach();
		DeleteObject(hBitmap);
		return png_data;
	}
	if (cimg.Save(pIStream, Gdiplus::ImageFormatPNG) != S_OK)
	{
		pIStream->Release();
		cimg.Detach();
		DeleteObject(hBitmap);
		return png_data;
	}
	cimg.Detach();
	DeleteObject(hBitmap);
	ULARGE_INTEGER ulnSize;
	LARGE_INTEGER lnOffset;
	lnOffset.QuadPart = 0;
	if (pIStream->Seek(lnOffset, STREAM_SEEK_END, &ulnSize) != S_OK)
	{
		pIStream->Release();
		return png_data;
	}
	if (pIStream->Seek(lnOffset, STREAM_SEEK_SET, NULL) != S_OK)
	{
		pIStream->Release();
		return png_data;
	}
	png_data.resize((unsigned int)ulnSize.QuadPart);
	ULONG ulBytesRead;
	if (pIStream->Read(&png_data[0], (ULONG)ulnSize.QuadPart, &ulBytesRead) != S_OK)
	{
		pIStream->Release();
		return png_data;
	}
	pIStream->Release();
	return png_data;
}

static int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0, size = 0;
	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0) return -1;
	Gdiplus::ImageCodecInfo* pImageCodecInfo = (Gdiplus::ImageCodecInfo*)malloc(size);
	if (pImageCodecInfo == NULL) return -1;
	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);
	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}
	free(pImageCodecInfo);
	return -1;
}

static std::vector<uchar> IconToImageFormat(HICON hIcon, const WCHAR* format)
{
	std::vector<uchar> png_data;
	ICONINFO ii = { 0 };
	GetIconInfo(hIcon, &ii);
	Gdiplus::Bitmap bmpIcon(ii.hbmColor, NULL);
	Gdiplus::Rect rectBounds(0, 0, bmpIcon.GetWidth(), bmpIcon.GetHeight());
	Gdiplus::BitmapData bmData;
	bmpIcon.LockBits(&rectBounds, Gdiplus::ImageLockModeRead, bmpIcon.GetPixelFormat(), &bmData);
	Gdiplus::Bitmap bmp(bmData.Width, bmData.Height, bmData.Stride, PixelFormat32bppARGB, (BYTE*)bmData.Scan0);
	bmpIcon.UnlockBits(&bmData);
	IStream* pIStream = NULL;
	if (CreateStreamOnHGlobal(NULL, TRUE, (LPSTREAM*)&pIStream) != S_OK)
	{
		return png_data;
	}
	CLSID clsid;
	if (GetEncoderClsid(format, &clsid) < 0)
	{
		pIStream->Release();
		return png_data;
	}
	bmp.Save(pIStream, &clsid, NULL);
	ULARGE_INTEGER ulnSize;
	LARGE_INTEGER lnOffset;
	lnOffset.QuadPart = 0;
	if (pIStream->Seek(lnOffset, STREAM_SEEK_END, &ulnSize) != S_OK)
	{
		pIStream->Release();
		return png_data;
	}
	if (pIStream->Seek(lnOffset, STREAM_SEEK_SET, NULL) != S_OK)
	{
		pIStream->Release();
		return png_data;
	}
	png_data.resize((unsigned int)ulnSize.QuadPart);
	ULONG ulBytesRead;
	if (pIStream->Read(&png_data[0], (ULONG)ulnSize.QuadPart, &ulBytesRead) != S_OK)
	{
		pIStream->Release();
		return png_data;
	}
	pIStream->Release();
	return png_data;
}

#if 0x0
std::vector<uchar> cvwin::IconToBmp(HICON hIcon)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	std::vector<uchar> png_data = IconToImageFormat(hIcon, L"image/bmp");
	Gdiplus::GdiplusShutdown(gdiplusToken);
	return png_data;
}
#endif

std::vector<uchar> cvwin::IconToPng(HICON hIcon)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	std::vector<uchar> png_data = IconToImageFormat(hIcon, L"image/png");
	Gdiplus::GdiplusShutdown(gdiplusToken);
	return png_data;
}

cv::Mat cvwin::BitmapToMat(HBITMAP hBitmap)
{
	std::vector<uchar> png_data = cvwin::BitmapToPng(hBitmap);
	return cv::imdecode(cv::Mat(png_data), cv::IMREAD_UNCHANGED);
}

cv::Mat cvwin::IconToMat(HICON hIcon)
{
	std::vector<uchar> png_data = cvwin::IconToPng(hIcon);
	return cv::imdecode(cv::Mat(png_data), cv::IMREAD_UNCHANGED);
}
