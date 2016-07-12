#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <windows.h>
//#include <stdio.h>
#include <string>
#include <vector>

#include "cvwin.hpp"

#pragma pack( push )
#pragma pack( 2 )
typedef struct
{
	BYTE   bWidth;               // Width, in pixels, of the image
	BYTE   bHeight;              // Height, in pixels, of the image
	BYTE   bColorCount;          // Number of colors in image (0 if >=8bpp)
	BYTE   bReserved;            // Reserved
	WORD   wPlanes;              // Color Planes
	WORD   wBitCount;            // Bits per pixel
	DWORD  dwBytesInRes;         // how many bytes in this resource?
	WORD   nID;                  // the ID
} GRPICONDIRENTRY, *LPGRPICONDIRENTRY;
#pragma pack( pop )

#pragma pack( push )
#pragma pack( 2 )
typedef struct
{
	WORD            idReserved;   // Reserved (must be 0)
	WORD            idType;       // Resource type (1 for icons)
	WORD            idCount;      // How many images?
	GRPICONDIRENTRY idEntries[1]; // The entries for each image
} GRPICONDIR, *LPGRPICONDIR;
#pragma pack( pop )

//typedef std::vector<GRPICONDIRENTRY> IconDirectory;

std::vector<GRPICONDIRENTRY> GetIconDirectory(HMODULE hMod, WORD Id) {
	HRSRC hRsrc = FindResourceW(hMod, MAKEINTRESOURCE(Id), RT_GROUP_ICON);
	HGLOBAL hGlobal = LoadResource(hMod, hRsrc);
	GRPICONDIR* lpGrpIconDir = (GRPICONDIR*)LockResource(hGlobal);
	std::vector<GRPICONDIRENTRY> dir;
	for (size_t i = 0; i < lpGrpIconDir->idCount; ++i) {
		dir.push_back(lpGrpIconDir->idEntries[i]);
	}
	return dir;
}

HICON LoadSpecificIcon(HMODULE hMod, WORD Id) {
	HRSRC hRsrc = FindResourceW(hMod, MAKEINTRESOURCE(Id), RT_ICON);
	HGLOBAL hGlobal = LoadResource(hMod, hRsrc);
	BYTE* lpData = (BYTE*)LockResource(hGlobal);
	DWORD dwSize = SizeofResource(hMod, hRsrc);
	HICON hIcon = CreateIconFromResourceEx(lpData, dwSize, TRUE, 0x00030000,
		0, 0, LR_DEFAULTCOLOR);
	return hIcon;
}

#if 0x0
struct _MyEnumResNameDataRecord
{
	// リソース ID (数値)
	UINT uID;
	// リソース ID (文字列)
	std::wstring szName;
};
#endif

struct _MyEnumResNameData
{
	//std::vector<_MyEnumResNameDataRecord> records;
	std::vector<WORD> ids;
};

BOOL CALLBACK _MyEnumResNameProc(HMODULE hModule, LPCTSTR lpszType, LPTSTR lpszName, _MyEnumResNameData* pData)
{
	//_MyEnumResNameDataRecord rec;
	if ((HIWORD(lpszName) == 0))
	{
		printf("lpszName=<%d>\n", LOWORD(lpszName));
		pData->ids.push_back(LOWORD(lpszName));
	}
	else
	{
		printf("lpszName=%ls\n", lpszName);
	}
	return TRUE;
}

int main(void)
{
	HMODULE hModule = LoadLibraryExW(
#if 0x0
		L"C:\\Windows\\notepad.exe",
#else
		L"C:\\Program Files (x86)\\Mozilla Firefox\\firefox.exe",
#endif
		NULL,
		LOAD_LIBRARY_AS_DATAFILE | LOAD_WITH_ALTERED_SEARCH_PATH);
	printf("hModule=0x%08x\n", (unsigned int)hModule);
	//EnumResourceNamesW(hModule, RT_GROUP_ICON,
	//	(ENUMRESNAMEPROC)_MyEnumResNameProc,
	//	(LPARAM)NULL);
	_MyEnumResNameData data;
	EnumResourceNamesW(hModule, RT_GROUP_ICON,
		(ENUMRESNAMEPROC)_MyEnumResNameProc,
		(LONG_PTR)&data);
	printf("data.records.size()=%u\n", data.ids.size());
	for (size_t i = 0; i < data.ids.size(); i++)
	{
		WORD id = data.ids[i];
		std::vector<GRPICONDIRENTRY> dir = GetIconDirectory(hModule, id);
		for (size_t j = 0; j < dir.size(); j++)
		{
			printf("j=%u(%u)\n", j, dir[j].bHeight);
			HICON hIcon = LoadSpecificIcon(hModule, dir[j].nID);
			if (!hIcon) continue;
			printf("hIcon=0x%08x\n", (unsigned int)hIcon);
			cv::Mat img = cvwin::IconToMat(hIcon);
			char name[256];
			sprintf(name, "Image_%03d_%03d", i, j);
			cv::namedWindow(name, CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
			cv::imshow(name, img);
			DestroyIcon(hIcon);
		}
#if 0x0
		HICON hIcon = LoadIcon(
			hModule, // アプリケーションインスタンスのハンドル
			MAKEINTRESOURCE(id)
		);
		if (!hIcon) continue;
		printf("hIcon=0x%08x\n", (unsigned int)hIcon);
		cv::Mat img = cvwin::IconToMat(hIcon);
		char name[256];
		sprintf(name, "Image_%03d", i);
		cv::namedWindow(name, CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
		cv::imshow(name, img);
		DestroyIcon(hIcon);
#endif
	}
	cv::waitKey(0);
	return 0;
}

#if 0x0
int main(void)
{
	HINSTANCE hInstance = ::GetModuleHandle(NULL);

	for (int i = 0; i < 256; i++)
	{
		HICON hIcon = ExtractIcon(
			hInstance,                   // インスタンスのハンドル
			//L"C:\\Windows\\notepad.exe", // ファイル名
			L"C:\\Program Files(x86)\\Mozilla Firefox\\firefox.exe", // ファイル名
			i                            // アイコンのインデックス番号
		);
		if (hIcon == NULL) continue;
		cv::Mat img = cvwin::IconToMat(hIcon);
		DestroyIcon(hIcon);
		//cv::imwrite("b.png", img);
		char name[256];
		sprintf(name, "Image_%03d", i);
		cv::namedWindow(name, CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
		cv::imshow(name, img);
	}

	cv::waitKey(0);
}
#endif