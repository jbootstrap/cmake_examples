#include <windows.h>

#include <iostream>
#include <memory>
#include <vector>

#include <boost/range/algorithm.hpp>

using namespace std;

class RegistryHandle;

std::shared_ptr<RegistryHandle> RegistryOpenKey(HKEY key, const std::string &subKey);

class RegistryHandle
{
public:
	HKEY key = NULL;
	RegistryHandle(HKEY aKey) : key(aKey)
	{
	}
	virtual ~RegistryHandle()
	{
		RegCloseKey(key);
	}
	std::vector<std::string> getSubKeys()
	{
		std::vector<std::string> result;

		char     achClass[MAX_PATH + 1] = "";
		DWORD    cchClassName = MAX_PATH;
		DWORD    cSubKeys = 0;
		DWORD    cbMaxSubKey;
		DWORD    cchMaxClass;
		DWORD    cValues;
		DWORD    cchMaxValue;
		DWORD    cbMaxValueData;
		DWORD    cbSecurityDescriptor;
		FILETIME ftLastWriteTime;

		if (RegQueryInfoKeyA(
			key,                     // key handle 
			achClass,                // buffer for class name 
			&cchClassName,           // size of class string 
			NULL,                    // reserved 
			&cSubKeys,               // number of subkeys 
			&cbMaxSubKey,            // longest subkey size 
			&cchMaxClass,            // longest class string 
			&cValues,                // number of values for this key 
			&cchMaxValue,            // longest value name 
			&cbMaxValueData,         // longest value data 
			&cbSecurityDescriptor,   // security descriptor 
			&ftLastWriteTime)        // last write time 
			!= ERROR_SUCCESS)
		{
			return result;
		}
		//#define MAX_KEY_LENGTH 255
		DWORD cbName;
		//char achKey[MAX_KEY_LENGTH];
		std::vector<char> achKey(cbMaxSubKey + 1);

		printf("\nNumber of subkeys: %u\n", cSubKeys);

		for (DWORD i = 0; i < cSubKeys; i++)
		{
			cbName = cbMaxSubKey;
			if (RegEnumKeyExA(key, i, &achKey[0], &cbName, NULL, NULL, NULL, &ftLastWriteTime) != ERROR_SUCCESS)
			{
				continue;
			}
			result.push_back(&achKey[0]);
		}
		return result;
	}
	std::shared_ptr<RegistryHandle> OpenKey(const std::string &subKey)
	{
		return RegistryOpenKey(key, subKey);
	}
	std::string QueryValue(const std::string &valueName)
	{
		DWORD size;
		if (RegQueryValueExA(key, valueName.c_str(), NULL, NULL, NULL, &size) != ERROR_SUCCESS)
		{
			return "";
		}
		std::vector<char> result(size + 1);
		if (RegQueryValueExA(key, valueName.c_str(), NULL, NULL, (LPBYTE)&result[0], &size) != ERROR_SUCCESS)
		{
			return "";
		}
		return &result[0];
	}
};

std::shared_ptr<RegistryHandle> RegistryOpenKey(HKEY key, const std::string &subKey)
{
	HKEY out;
	if (RegOpenKeyExA(key, subKey.c_str(), 0, KEY_ALL_ACCESS, &out) != ERROR_SUCCESS)
	{
		std::shared_ptr<RegistryHandle> result(NULL);
		return result;
	}
	std::shared_ptr<RegistryHandle> result(new RegistryHandle(out));
	return result;
}

template <typename List>
void split(const std::string& s, const std::string& delim, List& result)
{
	result.clear();

	using string = std::string;
	string::size_type pos = 0;

	while (pos != string::npos)
	{
		string::size_type p = s.find(delim, pos);

		if (p == string::npos)
		{
			result.push_back(s.substr(pos));
			break;
		}
		else {
			result.push_back(s.substr(pos, p - pos));
		}

		pos = p + delim.size();
	}
}

/* 指定したJRE BINディレクトリで client\jvm.dll または server\jvm.dll を検索します。
* jvm.dll の見つかったディレクトリを output に格納します。
* jvm.dll が見つからなかった場合は output に client のパスを格納します。
* useServerVM が TRUE の場合、Server VM を優先検索します。
* jvm.dll が見つかった場合は TRUE, 見つからなかった場合は FALSE を返します。
*/
BOOL FindJavaVM(char* output, const char* jre, BOOL useServerVM)
{
	char path[MAX_PATH];
	char buf[MAX_PATH];

	if (useServerVM)
	{
		lstrcpyA(path, jre);
		lstrcatA(path, "\\bin\\server");
		lstrcpyA(buf, path);
		lstrcatA(buf, "\\jvm.dll");
		if (GetFileAttributesA(buf) == -1)
		{
			lstrcpyA(path, jre);
			lstrcatA(path, "\\bin\\client");
			lstrcpyA(buf, path);
			lstrcatA(buf, "\\jvm.dll");
			if (GetFileAttributesA(buf) == -1)
			{
				path[0] = '\0';
			}
		}
	}
	else
	{
		lstrcpyA(path, jre);
		lstrcatA(path, "\\bin\\client");
		lstrcpyA(buf, path);
		lstrcatA(buf, "\\jvm.dll");
		if (GetFileAttributesA(buf) == -1)
		{
			lstrcpyA(path, jre);
			lstrcatA(path, "\\bin\\server");
			lstrcpyA(buf, path);
			lstrcatA(buf, "\\jvm.dll");
			if (GetFileAttributesA(buf) == -1)
			{
				path[0] = '\0';
			}
		}
	}
	if (path[0] != '\0')
	{
		lstrcpyA(output, path);
		return TRUE;
	}
	else
	{
		lstrcpyA(output, jre);
		lstrcatA(output, "\\bin\\client");
		return FALSE;
	}
}

bool FileExists(std::string filepath)
{
	if (GetFileAttributesA(filepath.c_str()) == -1) return false;
	return true;
}

std::string FindJvmDllPath(const std::string &homepath, BOOL useServerVM)
{
	std::vector<char> output(MAX_PATH + 1);
	if (FindJavaVM(&output[0], homepath.c_str(), useServerVM))
	{
		printf("&output[0]=%s\n", &output[0]);
		return std::string(&output[0]) + "\\jvm.dll";
	}
	if (FindJavaVM(&output[0], (homepath + "\\jre").c_str(), useServerVM))
	{
		printf("&output[0]=%s\n", &output[0]);
		return std::string(&output[0]) + "\\jvm.dll";
	}
	return "";
}

//void FindJavaHomeFromRegistry2(LPCSTR _subkey, std::vector<std::string> &out_homelist)
void FindJavaHomeFromRegistry2(const std::string &subkey, std::vector<std::string> &out_homelist)
{
	//std::string subkey = _subkey;
	auto key = RegistryOpenKey(HKEY_LOCAL_MACHINE, subkey);
	if (key == NULL) return;
	auto subkeys = key->getSubKeys();
	for (DWORD i = 0; i < subkeys.size(); i++)
	{
		auto key2 = key->OpenKey(subkeys[i]);
		if (key == NULL) continue;
		auto home = key2->QueryValue("JavaHome");
		if (home.empty()) continue;
		if (std::find(out_homelist.begin(), out_homelist.end(), home) == out_homelist.end()) {
			out_homelist.push_back(home);
			printf("out_homelist.size()=%u\n", out_homelist.size());
		}
	}
}

void FindJavaHomeFromMain() /**/
{
	if (getenv("JB_DEBUG")) printf("FindJavaHomeFromMain()\n");
	std::vector<std::string> subkeys =
		/*
		{
		"SOFTWARE\\Wow6432Node\\JavaSoft\\Java Development Kit",
		"SOFTWARE\\Wow6432Node\\JavaSoft\\Java Runtime Environment",
		"SOFTWARE\\JavaSoft\\Java Development Kit",
		"SOFTWARE\\JavaSoft\\Java Runtime Environment",
		};
		*/
	{
		//"SOFTWARE\\Wow6432Node\\JavaSoft\\Java Development Kit",
		//"SOFTWARE\\Wow6432Node\\JavaSoft\\Java Runtime Environment",
		"SOFTWARE\\JavaSoft\\Java Development Kit",
		"SOFTWARE\\JavaSoft\\Java Runtime Environment",
	};
	if (getenv("JB_DEBUG")) printf("FindJavaHomeFromMain()=%u\n", subkeys.size());
	std::vector<char> output(_MAX_PATH + 1);
	std::vector<std::string> out_list;
#if 0x0
	for (int i = 0; i < subkeys.size(); i++)
	{
		FindJavaHomeFromRegistry2(subkeys[i].c_str(), out_list);
	}
#else
	for (auto subkey : subkeys)
	{
		FindJavaHomeFromRegistry2(subkey, out_list);
	}
#endif
	printf("out_list.size()=%u\n", out_list.size());
	for (int i = 0; i < out_list.size(); i++)
	{
		printf("%s\n", out_list[i].c_str());
		std::string jvmdll = FindJvmDllPath(out_list[i], false);
		printf("jvmdll.c_str()=%s\n", jvmdll.c_str());

	}
}

int AdjustVersionDigit(const std::string &digit)
{
	int n = atoi(digit.c_str());
	if (n < 0) n = 0;
	if (n > 999) n = 999;
	return n;
}

void AdjustVersionString(const std::string &s, unsigned short default_digit = 0)
{
	char buff[256];
	sprintf(buff, "%u", default_digit);
	std::string default_digit_string = buff;
	printf("s.c_str()=%s\n", s.c_str());
	std::vector<std::string> split1;
	split(s, "_", split1);
	printf("split1.size()=%u\n", split1.size());
	std::string s1 = split1[0];
	printf("s1.c_str()=%s\n", s1.c_str());
	std::string s2;
	if (split1.size() >= 2)
	{
		s2 = split1[1];
	}
	else
	{
		s2 = default_digit_string;
	}
	std::vector<std::string> split2;
	split(s1, ".", split2);
	printf("split2.size()=%u\n", split2.size());
	while (split2.size() < 3)
	{
		split2.push_back(default_digit_string);
	}
	printf("split2.size()=%u\n", split2.size());
	int ver0 = AdjustVersionDigit(split2[0]);
	int ver1 = AdjustVersionDigit(split2[1]);
	int ver2 = AdjustVersionDigit(split2[2]);
	int ver3 = AdjustVersionDigit(s2);
	sprintf(buff, "%03d.%03d.%03d_%03d", ver0, ver1, ver2, ver3);
	printf("buff=%s\n", buff);
	printf("\n");
}


int
main(int argc, char *argv[])
{
	printf("argc=%d\n", argc);
	//if(argc<2) exit(1);

	if (argc >= 2)
	{
		std::string verName = argv[1];
		std::vector<std::string> list;
		split(verName, ".", list);
		for (auto s : list)
		{
			printf("[%s]\n", s.c_str());
		}
	}

	AdjustVersionString("1.8.0_92");

	AdjustVersionString("1.7", 999);

	AdjustVersionString("1.-1234", 000);

	FindJavaHomeFromMain();

	return 0;
}
