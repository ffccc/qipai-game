#pragma once

//////////////////////////////////////////////////////////////////////////////////

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef WINVER
#define WINVER 0x0500
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0400
#endif

#define _ATL_ATTRIBUTES
#define _AFX_ALL_WARNINGS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

//////////////////////////////////////////////////////////////////////////////////

//MFC �ļ�
#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxCmn.h>
#include <AfxDisp.h>

//////////////////////////////////////////////////////////////////////////////////
//���Ӵ���

//ϵͳ����
#pragma comment(lib,"VFW32.lib")

#ifndef _DEBUG
#ifndef _UNICODE
	#pragma comment (lib,"../../���ӿ�/Ansi/WHImage.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/DownLoad.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/ServiceCore.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/SkinControl.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/ShareControl.lib")
#else
	#pragma comment (lib,"../../���ӿ�/Unicode/WHImage.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/DownLoad.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/ServiceCore.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/SkinControl.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/ShareControl.lib")
#endif
#else
#ifndef _UNICODE
	#pragma comment (lib,"../../���ӿ�/Ansi/WHImageD.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/DownLoadD.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/ServiceCoreD.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/SkinControlD.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/ShareControlD.lib")
#else
	#pragma comment (lib,"../../���ӿ�/Unicode/WHImageD.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/DownLoadD.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/ServiceCoreD.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/SkinControlD.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/ShareControlD.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////
