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

//�O���[�v�A�C�R�����̑S�A�C�R���f�[�^���擾����
//�@�@�@HANDLE  hFile   ���W���[���n���h��
//�@�@�@LPTSTR  lpStr   ���\�[�X�̖��O
//�@�@�@=>PBYTE         �f�[�^�o�b�t�@�A�h���X�i�v:GlobalFree�j

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

	//�O���[�v�A�C�R�����\�[�X������
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

	//���������\�[�X�����[�h
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
	//���\�[�X�����b�N���āA�������A�h���X�𓾂�
	lpIconResHead = (LPICONRESHEAD)LockResource(hLoadGIcon);

	//�A�C�R���S�̂̃T�C�Y���v�Z����
	AllSize = GetGroupIconSize(lpIconResHead);

	pMem = (PBYTE)GlobalAlloc(GMEM_FIXED, AllSize);
	if (pMem == NULL) return 0;

	//�S�ẴA�C�R�����R�s�[
	if (StoreAllIconData(hFile, lpIconResHead, pMem) == FALSE) {
		GlobalFree(pMem);
		return  0;
	}
#endif

	*pRetMem = pMem;

	return  AllSize;
}

// EnumResourceNames �̃R�[���o�b�N�֐�
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
	// ���݂̃C���f�b�N�X���~�����A�C�R���̃C���f�b�N�X�ƈ�v���邩�ǂ���
	if (pData->nNowPos == pData->nIndex)
	{
		// ��v �� ��������
		pData->bFound = true;
		// lpszName �̏�ʃ��[�h�� 0 �̂Ƃ��� ID �����l
		// (������̃|�C���^�ŏ�ʃ��[�h�� 0 �̂��̂͂܂�����)
		if (pData->bIsID = (HIWORD(lpszName) == 0))
			pData->uID = LOWORD(lpszName);
		else
			// ������̃R�s�[���쐬����
			pData->lpszName = _tcsdup(lpszName);
		// FALSE ��Ԃ��ė񋓂��I������
		return FALSE;
	}
	// ���̃C���f�b�N�X�ɂ��Ă���
	pData->nNowPos++;
	*/
	return TRUE;
}

// lpszPathName:  �A�C�R���̃f�[�^�������Ă���A�v���P�[�V����/DLL
// nIndexOrID:    ���̒l(0 �܂�)�Ȃ�C���f�b�N�X�A���̒l�Ȃ� ID
// bSmallIcon:    16x16 �̃T�C�Y�̃A�C�R�����擾���邩�ǂ��� (false �Ȃ� 32x32)
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
    // ���s�͂��Ȃ��̂� LOAD_LIBRARY_AS_DATAFILE ���w�肷��
    hInstance = LoadLibraryEx(lpszPathName, NULL,
        LOAD_LIBRARY_AS_DATAFILE | LOAD_WITH_ALTERED_SEARCH_PATH);
    if (!hInstance)
        return NULL;

	printf("(2)\n");
	hIconRet = NULL;
    if (nIndexOrID < 0)
    {
        // ���̒l = ���ʎq
        // LoadIcon �� LoadImage ���� Win95/98/Me �œǂݍ��߂Ȃ��E�E�E
        //data.bIsID = true;
        //data.uID = (UINT) (-nIndexOrID);
    }
    else
    {
        // ���̒l = �C���f�b�N�X
        // ���\�[�X��񋓂��ēǂݍ���
        //data.nIndex = nIndexOrID;
        //data.nNowPos = 0;
        //data.bFound = false;
        //EnumResourceNames(hInstance, RT_GROUP_ICON, (ENUMRESNAMEPROC) _MyEnumResNameProc, (LPARAM)NULL);
		//EnumResourceNames(hInstance, RT_RCDATA, (ENUMRESNAMEPROC)_MyEnumResNameProc, (LPARAM)NULL);
		EnumResourceNames(hInstance, RT_GROUP_ICON, (ENUMRESNAMEPROC)_MyEnumResNameProc, (LPARAM)NULL);
		//EnumResourceNames(hInstance, RT_ICON, (ENUMRESNAMEPROC)_MyEnumResNameProc, (LPARAM)NULL);
		// ������Ȃ�������I��
		/*
        if (!data.bFound)
        {
            FreeLibrary(hInstance);
            return NULL;
        }
		*/
    }

	/*
    // �O���[�v�A�C�R�� ���\�[�X���擾
    // (�T�C�Y��F��񂪕����܂܂�Ă���\�������邽��)
    hRes = FindResource(hInstance,
        data.bIsID ? MAKEINTRESOURCE(data.uID) : (LPCTSTR) data.lpszName,
        RT_GROUP_ICON);
    if (hRes)
    {
        // FindResource �̂��Ƃ� LoadResource�ALockResource
        hMem = LoadResource(hInstance, hRes);
        lpv = LockResource(hMem);
        // �A�C�R���{�̂� ID (�w�肵���T�C�Y�A�F�����A�C�R��) ���擾����֐�
        // (�� 2 ������ FALSE �̏ꍇ�J�[�\�����擾)
        // (bSmallIcon �� true �Ȃ� 16x16�Afalse �Ȃ� 32x32)
        // (�F�̓f�t�H���g�̂��̂��g�p����)
        // �� �����ŃT�C�Y��C�ӂ̂��̂Ɏw��ł��܂��B(48x48 �Ȃ�)
        data.uID = LookupIconIdFromDirectoryEx((PBYTE) lpv, TRUE,
            bSmallIcon ? 16 : 32, bSmallIcon ? 16 : 32, LR_DEFAULTCOLOR);
        // �A�C�R���{�̂̃��\�[�X���擾
        hRes2 = FindResource(hInstance, MAKEINTRESOURCE(data.uID), RT_ICON);
        if (hRes2)
        {
            // FindResource �̂��Ƃ� LoadResource�ALockResource
            hMem2 = LoadResource(hInstance, hRes2);
            lpv2 = LockResource(hMem2);
            // ���\�[�X����A�C�R�����쐬
            hIconRet = CreateIconFromResource((PBYTE) lpv2,
                SizeofResource(hInstance, hRes2),
                TRUE, 0x00030000);
            // Win32 �ł̓��b�N�����A����͂���Ȃ� (�Ăяo���Ȃ�)
            //UnlockResource(hMem2);
            //FreeResource(hRes2);
        }
        //UnlockResource(hMem);
        //FreeResource(hRes);
    }

    // ��������g�p���Ă���ꍇ�͉������
    if (!data.bIsID)
        free(data.lpszName);

	*/
    FreeLibrary(hInstance);
    return hIconRet;
}
