//#include "stdafx.h"
#include "SwigClass.h"
#include <windows.h>
#include <stdio.h>

static int s_class_count = 0;

SwigClass::SwigClass()
{
	s_class_count++;
	printf("SwigClass::SwigClass(): %d\n", s_class_count);
}


SwigClass::~SwigClass()
{
	s_class_count--;
	printf("SwigClass::~SwigClass(): %d\n", s_class_count);
}

void SwigClass::setBytes(std::string bytes, std::string bytes2)
{
	printf("SwigClass::setBytes(1)\n");
	for (int i = 0; i < bytes.size(); i++)
	{
		printf("SwigClass::setBytes(): bytes[%d]=0x%02x\n", i, bytes[i]);
	}
	printf("SwigClass::setBytes(): bytes=[%s]\n", bytes.c_str());
	for (int i = 0; i < bytes2.size(); i++)
	{
		printf("SwigClass::setBytes(): bytes2[%d]=0x%02x\n", i, bytes2[i]);
	}
	printf("SwigClass::setBytes(): bytes2=[%s]\n", bytes2.c_str());
	printf("SwigClass::setBytes(end)\n");
}

void SwigClass::setBytes2(std::vector<char> bytes)
{
}

std::string SwigClass::getBytes()
{
	return "abc";
}

const std::string &SwigClass::getBytes2()
{
	return "abc";
}

int SwigClass::add2(int a, int b)
{
	return a + b;
}

std::wstring SwigClass::getString()
{
	return L"returnstr漢字";
}

int SwigClass::getClassCount()
{
	return s_class_count;
}

void setString(const std::wstring &s)
{
	setlocale(LC_ALL, "Japanese");
	printf("setString(): s=%ls\n", s.c_str());
}

int getClassCount()
{
	return s_class_count;
}

std::wstring SwigClass::getUserEnvVar(const std::wstring &name)
{
	HKEY key;
	if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Environment", 0, KEY_READ, &key) != ERROR_SUCCESS)
	{
		return L"";
	}

	std::vector<wchar_t> buf(100000);
	DWORD size;
	if (RegQueryValueExW(key, name.c_str(), NULL, NULL, (LPBYTE)&buf[0], &size) != ERROR_SUCCESS)
	{
		return L"";
	}

	return &buf[0];
}

void SwigClass::setUserEnvVar(const std::wstring &name, const std::wstring &value)
{
	HKEY key;
	if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Environment", 0, KEY_WRITE, &key) != ERROR_SUCCESS)
	{
		return;
	}

	if (RegSetValueEx(
		key,                             // 親キーのハンドル
		name.c_str(),                    // レジストリエントリ名
		0,                               // 予約済み
		REG_SZ,                          // レジストリエントリのデータ型
		(CONST BYTE*)value.c_str(),       // レジストリエントリのデータ
		(value.size()+1)*sizeof(wchar_t) // レジストリエントリのデータのサイズ
		) != ERROR_SUCCESS)
	{
	}

	/* https://msdn.microsoft.com/ja-jp/library/cc429936.aspx
	LONG RegSetValueEx(
		HKEY hKey,           // 親キーのハンドル
		LPCTSTR lpValueName, // レジストリエントリ名
		DWORD Reserved,      // 予約済み
		DWORD dwType,        // レジストリエントリのデータ型
		CONST BYTE *lpData,  // レジストリエントリのデータ
		DWORD cbData         // レジストリエントリのデータのサイズ
		);

	std::vector<wchar_t> buf(100000);
	DWORD size;
	if (RegQueryValueExW(key, name.c_str(), NULL, NULL, (LPBYTE)&buf[0], &size) != ERROR_SUCCESS)
	{
		return L"";
	}
	*/
}
