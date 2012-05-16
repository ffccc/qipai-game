#include "Stdafx.h"
#include <Afxdllx.h>

//���ͷ�ļ�
#include "GameServiceExport.h"

//////////////////////////////////////////////////////////////////////////

static AFX_EXTENSION_MODULE GameServiceDLL={NULL,NULL};

//DLL ����������
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(GameServiceDLL,hInstance)) return 0;
		new CDynLinkLibrary(GameServiceDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(GameServiceDLL);
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////


