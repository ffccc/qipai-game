#include "Stdafx.h"
#include "AfxDllx.h"
#include "UserServiceHead.h"

//////////////////////////////////////////////////////////////////////////////////

//静态变量
static AFX_EXTENSION_MODULE UserServiceDLL={NULL,NULL};

//////////////////////////////////////////////////////////////////////////////////

//导出函数
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(UserServiceDLL, hInstance)) return 0;
		new CDynLinkLibrary(UserServiceDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(UserServiceDLL);
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////////////
