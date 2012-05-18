#include "StdAfx.h"
#include "ExDispID.h"
#include "GamePlaza.h"
#include "PlazaViewItem.h"
#include "WndViewItemCtrl.h"

//////////////////////////////////////////////////////////////////////////////////

//�ؼ���ʶ
#define IDC_PLAZA_BROWSER			100									//����ؼ�

//////////////////////////////////////////////////////////////////////////////////

//��̬����
CPlazaViewItem * CPlazaViewItem::m_pPlazaViewItem=NULL;					//�㳡ָ��

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlazaViewItem, CDialog)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CPlazaBrowser, CHtmlView)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CPlazaBrowser, CHtmlView)
	ON_EVENT(CPlazaBrowser, AFX_IDW_PANE_FIRST, DISPID_WINDOWCLOSING, OnWindowClosing,VTS_BOOL VTS_PBOOL)   
END_EVENTSINK_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CPlazaBrowser::CPlazaBrowser()
{
}

//��������
CPlazaBrowser::~CPlazaBrowser()
{
}

//�󶨺���
VOID CPlazaBrowser::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//���ñ���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	return;
}

//���Ĵ���
VOID CPlazaBrowser::PostNcDestroy()
{
	return;
}

//��������
BOOL CPlazaBrowser::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT & rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	//���ñ���
	m_pCreateContext=pContext;

	//��������
	if (CView::Create(lpszClassName,lpszWindowName,dwStyle,rect,pParentWnd,nID,pContext)==FALSE)
	{
		return FALSE;
	}

	//��ȡ��С
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	if (m_wndBrowser.CreateControl(CLSID_WebBrowser,lpszWindowName,WS_VISIBLE|WS_CHILD,rcClient,this,AFX_IDW_PANE_FIRST)==FALSE)
	{
		DestroyWindow();
		return FALSE;
	}

	//��ѯ�ӿ�
	LPUNKNOWN lpUnknown=m_wndBrowser.GetControlUnknown();
	lpUnknown->QueryInterface(IID_IWebBrowser2,(VOID **)&m_pBrowserApp);

	return TRUE;
}

//�ر��¼�
void CPlazaBrowser::OnWindowClosing(BOOL IsChildWindow, BOOL *bCancel)   
{   
	//��ֹ�ر�
	if ((IsChildWindow==FALSE)&&(bCancel!=NULL))
	{
		*bCancel=TRUE;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CPlazaViewItem::CPlazaViewItem() : CDialog(IDD_GAME_PLAZA)
{
	//���ñ���
	m_bCreateFlag=false;

	//���ö���
	ASSERT(m_pPlazaViewItem==NULL);
	if (m_pPlazaViewItem==NULL) m_pPlazaViewItem=this;

	//�����Դ
	tagEncircleResource	EncircleBrowser;
	EncircleBrowser.pszImageTL=MAKEINTRESOURCE(IDB_BROWSER_TL);
	EncircleBrowser.pszImageTM=MAKEINTRESOURCE(IDB_BROWSER_TM);
	EncircleBrowser.pszImageTR=MAKEINTRESOURCE(IDB_BROWSER_TR);
	EncircleBrowser.pszImageML=MAKEINTRESOURCE(IDB_BROWSER_ML);
	EncircleBrowser.pszImageMR=MAKEINTRESOURCE(IDB_BROWSER_MR);
	EncircleBrowser.pszImageBL=MAKEINTRESOURCE(IDB_BROWSER_BL);
	EncircleBrowser.pszImageBM=MAKEINTRESOURCE(IDB_BROWSER_BM);
	EncircleBrowser.pszImageBR=MAKEINTRESOURCE(IDB_BROWSER_BR);
	m_BrowserEncircle.InitEncircleResource(EncircleBrowser,AfxGetInstanceHandle());

	return;
}

//��������
CPlazaViewItem::~CPlazaViewItem()
{
}

//�ӿڲ�ѯ
VOID * CPlazaViewItem::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE_IUNKNOWNEX(IUnknownEx,Guid,dwQueryVer);
	return NULL;
}

//�ؼ���
VOID CPlazaViewItem::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WEB_LAST, m_btWebLast);
	DDX_Control(pDX, IDC_WEB_NEXT, m_btWebNext);
	DDX_Control(pDX, IDC_WEB_STOP, m_btWebStop);
	DDX_Control(pDX, IDC_WEB_HOME, m_btWebHome);
	DDX_Control(pDX, IDC_WEB_RELOAD, m_btWebReload);
}

//��������
BOOL CPlazaViewItem::OnInitDialog()
{
	__super::OnInitDialog();

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	m_PlazaBrowser.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_PLAZA_BROWSER,NULL);

	//��ҳ��ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btWebLast.SetButtonImage(IDB_BT_BROWSER,TEXT("BT_LAST"),hInstance,true,false);
	m_btWebNext.SetButtonImage(IDB_BT_BROWSER,TEXT("BT_NEXT"),hInstance,true,false);
	m_btWebStop.SetButtonImage(IDB_BT_BROWSER,TEXT("BT_STOP"),hInstance,true,false);
	m_btWebHome.SetButtonImage(IDB_BT_BROWSER,TEXT("BT_HOME"),hInstance,true,false);
	m_btWebReload.SetButtonImage(IDB_BT_BROWSER,TEXT("BT_RELOAD"),hInstance,true,false);

	//��ʾ�ؼ�
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btWebLast,TEXT("����"));
	m_ToolTipCtrl.AddTool(&m_btWebNext,TEXT("ǰ��"));
	m_ToolTipCtrl.AddTool(&m_btWebStop,TEXT("ֹͣ"));
	m_ToolTipCtrl.AddTool(&m_btWebHome,TEXT("��ҳ"));
	m_ToolTipCtrl.AddTool(&m_btWebReload,TEXT("ˢ��"));

	//���ñ���
	m_bCreateFlag=true;

	return TRUE;
}

//��Ϣ����
BOOL CPlazaViewItem::PreTranslateMessage(MSG * pMsg)
{
	//��ʾ��Ϣ
	if (m_ToolTipCtrl.m_hWnd!=NULL) 
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	return __super::PreTranslateMessage(pMsg);
}

//�����
BOOL CPlazaViewItem::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	//�˵�����
	switch (nCommandID)
	{
	case IDC_WEB_LAST:		//���˰�ť
		{
			m_PlazaBrowser.GoBack();
			return TRUE;
		}
	case IDC_WEB_NEXT:		//ǰ����ť
		{
			m_PlazaBrowser.GoForward();
			return TRUE;
		}
	case IDC_WEB_STOP:		//ֹͣ��ť
		{
			m_PlazaBrowser.Stop();
			return TRUE;
		}
	case IDC_WEB_HOME:		//��ҳ��ť
		{
			WebBrowse(szPlatformLink);
			return TRUE;
		}
	case IDC_WEB_RELOAD:	//ˢ�°�ť
		{
			m_PlazaBrowser.Refresh();
			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//λ����Ϣ
VOID CPlazaViewItem::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}

//�滭����
BOOL CPlazaViewItem::OnEraseBkgnd(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�滭��ҳ
	CRect rcBrowse;
	rcBrowse.top=0;
	rcBrowse.left=0;
	rcBrowse.right=rcClient.Width();
	rcBrowse.bottom=rcClient.Height();
	m_BrowserEncircle.DrawEncircleFrame(pDC,rcBrowse);

	//�滭�ν�
	ASSERT(CWndViewItemCtrl::GetInstance()!=NULL);
	CWndViewItemCtrl::GetInstance()->DrawLinkBoradLine(this,pDC);

	return TRUE;
}

//���ҳ��
bool CPlazaViewItem::WebBrowse(LPCTSTR pszURL)
{
	m_PlazaBrowser.Navigate(pszURL,NULL,NULL,NULL,NULL);

	return true;
}

//�����ؼ�
VOID CPlazaViewItem::RectifyControl(INT nWidth, INT nHeight)
{
	//״̬�ж�
	if (m_bCreateFlag==false) return;
	if ((nWidth==0)||(nHeight==0)) return;

	//�ƶ�׼��
	HDWP hDwp=BeginDeferWindowPos(32);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//��ҳ����
	CRect rcWebButton;
	m_btWebLast.GetWindowRect(&rcWebButton);
	CSkinButton * pSkinButtons[]={&m_btWebLast,&m_btWebNext,&m_btWebStop,&m_btWebReload,&m_btWebHome};

	//����λ��
	for (INT i=0;i<CountArray(pSkinButtons);i++)
	{
		INT nXPos=i*(rcWebButton.Width()+8)+9;
		DeferWindowPos(hDwp,pSkinButtons[i]->m_hWnd,NULL,nXPos,3,0,0,uFlags|SWP_NOSIZE);
	}

	//����ؼ�
	CRect rcWebBrowse(0,0,nWidth,nHeight);
	m_BrowserEncircle.DeferWindowPos(&m_PlazaBrowser,hDwp,rcWebBrowse);

	//�ƶ�����
	EndDeferWindowPos(hDwp);

	//�ػ�����
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
