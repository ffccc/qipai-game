#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"

#include "Resource.h"
#include "GlobalOption.h"
//Ӧ�ó������
CGameClientApp theApp;

//////////////////////////////////////////////////////////////////////////


//���캯��
CGameClientApp::CGameClientApp()
{
	//m_pGameFrame=NULL;
}

//��������
CGameClientApp::~CGameClientApp()
{
}
/*
//��ʼ������
BOOL CGameClientApp::InitInstance()
{
	__super::InitInstance();

	//�ڲ���ʼ��
	AfxOleInit();
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//��ʼ�������
	IUnknownEx * pIUnknownEx=(IUnknownEx *)m_SkinRecordFile.QueryInterface(IID_IUnknownEx,VER_IUnknownEx);
	CSkinAttribute::LoadSkinAttribute(pIUnknownEx);

	//��������
	g_GlobalOption.LoadOptionParameter();

	try
	{
		//����������
		m_pGameFrame= new CGameClientDlg;
		ASSERT(m_pGameFrame!=NULL);
		if (m_pGameFrame==NULL) throw TEXT("��Ϸ�����ڴ���ʧ��");

		//����������
		m_pMainWnd=m_pGameFrame;
		m_pGameFrame->Create(IDD_GAME_FRAME,NULL);
	}
	catch (LPCTSTR pszMesssage)
	{
		CString strBuffer;
		strBuffer.Format(TEXT("���� [ %s ] ��Ϸ���򼴽��˳���"),pszMesssage);
		AfxMessageBox(strBuffer,MB_ICONSTOP);
		if (m_pGameFrame!=NULL) SafeDelete(m_pGameFrame);
		return FALSE;
	}

	return TRUE;
}

//�˳���Ϣ
int CGameClientApp::ExitInstance()
{
	//ɾ������
	if (m_pGameFrame!=NULL) SafeDelete(m_pGameFrame);
	return __super::ExitInstance();
}*/
//////////////////////////////////////////////////////////////////////////
