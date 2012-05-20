#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClient.h"

//////////////////////////////////////////////////////////////////////////
//按钮标识

#define IDC_START						100								//开始按钮
#define IDC_OUT_CARD					101								//出牌按钮
#define IDC_PASS_CARD					102								//PASS按钮
#define IDC_OUT_PROMPT					103								//提示按钮
#define IDC_SORT_BY_COLOR				104								//花色排序
#define IDC_SORT_BY_VALUE				105								//大小排序
#define IDC_STRUSTEE					106								//托管

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_OUT_CARD, OnOutCard)
	ON_BN_CLICKED(IDC_PASS_CARD, OnPassCard)
	ON_BN_CLICKED(IDC_OUT_PROMPT, OnOutPrompt)
	ON_BN_CLICKED(IDC_SORT_BY_COLOR, OnSortByColor)
	ON_BN_CLICKED(IDC_SORT_BY_VALUE, OnSortByValue)
	ON_BN_CLICKED(IDC_STRUSTEE, OnStrustee)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientView::CGameClientView(void) : CGameFrameView(true,24)
{
	//状态变量
	memset(m_bPass,0,sizeof(m_bPass));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	ZeroMemory(m_bStrustee,sizeof(m_bStrustee));

	//历史积分
	memset(m_lAllTurnScore,0,sizeof(m_lAllTurnScore));
	memset(m_lLastTurnScore,0,sizeof(m_lLastTurnScore));

	//加载资源
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageCardHeap.SetLoadInfo(IDB_CARD_HEAP,hInstance);
	m_ImageViewFill.SetLoadInfo(IDB_VIEW_FILL,hInstance);
	m_ImageViewBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageUserPass.SetLoadInfo(IDB_USER_PASS,hInstance);
	m_PngStrustee.LoadImage(hInstance,(LPCTSTR)IDP_STRUSTEE);

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

	//创建控件
	CRect CreateRect(0,0,0,0);
	m_ScoreView.Create(NULL,NULL,WS_CHILD,CreateRect,this,9);

	//创建扑克
	for (WORD i=0;i<4;i++)
	{
		//用户扑克
		m_UserCardControl[i].SetDirection(true);
		m_UserCardControl[i].SetDisplayFlag(true);
		m_UserCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS,CreateRect,this,20+i);

		//用户扑克
		LONG lStyle;
#ifdef	VIDEO_GAME
		if( i != 2 ) lStyle = WS_CHILD|WS_CLIPSIBLINGS;
		else lStyle = WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS;
#else
		lStyle = WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS;
#endif
		if ((i%2)==0)
		{
			m_HandCardControl[i].SetDirection(true);
			m_HandCardControl[i].SetDisplayFlag(false);
			m_HandCardControl[i].Create(NULL,NULL,lStyle,CreateRect,this,30+i);
		}
		else
		{
			m_HandCardControl[i].SetDirection(false);
			m_HandCardControl[i].SetDisplayFlag(false);
			m_HandCardControl[i].Create(NULL,NULL,lStyle,CreateRect,this,30+i);
		}

		//设置间距
		if (i!=2) m_HandCardControl[i].SetCardSpace(4,4,0);
	}

	//用户扑克
	m_HandCardControl[2].SetSinkWindow(AfxGetMainWnd());

	//创建按钮
	m_btStart.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,CreateRect,this,IDC_START);
	m_btOutCard.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,CreateRect,this,IDC_OUT_CARD);
	m_btPassCard.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,CreateRect,this,IDC_PASS_CARD);
	m_btOutPrompt.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,CreateRect,this,IDC_OUT_PROMPT);
	m_btSortByColor.Create(TEXT(""),WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,CreateRect,this,IDC_SORT_BY_COLOR);
	m_btSortByValue.Create(TEXT(""),WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,CreateRect,this,IDC_SORT_BY_VALUE);
	m_btStrustee.Create(TEXT(""),WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,CreateRect,this,IDC_STRUSTEE	);

	//设置按钮
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_START,hInstance,false);
	m_btOutCard.SetButtonImage(IDB_OUT_CARD,hInstance,false);
	m_btPassCard.SetButtonImage(IDB_PASS,hInstance,false);
	m_btOutPrompt.SetButtonImage(IDB_OUT_PROMPT,hInstance,false);
	m_btSortByColor.SetButtonImage(IDB_SORT_BY_COLOR,hInstance,false);
	m_btSortByValue.SetButtonImage(IDB_SORT_BY_ORDER,hInstance,false);
	m_btStrustee.SetButtonImage(IDB_START_TRUSTEE,hInstance,false);

		//创建视频
#ifdef VIDEO_GAME
	for (WORD i=0;i<4;i++)
	{
		//创建视频
		m_DlgVedioService[i].Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,CreateRect,this,70+i);
		m_DlgVedioService[i].InitVideoService(i==2,i==2);

		//设置视频
		g_VedioServiceManager.SetVideoServiceControl(i,&m_DlgVedioService[i]);
	}
#endif

#ifdef	TEST
	m_btStart.ShowWindow(SW_SHOW);
	m_btOutCard.ShowWindow(SW_SHOW);
	m_btOutPrompt.ShowWindow(SW_SHOW);
	m_btPassCard.ShowWindow(SW_SHOW);
	BYTE byCard[MAX_COUNT] = 
	{
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D
	};
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		m_bStrustee[i] = true;
		m_UserCardControl[i].SetCardData(byCard,5);
		m_HandCardControl[i].SetCardData(byCard,MAX_COUNT);
		m_ScoreView.SetGameScore(i,TEXT("S"),1L);
	}
	m_HandCardControl[2].SetDisplayFlag(true);
	m_HandCardControl[2].SetPositively(true);
	m_ScoreView.ShowWindow(SW_SHOW);
#endif

	return 0;
}

//鼠标消息
void CGameClientView::OnRButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnRButtonUp(nFlags, Point);

	//设置扑克
	m_btOutCard.EnableWindow(FALSE);
	m_HandCardControl[2].ShootAllCard(false);

	return;
}

//鼠标双击
void CGameClientView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	__super::OnLButtonDblClk(nFlags, point);

	//设置扑克
	m_btOutCard.EnableWindow(FALSE);
	m_HandCardControl[2].ShootAllCard(false);

	return;
}

//重置界面
void CGameClientView::ResetGameView()
{
	//状态变量
	memset(m_bPass,0,sizeof(m_bPass));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	ZeroMemory(m_bStrustee,sizeof(m_bStrustee));

	//历史积分
	memset(m_lAllTurnScore,0,sizeof(m_lAllTurnScore));
	memset(m_lLastTurnScore,0,sizeof(m_lLastTurnScore));

	//隐藏控件
	m_ScoreView.ShowWindow(SW_HIDE);

	//隐藏按钮
	m_btStart.ShowWindow(SW_HIDE);
	m_btOutCard.ShowWindow(SW_HIDE);
	m_btPassCard.ShowWindow(SW_HIDE);
	m_btOutPrompt.ShowWindow(SW_HIDE);

	//禁用控件
	m_btOutCard.EnableWindow(FALSE);
	m_btPassCard.EnableWindow(FALSE);

	//扑克控件
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_UserCardControl[i].SetCardData(NULL,0);
		m_HandCardControl[i].SetCardData(NULL,0);
		m_HandCardControl[i].SetPositively(false);
		m_HandCardControl[i].SetDisplayFlag(false);
	}

	return;
}

//调整控件
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
#ifdef VIDEO_GAME
	//设置坐标
	m_ptFace[0].x=nWidth/2-m_nXFace/2;
	m_ptFace[0].y=m_nYBorder+5;
	m_ptName[0].x=nWidth/2+5+m_nXFace/2;
	m_ptName[0].y=m_nYBorder+7;
	m_ptTimer[0].x=nWidth/2-m_nXFace/2-m_nXTimer/2-5;
	m_ptTimer[0].y=m_nYBorder+5+m_nYTimer/2;
	m_ptReady[0].x=nWidth/2-m_nXFace/2-50;
	m_ptReady[0].y=m_nYBorder+25;
	m_PointUserPass[0].x=nWidth/2-21;
	m_PointUserPass[0].y=m_nYBorder+m_nYFace+120;
	m_ptStrustee[0].x = nWidth/2-m_nXFace/2-m_nXTimer-80;
	m_ptStrustee[0].y = m_nYBorder+5;
	SetFlowerControlInfo( 0,m_ptFace[0].x+m_nXFace/2-BIG_FACE_WIDTH/2,m_ptFace[0].y+m_nYFace );

	//设置坐标
	m_ptFace[3].x=nWidth-m_nXBorder-m_nXFace-5;
	m_ptFace[3].y=nHeight/2-m_nYFace-32;
	m_ptName[3].x=nWidth-m_nXBorder-5;
	m_ptName[3].y=nHeight/2+5-32;
	m_ptTimer[3].x=nWidth-m_nXBorder-m_nXFace/2-8;
	m_ptTimer[3].y=nHeight/2-m_nYFace-m_nYTimer/2-5-32;
	m_ptReady[3].x=nWidth-m_nXBorder-45;
	m_ptReady[3].y=nHeight/2-m_nYFace-25-32;
	m_PointUserPass[3].x=nWidth-m_nXBorder-m_nXFace-210;
	m_PointUserPass[3].y=nHeight/2-30-32;
	m_ptStrustee[3].x = nWidth-m_nXBorder-m_nXFace-40;
	m_ptStrustee[3].y = nHeight/2-m_nYFace-32;
	SetFlowerControlInfo( 3,m_ptFace[3].x-BIG_FACE_WIDTH,m_ptFace[3].y+m_nYFace/2-BIG_FACE_HEIGHT );

	//设置坐标
	m_ptFace[2].x=nWidth/2-m_nXFace/2;
	m_ptFace[2].y=nHeight-m_nYBorder-m_nYFace-5;
	m_ptName[2].x=nWidth/2+5+m_nXFace/2;
	m_ptName[2].y=nHeight-m_nYBorder-m_nYFace;
	m_ptTimer[2].x=nWidth/2-m_nXFace/2-m_nXTimer/2-5;
	m_ptTimer[2].y=nHeight-m_nYBorder-m_nYTimer/2-5;
	m_ptReady[2].x=nWidth/2-m_nXFace/2-50;
	m_ptReady[2].y=nHeight-m_nYBorder-25;
	m_PointUserPass[2].x=nWidth/2-21;
	m_PointUserPass[2].y=nHeight-m_nYFace-m_nYBorder-180-64;
	m_ptStrustee[2].x = nWidth/2-m_nXFace/2-m_nXTimer-80;
	m_ptStrustee[2].y = nHeight-m_nYBorder-45;
	SetFlowerControlInfo( 2,m_ptFace[2].x+m_nXFace/2-BIG_FACE_WIDTH/2,m_ptFace[2].y-BIG_FACE_HEIGHT );

	//设置坐标
	m_ptFace[1].x=m_nXBorder+5;
	m_ptFace[1].y=nHeight/2-m_nYFace-32;
	m_ptName[1].x=m_nXBorder+5;
	m_ptName[1].y=nHeight/2+5-32;
	m_ptTimer[1].x=m_nXBorder+5+m_nXTimer/2;
	m_ptTimer[1].y=nHeight/2-m_nYFace-5-m_nYTimer/2-32;
	m_ptReady[1].x=m_nXBorder+45;
	m_ptReady[1].y=nHeight/2-m_nYFace-25-32;
	m_PointUserPass[1].x=m_nXBorder+m_nXFace+210;
	m_PointUserPass[1].y=nHeight/2-30-32;
	m_ptStrustee[1].x = m_nXBorder+m_nXFace+15;
	m_ptStrustee[1].y = nHeight/2-m_nYFace-32;
	SetFlowerControlInfo( 1,m_ptFace[1].x,m_ptFace[1].y+m_nYFace/2-BIG_FACE_HEIGHT/2 );

	//移动按钮
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	m_btStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-208-m_nYBorder,0,0,uFlags);

	m_btOutCard.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btOutCard,NULL,nWidth/2-rcButton.Width()*3/2-5,nHeight-210-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPassCard,NULL,nWidth/2-rcButton.Width()/2,nHeight-210-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOutPrompt,NULL,nWidth/2+rcButton.Width()/2+5,nHeight-210-m_nYBorder,0,0,uFlags);

	//排序按钮
	m_btSortByColor.GetWindowRect(&rcButton);	
	DeferWindowPos(hDwp,m_btStrustee,NULL,nWidth-m_nXBorder-rcButton.Width()-5,nHeight-m_nYBorder-rcButton.Height()-5,0,0,uFlags);
	DeferWindowPos(hDwp,m_btSortByColor,NULL,nWidth-m_nXBorder-rcButton.Width()-5,nHeight-m_nYBorder-rcButton.Height()*2-10,0,0,uFlags);
	DeferWindowPos(hDwp,m_btSortByValue,NULL,nWidth-m_nXBorder-rcButton.Width()-5,nHeight-m_nYBorder-rcButton.Height()*3-15,0,0,uFlags);

	//视频窗口
	CRect rcAVDlg;
	m_DlgVedioService[0].GetWindowRect(&rcAVDlg);
	DeferWindowPos(hDwp,m_DlgVedioService[3],NULL,nWidth-m_nXBorder-5-rcAVDlg.Width(),nHeight/2+42-32,0,0,uFlags);
	DeferWindowPos(hDwp,m_DlgVedioService[1],NULL,m_nXBorder+5,nHeight/2+42-32,0,0,uFlags);
	DeferWindowPos(hDwp,m_DlgVedioService[0],NULL,nWidth-m_nXBorder-5-rcAVDlg.Width(),5,0,0,uFlags);
	m_DlgVedioService[2].GetWindowRect(&rcAVDlg);
	DeferWindowPos(hDwp,m_DlgVedioService[2],NULL,m_nXBorder+5,nHeight-m_nYBorder-3-rcAVDlg.Height(),0,0,uFlags);

	EndDeferWindowPos(hDwp);

	//用户扑克
	m_HandCardControl[2].SetBenchmarkPos(nWidth/2,nHeight-m_nYFace-m_nYBorder-22,enXCenter,enYBottom);

	//用户扑克
	m_UserCardControl[0].SetBenchmarkPos(nWidth/2,m_nYBorder+m_nYFace+120-64,enXCenter,enYTop);
	m_UserCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+150,nHeight/2-30-32,enXLeft,enYCenter);
	m_UserCardControl[2].SetBenchmarkPos(nWidth/2,nHeight-m_nYFace-m_nYBorder-180,enXCenter,enYBottom);
	m_UserCardControl[3].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-150,nHeight/2-30-32,enXRight,enYCenter);

	//积分视图
	CRect rcControl;
	m_ScoreView.GetWindowRect(&rcControl);
	m_ScoreView.SetWindowPos(NULL,(nWidth-rcControl.Width())/2,(nHeight-rcControl.Height())/2-20,0,0,SWP_NOZORDER|SWP_NOSIZE);

#else	// !VIDEO_GAME

	//设置坐标
	m_ptFace[0].x=nWidth/2-m_nXFace/2;
	m_ptFace[0].y=m_nYBorder+5;
	m_ptName[0].x=nWidth/2+5+m_nXFace/2;
	m_ptName[0].y=m_nYBorder+7;
	m_ptTimer[0].x=nWidth/2-m_nXFace/2-m_nXTimer/2-5;
	m_ptTimer[0].y=m_nYBorder+5+m_nYTimer/2;
	m_ptReady[0].x=nWidth/2-m_nXFace/2-50;
	m_ptReady[0].y=m_nYBorder+25;
	m_PointUserPass[0].x=nWidth/2-21;
	m_PointUserPass[0].y=m_nYBorder+m_nYFace+120+64;
	m_ptStrustee[0].x = nWidth/2-m_nXFace/2-m_nXTimer-80;
	m_ptStrustee[0].y = m_nYBorder+5;
	SetFlowerControlInfo( 0,m_ptFace[0].x+m_nXFace/2-BIG_FACE_WIDTH/2,m_ptFace[0].y+m_nYFace );

	//设置坐标
	m_ptFace[3].x=nWidth-m_nXBorder-m_nXFace-5;
	m_ptFace[3].y=nHeight/2-m_nYFace;
	m_ptName[3].x=nWidth-m_nXBorder-5;
	m_ptName[3].y=nHeight/2+5;
	m_ptTimer[3].x=nWidth-m_nXBorder-m_nXFace/2-8;
	m_ptTimer[3].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
	m_ptReady[3].x=nWidth-m_nXBorder-45;
	m_ptReady[3].y=nHeight/2-m_nYFace-25;
	m_PointUserPass[3].x=nWidth-m_nXBorder-m_nXFace-210;
	m_PointUserPass[3].y=nHeight/2-30;
	m_ptStrustee[3].x = nWidth-m_nXBorder-m_nXFace-40;
	m_ptStrustee[3].y = nHeight/2-m_nYFace;
	SetFlowerControlInfo( 3,m_ptFace[3].x-BIG_FACE_WIDTH,m_ptFace[3].y+m_nYFace/2-BIG_FACE_HEIGHT );

	//设置坐标
	m_ptFace[2].x=nWidth/2-m_nXFace/2;
	m_ptFace[2].y=nHeight-m_nYBorder-m_nYFace-5;
	m_ptName[2].x=nWidth/2+5+m_nXFace/2;
	m_ptName[2].y=nHeight-m_nYBorder-m_nYFace;
	m_ptTimer[2].x=nWidth/2-m_nXFace/2-m_nXTimer/2-5;
	m_ptTimer[2].y=nHeight-m_nYBorder-m_nYTimer/2-5;
	m_ptReady[2].x=nWidth/2-m_nXFace/2-50;
	m_ptReady[2].y=nHeight-m_nYBorder-25;
	m_PointUserPass[2].x=nWidth/2-21;
	m_PointUserPass[2].y=nHeight-m_nYFace-m_nYBorder-180-64;
	m_ptStrustee[2].x = nWidth/2-m_nXFace/2-m_nXTimer-80;
	m_ptStrustee[2].y = nHeight-m_nYBorder-45;
	SetFlowerControlInfo( 2,m_ptFace[2].x+m_nXFace/2-BIG_FACE_WIDTH/2,m_ptFace[2].y-BIG_FACE_HEIGHT );

	//设置坐标
	m_ptFace[1].x=m_nXBorder+5;
	m_ptFace[1].y=nHeight/2-m_nYFace;
	m_ptName[1].x=m_nXBorder+5;
	m_ptName[1].y=nHeight/2+5;
	m_ptTimer[1].x=m_nXBorder+5+m_nXTimer/2;
	m_ptTimer[1].y=nHeight/2-m_nYFace-5-m_nYTimer/2;
	m_ptReady[1].x=m_nXBorder+45;
	m_ptReady[1].y=nHeight/2-m_nYFace-25;
	m_PointUserPass[1].x=m_nXBorder+m_nXFace+210;
	m_PointUserPass[1].y=nHeight/2-30;
	m_ptStrustee[1].x = m_nXBorder+m_nXFace+15;
	m_ptStrustee[1].y = nHeight/2-m_nYFace;
	SetFlowerControlInfo( 1,m_ptFace[1].x,m_ptFace[1].y+m_nYFace/2-BIG_FACE_HEIGHT/2 );

	//移动按钮
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	m_btStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-208-m_nYBorder,0,0,uFlags);

	m_btOutCard.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btOutCard,NULL,nWidth/2-rcButton.Width()*3/2-5,nHeight-210-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPassCard,NULL,nWidth/2-rcButton.Width()/2,nHeight-210-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOutPrompt,NULL,nWidth/2+rcButton.Width()/2+5,nHeight-210-m_nYBorder,0,0,uFlags);

	//排序按钮
	m_btSortByColor.GetWindowRect(&rcButton);	
	DeferWindowPos(hDwp,m_btStrustee,NULL,nWidth-m_nXBorder-rcButton.Width()-5,nHeight-m_nYBorder-rcButton.Height()-5,0,0,uFlags);
	DeferWindowPos(hDwp,m_btSortByColor,NULL,nWidth-m_nXBorder-rcButton.Width()-5,nHeight-m_nYBorder-rcButton.Height()*2-10,0,0,uFlags);
	DeferWindowPos(hDwp,m_btSortByValue,NULL,nWidth-m_nXBorder-rcButton.Width()-5,nHeight-m_nYBorder-rcButton.Height()*3-15,0,0,uFlags);

	EndDeferWindowPos(hDwp);

	//用户扑克
	m_HandCardControl[0].SetBenchmarkPos(nWidth/2,m_nYBorder+m_nYFace+22,enXCenter,enYTop);
	m_HandCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+60,nHeight/2-30,enXLeft,enYCenter);
	m_HandCardControl[2].SetBenchmarkPos(nWidth/2,nHeight-m_nYFace-m_nYBorder-22,enXCenter,enYBottom);
	m_HandCardControl[3].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-60,nHeight/2-30,enXRight,enYCenter);

	//用户扑克
	m_UserCardControl[0].SetBenchmarkPos(nWidth/2,m_nYBorder+m_nYFace+120,enXCenter,enYTop);
	m_UserCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+150,nHeight/2-30,enXLeft,enYCenter);
	m_UserCardControl[2].SetBenchmarkPos(nWidth/2,nHeight-m_nYFace-m_nYBorder-180,enXCenter,enYBottom);
	m_UserCardControl[3].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-150,nHeight/2-30,enXRight,enYCenter);

	//积分视图
	CRect rcControl;
	m_ScoreView.GetWindowRect(&rcControl);
	m_ScoreView.SetWindowPos(NULL,(nWidth-rcControl.Width())/2,(nHeight-rcControl.Height())/2-20,0,0,SWP_NOZORDER|SWP_NOSIZE);

#endif	//VIDEO_GAME

	return;
}

//绘画界面
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//绘画背景
	DrawViewImage(pDC,m_ImageViewFill,enMode_Spread);
	DrawViewImage(pDC,m_ImageViewBack,enMode_Centent);

	//绘画用户
	TCHAR szBuffer[32];
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//变量定义
#ifndef	TEST
		WORD wUserTimer=GetUserTimer(i);
		const tagUserData * pUserData=GetUserInfo(i);
#else
		WORD wUserTimer = 30;
		tagUserData *pUserData = new tagUserData;
		lstrcpy(pUserData->szName,TEXT("测试玩家"));
		pUserData->lScore = 10000L;
		pUserData->cbUserStatus = US_READY;
#endif
		//绘画用户
		if (pUserData!=NULL)
		{
			//用户名字
			pDC->SetTextAlign((i==3)?TA_RIGHT:TA_LEFT);
			pDC->SetTextColor((wUserTimer>0)?RGB(250,250,250):RGB(220,220,220));

			//构造帐号
			pDC->TextOut(m_ptName[i].x,m_ptName[i].y,pUserData->szName,lstrlen(pUserData->szName));

			//剩余扑克
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("剩余：%d 张"),m_bCardCount[i]);
			pDC->TextOut(m_ptName[i].x,m_ptName[i].y+18,szBuffer,lstrlen(szBuffer));

			//其他信息
			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);

			//托管标志
			if( m_bStrustee[i] )
				m_PngStrustee.DrawImage(pDC,m_ptStrustee[i].x,m_ptStrustee[i].y);
		}
#ifdef	TEST
		delete pUserData;
#endif
	}

	//放弃标志
	CImageHandle ImageHnadleUserPass(&m_ImageUserPass);
#ifdef	TEST
		memset(m_bPass,true,sizeof(m_bPass));
#endif
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_bPass[i]==true) 
		{
			int nXPos=m_PointUserPass[i].x-m_ImageUserPass.GetWidth()/2;
			m_ImageUserPass.AlphaDrawImage(pDC,nXPos,m_PointUserPass[i].y,RGB(255,0,255));
		}
	}

	return;
}

//设置放弃
void CGameClientView::SetPassFlag(WORD wChairID, bool bPass)
{
	//设置变量
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) m_bPass[i]=bPass;
	}
	else m_bPass[wChairID]=bPass;

	//更新界面
	UpdateGameView(NULL);

	return;
}

//扑克数目
void CGameClientView::SetCardCount(WORD wChairID, BYTE bCardCount)
{
	//设置变量
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) m_bCardCount[i]=bCardCount;
	}
	else m_bCardCount[wChairID]=bCardCount;

	//更新界面
	UpdateGameView(NULL);

	return;
}

//开始按钮
void CGameClientView::OnStart()
{
	AfxGetMainWnd()->SendMessage(IDM_START,0,0);
	return;
}

//出牌按钮
void CGameClientView::OnOutCard()
{
	AfxGetMainWnd()->SendMessage(IDM_OUT_CARD,1,1);
	return;
}

//放弃按钮
void CGameClientView::OnPassCard()
{
	AfxGetMainWnd()->SendMessage(IDM_PASS_CARD,1,1);
	return;
}

//提示按钮
void CGameClientView::OnOutPrompt()
{
	AfxGetMainWnd()->SendMessage(IDM_OUT_PROMPT,0,0);
	return;
}

//花色排序
void CGameClientView::OnSortByColor()
{
	AfxGetMainWnd()->SendMessage(IDM_SORT_BY_COLOR,0,0);
	return;
}

//大小排序
void CGameClientView::OnSortByValue()
{
	AfxGetMainWnd()->SendMessage(IDM_SORT_BY_VALUE,0,0);
	return;
}

//托管
void CGameClientView::OnStrustee()
{
	AfxGetMainWnd()->SendMessage(IDM_STRUSTEE,0,0);
	return;
}

//设置托管标志
void CGameClientView::SetStrustee( WORD wChairId,bool bStrustee )
{
	if( INVALID_CHAIR == wChairId )
	{
		memset(m_bStrustee,bStrustee,sizeof(m_bStrustee));
	}else
	{
		m_bStrustee[wChairId] = bStrustee;
	}
	UpdateGameView(NULL);
	return;
}

//////////////////////////////////////////////////////////////////////////

