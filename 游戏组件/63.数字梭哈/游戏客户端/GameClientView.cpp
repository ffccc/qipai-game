#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//按钮标识 
#define IDC_START						100								//开始按钮
#define IDC_ADD_GOLD					101								//加注按钮
#define IDC_GIVE_UP						102								//放弃按钮
#define IDC_FOLLOW						103								//跟注按钮
#define IDC_SHOW_HAND					104								//梭哈按钮

//定时器标识
#define IDI_SEND_CARD					107								//发牌定时器
#define IDI_SHOW_CARD					108								//
#define IDI_USER_SHOW_HAND				109								//

#define SPEED_MOVE_CARD					30								//

#define TIME_USER_SHOW_HAND				3000							//

//发牌定义
#define SEND_STEP_COUNT					6								//步数
#define SPEED_SEND_CARD					30								//发牌速度

#define MAX_COUNT						5

#define SHOW_CARD_ANIMATE												//设置看牌动画,如果要取消动画,则注释掉

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_FOLLOW, OnFollow)
	ON_BN_CLICKED(IDC_GIVE_UP, OnGiveUp)
	ON_BN_CLICKED(IDC_ADD_GOLD, OnAddGold)
	ON_BN_CLICKED(IDC_SHOW_HAND, OnShowHand)
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientView::CGameClientView(void) : CGameFrameView(true,24)
{
	//设置变量
	m_lMaxGold=0L;
	m_lBasicGold=0L;
	m_bUserShowHand = false;
	ZeroMemory( m_lUserGold,sizeof(m_lUserGold) );

	//动画变量
	m_SendCardPos.SetPoint(0,0);
	m_SendCardCurPos = m_SendCardPos;
	m_nStepCount = SEND_STEP_COUNT;
	m_nXStep = 0;
	m_nYStep = 0;

	//加载位图
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageTitle.SetLoadInfo(IDB_TITLE,hInstance);
	m_ImageViewFill.SetLoadInfo(IDB_VIEW_FILL,hInstance);
	m_ImageViewCenter.SetLoadInfo(IDB_BACK_GROUND,hInstance);
	m_ImageCardBack.SetLoadInfo(IDB_CARD,hInstance);
	m_PngShowHand.LoadImage( hInstance,TEXT("SHOW_HAND") );

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
	m_GoldControl.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS,CreateRect,this,8,NULL);
	m_ScoreView.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,CreateRect,this,9);

	//创建按钮
	m_btStart.Create(NULL,WS_CHILD,CreateRect,this,IDC_START);
	m_btFollow.Create(NULL,WS_CHILD,CreateRect,this,IDC_FOLLOW);
	m_btGiveUp.Create(NULL,WS_CHILD,CreateRect,this,IDC_GIVE_UP);
	m_btAddGold.Create(NULL,WS_CHILD,CreateRect,this,IDC_ADD_GOLD);
	m_btShowHand.Create(NULL,WS_CHILD,CreateRect,this,IDC_SHOW_HAND);

	//加载位图
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_BT_START,hInstance,false);
	m_btFollow.SetButtonImage(IDB_BT_FOLLOW,hInstance,false);
	m_btGiveUp.SetButtonImage(IDB_BT_GIVE_UP,hInstance,false);
	m_btAddGold.SetButtonImage(IDB_BT_ADD_GOLD,hInstance,false);
	m_btShowHand.SetButtonImage(IDB_BT_SHOW_HAND,hInstance,false);

	//test
	//BYTE byCard[] = {
	//	0x01,0x09,0x0a,0x0b,0x0c
	//};
	//for( WORD i = 0; i < GAME_PLAYER; i++ )
	//{
	//	m_CardControl[i].SetCardData( byCard,5 );
	//	//SetUserGoldInfo( i,false,1 );
	//	//SetUserGoldInfo( i,true,456 );
	//}
	//m_CardControl[2].AllowPositively( true );
	//end test

	return 0;
}

//开始按钮
void CGameClientView::OnStart()
{
	AfxGetMainWnd()->SendMessage(IDM_START,0,0);
	return;
}

//跟注按钮
void CGameClientView::OnFollow()
{
	AfxGetMainWnd()->SendMessage(IDM_FOLLOW,0,0);
	return;
}

//放弃按钮
void CGameClientView::OnGiveUp()
{
	AfxGetMainWnd()->SendMessage(IDM_GIVE_UP,0,0);
	return;
}

//加注按钮
void CGameClientView::OnAddGold()
{
	AfxGetMainWnd()->SendMessage(IDM_ADD_GOLD,0,0);
	return;
}

//梭哈按钮
void CGameClientView::OnShowHand()
{
	AfxGetMainWnd()->SendMessage(IDM_SHOWHAND,0,0);
	return;
}

//重置界面
void CGameClientView::ResetGameView()
{
	//设置变量
	m_lMaxGold=0L;
	m_lBasicGold=0L;
	m_bUserShowHand = false;
	ZeroMemory( m_lUserGold,sizeof(m_lUserGold) );

	//动画变量
	m_SendCardCurPos = m_SendCardPos;
	m_nStepCount = 0;
	m_nXStep = 0;
	m_nYStep = 0;
	m_SendCardItemArray.RemoveAll();

	//设置控件
	m_btStart.ShowWindow(SW_HIDE);
	m_btFollow.ShowWindow(SW_HIDE);
	m_btGiveUp.ShowWindow(SW_HIDE);
	m_btAddGold.ShowWindow(SW_HIDE);
	m_btShowHand.ShowWindow(SW_HIDE);
	m_ScoreView.ShowWindow(SW_HIDE);
	m_GoldControl.ShowWindow(SW_HIDE);

	//设置筹码
	for (int i=0;i<CountArray(m_GoldView);i++) m_GoldView[i].SetGold(0L);

	//设置扑克
	for (int i=0;i<CountArray(m_CardControl);i++)
	{
		m_CardControl[i].SetCardData(NULL,0);
		m_CardControl[i].ShowFirstCard(false);
		m_CardControl[i].AllowPositively(false);
	}

	return;
}

//调整控件
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//设置坐标
	m_ptFace[0].x=nWidth/2;
	m_ptFace[0].y=nHeight/2-347;
	m_ptName[0].x=nWidth/2+37;
	m_ptName[0].y=nHeight/2-347;
	m_ptTimer[0].x=nWidth/2-45;
	m_ptTimer[0].y=nHeight/2-326;
	m_ptReady[0].x=nWidth/2-47;
	m_ptReady[0].y=nHeight/2-326;
	SetFlowerControlInfo( 0,m_ptFace[0].x+m_nXFace/2-BIG_FACE_WIDTH/2,m_ptFace[0].y+m_nYFace );

	m_ptFace[1].x=nWidth/2+341;
	m_ptFace[1].y=nHeight/2-90;
	m_ptName[1].x=nWidth/2+336;
	m_ptName[1].y=nHeight/2-90;
	m_ptTimer[1].x=nWidth/2+352;
	m_ptTimer[1].y=nHeight/2-115;
	m_ptReady[1].x=nWidth/2+330;
	m_ptReady[1].y=nHeight/2-115;
	SetFlowerControlInfo( 1,m_ptFace[1].x-BIG_FACE_WIDTH,m_ptFace[1].y+m_nYFace/2-BIG_FACE_HEIGHT/2 );

	m_ptFace[2].x=nWidth/2-32;
	m_ptFace[2].y=nHeight/2+316;
	m_ptName[2].x=nWidth/2+5;
	m_ptName[2].y=nHeight/2+316;
	m_ptTimer[2].x=nWidth/2-77;
	m_ptTimer[2].y=nHeight/2+327;
	m_ptReady[2].x=nWidth/2+10;
	m_ptReady[2].y=nHeight/2+96;
	SetFlowerControlInfo( 2,m_ptFace[2].x+m_nXFace/2-BIG_FACE_WIDTH/2,m_ptFace[2].y-BIG_FACE_HEIGHT );

	m_ptFace[3].x=nWidth/2-373;
	m_ptFace[3].y=nHeight/2-90;
	m_ptName[3].x=nWidth/2-336;
	m_ptName[3].y=nHeight/2-90;
	m_ptTimer[3].x=nWidth/2-354;
	m_ptTimer[3].y=nHeight/2-115;
	m_ptReady[3].x=nWidth/2-333;
	m_ptReady[3].y=nHeight/2-115;
	SetFlowerControlInfo( 3,m_ptFace[2].x+m_nXFace,m_ptFace[3].y+m_nYFace/2-BIG_FACE_HEIGHT/2 );

	//筹码控件
	int nXCenterPos=nWidth/2;
	int nYCenterPos=nHeight/2;
	m_GoldStation[0].SetPoint(nXCenterPos-35,nYCenterPos-105);
	m_GoldStation[1].SetPoint(nXCenterPos+33,nYCenterPos-105);
	m_GoldStation[2].SetPoint(nXCenterPos+102,nYCenterPos-33);
	m_GoldStation[3].SetPoint(nXCenterPos+102,nYCenterPos+39);
	m_GoldStation[4].SetPoint(nXCenterPos+31,nYCenterPos+97);
	m_GoldStation[5].SetPoint(nXCenterPos-35,nYCenterPos+97);
	m_GoldStation[6].SetPoint(nXCenterPos-102,nYCenterPos+39);
	m_GoldStation[7].SetPoint(nXCenterPos-102,nYCenterPos-33);

	//按钮控件
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//开始按钮
	CRect rcStart;
	m_btStart.GetWindowRect(&rcStart);
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcStart.Width())/2+10,nHeight/2+156,0,0,uFlags);

	//控制按钮
	CRect rcControl;
	m_btAddGold.GetWindowRect(&rcControl);
	DeferWindowPos(hDwp,m_btAddGold,NULL,nWidth/2+125,nHeight/2+303-rcControl.Height(),0,0,uFlags);
	DeferWindowPos(hDwp,m_btFollow,NULL,nWidth/2+125,nHeight/2+303-rcControl.Height(),0,0,uFlags);
	DeferWindowPos(hDwp,m_btShowHand,NULL,nWidth/2+rcControl.Width()+130,nHeight/2+303-rcControl.Height(),0,0,uFlags);
	DeferWindowPos(hDwp,m_btGiveUp,NULL,nWidth/2+rcControl.Width()*2+135,nHeight/2+303-rcControl.Height(),0,0,uFlags);

	EndDeferWindowPos(hDwp);

	//加注窗口
	m_GoldControl.SetBenchmarkPos( nWidth/2+300,nHeight/2+293-rcControl.Height() );

	//成绩窗口
	CRect rcScoreView;
	m_ScoreView.GetWindowRect(&rcScoreView);
	m_ScoreView.MoveWindow((nWidth-rcScoreView.Width())/2,(nHeight-rcScoreView.Height())/2-10,rcScoreView.Width(),rcScoreView.Height());

	//扑克控件
	m_CardControl[0].SetBasicStation(CPoint(nWidth/2,nHeight/2-300),enXCenter,enYTop);
	m_CardControl[1].SetBasicStation(CPoint(nWidth/2+373,nHeight/2),enXRight,enYCenter);
	m_CardControl[2].SetBasicStation(CPoint(nWidth/2,nHeight/2+298),enXCenter,enYBottom);
	m_CardControl[3].SetBasicStation(CPoint(nWidth/2-373,nHeight/2),enXLeft,enYCenter);

	//发牌起始位置
	m_SendCardPos.SetPoint(nWidth/2-40,nHeight/2-52);

	return;
}

//绘画界面
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//绘画背景
	CImageHandle HandleTitle(&m_ImageTitle);
	DrawViewImage(pDC,m_ImageViewFill,enMode_Spread);
	DrawViewImage(pDC,m_ImageViewCenter,enMode_Centent);
	m_ImageTitle.BitBlt(pDC->m_hDC,m_nXBorder,m_nYBorder);

	//绘画用户
	WORD wUserTimer=0;
	TCHAR szBuffer[64];
	pDC->SetTextColor(RGB(255,255,0));
	for (WORD i=0;i<4;i++)
	{
		//变量定义
		const tagUserData * pUserData=GetUserInfo(i);
		
		////////////////////////////////////////////////////////
		//tagUserData *pUserData = new tagUserData;
		//strcpy( pUserData->szName, "dkfjdkfj" );
		//pUserData->cbUserStatus = US_READY ;
		//wUserTimer = 50;
		//////////////////////////////////////////////////////////
		

		if (pUserData!=NULL)
		{
			//用户名字
			pDC->SetTextAlign((i==1)?(TA_RIGHT|TA_TOP):(TA_LEFT|TA_TOP));
			pDC->TextOut(m_ptName[i].x,m_ptName[i].y,pUserData->szName,lstrlen(pUserData->szName));

			//用户金币
			LONG lUserScore = (pUserData->cbUserStatus==US_PLAY)?m_lUserGold[i]-m_GoldView[i*2].GetGold()-m_GoldView[i*2+1].GetGold():pUserData->lScore;
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),lUserScore);
			pDC->TextOut(m_ptName[i].x,m_ptName[i].y+16,szBuffer,lstrlen(szBuffer));

			//其他信息
			wUserTimer=GetUserTimer(i);
			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);
		}
	}

	for( WORD i = 0; i < GAME_PLAYER; i++ )
		m_CardControl[i].DrawCardControl(pDC);

	//绘画筹码
	LONG lTotalGold=0L;
	for (DWORD i=0;i<CountArray(m_GoldView);i++)
	{
		lTotalGold+=m_GoldView[i].GetGold();
		m_GoldView[i].DrawGoldView(pDC,m_GoldStation[i].x,m_GoldStation[i].y);
	}

	//游戏信息
	pDC->SetTextAlign(TA_LEFT|TA_TOP);
	pDC->SetTextColor(RGB(250,250,250));
	const UINT uDrawFormat=DT_END_ELLIPSIS|DT_CENTER|DT_SINGLELINE|DT_VCENTER;

	//最大下注
	if (m_lMaxGold!=0)
	{
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_lMaxGold);
		DrawText(pDC->m_hDC,szBuffer,lstrlen(szBuffer),CRect(m_nXBorder+80,m_nYBorder+10,m_nXBorder+140,m_nYBorder+22),uDrawFormat);
	}

	//基础下注
	if (m_lBasicGold!=0)
	{
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_lBasicGold);
		DrawText(pDC->m_hDC,szBuffer,lstrlen(szBuffer),CRect(m_nXBorder+80,m_nYBorder+29,m_nXBorder+140,m_nYBorder+41),uDrawFormat);
	}

	//游戏总注
	if (lTotalGold!=0L)
	{
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),lTotalGold);
		DrawText(pDC->m_hDC,szBuffer,lstrlen(szBuffer),CRect(m_nXBorder+80,m_nYBorder+48,m_nXBorder+140,m_nYBorder+60),uDrawFormat);
	}

	//绘画扑克
	if (m_SendCardItemArray.GetCount()>0)
	{
		//变量定义
		CImageHandle HandleCard(&m_ImageCardBack);
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

		//绘画扑克
		INT nCardWidth = m_ImageCardBack.GetWidth()/13;
		INT nCardHeight = m_ImageCardBack.GetHeight()/5;
		m_ImageCardBack.AlphaDrawImage(pDC,m_SendCardCurPos.x,m_SendCardCurPos.y,nCardWidth,nCardHeight,nCardWidth*2,nCardHeight*4,RGB(255,0,255));
	}

	//用户梭哈
	if( m_bUserShowHand )
	{
		m_PngShowHand.DrawImage( pDC,(nWidth-m_PngShowHand.GetWidth())/2,(nHeight-m_PngShowHand.GetHeight())/2 );
	}

	return;
}

//金币标题
void CGameClientView::SetGoldTitleInfo(DWORD lMaxGold, DWORD lBasicGold)
{
	//设置变量
	m_lMaxGold=lMaxGold;
	m_lBasicGold=lBasicGold;

	//更新视图
	UpdateGameView(NULL);

	return;
}

//用户筹码
void CGameClientView::SetUserGoldInfo(WORD wViewChairID, bool bTableGold, DWORD dwGold)
{
	//参数判断
	WORD wGoldPos=wViewChairID*2+((bTableGold==true)?1:0);
	if (wGoldPos>=CountArray(m_GoldStation)) return;

	//设置变量
	DWORD m_dwAllGold=0L;
	m_GoldView[wGoldPos].SetGold(dwGold);
	for (int i=0;i<CountArray(m_GoldStation);i++) m_dwAllGold+=m_GoldView[i].GetGold();

	//更新视图
	UpdateGameView(NULL);

	return;
}

//定时器消息
void CGameClientView::OnTimer(UINT nIDEvent)
{
	//发送处理
	if (nIDEvent==IDI_SEND_CARD)
	{
		//设置位置
		m_SendCardCurPos.x += m_nXStep;
		m_SendCardCurPos.y += m_nYStep;

		//更新界面
		UpdateGameView(NULL);

		//停止判断
		if ( --m_nStepCount == 0 )
		{
			//获取扑克
			BYTE cbCardData[MAX_COUNT];
			WORD wChairID=m_SendCardItemArray[0].wChairID;
			BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

			//设置扑克
			if (cbCardCount<MAX_COUNT)
			{
				cbCardData[cbCardCount++]=m_SendCardItemArray[0].cbCardData;
				m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);
			}

			//删除动画
			m_SendCardItemArray.RemoveAt(0);

			//继续动画
			if (m_SendCardItemArray.GetCount()>0)
			{
				//
				wChairID = m_SendCardItemArray[0].wChairID;
				m_nStepCount = SEND_STEP_COUNT;
				m_SendCardCurPos = m_SendCardPos;
				m_nXStep = (m_CardControl[wChairID].GetTailPos().x-m_SendCardPos.x)/m_nStepCount;
				m_nYStep = (m_CardControl[wChairID].GetTailPos().y-m_SendCardPos.y)/m_nStepCount;

				//播放声音
				CGameClientDlg * pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
				pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));

				return;
			}
			else 
			{
				//完成处理
				KillTimer(IDI_SEND_CARD);
				AfxGetMainWnd()->PostMessage(IDM_SEND_CARD_FINISH,0,0);

				return;
			}
		}

		return;
	}
#ifdef SHOW_CARD_ANIMATE
	else if( nIDEvent == IDI_SHOW_CARD )
	{
		if( !m_CardControl[2].PlayMoveCard() )
		{
			KillTimer( IDI_SHOW_CARD );
		}
		UpdateGameView(NULL);
		return;
	}
#endif
	else if( nIDEvent == IDI_USER_SHOW_HAND )
	{
		KillTimer( IDI_USER_SHOW_HAND );
		m_bUserShowHand = false;
		UpdateGameView( NULL );
		return;
	}
	__super::OnTimer(nIDEvent);
}

//动画判断
bool CGameClientView::IsDispatchCard()
{
	return m_SendCardItemArray.GetCount()>0;
}

//完成发牌
void CGameClientView::FinishDispatchCard()
{
	//完成判断
	if (m_SendCardItemArray.GetCount()==0) return;

	//完成动画
	for (INT_PTR i=0;i<m_SendCardItemArray.GetCount();i++)
	{
		//获取数据
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[i];

		//获取扑克
		BYTE cbCardData[MAX_COUNT];
		WORD wChairID=pSendCardItem->wChairID;
		BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

		//设置扑克
		if (cbCardCount<MAX_COUNT)
		{
			cbCardData[cbCardCount++]=pSendCardItem->cbCardData;
			m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);
		}
	}

	//删除处理
	KillTimer(IDI_SEND_CARD);
	m_SendCardItemArray.RemoveAll();

	AfxGetMainWnd()->SendMessage(IDM_SEND_CARD_FINISH,0,0);

	//更新界面
	UpdateGameView(NULL);

	return;
}

//发送扑克
void CGameClientView::DispatchUserCard(WORD wChairID, BYTE cbCardData)
{
	//效验参数
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return;

	//设置变量
	tagSendCardItem SendCardItem;
	SendCardItem.wChairID=wChairID;
	SendCardItem.cbCardData=cbCardData;
	m_SendCardItemArray.Add(SendCardItem);

	//启动动画
	if (m_SendCardItemArray.GetCount()==1) 
	{
		//设置定时器
		SetTimer(IDI_SEND_CARD,SPEED_SEND_CARD,NULL);

		//
		m_nStepCount = SEND_STEP_COUNT;
		m_SendCardCurPos = m_SendCardPos;
		m_nXStep = (m_CardControl[wChairID].GetTailPos().x-m_SendCardPos.x)/m_nStepCount;
		m_nYStep = (m_CardControl[wChairID].GetTailPos().y-m_SendCardPos.y)/m_nStepCount;

		//
		UpdateGameView(NULL);

		//播放声音
		CGameClientDlg * pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
		pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
	}

	return;
}

//光标消息
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//获取光标
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//扑克测试
	if (m_CardControl[2].OnEventSetCursor(MousePoint)==true) return TRUE;

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//鼠标消息
VOID CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//获取光标
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//扑克点击
	if (m_CardControl[2].OnEventLeftMouseDown(MousePoint)==true)
	{
#ifdef	SHOW_CARD_ANIMATE
		if( m_CardControl[2].BeginMoveCard() )
		{
			SetTimer( IDI_SHOW_CARD,SPEED_MOVE_CARD,NULL );
			//更新界面
			UpdateGameView(NULL);
		}
#else
		m_CardControl[2].ShowFirstCard(true);
		UpdateGameView(NULL);
#endif
	}

	return;
}

//
VOID CGameClientView::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags, Point);

	//获取光标
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//扑克点击
	if (m_CardControl[2].OnEventLeftMouseUp(MousePoint)==true)
	{
#ifndef	SHOW_CARD_ANIMATE
		m_CardControl[2].ShowFirstCard(false);
		UpdateGameView(NULL);
		PostMessage( WM_SETCURSOR );
#endif
	}
}

//
void CGameClientView::SetUserShowHand( bool bShowHand )
{
	if( m_bUserShowHand != bShowHand )
	{
		m_bUserShowHand = bShowHand;
		
		//
		if( m_bUserShowHand )
			SetTimer( IDI_USER_SHOW_HAND,TIME_USER_SHOW_HAND,NULL );

		UpdateGameView( NULL );
	}
}

//
void CGameClientView::SetUserGold( WORD wChairId, LONG lGold )
{
	if( wChairId == INVALID_CHAIR )
		ZeroMemory( m_lUserGold,sizeof(m_lUserGold) );
	else m_lUserGold[wChairId] = lGold;
	UpdateGameView( NULL );
}

//////////////////////////////////////////////////////////////////////////
