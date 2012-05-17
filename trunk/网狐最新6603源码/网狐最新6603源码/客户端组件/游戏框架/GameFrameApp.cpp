#include "Stdafx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "GameFrameApp.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameFrameApp::CGameFrameApp()
{
	//���ñ���
	m_pGameFrameWnd=NULL;
	m_pGameFrameEngine=NULL;

	return;
}

//��������
CGameFrameApp::~CGameFrameApp()
{
}

//���ú���
BOOL CGameFrameApp::InitInstance()
{
	__super::InitInstance();

	//�ڲ���ʼ��
	AfxOleInit();
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	try
	{
		//��������
		ASSERT(CGlobalUnits::GetInstance()!=NULL);
		CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

		//ȫ�ֵ�Ԫ
		bool bSuccess=pGlobalUnits->InitGlobalUnits();
		if (bSuccess==false) throw TEXT("��Ϸ������ʼ��ʧ��");

		//��������
		m_pGameFrameWnd=GetGameFrameWnd();
		if (m_pGameFrameWnd==NULL) throw TEXT("��Ϸ��ܴ��ڴ���ʧ��");

		//��������
		m_pGameFrameEngine=GetGameFrameEngine(VERSION_FRAME_SDK);
		if (m_pGameFrameEngine==NULL) throw TEXT("��Ϸ������󴴽�ʧ��");

		//�������
		m_pMainWnd=m_pGameFrameWnd;
		m_pGameFrameWnd->Create(NULL,NULL,WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,CRect(0,0,0,0));

		//��������
		m_D3DSound.CreateD3DSound(m_pMainWnd->GetSafeHwnd());

		//��������
		if (m_GameFrameService.CreateGameFrameService()==false)
		{
			throw TEXT("�ں����洴��ʧ��");
		}

		//���ӹ㳡
		IClientKernel * pIClientKernel=pGlobalUnits->m_ClientKernelModule.GetInterface();
		if ((pIClientKernel!=NULL)&&(pIClientKernel->IsSingleMode()==false)) pIClientKernel->CreateConnect();

		return TRUE;
	}
	catch (LPCTSTR pszMesssage)
	{
		//������Ϣ
		CString strBuffer;
		strBuffer.Format(TEXT("���� [ %s ] ��Ϸ��������ʧ�ܡ�"),pszMesssage);

		//��ʾ��Ϣ
		CInformation Information;
		Information.ShowMessageBox(TEXT("��Ϸ���"),strBuffer,MB_ICONSTOP,30L);

		//ɾ������
		SafeRelease(m_pGameFrameWnd);
		SafeRelease(m_pGameFrameEngine);

		return FALSE;
	}

	return TRUE;
}

//�˳�����
BOOL CGameFrameApp::ExitInstance()
{
	//ɾ������
	SafeRelease(m_pGameFrameEngine);

	//��������
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//�ر�����
	IClientKernel * pIClientKernel=pGlobalUnits->m_ClientKernelModule.GetInterface();
	if ((pIClientKernel!=NULL)&&(pIClientKernel->IsSingleMode()==false)) pIClientKernel->IntermitConnect();

	//ע�����
	pGlobalUnits->UnInitGlobalUnits();

	return __super::ExitInstance();
}

//��������
CGameFrameWnd * CGameFrameApp::GetGameFrameWnd()
{
	return new CGameFrameWnd;
}

//////////////////////////////////////////////////////////////////////////////////
