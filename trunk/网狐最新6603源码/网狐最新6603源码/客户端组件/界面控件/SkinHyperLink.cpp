#include "Stdafx.h"
#include "Resource.h"
#include "SkinHyperLink.h"
#include "SkinResourceManager.h"

//////////////////////////////////////////////////////////////////////////////////

//静态变量
CSkinHyperLinkAttribute				CSkinHyperLink::m_SkinAttribute;	//控件属性

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSkinHyperLink, CStatic)

	//系统消息
	ON_WM_PAINT()
	ON_WM_CREATE()
    ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()

	//自定消息
	ON_MESSAGE(WM_SETTEXT, OnSetTextMesage)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeaveMessage)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinHyperLinkAttribute::CSkinHyperLinkAttribute()
{
	//设置变量
	m_crBackColor=CLR_INVALID;
	m_crHoverText=CLR_INVALID;
	m_crNormalText=CLR_INVALID;
	m_crVisitedText=CLR_INVALID;

	return;
}

//析构函数
CSkinHyperLinkAttribute::~CSkinHyperLinkAttribute()
{
}

//配置资源
bool CSkinHyperLinkAttribute::Initialization(CSkinRenderManager * pSkinRenderManager)
{
	//设置变量
	m_crHoverText=RGB(250,0,0);
	m_crNormalText=RGB(0,0,220);
	m_crVisitedText=RGB(0,0,100);

	//背景颜色
	m_crBackColor=RGB(250,250,250);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinHyperLink::CSkinHyperLink()
{
	//状态变量
	m_bVisited=false;
	m_bHovering=false;

	//配置变量
	m_bUnderLine=true;
	m_AutoRectify=true;

	//颜色变量
	m_crBackColor=CLR_INVALID;
	m_crHoverText=CLR_INVALID;
	m_crNormalText=CLR_INVALID;
	m_crVisitedText=CLR_INVALID;

	return;
}

//析构函数
CSkinHyperLink::~CSkinHyperLink()
{
}

//控件绑定
VOID CSkinHyperLink::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//配置控件
	Initialization();

	return;
}

//建立消息
INT CSkinHyperLink::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//配置控件
	Initialization();

	return 0;
}

//绘画消息
VOID CSkinHyperLink::OnPaint()
{
	CPaintDC dc(this);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//设置环境
	dc.SetBkMode(TRANSPARENT);
	dc.SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//绘画背景
	if (m_crBackColor!=CLR_INVALID)
	{
		//自定背景
		dc.FillSolidRect(&rcClient,m_crBackColor);
	}
	else 
	{
		//默认背景
		dc.FillSolidRect(&rcClient,m_SkinAttribute.m_crBackColor);
	}

	//设置颜色
	if (m_bHovering==true)
	{
		//经过颜色
		dc.SetTextColor((m_crHoverText==CLR_INVALID)?m_SkinAttribute.m_crHoverText:m_crHoverText);
	}
	else if (m_bVisited==true)
	{
		//访问颜色
		dc.SetTextColor((m_crVisitedText==CLR_INVALID)?m_SkinAttribute.m_crVisitedText:m_crVisitedText);
	}
	else
	{
		//常规颜色
		dc.SetTextColor((m_crNormalText==CLR_INVALID)?m_SkinAttribute.m_crNormalText:m_crNormalText);
	}

	//获取格式
	LONG dwStyte=GetWindowLong(m_hWnd,GWL_STYLE);
	UINT uFormat=DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
	if (dwStyte&SS_RIGHT) uFormat|=DT_RIGHT;
	else if (dwStyte&SS_CENTER) uFormat|=DT_CENTER;
	else uFormat|=DT_LEFT;

	//绘画文字
	CString strText;
	GetWindowText(strText);
	dc.DrawText(strText,&rcClient,uFormat);

	return;
}

//光标消息
BOOL CSkinHyperLink::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//设置光标
	::SetCursor(LoadCursor(GetModuleHandle(SKIN_CONTROL_DLL_NAME),MAKEINTRESOURCE(IDC_SKIN_CURSOR)));

	return TRUE;
}

//鼠标消息
VOID CSkinHyperLink::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	//状态判断
	if (m_bHovering==false)
	{
		//设置变量
		m_bHovering=true;

		//注册消息
		TRACKMOUSEEVENT TrackMouseEvent;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.hwndTrack=GetSafeHwnd();
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		_TrackMouseEvent(&TrackMouseEvent);

		//重画窗口
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//标题消息
LRESULT	CSkinHyperLink::OnSetTextMesage(WPARAM wParam, LPARAM lParam)
{
	//默认调用
	LRESULT lResult=Default();

	//调整控件
	if (m_AutoRectify==true) RectifyControl();

	return lResult;
}

//鼠标消息
LRESULT CSkinHyperLink::OnMouseLeaveMessage(WPARAM wParam, LPARAM lParam)
{
	//设置变量
	m_bHovering=false;

	//重画窗口
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);

	return 0;
}

//设置访问
VOID CSkinHyperLink::SetVisited(bool bVisited)
{
	//状态判断
	if (m_bVisited!=bVisited)
	{
		//设置变量
		m_bVisited=bVisited;

		//重画窗口
		if (m_hWnd!=NULL)
		{
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
		}
	}

	return;
}

//设下划线
VOID CSkinHyperLink::SetUnderLine(bool bUnderLine)
{
	//状态判断
	if (m_bUnderLine!=bUnderLine)
	{
		//设置变量
		m_bUnderLine=bUnderLine;

		//重画窗口
		if (m_hWnd!=NULL)
		{
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
		}
	}

	return;
}

//设置调整
VOID CSkinHyperLink::SetAutoRectify(bool bAutoRectify)
{
	//状态判断
	if (m_AutoRectify!=bAutoRectify)
	{
		//设置变量
		m_AutoRectify=bAutoRectify;

		//调整控件
		if (m_AutoRectify==true) RectifyControl();
	}

	return;
}

//背景颜色
VOID CSkinHyperLink::SetBackGroundColor(COLORREF crBackColor)
{
	//设置变量
	m_crBackColor=crBackColor;

	//重画窗口
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//字体颜色
VOID CSkinHyperLink::SetHyperLinkTextColor(COLORREF crHoverText, COLORREF crNormalText, COLORREF crVisitedText)
{
	//设置变量
	m_crHoverText=crHoverText;
	m_crNormalText=crNormalText;
	m_crVisitedText=crVisitedText;

	//重画窗口
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//调整位置
VOID CSkinHyperLink::RectifyControl()
{
	//获取标题
	CString strText;
	GetWindowText(strText);

	//获取位置
	CRect rcControl;
	GetWindowRect(&rcControl);
	GetParent()->ScreenToClient(&rcControl);

	//计算长度
	CClientDC ClientDC(this);
	ClientDC.SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//移动位置
	CSize Size=ClientDC.GetTextExtent(strText);
	MoveWindow(rcControl.left,rcControl.top,Size.cx,Size.cy);

	return;
}

//配置控件
VOID CSkinHyperLink::Initialization()
{
	//调整窗口
	if (m_AutoRectify=true) RectifyControl();

	//设置属性
	SetWindowLong(m_hWnd,GWL_STYLE,GetStyle()|SS_NOTIFY);

	return;
}

//////////////////////////////////////////////////////////////////////////////////

