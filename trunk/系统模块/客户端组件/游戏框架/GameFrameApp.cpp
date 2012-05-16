#include "Stdafx.h"
#include "Resource.h"
#include "GameFrameApp.h"
#include "GlobalOption.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameFrameApp::CGameFrameApp()
{
	m_pGameFrame=NULL;
}

//��������
CGameFrameApp::~CGameFrameApp()
{
}

//��ʼ������
BOOL CGameFrameApp::InitInstance()
{
	__super::InitInstance();

	//�ڲ���ʼ��
	AfxOleInit();
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//������Դ
	if(g_GlobalOption.LoadGameFrameRes()==false)
	{
		ShowInformation(TEXT("������Դʧ��!"),0,MB_ICONSTOP);
		return FALSE;
	}

	//��������
	g_GlobalOption.LoadOptionParameter();

	try
	{
		//����������
		m_pGameFrame=GetGameFrameDlg();
		ASSERT(m_pGameFrame!=NULL);
		if (m_pGameFrame==NULL) throw TEXT("��Ϸ�����ڴ���ʧ��");

		//����������
		m_pMainWnd=m_pGameFrame;
		AfxSetResourceHandle(GetModuleHandle(GAME_FRAME_DLL_NAME));
		m_pGameFrame->Create(IDD_GAME_FRAME,NULL);
		AfxSetResourceHandle(GetModuleHandle(NULL));
	}
	catch (LPCTSTR pszMesssage)
	{
		CString strBuffer;
		strBuffer.Format(TEXT("���� [ %s ] ��Ϸ���򼴽��˳���"),pszMesssage);
		ShowInformation(strBuffer,0,MB_ICONSTOP);
		if (m_pGameFrame!=NULL) SafeDelete(m_pGameFrame);
		return FALSE;
	}

	return TRUE;
}

//�˳���Ϣ
int CGameFrameApp::ExitInstance()
{
	//ɾ������
	if (m_pGameFrame!=NULL) SafeDelete(m_pGameFrame);
	return __super::ExitInstance();
}

//////////////////////////////////////////////////////////////////////////
