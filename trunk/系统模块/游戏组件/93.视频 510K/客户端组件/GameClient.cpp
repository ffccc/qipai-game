#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//�������
CGameClientApp theApp;
//��Ƶ����
CVideoServiceManager g_VedioServiceManager;

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientApp::CGameClientApp()
{
}

//��������
CGameClientApp::~CGameClientApp()
{
}

////��������
//CGameFrameWnd * CGameClientApp::GetGameFrameWnd()
//{
//	return new CGameClientWnd;
//}
//
////��������
//CGameFrameEngine * CGameClientApp::GetGameFrameEngine(DWORD dwSDKVersion)
//{
//	//�汾���
//	if (InterfaceVersionCompare(VERSION_FRAME_SDK,dwSDKVersion)==false) return NULL;
//
//	//��������
//	return new CGameClientDlg;
//}

//////////////////////////////////////////////////////////////////////////
