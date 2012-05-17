#include "StdAfx.h"
#include "ExDispID.h"
#include "GamePlaza.h"
#include "PlazaViewItem.h"
#include "WndViewItemCtrl.h"

//////////////////////////////////////////////////////////////////////////////////

//控件标识
#define IDC_PLAZA_BROWSER			100									//浏览控件

//////////////////////////////////////////////////////////////////////////////////

//静态变量
CPlazaViewItem * CPlazaViewItem::m_pPlazaViewItem=NULL;					//广场指针

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

//构造函数
CPlazaBrowser::CPlazaBrowser()
{
}

//析构函数
CPlazaBrowser::~CPlazaBrowser()
{
}

//绑定函数
VOID CPlazaBrowser::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//设置背景
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	return;
}

//消耗处理
VOID CPlazaBrowser::PostNcDestroy()
{
	return;
}

//创建函数
BOOL CPlazaBrowser::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT & rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	//设置变量
	m_pCreateContext=pContext;

	//创建窗口
	if (CView::Create(lpszClassName,lpszWindowName,dwStyle,rect,pParentWnd,nID,pContext)==FALSE)
	{
		return FALSE;
	}

	//获取大小
	CRect rcClient;
	GetClientRect(&rcClient);

	//创建窗口
	if (m_wndBrowser.CreateControl(CLSID_WebBrowser,lpszWindowName,WS_VISIBLE|WS_CHILD,rcClient,this,AFX_IDW_PANE_FIRST)==FALSE)
	{
		DestroyWindow();
		return FALSE;
	}

	//查询接口
	LPUNKNOWN lpUnknown=m_wndBrowser.GetControlUnknown();
	lpUnknown->QueryInterface(IID_IWebBrowser2,(VOID **)&m_pBrowserApp);

	return TRUE;
}

//关闭事件
void CPlazaBrowser::OnWindowClosing(BOOL IsChildWindow, BOOL *bCancel)   
{   
	//禁止关闭
	if ((IsChildWindow==FALSE)&&(bCancel!=NULL))
	{
		*bCancel=TRUE;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CPlazaViewItem::CPlazaViewItem() : CDialog(IDD_GAME_PLAZA)
{
	//设置变量
	m_bCreateFlag=false;

	//设置对象
	ASSERT(m_pPlazaViewItem==NULL);
	if (m_pPlazaViewItem==NULL) m_pPlazaViewItem=this;

	//浏览资源
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

//析构函数
CPlazaViewItem::~CPlazaViewItem()
{
}

//接口查询
VOID * CPlazaViewItem::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE_IUNKNOWNEX(IUnknownEx,Guid,dwQueryVer);
	return NULL;
}

//控件绑定
VOID CPlazaViewItem::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WEB_LAST, m_btWebLast);
	DDX_Control(pDX, IDC_WEB_NEXT, m_btWebNext);
	DDX_Control(pDX, IDC_WEB_STOP, m_btWebStop);
	DDX_Control(pDX, IDC_WEB_HOME, m_btWebHome);
	DDX_Control(pDX, IDC_WEB_RELOAD, m_btWebReload);
}

//创建函数
BOOL CPlazaViewItem::OnInitDialog()
{
	__super::OnInitDialog();

	//创建控件
	CRect rcCreate(0,0,0,0);
	m_PlazaBrowser.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_PLAZA_BROWSER,NULL);

	//网页按钮
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btWebLast.SetButtonImage(IDB_BT_BROWSER,TEXT("BT_LAST"),hInstance,true,false);
	m_btWebNext.SetButtonImage(IDB_BT_BROWSER,TEXT("BT_NEXT"),hInstance,true,false);
	m_btWebStop.SetButtonImage(IDB_BT_BROWSER,TEXT("BT_STOP"),hInstance,true,false);
	m_btWebHome.SetButtonImage(IDB_BT_BROWSER,TEXT("BT_HOME"),hInstance,true,false);
	m_btWebReload.SetButtonImage(IDB_BT_BROWSER,TEXT("BT_RELOAD"),hInstance,true,false);

	//提示控件
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btWebLast,TEXT("后退"));
	m_ToolTipCtrl.AddTool(&m_btWebNext,TEXT("前进"));
	m_ToolTipCtrl.AddTool(&m_btWebStop,TEXT("停止"));
	m_ToolTipCtrl.AddTool(&m_btWebHome,TEXT("主页"));
	m_ToolTipCtrl.AddTool(&m_btWebReload,TEXT("刷新"));

	//设置变量
	m_bCreateFlag=true;

	return TRUE;
}

//消息过虑
BOOL CPlazaViewItem::PreTranslateMessage(MSG * pMsg)
{
	//提示消息
	if (m_ToolTipCtrl.m_hWnd!=NULL) 
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	return __super::PreTranslateMessage(pMsg);
}

//命令函数
BOOL CPlazaViewItem::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID=LOWORD(wParam);

	//菜单命令
	switch (nCommandID)
	{
	case IDC_WEB_LAST:		//后退按钮
		{
			m_PlazaBrowser.GoBack();
			return TRUE;
		}
	case IDC_WEB_NEXT:		//前进按钮
		{
			m_PlazaBrowser.GoForward();
			return TRUE;
		}
	case IDC_WEB_STOP:		//停止按钮
		{
			m_PlazaBrowser.Stop();
			return TRUE;
		}
	case IDC_WEB_HOME:		//主页按钮
		{
			WebBrowse(szPlatformLink);
			return TRUE;
		}
	case IDC_WEB_RELOAD:	//刷新按钮
		{
			m_PlazaBrowser.Refresh();
			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//位置消息
VOID CPlazaViewItem::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);

	return;
}

//绘画背景
BOOL CPlazaViewItem::OnEraseBkgnd(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//绘画网页
	CRect rcBrowse;
	rcBrowse.top=0;
	rcBrowse.left=0;
	rcBrowse.right=rcClient.Width();
	rcBrowse.bottom=rcClient.Height();
	m_BrowserEncircle.DrawEncircleFrame(pDC,rcBrowse);

	//绘画衔接
	ASSERT(CWndViewItemCtrl::GetInstance()!=NULL);
	CWndViewItemCtrl::GetInstance()->DrawLinkBoradLine(this,pDC);

	return TRUE;
}

//浏览页面
bool CPlazaViewItem::WebBrowse(LPCTSTR pszURL)
{
	m_PlazaBrowser.Navigate(pszURL,NULL,NULL,NULL,NULL);

	return true;
}

//调整控件
VOID CPlazaViewItem::RectifyControl(INT nWidth, INT nHeight)
{
	//状态判断
	if (m_bCreateFlag==false) return;
	if ((nWidth==0)||(nHeight==0)) return;

	//移动准备
	HDWP hDwp=BeginDeferWindowPos(32);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//网页区域
	CRect rcWebButton;
	m_btWebLast.GetWindowRect(&rcWebButton);
	CSkinButton * pSkinButtons[]={&m_btWebLast,&m_btWebNext,&m_btWebStop,&m_btWebReload,&m_btWebHome};

	//调整位置
	for (INT i=0;i<CountArray(pSkinButtons);i++)
	{
		INT nXPos=i*(rcWebButton.Width()+8)+9;
		DeferWindowPos(hDwp,pSkinButtons[i]->m_hWnd,NULL,nXPos,3,0,0,uFlags|SWP_NOSIZE);
	}

	//浏览控件
	CRect rcWebBrowse(0,0,nWidth,nHeight);
	m_BrowserEncircle.DeferWindowPos(&m_PlazaBrowser,hDwp,rcWebBrowse);

	//移动结束
	EndDeferWindowPos(hDwp);

	//重画界面
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
