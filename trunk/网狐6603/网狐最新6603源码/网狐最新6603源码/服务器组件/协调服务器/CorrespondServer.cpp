#include "Stdafx.h"
#include "CorrespondServer.h"
#include "CorrespondServerDlg.h"

//////////////////////////////////////////////////////////////////////////////////

//�������
CCorrespondServerApp theApp;

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CCorrespondServerApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CCorrespondServerApp::CCorrespondServerApp()
{
}

//��������
BOOL CCorrespondServerApp::InitInstance()
{
	__super::InitInstance();

	//�������
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//����ע���
	SetRegistryKey(szProduct);

	//��ʾ����
	CCorrespondServerDlg CorrespondServerDlg;
	m_pMainWnd=&CorrespondServerDlg;
	CorrespondServerDlg.DoModal();

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////
