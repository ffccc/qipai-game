#include "Stdafx.h"
#include "AfxDLLx.h"
#include "PlatformDataHead.h"

//////////////////////////////////////////////////////////////////////////////////

//静态变量
static AFX_EXTENSION_MODULE PlatformDataDLL={NULL,NULL};

//////////////////////////////////////////////////////////////////////////////////

//DLL 主函数
extern "C" INT APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(PlatformDataDLL, hInstance)) return 0;
		new CDynLinkLibrary(PlatformDataDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(PlatformDataDLL);
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////////////
