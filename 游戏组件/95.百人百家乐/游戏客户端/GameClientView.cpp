#include "StdAfx.h"
#include "Math.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_FLASH_WINNER			100									//闪动标识
#define IDI_SHOW_CHANGE_BANKER		101									//轮换庄家
#define IDI_DISPATCH_CARD			102									//发牌标识
#define IDI_BOMB_EFFECT				200									//爆炸标识


//按钮标识
#define IDC_JETTON_BUTTON_100		200									//按钮标识
#define IDC_JETTON_BUTTON_1000		201									//按钮标识
#define IDC_JETTON_BUTTON_10000		202									//按钮标识
#define IDC_JETTON_BUTTON_100000	203									//按钮标识
#define IDC_JETTON_BUTTON_1000000	204									//按钮标识
#define IDC_JETTON_BUTTON_5000000	205									//按钮标识
#define IDC_APPY_BANKER				206									//按钮标识
#define IDC_CANCEL_BANKER			207									//按钮标识
#define IDC_SCORE_MOVE_L			209									//按钮标识
#define IDC_SCORE_MOVE_R			210									//按钮标识
#define IDC_VIEW_CHART				211									//按钮标识
#define IDC_BANK_STORAGE			212									//按钮标识
#define IDC_BANK_DRAW				213									//按钮标识

#define IDC_UP						223									//按钮标识
#define IDC_DOWN					224									//按钮标识


//爆炸数目
#define BOMB_EFFECT_COUNT			8									//爆炸数目

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_BT_ADMIN,OpenAdminWnd)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_100, OnJettonButton100)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_1000, OnJettonButton1000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_10000, OnJettonButton10000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_100000, OnJettonButton100000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_1000000, OnJettonButton1000000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_5000000, OnJettonButton5000000)

	ON_BN_CLICKED(IDC_VIEW_CHART, OnViewChart)
	ON_BN_CLICKED(IDC_APPY_BANKER, OnApplyBanker)
	ON_BN_CLICKED(IDC_CANCEL_BANKER, OnCancelBanker)
	ON_BN_CLICKED(IDC_SCORE_MOVE_L, OnScoreMoveL)
	ON_BN_CLICKED(IDC_SCORE_MOVE_R, OnScoreMoveR)

	ON_BN_CLICKED(IDC_BANK_STORAGE, OnBankStorage)
	ON_BN_CLICKED(IDC_BANK_DRAW, OnBankDraw)

	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_DOWN, OnDown)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientView::CGameClientView() : CGameFrameView(true,24)
{
	//下注信息
	m_lMeTieScore=0L;
	m_lMeBankerScore=0L;
	m_lMePlayerScore=0L;
	m_lMeTieSamePointScore=0L;
	m_lMePlayerKingScore=0L;
	m_lMeBankerKingScore=0L;
	m_lMePlayerTwoPair=0L;
	m_lMeBankerTwoPair=0L;

	//发牌变量
	ZeroMemory(m_cbSendCount,sizeof(m_cbSendCount));
	
	//全体下注
	m_lAllTieScore=0L;
	m_lAllBankerScore=0L;
	m_lAllPlayerScore=0L;
	m_lAllTieSamePointScore=0L;
	m_lAllBankerKingScore=0L;
	m_lAllPlayerKingScore=0L;
	m_lAllPlayerTwoPair=0L;
	m_lAllBankerTwoPair=0L;

	//庄家信息
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_lBankerScore=0L;	
	m_lBankerWinScore=0L;
	m_lTmpBankerWinScore=0;

	//当局成绩
	m_lMeCurGameScore=0L;	
	m_lMeCurGameReturnScore=0L;
	m_lBankerCurGameScore=0L;
	m_lGameRevenue=0L;

	//动画变量
	ZeroMemory(m_bBombEffect,sizeof(m_bBombEffect));
	ZeroMemory(m_cbBombFrameIndex,sizeof(m_cbBombFrameIndex));

	//状态信息
	m_lCurrentJetton=0L;
	m_cbWinnerSide=0xFF;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=false;
	m_bNeedSetGameRecord=false;
	m_blCanStore=false;

	m_lMeCurGameScore=0L;			
	m_lMeCurGameReturnScore=0L;	
	m_lBankerCurGameScore=0L;		

	m_lAreaLimitScore=0L;	

	//位置信息
	m_nScoreHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;	

	//历史成绩
	m_lMeStatisticScore=0;

	//控件变量
	m_pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);

	//加载位图
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageViewFill.SetLoadInfo(IDB_VIEW_FILL,hInstance);
	m_ImageViewBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageWinFlags.SetLoadInfo(IDB_WIN_FLAGS,hInstance);
	m_ImageJettonView.SetLoadInfo(IDB_JETTOM_VIEW,hInstance);
	m_ImageScoreNumber.SetLoadInfo(IDB_SCORE_NUMBER,hInstance);
	m_ImageMeScoreNumber.SetLoadInfo(IDB_ME_SCORE_NUMBER,hInstance);

	m_ImageFrameXianJia.SetLoadInfo( IDB_FRAME_XIAN_JIA, hInstance );
	m_ImageFrameZhuangJia.SetLoadInfo( IDB_FRAME_ZHUANG_JIA, hInstance );
	m_ImageFrameXianTianWang.SetLoadInfo( IDB_FRAME_XIAN_TIAN_WANG, hInstance );
	m_ImageFrameZhuangTianWang.SetLoadInfo( IDB_FRAME_ZHUANG_TIAN_WANG, hInstance );
	m_ImageFramePingJia.SetLoadInfo( IDB_FRAME_PING_JIA, hInstance );
	m_ImageFrameTongDianPing.SetLoadInfo( IDB_FRAME_TONG_DIAN_PING, hInstance );
	m_ImageFramePlayerTwoPair.SetLoadInfo( IDB_PLAYER_TWO_PAIR, hInstance );
	m_ImageFrameBankerTwoPair.SetLoadInfo( IDB_BANKER_TWO_PAIR, hInstance );


	m_ImageGameEnd.SetLoadInfo( IDB_GAME_END, hInstance );
	m_ImageGameEndFrame.SetLoadInfo( IDB_GAME_END_FRAME, hInstance );
	m_ImageGamePoint.SetLoadInfo( IDB_GAME_POINT, hInstance );

	m_ImageMeBanker.SetLoadInfo( IDB_ME_BANKER, hInstance );
	m_ImageChangeBanker.SetLoadInfo( IDB_CHANGE_BANKER, hInstance );
	m_ImageNoBanker.SetLoadInfo( IDB_NO_BANKER, hInstance );	

	m_ImageTimeFlag.SetLoadInfo(IDB_TIME_FLAG, hInstance);

	m_ImageBombEffect.LoadImage(hInstance,TEXT("FIRE_EFFECT"));

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
	CRect rcCreate(0,0,0,0);
	m_GameRecord.Create(IDD_DLG_GAME_RECORD,this);
	m_ApplyUser.Create( IDD_DLG_GAME_RECORD	, this );
	m_GameRecord.ShowWindow(SW_HIDE);
	
	//下注按钮
	m_btJetton100.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100);
	m_btJetton1000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1000);
	m_btJetton10000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_10000);
	m_btJetton100000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100000);
	m_btJetton1000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1000000);
	m_btJetton5000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_5000000);
	m_btViewChart.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_VIEW_CHART);
		
	//申请按钮
	m_btApplyBanker.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_APPY_BANKER);
	m_btCancelBanker.Create(NULL,WS_CHILD|WS_DISABLED,rcCreate,this,IDC_CANCEL_BANKER);

	m_btScoreMoveL.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_L);
	m_btScoreMoveR.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_R);

	m_btOpenAdmin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(4,4,11,11),this,IDC_BT_ADMIN);
	m_btOpenAdmin.ShowWindow(SW_HIDE);

	m_btUp.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_UP);
	m_btDown.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_DOWN);
	m_btUp.ShowWindow(SW_SHOW);
	m_btDown.ShowWindow(SW_SHOW);

	//设置按钮
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btJetton100.SetButtonImage(IDB_BT_JETTON_100,hResInstance,false);
	m_btJetton1000.SetButtonImage(IDB_BT_JETTON_1000,hResInstance,false);
	m_btJetton10000.SetButtonImage(IDB_BT_JETTON_10000,hResInstance,false);
	m_btJetton100000.SetButtonImage(IDB_BT_JETTON_100000,hResInstance,false);
	m_btJetton1000000.SetButtonImage(IDB_BT_JETTON_1000000,hResInstance,false);
	m_btJetton5000000.SetButtonImage(IDB_BT_JETTON_5000000,hResInstance,false);

	m_btViewChart.SetButtonImage(IDB_BT_CHART,hResInstance,false);	

	m_btApplyBanker.SetButtonImage(IDB_BT_APPLY_BANKER,hResInstance,false);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,hResInstance,false);

	m_btScoreMoveL.SetButtonImage(IDB_BT_SCORE_MOVE_L,hResInstance,false);
	m_btScoreMoveR.SetButtonImage(IDB_BT_SCORE_MOVE_R,hResInstance,false);

	m_btUp.SetButtonImage(IDB_BT_BT_S,hResInstance,false);
	m_btDown.SetButtonImage(IDB_BT_BT_X,hResInstance,false);

	//扑克控件
	m_CardControl[INDEX_PLAYER].SetDisplayFlag(true);
	m_CardControl[INDEX_BANKER].SetDisplayFlag(true);

	//路单控件
	if (m_DlgViewChart.m_hWnd==NULL) m_DlgViewChart.Create(IDD_VIEW_CHART,this);

	//银行
#ifdef __SPECIAL___
	if(m_DlgBank.m_hWnd==NULL) m_DlgBank.Create(IDD_BANK_STORAGE,this);

	m_btBankerStorage.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BANK_STORAGE);
	m_btBankerDraw.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BANK_DRAW);

	m_btBankerStorage.SetButtonImage(IDB_BT_STORAGE,hResInstance,false);
	m_btBankerDraw.SetButtonImage(IDB_BT_DRAW,hResInstance,false);
#endif

	return 0;
}

//重置界面
void CGameClientView::ResetGameView()
{
	//下注信息
	m_lMeTieScore=0L;
	m_lMeBankerScore=0L;
	m_lMePlayerScore=0L;
	m_lMeTieSamePointScore=0L;
	m_lMePlayerKingScore=0L;
	m_lMeBankerKingScore=0L;
	m_lMePlayerTwoPair=0L;
	m_lMeBankerTwoPair=0L;

	//发牌变量
	ZeroMemory(m_cbSendCount,sizeof(m_cbSendCount));
	
	//全体下注
	m_lAllTieScore=0L;
	m_lAllBankerScore=0L;
	m_lAllPlayerScore=0L;
	m_lAllTieSamePointScore=0L;
	m_lAllBankerKingScore=0L;
	m_lAllPlayerKingScore=0L;
	m_lAllPlayerTwoPair=0L;
	m_lAllBankerTwoPair=0L;

	//庄家信息
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_lBankerScore=0L;	
	m_lBankerWinScore=0L;
	m_lTmpBankerWinScore=0;

	//当局成绩
	m_lMeCurGameScore=0L;	
	m_lMeCurGameReturnScore=0L;
	m_lBankerCurGameScore=0L;
	m_lGameRevenue=0L;

	//动画变量
	ZeroMemory(m_bBombEffect,sizeof(m_bBombEffect));
	ZeroMemory(m_cbBombFrameIndex,sizeof(m_cbBombFrameIndex));

	//状态信息
	m_lCurrentJetton=0L;
	m_cbWinnerSide=0xFF;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=false;
	m_bNeedSetGameRecord=false;

	m_lMeCurGameScore=0L;			
	m_lMeCurGameReturnScore=0L;	
	m_lBankerCurGameScore=0L;		

	m_lAreaLimitScore=0L;	

	//位置信息
	m_nScoreHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;	

	//历史成绩
	m_lMeStatisticScore=0;

	//发牌变量
	ZeroMemory(m_cbSendCount,sizeof(m_cbSendCount));

	//清空列表
	m_ApplyUser.ClearAll();

	//清除桌面
	CleanUserJetton();

	//动画变量
	ZeroMemory(m_bBombEffect,sizeof(m_bBombEffect));
	ZeroMemory(m_cbBombFrameIndex,sizeof(m_cbBombFrameIndex));

	//设置按钮
	m_btApplyBanker.ShowWindow(SW_SHOW);
	m_btApplyBanker.EnableWindow(FALSE);
	m_btCancelBanker.ShowWindow(SW_HIDE);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,AfxGetInstanceHandle(),false);

	return;
}

//调整控件
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	m_rcTrigger.SetRect(0,0,60,40);
	//位置信息
	m_nWinFlagsExcursionX = nWidth/2-148;	
	m_nWinFlagsExcursionY = nHeight/2 + 251;
	m_nWinPointsExcursionX = nWidth/2-343;
	m_nWinPointsExcursionY = nHeight/2 - 267;	

	//区域位置
	int nCenterX=nWidth/2, nCenterY=nHeight/2;
	m_rcPlayer.left=nCenterX - 314;
	m_rcPlayer.top=nCenterY-105;
	m_rcPlayer.right=m_rcPlayer.left+186;
	m_rcPlayer.bottom=m_rcPlayer.top+153;
	
	m_rcPlayerKing.left=m_rcPlayer.left-1;
	m_rcPlayerKing.top=nCenterY+45;
	m_rcPlayerKing.right=m_rcPlayerKing.left+188;
	m_rcPlayerKing.bottom=m_rcPlayerKing.top+87;

	m_rcPlayerTwoPair.left=m_rcPlayer.left;
	m_rcPlayerTwoPair.top=nCenterY-160;
	m_rcPlayerTwoPair.right=m_rcPlayerTwoPair.left+186;
	m_rcPlayerTwoPair.bottom=m_rcPlayerTwoPair.top+58;

	m_rcTie.left=nCenterX-130;
	m_rcTie.top=nCenterY-61;
	m_rcTie.right=m_rcTie.left+266;
	m_rcTie.bottom=m_rcTie.top+109;

	m_rcTieKing.left=nCenterX-130;
	m_rcTieKing.top=nCenterY+45;
	m_rcTieKing.right=m_rcTieKing.left+266;
	m_rcTieKing.bottom=m_rcTieKing.top+87;

	m_rcBanker.left=nCenterX+133;
	m_rcBanker.top=nCenterY-107;
	m_rcBanker.right=m_rcBanker.left+185;
	m_rcBanker.bottom=m_rcBanker.top+154;

	m_rcBankerKing.left=nCenterX+133;
	m_rcBankerKing.top=nCenterY+45;
	m_rcBankerKing.right=m_rcBankerKing.left+186;
	m_rcBankerKing.bottom=m_rcBankerKing.top+87;

	m_rcBankerTwoPair.left=nCenterX+133;
	m_rcBankerTwoPair.top=nCenterY-160;
	m_rcBankerTwoPair.right=m_rcBankerTwoPair.left+185;
	m_rcBankerTwoPair.bottom=m_rcBankerTwoPair.top+57;

	//筹码数字
	int ExcursionY=10;
	m_PointJettonNumber[ID_XIAN_JIA-1].SetPoint((m_rcPlayer.right+m_rcPlayer.left)/2, (m_rcPlayer.bottom+m_rcPlayer.top)/2-ExcursionY);
	m_PointJettonNumber[ID_XIAN_TIAN_WANG-1].SetPoint((m_rcPlayerKing.right+m_rcPlayerKing.left)/2, (m_rcPlayerKing.bottom+m_rcPlayerKing.top)/2-ExcursionY);
	m_PointJettonNumber[ID_PLAYER_TWO_PAIR-1].SetPoint((m_rcPlayerTwoPair.right+m_rcPlayerTwoPair.left)/2, (m_rcPlayerTwoPair.bottom+m_rcPlayerTwoPair.top)/2-ExcursionY);
	m_PointJettonNumber[ID_ZHUANG_JIA-1].SetPoint((m_rcBanker.right+m_rcBanker.left)/2, (m_rcBanker.bottom+m_rcBanker.top)/2-ExcursionY);
	m_PointJettonNumber[ID_ZHUANG_TIAN_WANG-1].SetPoint((m_rcBankerKing.right+m_rcBankerKing.left)/2, (m_rcBankerKing.bottom+m_rcBankerKing.top)/2-ExcursionY);
	m_PointJettonNumber[ID_BANKER_TWO_PAIR-1].SetPoint((m_rcBankerTwoPair.right+m_rcBankerTwoPair.left)/2, (m_rcBankerTwoPair.bottom+m_rcBankerTwoPair.top)/2-ExcursionY);
	m_PointJettonNumber[ID_PING_JIA-1].SetPoint((m_rcTie.right+m_rcTie.left)/2, (m_rcTie.bottom+m_rcTie.top)/2-ExcursionY);
	m_PointJettonNumber[ID_TONG_DIAN_PING-1].SetPoint((m_rcTieKing.right+m_rcTieKing.left)/2, (m_rcTieKing.bottom+m_rcTieKing.top)/2-ExcursionY);
	
	//筹码位置
	m_PointJetton[ID_XIAN_JIA-1].SetPoint(m_rcPlayer.left, m_rcPlayer.top);
	m_PointJetton[ID_XIAN_TIAN_WANG-1].SetPoint(m_rcPlayerKing.left, m_rcPlayerKing.top);
	m_PointJetton[ID_PLAYER_TWO_PAIR-1].SetPoint(m_rcPlayerTwoPair.left, m_rcPlayerTwoPair.top);
	m_PointJetton[ID_ZHUANG_JIA-1].SetPoint(m_rcBanker.left, m_rcBanker.top);
	m_PointJetton[ID_ZHUANG_TIAN_WANG-1].SetPoint(m_rcBankerKing.left, m_rcBankerKing.top);
	m_PointJetton[ID_BANKER_TWO_PAIR-1].SetPoint(m_rcBankerTwoPair.left, m_rcBankerTwoPair.top);
	m_PointJetton[ID_PING_JIA-1].SetPoint(m_rcTie.left, m_rcTie.top);
	m_PointJetton[ID_TONG_DIAN_PING-1].SetPoint(m_rcTieKing.left, m_rcTieKing.top);

	//扑克控件
	m_CardControl[0].SetBenchmarkPos(CPoint(nWidth/2-25+17,nHeight/2-190),enXRight,enYTop);
	m_CardControl[1].SetBenchmarkPos(CPoint(nWidth/2+25-15,nHeight/2-190),enXLeft,enYTop);

	//移动控件
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	//列表控件
	DeferWindowPos(hDwp,m_ApplyUser,NULL,nWidth/2 + 118,nHeight/2-315,232,80,uFlags);
	DeferWindowPos(hDwp,m_btUp,NULL,nWidth/2 + 351,nHeight/2-318,100/5,20,uFlags);
	DeferWindowPos(hDwp,m_btDown,NULL,nWidth/2 + 351,nHeight/2-250,100/5,20,uFlags);

	//筹码按钮
	CRect rcJetton;
	m_btJetton100.GetWindowRect(&rcJetton);
	int nYPos = nHeight/2+147;
	int nXPos = nWidth/2-130+8;
	int nSpace = 5;

	DeferWindowPos(hDwp,m_btJetton100,NULL,nXPos,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton1000,NULL,nXPos + nSpace + rcJetton.Width(),nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton10000,NULL,nXPos + nSpace * 2 + rcJetton.Width() * 2,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton100000,NULL,nXPos + nSpace * 3 + rcJetton.Width() * 3,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton1000000,NULL,nXPos + nSpace * 4 + rcJetton.Width() * 4,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton5000000,NULL,nXPos + nSpace * 5 + rcJetton.Width() * 5,nYPos,0,0,uFlags|SWP_NOSIZE);

	//上庄按钮
	DeferWindowPos(hDwp,m_btApplyBanker,NULL,nWidth/2+280,nHeight/2-352,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btCancelBanker,NULL,nWidth/2+280,nHeight/2-352,0,0,uFlags|SWP_NOSIZE);

	DeferWindowPos(hDwp,m_btScoreMoveL,NULL,nWidth/2-203,nHeight/2+272,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btScoreMoveR,NULL,nWidth/2+302,nHeight/2+270,0,0,uFlags|SWP_NOSIZE);

	//查看路子
	DeferWindowPos(hDwp,m_btViewChart,NULL,nWidth/2-260,nHeight/2+190,0,0,uFlags|SWP_NOSIZE);

	//银行
#ifdef __SPECIAL___
	DeferWindowPos(hDwp,m_btBankerStorage,NULL,nWidth/2-370,nHeight/2+190,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btBankerDraw,NULL,nWidth/2-315,nHeight/2+190,0,0,uFlags|SWP_NOSIZE);
#endif

	//结束移动
	EndDeferWindowPos(hDwp);

	return;
}

//绘画界面
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//绘画背景
	DrawViewImage(pDC,m_ImageViewFill,enMode_Spread);
	DrawViewImage(pDC,m_ImageViewBack,enMode_Centent);
	DrawAlphaRect(pDC, CRect(nWidth/2 + 351, nHeight/2-318, nWidth/2 + 351 + 20, nHeight/2-250+20), RGB(0,0,0), 0.5);

	//获取状态
	BYTE cbGameStatus=m_pGameClientDlg->GetGameStatus();

	//状态提示
	CFont static InfoFont;
	InfoFont.CreateFont(-16,0,0,0,400,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("宋体"));
	CFont * pOldFont=pDC->SelectObject(&InfoFont);
	pDC->SetTextColor(RGB(255,234,0));
	CRect rcDispatchCardTips(m_nWinPointsExcursionX, m_nWinPointsExcursionY+50, m_nWinPointsExcursionX + 220, m_nWinPointsExcursionY+115);
	
	if (m_pGameClientDlg->GetGameStatus()==GS_PLACE_JETTON)
		pDC->DrawText( TEXT("游戏开始了，请您下分"), rcDispatchCardTips, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_WORDBREAK );
	else if (m_pGameClientDlg->GetGameStatus()==GS_GAME_END)
		pDC->DrawText( m_strDispatchCardTips, rcDispatchCardTips, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_WORDBREAK );

	pDC->SelectObject(pOldFont);
	InfoFont.DeleteObject();

	//时间提示
	CImageHandle ImageHandleTimeFlag(&m_ImageTimeFlag);
	int nTimeFlagWidth = m_ImageTimeFlag.GetWidth()/3;
	int nFlagIndex=0;
	if (cbGameStatus==GS_FREE) nFlagIndex=0;
	else if (cbGameStatus==GS_PLACE_JETTON) nFlagIndex=1;
	else if (cbGameStatus==GS_GAME_END) nFlagIndex=2;
	m_ImageTimeFlag.BitBlt(pDC->GetSafeHdc(), nWidth/2-195, nHeight/2+195, nTimeFlagWidth, m_ImageTimeFlag.GetHeight(),
		nFlagIndex * nTimeFlagWidth, 0);

	//最大下注
	if (m_wBankerUser != INVALID_CHAIR || m_bEnableSysBanker)
	{
		pDC->SetTextColor(RGB(255,234,0));
		LONGLONG lLimitScore=GetMaxBankerScore();
		DrawNumberStringWithSpace(pDC,lLimitScore,nWidth/2-300, nHeight/2 - 347);
		lLimitScore=GetMaxPlayerScore();
		DrawNumberStringWithSpace(pDC,lLimitScore,nWidth/2-300, nHeight/2 - 322);
		lLimitScore=GetMaxTieScore();
		DrawNumberStringWithSpace(pDC,lLimitScore,nWidth/2-300, nHeight/2 - 299);
	}

	//胜利边框
	FlashJettonAreaFrame(nWidth,nHeight,pDC);

	//筹码资源
	CImageHandle HandleJettonView(&m_ImageJettonView);
	CSize SizeJettonItem(m_ImageJettonView.GetWidth()/JETTON_COUNT,m_ImageJettonView.GetHeight());

	//绘画筹码
	for (INT i=0;i<8;i++)
	{
		//变量定义
		LONGLONG lScoreCount=0L;
		LONGLONG lScoreJetton[JETTON_COUNT]={100L,1000L,10000L,100000L,1000000L,5000000L};

		//绘画筹码
		for (INT_PTR j=0;j<m_JettonInfoArray[i].GetCount();j++)
		{
			//获取信息
			tagJettonInfo * pJettonInfo=&m_JettonInfoArray[i][j];

			//累计数字
			ASSERT(pJettonInfo->cbJettonIndex<JETTON_COUNT);
			lScoreCount+=lScoreJetton[pJettonInfo->cbJettonIndex];

			//绘画界面
			m_ImageJettonView.AlphaDrawImage(pDC,pJettonInfo->nXPos+m_PointJetton[i].x,
				pJettonInfo->nYPos+m_PointJetton[i].y,SizeJettonItem.cx,SizeJettonItem.cy,
				pJettonInfo->cbJettonIndex*SizeJettonItem.cx,0,RGB(255,0,255));
		}

		//绘画数字
		if (lScoreCount>0L)	DrawNumberString(pDC,lScoreCount,m_PointJettonNumber[i].x,m_PointJettonNumber[i].y);
	}

	//庄家信息																											
	pDC->SetTextColor(RGB(255,234,0));

	//获取玩家
	tagUserData const *pUserData = m_wBankerUser==INVALID_CHAIR ? NULL : GetUserInfo(m_wBankerUser);

	//位置信息
	CRect static StrRect;
	StrRect.left = nWidth/2-85;
	StrRect.top = nHeight/2 - 340;
	StrRect.right = StrRect.left + 190;
	StrRect.bottom = StrRect.top + 15;

	//庄家名字
	pDC->DrawText(pUserData==NULL?(m_bEnableSysBanker?TEXT("系统坐庄"):TEXT("无人坐庄")):pUserData->szName, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_SINGLELINE );

	//庄家总分
	StrRect.left = nWidth/2-85;
	StrRect.top = nHeight/2 - 316;
	StrRect.right = StrRect.left + 190;
	StrRect.bottom = StrRect.top + 15;
	DrawNumberStringWithSpace(pDC,pUserData==NULL?0:pUserData->lScore, StrRect);

	//庄家局数
	StrRect.left = nWidth/2-85;
	StrRect.top = nHeight/2 - 266;
	StrRect.right = StrRect.left + 190;
	StrRect.bottom = StrRect.top + 15;
	DrawNumberStringWithSpace(pDC,m_wBankerTime,StrRect);

	//庄家成绩
	StrRect.left = nWidth/2-85;
	StrRect.top = nHeight/2 - 291;
	StrRect.right = StrRect.left + 190;
	StrRect.bottom = StrRect.top + 15;
	DrawNumberStringWithSpace(pDC,m_lBankerWinScore,StrRect);

	//绘画用户
	if (m_wMeChairID!=INVALID_CHAIR)
	{
		const tagUserData *pMeUserData = GetUserInfo(m_wMeChairID);
		if ( pMeUserData != NULL )
		{
			//游戏信息
			pDC->SetTextColor(RGB(255,255,255));
			LONGLONG lMeJetton = m_lMeTieScore+m_lMeBankerScore+m_lMePlayerScore+m_lMeTieSamePointScore+m_lMePlayerKingScore+
				m_lMeBankerKingScore+m_lMePlayerTwoPair+m_lMeBankerTwoPair;
			int iLeftX = nWidth/2-312, iW = 90;
			CRect rcAccount(CPoint(iLeftX,nHeight/2+268),CPoint(iLeftX+iW,nHeight/2+272+15));
			CRect rcGameScore(CPoint(iLeftX,nHeight/2+294),CPoint(iLeftX+iW,nHeight/2+294+10));
			CRect rcResultScore(CPoint(iLeftX,nHeight/2+294+23),CPoint(iLeftX+iW,nHeight/2+294+10+23));
			DrawNumberStringWithSpace(pDC,pMeUserData->lScore-lMeJetton,rcGameScore,DT_VCENTER|DT_SINGLELINE|DT_CENTER|DT_END_ELLIPSIS);
			DrawNumberStringWithSpace(pDC,m_lMeStatisticScore,rcResultScore,DT_VCENTER|DT_SINGLELINE|DT_CENTER|DT_END_ELLIPSIS);
			pDC->DrawText(pMeUserData->szName,lstrlen(pMeUserData->szName),rcAccount,DT_VCENTER|DT_SINGLELINE|DT_CENTER|DT_END_ELLIPSIS);
		}
	}

	//切换庄家
	if ( m_bShowChangeBanker )
	{
		int	nXPos = nWidth / 2 - 130;
		int	nYPos = nHeight / 2 - 160;

		//由我做庄
		if ( m_wMeChairID == m_wBankerUser )
		{
			CImageHandle ImageHandleBanker(&m_ImageMeBanker);
			m_ImageMeBanker.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);
		}
		else if ( m_wBankerUser != INVALID_CHAIR )
		{
			CImageHandle ImageHandleBanker(&m_ImageChangeBanker);
			m_ImageChangeBanker.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);
		}
		else
		{
			CImageHandle ImageHandleBanker(&m_ImageNoBanker);
			m_ImageNoBanker.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);
		}
	}


	//我的下注
	DrawMeJettonNumber(pDC);

	//绘画时间
	if (m_wMeChairID!=INVALID_CHAIR)
	{
		WORD wUserTimer=GetUserTimer(m_wMeChairID);
		if (wUserTimer!=0) DrawUserTimer(pDC,nWidth/2-160,nHeight/2-260+430,wUserTimer);
	}

	//结束状态
	if (cbGameStatus==GS_GAME_END)
	{
		CImageHandle ImageHandleGameEndFrame(&m_ImageGameEndFrame);
		int	nXPos = nWidth / 2 - 129;
		int	nYPos = nHeight / 2 - 208;

		CRect rcAlpha(nXPos, nYPos, nXPos+ m_ImageGameEndFrame.GetWidth(), nYPos+m_ImageGameEndFrame.GetHeight());
		DrawAlphaRect(pDC, &rcAlpha, RGB(74,70,73), 0.8f);

		//绘画边框
		m_ImageGameEndFrame.AlphaDrawImage(pDC, nXPos, nYPos, RGB(255,0,255));

		//绘画扑克
		m_CardControl[INDEX_PLAYER].DrawCardControl(pDC);
		m_CardControl[INDEX_BANKER].DrawCardControl(pDC);

		//发牌扑克
		if (m_ptDispatchCard.x!=0 && m_ptDispatchCard.y!=0) 
		{
			m_DispatchCard.SetBenchmarkPos(m_ptDispatchCard.x,m_ptDispatchCard.y,enXCenter,enYCenter);
			m_DispatchCard.DrawCardControl(pDC);
		}		
	}

	//胜利标志
	DrawWinFlags(pDC);

	//显示结果
	ShowGameResult(pDC, nWidth, nHeight);	

	//扑克点数
	if (m_cbSendCount[INDEX_PLAYER]+m_cbSendCount[INDEX_BANKER]!=0)
	{
		//变量定义
		CImageHandle ImageHandleGamePoint(&m_ImageGamePoint);
		BYTE cbPlayerPoint, cbBankerPoint;
		int	nXPos = nWidth / 2 - 129;
		int	nYPos = nHeight / 2 - 208;		

		//绘画点数
		int nPointWidth = m_ImageGamePoint.GetWidth() / 10;
		if (m_cbSendCount[INDEX_PLAYER]!=0)
		{
			//计算点数
			cbPlayerPoint = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER], m_cbSendCount[INDEX_PLAYER]);
			m_ImageGamePoint.AlphaDrawImage(pDC, nXPos + 28, nYPos + 10, nPointWidth, m_ImageGamePoint.GetHeight(),
				cbPlayerPoint * nPointWidth, 0, RGB(255, 0, 255));
		}
		if (m_cbSendCount[INDEX_BANKER]!=0)
		{
			//计算点数
			cbBankerPoint = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER], m_cbSendCount[INDEX_BANKER]);
			m_ImageGamePoint.AlphaDrawImage(pDC, nXPos + 28 + 136, nYPos + 10, nPointWidth, m_ImageGamePoint.GetHeight(),
				cbBankerPoint * nPointWidth, 0, RGB(255, 0, 255));
		}
	}

	//爆炸效果
	DrawBombEffect(pDC);
	
	return;
}

//设置信息
void CGameClientView::SetMeMaxScore(LONGLONG lMeMaxScore)
{
	if (m_lMeMaxScore!=lMeMaxScore)
	{
		//设置变量
		m_lMeMaxScore=lMeMaxScore;
	}

	return;
}

//最大下注
void CGameClientView::SetAreaLimitScore(LONGLONG lAreaLimitScore)
{
	if ( m_lAreaLimitScore!= lAreaLimitScore )
	{
		//设置变量
		m_lAreaLimitScore=lAreaLimitScore;
	}
}

//设置筹码
void CGameClientView::SetCurrentJetton(LONGLONG lCurrentJetton)
{
	//设置变量
	ASSERT(lCurrentJetton>=0L);
	m_lCurrentJetton=lCurrentJetton;

	if (m_lCurrentJetton == 0L && m_pGameClientDlg->GetGameStatus()!=GS_GAME_END && m_cbAreaFlash!=0xFF)
	{
		m_cbAreaFlash=0xFF;
		UpdateGameView(NULL);
	}

	return;
}

//历史记录
void CGameClientView::SetGameHistory(enOperateResult OperateResult, BYTE cbPlayerCount, BYTE cbBankerCount, BYTE cbKingWinner, bool bPlayerTwoPair, bool bBankerTwoPair)
{
	//设置数据
	tagClientGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.enOperateFlags = OperateResult;
	GameRecord.cbPlayerCount = cbPlayerCount;
	GameRecord.cbBankerCount = cbBankerCount;
	GameRecord.bPlayerTwoPair=bPlayerTwoPair;
	GameRecord.bBankerTwoPair=bBankerTwoPair;
	GameRecord.cbKingWinner=cbKingWinner;

	//移动下标
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst )
	{
		m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
		if ( m_nScoreHead < m_nRecordFirst ) m_nScoreHead = m_nRecordFirst;
	}

	int nHistoryCount = (m_nRecordLast - m_nRecordFirst + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
	if ( 12 < nHistoryCount ) m_btScoreMoveR.EnableWindow(TRUE);

	//移到最新记录
	if ( 12 < nHistoryCount )
	{
		m_nScoreHead = (m_nRecordLast - 12 + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
		m_btScoreMoveL.EnableWindow(TRUE);
		m_btScoreMoveR.EnableWindow(FALSE);
	}

	//更新路子
	if (m_DlgViewChart.m_hWnd!=NULL)
	{
		m_DlgViewChart.SetGameRecord(GameRecord);
	}

	return;
}

//清理筹码
void CGameClientView::CleanUserJetton()
{
	//清理数组
	for (BYTE i=0;i<CountArray(m_JettonInfoArray);i++)
	{
		m_JettonInfoArray[i].RemoveAll();
	}

	//下注信息
	m_lMeTieScore=0L;
	m_lMeBankerScore=0L;
	m_lMePlayerScore=0L;
	m_lMeTieSamePointScore=0L;
	m_lMePlayerKingScore=0L;
	m_lMeBankerKingScore=0L;
	m_lMePlayerTwoPair=0L;
	m_lMeBankerTwoPair=0L;
	
	//全体下注
	m_lAllTieScore=0L;
	m_lAllBankerScore=0L;
	m_lAllPlayerScore=0L;
	m_lAllTieSamePointScore=0L;
	m_lAllBankerKingScore=0L;
	m_lAllPlayerKingScore=0L;
	m_lAllPlayerTwoPair=0L;
	m_lAllBankerTwoPair=0L;

	m_strDispatchCardTips=TEXT("");
	
	//更新界面
	UpdateGameView(NULL);

	return;
}

//个人下注
void CGameClientView::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	//效验参数
	ASSERT(cbViewIndex<=ID_BANKER_TWO_PAIR);
	if (cbViewIndex>ID_BANKER_TWO_PAIR) return;

	if (cbViewIndex==ID_XIAN_JIA) m_lMePlayerScore=lJettonCount;
	else if (cbViewIndex==ID_PING_JIA) m_lMeTieScore=lJettonCount;
	else if (cbViewIndex==ID_ZHUANG_JIA) m_lMeBankerScore=lJettonCount;
	else if (cbViewIndex==ID_XIAN_TIAN_WANG) m_lMePlayerKingScore=lJettonCount;
	else if (cbViewIndex==ID_ZHUANG_TIAN_WANG) m_lMeBankerKingScore=lJettonCount;
	else if (cbViewIndex==ID_TONG_DIAN_PING) m_lMeTieSamePointScore=lJettonCount;
	else if (cbViewIndex==ID_BANKER_TWO_PAIR) m_lMeBankerTwoPair=lJettonCount;
	else if (cbViewIndex==ID_PLAYER_TWO_PAIR) m_lMePlayerTwoPair=lJettonCount;
	else {ASSERT(FALSE);return;}

	//更新界面
	UpdateGameView(NULL);
}

//设置扑克
void CGameClientView::SetCardInfo(BYTE cbCardCount[2], BYTE cbTableCardArray[2][3])
{
	if (cbCardCount!=NULL)
	{
		m_cbCardCount[0]=cbCardCount[0];
		m_cbCardCount[1]=cbCardCount[1];
		CopyMemory(m_cbTableCardArray,cbTableCardArray,sizeof(m_cbTableCardArray));

		//开始发牌
		DispatchCard();
	}
	else
	{
		ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
		ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));
	}
}

//设置筹码
void CGameClientView::PlaceUserJetton(BYTE cbViewIndex, LONGLONG lScoreCount)
{
	//效验参数
	ASSERT(cbViewIndex<=ID_BANKER_TWO_PAIR);
	if (cbViewIndex>ID_BANKER_TWO_PAIR) return;

	//设置炸弹
	if (lScoreCount==5000000L) SetBombEffect(true,cbViewIndex);

	//变量定义
	bool bPlaceJetton=false;
	LONGLONG lScoreIndex[JETTON_COUNT]={100L,1000L,10000L,100000L,1000000L,5000000L};

	//边框宽度
	int nFrameWidth=0, nFrameHeight=0;
	int nBorderWidth=6;

	switch (cbViewIndex)
	{
	case ID_XIAN_JIA:
		{ 
			m_lAllPlayerScore += lScoreCount;
			nFrameWidth = m_rcPlayer.right-m_rcPlayer.left;
			nFrameHeight = m_rcPlayer.bottom-m_rcPlayer.top;
			break;
		}
	case ID_PING_JIA:
		{
			m_lAllTieScore += lScoreCount;
			nFrameWidth = m_rcTie.right-m_rcTie.left;
			nFrameHeight = m_rcTie.bottom-m_rcTie.top;
			break;
		}
	case ID_ZHUANG_JIA:
		{ 
			m_lAllBankerScore += lScoreCount;
			nFrameWidth = m_rcBanker.right-m_rcBanker.left;
			nFrameHeight = m_rcBanker.bottom-m_rcBanker.top;
			break;
		}
	case ID_XIAN_TIAN_WANG:
		{ 
			m_lAllPlayerKingScore += lScoreCount;
			nFrameWidth = m_rcPlayerKing.right-m_rcPlayerKing.left;
			nFrameHeight = m_rcPlayerKing.bottom-m_rcPlayerKing.top;
			break;
		}
	case ID_ZHUANG_TIAN_WANG:
		{ 
			m_lAllBankerKingScore += lScoreCount;
			nFrameWidth = m_rcBankerKing.right-m_rcBankerKing.left;
			nFrameHeight = m_rcBankerKing.bottom-m_rcBankerKing.top;
			break;
		}
	case ID_TONG_DIAN_PING:
		{ 
			m_lAllTieSamePointScore += lScoreCount;
			nFrameWidth = m_rcTieKing.right-m_rcTieKing.left;
			nFrameHeight = m_rcTieKing.bottom-m_rcTieKing.top;
			break;
		}
	case ID_BANKER_TWO_PAIR:
		{ 
			m_lAllBankerTwoPair += lScoreCount;
			nFrameWidth = m_rcBankerTwoPair.right-m_rcBankerTwoPair.left;
			nFrameHeight = m_rcBankerTwoPair.bottom-m_rcBankerTwoPair.top;
			break;
		}
	case ID_PLAYER_TWO_PAIR:
		{ 
			m_lAllPlayerTwoPair += lScoreCount;
			nFrameWidth = m_rcPlayerTwoPair.right-m_rcPlayerTwoPair.left;
			nFrameHeight = m_rcPlayerTwoPair.bottom-m_rcPlayerTwoPair.top;
			break;
		}
	default:
		{
			ASSERT(FALSE);
			return;
		}
	}

	nFrameWidth += nBorderWidth;
	nFrameHeight += nBorderWidth;

	//增加判断
	bool bAddJetton=(lScoreCount>0L)?true:false;
	if(lScoreCount < 0L) lScoreCount = (lScoreCount*-1L);

	//增加筹码
	for (BYTE i=0;i<CountArray(lScoreIndex);i++)
	{
		//计算数目
		BYTE cbScoreIndex=JETTON_COUNT-i-1;
		LONGLONG lCellCount=lScoreCount/lScoreIndex[cbScoreIndex];

		//插入过虑
		if (lCellCount==0L) continue;

		//加入筹码
		for (LONGLONG j=0;j<lCellCount;j++)
		{
			if (true==bAddJetton)
			{
				//构造变量
				tagJettonInfo JettonInfo;
				int nJettonSize=68;
				JettonInfo.cbJettonIndex=cbScoreIndex;
				JettonInfo.nXPos=rand()%(nFrameWidth-nJettonSize);
				JettonInfo.nYPos=rand()%(nFrameHeight-nJettonSize);

				//插入数组
				bPlaceJetton=true;
				m_JettonInfoArray[cbViewIndex-1].Add(JettonInfo);
			}
			else
			{
				for (int nIndex=0; nIndex<m_JettonInfoArray[cbViewIndex-1].GetCount(); ++nIndex)
				{
					//移除判断
					tagJettonInfo &JettonInfo=m_JettonInfoArray[cbViewIndex-1][nIndex];
					if (JettonInfo.cbJettonIndex==cbScoreIndex)
					{
						m_JettonInfoArray[cbViewIndex-1].RemoveAt(nIndex);
						break;
					}
				}
			}
		}

		//减少数目
		lScoreCount-=lCellCount*lScoreIndex[cbScoreIndex];
	}

	//更新界面
	if (bPlaceJetton==true) UpdateGameView(NULL);

	return;
}

//当局成绩
void CGameClientView::SetCurGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore, LONGLONG lGameRevenue)
{
	m_lMeCurGameScore=lMeCurGameScore;			
	m_lMeCurGameReturnScore=lMeCurGameReturnScore;			
	m_lBankerCurGameScore=lBankerCurGameScore;			
	m_lGameRevenue=lGameRevenue;					
}

//设置胜方
void CGameClientView::SetWinnerSide(BYTE cbWinnerSide,bool blSet)
{
	//设置变量
	m_cbWinnerSide=cbWinnerSide;
	m_cbAreaFlash=cbWinnerSide;
	m_bFlashResult=blSet;

	//设置时间
	if (cbWinnerSide!=0xFF)
	{
		SetTimer(IDI_FLASH_WINNER,500,NULL);
	}
	else KillTimer(IDI_FLASH_WINNER);

	//更新界面
	UpdateGameView(NULL);

	return;
}

//获取区域
BYTE CGameClientView::GetJettonArea(CPoint MousePoint)
{
	if (m_rcPlayer.PtInRect(MousePoint)) return ID_XIAN_JIA;
	if (m_rcTie.PtInRect(MousePoint)) return ID_PING_JIA;
	if (m_rcBanker.PtInRect(MousePoint)) return ID_ZHUANG_JIA;
	if (m_rcPlayerKing.PtInRect(MousePoint)) return ID_XIAN_TIAN_WANG;	
	if (m_rcTieKing.PtInRect(MousePoint)) return ID_TONG_DIAN_PING;	
	if (m_rcBankerKing.PtInRect(MousePoint)) return ID_ZHUANG_TIAN_WANG;
	if (m_rcPlayerTwoPair.PtInRect(MousePoint)) return ID_PLAYER_TWO_PAIR;
	if (m_rcBankerTwoPair.PtInRect(MousePoint)) return ID_BANKER_TWO_PAIR;

	return 0xFF;
}

//绘画数字
void CGameClientView::DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos, bool bMeScore)
{
	//加载资源
	CImageHandle HandleScoreNumber(&m_ImageScoreNumber);
	CImageHandle HandleMeScoreNumber(&m_ImageMeScoreNumber);
	CSize SizeScoreNumber(m_ImageScoreNumber.GetWidth()/10,m_ImageScoreNumber.GetHeight());

	if ( bMeScore ) SizeScoreNumber.SetSize(m_ImageMeScoreNumber.GetWidth()/10, m_ImageMeScoreNumber.GetHeight());

	//计算数目
	LONG lNumberCount=0;
	LONGLONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	//位置定义
	INT nYDrawPos=nYPos-SizeScoreNumber.cy/2;
	INT nXDrawPos=nXPos+lNumberCount*SizeScoreNumber.cx/2-SizeScoreNumber.cx;

	//绘画桌号
	for (LONG i=0;i<lNumberCount;i++)
	{
		//绘画号码
		LONG lCellNumber=(LONG)(lNumber%10);
		if ( bMeScore )
		{
			m_ImageMeScoreNumber.AlphaDrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
				lCellNumber*SizeScoreNumber.cx,0,RGB(255,0,255));
		}
		else
		{
			m_ImageScoreNumber.AlphaDrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
				lCellNumber*SizeScoreNumber.cx,0,RGB(255,0,255));
		}

		//设置变量
		lNumber/=10;
		nXDrawPos-=SizeScoreNumber.cx;
	};

	return;
}

//绘画数字
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos)
{
	CString strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;
	if (lNumber==0) strNumber=TEXT("0");
	int nNumberCount=0;
	LONGLONG lTmpNumber=lNumber;
	if (lNumber<0) lNumber=-lNumber;
	while (lNumber>0)
	{
		strTmpNumber1.Format(TEXT("%I64d"),lNumber%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if (nNumberCount==3)
		{
			if(strNumber.GetLength() > 1)
				strTmpNumber2 += (TEXT(",") +strNumber);
			strNumber=strTmpNumber2;
			nNumberCount=0;
			strTmpNumber2=TEXT("");
		}
		lNumber/=10;
	}

	if (strTmpNumber2.IsEmpty()==FALSE)
	{
		if(strNumber.GetLength() > 1)
			strTmpNumber2 += (TEXT(",") +strNumber);
		strNumber=strTmpNumber2;
	}

	if (lTmpNumber<0) strNumber=TEXT("-")+strNumber;

	//输出数字
	pDC->TextOut(nXPos,nYPos,strNumber);
}

//绘画数字
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, CRect rcRect, INT nFormat)
{
	CString strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;
	if (lNumber==0) strNumber=TEXT("0");
	int nNumberCount=0;
	LONGLONG lTmpNumber=lNumber;
	if (lNumber<0) lNumber=-lNumber;
	while (lNumber>0)
	{
		strTmpNumber1.Format(TEXT("%I64d"),lNumber%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if (nNumberCount==3)
		{
			if(strNumber.GetLength() > 1)
				strTmpNumber2 += (TEXT(",") +strNumber);
			strNumber=strTmpNumber2;
			nNumberCount=0;
			strTmpNumber2=TEXT("");
		}
		lNumber/=10;
	}

	if (strTmpNumber2.IsEmpty()==FALSE)
	{
		if(strNumber.GetLength() > 1)
			strTmpNumber2 += (TEXT(",") +strNumber);
		strNumber=strTmpNumber2;
	}

	if (lTmpNumber<0) strNumber=TEXT("-")+strNumber;

	//输出数字
	if (nFormat==-1) pDC->DrawText(strNumber,rcRect,DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);
	else pDC->DrawText(strNumber,rcRect,nFormat);
}

//筹码按钮
void CGameClientView::OnJettonButton100()
{
	//设置变量
	m_lCurrentJetton=100L;

	return;
}

//筹码按钮
void CGameClientView::OnJettonButton1000()
{
	//设置变量
	m_lCurrentJetton=1000L;

	return;
}

//筹码按钮
void CGameClientView::OnJettonButton10000()
{
	//设置变量
	m_lCurrentJetton=10000L;

	return;
}

//筹码按钮
void CGameClientView::OnJettonButton100000()
{
	//设置变量
	m_lCurrentJetton=100000L;

	return;
}

//筹码按钮
void CGameClientView::OnJettonButton1000000()
{
	//设置变量
	m_lCurrentJetton=1000000L;

	return;
}
//筹码按钮
void CGameClientView::OnJettonButton5000000()
{
	//设置变量
	m_lCurrentJetton=5000000L;

	return;
}
//查看路子
void CGameClientView::OnViewChart()
{
	if (m_DlgViewChart.m_hWnd==NULL) m_DlgViewChart.Create(IDD_VIEW_CHART,this);

	//显示判断
	if (!m_DlgViewChart.IsWindowVisible())
	{
		m_DlgViewChart.ShowWindow(SW_SHOW);
		m_DlgViewChart.CenterWindow(this);
	}
	else
	{
		m_DlgViewChart.ShowWindow(SW_HIDE);
	}
}

//定时器消息
void CGameClientView::OnTimer(UINT nIDEvent)
{
	//闪动胜方
	if (nIDEvent==IDI_FLASH_WINNER)
	{
		//设置变量
		if (m_cbAreaFlash!=m_cbWinnerSide)
		{
			m_cbAreaFlash=m_cbWinnerSide;
		}
		else 
		{
			m_cbAreaFlash=0xFF;
		}
		//设置变量
		m_bFlashResult=!m_bFlashResult;

		//更新界面
		UpdateGameView(NULL);

		return;
	}

	//轮换庄家
	if ( nIDEvent == IDI_SHOW_CHANGE_BANKER )
	{
		ShowChangeBanker( false );

		return;
	}

	else if (nIDEvent==IDI_DISPATCH_CARD)
	{
		bool bPlayerCard=false;
		if ((m_cbSendCount[0]+m_cbSendCount[1])<4)
		{
			//定义索引
			WORD wIndex=(m_cbSendCount[0]+m_cbSendCount[1])%2;
			if (wIndex==0) bPlayerCard=true;
		}
		else if (m_cbSendCount[INDEX_PLAYER]<m_cbCardCount[INDEX_PLAYER]) bPlayerCard=true;

		//发牌判断
		CRect rcView;
		GetClientRect(&rcView);
		int nEndPosX = 0;
		if (bPlayerCard) nEndPosX=rcView.Width()/2;
		else nEndPosX=rcView.Width()/2+110;
		int nEndPosY = rcView.Height()/2-185;
		if ( (m_ptDispatchCard.x > nEndPosX || m_ptDispatchCard.y < nEndPosY) &&
			m_ptDispatchCard.x != 0 && m_ptDispatchCard.y != 0 )
		{
			//修改位置
			if (m_ptDispatchCard.y < nEndPosY) m_ptDispatchCard.y += 73; 
			else m_ptDispatchCard.x -= 115;

			//更新界面
			UpdateGameView(NULL);

			return;
		}

		//停顿扑克
		static BYTE cbStopCount=0;
		if (cbStopCount<6)
		{			
			cbStopCount++;
			return;
		}
		else 
		{
			cbStopCount=0;			
		}

		if ( m_cbSendCount[INDEX_PLAYER]+m_cbSendCount[INDEX_BANKER]<m_cbCardCount[INDEX_PLAYER] + m_cbCardCount[INDEX_BANKER]-1 ) 
		{
			m_ptDispatchCard.x = rcView.Width()/2+220;
			m_ptDispatchCard.y = rcView.Height()/2-200;	
		}
		else
		{
			m_DispatchCard.SetCardData(NULL,0);
		}

		//播放声音
		m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("DISPATCH_CARD"));

		//发送扑克
		if ((m_cbSendCount[0]+m_cbSendCount[1])<4)
		{
			//定义索引
			WORD wIndex=(m_cbSendCount[0]+m_cbSendCount[1])%2;

			//设置扑克
			m_cbSendCount[wIndex]++;
			m_CardControl[wIndex].SetCardData(m_cbTableCardArray[wIndex],m_cbSendCount[wIndex]);

			//发牌提示
			SetDispatchCardTips();

			//更新界面
			UpdateGameView(NULL);

			return;
		}

		//闲三扑克
		if (m_cbSendCount[INDEX_PLAYER]<m_cbCardCount[INDEX_PLAYER])
		{
			//设置扑克
			m_cbSendCount[INDEX_PLAYER]++;
			m_CardControl[INDEX_PLAYER].SetCardData(m_cbTableCardArray[INDEX_PLAYER],m_cbSendCount[INDEX_PLAYER]);

			//发牌提示
			SetDispatchCardTips();

			//更新界面
			UpdateGameView(NULL);
			return;
		}

		//庄三扑克
		if (m_cbSendCount[INDEX_BANKER]<m_cbCardCount[INDEX_BANKER])
		{
			//设置扑克
			m_cbSendCount[INDEX_BANKER]++;
			m_CardControl[INDEX_BANKER].SetCardData(m_cbTableCardArray[INDEX_BANKER],m_cbSendCount[INDEX_BANKER]);

			//发牌提示
			SetDispatchCardTips();

			return;
		}

		m_ptDispatchCard.x = 0;
		m_ptDispatchCard.y = 0;

        //完成发牌
		FinishDispatchCard();

		//更新界面
		UpdateGameView(NULL);

		return;
	}

	//爆炸动画
	if (nIDEvent<=IDI_BOMB_EFFECT+ID_BANKER_TWO_PAIR && IDI_BOMB_EFFECT+ID_XIAN_JIA<=nIDEvent)
	{
		WORD wIndex=nIDEvent-IDI_BOMB_EFFECT;
		//停止判断
		if (m_bBombEffect[wIndex]==false)
		{
			KillTimer(nIDEvent);
			return;
		}

		//设置变量
		if ((m_cbBombFrameIndex[wIndex]+1)>=BOMB_EFFECT_COUNT)
		{
			//删除时间
			KillTimer(nIDEvent);

			//设置变量
			m_bBombEffect[wIndex]=false;
			m_cbBombFrameIndex[wIndex]=0;
		}
		else m_cbBombFrameIndex[wIndex]++;

		//更新界面
		UpdateGameView(NULL);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//鼠标消息
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{



	if (m_lCurrentJetton!=0L)
	{
		BYTE cbJettonArea=GetJettonArea(Point);

		//大小判断
		switch( cbJettonArea )
		{
		case ID_XIAN_JIA:
			{
				LONGLONG lMaxPlayerScore = GetMaxPlayerScore();
				if ( lMaxPlayerScore < m_lCurrentJetton )
					return ;
				break;
			}
		case ID_PING_JIA:
			{
				LONGLONG lMaxTieScore  = GetMaxTieScore();
				if ( lMaxTieScore < m_lCurrentJetton )
					return ;
				break;
			}
		case ID_ZHUANG_JIA:
			{
				LONGLONG lMaxBankerScore = GetMaxBankerScore();
				if ( lMaxBankerScore < m_lCurrentJetton )
					return ;
				break;
			}
		case ID_TONG_DIAN_PING:
			{
				LONGLONG lMaxTieKingScore= GetMaxTieKingScore();
				if ( lMaxTieKingScore < m_lCurrentJetton )
					return ;
				break;
			}
		case ID_XIAN_TIAN_WANG:
			{
				LONGLONG lMaxPlayerKingScore = GetMaxPlayerKingScore();
				if ( lMaxPlayerKingScore < m_lCurrentJetton )
					return ;
				break;
			}
		case ID_ZHUANG_TIAN_WANG:
			{
				LONGLONG lMaxBankerKingScore = GetMaxBankerKingScore();
				if ( lMaxBankerKingScore < m_lCurrentJetton )
					return ;
				break;
			}
		case ID_PLAYER_TWO_PAIR:
			{
				LONGLONG lMaxPlayerTwoPair=GetMaxPlayerTwoPair();
				if (lMaxPlayerTwoPair<m_lCurrentJetton)
					return ;
				break;
			}
		case ID_BANKER_TWO_PAIR:
			{
				LONGLONG lMaxBankerTwoPair=GetMaxBankerTwoPair();
				if (lMaxBankerTwoPair<m_lCurrentJetton)
					return ;
				break;
			}
		}

		if (cbJettonArea!=0xFF) AfxGetMainWnd()->SendMessage(IDM_PLACE_JETTON,cbJettonArea,0);
	}

	__super::OnLButtonDown(nFlags,Point);
}

//鼠标消息
void CGameClientView::OnRButtonDown(UINT nFlags, CPoint Point)
{
	//设置变量
	m_lCurrentJetton=0L;

	if (m_pGameClientDlg->GetGameStatus()!=GS_GAME_END && m_cbAreaFlash!=0xFF)
	{
		m_cbAreaFlash=0xFF;
		UpdateGameView(NULL);
	}

	__super::OnLButtonDown(nFlags,Point);
}

//光标消息
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	if (m_lCurrentJetton!=0L)
	{
		//获取区域
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);
		BYTE cbJettonArea=GetJettonArea(MousePoint);

		//设置变量
		if ( m_cbAreaFlash!= cbJettonArea )
		{
			m_cbAreaFlash = cbJettonArea;
			UpdateGameView(NULL);
		}

		//区域判断
		if (cbJettonArea==0xFF)
		{
			SetCursor(LoadCursor(NULL,IDC_ARROW));
			return TRUE;
		}

		//大小判断
		switch( cbJettonArea )
		{
		case ID_XIAN_JIA:
			{
				LONGLONG lMaxPlayerScore = GetMaxPlayerScore();
				if ( lMaxPlayerScore < m_lCurrentJetton )
				{
					SetCursor(LoadCursor(NULL,IDC_NO));
					return TRUE;
				}
				break;
			}
		case ID_PING_JIA:
			{
				LONGLONG lMaxTieScore  = GetMaxTieScore();
				if ( lMaxTieScore < m_lCurrentJetton )
				{
					SetCursor(LoadCursor(NULL,IDC_NO));
					return TRUE;
				}
				break;
			}
		case ID_ZHUANG_JIA:
			{
				LONGLONG lMaxBankerScore = GetMaxBankerScore();
				if ( lMaxBankerScore < m_lCurrentJetton  )
				{
					SetCursor(LoadCursor(NULL,IDC_NO));
					return TRUE;
				};
				break;
			}
		case ID_TONG_DIAN_PING:
			{
				LONGLONG lMaxTieKingScore= GetMaxTieKingScore();
				if ( lMaxTieKingScore < m_lCurrentJetton )
				{
					SetCursor(LoadCursor(NULL,IDC_NO));
					return TRUE;
				}
				break;
			}
		case ID_XIAN_TIAN_WANG:
			{
				LONGLONG lMaxPlayerKingScore = GetMaxPlayerKingScore();
				if ( lMaxPlayerKingScore < m_lCurrentJetton )
				{
					SetCursor(LoadCursor(NULL,IDC_NO));
					return TRUE;
				}
				break;
			}
		case ID_ZHUANG_TIAN_WANG:
			{
				LONGLONG lMaxBankerKingScore = GetMaxBankerKingScore();
				if ( lMaxBankerKingScore < m_lCurrentJetton )
				{
					SetCursor(LoadCursor(NULL,IDC_NO));
					return TRUE;
				}
				break;
			}
		}


		//设置光标
		switch (m_lCurrentJetton)
		{
		case 100:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_100)));
				return TRUE;
			}
		case 1000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1000)));
				return TRUE;
			}
		case 10000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_10000)));
				return TRUE;
			}
		case 100000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_100000)));
				return TRUE;
			}
		case 1000000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1000000)));
				return TRUE;
			}
		case 5000000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_5000000)));
				return TRUE;
			}		
		}
	}
	else
	{
		ClearAreaFlash();
	}

	return __super::OnSetCursor(pWnd, nHitTest, uMessage);
}

//轮换庄家
void CGameClientView::ShowChangeBanker( bool bChangeBanker )
{
	//轮换庄家
	if ( bChangeBanker )
	{
		SetTimer( IDI_SHOW_CHANGE_BANKER, 3000, NULL );
		m_bShowChangeBanker = true;
	}
	else
	{
		KillTimer( IDI_SHOW_CHANGE_BANKER );
		m_bShowChangeBanker = false ;
	}

	//更新界面
	UpdateGameView(NULL);
}

//上庄按钮
void CGameClientView::OnApplyBanker()
{
	AfxGetMainWnd()->SendMessage(IDM_APPLY_BANKER,1,0);

}

//下庄按钮
void CGameClientView::OnCancelBanker()
{
	AfxGetMainWnd()->SendMessage(IDM_APPLY_BANKER,0,0);
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
//庄家信息
void CGameClientView::SetBankerInfo(WORD wBankerUser, LONGLONG lBankerScore) 
{
	//切换判断
	if (m_wBankerUser!=wBankerUser)
	{
		m_wBankerUser=wBankerUser;
		m_wBankerTime=0L;
		m_lBankerWinScore=0L;	
		m_lTmpBankerWinScore=0L;
	}
	m_lBankerScore=lBankerScore;
}

//绘画标识
void CGameClientView::DrawWinFlags(CDC * pDC)
{
	CImageHandle ImageHandleWinFlags(&m_ImageWinFlags);
	int nIndex = m_nScoreHead;
	COLORREF clrOld ;
	clrOld = pDC->SetTextColor(RGB(255,234,0));
	CString strPoint;
	int nDrawCount = 0;
	while ( nIndex != m_nRecordLast && ( m_nRecordLast!=m_nRecordFirst ) && nDrawCount < 12 )
	{
		tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[nIndex];
		int nYPos = 0;
		if ( ClientGameRecord.cbBankerCount > ClientGameRecord.cbPlayerCount ) nYPos = m_nWinFlagsExcursionY ;
		else if (ClientGameRecord.cbBankerCount < ClientGameRecord.cbPlayerCount ) nYPos = m_nWinFlagsExcursionY + 31;
		else nYPos = m_nWinFlagsExcursionY + 2 * 31;

		int nXPos = m_nWinFlagsExcursionX + ((nIndex - m_nScoreHead + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY) * 37;

		int nFlagsIndex = 0;
		if ( ClientGameRecord.enOperateFlags == enOperateResult_NULL ) nFlagsIndex = 0;
		else if ( ClientGameRecord.enOperateFlags == enOperateResult_Win) nFlagsIndex = 1;
		else if ( ClientGameRecord.enOperateFlags == enOperateResult_Lost) nFlagsIndex = 2;

		m_ImageWinFlags.AlphaDrawImage( pDC, nXPos, nYPos, m_ImageWinFlags.GetWidth()/3 , 
			m_ImageWinFlags.GetHeight(),m_ImageWinFlags.GetWidth()/3 * nFlagsIndex, 0, RGB(255, 0, 255) );

		strPoint.Format(TEXT("%d"), ClientGameRecord.cbBankerCount);
		pDC->TextOut(m_nWinPointsExcursionX + nDrawCount * 17, m_nWinPointsExcursionY, strPoint);

		strPoint.Format(TEXT("%d"), ClientGameRecord.cbPlayerCount);
		pDC->TextOut(m_nWinPointsExcursionX + nDrawCount * 17, m_nWinPointsExcursionY + 21, strPoint);

		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
		nDrawCount++;
	}
	pDC->SetTextColor(clrOld);

	//统计个数
	nIndex = m_nRecordFirst;
	int nPlayerCount = 0, nBankerCount = 0, nTieCount = 0;
	while ( nIndex != m_nRecordLast && ( m_nRecordLast!=m_nRecordFirst ))
	{
		tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[nIndex];
		if ( ClientGameRecord.cbBankerCount < ClientGameRecord.cbPlayerCount ) nPlayerCount++;
		else if ( ClientGameRecord.cbBankerCount == ClientGameRecord.cbPlayerCount ) nTieCount++;
		else nBankerCount++;

		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
	}

	int nWidth, nHeight;
	CRect rcClient;
	GetClientRect(&rcClient);
	nWidth = rcClient.Width();
	nHeight = rcClient.Height();
	CFont font;
	VERIFY(font.CreateFont(25,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,"Arial"));                

	pDC->SetTextColor(RGB(255,255,255));
	CFont* def_font = pDC->SelectObject(&font);
	strPoint.Format(TEXT("%d"), nBankerCount);
	pDC->TextOut(nWidth/2-350+5*120+78, nHeight/2+257, strPoint);
	strPoint.Format(TEXT("%d"), nPlayerCount);
	pDC->TextOut(nWidth/2-350+5*120+78, nHeight/2+288, strPoint);
	strPoint.Format(TEXT("%d"), nTieCount);
	pDC->TextOut(nWidth/2-350+5*120+78, nHeight/2+320, strPoint);
	pDC->SelectObject(def_font);
	font.DeleteObject();
}

//移动按钮
void CGameClientView::OnScoreMoveL()
{
	if ( m_nRecordFirst == m_nScoreHead ) return;

	m_nScoreHead = (m_nScoreHead - 1 + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
	if ( m_nScoreHead == m_nRecordFirst ) m_btScoreMoveL.EnableWindow(FALSE);

	m_btScoreMoveR.EnableWindow(TRUE);

	//更新界面
	UpdateGameView(NULL);
}

//移动按钮
void CGameClientView::OnScoreMoveR()
{
	int nHistoryCount = ( m_nRecordLast - m_nScoreHead + MAX_SCORE_HISTORY ) % MAX_SCORE_HISTORY;
	if ( nHistoryCount == 12 ) return;

	m_nScoreHead = ( m_nScoreHead + 1 ) % MAX_SCORE_HISTORY;
	if ( nHistoryCount-1 == 12 ) m_btScoreMoveR.EnableWindow(FALSE);

	m_btScoreMoveL.EnableWindow(TRUE);

	//更新界面
	UpdateGameView(NULL);
}

//显示结果
void CGameClientView::ShowGameResult(CDC *pDC, int nWidth, int nHeight)
{
	//显示判断
	if (m_pGameClientDlg->GetGameStatus()!=GS_GAME_END || m_cbSendCount[INDEX_PLAYER]+m_cbSendCount[INDEX_BANKER]!=0 ||
		m_ptDispatchCard.x!=0 || m_ptDispatchCard.y!=0) return;

	int	nXPos = nWidth / 2 - 129;
	int	nYPos = nHeight / 2 - 208;
	CImageHandle ImageHandleGameEnd(&m_ImageGameEnd);
	CImageHandle ImageHandleGameEndFrame(&m_ImageGameEndFrame);	
	CImageHandle ImageHandleGamePoint(&m_ImageGamePoint);

	//绘画边框
	m_ImageGameEndFrame.AlphaDrawImage(pDC, nXPos, nYPos, RGB(255,0,255));

	CRect rcAlpha(nXPos+2, nYPos+70, nXPos+2 + m_ImageGameEnd.GetWidth(), nYPos+70+m_ImageGameEnd.GetHeight());
	DrawAlphaRect(pDC, &rcAlpha, RGB(74,70,73), 0.8f);
	m_ImageGameEnd.AlphaDrawImage(pDC, nXPos+2, nYPos+70, RGB(255,0,255));

	//计算点数
	BYTE cbPlayerPoint, cbBankerPoint;
	cbPlayerPoint = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER], m_cbCardCount[INDEX_PLAYER]);

	//计算点数
	cbBankerPoint = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER], m_cbCardCount[INDEX_BANKER]);

	//绘画点数
	int nPointWidth = m_ImageGamePoint.GetWidth() / 10;
	m_ImageGamePoint.AlphaDrawImage(pDC, nXPos + 28, nYPos + 10, nPointWidth, m_ImageGamePoint.GetHeight(),
		cbPlayerPoint * nPointWidth, 0, RGB(255, 0, 255));
	m_ImageGamePoint.AlphaDrawImage(pDC, nXPos + 28 + 136, nYPos + 10, nPointWidth, m_ImageGamePoint.GetHeight(),
		cbBankerPoint * nPointWidth, 0, RGB(255, 0, 255));

	pDC->SetTextColor(RGB(255,234,0));
	CRect rcMeWinScore, rcMeReturnScore;
	rcMeWinScore.left = nXPos+2 + 40;
	rcMeWinScore.top = nYPos+70 + 32;
	rcMeWinScore.right = rcMeWinScore.left + 111;
	rcMeWinScore.bottom = rcMeWinScore.top + 34;

	rcMeReturnScore.left = nXPos+2 + 150;
	rcMeReturnScore.top = nYPos+70 + 32;
	rcMeReturnScore.right = rcMeReturnScore.left + 111;
	rcMeReturnScore.bottom = rcMeReturnScore.top + 34;

	CString strMeGameScore, strMeReturnScore;
	DrawNumberStringWithSpace(pDC,m_lMeCurGameScore,rcMeWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	DrawNumberStringWithSpace(pDC,m_lMeCurGameReturnScore,rcMeReturnScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	CRect rcBankerWinScore;
	rcBankerWinScore.left = nXPos+2 + 40;
	rcBankerWinScore.top = nYPos+70 + 69;
	rcBankerWinScore.right = rcBankerWinScore.left + 111;
	rcBankerWinScore.bottom = rcBankerWinScore.top + 34;

	CString strBankerCurGameScore;
	strBankerCurGameScore.Format(TEXT("%I64d"), m_lBankerCurGameScore);
	DrawNumberStringWithSpace(pDC,m_lBankerCurGameScore,rcBankerWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
}

//透明绘画
bool CGameClientView::DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha)
{
	ASSERT(pDC != 0 && lpRect != 0 && clr != CLR_NONE);
	if(pDC == 0 || lpRect == 0 || clr == CLR_NONE)
	{
		return false;
	}
	//全透明
	if( abs(fAlpha) <= 0.000001 )
	{
		return true;
	}

	for(LONG l=lpRect->top; l<lpRect->bottom; l++)
	{
		for(LONG k=lpRect->left; k<lpRect->right; k++)
		{
			COLORREF clrBk = pDC->GetPixel(k, l);
			COLORREF clrBlend = RGB(GetRValue(clrBk)*(1-fAlpha)+GetRValue(clr)*fAlpha, 
								GetGValue(clrBk)*(1-fAlpha)+GetGValue(clr)*fAlpha,
								GetBValue(clrBk)*(1-fAlpha)+GetBValue(clr)*fAlpha);
			pDC->SetPixel(k, l, clrBlend);
		}
	}

	return true;
}

//最大下注
LONGLONG CGameClientView::GetMaxPlayerScore()
{	
	LONGLONG lMaxPlayerScore = 0L;

	//庄家判断
	if (m_wBankerUser!=INVALID_CHAIR) 
	{
		//其他区域
		LONGLONG lOtherAreaScore = m_lAllTieScore + m_lAllTieSamePointScore + m_lAllBankerScore + m_lAllBankerKingScore ;

		//最大下注
		lMaxPlayerScore = lOtherAreaScore + m_lBankerScore;
		lMaxPlayerScore -= (m_lAllPlayerScore + m_lAllPlayerKingScore*2 + (m_lAllPlayerTwoPair+m_lAllBankerTwoPair)*RATE_TWO_PAIR);
		lMaxPlayerScore = min(lMaxPlayerScore, (m_lAreaLimitScore-m_lAllPlayerScore));
	}
	else
	{
		lMaxPlayerScore=m_lAreaLimitScore-m_lAllPlayerScore;
	}

	return lMaxPlayerScore;
}

//最大下注
LONGLONG CGameClientView::GetMaxPlayerKingScore()
{
	LONGLONG lMaxPlayerScore = 0L;
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//其他区域
		LONGLONG lOtherAreaScore = m_lAllTieScore + m_lAllTieSamePointScore + m_lAllBankerScore + m_lAllBankerKingScore;

		//最大下注
		lMaxPlayerScore = lOtherAreaScore + m_lBankerScore;
		lMaxPlayerScore -= (m_lAllPlayerScore + m_lAllPlayerKingScore*2+ (m_lAllPlayerTwoPair+m_lAllBankerTwoPair)*RATE_TWO_PAIR);
		lMaxPlayerScore = min(lMaxPlayerScore/2, (m_lAreaLimitScore-m_lAllPlayerKingScore));
	}
	else
	{
		lMaxPlayerScore=m_lAreaLimitScore-m_lAllPlayerKingScore;
	}

	return lMaxPlayerScore;
}

//最大下注
LONGLONG CGameClientView::GetMaxPlayerTwoPair()
{
	LONGLONG lMaxPlayerScore = 0L;
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//区域下注
		LONGLONG lPlayerJettonCount=m_lAllPlayerKingScore+m_lAllPlayerScore;
		LONGLONG lBankerJettonCount=m_lAllBankerKingScore+m_lAllBankerScore;
		LONGLONG lTieJettonCount=m_lAllTieSamePointScore+m_lAllTieScore;

		//区域赔额
		LONGLONG lPlayerCompensateCount=m_lAllPlayerKingScore*2+m_lAllPlayerScore;
		LONGLONG lBankerCompensateCount=m_lAllBankerKingScore*2+m_lAllBankerScore;
		LONGLONG lTieCompensateCount=m_lAllTieSamePointScore*33+m_lAllTieScore*8;

		//庄家输额
		LONGLONG lTieWinCount=lTieCompensateCount;
		LONGLONG lPlayerWinCount=lPlayerCompensateCount-(lBankerJettonCount+lTieJettonCount);
		LONGLONG lBankerWinCount=lBankerCompensateCount-(lPlayerJettonCount+lTieJettonCount);

		//最大输额
		LONGLONG lOtherAreaScore = max(lTieWinCount,max(lPlayerWinCount,lBankerWinCount));

		//最大下注
		lMaxPlayerScore = m_lBankerScore-lOtherAreaScore;
		lMaxPlayerScore -= ((m_lAllPlayerTwoPair+m_lAllBankerTwoPair)*RATE_TWO_PAIR);
		lMaxPlayerScore = min(lMaxPlayerScore/RATE_TWO_PAIR, (m_lAreaLimitScore-m_lAllPlayerTwoPair));
	}
	else
	{
		lMaxPlayerScore=m_lAreaLimitScore-m_lAllPlayerTwoPair;
	}

	return lMaxPlayerScore;
}
//最大下注
LONGLONG CGameClientView::GetMaxBankerTwoPair()
{
	LONGLONG lMaxPlayerScore = 0L;
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//区域下注
		LONGLONG lPlayerJettonCount=m_lAllPlayerKingScore+m_lAllPlayerScore;
		LONGLONG lBankerJettonCount=m_lAllBankerKingScore+m_lAllBankerScore;
		LONGLONG lTieJettonCount=m_lAllTieSamePointScore+m_lAllTieScore;

		//区域赔额
		LONGLONG lPlayerCompensateCount=m_lAllPlayerKingScore*2+m_lAllPlayerScore;
		LONGLONG lBankerCompensateCount=m_lAllBankerKingScore*2+m_lAllBankerScore;
		LONGLONG lTieCompensateCount=m_lAllTieSamePointScore*33+m_lAllTieScore*8;

		//庄家输额
		LONGLONG lTieWinCount=lTieCompensateCount;
		LONGLONG lPlayerWinCount=lPlayerCompensateCount-(lBankerJettonCount+lTieJettonCount);
		LONGLONG lBankerWinCount=lBankerCompensateCount-(lPlayerJettonCount+lTieJettonCount);

		//最大输额
		LONGLONG lOtherAreaScore = max(lTieWinCount,max(lPlayerWinCount,lBankerWinCount));

		//最大下注
		lMaxPlayerScore = m_lBankerScore-lOtherAreaScore;
		lMaxPlayerScore -= ((m_lAllPlayerTwoPair+m_lAllBankerTwoPair)*RATE_TWO_PAIR);
		lMaxPlayerScore = min(lMaxPlayerScore/RATE_TWO_PAIR, (m_lAreaLimitScore-m_lAllPlayerTwoPair));
	}
	else
	{
		lMaxPlayerScore=m_lAreaLimitScore-m_lAllBankerTwoPair;
	}

	return lMaxPlayerScore;
}

//最大下注
LONGLONG CGameClientView::GetMaxBankerScore()
{
	LONGLONG lMaxPlayerScore = 0L;
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//其他区域
		LONGLONG lOtherAreaScore = m_lAllTieScore + m_lAllPlayerScore + m_lAllTieSamePointScore + m_lAllPlayerKingScore;

		//最大下注
		lMaxPlayerScore = lOtherAreaScore + m_lBankerScore;
		lMaxPlayerScore -= (m_lAllBankerScore + m_lAllBankerKingScore*2 + (m_lAllPlayerTwoPair+m_lAllBankerTwoPair)*RATE_TWO_PAIR);
		lMaxPlayerScore = min(lMaxPlayerScore, (m_lAreaLimitScore-m_lAllBankerScore));
	}
	else
	{
		lMaxPlayerScore = m_lAreaLimitScore-m_lAllBankerScore;
	}

	return lMaxPlayerScore;
}

//最大下注
LONGLONG CGameClientView::GetMaxBankerKingScore()
{
	LONGLONG lMaxPlayerScore = 0L;
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//其他区域
		LONGLONG lOtherAreaScore = m_lAllTieScore + m_lAllPlayerScore + m_lAllTieSamePointScore + m_lAllPlayerKingScore;

		//最大下注
		lMaxPlayerScore = lOtherAreaScore + m_lBankerScore;
		lMaxPlayerScore -= (m_lAllBankerScore + m_lAllBankerKingScore*2+(m_lAllPlayerTwoPair+m_lAllBankerTwoPair)*RATE_TWO_PAIR);
		lMaxPlayerScore = min(lMaxPlayerScore/2, (m_lAreaLimitScore-m_lAllBankerKingScore));
	}
	else
	{
		lMaxPlayerScore=m_lAreaLimitScore-m_lAllBankerKingScore;
	}

	return lMaxPlayerScore;
}

//最大下注
LONGLONG CGameClientView::GetMaxTieScore()
{
	LONGLONG lMaxTieScore = 0L;
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//返回积分
		LONGLONG lReturnScore = m_lAllTieSamePointScore * 33 + m_lAllTieScore * 8+(m_lAllPlayerTwoPair+m_lAllBankerTwoPair)*RATE_TWO_PAIR;

		//可下积分
		lMaxTieScore = min((m_lBankerScore - lReturnScore)/8, (m_lAreaLimitScore-m_lAllTieScore));
	}
	else
	{
		lMaxTieScore = m_lAreaLimitScore-m_lAllTieScore;
	}

	return lMaxTieScore;
}

//最大下注
LONGLONG CGameClientView::GetMaxTieKingScore()
{
	LONGLONG lMaxTieScore = 0L;
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//返回积分
		LONGLONG lReturnScore = m_lAllTieSamePointScore * 33 + m_lAllTieScore * 8+(m_lAllPlayerTwoPair+m_lAllBankerTwoPair)*RATE_TWO_PAIR;

		//可下积分
		lMaxTieScore = min(( m_lBankerScore - lReturnScore )/33, (m_lAreaLimitScore-m_lAllTieSamePointScore)) ;
	}
	else
	{
		lMaxTieScore = m_lAreaLimitScore-m_lAllTieSamePointScore;
	}

	return lMaxTieScore;
}

//获取所有下注
LONGLONG CGameClientView::GetTotalChip()
{
	//计算总和
	LONGLONG lTotalChip = m_lAllTieScore + m_lAllBankerScore + m_lAllPlayerScore + m_lAllTieSamePointScore + 
		m_lAllBankerKingScore + m_lAllPlayerKingScore + m_lAllPlayerTwoPair + m_lAllBankerTwoPair;

	return lTotalChip;
}

//成绩设置
void CGameClientView::SetGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore)
{
	m_lMeCurGameScore=lMeCurGameScore;
	m_lMeCurGameReturnScore=lMeCurGameReturnScore;
	m_lBankerCurGameScore=lBankerCurGameScore;	
}

//绘画数字
void CGameClientView::DrawMeJettonNumber(CDC *pDC)
{
	//绘画数字
	if (0 < m_lMePlayerScore) DrawNumberString(pDC,m_lMePlayerScore,m_PointJettonNumber[ID_XIAN_JIA-1].x,m_PointJettonNumber[ID_XIAN_JIA-1].y+25, true);
	if (0 < m_lMeTieScore) DrawNumberString(pDC,m_lMeTieScore,m_PointJettonNumber[ID_PING_JIA-1].x,m_PointJettonNumber[ID_PING_JIA-1].y+25, true);
	if (0 < m_lMeBankerScore) DrawNumberString(pDC,m_lMeBankerScore,m_PointJettonNumber[ID_ZHUANG_JIA-1].x,m_PointJettonNumber[ID_ZHUANG_JIA-1].y+25, true);
	if (0 < m_lMePlayerKingScore) DrawNumberString(pDC,m_lMePlayerKingScore,m_PointJettonNumber[ID_XIAN_TIAN_WANG-1].x,m_PointJettonNumber[ID_XIAN_TIAN_WANG-1].y+25, true);
	if (0 < m_lMeBankerKingScore) DrawNumberString(pDC,m_lMeBankerKingScore,m_PointJettonNumber[ID_ZHUANG_TIAN_WANG-1].x,m_PointJettonNumber[ID_ZHUANG_TIAN_WANG-1].y+25, true);
	if (0 < m_lMeTieSamePointScore) DrawNumberString(pDC,m_lMeTieSamePointScore,m_PointJettonNumber[ID_TONG_DIAN_PING-1].x,m_PointJettonNumber[ID_TONG_DIAN_PING-1].y+25, true);
	if (0 < m_lMeBankerTwoPair) DrawNumberString(pDC,m_lMeBankerTwoPair,m_PointJettonNumber[ID_BANKER_TWO_PAIR-1].x,m_PointJettonNumber[ID_BANKER_TWO_PAIR-1].y+25, true);
	if (0 < m_lMePlayerTwoPair) DrawNumberString(pDC,m_lMePlayerTwoPair,m_PointJettonNumber[ID_PLAYER_TWO_PAIR-1].x,m_PointJettonNumber[ID_PLAYER_TWO_PAIR-1].y+25, true);
}

//开始发牌
void CGameClientView::DispatchCard()
{
	//设置变量
	m_cbSendCount[INDEX_PLAYER]=0;
	m_cbSendCount[INDEX_BANKER]=0;

	//设置界面
	m_CardControl[INDEX_PLAYER].SetCardData(NULL,0);
	m_CardControl[INDEX_BANKER].SetCardData(NULL,0);

	//发牌扑克
	BYTE cbCardData=0;
	m_DispatchCard.SetCardData(&cbCardData,1);

	//设置位置
	CRect rcView;
	GetClientRect(&rcView);
	m_ptDispatchCard.x = rcView.Width()/2+220;
	m_ptDispatchCard.y = rcView.Height()/2-200;

	//设置定时器
	SetTimer(IDI_DISPATCH_CARD,2,NULL);

	//设置标识
	m_bNeedSetGameRecord=true;
}

//结束发牌
void CGameClientView::FinishDispatchCard(bool bScene)
{
	//完成判断
	if (m_bNeedSetGameRecord==false) return;

	//设置标识
	m_bNeedSetGameRecord=false;

	//删除定时器
	KillTimer(IDI_DISPATCH_CARD);

	//设置扑克
	m_CardControl[INDEX_PLAYER].SetCardData(m_cbTableCardArray[0],m_cbCardCount[0]);
	m_CardControl[INDEX_BANKER].SetCardData(m_cbTableCardArray[1],m_cbCardCount[1]);
	m_DispatchCard.SetCardData(NULL,0);

	//清楚变量
	ZeroMemory(m_cbSendCount,sizeof(m_cbSendCount));

	//设置记录
	//if (m_pGameClientDlg->GetGameStatus()==GS_GAME_END)
	{
		//扑克点数
		BYTE cbPlayerPoint=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER],m_cbCardCount[INDEX_PLAYER]);
		BYTE cbBankerPoint=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER],m_cbCardCount[INDEX_BANKER]);

		//操作类型
		enOperateResult OperateResult = enOperateResult_NULL;
		if (0 < m_lMeCurGameScore) OperateResult = enOperateResult_Win;
		else if (m_lMeCurGameScore < 0) OperateResult = enOperateResult_Lost;
		else OperateResult = enOperateResult_NULL;

		//推断赢家
		BYTE cbWinner,cbKingWinner;
		bool bPlayerTwoPair,bBankerTwoPair;
		DeduceWinner(cbWinner, cbKingWinner, bPlayerTwoPair,bBankerTwoPair);

		//保存记录
		if(!bScene)
			SetGameHistory(OperateResult, cbPlayerPoint, cbBankerPoint, cbKingWinner, bPlayerTwoPair,bBankerTwoPair);

		//累计积分
		m_lMeStatisticScore+=m_lMeCurGameScore;
		m_lBankerWinScore=m_lTmpBankerWinScore;

		//设置赢家
		SetWinnerSide(cbWinner);

		//播放声音
		if (m_lMeCurGameScore>0) m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_WIN"));
		else if (m_lMeCurGameScore<0) m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_LOST"));
		else m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_DRAW"));
	}
}

//胜利边框
void CGameClientView::FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC)
{
	if (m_cbAreaFlash==0xFF && false==m_bFlashResult) return;

	//加载资源
	CImageHandle HandleWinFlags(&m_ImageWinFlags);
	CSize SizeWinFlags(m_ImageWinFlags.GetWidth()/3,m_ImageWinFlags.GetHeight());

	//位置变量
	int nXPos = 0;
	int nYPos = 0;
	if (false==m_bFlashResult)
	{
		//闲家标志
		switch (m_cbAreaFlash)
		{
		case ID_XIAN_JIA:	
			{
				CImageHandle ImageHandleXianJia(&m_ImageFrameXianJia);
				m_ImageFrameXianJia.AlphaDrawImage(pDC,m_rcPlayer.left, m_rcPlayer.top, RGB(255,0,255));
				break;
			}
		case ID_XIAN_TIAN_WANG:
			{
				CImageHandle ImageHandleXianTianWang(&m_ImageFrameXianTianWang);
				m_ImageFrameXianTianWang.AlphaDrawImage(pDC,m_rcPlayerKing.left, m_rcPlayerKing.top, RGB(255,0,255));
				break;
			}
		case ID_PLAYER_TWO_PAIR:
			{		
				CImageHandle ImageHandlePlayerTwoPair(&m_ImageFramePlayerTwoPair);
				m_ImageFramePlayerTwoPair.AlphaDrawImage(pDC,m_rcPlayerTwoPair.left, m_rcPlayerTwoPair.top, RGB(255,0,255));
				break;
			}
		case ID_PING_JIA:
			{
				CImageHandle ImageHandlePingJia(&m_ImageFramePingJia);			
				m_ImageFramePingJia.AlphaDrawImage(pDC,m_rcTie.left, m_rcTie.top, RGB(255,0,255));
				break;
			}
		case ID_TONG_DIAN_PING:
			{
				CImageHandle ImageHandleTongDianPing(&m_ImageFrameTongDianPing);			
				m_ImageFrameTongDianPing.AlphaDrawImage(pDC,m_rcTieKing.left, m_rcTieKing.top, RGB(255,0,255));
				break;
			}
		case ID_ZHUANG_JIA:
			{
				CImageHandle ImageHandleZhuangJia(&m_ImageFrameZhuangJia);
				m_ImageFrameZhuangJia.AlphaDrawImage(pDC,m_rcBanker.left, m_rcBanker.top, RGB(255,0,255));
				break;
			}
		case ID_ZHUANG_TIAN_WANG:
			{
				CImageHandle ImageHandleZhuangTianWang(&m_ImageFrameZhuangTianWang);
				m_ImageFrameZhuangTianWang.AlphaDrawImage(pDC,m_rcBankerKing.left, m_rcBankerKing.top, RGB(255,0,255));
				break;
			}
		case ID_BANKER_TWO_PAIR:
			{
				CImageHandle ImageHandleBankerTwoPair(&m_ImageFrameBankerTwoPair);
				m_ImageFrameBankerTwoPair.AlphaDrawImage(pDC,m_rcBankerTwoPair.left, m_rcBankerTwoPair.top, RGB(255,0,255));
				break;
			}
		}

	}else
	{

		//结束判断
		if (m_pGameClientDlg->GetGameStatus()==GS_GAME_END && m_cbSendCount[0]+m_cbSendCount[0]==0)
		{
			//推断赢家
			BYTE cbWinner,cbKingWinner;
			bool bPlayerTwoPair,bBankerTwoPair;
			DeduceWinner(cbWinner, cbKingWinner, bPlayerTwoPair,bBankerTwoPair);

		//闲天王
		if (cbKingWinner==ID_XIAN_TIAN_WANG)
		{
			CImageHandle ImageHandleXianTianWang(&m_ImageFrameXianTianWang);
			m_ImageFrameXianTianWang.AlphaDrawImage(pDC,m_rcPlayerKing.left, m_rcPlayerKing.top, RGB(255,0,255));
		}
		else if (cbKingWinner==ID_ZHUANG_TIAN_WANG)
		{
			CImageHandle ImageHandleZhuangTianWang(&m_ImageFrameZhuangTianWang);
			m_ImageFrameZhuangTianWang.AlphaDrawImage(pDC,m_rcBankerKing.left, m_rcBankerKing.top, RGB(255,0,255));
		}
		else if (cbKingWinner==ID_TONG_DIAN_PING)
		{
			CImageHandle ImageHandleTongDianPing(&m_ImageFrameTongDianPing);
			m_ImageFrameTongDianPing.AlphaDrawImage(pDC,m_rcTieKing.left, m_rcTieKing.top, RGB(255,0,255));
		}

			//对子		
			if (bBankerTwoPair)
			{
				CImageHandle ImageHandleBankerTwoPair(&m_ImageFrameBankerTwoPair);
				m_ImageFrameBankerTwoPair.AlphaDrawImage(pDC,m_rcBankerTwoPair.left, m_rcBankerTwoPair.top, RGB(255,0,255));
			}
			if (bPlayerTwoPair)
			{
				CImageHandle ImageHandlePlayerTwoPair(&m_ImageFramePlayerTwoPair);
				m_ImageFramePlayerTwoPair.AlphaDrawImage(pDC,m_rcPlayerTwoPair.left, m_rcPlayerTwoPair.top, RGB(255,0,255));
			}

			//闲家标志
			switch (m_cbAreaFlash)
			{
			case ID_XIAN_JIA:	
				{
					CImageHandle ImageHandleXianJia(&m_ImageFrameXianJia);
					m_ImageFrameXianJia.AlphaDrawImage(pDC,m_rcPlayer.left, m_rcPlayer.top, RGB(255,0,255));
					break;
				}
			case ID_XIAN_TIAN_WANG:
				{
					CImageHandle ImageHandleXianTianWang(&m_ImageFrameXianTianWang);
					m_ImageFrameXianTianWang.AlphaDrawImage(pDC,m_rcPlayerKing.left, m_rcPlayerKing.top, RGB(255,0,255));
					break;
				}
			case ID_PLAYER_TWO_PAIR:
				{		
					CImageHandle ImageHandlePlayerTwoPair(&m_ImageFramePlayerTwoPair);
					m_ImageFramePlayerTwoPair.AlphaDrawImage(pDC,m_rcPlayerTwoPair.left, m_rcPlayerTwoPair.top, RGB(255,0,255));
					break;
				}
			case ID_PING_JIA:
				{
					CImageHandle ImageHandlePingJia(&m_ImageFramePingJia);			
					m_ImageFramePingJia.AlphaDrawImage(pDC,m_rcTie.left, m_rcTie.top, RGB(255,0,255));
					break;
				}
			case ID_TONG_DIAN_PING:
				{
					CImageHandle ImageHandleTongDianPing(&m_ImageFrameTongDianPing);			
					m_ImageFrameTongDianPing.AlphaDrawImage(pDC,m_rcTieKing.left, m_rcTieKing.top, RGB(255,0,255));
					break;
				}
			case ID_ZHUANG_JIA:
				{
					CImageHandle ImageHandleZhuangJia(&m_ImageFrameZhuangJia);
					m_ImageFrameZhuangJia.AlphaDrawImage(pDC,m_rcBanker.left, m_rcBanker.top, RGB(255,0,255));
					break;
				}
			case ID_ZHUANG_TIAN_WANG:
				{
					CImageHandle ImageHandleZhuangTianWang(&m_ImageFrameZhuangTianWang);
					m_ImageFrameZhuangTianWang.AlphaDrawImage(pDC,m_rcBankerKing.left, m_rcBankerKing.top, RGB(255,0,255));
					break;
				}
			case ID_BANKER_TWO_PAIR:
				{
					CImageHandle ImageHandleBankerTwoPair(&m_ImageFrameBankerTwoPair);
					m_ImageFrameBankerTwoPair.AlphaDrawImage(pDC,m_rcBankerTwoPair.left, m_rcBankerTwoPair.top, RGB(255,0,255));
					break;
				}
			}
		
		}

	}
}


//推断赢家
void CGameClientView::DeduceWinner(BYTE &cbWinner, BYTE &cbKingWinner, bool &bPlayerTwoPair, bool &bBankerTwoPair)
{
	cbWinner = 0;
	cbKingWinner = 0;

	//计算牌点
	BYTE cbPlayerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER],m_cbCardCount[INDEX_PLAYER]);
	BYTE cbBankerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER],m_cbCardCount[INDEX_BANKER]);

	//胜利玩家
	if (cbPlayerCount==cbBankerCount)
	{
		cbWinner=ID_PING_JIA;

		//同点平判断
		bool bAllPointSame = false;
		if ( m_cbCardCount[INDEX_PLAYER] == m_cbCardCount[INDEX_BANKER] )
		{
			for (WORD wCardIndex = 0; wCardIndex < m_cbCardCount[INDEX_PLAYER]; ++wCardIndex )
			{
				BYTE cbBankerValue = m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_BANKER][wCardIndex]);
				BYTE cbPlayerValue = m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_PLAYER][wCardIndex]);
				if ( cbBankerValue != cbPlayerValue ) break;
			}
			if ( wCardIndex == m_cbCardCount[INDEX_PLAYER] ) bAllPointSame = true;
		}
		if ( bAllPointSame ) cbKingWinner = ID_TONG_DIAN_PING;
	}
	else if (cbPlayerCount<cbBankerCount) 
	{
		cbWinner=ID_ZHUANG_JIA;

		//天王判断
		if ( cbBankerCount == 8 || cbBankerCount == 9 ) cbKingWinner = ID_ZHUANG_TIAN_WANG;
	}
	else 
	{
		cbWinner=ID_XIAN_JIA;

		//天王判断
		if ( cbPlayerCount == 8 || cbPlayerCount == 9 ) cbKingWinner = ID_XIAN_TIAN_WANG;
	}

	//对子判断
	bPlayerTwoPair=false;
	bBankerTwoPair=false;
	if (m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_PLAYER][0])==m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_PLAYER][1]))
		bPlayerTwoPair=true;
	if (m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_BANKER][0])==m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_BANKER][1]))
		bBankerTwoPair=true;
}

//发牌提示
void CGameClientView::SetDispatchCardTips()
{
	if (m_cbSendCount[INDEX_PLAYER]+m_cbSendCount[INDEX_BANKER] < 4) return;

	//计算点数
	BYTE cbBankerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER],2);
	BYTE cbPlayerTwoCardCount = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER],2);

	//闲家补牌	
	if(cbPlayerTwoCardCount<=5 && cbBankerCount<8 && m_cbSendCount[INDEX_PLAYER]+m_cbSendCount[INDEX_BANKER]==4)
	{		
		CString strTips;
		strTips.Format(TEXT("闲%d点，庄%d点，闲继续拿牌\n"), cbPlayerTwoCardCount, cbBankerCount);
		m_strDispatchCardTips = strTips;

		return;
	}

	if (m_cbSendCount[INDEX_BANKER]==3) return;

	BYTE cbPlayerThirdCardValue = 0 ;	//第三张牌点数

	//计算点数
	cbPlayerThirdCardValue = m_GameLogic.GetCardPip(m_cbTableCardArray[INDEX_PLAYER][2]);

	//庄家补牌
	CString strTips;
	if(cbPlayerTwoCardCount<8 && cbBankerCount<8)
	{
		switch(cbBankerCount)
		{
		case 0:
		case 1:
		case 2:
			strTips.Format(TEXT("闲前两张牌%d点，庄%d点，庄继续拿牌\n"), cbPlayerTwoCardCount, cbBankerCount);
			m_strDispatchCardTips += strTips;
			break;

		case 3:
			if(m_cbCardCount[INDEX_PLAYER]==3 && cbPlayerThirdCardValue!=8)
			{
				strTips.Format(TEXT("闲第三张牌%d点，庄%d点，庄继续拿牌\n"), cbPlayerThirdCardValue, cbBankerCount);
				m_strDispatchCardTips += strTips;
			}
			else if (m_cbCardCount[INDEX_PLAYER]==2) 
			{
				strTips.Format(TEXT("闲不补牌，庄%d点，庄继续拿牌\n"), cbBankerCount);
				m_strDispatchCardTips += strTips;
			}			
			break;

		case 4:
			if(m_cbCardCount[INDEX_PLAYER]==3 && cbPlayerThirdCardValue!=1 && cbPlayerThirdCardValue!=8 && cbPlayerThirdCardValue!=9 && cbPlayerThirdCardValue!=0)
			{
				strTips.Format(TEXT("闲第三张牌%d点，庄%d点，庄继续拿牌\n"), cbPlayerThirdCardValue, cbBankerCount);
				m_strDispatchCardTips += strTips;
			}
			else if ( m_cbCardCount[INDEX_PLAYER]==2) 
			{
				strTips.Format(TEXT("闲不补牌，庄%d点，庄继续拿牌\n"), cbBankerCount);
				m_strDispatchCardTips += strTips;
			}
			break;

		case 5:
			if(m_cbCardCount[INDEX_PLAYER]==3 && cbPlayerThirdCardValue!=1 && cbPlayerThirdCardValue!=2 && cbPlayerThirdCardValue!=3  && cbPlayerThirdCardValue!=8 && cbPlayerThirdCardValue!=9 &&  cbPlayerThirdCardValue!=0)
			{
				strTips.Format(TEXT("闲第三张牌%d点，庄%d点，庄继续拿牌\n"), cbPlayerThirdCardValue, cbBankerCount);
				m_strDispatchCardTips += strTips;
			}
			else if ( m_cbCardCount[INDEX_PLAYER]==2) 
			{
				strTips.Format(TEXT("闲不补牌，庄%d点，庄继续拿牌\n"), cbBankerCount);
				m_strDispatchCardTips += strTips;
			}
			break;

		case 6:
			if(m_cbCardCount[INDEX_PLAYER]==3 && (cbPlayerThirdCardValue==6 || cbPlayerThirdCardValue==7))
			{
				strTips.Format(TEXT("闲第三张牌%d点，庄%d点，庄继续拿牌\n"), cbPlayerThirdCardValue, cbBankerCount);
				m_strDispatchCardTips += strTips;
			}
			break;

			//不须补牌
		case 7:
		case 8:
		case 9:
			break;
		default:
			break;
		}
	}

	return ;
}

//设置爆炸
bool CGameClientView::SetBombEffect(bool bBombEffect, WORD wAreaIndex)
{
	if (bBombEffect==true)
	{
		//设置变量
		m_bBombEffect[wAreaIndex]=true;
		m_cbBombFrameIndex[wAreaIndex]=0;

		//启动时间
		SetTimer(IDI_BOMB_EFFECT+wAreaIndex,100,NULL);
	}
	else
	{
		//停止动画
		if (m_bBombEffect[wAreaIndex]==true)
		{
			//删除时间
			KillTimer(IDI_BOMB_EFFECT+wAreaIndex);

			//设置变量
			m_bBombEffect[wAreaIndex]=false;
			m_cbBombFrameIndex[wAreaIndex]=0;

			//更新界面
			UpdateGameView(NULL);
		}
	}

	return true;
}

//绘画爆炸
void CGameClientView::DrawBombEffect(CDC *pDC)
{
	//绘画爆炸
	INT nImageHeight=m_ImageBombEffect.GetHeight();
	INT nImageWidth=m_ImageBombEffect.GetWidth()/BOMB_EFFECT_COUNT;
	if (m_bBombEffect[ID_XIAN_JIA]==true)
	{
		m_ImageBombEffect.DrawImage(pDC,m_PointJettonNumber[ID_XIAN_JIA-1].x-nImageWidth/2,m_PointJettonNumber[ID_XIAN_JIA-1].y,nImageWidth,nImageHeight,
			nImageWidth*(m_cbBombFrameIndex[ID_XIAN_JIA]%BOMB_EFFECT_COUNT),0);
	}
	if (m_bBombEffect[ID_PING_JIA]==true)
	{
		m_ImageBombEffect.DrawImage(pDC,m_PointJettonNumber[ID_PING_JIA-1].x-nImageWidth/2,m_PointJettonNumber[ID_PING_JIA-1].y,nImageWidth,nImageHeight,
			nImageWidth*(m_cbBombFrameIndex[ID_PING_JIA]%BOMB_EFFECT_COUNT),0);
	}
	if (m_bBombEffect[ID_ZHUANG_JIA]==true)
	{
		m_ImageBombEffect.DrawImage(pDC,m_PointJettonNumber[ID_ZHUANG_JIA-1].x-nImageWidth/2,m_PointJettonNumber[ID_ZHUANG_JIA-1].y,nImageWidth,nImageHeight,
			nImageWidth*(m_cbBombFrameIndex[ID_ZHUANG_JIA]%BOMB_EFFECT_COUNT),0);
	}
	if (m_bBombEffect[ID_XIAN_TIAN_WANG]==true)
	{
		m_ImageBombEffect.DrawImage(pDC,m_PointJettonNumber[ID_XIAN_TIAN_WANG-1].x-nImageWidth/2,m_PointJettonNumber[ID_XIAN_TIAN_WANG-1].y,nImageWidth,nImageHeight,
			nImageWidth*(m_cbBombFrameIndex[ID_XIAN_TIAN_WANG]%BOMB_EFFECT_COUNT),0);
	}
	if (m_bBombEffect[ID_ZHUANG_TIAN_WANG]==true)
	{
		m_ImageBombEffect.DrawImage(pDC,m_PointJettonNumber[ID_ZHUANG_TIAN_WANG-1].x-nImageWidth/2,m_PointJettonNumber[ID_ZHUANG_TIAN_WANG-1].y,nImageWidth,nImageHeight,
			nImageWidth*(m_cbBombFrameIndex[ID_ZHUANG_TIAN_WANG]%BOMB_EFFECT_COUNT),0);
	}
	if (m_bBombEffect[ID_TONG_DIAN_PING]==true)
	{
		m_ImageBombEffect.DrawImage(pDC,m_PointJettonNumber[ID_TONG_DIAN_PING-1].x-nImageWidth/2,m_PointJettonNumber[ID_TONG_DIAN_PING-1].y,nImageWidth,nImageHeight,
			nImageWidth*(m_cbBombFrameIndex[ID_TONG_DIAN_PING]%BOMB_EFFECT_COUNT),0);
	}
	if (m_bBombEffect[ID_PLAYER_TWO_PAIR]==true)
	{
		m_ImageBombEffect.DrawImage(pDC,m_PointJettonNumber[ID_PLAYER_TWO_PAIR-1].x-nImageWidth/2,m_PointJettonNumber[ID_PLAYER_TWO_PAIR-1].y,nImageWidth,nImageHeight,
			nImageWidth*(m_cbBombFrameIndex[ID_PLAYER_TWO_PAIR]%BOMB_EFFECT_COUNT),0);
	}
	if (m_bBombEffect[ID_BANKER_TWO_PAIR]==true)
	{
		m_ImageBombEffect.DrawImage(pDC,m_PointJettonNumber[ID_BANKER_TWO_PAIR-1].x-nImageWidth/2,m_PointJettonNumber[ID_BANKER_TWO_PAIR-1].y,nImageWidth,nImageHeight,
			nImageWidth*(m_cbBombFrameIndex[ID_BANKER_TWO_PAIR]%BOMB_EFFECT_COUNT),0);
	}

}
//////////////////////////////////////////////////////////////////////////
//银行存款
void CGameClientView::OnBankStorage()
{
#ifdef __SPECIAL___
	//获取接口
	CGameClientDlg *pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientDlg->GetClientKernel(IID_IClientKernel,VER_IClientKernel);

	if (NULL!=pIClientKernel)
	{
		if (NULL==m_DlgBank.m_hWnd) m_DlgBank.Create(IDD_BANK_STORAGE,this);

		if (!m_DlgBank.IsWindowVisible() || !m_DlgBank.m_bBankStorage)
		{
			m_DlgBank.SetSendInfo(pIClientKernel,pIClientKernel->GetMeUserInfo());
			m_DlgBank.SetBankerActionType(true);
			m_DlgBank.ShowWindow(SW_SHOW);

			CRect btRect;
			m_btBankerStorage.GetWindowRect(&btRect);
			m_DlgBank.SetPostPoint(CPoint(btRect.right,btRect.top));
		}
		else
			m_DlgBank.ShowWindow(SW_HIDE);

	}
#endif
}

//银行取款
void CGameClientView::OnBankDraw()
{
#ifdef __SPECIAL___
	//获取接口
	CGameClientDlg *pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientDlg->GetClientKernel(IID_IClientKernel,VER_IClientKernel);

	if (NULL!=pIClientKernel)
	{
		if (NULL==m_DlgBank.m_hWnd) m_DlgBank.Create(IDD_BANK_STORAGE,this);

		if (!m_DlgBank.IsWindowVisible() || m_DlgBank.m_bBankStorage)
		{
			m_DlgBank.SetSendInfo(pIClientKernel,pIClientKernel->GetMeUserInfo());
			m_DlgBank.SetBankerActionType(false);
			m_DlgBank.ShowWindow(SW_SHOW);

			CRect btRect;
			m_btBankerDraw.GetWindowRect(&btRect);
			m_DlgBank.SetPostPoint(CPoint(btRect.right,btRect.top));
		}
		else
			m_DlgBank.ShowWindow(SW_HIDE);
	}
#endif
}

//////////////////////////////////////////////////////////////////////////
void CGameClientView::OpenAdminWnd()
{
	//如果有此权限
	if((GetUserInfo(m_wMeChairID)->dwUserRight&UR_GAME_CONTROL)!=0)
	{
		if (NULL==m_AdminDlg.m_hWnd) m_AdminDlg.Create(IDD_DIALOG_ADMIN,this);

		if(!m_AdminDlg.IsWindowVisible()) m_AdminDlg.ShowWindow(SW_SHOW);
		else m_AdminDlg.ShowWindow(SW_HIDE);
	}
}

void CGameClientView::OnUp()
{
	m_ApplyUser.m_AppyUserList.SendMessage(WM_VSCROLL, MAKELONG(SB_LINEUP,0),NULL);
	m_ApplyUser.m_AppyUserList.Invalidate(TRUE);
	double nPos = m_ApplyUser.m_AppyUserList.GetScrollPos(SB_VERT);
	double nMax = m_ApplyUser.m_AppyUserList.GetScrollLimit(SB_VERT);

}

void CGameClientView::OnDown()
{
	double nPos = m_ApplyUser.m_AppyUserList.GetScrollPos(SB_VERT);
	if(nPos>m_ApplyUser.m_AppyUserList.GetItemCount()-3)
	{
		return ;
	}
	m_ApplyUser.m_AppyUserList.SendMessage(WM_VSCROLL, MAKELONG(SB_LINEDOWN,0),NULL);
	m_ApplyUser.m_AppyUserList.Invalidate(TRUE);
	double nMax = m_ApplyUser.m_AppyUserList.GetScrollLimit(SB_VERT);
}