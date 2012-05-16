#include "Stdafx.h"
#include <Afxdllx.h>

//���ͷ�ļ�
#include "Companion.h"

//////////////////////////////////////////////////////////////////////////

static AFX_EXTENSION_MODULE CompanionDLL={NULL,NULL};

//DLL ����������
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(CompanionDLL,hInstance)) return 0;
		new CDynLinkLibrary(CompanionDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(CompanionDLL);
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////
