// BJServer.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include <afxdllx.h>

//////////////////////////////////////////////////////////////////////////

static AFX_EXTENSION_MODULE BJServerDLL = { NULL, NULL };
//DLL ����������
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(BJServerDLL, hInstance)) return 0;
		new CDynLinkLibrary(BJServerDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(BJServerDLL);
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////
