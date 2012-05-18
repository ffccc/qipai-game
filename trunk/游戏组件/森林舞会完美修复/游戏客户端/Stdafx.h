#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ��ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ�� Windows 95 �� Windows NT 4 ����߰汾���ض����ܡ�
#define WINVER 0x0400		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_WINNT		// ����ʹ�� Windows NT 4 ����߰汾���ض����ܡ�
#define _WIN32_WINNT 0x0400		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ�� Windows 98 ����߰汾���ض����ܡ�
#define _WIN32_WINDOWS 0x0410 //Ϊ Windows Me �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ�� IE 4.0 ����߰汾���ض����ܡ�
#define _WIN32_IE 0x0400	//Ϊ IE 5.0 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ��������������ȫ���Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ���ĺͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <afxdisp.h>        // MFC �Զ�����

#include <afxdtctl.h>		// Internet Explorer 4 �����ؼ��� MFC ֧��
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows �����ؼ��� MFC ֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

//////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <fstream>
#include "Afxmt.h"
#include "../��Ϣ����/CMD_Baccarat.h"
#include "../../../ϵͳģ��/������/Include/GameFrame.h"


//����include˳��������ܳ���
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

//����ϵͳ������ׯ
#define _NO_SYS_BANKER

//�����ļ�
#define	SOUND_ADD_GOLD								TEXT("add_gold.wav"     )
#define	SOUND_GAME_END								TEXT("game_end.wav"     )
#define	SOUND_PLACE_JETTON							TEXT("place_jetton.wav" )

#define	SOUND_STOP_JETTON							TEXT("stop_jetton.wav"  )
#define	SOUND_WIN									TEXT("win.wav"     )
#define	SOUND_FREE									TEXT("game_free.wav"     )
//��Ϣ����
#define IDM_INSERT_SYSTEM_STRING		WM_USER+202							//д����������Ϣ��������
#define IDM_PLACE_JETTON				WM_USER+200							//��ס��Ϣ
#define IDM_APPLY_BANKER				WM_USER+201							//������Ϣ
#define IDM_CANCEL_FANG			     	WM_USER+203							//ȡ����Ϣ

#define IDM_SEND_CARD_CARTOON_FINISH	WM_USER+300							//���ƶ������
#define IDM_PLAY_SOUND					WM_USER+301							//��������

#define WM_SHOW				  WM_USER+100								//���ƶ���ʱ��
#define WM_CARD				  WM_USER+101								//���ƶ���ʱ��
#define WM_POS				  WM_USER+102								//���ƶ���ʱ��

#include "GameClientDlg.h"
extern CGameClientDlg * pGlobalDlg;