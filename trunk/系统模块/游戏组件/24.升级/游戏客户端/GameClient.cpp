#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//Ӧ�ó������
CGameClientApp theApp;

//��Ƶ����
#ifdef VIDEO_GAME
CVideoServiceManager g_VedioServiceManager;
#endif

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientApp::CGameClientApp()
{
}

//��������
CGameClientApp::~CGameClientApp() 
{
}

//////////////////////////////////////////////////////////////////////////
