#include "Stdafx.h"
#include "DlgEnquire.h"
#include "GlobalUnits.h"
#include "PlatformFrame.h"

//////////////////////////////////////////////////////////////////////////////////

//圆角大小
#define ROUND_CX					5									//圆角宽度
#define ROUND_CY					5									//圆角高度

//屏幕位置
#define LAYERED_SIZE				5									//分层大小
#define CAPTION_SIZE				60									//标题大小

//控件标识
#define IDC_WEB_PUBLICIZE			300									//浏览控件

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgEnquire, CDialog)

	//系统消息
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGED()

	//按钮消息
	ON_BN_CLICKED(IDC_CLOSE_PLAZA, OnBnClickedClosePlaza)
	ON_BN_CLICKED(IDC_CLOSE_SERVER, OnBnClickedCloseServer)
	ON_BN_CLICKED(IDC_SWITCH_ACCOUNTS, OnBnClickedSwitchAccounts)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgEnquire::CDlgEnquire() : CDialog(IDD_DLG_ENQUIRE)
{
}

//析构函数
CDlgEnquire::~CDlgEnquire()
{
}

//控件绑定
VOID CDlgEnquire::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//按钮控件
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_CLOSE_PLAZA, m_btClosePlaza);
	DDX_Control(pDX, IDC_CLOSE_SERVER, m_btCloseServer);
	DDX_Control(pDX, IDC_SWITCH_ACCOUNTS, m_btSwitchAccounts);
}

//配置函数
BOOL CDlgEnquire::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("温馨提示"));

	//加载资源
	CPngImage ImageBack;
	ImageBack.LoadImage(AfxGetInstanceHandle(),TEXT("DLG_ENQUIRE_BACK"));

	//设置大小
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());
	SetWindowPos(NULL,0,0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	//控制按钮
	CWndViewItemCtrl * pWndViewItemCtrl=CWndViewItemCtrl::GetInstance();
	m_btCloseServer.EnableWindow((pWndViewItemCtrl->GetActiveItem()>=1)?TRUE:FALSE);

	//设置按钮
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btCancel.SetButtonImage(IDB_BT_ENQUIRE_CANCEL,TEXT("BT_ENQUIRE_CANCEL"),hInstance,false,false);
	m_btClosePlaza.SetButtonImage(IDB_BK_ENQUIRE_BUTTON,TEXT("BT_ENQUIRE_PLAZA"),hInstance,false,false);
	m_btCloseServer.SetButtonImage(IDB_BK_ENQUIRE_BUTTON,TEXT("BT_ENQUIRE_SERVER"),hInstance,false,false);
	m_btSwitchAccounts.SetButtonImage(IDB_BK_ENQUIRE_BUTTON,TEXT("BT_ENQUIRE_SWITCH"),hInstance,false,false);

	//关闭按钮
	CRect rcCancel;
	m_btCancel.GetWindowRect(&rcCancel);
	m_btCancel.SetWindowPos(NULL,SizeWindow.cx-rcCancel.Width()-15,15,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOREDRAW);

	//按钮大小
	CRect rcButton;
	m_btClosePlaza.GetWindowRect(&rcButton);

	//功能按钮
	INT nButtonSpace=(SizeWindow.cx-rcButton.Width()*3)/4;
	m_btClosePlaza.SetWindowPos(NULL,nButtonSpace*3+rcButton.Width()*2,148,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOREDRAW);
	m_btCloseServer.SetWindowPos(NULL,nButtonSpace*2+rcButton.Width()*1,148,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOREDRAW);
	m_btSwitchAccounts.SetWindowPos(NULL,nButtonSpace*1+rcButton.Width()*0,148,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOREDRAW);

	//广告控件
	CRect rcCreate(0,0,0,0);
	m_PlatformPublicize.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_WEB_PUBLICIZE);

	//构造地址
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/Ads/ClosePlaza.asp"),szPlatformLink);

	//设置广告
	m_PlatformPublicize.Navigate(szBillUrl);
	m_PlatformPublicize.SetWindowPos(NULL,7,58,SizeWindow.cx-14,80,SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOACTIVATE);

	//居中窗口
	CenterWindow(this);

	//获取窗口
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//计算位置
	CRect rcUnLayered;
	rcUnLayered.top=LAYERED_SIZE;
	rcUnLayered.left=LAYERED_SIZE;
	rcUnLayered.right=rcWindow.Width()-LAYERED_SIZE;
	rcUnLayered.bottom=rcWindow.Height()-LAYERED_SIZE;

	//设置区域
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(LAYERED_SIZE,LAYERED_SIZE,SizeWindow.cx-LAYERED_SIZE+1,SizeWindow.cy-LAYERED_SIZE+1,ROUND_CX,ROUND_CY);

	//设置区域
	SetWindowRgn(RgnWindow,FALSE);

	//分层窗口
	m_SkinLayered.CreateLayered(this,rcWindow);
	m_SkinLayered.InitLayeredArea(ImageBack,255,rcUnLayered,CPoint(ROUND_CX,ROUND_CY),false);

	return FALSE;
}

//确定函数
VOID CDlgEnquire::OnOK()
{
	return;
}

//绘画背景
BOOL CDlgEnquire::OnEraseBkgnd(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//加载资源
	CPngImage ImageBack;
	ImageBack.LoadImage(AfxGetInstanceHandle(),TEXT("DLG_ENQUIRE_BACK"));

	//绘画背景
	ImageBack.DrawImage(pDC,0,0);

	return TRUE;
}

//显示消息
VOID CDlgEnquire::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	//显示分层
	if (m_SkinLayered.m_hWnd!=NULL)
	{
		m_SkinLayered.ShowWindow((bShow==FALSE)?SW_HIDE:SW_SHOW);
	}

	return;
}

//鼠标消息
VOID CDlgEnquire::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//模拟标题
	if (Point.y<=CAPTION_SIZE)
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	return;
}

//位置改变
VOID CDlgEnquire::OnWindowPosChanged(WINDOWPOS * lpWndPos)
{
	__super::OnWindowPosChanging(lpWndPos);

	//移动分层
	if ((m_SkinLayered.m_hWnd!=NULL)&&(lpWndPos->cx>=0)&&(lpWndPos->cy>0))
	{
		m_SkinLayered.SetWindowPos(NULL,lpWndPos->x,lpWndPos->y,lpWndPos->cx,lpWndPos->cy,SWP_NOZORDER);
	}

	return;
}

//关闭大厅
VOID CDlgEnquire::OnBnClickedClosePlaza()
{
	EndDialog(IDC_CLOSE_PLAZA);

	return;
}

//关闭房间
VOID CDlgEnquire::OnBnClickedCloseServer()
{
	EndDialog(IDC_CLOSE_SERVER);

	return;
}

//切换帐号
VOID CDlgEnquire::OnBnClickedSwitchAccounts()
{
	EndDialog(IDC_SWITCH_ACCOUNTS);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
