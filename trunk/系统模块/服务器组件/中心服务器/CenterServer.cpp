#include "Stdafx.h"
#include "CenterServer.h"
#include "CenterServerDlg.h"

//////////////////////////////////////////////////////////////////////////

//Ӧ�ó������
CCenterServerApp theApp;

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CCenterServerApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

CCenterServerApp::CCenterServerApp()
{
}

//��ʼ������
BOOL CCenterServerApp::InitInstance()
{
	__super::InitInstance();

	//��ʼ�����
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//����ע���
	SetRegistryKey(szSystemRegKey);

	//��ʾ������
	CCenterServerDlg ServerDlg;
	m_pMainWnd=&ServerDlg;
	ServerDlg.DoModal();

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
