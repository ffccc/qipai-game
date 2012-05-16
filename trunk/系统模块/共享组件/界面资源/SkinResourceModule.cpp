#include "Stdafx.h"
#include <Afxdllx.h>

//���ͷ�ļ�
#include "SkinResourceModule.h"

//////////////////////////////////////////////////////////////////////////

static AFX_EXTENSION_MODULE SkinResourceDLL={NULL,NULL};

//DLL ����������
extern "C" INT APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(SkinResourceDLL,hInstance)) return 0;
		new CDynLinkLibrary(SkinResourceDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(SkinResourceDLL);
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////
