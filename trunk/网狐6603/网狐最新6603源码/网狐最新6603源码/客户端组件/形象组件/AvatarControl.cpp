#include "Stdafx.h"
#include "AfxDLLx.h"

static AFX_EXTENSION_MODULE AvatarControlDLL={NULL,NULL};

//////////////////////////////////////////////////////////////////////////////////

//DLL Ö÷º¯Êý
extern "C" INT APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(AvatarControlDLL, hInstance)) return 0;
		new CDynLinkLibrary(AvatarControlDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(AvatarControlDLL);
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////////////
