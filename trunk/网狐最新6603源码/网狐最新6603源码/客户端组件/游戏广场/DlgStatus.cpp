#include "StdAfx.h"
#include "DlgStatus.h"
#include "PlatformFrame.h"
#include "ServerViewItem.h"

//////////////////////////////////////////////////////////////////////////////////

//圆角大小
#define ROUND_CX					5									//圆角宽度
#define ROUND_CY					5									//圆角高度

//时间标识
#define IDT_STATUS					10									//状态时间
#define TIMER_STATUS				20									//状态时间

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgStatus, CDialog)

	//系统消息
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()

	//分层消息
	ON_WM_SHOWWINDOW()
	ON_WM_WINDOWPOSCHANGED()

	//控件消息
	ON_BN_CLICKED(IDC_GIVE_UP, OnBnClickedGiveUp)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgStatus::CDlgStatus() : CDialog(IDD_DLG_STATUS)
{
	//状态变量
	m_uImageIndex=0;

	//接口变量
	m_pIStatusViewSink=NULL;

	return;
}

//析构函数
CDlgStatus::~CDlgStatus()
{
}

//控件绑定
VOID CDlgStatus::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GIVE_UP, m_btGiveUp);
}

//消息解释
BOOL CDlgStatus::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if ((pMsg->message==WM_KEYDOWN)&&((pMsg->wParam==VK_ESCAPE)||(pMsg->wParam==VK_RETURN)))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//配置函数
BOOL CDlgStatus::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(szProduct);

	//设置控件
	SetTimer(IDT_STATUS,TIMER_STATUS,NULL);

	//加载资源
	CPngImage ImageStatus;
	ImageStatus.LoadImage(AfxGetInstanceHandle(),TEXT("STATUS_IMAGE"));

	//设置按钮
	m_btGiveUp.SetWindowPos(NULL,348,39,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btGiveUp.SetButtonImage(IDB_BT_STATUS_CANCEL,AfxGetInstanceHandle(),false,false);

	//设置大小
	CSize SizeWindow(ImageStatus.GetWidth(),ImageStatus.GetHeight());
	SetWindowPos(NULL,0,0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	//居中窗口
	CenterWindow();

	//获取窗口
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//计算位置
	CRect rcUnLayered;
	rcUnLayered.top=35;
	rcUnLayered.left=30;
	rcUnLayered.right=rcWindow.Width()-10;
	rcUnLayered.bottom=rcWindow.Height()-10;

	//设置区域
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(rcUnLayered.left,rcUnLayered.top,rcUnLayered.right+1,rcUnLayered.bottom+1,ROUND_CX,ROUND_CY);

	//设置区域
	SetWindowRgn(RgnWindow,FALSE);

	//分层窗口
	m_SkinLayered.CreateLayered(this,rcWindow);
	m_SkinLayered.InitLayeredArea(ImageStatus,255,rcUnLayered,CPoint(ROUND_CX,ROUND_CY),false);

	return TRUE;
}

//关闭窗口
VOID CDlgStatus::HideStatusWindow()
{
	//关闭窗口
	if (m_hWnd!=NULL) DestroyWindow();

	return;
}

//设置消息
VOID CDlgStatus::ShowStatusWindow(LPCTSTR pszDescribe)
{
	//创建窗口
	if (m_hWnd==NULL)
	{
		if (CPlatformFrame::GetInstance()->IsWindowVisible()==FALSE)
		{
			Create(IDD_DLG_STATUS,GetDesktopWindow());
		}
		else
		{
			Create(IDD_DLG_STATUS,CPlatformFrame::GetInstance());
		}
	}

	//设置描述
	m_strDescribe=pszDescribe;

	//显示窗口
	ShowWindow(SW_SHOW);

	//更新窗口
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

	return;
}

//设置接口
VOID CDlgStatus::SetStatusViewSink(IStatusViewSink * pIStatusViewSink)
{
	//设置变量
	m_pIStatusViewSink=pIStatusViewSink;

	return;
}

//时间消息
VOID CDlgStatus::OnTimer(UINT nIDEvent)
{
	//设置进度
	if (nIDEvent==IDT_STATUS)
	{
		//设置索引
		m_uImageIndex=(m_uImageIndex>=100)?0:(m_uImageIndex+1L);

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//取消按钮
VOID CDlgStatus::OnBnClickedGiveUp()
{
	//隐藏界面
	HideStatusWindow();

	//投递消息
	ASSERT(m_pIStatusViewSink!=NULL);
	if (m_pIStatusViewSink!=NULL) m_pIStatusViewSink->OnStatusCancel();

	return;
}

//绘画背景
BOOL CDlgStatus::OnEraseBkgnd(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//创建缓冲
	CBitImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//创建设备
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//设置环境
	pBufferDC->SetBkMode(TRANSPARENT);
	pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//绘画背景
	CPngImage ImageStatus;
	ImageStatus.LoadImage(AfxGetInstanceHandle(),TEXT("STATUS_IMAGE"));
	ImageStatus.DrawImage(pBufferDC,0,0,rcClient.Width(),rcClient.Height(),0,0);

	//加载资源
	CPngImage ImageStatusLine;
	CPngImage ImageStatusPoint;
	ImageStatusLine.LoadImage(AfxGetInstanceHandle(),TEXT("STATUS_LINE"));
	ImageStatusPoint.LoadImage(AfxGetInstanceHandle(),TEXT("STATUS_POINT"));

	//获取大小
	CSize SizeStatusLine;
	CSize SizeStatusPoint;
	SizeStatusLine.SetSize(ImageStatusLine.GetWidth(),ImageStatusLine.GetHeight()/2);
	SizeStatusPoint.SetSize(ImageStatusPoint.GetWidth(),ImageStatusPoint.GetHeight());

	//绘画进度
	ImageStatusLine.DrawImage(pBufferDC,105,45,SizeStatusLine.cx,SizeStatusLine.cy,0,0);
	ImageStatusLine.DrawImage(pBufferDC,105,45,SizeStatusLine.cx*m_uImageIndex/100L,SizeStatusLine.cy,0,SizeStatusLine.cy);

	//绘画进度
	INT nXPoint=SizeStatusLine.cx*m_uImageIndex/100L;
	ImageStatusPoint.DrawImage(pBufferDC,105+nXPoint-SizeStatusPoint.cx/2,45+(SizeStatusLine.cy-SizeStatusPoint.cy)/2);

	//文字位置
	CRect rcDescribe;
	rcDescribe.SetRect(115,72,320,84);

	//绘画文字
	pBufferDC->SetTextColor(RGB(0,0,0));
	pBufferDC->DrawText(m_strDescribe,rcDescribe,DT_VCENTER|DT_END_ELLIPSIS|DT_SINGLELINE);

	//绘画界面
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//显示消息
VOID CDlgStatus::OnShowWindow(BOOL bShow, UINT nStatus)
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
VOID CDlgStatus::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//模拟标题
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));

	return;
}

//位置改变
VOID CDlgStatus::OnWindowPosChanged(WINDOWPOS * lpWndPos)
{
	__super::OnWindowPosChanging(lpWndPos);

	//移动分层
	if ((m_SkinLayered.m_hWnd!=NULL)&&(lpWndPos->cx>=0)&&(lpWndPos->cy>0))
	{
		m_SkinLayered.SetWindowPos(NULL,lpWndPos->x,lpWndPos->y,lpWndPos->cx,lpWndPos->cy,SWP_NOZORDER);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
