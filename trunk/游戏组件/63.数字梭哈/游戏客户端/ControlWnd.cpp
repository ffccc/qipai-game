#include "Stdafx.h"
#include "GameClient.h"
#include "ControlWnd.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//属性定义
#define INVALID_ITEM					0xFFFF							//无效索引

//按钮标识
#define IDC_MAX_SCORE					100								//最大按钮
#define IDC_LESS_SCORE					101								//最小按钮
#define IDC_SEND_SCORE					102								//放出按钮

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_MAX_SCORE, OnBnClickedScoreMax)
	ON_BN_CLICKED(IDC_LESS_SCORE, OnBnClickedScoreLess)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CControlWnd::CControlWnd()
{
	//设置变量
	m_lScoreMax=0L;
	m_lScoreLess=0L;

	//状态变量
	m_wItemCount=0;
	m_lScoreCurrent=0L;
	m_wCurrentItem = INVALID_ITEM;

	//位置变量
	m_BenchmarkPos.SetPoint(0,0);

	//加载资源
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_ImageControlL.SetLoadInfo(IDB_CONTROL_L,hResInstance);
	m_ImageControlM.SetLoadInfo(IDB_CONTROL_M,hResInstance);
	m_ImageControlR.SetLoadInfo(IDB_CONTROL_R,hResInstance);
	m_ImageArrow.SetLoadInfo(IDB_CONTROL_ARROW,hResInstance);
	m_ImageNumber.SetLoadInfo(IDB_CONTROL_NUMBER,hResInstance);
	m_ImageNumberDot.SetLoadInfo( IDB_CONTROL_NUMBER_DOT,hResInstance );

	//获取大小
	CImageHandle HandleControlL(&m_ImageControlL);
	CImageHandle HandleControlM(&m_ImageControlM);
	CImageHandle HandleControlR(&m_ImageControlR);
	m_SizeItem.SetSize(m_ImageControlM.GetWidth(),m_ImageControlM.GetHeight());
	m_SizeControl.SetSize(m_ImageControlL.GetWidth()+m_ImageControlR.GetWidth(),m_SizeItem.cy);

	//辅助尺寸
	m_ControlHead=m_ImageControlL.GetWidth();
	m_ControlTail=m_ImageControlR.GetWidth();

	return;
}

//析构函数
CControlWnd::~CControlWnd()
{
}

//设置位置
VOID CControlWnd::SetBenchmarkPos(INT nXPos, INT nYPos)
{
	//设置变量
	m_BenchmarkPos.SetPoint(nXPos,nYPos);

	//调整控件
	if (m_hWnd!=NULL)
	{
		//获取位置
		CRect rcClient;
		GetClientRect(&rcClient);

		//移动窗口
		SetWindowPos(NULL,nXPos-rcClient.Width(),nYPos-rcClient.Height(),0,0,SWP_NOSIZE|SWP_NOZORDER);
	}

	return;
}

//设置信息
VOID CControlWnd::SetScoreControl(LONG lScoreMax, LONG lScoreLess, LONG lScoreCurrent)
{
	//设置变量
	m_lScoreMax=lScoreMax;
	m_lScoreLess=lScoreLess;
	m_lScoreCurrent=lScoreCurrent;

	//状态变量
	m_wItemCount=0;
	m_lScoreCurrent=__min(m_lScoreMax,__max(m_lScoreCurrent,m_lScoreLess));
	m_wCurrentItem = INVALID_ITEM;

	//计算数目
	do
	{
		lScoreMax/=10L;
		m_wItemCount++;
	} while (lScoreMax>0L);

	//移动窗口
	INT nControlWidth=m_wItemCount*m_SizeItem.cx+m_SizeControl.cx;
	SetWindowPos(NULL,m_BenchmarkPos.x-nControlWidth,m_BenchmarkPos.y-m_SizeControl.cy,nControlWidth,m_SizeControl.cy,SWP_NOZORDER|SWP_SHOWWINDOW);

	//刷新界面
	Invalidate(FALSE);

	return;
}

//播放声音
VOID CControlWnd::PlayGoldSound()
{
	//播放声音
	return;
}

//转换索引
WORD CControlWnd::SwitchToIndex(CPoint MousePoint)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//位置判断
	if ((MousePoint.x>m_ControlHead)&&(MousePoint.x<(rcClient.Width()-m_ControlTail))&&
		MousePoint.y>5&&MousePoint.y<rcClient.Height()-5)
	{
		//变量定义
		INT nXPos=MousePoint.x-m_ControlHead;

		//寻找索引
		return m_wItemCount-WORD(nXPos/m_SizeItem.cx)-1;
	}

	return INVALID_ITEM;
}

//最大按钮
VOID CControlWnd::OnBnClickedScoreMax()
{
	//设置判断
	if (m_lScoreCurrent!=m_lScoreMax)
	{
		//设置变量
		m_lScoreCurrent=m_lScoreMax;

		//播放声音
		PlayGoldSound();

		//更新界面
		Invalidate(FALSE);

		//发送消息
		AfxGetMainWnd()->PostMessage( IDM_ADD_SCORE_CONTROL,m_lScoreCurrent );
	}

	return;
}

//最小按钮
VOID CControlWnd::OnBnClickedScoreLess()
{
	//设置判断
	if (m_lScoreCurrent!=m_lScoreLess)
	{
		//设置变量
		m_lScoreCurrent=m_lScoreLess;

		//播放声音
		PlayGoldSound();

		//更新界面
		Invalidate(FALSE);

		//发送消息
		AfxGetMainWnd()->PostMessage( IDM_ADD_SCORE_CONTROL,m_lScoreCurrent );
	}

	return;
}

//重画函数
VOID CControlWnd::OnPaint()
{
	CPaintDC dc(this);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//加载资源
	CImageHandle HandleControlL(&m_ImageControlL);
	CImageHandle HandleControlM(&m_ImageControlM);
	CImageHandle HandleControlR(&m_ImageControlR);
	CImageHandle HandleArrow(&m_ImageArrow);

	//创建缓冲
	CDC BufferDC;
	CBitmap BufferImage;
	BufferDC.CreateCompatibleDC(&dc);
	BufferImage.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//设置背景
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SelectObject(&BufferImage);

	//绘画背景
	m_ImageControlL.BitBlt(BufferDC,0,0);
	m_ImageControlR.BitBlt(BufferDC,rcClient.Width()-m_ImageControlR.GetWidth(),0);

	//加载资源
	CImageHandle HandleNumber(&m_ImageNumber);
	CImageHandle HandleNumDot(&m_ImageNumberDot);

	//变量定义
	LONG lScoreCurrent=m_lScoreCurrent;
	INT nNumberHeight=m_ImageNumber.GetHeight();
	INT nNumberWidht=m_ImageNumber.GetWidth()/10;
	INT nXItemPos=rcClient.Width()-m_ImageControlR.GetWidth();

	//绘画子项
	for (WORD i=0;i<m_wItemCount;i++)
	{
		//绘画背景
		nXItemPos-=m_SizeItem.cx;
		m_ImageControlM.BitBlt(BufferDC,nXItemPos,0);

		//绘画号码
		LONG lNumber=(LONG)(lScoreCurrent%10L);
		m_ImageNumber.BitBlt(BufferDC.m_hDC,nXItemPos,12,nNumberWidht,nNumberHeight,lNumber*nNumberWidht,0);

		//绘画逗号
		if( i != 0 && i%3 == 0 )
			m_ImageNumberDot.AlphaDrawImage( &BufferDC,nXItemPos+m_SizeItem.cx-4,4,RGB(255,0,255) );

		//当前焦点
		if( m_wCurrentItem == i )
			m_ImageArrow.AlphaDrawImage(&BufferDC,nXItemPos+6,4,RGB(255,0,255));

		//设置变量
		lScoreCurrent/=10L;
	}

	//绘画界面
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

	//清理资源
	BufferDC.DeleteDC();
	BufferImage.DeleteObject();

	return;
}

//位置消息
VOID CControlWnd::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//移动控件
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//调整控件
	DeferWindowPos(hDwp,m_btMaxScore,NULL,cx-28,4,0,0,uFlags);
	DeferWindowPos(hDwp,m_btLessScore,NULL,cx-28,26,0,0,uFlags);

	//结束移动
	EndDeferWindowPos(hDwp);

	return;
}

//建立消息
INT CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//创建按钮
	CRect rcCreate(0,0,0,0);
	m_btMaxScore.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_MAX_SCORE);
	m_btLessScore.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_LESS_SCORE);

	//设置按钮
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btMaxScore.SetButtonImage(IDB_BT_CONTROL_MAX,hResInstance,false);
	m_btLessScore.SetButtonImage(IDB_BT_CONTROL_LESS,hResInstance,false);

	return 0;
}

//鼠标消息
VOID CControlWnd::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags, Point);

	//获取索引
	WORD wItemIndex=SwitchToIndex(Point);

	//设置变量
	if (wItemIndex!=INVALID_ITEM)
	{
		//计算增量
		LONG lIncrease=1L;
		for (WORD i=0;i<wItemIndex;i++) lIncrease*=10;

		//调整变量
		LONG lScoreCurrent=m_lScoreCurrent+lIncrease;
		lScoreCurrent=__min(m_lScoreMax,__max(lScoreCurrent,m_lScoreLess));

		//设置控件
		if (lScoreCurrent!=m_lScoreCurrent)
		{
			//设置变量
			m_lScoreCurrent=lScoreCurrent;

			//播放声音
			PlayGoldSound();

			//更新界面
			Invalidate(FALSE);

			//发送消息
			AfxGetMainWnd()->PostMessage( IDM_ADD_SCORE_CONTROL,m_lScoreCurrent );
		}

		return;
	}

	return;
}

//鼠标消息
VOID CControlWnd::OnRButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnRButtonUp(nFlags, Point);

	//获取索引
	WORD wItemIndex=SwitchToIndex(Point);

	//设置变量
	if (wItemIndex!=INVALID_ITEM)
	{
		//计算增量
		LONG lDecrease=1L;
		for (WORD i=0;i<wItemIndex;i++) lDecrease*=10;

		//调整变量
		LONG lScoreCurrent=m_lScoreCurrent-lDecrease;
		lScoreCurrent=__min(m_lScoreMax,__max(lScoreCurrent,m_lScoreLess));

		//设置控件
		if (lScoreCurrent!=m_lScoreCurrent)
		{
			//设置变量
			m_lScoreCurrent=lScoreCurrent;

			//播放声音
			PlayGoldSound();

			//更新界面
			Invalidate(FALSE);

			//发送消息
			AfxGetMainWnd()->PostMessage( IDM_ADD_SCORE_CONTROL,m_lScoreCurrent );
		}

		return;
	}

	return;
}

//光标消息
BOOL CControlWnd::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//获取光标
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//转换索引
	WORD wPreItem = m_wCurrentItem;
	m_wCurrentItem=SwitchToIndex(MousePoint);

	if( wPreItem != m_wCurrentItem )
	{
		CRect rc;
		GetClientRect( &rc );
		rc.DeflateRect( m_ControlHead,0,m_ControlTail,0 );
		InvalidateRect( &rc,FALSE );
	}

	//设置光标
	if (m_wCurrentItem!=INVALID_ITEM)
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
		return TRUE;
	}

	return __super::OnSetCursor(pWnd, nHitTest, uMessage);
}

//////////////////////////////////////////////////////////////////////////
