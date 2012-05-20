#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"
#include "SetChip.h"
#include "GameClient.h"

//////////////////////////////////////////////////////////////////////////
//按钮标识 

#define IDC_SHOW_CARD						100								//摊牌按钮
#define IDC_FRONT_CARD						101								//设置前墩
#define IDC_MID_CARD						102								//设置中墩
#define IDC_BACK_CARD						103								//设置后墩
#define	IDC_START							104								//开始按钮
#define IDC_DESCEND_SORT					106								//降序按钮
#define IDC_ASCEND_SORT						107								//升序按钮
#define IDC_COLOR_SORT						108								//花色按钮
#define IDC_AUTO_OUT_CARD				    109                             //自动代打
#define IDC_CANCEL_AUTO_OUT_CARD			110                             //取消代打
#define IDC_BAO_PAI							111								//包牌按钮
#define IDC_RESTORE_CARD					112								//包牌按钮
#define IDC_CALL_BANKER						113								//叫庄按钮
#define IDC_NO_CALL_BANKER					114								//不叫按钮
#define IDC_SET_CHIP_0						115								//压注按钮
#define IDC_SET_CHIP_1						116								//压注按钮
#define IDC_SET_CHIP_2						117								//压注按钮
#define IDC_SET_CHIP_3						118								//压注按钮


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_SHOW_CARD, OnShowCard)
	ON_BN_CLICKED(IDC_FRONT_CARD, OnFrontCard)
	ON_BN_CLICKED(IDC_MID_CARD, OnMidCard)
	ON_BN_CLICKED(IDC_BACK_CARD, OnBackCard)
	ON_BN_CLICKED(IDC_SET_CHIP_0, OnSetChip0)
	ON_BN_CLICKED(IDC_SET_CHIP_1, OnSetChip1)
	ON_BN_CLICKED(IDC_SET_CHIP_2, OnSetChip2)
	ON_BN_CLICKED(IDC_SET_CHIP_3, OnSetChip3)
	ON_BN_CLICKED(IDC_DESCEND_SORT , OnDescendSort)
	ON_BN_CLICKED(IDC_ASCEND_SORT , OnAscendSort)
	ON_BN_CLICKED(IDC_COLOR_SORT , OnColorSort)
	ON_BN_CLICKED(IDC_AUTO_OUT_CARD, OnAutoOutCard)
	ON_BN_CLICKED( IDC_CALL_BANKER, OnCallBanker ) 
	ON_BN_CLICKED( IDC_NO_CALL_BANKER, OnNoCallBanker ) 
	ON_BN_CLICKED(IDC_RESTORE_CARD, OnRestoreCard)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientView::CGameClientView(void) : CGameFrameView(true,24)
{
	//加载资源
	HINSTANCE hInstance=AfxGetInstanceHandle();	
	m_ImageBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageBanker.SetLoadInfo(IDB_BANKER , hInstance) ;
	m_ImageShowUserChip.SetLoadInfo(IDB_SHOW_USER_CHIP , hInstance) ;
	m_ImageCenter.SetLoadInfo(IDB_VIEW_CENTER, hInstance) ;
	m_ImageCallBankerInfo.SetLoadInfo( IDB_CALL_BANKER_INFO, hInstance ) ;
	m_ImageChipBack.SetLoadInfo( IDB_CHIP_BACK, hInstance ) ;

	//游戏变量
	m_wBanker=INVALID_CHAIR ;
	for(WORD i=0 ; i<GAME_PLAYER ; ++i) m_nChip[i] = -1 ;
	m_bShowCompare=false;	
	m_enSegmentType=enErr ;
	m_bShowBankerName=false ;
	m_wMaxChip=20 ;
	for ( WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++ ) m_bNoCallBanker[wChairID] = false ;

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
	m_ScoreView.Create(IDD_GAME_SCORE,this);

	//创建扑克
	for (WORD i=0 ; i<GAME_PLAYER ; i++)
	{
		m_UserCardControl[i].SetDirection(true);
		m_UserCardControl[i].SetDisplayFlag(true);
		m_UserCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,20+i);

		m_SegCardControl[i].SetDirection(true);
		m_SegCardControl[i].SetDisplayFlag(true);
		m_SegCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,30+i);

		if(1==i)
		{
			m_UserCardControl[i].SetDirection(false) ;
			m_UserCardControl[i].SetCardSide(enLeft) ;
			m_UserCardControl[i].SetCardSpace(20 , 10 , false) ;

			m_SegCardControl[i].SetDirection(false) ;
			m_SegCardControl[i].SetCardSide(enLeft) ;
			m_SegCardControl[i].SetCardSpace(20 , 10 , false) ;
		}
		else if(3==i)
		{
			m_UserCardControl[i].SetDirection(false) ;
			m_UserCardControl[i].SetCardSide(enRight) ;
			m_UserCardControl[i].SetCardSpace(20 , 10 , false) ;

			m_SegCardControl[i].SetDirection(false) ;
			m_SegCardControl[i].SetCardSide(enRight) ;
			m_SegCardControl[i].SetCardSpace(20 , 10 , false) ;
		}
		else 
		{
            m_UserCardControl[i].SetCardSide(enHorizon) ;

			m_SegCardControl[i].SetCardSide(enHorizon) ;
		}
		if(0==i)
		{
			m_UserCardControl[i].SetCardSpace(10 , 10 , false) ;
			m_UserCardControl[i].SetDisplayFlag(false);

			m_SegCardControl[i].SetCardSpace(10 , 10 , false) ;
			m_SegCardControl[i].SetDisplayFlag(false);
		}
	}
	
	m_UserCardControl[2].SetSinkWindow(AfxGetMainWnd()) ;
//	m_UserCardControl[2].SetPositively(true) ;

	for(WORD i=0 ; i<GAME_PLAYER ; ++i)
	{
		//所以分段
		for(BYTE j=0 ; j<3 ; ++j)
		{
			m_AllSegCardControl[i][j].SetDirection(true);
			m_AllSegCardControl[i][j].SetDisplayFlag(true);
			m_AllSegCardControl[i][j].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,40+i+j);
			m_AllSegCardControl[i][j].SetCardSide(enHorizon) ;
		}

		//对比扑克
		m_CompareCarControl[i].SetDirection(true);
		m_CompareCarControl[i].SetDisplayFlag(true);
		m_CompareCarControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,60+i);
		m_CompareCarControl[i].SetCardSide(enHorizon) ;

		//庄家扑克
		m_BankerCardControl[i].SetDirection(true);
		m_BankerCardControl[i].SetDisplayFlag(true);
		m_BankerCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,70+i);
		m_BankerCardControl[i].SetCardSide(enHorizon) ;		
	}

	//手上分段
	for(BYTE i=0 ; i<3 ; ++i)
	{
		m_HandSegCardControl[i].SetDirection(true);
		m_HandSegCardControl[i].SetDisplayFlag(true);
		m_HandSegCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,80+i);
		m_HandSegCardControl[i].SetCardSide(enHorizon) ;
		m_HandSegCardControl[i].SetSinkWindow(AfxGetMainWnd()) ;
		m_HandSegCardControl[i].SetPositively(true) ;
	}


	
	//创建按钮
	m_btStart.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_START);
	m_btShowCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_SHOW_CARD);
	m_btFrontCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_FRONT_CARD);
	m_btMidCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_MID_CARD);
	m_btBackCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_BACK_CARD);
	m_btSetChip[0].Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_SET_CHIP_0);
	m_btSetChip[1].Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_SET_CHIP_1);
	m_btSetChip[2].Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_SET_CHIP_2);
	m_btSetChip[3].Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_SET_CHIP_3);
	m_btDescend.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_DESCEND_SORT);
	m_btAscend.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_ASCEND_SORT);	
	m_btColor.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_COLOR_SORT);	
	m_btAutoOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_AUTO_OUT_CARD);
	m_btBaoPai.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_BAO_PAI);
	m_btRestoreCard.Create(TEXT(""), WS_CHILD,CreateRect,this,IDC_RESTORE_CARD);
	m_btCallBanker.Create( TEXT( "" ), WS_CHILD, CreateRect, this, IDC_CALL_BANKER );
	m_btNoCallBanker.Create( TEXT( "" ), WS_CHILD, CreateRect, this, IDC_NO_CALL_BANKER );
    
	//设置按钮
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_BT_START,hInstance,false);
	m_btShowCard.SetButtonImage(IDB_BT_SHOW_CARD,hInstance,false);
	m_btFrontCard.SetButtonImage(IDB_BT_FRONT_CARD,hInstance,false);
	m_btMidCard.SetButtonImage(IDB_BT_MID_CARD,hInstance,false);
	m_btBackCard.SetButtonImage(IDB_BT_BACK_CARD,hInstance,false);
	m_btSetChip[0].SetButtonImage(IDB_BT_SETCHIP,hInstance,false);
	m_btSetChip[1].SetButtonImage(IDB_BT_SETCHIP,hInstance,false);
	m_btSetChip[2].SetButtonImage(IDB_BT_SETCHIP,hInstance,false);
	m_btSetChip[3].SetButtonImage(IDB_BT_SETCHIP,hInstance,false);
	m_btDescend.SetButtonImage(IDB_BT_DESCEND_SORT_CARD,hInstance,false);
	m_btAscend.SetButtonImage(IDB_BT_ASCEND_SORT_CARD,hInstance,false);
	m_btColor.SetButtonImage(IDB_BT_COLOR_SORT_CARD,hInstance,false);
	m_btAutoOutCard.SetButtonImage(IDB_BT_HINT,hInstance,false);
	m_btBaoPai.SetButtonImage(IDB_BT_BAO_PAI, hInstance, false) ;
	m_btRestoreCard.SetButtonImage(IDB_BT_RESTORE_CARD, hInstance, false) ;
	m_btCallBanker.SetButtonImage( IDB_BT_CALL_BANKER, hInstance, false ) ;
	m_btNoCallBanker.SetButtonImage( IDB_BT_NO_CALL_BANKER, hInstance, false ) ;


	//////////////////////////////////////////////////////////////////////////
/*
	m_btStart.ShowWindow(SW_SHOW) ;
	m_btShowCard.ShowWindow(SW_SHOW) ;
	m_btFrontCard.ShowWindow(SW_SHOW) ;
	m_btMidCard.ShowWindow(SW_SHOW) ;
	m_btBackCard.ShowWindow(SW_SHOW) ;
	m_btSetChip.ShowWindow(SW_SHOW) ;
	m_btDescend.ShowWindow(SW_SHOW) ;
	m_btAscend.ShowWindow(SW_SHOW) ;
	m_btColor.ShowWindow(SW_SHOW) ;
	m_btAutoOutCard.ShowWindow(SW_SHOW) ;
	m_btCancelAutoOutCard.ShowWindow(SW_SHOW) ;*/



	//////////////////////////////////////////////////////////////////////////

	
#ifdef VIDEO_GAME
	//创建视频
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//创建视频
		m_DlgVideoService[i].Create(NULL,NULL,WS_CHILD|WS_VISIBLE,CreateRect,this,200+i);
		m_DlgVideoService[i].InitVideoService(i==2,i!=2);

		//设置视频
		g_VideoServiceManager.SetVideoServiceControl(i,&m_DlgVideoService[i]);
	}
#endif
	

	return 0;
}

//重置界面
void CGameClientView::ResetGameView()
{
	//游戏变量
	m_wBanker=INVALID_CHAIR ;
	for(WORD i=0 ; i<GAME_PLAYER ; ++i) m_nChip[i] = -1 ;
	m_bShowCompare=false;	
	m_enSegmentType=enErr ;
	m_bShowBankerName=false ;
	m_wMaxChip=20 ;
	for ( WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++ ) m_bNoCallBanker[wChairID] = false ;
    
	return;
}

//调整控件
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//积分视图
	CRect rcControl;
	m_ScoreView.ShowWindow(SW_HIDE);
	m_ScoreView.GetWindowRect(&rcControl);
//	m_ScoreView.SetWindowPos(NULL,(nWidth-rcControl.Width())/2,(nHeight-rcControl.Height()-100)/2+60,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//用户扑克
	m_UserCardControl[0].SetBenchmarkPos(nWidth / 2 - 30 , nHeight / 2 - 200 , enXLeft , enYCenter);
	m_UserCardControl[1].SetBenchmarkPos(nWidth / 2 - 270 , nHeight / 2 - 57, enXLeft , enYTop);
	m_UserCardControl[2].SetBenchmarkPos(nWidth / 2 - 60 , nHeight / 2 + 280 , enXLeft , enYBottom);
	m_UserCardControl[3].SetBenchmarkPos(nWidth / 2 + 275, nHeight / 2 - 57, enXRight, enYTop);

	m_SegCardControl[0].SetBenchmarkPos(nWidth / 2 - 105, nHeight / 2 - 200 , enXCenter , enYCenter);
	m_SegCardControl[1].SetBenchmarkPos(nWidth / 2 - 270 , nHeight / 2 - 120 , enXLeft , enYCenter);
	m_SegCardControl[2].SetBenchmarkPos(nWidth / 2 - 150 , nHeight / 2 + 280 , enXCenter , enYBottom);
	m_SegCardControl[3].SetBenchmarkPos(nWidth / 2 + 275, nHeight / 2 - 120 , enXRight , enYCenter);

	POINT ptCardControl[GAME_PLAYER] ;
	ptCardControl[0].x = nWidth / 2 + 10 ;
	ptCardControl[0].y = nHeight / 4 - 70 ;

	ptCardControl[1].x = nWidth / 2 - 230 ;
	ptCardControl[1].y = nHeight / 2 - 100 ;

	ptCardControl[2].x = nWidth / 2 + 10 ;
	ptCardControl[2].y = nHeight - 80 ;

	ptCardControl[3].x = nWidth / 2 + 310;
	ptCardControl[3].y = nHeight / 2 - 100 ;


//	BYTE bCardData[13] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D} ;

	for(WORD i=0 ; i<GAME_PLAYER ; ++i)
	{
		//所有分段
		for(BYTE j=2 ; j!=255 ; --j)
		{
			if ( i == 2)
				m_AllSegCardControl[i][j].SetBenchmarkPos(ptCardControl[i].x , ptCardControl[i].y+30*j - 100 , enXCenter , enYCenter);
			else if ( i == 0)
				m_AllSegCardControl[i][j].SetBenchmarkPos(ptCardControl[i].x , ptCardControl[i].y+30*j + 20, enXCenter , enYCenter);
			else if ( i == 1)
				m_AllSegCardControl[i][j].SetBenchmarkPos(ptCardControl[i].x, ptCardControl[i].y+30*j + 65 , enXCenter , enYCenter);
			else 
				m_AllSegCardControl[i][j].SetBenchmarkPos(ptCardControl[i].x - 70, ptCardControl[i].y+30*j + 65, enXCenter , enYCenter);
		}
	}

	//对比扑克
	m_CompareCarControl[0].SetBenchmarkPos(ptCardControl[0].x , ptCardControl[0].y+100 , enXCenter , enYCenter) ;
	m_CompareCarControl[1].SetBenchmarkPos(ptCardControl[1].x , ptCardControl[1].y+120 , enXCenter , enYCenter) ;
	m_CompareCarControl[2].SetBenchmarkPos(ptCardControl[2].x , ptCardControl[2].y - 37 , enXCenter , enYCenter) ;
	m_CompareCarControl[3].SetBenchmarkPos(ptCardControl[3].x-80 , ptCardControl[3].y+120 , enXCenter , enYCenter) ;

	//庄家扑克
	m_BankerCardControl[0].SetBenchmarkPos(ptCardControl[0].x , ptCardControl[0].y , enXCenter , enYCenter) ;
	m_BankerCardControl[1].SetBenchmarkPos(ptCardControl[1].x , ptCardControl[1].y+20 , enXCenter , enYCenter) ;
	m_BankerCardControl[2].SetBenchmarkPos(ptCardControl[2].x , ptCardControl[2].y - 140 , enXCenter , enYCenter) ;
	m_BankerCardControl[3].SetBenchmarkPos(ptCardControl[3].x-80, ptCardControl[3].y+20 , enXCenter , enYCenter) ;	

	//手上分段
	m_HandSegCardControl[0].SetBenchmarkPos(nWidth / 2 - 180 , nHeight / 2 + 280 , enXCenter , enYBottom);
	m_HandSegCardControl[1].SetBenchmarkPos(nWidth / 2 - 180 + 180 , nHeight / 2 + 280 , enXCenter , enYBottom);
	m_HandSegCardControl[2].SetBenchmarkPos(nWidth / 2 - 180 + 380 , nHeight / 2 + 280 , enXCenter , enYBottom);

//	BYTE cbCardData[10] = {0} ;
	//m_HandSegCardControl[0].SetCardData(bCardData , 3) ;
	//m_HandSegCardControl[1].SetCardData(bCardData , 5) ;
	//m_HandSegCardControl[2].SetCardData(bCardData , 5) ;

	//m_btShowCard.ShowWindow(SW_SHOW) ;

	//for(WORD i=0 ; i<GAME_PLAYER ; ++i)
	//{
		//m_CompareCarControl[i].SetCardData(cbCardData, 5) ;
		//m_BankerCardControl[i].SetCardData(cbCardData, 5) ;
		//m_SegCardControl[i].SetCardData(cbCardData, 5) ;
		//m_UserCardControl[i].SetCardData(cbCardData, 10) ;
	//}

	//for(WORD i=0 ; i<GAME_PLAYER ; ++i)
	//{
	/*	for(BYTE j=0 ; j<3 ; ++j)
            m_AllSegCardControl[i][j].SetCardData(bCardData , 5) ;*/
	/*	m_UserCardControl[i].SetCardData(cbCardData , 13) ;
		m_SegCardControl[i].SetCardData(cbCardData , 5) ;*/
//	}
//	m_ScoreView.ShowWindow(SW_SHOW);



	//玩家信息
	m_ptFace[3].x=nWidth-m_nXBorder-m_nXFace-5;
	m_ptFace[3].y=nHeight/2-m_nYFace-120;
	m_ptName[3].x=nWidth-m_nXBorder-m_nXFace-10;
	m_ptName[3].y=nHeight/2-m_nYFace+3-120;
	m_ptTimer[3].x=nWidth-m_nXBorder-m_nXFace-13;
	m_ptTimer[3].y=nHeight/2-m_nYFace-m_nYTimer-120;
	m_ptReady[3].x=nWidth-m_nXBorder-m_nXTimer-5;
	m_ptReady[3].y=nHeight/2-m_nYFace-m_nYTimer/2-120;

	m_ptFace[1].x=m_nXBorder+5;
	m_ptFace[1].y=nHeight/2-m_nYFace-120;
	m_ptName[1].x=m_nXBorder+m_nXFace+10;
	m_ptName[1].y=nHeight/2-m_nYFace+3-120;
	m_ptTimer[1].x=m_nXBorder+5;
	m_ptTimer[1].y=nHeight/2-m_nYFace-5-m_nYTimer-120;
	m_ptReady[1].x=m_nXBorder+5+m_nXTimer;
	m_ptReady[1].y=nHeight/2-m_nYFace-m_nYTimer/2-120;


	m_ptFace[0].x=nWidth/2-m_nXFace/2;
	m_ptFace[0].y=m_nYBorder+5;
	m_ptName[0].x=nWidth/2+5+m_nXFace/2;
	m_ptName[0].y=m_nYBorder+5;
	m_ptTimer[0].x=nWidth/2-m_nXFace/2-m_nXTimer*2-5;
	m_ptTimer[0].y=m_nYBorder+10;
	m_ptReady[0].x=nWidth/2-m_nXTimer-m_nXFace/2-5;
	m_ptReady[0].y=m_nYBorder+m_nYTimer/2+10;

	m_ptFace[2].x=nWidth/2-m_nXFace/2;
	m_ptFace[2].y=nHeight-m_nYBorder-m_nYFace-5;
	m_ptName[2].x=nWidth/2+5+m_nXFace/2-5;
	m_ptName[2].y=nHeight-m_nYBorder-m_nYFace-5;
	m_ptTimer[2].x=nWidth/2-m_nXFace/2-m_nXTimer*2-8;
	m_ptTimer[2].y=nHeight-m_nYBorder-m_nYTimer-5;
	m_ptReady[2].x=nWidth/2-m_nXTimer-m_nXFace/2-5;
	m_ptReady[2].y=nHeight-m_nYBorder-m_nYTimer/2-5;


	//叫庄信息
	CImageHandle NoCallBankerImageHandle( &m_ImageCallBankerInfo ) ;
	INT nImageWidth = m_ImageCallBankerInfo.GetWidth() / 8 ;
	INT nImageHeight = m_ImageCallBankerInfo.GetHeight() ;

	//中心距离
	INT nCenterDistance = 230 ;

	m_ptCallBankerInfo[0].x = ( nWidth - nImageWidth ) / 2 ; 
	m_ptCallBankerInfo[0].y = nHeight / 2 - nCenterDistance - nImageWidth + 20 ;

	m_ptCallBankerInfo[1].x = nWidth / 2 - nCenterDistance - nImageWidth ; 
	m_ptCallBankerInfo[1].y = ( nHeight - nImageHeight ) / 2 ;

	m_ptCallBankerInfo[2].x = ( nWidth - nImageWidth ) / 2 ; 
	m_ptCallBankerInfo[2].y = nHeight / 2 + nCenterDistance - 20 ;

	m_ptCallBankerInfo[3].x = nWidth / 2 + nCenterDistance ; 
	m_ptCallBankerInfo[3].y = ( nHeight - nImageHeight ) / 2 ;

	//移动按钮
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(11);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	m_btShowCard.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStart, NULL, (nWidth - rcButton.Width()) / 2, ( nHeight - rcButton.Height() ) / 2 + 120, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btSetChip[0], NULL, (nWidth - rcButton.Width() * 4 - 3 * 5 ) / 2, ( nHeight - rcButton.Height() ) / 2 + 120 - rcButton.Height() - 5, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btSetChip[1], NULL, (nWidth - rcButton.Width() * 4 - 3 * 5 ) / 2 + rcButton.Width() + 5 , ( nHeight - rcButton.Height() ) / 2 + 120 - rcButton.Height() - 5, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btSetChip[2], NULL, (nWidth - rcButton.Width() * 4 - 3 * 5 ) / 2 + rcButton.Width() * 2 + 10, ( nHeight - rcButton.Height() ) / 2 + 120 - rcButton.Height() - 5, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btSetChip[3], NULL, (nWidth - rcButton.Width() * 4 - 3 * 5 ) / 2 + rcButton.Width() * 3 + 15, ( nHeight - rcButton.Height() ) / 2 + 120 - rcButton.Height() - 5, 0, 0, uFlags);

	DeferWindowPos(hDwp,m_btCallBanker, NULL, (nWidth - rcButton.Width()) / 2, ( nHeight - rcButton.Height() ) / 2 + 140, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btNoCallBanker, NULL, (nWidth - rcButton.Width()) / 2, ( nHeight - rcButton.Height() ) / 2 + 140 - rcButton.Height() - 5, 0, 0, uFlags);

	DeferWindowPos(hDwp, m_btShowCard, NULL, (nWidth - rcButton.Width()) / 2 + 150, ( nHeight - rcButton.Height() ) / 2 + 120 + rcButton.Height() + 5, 0, 0, uFlags);
	DeferWindowPos(hDwp,m_btRestoreCard, NULL, (nWidth - rcButton.Width()) / 2 + 150 + rcButton.Width() + 10, ( nHeight - rcButton.Height() ) / 2 + 120 + rcButton.Height() + 5, 0, 0, uFlags);

	DeferWindowPos(hDwp, m_btAutoOutCard, NULL, (nWidth - rcButton.Width()) / 2 + 150, ( nHeight - rcButton.Height() ) / 2 + 120 + 10 , 0, 0, uFlags);

//	DeferWindowPos(hDwp,m_btBaoPai,NULL,nWidth-2*rcButton.Width()-10,nHeight-4*rcButton.Height()-5,0,0,uFlags);

	m_btColor.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp, m_btColor, NULL, (nWidth - rcButton.Width()) / 2 + 170, ( nHeight - rcButton.Height() ) / 2 + 300, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btDescend, NULL, (nWidth - rcButton.Width()) / 2 + 170 + rcButton.Width() + 5, ( nHeight - rcButton.Height() ) / 2 + 300, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btAscend, NULL, (nWidth - rcButton.Width()) / 2 + 170 + 2 * rcButton.Width() + 10, ( nHeight - rcButton.Height() ) / 2 + 300, 0, 0, uFlags);

	DeferWindowPos(hDwp, m_btFrontCard, NULL, (nWidth - rcButton.Width()) / 2 + 170, ( nHeight - rcButton.Height() ) / 2 + 300 + rcButton.Height() + 5, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btMidCard, NULL, (nWidth - rcButton.Width()) / 2 + 170 + rcButton.Width() + 5, ( nHeight - rcButton.Height() ) / 2 + 300 + rcButton.Height() + 5, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btBackCard, NULL, (nWidth - rcButton.Width()) / 2 + 170 + 2 * rcButton.Width() + 10, ( nHeight - rcButton.Height() ) / 2 + 300 + rcButton.Height() + 5, 0, 0, uFlags);
#ifdef VIDEO_GAME

	//视频窗口
	DeferWindowPos(hDwp,m_DlgVideoService[0],NULL,nWidth/2-162,nHeight/2-350,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_DlgVideoService[1],NULL,nWidth/2+287,nHeight/2-172,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_DlgVideoService[2],NULL,nWidth/2-290,nHeight/2+195,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_DlgVideoService[3],NULL,nWidth/2-375,nHeight/2-172,0,0,uFlags|SWP_NOSIZE);
#endif
	EndDeferWindowPos(hDwp);

/*	m_btStart.ShowWindow(SW_SHOW);			
	m_btShowCard.ShowWindow(SW_SHOW);		
	m_btFrontCard.ShowWindow(SW_SHOW);		
	m_btMidCard.ShowWindow(SW_SHOW);		
	m_btBackCard.ShowWindow(SW_SHOW);
	m_btSetChip.ShowWindow(SW_SHOW);
	m_btDescend.ShowWindow(SW_SHOW);
	m_btAscend.ShowWindow(SW_SHOW);
	m_btColor.ShowWindow(SW_SHOW);
	m_btAutoOutCard.ShowWindow(SW_SHOW);
	m_btRestoreCard.ShowWindow(SW_SHOW);*/



	//动画位置
	SetFlowerControlInfo(3,nWidth-m_nXBorder-5-BIG_FACE_WIDTH,nHeight/2-3);
	SetFlowerControlInfo(1,m_nXBorder+5,nHeight/2-3);

	SetFlowerControlInfo(0,nWidth/2,m_nYBorder+m_nYFace+BIG_FACE_HEIGHT);
	SetFlowerControlInfo(2,nWidth/2,nHeight-m_nYBorder-m_nYFace-BIG_FACE_HEIGHT);

	return;
}

//绘画界面
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//绘画背景
	DrawViewImage(pDC,m_ImageBack,enMode_Spread);
	DrawViewImage(pDC,m_ImageCenter,enMode_Centent);

	CImageHandle ChipBackImageHandle( &m_ImageShowUserChip ) ;

	//for ( BYTE cbChipIdx = 0; cbChipIdx < 2; ++cbChipIdx )
	//	m_ImageShowUserChip.AlphaDrawImage( pDC, 185, 187-cbChipIdx * 6, m_ImageShowUserChip.GetWidth() / 6, m_ImageShowUserChip.GetHeight(), 
	//	m_ImageShowUserChip.GetWidth() / 6 * 1, 0, RGB(255,0,255)) ;

	//for ( BYTE cbChipIdx = 0; cbChipIdx < 3; ++cbChipIdx )
	//	m_ImageShowUserChip.AlphaDrawImage( pDC, 210, 200-cbChipIdx * 6, m_ImageShowUserChip.GetWidth() / 6, m_ImageShowUserChip.GetHeight(), 
	//	2 * m_ImageShowUserChip.GetWidth() / 6 * 1, 0, RGB(255,0,255)) ;

	//for ( BYTE cbChipIdx = 0; cbChipIdx < 2; ++cbChipIdx )
	//	m_ImageShowUserChip.AlphaDrawImage( pDC, 190, 217-cbChipIdx * 6, m_ImageShowUserChip.GetWidth() / 6, m_ImageShowUserChip.GetHeight(), 
	//	3 * m_ImageShowUserChip.GetWidth() / 6 * 1, 0, RGB(255,0,255)) ;


	//显示比较
	if(true==m_bShowCompare)
	{
		CFont font;
		VERIFY(font.CreateFont(
			30,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			"Arial"));                 // lpszFacename

		CFont* def_font = pDC->SelectObject(&font);

		DrawTextString(pDC,m_strShowCompare,RGB(250 , 0 , 0),RGB(50,50,50),nWidth/2+55,nHeight/2-30);

		pDC->SelectObject(def_font);
		font.DeleteObject();
	}

	//绘画庄家
	if ( INVALID_CHAIR != m_wBanker )
	{
		CImageHandle ImageHandle(&m_ImageBanker) ;

		int nPosX , nPosY ;
		if(0==m_wBanker)
		{
			nPosX = nWidth/2-m_nXBorder-m_nXFace-5-m_ImageBanker.GetWidth()-15;
			nPosY = m_nYBorder+5;
		}
		else if(1==m_wBanker)
		{	
			nPosX = m_nXBorder+5;
			nPosY = nHeight/2-m_nYFace-60+75+m_ImageBanker.GetHeight();
		}
		else if ( 2 == m_wBanker )
		{
			nPosX = nWidth/2-m_nXBorder-m_nXFace-5-m_ImageBanker.GetWidth()-15;
			nPosY = nHeight-m_nYBorder-m_ImageBanker.GetHeight()-5 ;
		}
		else if(3==m_wBanker)
		{
			nPosX = nWidth-m_nXBorder-m_ImageBanker.GetWidth()-5;
			nPosY = nHeight/2-m_nYFace-60+75+m_ImageBanker.GetHeight();
		}
		m_ImageBanker.AlphaDrawImage(pDC , nPosX , nPosY , RGB(255,0,255)) ;
	}

	//显示庄家 
	if(true==m_bShowBankerName)
	{
		POINT point[GAME_PLAYER] ;
		point[0].x = nWidth/2-140;
		point[0].y = nHeight/4-100;
		point[1].x = m_nXBorder ;
		point[1].y = nHeight/2-100 ;
		point[2].x = nWidth/2-140;
		point[2].y = nHeight-260 ;
		point[3].x = nWidth-m_nXBorder-80 ; 
		point[3].y = nHeight/2-100 ;

		CFont font;
		VERIFY(font.CreateFont(
			20,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			"Arial"));                 // lpszFacename

		CFont* def_font = pDC->SelectObject(&font);

		pDC->SetTextAlign(TA_TOP|TA_LEFT);
		CString str = CString("庄家扑克");
		for(WORD i=0 ; i<GAME_PLAYER ; ++i)
		{
			const tagUserData * pUserData=GetUserInfo(i);
			if(i!=m_wBanker && NULL!=pUserData) 
				DrawTextString(pDC,str,RGB(250 , 0 , 0),RGB(50,50,50),point[i].x,point[i].y);
		}

		pDC->SelectObject(def_font);
		font.DeleteObject();
	}

	//显示压注
	bool bShowChip = false ;
	for(WORD i=0 ; i<GAME_PLAYER ; ++i) if(-1!=m_nChip[i]) bShowChip=true ;
	if(true==bShowChip && !m_bShowCompare) 
	{
		CImageHandle ImageHandle(&m_ImageShowUserChip) ;
		CRect		 drawRect ;
		CString		 str ;
		BYTE		 bChipHeight = m_ImageShowUserChip.GetHeight() ,
					 bChipWidth = m_ImageShowUserChip.GetWidth()/6 ;
		int			 nPosX , nPosY ;

		for(WORD i=0 ; i<GAME_PLAYER ; ++i)
			if(-1!=m_nChip[i])
			{
				if(0==i)
				{
					nPosX = nWidth/2-5 ;
					nPosY = nHeight/4+90 ; 
				}
				else if(1==i)
				{
					nPosX = nWidth/2 - 145 ;
					nPosY = nHeight/3+115 ; 
				}
				else if(2==i)
				{
					nPosX = nWidth/2-25 ;
					nPosY = nHeight/2+25 ; 
				}
				else if(3==i)
				{
					nPosX = nWidth/2 + 80 ;
					nPosY = nHeight/3+115 ; 
				}

				//绘画底图
				for ( BYTE cbChipIdx = 0; cbChipIdx < 2; ++cbChipIdx )
					m_ImageShowUserChip.AlphaDrawImage(pDC , nPosX - 5 , nPosY - cbChipIdx * 6 , bChipWidth , bChipHeight , 0, 0 , RGB(255,0,255)) ;

				for ( BYTE cbChipIdx = 0; cbChipIdx < 3; ++cbChipIdx )
					m_ImageShowUserChip.AlphaDrawImage(pDC , nPosX + 28 , nPosY + 13 - cbChipIdx * 6 , bChipWidth , bChipHeight , bChipWidth , 0 , RGB(255,0,255)) ;

				for ( BYTE cbChipIdx = 0; cbChipIdx < 2; ++cbChipIdx )
					m_ImageShowUserChip.AlphaDrawImage(pDC , nPosX + 25 - 20 , nPosY + 13 + 17 - cbChipIdx * 6, bChipWidth , bChipHeight , 2 * bChipWidth , 0 , RGB(255,0,255)) ;

				CImageHandle ChipBackImageHandle( &m_ImageChipBack ) ;
				int nChipBackXPos = nPosX - 5;
				int nChipBackYPos = nPosY + 13 + 17 - bChipHeight + 53 ;

				//m_ImageChipBack.AlphaDrawImage( pDC, nChipBackXPos, nChipBackYPos, m_ImageChipBack.GetWidth() / 3, m_ImageChipBack.GetHeight(), 
				//	m_ImageChipBack.GetWidth() / 3 * 1, 0, RGB(255,0,255)) ;

				CFont font;
				VERIFY(font.CreateFont(
					18,                        // nHeight
					0,                         // nWidth
					0,                         // nEscapement
					0,                         // nOrientation
					FW_NORMAL,                 // nWeight
					FALSE,                     // bItalic
					FALSE,                     // bUnderline
					0,                         // cStrikeOut
					ANSI_CHARSET,              // nCharSet
					OUT_DEFAULT_PRECIS,        // nOutPrecision
					CLIP_DEFAULT_PRECIS,       // nClipPrecision
					DEFAULT_QUALITY,           // nQuality
					DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
					"宋体"));                 // lpszFacename

				CFont* def_font = pDC->SelectObject(&font);

				//显示下注
				str.Format("%d" , m_nChip[i]) ;

				CRect rcChip(nChipBackXPos, nChipBackYPos, nChipBackXPos + m_ImageChipBack.GetWidth() / 3, nChipBackYPos + m_ImageChipBack.GetHeight() )  ;

				pDC->SetTextAlign(TA_LEFT | VTA_TOP );
				pDC->SetTextColor( RGB( 0, 0, 0 ) ) ;

			//	DrawTextString(pDC,str,RGB(0, 0, 0),RGB(56, 210, 255),nChipBackXPos, nChipBackYPos);
				pDC->DrawText( str, str.GetLength(), rcChip, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

				pDC->SelectObject(def_font);
				font.DeleteObject();


			}
	}

	//绘画用户
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//变量定义
		WORD wUserTimer=GetUserTimer(i);
		const tagUserData * pUserData=GetUserInfo(i);
		//////////////////////////////////////////////////////////////////////////

			//	测试代码
				//tagUserData UserData ;
				//memset(&UserData , 0 , sizeof(UserData)) ;
				//UserData.wChairID = i ;
				//UserData.wFaceID  = i ;
				//UserData.cbUserStatus=US_READY ;
				//CopyMemory(UserData.szName , "abcdefg" , lstrlen("abcdefg")) ;
				//pUserData = &UserData ;
				//wUserTimer = i+1 ;

		//////////////////////////////////////////////////////////////////////////
		//绘画用户
		if (pUserData!=NULL)
		{

			if(3==i) pDC->SetTextAlign(TA_RIGHT);
			else if(1==i) pDC->SetTextAlign(TA_LEFT);
			else pDC->SetTextAlign(TA_TOP|TA_LEFT);
			DrawTextString(pDC,pUserData->szName,RGB(240,240,240),RGB(50,50,50),m_ptName[i].x,m_ptName[i].y);

			//举手
			if (pUserData->cbUserStatus==US_READY)
			{
				DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			}
			//其他信息

			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer , 99);

			DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);

			//显示金币
		//	if ( i == 2 )
			{
				LONG lMeScore = pUserData->lScore ;
				CString strScore ;

				if ( m_nChip[2] != -1 )
					strScore.Format( TEXT( "￥%ld" ), lMeScore - m_nChip[i] ) ;
				else 
					strScore.Format( TEXT( "￥%ld" ), lMeScore ) ;

				pDC->TextOut( m_ptName[i].x, m_ptName[i].y + 18, strScore ) ;
			}
		}
	}



	//叫庄信息
	CImageHandle NoCallBankerImageHandle( &m_ImageCallBankerInfo ) ;
	INT nImageWidth = m_ImageCallBankerInfo.GetWidth() / 8 ;
	INT nImageHeight = m_ImageCallBankerInfo.GetHeight() ;

	for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
		if ( m_bNoCallBanker[wChairID] )
			m_ImageCallBankerInfo.AlphaDrawImage( pDC, m_ptCallBankerInfo[wChairID].x, m_ptCallBankerInfo[wChairID].y, 
			nImageWidth, nImageHeight, nImageWidth * 6, 0,  RGB( 255, 0, 255 ) ) ;

	return;
}


//艺术字体
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
	//变量定义
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//绘画边框
	pDC->SetTextColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//绘画字体
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);

	return;
}

//摊牌函数
void CGameClientView::OnShowCard()
{
	AfxGetMainWnd()->SendMessage(IDM_SHOWCARD,0,0);
	return ;
}

//设置后墩
void CGameClientView::OnBackCard()
{
	AfxGetMainWnd()->SendMessage(IDM_BACKCARD,0,0);
	return ;
}
//设置中墩
void CGameClientView::OnMidCard()
{
	AfxGetMainWnd()->SendMessage(IDM_MIDCARD,0,0);
	return ;
}
//设置前墩
void CGameClientView::OnFrontCard()
{
	AfxGetMainWnd()->SendMessage(IDM_FRONTCARD,0,0);
	return ;
}

//开始按钮
void CGameClientView::OnStart()
{
//	AfxMessageBox("1") ;

	AfxGetMainWnd()->SendMessage(IDM_START,0,0);

	//更新界面
	UpdateGameView(NULL);

	return ;
}

//设置扑克
void CGameClientView::SetCardData(const WORD wChairID , const BYTE bCardData[] , const BYTE bCardCount , const enCardType bCardType , const BYTE bSeg) 
{
	switch(bCardType) 
	{
	case enHandCard:		//手上扑克
		if(INVALID_CHAIR==wChairID)
		{
			for(WORD i=0 ; i<GAME_PLAYER ; ++i)	m_UserCardControl[i].SetCardData(NULL , 0) ;
			return ;
		}
		
		m_UserCardControl[wChairID].SetCardData(bCardData , bCardCount) ;

		break;

	case enSegmentCard:		//分段扑克
		if(INVALID_CHAIR==wChairID)
		{
			for(WORD i=0 ; i<GAME_PLAYER ; ++i)	m_SegCardControl[i].SetCardData(NULL , 0) ;
			return ;
		}

		m_SegCardControl[wChairID].SetCardData(bCardData , bCardCount) ;

		break;

    case enAllSegmentCard:	//所有分段
		if(INVALID_CHAIR==wChairID)
		{
			for(WORD i=0 ; i<GAME_PLAYER ; ++i)
				for(BYTE j=0 ; j<3 ; ++j)
                    m_AllSegCardControl[i][j].SetCardData(NULL , 0) ;
			return ;
		}

		m_AllSegCardControl[wChairID][bSeg].SetDisplayFlag(true) ;
		m_AllSegCardControl[wChairID][bSeg].SetCardData(bCardData , bCardCount) ;

		break;
	case enCompareCard:		//比较扑克
		{
			if(INVALID_CHAIR==wChairID) 
			{
				for(WORD i=0 ; i<GAME_PLAYER ; ++i)
					m_CompareCarControl[i].SetCardData(NULL , 0 ) ;
				return ;
			}
			m_CompareCarControl[wChairID].SetCardData(bCardData , bCardCount) ;
			return ;
		}
	case enBankCard:
		{
			if(INVALID_CHAIR==wChairID)
			{
				for(WORD i=0 ; i<GAME_PLAYER ; ++i)
					m_BankerCardControl[i].SetCardData(NULL , 0) ;
				return ;
			}
			m_BankerCardControl[wChairID].SetCardData(bCardData , bCardCount) ;
			return ;
		}

	case enErrorType:		//错误类型
		AfxMessageBox("错误扑克类型！") ;
		break;
	}
}

//设置庄家
void CGameClientView::SetBanker(const WORD wBanker)
{
	m_wBanker = wBanker ;
	//更新界面
	UpdateGameView(NULL);
}

//设置压注
void CGameClientView::SetChip(const WORD wChiarID , const int bChip) 
{
	if(INVALID_CHAIR==wChiarID)
	{
		for(WORD i=0 ; i<GAME_PLAYER ; ++i) m_nChip[i] = -1 ;
		//更新界面
		UpdateGameView(NULL);
		return ;
	}

	m_nChip[wChiarID] = bChip ;
	
	//更新界面
	UpdateGameView(NULL);
}

//显示比较
void CGameClientView::SetShowCompare(bool bShow , const enSegmentType SegmentType)
{
    if(false==bShow)
	{
		m_strShowCompare="" ;
		m_bShowCompare=false ;
		//更新界面
		UpdateGameView(NULL);
		return ;
	}

	m_bShowCompare = true ;
	switch(SegmentType)
	{
	case enFront:
		m_strShowCompare="前墩比较" ;
		break;

	case enMid:
		m_strShowCompare="中墩比较" ;
		break;

	case enBack:
		m_strShowCompare="后墩比较" ;
		break;

	case enAllSeg:
		m_strShowCompare="三墩比较" ;
		break;

	case enErr:
		{
			AfxMessageBox("错误比较类型!") ;
			break;
		}
	default:
		{
			AfxMessageBox("未知比较类型!") ;
			m_bShowCompare=false ;
			break; 
		}
	}
	//更新界面
	UpdateGameView(NULL);
}

//降序排列
void CGameClientView::OnDescendSort()
{
	AfxGetMainWnd()->SendMessage(IDM_DESCEND_SORT,0,0);
}
//升序排列
void CGameClientView::OnAscendSort() 
{
	AfxGetMainWnd()->SendMessage(IDM_ASCEND_SORT,0,0);
}
//花色排列
void CGameClientView::OnColorSort() 
{
	AfxGetMainWnd()->SendMessage(IDM_COLOR_SORT,0,0);
}

//自动代打
void CGameClientView::OnAutoOutCard() 
{
	AfxGetMainWnd()->SendMessage(IDM_AUTO_OUT_CARD,1,1);
	return;
}

//显示庄家
void CGameClientView::ShowBankerName(bool bShow) 
{
	if(m_bShowBankerName==bShow) return ;
    m_bShowBankerName=bShow ;

	//更新界面
	UpdateGameView(NULL);
}

//最高下注
void CGameClientView::SetMaxChip(LONG wMaxChip)
{
	m_wMaxChip = wMaxChip ;
}

//////////////////////////////////////////////////////////////////////////

//恢复扑克
void CGameClientView::OnRestoreCard() 
{
	AfxGetMainWnd()->SendMessage(IDM_RESTORE_CARD, 0, 0) ;
}

//叫庄消息
void CGameClientView::OnCallBanker()
{
	AfxGetMainWnd()->SendMessage( IDM_CALL_BANKER, 0, 0 ) ;
}

//不叫消息
void CGameClientView::OnNoCallBanker()
{
	AfxGetMainWnd()->SendMessage( IDM_NO_CALL_BANKER, 0, 0 ) ;
}

//叫庄信息
void CGameClientView::SetCallBankerInfo( WORD wChairID, bool bNoCallBanker )
{
	if ( wChairID == INVALID_CHAIR )
	{	
		for ( WORD wChairIndx = 0; wChairIndx < GAME_PLAYER; ++wChairIndx )
			m_bNoCallBanker[wChairIndx] = false ;
	}
	else
		m_bNoCallBanker[wChairID] = bNoCallBanker ;

	//更新界面
	UpdateGameView( NULL ) ;
}

//设置下注
void CGameClientView::SetChips( LONG lChipsArray[SET_CHIP_COUNT] )
{
	CopyMemory( m_lChipsArray, lChipsArray, sizeof( m_lChipsArray ) ) ;
}

//设置压注
void CGameClientView::OnSetChip0() 
{

	AfxGetMainWnd()->SendMessage(IDM_SETCHIP , (WPARAM)m_lChipsArray[0] , 0) ;
	return ;
}

//设置压注
void CGameClientView::OnSetChip1() 
{

	AfxGetMainWnd()->SendMessage(IDM_SETCHIP , (WPARAM)m_lChipsArray[1] , 0) ;
	return ;
}
//设置压注
void CGameClientView::OnSetChip2() 
{

	AfxGetMainWnd()->SendMessage(IDM_SETCHIP , (WPARAM)m_lChipsArray[2] , 0) ;
	return ;
}
//设置压注
void CGameClientView::OnSetChip3() 
{

	AfxGetMainWnd()->SendMessage(IDM_SETCHIP , (WPARAM)m_lChipsArray[3] , 0) ;
	return ;
}
