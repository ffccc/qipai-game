#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//按钮标识 
#define IDC_START						100								//开始按钮

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_START, OnStart)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientView::CGameClientView(void) : CGameFrameView(true,24)
{
	//游戏变量
	m_lCellScore=0L;
	m_bLeftCardCount=0xFF;
	m_wBankerUser=INVALID_CHAIR;

	//玩家状态
	m_bOutCard=false;
	m_bWaitOther=false;
	m_bHuangZhuang=false;

	//辅助变量
	m_bMoveCard=false;
	m_bSwitchCard=false;
	m_wMouseDownItem=INVALID_ITEM;

	//用户状态
	m_cbCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_bUserAction,sizeof(m_bUserAction));
	for (BYTE i=0;i<CountArray(m_bUserHuXiCount);i++) m_bUserHuXiCount[i]=0xFF;

	//加载位图
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageTitle.SetLoadInfo(IDB_TITLE,hInstance);
	m_ImageWait.SetLoadInfo(IDB_WAIT_TIP,hInstance);
	m_ImageBanker.SetLoadInfo(IDB_BANKER,hInstance);
	m_ImageViewBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageOutCard.SetLoadInfo(IDB_OUT_CARD_TIP,hInstance);
	m_ImageCardFrame.SetLoadInfo(IDB_CARD_FRAME,hInstance);
	m_ImageUserAction.SetLoadInfo(IDB_USER_ACTION,hInstance);
	m_ImageActionBack.SetLoadInfo(IDB_ACTION_BACK,hInstance);
	m_ImageHuangZhuang.SetLoadInfo(IDB_HUANG_ZHUANG,hInstance);

	//获取大小
	CImageHandle ImageHandleBanker(&m_ImageBanker);
	m_BankerSize.cx=m_ImageBanker.GetWidth();
	m_BankerSize.cy=m_ImageBanker.GetHeight();

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

	//变量定义
	enDirection Direction[]={Direction_East,Direction_South,Direction_West};

	//设置控件
	m_UserCard[0].SetDirection(Direction_East);
	m_UserCard[1].SetDirection(Direction_West);
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		//丢弃扑克
		m_DiscardCard[i].SetDirection(Direction[i]);

		//组合扑克
		for (WORD j=0;j<CountArray(m_WeaveCard[i]);j++) m_WeaveCard[i][j].SetDirection(Direction[i]);
	}

	//创建按钮
	CRect rcCreate(0,0,0,0);
	m_btStart.Create(NULL,WS_CHILD,rcCreate,this,IDC_START);
	m_ChooseWnd.Create(NULL,NULL,WS_CHILD,rcCreate,this,100);
	m_ControlWnd.Create(NULL,NULL,WS_CHILD,rcCreate,this,101);
	m_ScoreView.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,102);

	//加载位图
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_BT_START,hInstance,false);

	return 0;
}

//重置界面
void CGameClientView::ResetGameView()
{
	//游戏变量
	m_lCellScore=0L;
	m_bLeftCardCount=0xFF;
	m_wBankerUser=INVALID_CHAIR;

	//玩家状态
	m_bOutCard=false;
	m_bWaitOther=false;
	m_bHuangZhuang=false;

	//辅助变量
	m_bMoveCard=false;
	m_bSwitchCard=false;
	m_wMouseDownItem=INVALID_ITEM;

	//用户状态
	m_cbCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_bUserAction,sizeof(m_bUserAction));
	for (BYTE i=0;i<CountArray(m_bUserHuXiCount);i++) m_bUserHuXiCount[i]=0xFF;

	//隐藏控件
	m_btStart.ShowWindow(SW_HIDE);
	m_ScoreView.ShowWindow(SW_HIDE);
	m_ChooseWnd.ShowWindow(SW_HIDE);
	m_ControlWnd.ShowWindow(SW_HIDE);

	//设置扑克
	m_UserCard[0].SetCardData(0);
	m_UserCard[1].SetCardData(0);
	m_HandCardControl.SetCardData(NULL,0);
	m_HandCardControl.SetPositively(false);
	m_HandCardControl.SetDisplayItem(false);

	//变量定义
	tagWeaveItem WeaveItem;
	WeaveItem.cbWeaveKind=ACK_NULL;

	//设置扑克
	for (int i=0;i<GAME_PLAYER;i++)
	{
		m_DiscardCard[i].SetCardData(NULL,0);
		for (int j=0;j<CountArray(m_WeaveCard[0]);j++) m_WeaveCard[i][j].SetCardData(WeaveItem);
	}

	return;
}

//调整控件
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//设置坐标
	m_ptFace[0].x=nWidth-m_nXBorder-m_nXFace-5;
	m_ptFace[0].y=nHeight/2-m_nYFace-50;
	m_ptName[0].x=nWidth-m_nXBorder-5;
	m_ptName[0].y=nHeight/2-45;
	m_ptTimer[0].x=nWidth-m_nXBorder-m_nXFace-5;
	m_ptTimer[0].y=nHeight/2-m_nYFace-55-m_nYTimer;
	m_ptReady[0].x=nWidth-m_nXBorder-m_nXTimer-5;
	m_ptReady[0].y=nHeight/2-m_nYFace-55-m_nYTimer/2;
	m_BankerPoint[0].x=nWidth-m_nXBorder-m_BankerSize.cx-3;
	m_BankerPoint[0].y=nHeight/2-10;

	m_ptFace[1].x=nWidth/2-m_nXFace/2;
	m_ptFace[1].y=nHeight-m_nYBorder-m_nYFace-5;
	m_ptName[1].x=nWidth/2+5+m_nXFace/2;
	m_ptName[1].y=nHeight-m_nYBorder-m_nYFace-2;
	m_ptTimer[1].x=nWidth/2-m_nXFace/2-m_nXTimer*2-5;
	m_ptTimer[1].y=nHeight-m_nYBorder-m_nYTimer-10;
	m_ptReady[1].x=nWidth/2-m_nXTimer-m_nXFace/2-5;
	m_ptReady[1].y=nHeight-m_nYBorder-m_nYTimer/2-10;
	m_BankerPoint[1].x=nWidth/2-m_nXFace/2-m_BankerSize.cx-m_nXTimer*2-10;
	m_BankerPoint[1].y=nHeight-m_nXBorder-m_BankerSize.cy-5;

	m_ptFace[2].x=m_nXBorder+5;
	m_ptFace[2].y=nHeight/2-m_nYFace-50;
	m_ptName[2].x=m_nXBorder+5;
	m_ptName[2].y=nHeight/2-45;
	m_ptTimer[2].x=m_nXBorder+5;
	m_ptTimer[2].y=nHeight/2-m_nYFace-55-m_nYTimer;
	m_ptReady[2].x=m_nXBorder+5+m_nXTimer;
	m_ptReady[2].y=nHeight/2-m_nYFace-55-m_nYTimer/2;
	m_BankerPoint[2].x=m_nXBorder+3;
	m_BankerPoint[2].y=nHeight/2-10;

	//丢弃扑克
	m_DiscardCard[0].SetControlPoint(nWidth-m_nXBorder-130,nHeight/2-290);
	m_DiscardCard[1].SetControlPoint(nWidth/2+295,nHeight-m_nYFace-m_nYBorder-105);
	m_DiscardCard[2].SetControlPoint(m_nXBorder+130,nHeight/2+170);

	//组合扑克
	for (WORD i=0;i<CountArray(m_WeaveCard[0]);i++)
	{
		m_WeaveCard[0][i].SetControlPoint(nWidth-m_nXBorder-112,nHeight/2-i*30+53);
		m_WeaveCard[1][i].SetControlPoint(nWidth/2-110+i*35,nHeight-270);
		m_WeaveCard[2][i].SetControlPoint(m_nXBorder+112,nHeight/2-193+i*30);
	}

	//用户扑克
	m_UserCard[0].SetControlPoint(nWidth-m_nXBorder-70,nHeight/2+140);
	m_UserCard[1].SetControlPoint(m_nXBorder+70,nHeight/2-260);
	m_HandCardControl.SetBenchmarkPos(nWidth/2,nHeight-m_nYFace-m_nYBorder-15,enXCenter,enYBottom);

	//移动按钮
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(6);
	m_btStart.GetWindowRect(&rcButton);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	DeferWindowPos(hDwp,m_ControlWnd,NULL,nWidth-400-m_nXBorder,nHeight-215-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-195-m_nYBorder,0,0,uFlags);
	EndDeferWindowPos(hDwp);

	//控制窗口
	m_ChooseWnd.SetBenchmarkPos(nWidth-200-m_nXBorder,nHeight-225-m_nYBorder);

	//积分视图
	CRect rcControl;
	m_ScoreView.GetWindowRect(&rcControl);
	m_ScoreView.SetWindowPos(NULL,(nWidth-rcControl.Width())/2,130,0,0,SWP_NOZORDER|SWP_NOSIZE);

	return;
}

//绘画界面
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//绘画背景
	CImageHandle HandleTitle(&m_ImageTitle);
	DrawViewImage(pDC,m_ImageViewBack,enMode_Centent);
	m_ImageTitle.BitBlt(pDC->m_hDC,m_nXBorder+10,m_nYBorder+10);

	//扑克背景
	CImageHandle HandleCardFrame(&m_ImageCardFrame);
	m_ImageCardFrame.BitBlt(pDC->m_hDC,(nWidth-m_ImageCardFrame.GetWidth())/2,m_nYBorder+10);

	//绘画用户
	TCHAR szBuffer[32];
	pDC->SetTextColor(RGB(255,255,0));
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//变量定义
		const tagUserData * pUserData=GetUserInfo(i);
		if (pUserData!=NULL)
		{
			//用户名字
			pDC->SetTextAlign((i==0)?(TA_RIGHT|TA_TOP):(TA_LEFT|TA_TOP));
			pDC->TextOut(m_ptName[i].x,m_ptName[i].y,pUserData->szName,lstrlen(pUserData->szName));

			//剩余扑克
			if (m_bUserHuXiCount[i]!=0xFF)
			{
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("（%ld 胡息）"),m_bUserHuXiCount[i]);
				pDC->TextOut(m_ptName[i].x,m_ptName[i].y+18,szBuffer,lstrlen(szBuffer));
			}

			//其他信息
			WORD wUserTimer=GetUserTimer(i);
			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);
		}
	}

	//庄家信息
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//获取用户
		const tagUserData * pUserData=GetUserInfo(m_wBankerUser);
		if (pUserData!=NULL)
		{
			//变量定义
			CImageHandle ImageHandleBanker(&m_ImageBanker);

			//设置 DC
			pDC->SetTextAlign(TA_LEFT|TA_TOP);
			pDC->SetTextColor(RGB(250,250,250));

			//庄家名字
			UINT uFormat=DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS;
			CRect rcBanker(m_nXBorder+86,m_nYBorder+34,m_nXBorder+166,m_nYBorder+50);
			pDC->DrawText(pUserData->szName,lstrlen(pUserData->szName),&rcBanker,uFormat);

			//庄家标志
			m_ImageBanker.AlphaDrawImage(pDC,m_BankerPoint[m_wBankerUser].x,m_BankerPoint[m_wBankerUser].y,RGB(255,0,255));
		}
	}

	//基础积分
	if (m_lCellScore!=0L)
	{
		pDC->SetTextColor(RGB(250,250,250));
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_lCellScore);
		CRect rcCellScore(m_nXBorder+86,m_nYBorder+14,m_nXBorder+166,m_nYBorder+29);
		pDC->DrawText(szBuffer,lstrlen(szBuffer),&rcCellScore,DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS);
	}

	//剩余扑克
	if (m_bLeftCardCount!=0xFF)
	{
		//剩余信息
		pDC->SetTextColor(RGB(250,250,250));
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld 张"),m_bLeftCardCount);
		CRect rcCellScore(m_nXBorder+86,m_nYBorder+53,m_nXBorder+166,m_nYBorder+68);
		pDC->DrawText(szBuffer,lstrlen(szBuffer),&rcCellScore,DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS);

		//扑克信息
		if (m_bLeftCardCount>0)
		{
			int nYPos=m_nYBorder+19,nSpace=9;
			int nXPos=nWidth/2-((m_bLeftCardCount-1)*nSpace+g_CardResource.m_ImageTableTop.GetItemWidth())/2;
			for (BYTE i=0;i<m_bLeftCardCount;i++) g_CardResource.m_ImageTableTop.DrawCardItem(pDC,0,nXPos+i*nSpace,nYPos);
		}
	}

	//用户扑克
	m_UserCard[0].DrawCardControl(pDC);
	m_UserCard[1].DrawCardControl(pDC);
	m_HandCardControl.DrawCardControl(pDC);

	//绘画扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//桌面扑克
		m_DiscardCard[i].DrawCardControl(pDC);

		//组合扑克
		for (WORD j=0;j<CountArray(m_WeaveCard[i]);j++) m_WeaveCard[i][j].DrawCardControl(pDC);
	}

	//出牌提示
	if (m_bOutCard==true)
	{
		CImageHandle HandleOutCard(&m_ImageOutCard);
		m_ImageOutCard.AlphaDrawImage(pDC,(nWidth-m_ImageOutCard.GetWidth())/2,nHeight-300,RGB(255,0,255));
	}

	//等待提示
	if (m_bWaitOther==true)
	{
		CImageHandle HandleWait(&m_ImageWait);
		m_ImageWait.AlphaDrawImage(pDC,(nWidth-m_ImageWait.GetWidth())/2,nHeight-300,RGB(255,0,255));
	}

	//荒庄标志
	if (m_bHuangZhuang==true)
	{
		CImageHandle HandleHuangZhuang(&m_ImageHuangZhuang);
		m_ImageHuangZhuang.AlphaDrawImage(pDC,(nWidth-m_ImageHuangZhuang.GetWidth())/2,nHeight/2-115,RGB(255,0,255));
	}

	//绘画出牌
	if (m_cbCardData!=0)
	{
		//计算位置
		switch (m_wOutCardUser)
		{
		case 0:	//东向
			{
				int nYPos=nHeight/2-60-g_CardResource.m_ImageUserRight.GetItemHeight()/2;
				int nXPos=nWidth-m_nXBorder-290-g_CardResource.m_ImageUserRight.GetItemWidth()/2;
				g_CardResource.m_ImageUserRight.DrawCardItem(pDC,m_cbCardData,nXPos,nYPos);

				break;
			}
		case 1:	//南向
			{
				int nXPos=nWidth/2-g_CardResource.m_ImageUserBottom.GetItemWidth()/2;;
				int nYPos=nHeight-m_nYBorder-335-g_CardResource.m_ImageUserBottom.GetItemHeight()/2;
				g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,m_cbCardData,nXPos,nYPos);

				break;
			}
		case 2:	//西向
			{
				int nXPos=m_nXBorder+290-g_CardResource.m_ImageUserLeft.GetItemWidth()/2;
				int nYPos=nHeight/2-60-g_CardResource.m_ImageUserLeft.GetItemHeight()/2;
				g_CardResource.m_ImageUserLeft.DrawCardItem(pDC,m_cbCardData,nXPos,nYPos);

				break;
			}
		}
	}

	//绘画动作
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_bUserAction[i]!=0)
		{
			//计算位置
			int nXPos=0,nYPos=0;
			int nActionWidth=129,nActionHeight=109;
			switch (i)
			{
			case 0:	//东向
				{
					nXPos=nWidth-m_nXBorder-145-nActionWidth;
					nYPos=nHeight/2-103;
					break;
				}
			case 1:	//南向
				{
					nXPos=(nWidth-nActionWidth)/2;
					nYPos=nHeight-m_nYBorder-150-nActionHeight;
					break;
				}
			case 2:	//西向
				{
					nXPos=m_nXBorder+145;
					nYPos=nHeight/2-103;
					break;
				}
			}

			//变量定义
			int nXImagePos=0;
			CImageHandle HandleUserAction(&m_ImageUserAction);
			int nItemWidth=m_ImageUserAction.GetWidth()/6;

			//计算位置
			if (m_bUserAction[i]&ACK_TI) nXImagePos=0;
			else if (m_bUserAction[i]&ACK_PAO) nXImagePos=nItemWidth;
			else if (m_bUserAction[i]&ACK_WEI) nXImagePos=nItemWidth*2;
			else if (m_bUserAction[i]&ACK_PENG) nXImagePos=nItemWidth*4;
			else if (m_bUserAction[i]&ACK_CHI) nXImagePos=nItemWidth*3;

			//动作背景
			CImageHandle HandleActionBack(&m_ImageActionBack);
			m_ImageActionBack.AlphaDrawImage(pDC,nXPos,nYPos,m_ImageActionBack.GetWidth()/4,m_ImageActionBack.GetHeight(),
				(i+1)*m_ImageActionBack.GetWidth()/4,0,RGB(255,0,255));

			//绘画动作
			int nItemHeight=m_ImageUserAction.GetHeight();
			m_ImageUserAction.AlphaDrawImage(pDC,nXPos+40,nYPos+28,nItemWidth,nItemHeight,nXImagePos,0,RGB(255,0,255));
		}
	}

	return;
}

//基础积分
void CGameClientView::SetCellScore(LONG lCellScore)
{
	//设置扑克
	if (lCellScore!=m_lCellScore)
	{
		//设置变量
		m_lCellScore=lCellScore;

		//更新界面
		UpdateGameView(NULL);
	}

	return;
}

//庄家用户
void CGameClientView::SetBankerUser(WORD wBankerUser)
{
	//设置用户
	if (wBankerUser!=m_wBankerUser)
	{
		//设置变量
		m_wBankerUser=wBankerUser;

		//更新界面
		UpdateGameView(NULL);
	}

	return;
}

//设置荒庄
void CGameClientView::SetHuangZhuang(bool bHuangZhuang)
{
	//设置用户
	if (bHuangZhuang!=m_bHuangZhuang)
	{
		//设置变量
		m_bHuangZhuang=bHuangZhuang;

		//更新界面
		UpdateGameView(NULL);
	}

	return;
}

//剩余扑克
void CGameClientView::SetLeftCardCount(BYTE bLeftCardCount)
{
	//设置扑克
	if (bLeftCardCount!=m_bLeftCardCount)
	{
		//设置变量
		m_bLeftCardCount=bLeftCardCount;

		//更新界面
		UpdateGameView(NULL);
	}

	return;
}

//状态标志
void CGameClientView::SetStatusFlag(bool bOutCard, bool bWaitOther)
{
	//设置变量
	m_bOutCard=bOutCard;
	m_bWaitOther=bWaitOther;

	//更新界面
	UpdateGameView(NULL);

	return;
}

//胡息数目
void CGameClientView::SetUserHuXiCount(WORD wChairID, BYTE bHuXiCount)
{
	//设置变量
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<CountArray(m_bUserHuXiCount);i++) m_bUserHuXiCount[i]=bHuXiCount;
	}
	else m_bUserHuXiCount[wChairID]=bHuXiCount;

	//更新界面
	UpdateGameView(NULL);

	return;
}

//动作信息
void CGameClientView::SetUserAction(WORD wViewChairID, BYTE bUserAction)
{
	//设置变量
	if (wViewChairID<GAME_PLAYER) m_bUserAction[wViewChairID]=bUserAction;
	else ZeroMemory(m_bUserAction,sizeof(m_bUserAction));

	//更新界面
	UpdateGameView(NULL);

	return;
}

//出牌信息
void CGameClientView::SetOutCardInfo(WORD wViewChairID, BYTE cbCardData)
{
	//设置变量
	m_cbCardData=cbCardData;
	m_wOutCardUser=wViewChairID;

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

//鼠标消息
void CGameClientView::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags, Point);

	//出牌处理
	if (m_bMoveCard==false)
	{
		BYTE cbHoverCard=m_HandCardControl.GetHoverCard();
		if (cbHoverCard!=0) AfxGetMainWnd()->PostMessage(IDM_OUT_CARD,cbHoverCard,cbHoverCard);
	}

	//交换处理
	if (m_bSwitchCard==true)
	{
		//释放鼠标
		ReleaseCapture();

		//设置变量
		m_bMoveCard=false;
		m_bSwitchCard=false;
		m_wMouseDownItem=INVALID_ITEM;
	}

	return;
}

//鼠标消息
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//获取索引
	m_wMouseDownItem=m_HandCardControl.SwitchCardPoint(Point);

	//交换判断
	if ((m_wMouseDownItem!=INVALID_ITEM)&&(m_HandCardControl.IsPositively()==true))
	{
		//设置鼠标
		SetCapture();

		//设置变量
		m_bMoveCard=false;
		m_bSwitchCard=true;
	}

	return;
}

//鼠标消息
void CGameClientView::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	//交换扑克
	if (m_bSwitchCard==true)
	{
		//获取光标
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);

		//获取索引
		WORD wMouseDownItem=m_wMouseDownItem;
		m_wMouseDownItem=m_HandCardControl.SwitchCardPoint(MousePoint);

		//交换判断
		if (wMouseDownItem!=m_wMouseDownItem)
		{
			//交换扑克
			m_bMoveCard=true;
			m_HandCardControl.SwitchCardItem(wMouseDownItem,m_wMouseDownItem);

			//更新界面
			UpdateGameView(NULL);
		}
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

	//点击测试
	bool bRePaint=false;
	bool bHandle=m_HandCardControl.OnEventSetCursor(MousePoint,bRePaint);

	//重画控制
	if (bRePaint==true) UpdateGameView(NULL);

	//光标控制
	if (bHandle==false) __super::OnSetCursor(pWnd,nHitTest,uMessage);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
