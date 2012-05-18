#include "Stdafx.h"
#include "AfxDllx.h"
#include "DownLoadHead.h"

//////////////////////////////////////////////////////////////////////////////////

//静态变量
static AFX_EXTENSION_MODULE DownLoadDLL={NULL,NULL};

//////////////////////////////////////////////////////////////////////////////////

//导出函数
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(DownLoadDLL, hInstance)) return 0;
		new CDynLinkLibrary(DownLoadDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(DownLoadDLL);
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////////////
