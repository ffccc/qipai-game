#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include ".\gameclient.h"

//应用程序对象
CGameClientApp theApp;

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientApp::CGameClientApp()
{
}

//析构函数
CGameClientApp::~CGameClientApp() 
{
}

//////////////////////////////////////////////////////////////////////////

BOOL CGameClientApp::InitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	//m_lpCmdLine = "/RoomToken:0x817280964485028 /ServerType:2";
	return CGameFrameApp::InitInstance();
}
