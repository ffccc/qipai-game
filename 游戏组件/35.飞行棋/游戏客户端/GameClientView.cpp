#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientView::CGameClientView(void) : CGameFrameView(false,NULL)
{
	return;
}

//析构函数
CGameClientView::~CGameClientView(void)
{
}

//建立消息
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;
	//创建棋盘
	m_wndD3D.CreateEx(0,AfxRegisterWndClass(0,NULL,NULL,NULL),"00",
		WS_CHILD|WS_VISIBLE,CRect(5,6,750, 720),this,0);
	m_wndD3D.ShowWindow(SW_SHOW);
	SetTimer(0,10000000,NULL);
	if(!m_GameScoreView.Create(IDD_SCORE_VIEW, &m_wndD3D))
	{
		return -1;
	}
	m_GameScoreView.ShowWindow(SW_HIDE);
	return 0;
}

//重置界面
void CGameClientView::ResetGameView()
{
	return;
}

//调整控件
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	return;
}

//绘画界面
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//绘画界面
	pDC->FillSolidRect(0,0,nWidth,nHeight,RGB(255,255,0));

	return;
}

//////////////////////////////////////////////////////////////////////////
