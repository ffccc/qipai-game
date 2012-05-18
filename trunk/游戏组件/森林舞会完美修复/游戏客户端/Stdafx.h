#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 标头中排除不常使用的资料
#endif

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用 Windows 95 和 Windows NT 4 或更高版本的特定功能。
#define WINVER 0x0400		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#endif

#ifndef _WIN32_WINNT		// 允许使用 Windows NT 4 或更高版本的特定功能。
#define _WIN32_WINNT 0x0400		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用 Windows 98 或更高版本的特定功能。
#define _WIN32_WINDOWS 0x0410 //为 Windows Me 及更新版本改变为适当的值。
#endif

#ifndef _WIN32_IE			// 允许使用 IE 4.0 或更高版本的特定功能。
#define _WIN32_IE 0x0400	//为 IE 5.0 及更新版本改变为适当的值。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常被安全忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心和标准组件
#include <afxext.h>         // MFC 扩展
#include <afxdisp.h>        // MFC 自动化类

#include <afxdtctl.h>		// Internet Explorer 4 公共控件的 MFC 支持
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows 公共控件的 MFC 支持
#endif // _AFX_NO_AFXCMN_SUPPORT

//////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <fstream>
#include "Afxmt.h"
#include "../消息定义/CMD_Baccarat.h"
#include "../../../系统模块/开发库/Include/GameFrame.h"


//以下include顺序调整可能出错
#include "../../../QPLIB/include/UI/GameButton.h"
#include "../../../QPLIB/include/UI/WndMsgDefine.h"
#include "../../../QPLIB/include/UI/GameApp.h"
#include "../../../QPLIB/include/EngineInterface.h"
#include "../../../QPLIB/include/UI/GameSound.h"
#include "../../../QPLIB/include/SurfaceManager.h"
//////////////////////////////////////////////////////////////////////////
#if defined(_DEBUG)||defined(DEBUG)
	#pragma comment(lib, "../../../QPLIB/Lib/UIDebug.lib")
#else
	#pragma comment(lib, "../../../QPLIB/Lib/EngineLib.lib")
	#pragma comment(lib, "../../../QPLIB/Lib/UI.lib")
	#pragma comment(lib, "../../../QPLIB/Lib/XmlLib.lib")
	#pragma comment(lib, "../../../QPLIB/Lib/fmodvc.lib")
#endif

//定义系统不能做庄
#define _NO_SYS_BANKER

//声音文件
#define	SOUND_ADD_GOLD								TEXT("add_gold.wav"     )
#define	SOUND_GAME_END								TEXT("game_end.wav"     )
#define	SOUND_PLACE_JETTON							TEXT("place_jetton.wav" )

#define	SOUND_STOP_JETTON							TEXT("stop_jetton.wav"  )
#define	SOUND_WIN									TEXT("win.wav"     )
#define	SOUND_FREE									TEXT("game_free.wav"     )
//消息定义
#define IDM_INSERT_SYSTEM_STRING		WM_USER+202							//写聊天区域信息，测试用
#define IDM_PLACE_JETTON				WM_USER+200							//加住信息
#define IDM_APPLY_BANKER				WM_USER+201							//申请信息
#define IDM_CANCEL_FANG			     	WM_USER+203							//取分信息

#define IDM_SEND_CARD_CARTOON_FINISH	WM_USER+300							//发牌动画完成
#define IDM_PLAY_SOUND					WM_USER+301							//播放声音

#define WM_SHOW				  WM_USER+100								//发牌动画时间
#define WM_CARD				  WM_USER+101								//发牌动画时间
#define WM_POS				  WM_USER+102								//发牌动画时间

#include "GameClientDlg.h"
extern CGameClientDlg * pGlobalDlg;