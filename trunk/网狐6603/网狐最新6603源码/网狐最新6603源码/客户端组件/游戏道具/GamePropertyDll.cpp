#include "Stdafx.h"
#include "AfxDllx.h"
#include "GamePropertyHead.h"

//////////////////////////////////////////////////////////////////////////////////

//静态变量
static AFX_EXTENSION_MODULE GamePropertyDLL={NULL,NULL};

//////////////////////////////////////////////////////////////////////////////////

//导出函数
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(GamePropertyDLL, hInstance)) return 0;
		new CDynLinkLibrary(GamePropertyDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(GamePropertyDLL);
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////////////
