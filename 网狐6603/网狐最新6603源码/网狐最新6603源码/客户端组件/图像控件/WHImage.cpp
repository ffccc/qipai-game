#include "Stdafx.h"
#include "AfxDllx.h"
#include "WHImageHead.h"

//////////////////////////////////////////////////////////////////////////////////

//静态变量
static ULONG_PTR g_lGdiPlusToken=0L;
static AFX_EXTENSION_MODULE WHImageDLL={NULL,NULL};

//////////////////////////////////////////////////////////////////////////////////

//导出函数
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(WHImageDLL, hInstance)) return 0;
		new CDynLinkLibrary(WHImageDLL);

		GdiplusStartupInput StartupInput;
		GdiplusStartup(&g_lGdiPlusToken,&StartupInput,NULL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		GdiplusShutdown(g_lGdiPlusToken);
		AfxTermExtensionModule(WHImageDLL);
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////////////
