#include "Stdafx.h"
#include <Afxdllx.h>

//���ͷ�ļ�
#include "ClientShare.h"

//////////////////////////////////////////////////////////////////////////

static AFX_EXTENSION_MODULE ClientShareDLL={NULL,NULL};

//DLL ����������
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(ClientShareDLL,hInstance)) return 0;
		new CDynLinkLibrary(ClientShareDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(ClientShareDLL);
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////
