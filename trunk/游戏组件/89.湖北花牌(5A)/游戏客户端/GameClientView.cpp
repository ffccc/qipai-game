#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////
//按钮标识 

//按钮标识 
#define IDC_DISTUOGUAN					109								//取消托管按钮
#define IDC_TUOGUAN						105								//托管按钮

#define IDC_START						100								//开始按钮
#define IDC_CHANG_CARD					104                             //查看藏牌按钮								
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_START, OnStart)

	
	ON_BN_CLICKED(IDC_DISTUOGUAN, OnDisTuoGuan)
	ON_BN_CLICKED(IDC_TUOGUAN, OnTuoGuan)

	ON_REGISTERED_MESSAGE(WM_DD_DRAGENTER,  OnDragEnter)
	ON_REGISTERED_MESSAGE(WM_DD_DRAGDROP,	OnDragDrop)
	ON_REGISTERED_MESSAGE(WM_DD_DRAGABORT,	OnDragAbort)

	ON_MESSAGE(IDM_MOUSE_MOVE,OnSonMouseMove)	
END_MESSAGE_MAP()


//构造函数
CGameClientView::CGameClientView()
{
	m_IsStartSendCard=false;
	//标志变量
	m_bOutCard=false;
	m_bWaitOther=false;
	m_bHuangZhuang=false;
	m_bDecideBanker=false;

	//游戏属性
	m_lCellScore=0l;
	m_wBankerUser=INVALID_CHAIR;
	m_leaveHeapCardData=0;

	//用户状态
	m_cbCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbTongCount,sizeof(m_cbTongCount));

	//加载位图
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageWait.SetLoadInfo(IDB_WAIT_TIP,hInstance);
	m_ImageBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageUserFlag.SetLoadInfo(IDB_USER_FLAG,hInstance);
	m_ImageOutCard.SetLoadInfo(IDB_OUT_CARD_TIP,hInstance);
	m_ImageUserAction.SetLoadInfo(IDB_USER_ACTION,hInstance);
	m_ImageActionBack.SetLoadInfo(IDB_ACTION_BACK,hInstance);
	m_ImageHuangZhuang.SetLoadInfo(IDB_HUANG_ZHUANG,hInstance);
	
	ZeroMemory(m_DecideBankerCard,sizeof(m_UserDecideCardPos));
	ZeroMemory(m_UserDecideCardPos,sizeof(m_UserDecideCardPos));

	m_cbCurrentCard = 0;					//清空当前牌

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

	enDirection Direction[]={Direction_East,Direction_South,Direction_West};

	//设置控件
	for (WORD i=0;i<3;i++)
	{

		m_TableCard[i].SetDirection(Direction[i]);
		m_DiscardCard[i].SetDirection(Direction[i]);

		//组合扑克
		m_WeaveCard[i][0].SetDisplayItem(true);
		m_WeaveCard[i][1].SetDisplayItem(true);
		m_WeaveCard[i][2].SetDisplayItem(true);
		m_WeaveCard[i][3].SetDisplayItem(true);
		m_WeaveCard[i][4].SetDisplayItem(true);
		m_WeaveCard[i][5].SetDisplayItem(true);
		m_WeaveCard[i][6].SetDisplayItem(true);
		m_WeaveCard[i][7].SetDisplayItem(true);


		m_WeaveCard[i][0].SetDirection(Direction[i]);
		m_WeaveCard[i][1].SetDirection(Direction[i]);
		m_WeaveCard[i][2].SetDirection(Direction[i]);
		m_WeaveCard[i][3].SetDirection(Direction[i]);
		m_WeaveCard[i][4].SetDirection(Direction[i]);
		m_WeaveCard[i][5].SetDirection(Direction[i]);
		m_WeaveCard[i][6].SetDirection(Direction[i]);
		m_WeaveCard[i][7].SetDirection(Direction[i]);

	}

	m_UserCard[0].SetDirection(Direction_East);
	m_UserCard[2].SetDirection(Direction_West);

	//创建控件
	CRect rcCreate(0,0,0,0);
	
	m_GameScoreWnd.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,100);
	m_ControlWnd.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,10,NULL);

	for(BYTE k=0;k<9;k++)
	{
		m_HandCard[k].Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,1000+k,NULL);
	}

	//11-28
	m_HuCardDlg.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,11);

	m_btDisTuoGuan.Create(NULL,WS_CHILD,rcCreate,this,IDC_DISTUOGUAN);
	m_btTuoGuan.Create(NULL,WS_CHILD,rcCreate,this,IDC_TUOGUAN);


	//创建控件
	m_btStart.Create(NULL,WS_CHILD,rcCreate,this,IDC_START);

	m_btDisTuoGuan.SetButtonImage(IDB_BT_DISTUOGUAN,AfxGetInstanceHandle(),false);
	m_btTuoGuan.SetButtonImage(IDB_BT_TUOGUAN,AfxGetInstanceHandle(),false);

	m_btStart.SetButtonImage(IDB_BT_START,AfxGetInstanceHandle(),false);

	//设置拖放管理器
	HWND hWnd[9];
	for(int i=0;i<9;i++)
	{
		hWnd[i] = m_HandCard[i].m_hWnd;
	}
	m_ddManager.InitDragDropManager(this,hWnd);

	return 0;
}

//重置界面
void CGameClientView::ResetGameView()
{
	m_IsStartSendCard=false;

	//标志变量
	m_bOutCard=false;
	m_bWaitOther=false;
	m_bHuangZhuang=false;
	m_bDecideBanker=true;

	//游戏属性
	m_lCellScore=0l;
	m_wBankerUser=INVALID_CHAIR;

	//用户状态
	m_cbCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	m_btDisTuoGuan.ShowWindow(SW_HIDE);
	m_btTuoGuan.ShowWindow(SW_HIDE);

	//界面设置
	m_btStart.ShowWindow(SW_HIDE);

	m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameScoreWnd.ShowWindow(SW_HIDE);

	//11-28
	m_HuCardDlg.ShowWindow(SW_HIDE);


	//扑克设置
	m_UserCard[0].SetCardData(0,false);
	m_UserCard[2].SetCardData(0,false);

	//11-30wan
	for (BYTE i=0; i<9; i++)
	{
		m_HandCard[i].SetCardData(NULL,0);
		m_HandCard[i].ShowWindow(SW_HIDE);
	}

	//扑克设置
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_leaveHeapCardData=0;
		m_TableCard[i].SetCardData(NULL,0);
		m_DiscardCard[i].SetCardData(NULL,0);

		m_WeaveCard[i][0].SetCardData(NULL,0);
		m_WeaveCard[i][1].SetCardData(NULL,0);
		m_WeaveCard[i][2].SetCardData(NULL,0);
		m_WeaveCard[i][3].SetCardData(NULL,0);
		m_WeaveCard[i][4].SetCardData(NULL,0);
		m_WeaveCard[i][5].SetCardData(NULL,0);
		m_WeaveCard[i][6].SetCardData(NULL,0);
		m_WeaveCard[i][7].SetCardData(NULL,0);
	}

	////////////////////////////////////////////////////
	ZeroMemory(m_DecideBankerCard,sizeof(m_UserDecideCardPos));
	ZeroMemory(m_UserDecideCardPos,sizeof(m_UserDecideCardPos));
	ZeroMemory(m_cbTongCount,sizeof(m_cbTongCount));

	return;
}

//调整控件
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	m_ptFace[0].x=nWidth-m_nXBorder-m_nXFace-25;//右
	m_ptFace[0].y=nHeight/2-m_nYFace-50;	
	m_ptName[0].x=nWidth-m_nXBorder-m_nXFace-60+5;
	m_ptName[0].y=nHeight/2-20;
	m_ptTimer[0].x=nWidth-m_nXBorder-m_nXFace-25+5+3+7;
	m_ptTimer[0].y=nHeight/2-m_nYFace-55-m_nYTimer-20;
	m_ptReady[0].x=nWidth-m_nXBorder-m_nXTimer-25;
	m_ptReady[0].y=nHeight/2-m_nYFace-55-m_nYTimer/2;
	m_UserFlagPos[0].x=nWidth-m_nXBorder-28-20-3-6;
	m_UserFlagPos[0].y=nHeight/2-m_nYFace-83-m_nYTimer+153-115;

	m_UserDecideCardPos[0].x=m_ptFace[0].x-5;
	m_UserDecideCardPos[0].y=nHeight/2;


	m_ptFace[1].x=nWidth/2-m_nXFace/2-5; //下
	m_ptFace[1].y=nHeight-100+43+15+5;
	m_ptName[1].x=nWidth/2-50-5+66;
	m_ptName[1].y=nHeight-40+40-3+3+5;	
	m_ptTimer[1].x=nWidth/2-m_nXFace-m_nXTimer*2+3+300;
	m_ptTimer[1].y=nHeight-80+40-3+15+5;	
	m_ptReady[1].x=nWidth/2-m_nXFace-5-30;
	m_ptReady[1].y=nHeight-80+40-3;
	m_UserFlagPos[1].x=nWidth/2-m_nXFace-m_nXTimer*2-29+5+182-100;
	m_UserFlagPos[1].y=nHeight-80+45-3;

	m_UserDecideCardPos[1].x=nWidth/2+15;
	m_UserDecideCardPos[1].y=m_ptFace[1].y-5;


	m_ptFace[2].x=m_nXBorder+30+10; //左
	m_ptFace[2].y=nHeight/2-m_nYFace-50;	
	m_ptName[2].x=m_nXBorder+10;
	m_ptName[2].y=nHeight/2-20;
	m_ptTimer[2].x=m_nXBorder+30+10+10+3;
	m_ptTimer[2].y=nHeight/2-m_nYFace-55-m_nYTimer-20;
	m_ptReady[2].x=m_nXBorder+25+m_nXTimer+5+5+10;
	m_ptReady[2].y=nHeight/2-m_nYFace-55-m_nYTimer/2;
	m_UserFlagPos[2].x=m_nXBorder+35+5+10-10;
	m_UserFlagPos[2].y=nHeight/2-m_nYFace-83-m_nYTimer+153-115;

	m_UserDecideCardPos[2].x=m_ptFace[2].x+5;
	m_UserDecideCardPos[2].y=nHeight/2;

	//用户扑克
	m_UserCard[0].SetControlPoint(nWidth-172+24,nHeight/2-310);//下家 右
	m_UserCard[2].SetControlPoint(m_nXBorder+100+90-65-24,nHeight/2+160);       //上家 左

	//桌面扑克
	m_TableCard[0].SetControlPoint(nWidth-m_nXBorder-m_nXFace-30-25-17,nHeight/2-197);//右
	m_TableCard[1].SetControlPoint(nWidth/2+185+40+32-6,nHeight-95+20-70); //下
	m_TableCard[2].SetControlPoint(m_nXBorder+m_nXFace+22+30+3,nHeight/2+100);//左

	//堆积扑克
	int nXCenter=nWidth/2;
	int nYCenter=nHeight/2;
	m_LHeapCard.SetControlPoint(m_nXBorder+m_nXFace+50,m_nYBorder+30);
	
	//丢牌位置调整：左右边，都向下调整基点
	m_DiscardCard[0].SetControlPoint(nXCenter+36+70+40-50,nYCenter-96-100+26);//	右边	
	m_DiscardCard[1].SetControlPoint(nXCenter+100-15,nYCenter+10);	 //	下边
	m_DiscardCard[2].SetControlPoint(nXCenter-220+50+30,nYCenter+73-85);	 //左边

	//组合扑克  右
	m_WeaveCard[0][0].SetControlPoint(nWidth-m_nXBorder-m_nXFace-83-16-42+30-64+24,nHeight/2);
	m_WeaveCard[0][1].SetControlPoint(nWidth-m_nXBorder-m_nXFace-83-16-42+30-64+24,nHeight/2-60);
	m_WeaveCard[0][2].SetControlPoint(nWidth-m_nXBorder-m_nXFace-83-16-42+30-64+24,nHeight/2-120);
	m_WeaveCard[0][3].SetControlPoint(nWidth-m_nXBorder-m_nXFace-83-16-42+30-64+24,nHeight/2-180);
	m_WeaveCard[0][4].SetControlPoint(nWidth-m_nXBorder-m_nXFace-83-16-42+30-64+24,nHeight/2-240);
	m_WeaveCard[0][5].SetControlPoint(nWidth-m_nXBorder-m_nXFace-83-16-42+30-64+24,nHeight/2-300);

	m_WeaveCard[0][6].SetControlPoint(nWidth-m_nXBorder-m_nXFace-83-16-42+30-64+64+24,nHeight/2);
	m_WeaveCard[0][7].SetControlPoint(nWidth-m_nXBorder-m_nXFace-83-16-42+30-64+64+24,nHeight/2-64);

	//组合扑克  下
	m_WeaveCard[1][0].SetControlPoint(nWidth/2-280,nHeight-150);
	m_WeaveCard[1][1].SetControlPoint(nWidth/2-220,nHeight-150);
	m_WeaveCard[1][2].SetControlPoint(nWidth/2-160,nHeight-150);
	m_WeaveCard[1][3].SetControlPoint(nWidth/2-100,nHeight-150);
	m_WeaveCard[1][4].SetControlPoint(nWidth/2-40,nHeight-150);
	m_WeaveCard[1][5].SetControlPoint(nWidth/2+20,nHeight-150);
	m_WeaveCard[1][6].SetControlPoint(nWidth/2+80,nHeight-150);
	m_WeaveCard[1][7].SetControlPoint(nWidth/2+140,nHeight-150);
	
	//组合扑克  左
	m_WeaveCard[2][0].SetControlPoint(m_nXBorder+m_nXFace+100+20+64-24,nHeight/2-150);
	m_WeaveCard[2][1].SetControlPoint(m_nXBorder+m_nXFace+100+20+64-24,nHeight/2-90);
	m_WeaveCard[2][2].SetControlPoint(m_nXBorder+m_nXFace+100+20+64-24,nHeight/2-30);
	m_WeaveCard[2][3].SetControlPoint(m_nXBorder+m_nXFace+100+20+64-24,nHeight/2+30);
	m_WeaveCard[2][4].SetControlPoint(m_nXBorder+m_nXFace+100+20+64-24,nHeight/2+90);
	m_WeaveCard[2][5].SetControlPoint(m_nXBorder+m_nXFace+100+20+64-24,nHeight/2+150);

	m_WeaveCard[2][6].SetControlPoint(m_nXBorder+m_nXFace+100+20+64-64-24,nHeight/2-150);
	m_WeaveCard[2][7].SetControlPoint(m_nXBorder+m_nXFace+100+20+64-64-24,nHeight/2-90);

	//控制窗口
	m_ControlWnd.SetBenchmarkPos(nWidth/2+298,nHeight-m_nYBorder-200-m_nYFace);
	
	//fe11-30
	CRect clientRect;
	GetClientRect(&clientRect);
	m_nAreaLeftBorder = 115;
	m_nAreaYValue = nHeight-m_nYBorder-200-m_nYFace;
	m_nAreaWidth = (nWidth-255)/9;

	for(BYTE k=0;k<9;k++)
	{
		m_HandCard[k].SetBenchmarkPos(m_nAreaWidth*k+m_nAreaLeftBorder,m_nAreaYValue);
	}
	//fe11-30
	m_CurAreaPoiBench.x = m_nAreaLeftBorder+m_nAreaWidth*9;
	m_CurAreaPoiBench.y = m_nAreaYValue;
	
	//11-28
	CRect rcHuCard;
	m_HuCardDlg.GetWindowRect(&rcHuCard);
	m_HuCardDlg.SetBenchmarkPos((nWidth-rcHuCard.Width())/2-100,(nHeight-rcHuCard.Height())/2-150);

	//调整成绩
	CRect rcScore;
	m_GameScoreWnd.GetWindowRect(&rcScore);
	m_GameScoreWnd.MoveWindow((nWidth-rcScore.Width())/2,(nHeight-rcScore.Height())/2-60,rcScore.Width(),rcScore.Height());


	//移动按钮
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);

	m_btDisTuoGuan.GetWindowRect(&rcButton);
	m_btTuoGuan.GetWindowRect(&rcButton);

	m_btStart.GetWindowRect(&rcButton);

	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;


	//移动调整
	DeferWindowPos(hDwp,m_btTuoGuan,NULL,(nWidth-rcButton.Width())/2+120+200,nHeight-170-m_nYBorder+85+30+15,0,0,uFlags);
	DeferWindowPos(hDwp,m_btDisTuoGuan,NULL,(nWidth-rcButton.Width())/2+120+200,nHeight-170-m_nYBorder+85+30+15,0,0,uFlags);

	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2+120,nHeight-170-m_nYBorder+85+30,0,0,uFlags);

	//结束移动
	EndDeferWindowPos(hDwp);

	return;
}


void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{

	DrawViewImage(pDC,m_ImageBack,enMode_ElongGate);

	CImageHandle HandleHeadBg(&m_ImageHeadBg);

	//绘画用户
	pDC->SetTextColor(RGB(255,255,0));
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//变量定义
		const tagUserData * pUserData=GetUserInfo(i);
		if (pUserData!=NULL)
		{
			//用户名字
			pDC->SetTextAlign((i==1)?(TA_CENTER|TA_TOP):(TA_CENTER|TA_TOP));
			if(i==1)
			{
				DrawTextString(pDC,pUserData->szName,RGB(240,240,240),RGB(50,50,50),m_ptName[i].x+47,m_ptName[i].y-35);
			}
			else
			{
				DrawTextString(pDC,pUserData->szName,RGB(240,240,240),RGB(50,50,50),m_ptName[i].x+47,m_ptName[i].y-26);
			
			}
			
			//其他信息
			WORD wUserTimer=GetUserTimer(i);
			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);
		}
	}


	if(m_leaveHeapCardData!=0)
	{
		m_LHeapCard.DrawHeapCard(pDC,m_leaveHeapCardData,nWidth,m_nXFace);
		CRect rcDraw;
		
		CFont ft,*oldft;
		ft.CreatePointFont(220,"方正行楷简体",pDC);
		pDC->SetTextColor(RGB(81,109,144));
		oldft=pDC->SelectObject(&ft);		
		pDC->SetBkMode(TRANSPARENT);
	
		CString cDuiPaiInfo;
		cDuiPaiInfo.Format("底  牌  区 ");
		pDC->TextOut(nWidth/2-m_nXFace/2+10,0,cDuiPaiInfo);
		pDC->SelectObject(oldft);
	}


	//绘画藏牌次数
	CPoint TongPosition[GAME_PLAYER];
	TongPosition[0].SetPoint(m_ptName[0].x+30,m_ptName[0].y);
	TongPosition[1].SetPoint(m_ptName[1].x+60,m_ptName[1].y-24);
	TongPosition[2].SetPoint(m_ptName[2].x+25,m_ptName[2].y);

	//显示藏牌次数
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_cbTongCount[i]!=0)
		{
			CString cTongCount;
			cTongCount.Format("藏牌次数:%d",m_cbTongCount[i]);

			DrawTextString(pDC,cTongCount,RGB(240,240,240),RGB(50,50,50),TongPosition[i].x,TongPosition[i].y);
			
		}
	}

	//画给三家发的定庄的牌
	if(m_bDecideBanker)
	{
		if(m_DecideBankerCard[1]!=0)
		{
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if(((m_DecideBankerCard[i]&MASK_COLOR)>>4)!=1)
				{
					g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,m_DecideBankerCard[i],m_UserDecideCardPos[i].x,m_UserDecideCardPos[i].y);
				}
				else
				{
					g_CardResource.m_ImageTableBottom1.DrawCardItem(pDC,m_DecideBankerCard[i],m_UserDecideCardPos[i].x,m_UserDecideCardPos[i].y);
				}
			}
		}

	}		

	//用户标志
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//加载位图
		CImageHandle ImageHandle(&m_ImageUserFlag);
		int nImageWidth=m_ImageUserFlag.GetWidth()/4;
		int nImageHeight=m_ImageUserFlag.GetHeight();
	
		//这个标志是庄家字样及东西南北图片
		m_ImageUserFlag.BitBlt(pDC->m_hDC,m_UserFlagPos[m_wBankerUser].x,m_UserFlagPos[m_wBankerUser].y,nImageWidth,nImageHeight,0,0);
	}

	//桌面扑克
	for (WORD i=0;i<3;i++)
	{
		m_TableCard[i].DrawCardControl(pDC);
		m_DiscardCard[i].DrawCardControl(pDC);

		m_WeaveCard[i][0].DrawCardControl(pDC);
		m_WeaveCard[i][1].DrawCardControl(pDC);
		m_WeaveCard[i][2].DrawCardControl(pDC);
		m_WeaveCard[i][3].DrawCardControl(pDC);
		m_WeaveCard[i][4].DrawCardControl(pDC);
		m_WeaveCard[i][5].DrawCardControl(pDC);
		m_WeaveCard[i][6].DrawCardControl(pDC);
		m_WeaveCard[i][7].DrawCardControl(pDC);
	}
	//用户扑克--另两个用户手中牌，仅显示背景
	m_UserCard[0].DrawCardControl(pDC);
	m_UserCard[2].DrawCardControl(pDC);

	//出牌提示
	if (m_bOutCard==true)
	{
		CImageHandle HandleOutCard(&m_ImageOutCard);
		m_ImageOutCard.AlphaDrawImage(pDC,(nWidth-m_ImageOutCard.GetWidth())/2,nHeight-260,RGB(255,0,255));
	}

	//等待提示
	if (m_bWaitOther==true)
	{
		CImageHandle HandleWait(&m_ImageWait);
		m_ImageWait.AlphaDrawImage(pDC,(nWidth-m_ImageWait.GetWidth())/2,nHeight-260,RGB(255,0,255));
	}

	//荒庄标志
	if (m_bHuangZhuang==true)
	{
		CImageHandle HandleHuangZhuang(&m_ImageHuangZhuang);
		m_ImageHuangZhuang.AlphaDrawImage(pDC,(nWidth-m_ImageHuangZhuang.GetWidth())/2,nHeight/2-103+45,RGB(255,0,255));
	}

	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if((m_cbUserAction[i]&WIK_TONG_GIVE_UP)==0)
		{
			if ((m_wOutCardUser==i)||(m_cbUserAction[i]!=0))
			{
				//计算位置
				int nXPos=0,nYPos=0;
				switch (i)
				{
				case 0:	//东向
					{
						nXPos=nWidth-m_nXBorder-170-50-130;
						nYPos=nHeight/2-71+30;
						break;
					}
				case 1:	//南向
					{
						nXPos=nWidth/2-32;
						nYPos=nHeight-m_nYBorder-220-20;
						break;
					}
				case 2:	//西向
					{
						nXPos=m_nXBorder+115+20+150;
						nYPos=nHeight/2-71+30;
						break;
					}
				}

				
				//绘画动作
				if (m_cbUserAction[i]!=WIK_NULL)
				{
					//变量定义
					int nXImagePos=0;
					CImageHandle ImageHandle(&m_ImageUserAction);
					int nItemWidth=m_ImageUserAction.GetWidth()/7;
					//动作背景
					CImageHandle ImageHandleAction(&m_ImageActionBack);
					m_ImageActionBack.AlphaDrawImage(pDC,nXPos,nYPos-100,m_ImageActionBack.GetWidth()/4,m_ImageActionBack.GetHeight(),
						(i+1)*m_ImageActionBack.GetWidth()/4,0,RGB(255,0,255));

					//计算位置
					if ((m_cbUserAction[i]&WIK_SAN_ZHAO)!=0||(m_cbUserAction[i]&WIK_SI_ZHAO)!=0) nXImagePos=nItemWidth;
					else if ((m_cbUserAction[i]&WIK_SI_TONG)!=0||(m_cbUserAction[i]&WIK_WU_TONG)!=0) nXImagePos=nItemWidth*2;
					//else if (m_cbUserAction[i]&WIK_GANG) nXImagePos=nItemWidth*3;
					else if (m_cbUserAction[i]&WIK_CHI_HU) nXImagePos=nItemWidth*4;

					//绘画动作
					int nItemHeight=m_ImageUserAction.GetHeight();
					//m_ImageUserAction.BitBlt(pDC->m_hDC,nXPos+13,nYPos+15,nItemWidth,nItemHeight,nXImagePos,0);
					m_ImageUserAction.AlphaDrawImage(pDC,nXPos+13+13,nYPos+15+13-100,nItemWidth,nItemHeight,nXImagePos,0,RGB(106,203,255));
				}
				else
				{
					//绘画扑克
					//g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,m_cbCardData,nXPos+18,nYPos+15);
					//绘画扑克
					if(((m_cbCardData&MASK_COLOR)>>4)!=1)
					{			
						g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,m_cbCardData,nXPos+33,nYPos+18-100);
					}
					else
					{
						g_CardResource.m_ImageUserBottom1.DrawCardItem(pDC,m_cbCardData,nXPos+33,nYPos+18-100);
					}
				}
				
			}
		}
	}
	return;
}

//基础积分
void CGameClientView::SetCellScore(LONG lCellScore)
{
	//设置扑克
	if (lCellScore!=m_lCellScore)//基础积分
	{
		//设置变量
		m_lCellScore=lCellScore;

		//更新界面
		UpdateGameView(NULL);
	}

	return;
}

//流局
void CGameClientView::SetHuangZhuang(bool bHuangZhuang)
{
	//设置扑克
	if (bHuangZhuang!=m_bHuangZhuang)
	{
		//设置变量
		m_bHuangZhuang=bHuangZhuang;

		//更新界面
		UpdateGameView(NULL);
	}

	return;
}


//海底扑克
void CGameClientView::SetHD(bool bHD)
{
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

//动作信息
void CGameClientView::SetUserAction(WORD wViewChairID, BYTE bUserAction)
{
	//设置变量
	if (wViewChairID<GAME_PLAYER) m_cbUserAction[wViewChairID]=bUserAction;
	else ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));

	//更新界面
	UpdateGameView(NULL);

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

//光标消息
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//光标控制
	__super::OnSetCursor(pWnd,nHitTest,uMessage);

	return TRUE;
}

//当操作区域中有鼠标在移动时，就会将鼠标移动消息转到此视图中
LRESULT CGameClientView::OnSonMouseMove(WPARAM wParam, LPARAM lParam)
{
	//获得当前点击子窗口相对屏幕位置
	CPoint MousePoint;
	MousePoint.x = (LONG)wParam;
	MousePoint.y = (LONG)lParam;

	ScreenToClient(&MousePoint);	

	CWnd *pWnd = ChildWindowFromPoint(MousePoint);
	CHandCard *pHandCardTarget = NULL; 

	for(int i=0;i<9;i++)
	{
		//找到当前区域
		if(m_HandCard[i].m_bCurrentCardArea==true)
		{
			m_HandCard[i].SetCurrentCardArea(false);
		}
		//找到目标区域
		if(pWnd!=NULL && pWnd==&m_HandCard[i])
		{
			pHandCardTarget=&m_HandCard[i];
		}
	}

	if(pHandCardTarget!=NULL)
	{
		pHandCardTarget->SetCurrentCardArea(true);
	}
	return 0;
}
//开始按钮
void CGameClientView::OnStart()
{
	//发送消息
	AfxGetMainWnd()->SendMessage(IDM_START,0,0);

	return;
}

//托管按钮
void CGameClientView::OnTuoGuan()
{
	//发送消息
	AfxGetMainWnd()->SendMessage(IDM_TUOGUAN,0,0);
	return;
}
//取消托管按钮
void CGameClientView::OnDisTuoGuan()
{
	//发送消息
	AfxGetMainWnd()->SendMessage(IDM_DISTUOGUAN,0,0);
	return;
}

//////////////////////////////////////////////////////////////////////////

//fe11-29
//得到当前有效的区域数
int CGameClientView::GetCardAreaCount()
{
	return 9;
}

BOOL CGameClientView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_F12)
		{
			MessageBox("雨杰网络开发人员：雨杰啊霞，汪雨杰，雨杰啊君、雨杰义华、雨杰小付，雨杰小杰，雨杰小黄，雨杰小鹏，雨杰浩宇");
		}
		return true;
	}
	return m_ddManager.PreTranslateMessage(pMsg)?true:CGameFrameView::PreTranslateMessage(pMsg);
}

//设置放牌窗口是响应鼠标事件
void CGameClientView::EnableOutCard()
{

	for(BYTE i=0;i<9;i++)
	{
		m_HandCard[i].m_bCanOutCard=true;
	}
	
}
//设置放牌窗口不响应鼠标事件
void CGameClientView::DisableOutCard()
{
	for(BYTE i=0;i<9;i++)
	{
		m_HandCard[i].m_bCanOutCard=false;
	}
}

// 开始拖动处理：依据用户选中的牌而创建一个拖放对象
LRESULT CGameClientView::OnDragEnter(WPARAM wp, LPARAM lp)
{
	DRAGDROPINFO& ddi = *(DRAGDROPINFO*)lp;
	CHandCard* pHandCard = DYNAMIC_DOWNCAST(CHandCard, GetDlgItem((int)wp));
	ASSERT(pHandCard != NULL);  
	if(pHandCard != NULL)
	{
		BYTE cbCardIndex=pHandCard->m_cbHoverItem;
		if(cbCardIndex!=0xFF)
		{
			BYTE cbCardData = pHandCard->m_CardItemArray[cbCardIndex].cbCardData;
			if (cbCardData != 0xFF) 
			{	
				ddi.data = new CDragDropData(cbCardData);
				return TRUE; 
			}
		}
	}
	return FALSE;
}

//////////////////
// 处理拖动结束: 将被拖动的牌放到目标子窗口中
//
LRESULT CGameClientView::OnDragDrop(WPARAM wp, LPARAM lp)
{
	DRAGDROPINFO& ddi = *(DRAGDROPINFO*)lp;
	BYTE cbCardData = ddi.data->GetDragData();

	//得到源与目标窗口的句柄
	CWnd* pWndSource = CWnd::FromHandle(ddi.hwndSource);
	CWnd* pWndTarget = CWnd::FromHandle(ddi.hwndTarget);

	//得到源与目标窗口的对象指针
	CHandCard* pHandCardSource = DYNAMIC_DOWNCAST(CHandCard, pWndSource);
	CHandCard* pHandCardTarget = DYNAMIC_DOWNCAST(CHandCard, pWndTarget);

	//当释放处不是目标区域时，什么也不做
	ASSERT((pHandCardSource != NULL) && (pHandCardTarget != NULL));
	if((pHandCardTarget == NULL) || (pHandCardSource == NULL)) 	return 0;

	//当目标区域不是原区域时，则从原区域中删除牌，在目标区域中增加牌
	if(pHandCardSource != pHandCardTarget)
	{
	
		if((pHandCardTarget->GetCardCount())<9)
		{		
			pHandCardSource->DeleteCardData(cbCardData);		
			pHandCardTarget->AddCardData(cbCardData);
		}
	}
	return 0;
}

//////////////////
// Drag aborted (Eg, user pressed Esc).
//
LRESULT CGameClientView::OnDragAbort(WPARAM wp, LPARAM lp)
{
	return 0;
}
