#include "Stdafx.h"
#include "AfxDllx.h"
#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//静态变量
static AFX_EXTENSION_MODULE ShareControlDLL={NULL,NULL};

//////////////////////////////////////////////////////////////////////////////////

//导出函数
extern "C" INT APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(ShareControlDLL, hInstance)) return 0;
		new CDynLinkLibrary(ShareControlDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(ShareControlDLL);
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////////////
