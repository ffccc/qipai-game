#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//Ӧ�ó������
CGameClientApp theApp;

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientApp::CGameClientApp()
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sCGameClientApp.log",strTime);

	strTemp.Format("into CGameClientApp()");
	WriteLog(strFile, strTemp);
}

//��������
CGameClientApp::~CGameClientApp() 
{
}

//////////////////////////////////////////////////////////////////////////
