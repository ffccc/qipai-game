#include "Stdafx.h"
#include "AfxDllx.h"
#include "GameServiceHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��̬����
static AFX_EXTENSION_MODULE GameServiceDLL={NULL,NULL};

//////////////////////////////////////////////////////////////////////////////////

//��������
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(GameServiceDLL, hInstance)) return 0;
		new CDynLinkLibrary(GameServiceDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(GameServiceDLL);
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////////////
