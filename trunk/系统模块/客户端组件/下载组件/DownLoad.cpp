#include "Stdafx.h"
#include <Afxdllx.h>

//���ͷ�ļ�
#include "DownLoad.h"

//////////////////////////////////////////////////////////////////////////

static AFX_EXTENSION_MODULE DownLoadDLL={NULL,NULL};

//DLL ����������
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(DownLoadDLL,hInstance)) return 0;
		new CDynLinkLibrary(DownLoadDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(DownLoadDLL);
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////
