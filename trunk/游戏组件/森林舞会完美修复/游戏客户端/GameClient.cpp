#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include ".\gameclient.h"

//Ӧ�ó������
CGameClientApp theApp;

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

BOOL CGameClientApp::InitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	//m_lpCmdLine = "/RoomToken:0x817280964485028 /ServerType:2";
	return CGameFrameApp::InitInstance();
}
