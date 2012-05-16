#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//���ͷ�ļ�
#include "PropertyModule.h"
//////////////////////////////////////////////////////////////////////////

static AFX_EXTENSION_MODULE PropertyModuleDLL = { NULL, NULL };

//DLL ����������
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("PropertyModule.DLL Initializing!\n");
		if (!AfxInitExtensionModule(PropertyModuleDLL, hInstance))
			return 0;

		new CDynLinkLibrary(PropertyModuleDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("PropertyModule.DLL Terminating!\n");
		AfxTermExtensionModule(PropertyModuleDLL);
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////
