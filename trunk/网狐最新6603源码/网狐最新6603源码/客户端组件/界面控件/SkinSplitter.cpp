#include "Stdafx.h"
#include "SkinSplitter.h"
#include "SkinResourceManager.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSkinSplitter, CWnd)
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinSplitter::CSkinSplitter()
{
	//状态变量
	m_bMouseDown=false;
	m_rcDrawLine.SetRect(0,0,0,0);

	//颜色变量
	m_ColorS=0.99;
	m_ColorB=0.42;
	m_crBackColor=CLR_INVALID;

	//属性变量
	m_nMaxDragPos=0;
	m_nMinDragPos=0;
	m_SplitterType=SplitterType_Vor;

	//组件变量
	m_pISplitterEvent=NULL;

	return;
}

//析构函数
CSkinSplitter::~CSkinSplitter()
{
}

//绑定函数
VOID CSkinSplitter::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//设置属性
	ModifyStyle(0,SS_NOTIFY|SS_OWNERDRAW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS);

	return;
}

//命令消息
BOOL CSkinSplitter::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//消息转发
	GetParent()->SendMessage(WM_COMMAND,wParam,lParam);

	return TRUE;
}

//设置类型
VOID CSkinSplitter::SetSplitterType(enSplitterType SplitterType)
{
	//设置变量
	m_SplitterType=SplitterType;

	return;
}

//设置范围
VOID CSkinSplitter::SetSplitterRange(INT nMinDragPos, INT nMaxDragPos)
{
	//设置变量
	m_nMaxDragPos=nMaxDragPos;
	m_nMinDragPos=nMinDragPos;

	return;
}

//设置接口
VOID CSkinSplitter::SetSplitterEvent(ISplitterEvent * pISplitterEvent)
{
	//设置变量
	m_pISplitterEvent=pISplitterEvent;

	return;
}

//背景颜色
VOID CSkinSplitter::SetSplitterColor(DOUBLE S, DOUBLE B)
{
	//设置变量
	m_ColorS=S;
	m_ColorB=B;
	m_crBackColor=CLR_INVALID;

	//重画窗口
	if (m_hWnd!=NULL) RedrawWindow(NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_INVALIDATE|RDW_ALLCHILDREN|RDW_ERASENOW);

	return;
}

//背景颜色
VOID CSkinSplitter::SetSplitterColor(COLORREF crBackColor)
{
	//设置变量
	m_ColorS=0.0;
	m_ColorB=0.0;
	m_crBackColor=crBackColor;

	//重画窗口
	if (m_hWnd!=NULL) RedrawWindow(NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_INVALIDATE|RDW_ALLCHILDREN|RDW_ERASENOW);

	return;
}

//重画消息
VOID CSkinSplitter::OnPaint()
{
	CPaintDC dc(this);

	//获取大小
	CRect rcClient;
	GetClientRect(&rcClient);

	//绘画背景
	if (m_crBackColor==CLR_INVALID)
	{
		CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();
		dc.FillSolidRect(&rcClient,pSkinRenderManager->RenderColor(m_ColorS,m_ColorB));
	}
	else dc.FillSolidRect(&rcClient,m_crBackColor);

	return;
}

//绘画背景
BOOL CSkinSplitter::OnEraseBkgnd(CDC * pDC)
{
	//更新界面
	Invalidate(FALSE);
	UpdateWindow();

	return TRUE;
}

//失去焦点
VOID CSkinSplitter::OnKillFocus(CWnd * pNewWnd)
{
	__super::OnKillFocus(pNewWnd);

	if (m_bMouseDown==true)
	{
		//获取 DC
		CDC * pDC=CDC::FromHandle(::GetDC(NULL));
		pDC->SelectObject(CDC::GetHalftoneBrush());

		//绘画线条
		pDC->PatBlt(m_rcDrawLine.left,m_rcDrawLine.top,m_rcDrawLine.Width(),m_rcDrawLine.Height(),PATINVERT);

		//释放资源
		ReleaseCapture();
		::ReleaseDC(NULL,pDC->m_hDC);

		//设置变量
		m_bMouseDown=false;
		m_rcDrawLine.SetRectEmpty();
	}

	return;
}

//鼠标消息
VOID CSkinSplitter::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags,Point);

	if (m_bMouseDown==true)
	{
		//获取位置
		CRect rcWindow;
		GetWindowRect(&rcWindow);

		//效验位置
		CPoint PointParent=Point;
		MapWindowPoints(GetParent(),&PointParent,1);

		//控件位置
		CSize SizeControl;
		SizeControl.SetSize(rcWindow.Width(),rcWindow.Height());

		//设置光标
		if (m_SplitterType==SplitterType_Hor)
		{
			if ((PointParent.y>(m_nMaxDragPos-SizeControl.cy/2))||(PointParent.y<(m_nMinDragPos+SizeControl.cy/2)))
			{
				//设置光标
				SetCursor(LoadCursor(NULL,IDC_NO));

				//设置位置
				PointParent.y=__min(m_nMaxDragPos-SizeControl.cy/2,PointParent.y);
				PointParent.y=__max(m_nMinDragPos+SizeControl.cy/2,PointParent.y);
			}
			else SetCursor(LoadCursor(NULL,IDC_SIZENS));
		}
		else
		{
			if ((PointParent.x>(m_nMaxDragPos-SizeControl.cx/2))||(PointParent.x<(m_nMinDragPos+SizeControl.cx/2)))
			{
				//设置光标
				SetCursor(LoadCursor(NULL,IDC_NO));

				//设置位置
				PointParent.x=__min(m_nMaxDragPos-SizeControl.cx/2,PointParent.x);
				PointParent.x=__max(m_nMinDragPos+SizeControl.cx/2,PointParent.x);
			}
			else SetCursor(LoadCursor(NULL,IDC_SIZEWE));
		}

		//转换位置
		CPoint PointScreen=PointParent;
		GetParent()->ClientToScreen(&PointScreen);
		
		//计算区域
		CRect rcCurrentDraw=m_rcDrawLine;
		if (m_SplitterType==SplitterType_Hor)
		{
			rcCurrentDraw.top=PointScreen.y-SizeControl.cy/2;
			rcCurrentDraw.bottom=rcCurrentDraw.top+SizeControl.cy;
		}
		else
		{
			rcCurrentDraw.left=PointScreen.x-SizeControl.cx/2;
			rcCurrentDraw.right=rcCurrentDraw.left+SizeControl.cx;
		}
		
		//绘画线条
		if (rcCurrentDraw!=m_rcDrawLine)
		{
			//设置 DC
			CDC * pDC=CDC::FromHandle(::GetDC(NULL));
			pDC->SelectObject(CDC::GetHalftoneBrush());

			//绘画拆分
			pDC->PatBlt(m_rcDrawLine.left,m_rcDrawLine.top,m_rcDrawLine.Width(),m_rcDrawLine.Height(),PATINVERT);
			pDC->PatBlt(rcCurrentDraw.left,rcCurrentDraw.top,rcCurrentDraw.Width(),rcCurrentDraw.Height(),PATINVERT);

			//释放资源
			::ReleaseDC(NULL,pDC->m_hDC);

			//设置变量
			m_rcDrawLine=rcCurrentDraw;
		}
	}

	return;
}

//鼠标消息
VOID CSkinSplitter::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags, Point);

	if (m_bMouseDown==true)
	{
		//恢复状态
		ReleaseCapture();

		//获取 DC
		CDC * pDC=CDC::FromHandle(::GetDC(NULL));
		pDC->SelectObject(CDC::GetHalftoneBrush());

		//绘画线条
		pDC->PatBlt(m_rcDrawLine.left,m_rcDrawLine.top,m_rcDrawLine.Width(),m_rcDrawLine.Height(),PATINVERT);

		//释放资源
		::ReleaseDC(NULL,pDC->m_hDC);

		//变量定义
		bool bRectify=false;

		//获取位置
		CRect rcControl;
		GetClientRect(&rcControl);
		MapWindowPoints(GetParent(),rcControl);

		//转换坐标
		CPoint PointParent=Point;
		MapWindowPoints(GetParent(),&PointParent,1);

		//控件位置
		CSize SizeControl;
		SizeControl.SetSize(rcControl.Width(),rcControl.Height());

		//调整位置
		if (m_SplitterType==SplitterType_Hor)
		{
			//移动位置
			PointParent.y=__min(m_nMaxDragPos-SizeControl.cy/2,PointParent.y);
			PointParent.y=__max(m_nMinDragPos+SizeControl.cy/2,PointParent.y);

			//移动窗口
			if (rcControl.top!=(PointParent.y-SizeControl.cy/2))
			{
				bRectify=true;
				SetWindowPos(NULL,rcControl.left,PointParent.y-SizeControl.cy/2,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_NOCOPYBITS);
			}
		}
		else
		{
			//移动位置
			PointParent.x=__min(m_nMaxDragPos-SizeControl.cx/2,PointParent.x);
			PointParent.x=__max(m_nMinDragPos+SizeControl.cx/2,PointParent.x);

			//移动窗口
			if (rcControl.left!=(PointParent.x-SizeControl.cx/2))
			{
				bRectify=true;
				SetWindowPos(NULL,PointParent.x-SizeControl.cx/2,rcControl.top,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_NOCOPYBITS);
			}
		}

		//发送通知
		if (bRectify==true)
		{
			//发送通知
			if (m_pISplitterEvent!=NULL) 
			{
				UINT uControlID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pISplitterEvent->OnSplitterEvent(uControlID,PointParent.x,PointParent.y);
			}
		}

		//设置变量
		m_bMouseDown=false;
		m_rcDrawLine.SetRectEmpty();
	}

	return;
}

//鼠标消息
VOID CSkinSplitter::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//设置状态
	SetFocus();
	SetCapture();

	//获取位置
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//设置变量
	m_bMouseDown=true;
	m_rcDrawLine=rcWindow;

	//设置 DC
	CDC * pDC=CDC::FromHandle(::GetDC(NULL));
	pDC->SelectObject(CDC::GetHalftoneBrush());

	//绘画线条
	pDC->PatBlt(m_rcDrawLine.left,m_rcDrawLine.top,m_rcDrawLine.Width(),m_rcDrawLine.Height(),PATINVERT);

	//释放资源
	ReleaseDC(pDC);

	return;
}

//光标消息
BOOL CSkinSplitter::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMmessage)
{
	//获取位置
	CPoint Point;
	GetCursorPos(&Point);

	//设置光标
	if (pWnd==this)
	{
		SetCursor(LoadCursor(NULL,(m_SplitterType==SplitterType_Vor)?IDC_SIZEWE:IDC_SIZENS));
		return TRUE;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMmessage);
}

//////////////////////////////////////////////////////////////////////////////////
