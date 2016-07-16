#include <windows.h>
#include <stdio.h>
#include <locale.h>

static HICON MyExtractIcon(LPCTSTR lpszPathName, int nIndexOrID, bool bSmallIcon);

int wmain()
{
	printf("hello\n");
	MyExtractIcon(L"C:/Windows/notepad.exe", 0, false);
	return 0;
}

//グループアイコン内の全アイコンデータを取得する
//　　　HANDLE  hFile   モジュールハンドル
//　　　LPTSTR  lpStr   リソースの名前
//　　　=>PBYTE         データバッファアドレス（要:GlobalFree）

DWORD GetGroupIconData(HMODULE hFile, LPTSTR lpStr, PBYTE *pRetMem)
{
	HRSRC           hGIcon;
	HGLOBAL         hLoadGIcon;
	DWORD           AllSize;
	PBYTE           pMem;
#if 0x0
	LPICONRESHEAD   lpIconResHead;
#endif

	*pRetMem = NULL;

	//グループアイコンリソースを検索
	hGIcon = FindResource(hFile, lpStr, RT_GROUP_ICON);
	if (hGIcon == NULL)
	{
		printf("NOT FOUND\n");
		return 0;
	}
	else
	{
		printf("FOUND\n");
	}

	//見つけたリソースをロード
	hLoadGIcon = LoadResource(hFile, hGIcon);
	if (hLoadGIcon == NULL)
	{
		printf("NG(1)\n");
		return 0;
	}
	else
	{
		printf("OK(1)\n");
	}

#if 0x0
	//リソースをロックして、メモリアドレスを得る
	lpIconResHead = (LPICONRESHEAD)LockResource(hLoadGIcon);

	//アイコン全体のサイズを計算する
	AllSize = GetGroupIconSize(lpIconResHead);

	pMem = (PBYTE)GlobalAlloc(GMEM_FIXED, AllSize);
	if (pMem == NULL) return 0;

	//全てのアイコンをコピー
	if (StoreAllIconData(hFile, lpIconResHead, pMem) == FALSE) {
		GlobalFree(pMem);
		return  0;
	}
#endif

	*pRetMem = pMem;

	return  AllSize;
}

// EnumResourceNames のコールバック関数
extern "C" BOOL CALLBACK _MyEnumResNameProc(HMODULE hModule, LPCTSTR lpszType,
	LPTSTR lpszName, void* pData)
{
	setlocale(LC_ALL, "japanese");
	//printf("lpszName=%ls\n", lpszName);
	printf("lpszName=0x%08x\n", lpszName);
	printf("pData=0x%08x\n", pData);
	//printf("lpszType=%ls\n", lpszType);
	//printf("lpszName=%s\n", lpszName);

	DWORD       Size, wSize;
	PBYTE       pMem;
	char        IcoName[MAX_PATH];
	HANDLE      hFile;

	Size = GetGroupIconData(hModule, lpszName, &pMem);

	/*
	// 現在のインデックスが欲しいアイコンのインデックスと一致するかどうか
	if (pData->nNowPos == pData->nIndex)
	{
		// 一致 → 見つかった
		pData->bFound = true;
		// lpszName の上位ワードが 0 のときは ID が数値
		// (文字列のポインタで上位ワードが 0 のものはまず無い)
		if (pData->bIsID = (HIWORD(lpszName) == 0))
			pData->uID = LOWORD(lpszName);
		else
			// 文字列のコピーを作成する
			pData->lpszName = _tcsdup(lpszName);
		// FALSE を返して列挙を終了する
		return FALSE;
	}
	// 次のインデックスにしておく
	pData->nNowPos++;
	*/
	return TRUE;
}

// lpszPathName:  アイコンのデータが入っているアプリケーション/DLL
// nIndexOrID:    正の値(0 含む)ならインデックス、負の値なら ID
// bSmallIcon:    16x16 のサイズのアイコンを取得するかどうか (false なら 32x32)
HICON MyExtractIcon(LPCTSTR lpszPathName, int nIndexOrID, bool bSmallIcon)
{
    HINSTANCE hInstance;
    HICON hIconRet;
    //_CMyExtractIconData data;
    HRSRC hRes, hRes2;
    HGLOBAL hMem, hMem2;
    LPVOID lpv, lpv2;

	printf("(1)\n");
    if (!lpszPathName)
        return NULL;
    // 実行はしないので LOAD_LIBRARY_AS_DATAFILE を指定する
    hInstance = LoadLibraryEx(lpszPathName, NULL,
        LOAD_LIBRARY_AS_DATAFILE | LOAD_WITH_ALTERED_SEARCH_PATH);
    if (!hInstance)
        return NULL;

	printf("(2)\n");
	hIconRet = NULL;
    if (nIndexOrID < 0)
    {
        // 負の値 = 識別子
        // LoadIcon や LoadImage だと Win95/98/Me で読み込めない・・・
        //data.bIsID = true;
        //data.uID = (UINT) (-nIndexOrID);
    }
    else
    {
        // 正の値 = インデックス
        // リソースを列挙して読み込み
        //data.nIndex = nIndexOrID;
        //data.nNowPos = 0;
        //data.bFound = false;
        //EnumResourceNames(hInstance, RT_GROUP_ICON, (ENUMRESNAMEPROC) _MyEnumResNameProc, (LPARAM)NULL);
		//EnumResourceNames(hInstance, RT_RCDATA, (ENUMRESNAMEPROC)_MyEnumResNameProc, (LPARAM)NULL);
		EnumResourceNames(hInstance, RT_GROUP_ICON, (ENUMRESNAMEPROC)_MyEnumResNameProc, (LPARAM)NULL);
		//EnumResourceNames(hInstance, RT_ICON, (ENUMRESNAMEPROC)_MyEnumResNameProc, (LPARAM)NULL);
		// 見つからなかったら終了
		/*
        if (!data.bFound)
        {
            FreeLibrary(hInstance);
            return NULL;
        }
		*/
    }

	/*
    // グループアイコン リソースを取得
    // (サイズや色情報が複数含まれている可能性もあるため)
    hRes = FindResource(hInstance,
        data.bIsID ? MAKEINTRESOURCE(data.uID) : (LPCTSTR) data.lpszName,
        RT_GROUP_ICON);
    if (hRes)
    {
        // FindResource のあとは LoadResource、LockResource
        hMem = LoadResource(hInstance, hRes);
        lpv = LockResource(hMem);
        // アイコン本体の ID (指定したサイズ、色を持つアイコン) を取得する関数
        // (第 2 引数が FALSE の場合カーソルを取得)
        // (bSmallIcon が true なら 16x16、false なら 32x32)
        // (色はデフォルトのものを使用する)
        // ※ ここでサイズを任意のものに指定できます。(48x48 など)
        data.uID = LookupIconIdFromDirectoryEx((PBYTE) lpv, TRUE,
            bSmallIcon ? 16 : 32, bSmallIcon ? 16 : 32, LR_DEFAULTCOLOR);
        // アイコン本体のリソースを取得
        hRes2 = FindResource(hInstance, MAKEINTRESOURCE(data.uID), RT_ICON);
        if (hRes2)
        {
            // FindResource のあとは LoadResource、LockResource
            hMem2 = LoadResource(hInstance, hRes2);
            lpv2 = LockResource(hMem2);
            // リソースからアイコンを作成
            hIconRet = CreateIconFromResource((PBYTE) lpv2,
                SizeofResource(hInstance, hRes2),
                TRUE, 0x00030000);
            // Win32 ではロック解除、解放はいらない (呼び出せない)
            //UnlockResource(hMem2);
            //FreeResource(hRes2);
        }
        //UnlockResource(hMem);
        //FreeResource(hRes);
    }

    // 文字列を使用している場合は解放する
    if (!data.bIsID)
        free(data.lpszName);

	*/
    FreeLibrary(hInstance);
    return hIconRet;
}
