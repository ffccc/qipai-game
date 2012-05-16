#include "Stdafx.h"
#include "GamePlaza.h"
#include "GameFrame.h"
#include "GlobalUnits.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGamePlazaApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//Ӧ�ó������˵��
CGamePlazaApp theApp;

//���캯��
CGamePlazaApp::CGamePlazaApp()
{
}

//��ʼ������
BOOL CGamePlazaApp::InitInstance()
{
	__super::InitInstance();

	//�ж��Ƿ�����
#ifndef ADMIN_PLAZA
#ifndef _DEBUG
	bool bExistIGame=false;
	CMutex Mutex(FALSE,MAIN_DLG_CLASSNAME,NULL);
	if (Mutex.Lock(0)==FALSE) bExistIGame=true;
	CWnd * pIGameWnd=CWnd::FindWindow(MAIN_DLG_CLASSNAME,NULL);
	if (pIGameWnd!=NULL) 
	{
		bExistIGame=true;
		if (pIGameWnd->IsIconic()) pIGameWnd->ShowWindow(SW_RESTORE);
		pIGameWnd->SetActiveWindow();
		pIGameWnd->BringWindowToTop();
		pIGameWnd->SetForegroundWindow();
	}
	if (bExistIGame==true) return FALSE;
#endif
#endif

	//�����ʼ��
	AfxOleInit();
	AfxInitRichEdit();
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//����ע���
	SetRegistryKey(szSystemRegKey);

	////���ؽ����
	//CSkinAttribute::LoadSkinAttribute(QUERY_OBJECT_INTERFACE(m_SkinRecordFile,IUnknownEx));

	//��������
	g_GlobalOption.LoadOptionParameter();

	//ȫ����Ϣ
	if (g_GlobalUnits.InitGlobalUnits()==false)
	{
		ShowInformation(TEXT("��Ϸ�㳡��ʼ��ʧ�ܣ����򼴽��˳���"),0,MB_ICONSTOP);
		return FALSE;
	}

	//��������
	WNDCLASS WndClasss;
	ZeroMemory(&WndClasss,sizeof(WndClasss));

	//ע�ᴰ��
	WndClasss.style=CS_DBLCLKS;
	WndClasss.lpfnWndProc=DefWindowProc;
	WndClasss.lpszClassName=MAIN_DLG_CLASSNAME;
	WndClasss.hInstance=AfxGetInstanceHandle();
	WndClasss.hIcon=LoadIcon(MAKEINTRESOURCE(IDR_MAINFRAME));
	WndClasss.hCursor=LoadStandardCursor(MAKEINTRESOURCE(IDC_ARROW));
	if (AfxRegisterClass(&WndClasss)==FALSE) AfxThrowResourceException();

	//��������
	CGameFrame * pMainFrame=new CGameFrame();
	pMainFrame->Create(MAIN_DLG_CLASSNAME,TEXT(""),WS_MINIMIZEBOX|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,CRect(0,0,0,0));
	m_pMainWnd=pMainFrame;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

