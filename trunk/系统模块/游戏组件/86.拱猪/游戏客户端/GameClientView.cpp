#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameLogic.h"
#include ".\gameclientview.h"

//////////////////////////////////////////////////////////////////////////
//按钮标识 

#define IDC_START						100								//开始按钮
#define IDC_OUT_CARD					101								//出牌按钮
#define IDC_SHOW_CARD                   102	                            //亮牌按钮
#define IDC_NOT_SHOW_CARD               103	                            //不亮按钮
#define IDC_AUTO_OUT_CARD               104                             //自动代打
#define IDC_CANCEL_AUTO_OUT_CARD		105                             //取消代打
//////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_OUT_CARD, OnOutCard)
	ON_BN_CLICKED(IDC_SHOW_CARD, OnShowCard)
	ON_BN_CLICKED(IDC_NOT_SHOW_CARD, OnNotShowCard)
    ON_BN_CLICKED(IDC_AUTO_OUT_CARD, OnAutoOutCard)
	ON_BN_CLICKED(IDC_CANCEL_AUTO_OUT_CARD, OnCancelAutoOutCard)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientView::CGameClientView(void) : CGameFrameView(true,24)
{
	//游戏变量
    ZeroMemory(m_bScoreCard,sizeof(m_bScoreCard));
	ZeroMemory(m_bScoreCardCount , sizeof(m_bScoreCardCount)); 
	//亮牌玩家
	ZeroMemory(m_bShowCard , sizeof(m_bShowCard)) ;
	ZeroMemory(m_bShowCardCount , sizeof(m_bShowCardCount)) ;

	//配置变量
	m_bDeasilOrder = false;					            	//出牌顺序

	//状态变量
    m_bSendWaiMsg = false ;                                 //是否举手
    ZeroMemory(m_bCardCount,sizeof(m_bCardCount));
	m_bShowCardHint=false ;

	//加载资源
	HINSTANCE hInstance=AfxGetInstanceHandle();	
	m_ImageBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageBackGround.SetLoadInfo(IDB_BACK_GROUND,hInstance);

	m_ImageScoreLeft.SetLoadInfo(IDB_SHOW_SCORE_LEFT,hInstance);	
	m_ImageScoreMid.SetLoadInfo(IDB_SHOW_SCORE_MID,hInstance);	
	m_ImageScoreRight.SetLoadInfo(IDB_SHOW_SCORE_RIGHT,hInstance);	
	m_ImageScoreSeparator.SetLoadInfo(IDB_SHOW_SCORE_SEPARATOR,hInstance);


	m_ImageScore[0].SetLoadInfo(IDB_HEARTS_2,hInstance); 
	m_ImageScore[1].SetLoadInfo(IDB_HEARTS_3,hInstance);                     
	m_ImageScore[2].SetLoadInfo(IDB_HEARTS_4,hInstance);                     
	m_ImageScore[3].SetLoadInfo(IDB_HEARTS_5,hInstance);                     
	m_ImageScore[4].SetLoadInfo(IDB_HEARTS_6,hInstance);                     
	m_ImageScore[5].SetLoadInfo(IDB_HEARTS_7,hInstance);                     
	m_ImageScore[6].SetLoadInfo(IDB_HEARTS_8,hInstance);                     
	m_ImageScore[7].SetLoadInfo(IDB_HEARTS_9,hInstance);                     
	m_ImageScore[8].SetLoadInfo(IDB_HEARTS_10,hInstance);                    
	m_ImageScore[9].SetLoadInfo(IDB_HEARTS_J,hInstance);                     
	m_ImageScore[10].SetLoadInfo(IDB_HEARTS_Q,hInstance);                     
	m_ImageScore[11].SetLoadInfo(IDB_HEARTS_K,hInstance);                     
	m_ImageScore[12].SetLoadInfo(IDB_HEARTS_A,hInstance);                     
	m_ImageScore[13].SetLoadInfo(IDB_DIAMONDS_J,hInstance);                   
	m_ImageScore[14].SetLoadInfo(IDB_SPADE_Q,hInstance);  
	m_ImageScore[15].SetLoadInfo(IDB_CLUBS_10,hInstance);                    

	//读取配置
	m_bDeasilOrder=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),FALSE)?true:false;

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
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_UserCardControl[i].SetDirection(true);
		m_UserCardControl[i].SetDisplayFlag(true);
		m_UserCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,20+i);
	}
	m_HandCardControl.SetSinkWindow(AfxGetMainWnd());
	m_HandCardControl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,40);

	//创建按钮
	m_btStart.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_START);
	m_btOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_OUT_CARD);
	m_btShowCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_SHOW_CARD);
	m_btNotShowCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_NOT_SHOW_CARD);
	m_btAutoOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_AUTO_OUT_CARD);
	m_btCancelAutoOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_CANCEL_AUTO_OUT_CARD); 


	//设置按钮
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_BT_START,hInstance,false);
	m_btOutCard.SetButtonImage(IDB_BT_OUT_CARD,hInstance,false);
	m_btShowCard.SetButtonImage(IDB_BT_SHOW_CARD,hInstance,false);
	m_btNotShowCard.SetButtonImage(IDB_BT_NOT_SHOW_CARD,hInstance,false);
    m_btAutoOutCard.SetButtonImage(IDB_BT_AUTOMATISM_OUTCARD,hInstance,false);
	m_btCancelAutoOutCard.SetButtonImage(IDB_CANCEL_AUTOMATISM_OUTCARD,hInstance,false);

	m_btOutCard.ShowWindow(SW_HIDE) ;
	m_btShowCard.ShowWindow(SW_HIDE) ;
	m_btNotShowCard.ShowWindow(SW_HIDE) ;
	m_btAutoOutCard.ShowWindow(SW_HIDE) ;
	m_btCancelAutoOutCard.ShowWindow(SW_HIDE) ;

	return 0;
}

//重置界面
void CGameClientView::ResetGameView()
{
	//游戏变量
	ZeroMemory(m_bScoreCard,sizeof(m_bScoreCard));
	ZeroMemory(m_bScoreCardCount , sizeof(m_bScoreCardCount)); 
	//亮牌玩家
	ZeroMemory(m_bShowCard , sizeof(m_bShowCard)) ;
	ZeroMemory(m_bShowCardCount , sizeof(m_bShowCardCount)) ;


	//配置变量
	m_bDeasilOrder        = false ;

	//状态变量
	m_bSendWaiMsg         = false ;
	ZeroMemory(m_bCardCount , sizeof(m_bCardCount)) ;
	m_bShowCardHint=false ;

	//隐藏控件
	m_btStart.ShowWindow(SW_HIDE) ;
	m_btOutCard.ShowWindow(SW_HIDE) ;

	//隐藏控件
	m_ScoreView.ShowWindow(SW_HIDE);

	//设置扑克
	for(WORD i=0 ; i<GAME_PLAYER ; ++i) 
	{
		m_UserCardControl[i].SetCardData(NULL , 0);
	}
	m_HandCardControl.SetCardData(NULL,0);
	m_HandCardControl.SetPositively(false);
	m_HandCardControl.SetDisplayFlag(false);

	return;
}

//调整控件
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//设置坐标
	if (m_bDeasilOrder==true)
	{
		m_ptFace[1].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptFace[1].y=nHeight/2-m_nYFace-60;
		m_ptName[1].x=nWidth-m_nXBorder-10;
		m_ptName[1].y=nHeight/2-m_nYFace-20;
		m_ptTimer[1].x=nWidth-m_nXBorder-m_nXFace-20;
		m_ptTimer[1].y=nHeight/2-m_nYFace-m_nYTimer-65;
		m_ptReady[1].x=nWidth-m_nXBorder-m_nXTimer-5;
		m_ptReady[1].y=nHeight/2-m_nYFace-m_nYTimer/2-65;

		m_ptFace[3].x=m_nXBorder+30;
		m_ptFace[3].y=nHeight/2-m_nYFace-60;
		m_ptName[3].x=m_nXBorder+m_nXFace;
		m_ptName[3].y=nHeight/2-m_nYFace-20;
		m_ptTimer[3].x=m_nXBorder+20;
		m_ptTimer[3].y=nHeight/2-m_nYFace-5-m_nYTimer-60;
		m_ptReady[3].x=m_nXBorder+20+m_nXTimer;
		m_ptReady[3].y=nHeight/2-m_nYFace-m_nYTimer/2-65;
	}
	else
	{
		m_ptFace[3].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptFace[3].y=nHeight/2-m_nYFace-60;
		m_ptName[3].x=nWidth-m_nXBorder-10;
		m_ptName[3].y=nHeight/2-m_nYFace-20;
		m_ptTimer[3].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptTimer[3].y=nHeight/2-m_nYFace-m_nYTimer-65;
		m_ptReady[3].x=nWidth-m_nXBorder-m_nXTimer-5;
		m_ptReady[3].y=nHeight/2-m_nYFace-m_nYTimer/2-65;

		m_ptFace[1].x=m_nXBorder+30;
		m_ptFace[1].y=nHeight/2-m_nYFace-60;
		m_ptName[1].x=m_nXBorder+m_nXFace;
		m_ptName[1].y=nHeight/2-m_nYFace-20;
		m_ptTimer[1].x=m_nXBorder+20;
		m_ptTimer[1].y=nHeight/2-m_nYFace-5-m_nYTimer-60;
		m_ptReady[1].x=m_nXBorder+20+m_nXTimer;
		m_ptReady[1].y=nHeight/2-m_nYFace-m_nYTimer/2-65;
	}

	m_ptFace[0].x=nWidth/2-m_nXFace/2;
	m_ptFace[0].y=m_nYBorder+20;
	m_ptName[0].x=nWidth/2+5+m_nXFace/2;
	m_ptName[0].y=m_nYBorder+20;
	m_ptTimer[0].x=nWidth/2-m_nXFace/2-m_nXTimer*2-5;
	m_ptTimer[0].y=m_nYBorder+25;
	m_ptReady[0].x=nWidth/2-m_nXTimer-m_nXFace/2-5;
	m_ptReady[0].y=m_nYBorder+m_nYTimer/2+25;

	m_ptFace[2].x=nWidth/2-m_nXFace/2;
	m_ptFace[2].y=nHeight-m_nYBorder-m_nYFace-150;
	m_ptName[2].x=nWidth/2+5+m_nXFace/2;
	m_ptName[2].y=nHeight-m_nYBorder-m_nYFace-140;
	m_ptTimer[2].x=nWidth/2-m_nXFace/2-m_nXTimer*2-5;
	m_ptTimer[2].y=nHeight-m_nYBorder-m_nYTimer-150;
	m_ptReady[2].x=nWidth/2-m_nXTimer-m_nXFace/2-5;
	m_ptReady[2].y=nHeight-m_nYBorder-m_nYTimer/2-150;

	//移动按钮
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(5);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	m_btStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStart,NULL,nWidth-2*rcButton.Width()+30,nHeight-208-m_nYBorder,0,0,uFlags);
//	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-275-m_nYBorder,0,0,uFlags);

	m_btOutCard.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btOutCard,NULL,(nWidth-rcButton.Width())/2,nHeight-330-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btShowCard,NULL,(nWidth)/2-rcButton.Width(),nHeight-340-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btNotShowCard,NULL,(nWidth)/2,nHeight-340-m_nYBorder,0,0,uFlags);
    DeferWindowPos(hDwp,m_btAutoOutCard,NULL,nWidth-2*rcButton.Width(),nHeight-200-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btCancelAutoOutCard,NULL,nWidth-2*rcButton.Width(),nHeight-200-m_nYBorder,0,0,uFlags);
	EndDeferWindowPos(hDwp);

	//移动扑克

	m_HandCardControl.SetBenchmarkPos(nWidth/2 , nHeight-250 , enXCenter , enYCenter);

	//用户扑克
	if (m_bDeasilOrder==true)
	{
		m_UserCardControl[0].SetBenchmarkPos(nWidth/2 , nHeight/2-185 , enXCenter , enYCenter);
		m_UserCardControl[3].SetBenchmarkPos(m_nXBorder+m_nXFace+67 , nHeight/2-80 , enXLeft , enYCenter);
		m_UserCardControl[2].SetBenchmarkPos(nWidth/2 , nHeight-357 , enXCenter , enYBottom);
		m_UserCardControl[1].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-70 , nHeight/2-80 , enXRight , enYCenter);
	}
	else 
	{
		m_UserCardControl[0].SetBenchmarkPos(nWidth/2 , nHeight/2-185 , enXCenter , enYCenter);
		m_UserCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+67 , nHeight/2-80 , enXLeft , enYCenter);
		m_UserCardControl[2].SetBenchmarkPos(nWidth/2 , nHeight-357 , enXCenter , enYBottom);
		m_UserCardControl[3].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-70 , nHeight/2-80 , enXRight , enYCenter);
	}


	//积分视图
	CRect rcControl;
	m_ScoreView.GetWindowRect(&rcControl);
	m_ScoreView.SetWindowPos(NULL,(nWidth-rcControl.Width())/2,(nHeight-rcControl.Height()-100)/2,0,0,SWP_NOZORDER|SWP_NOSIZE);


 return;
}

//绘画界面
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//绘画背景
	CImageHandle HImagBackGround(&m_ImageBackGround);
	DrawViewImage(pDC,m_ImageBackGround,enMode_Spread);
	DrawViewImage(pDC,m_ImageBack,enMode_Centent);
    BYTE bRise=24;
	CRect rect ;
	GetClientRect(&rect) ;

	
	//显示积分框
	int nImageWidth ,nImageHeight;
	int nPosX ,	nPosY ;

	//绘画左边
	CImageHandle HImageScoreLeft(&m_ImageScoreLeft);
	nImageWidth  = m_ImageScoreLeft.GetWidth() ;
	nImageHeight = m_ImageScoreLeft.GetHeight() ;
	nPosX		 = 5 ;
	nPosY		 = rect.bottom-nImageHeight-bRise ;
	m_ImageScoreLeft.BitBlt(pDC->m_hDC , nPosX , nPosY-20 , SRCCOPY);
	

	//绘画中间
	CImageHandle HImageScoreMid(&m_ImageScoreMid);
	nImageWidth  = m_ImageScoreMid.GetWidth() ;
	nImageHeight = m_ImageScoreMid.GetHeight() ;
	nPosX		 = m_ImageScoreLeft.GetWidth()+5 ;
	nPosY		 = rect.bottom-nImageHeight-bRise ;
	m_ImageScoreMid.BitBlt(pDC->m_hDC , nPosX , nPosY-20 , SRCCOPY) ;
	m_ImageScoreMid.BitBlt(pDC->m_hDC , nPosX+nImageWidth , nPosY-20 , SRCCOPY) ;

	

	//绘画分割

	CImageHandle ImageScoreSeparator(&m_ImageScoreSeparator);
	nImageWidth  = m_ImageScoreSeparator.GetWidth() ;
	nImageHeight = m_ImageScoreSeparator.GetHeight() ;
	nPosX		 = m_ImageScoreLeft.GetWidth()+5+90 ;
	nPosY		 = rect.bottom-nImageHeight-bRise ;
	m_ImageScoreSeparator.BitBlt(pDC->m_hDC , nPosX , nPosY-20 , SRCCOPY);


	//绘画中间
	nImageWidth  = m_ImageScoreMid.GetWidth() ;
	nImageHeight = m_ImageScoreMid.GetHeight() ;
	nPosX		 = m_ImageScoreLeft.GetWidth()+5+90+m_ImageScoreSeparator.GetWidth() ;
	nPosY		 = rect.bottom-nImageHeight-bRise ;
	m_ImageScoreMid.BitBlt(pDC->m_hDC , nPosX , nPosY-20 , SRCCOPY) ;
	m_ImageScoreMid.BitBlt(pDC->m_hDC , nPosX+nImageWidth , nPosY-20 , SRCCOPY) ;

	//绘画分割

	nImageWidth  = m_ImageScoreSeparator.GetWidth() ;
	nImageHeight = m_ImageScoreSeparator.GetHeight() ;
	nPosX		 = m_ImageScoreLeft.GetWidth()+5+90+m_ImageScoreSeparator.GetWidth()+120 ;
	nPosY		 = rect.bottom-nImageHeight -bRise;
	m_ImageScoreSeparator.BitBlt(pDC->m_hDC , nPosX , nPosY-20 , SRCCOPY);

	//绘画中间
	CImageHandle ImageScoreRight(&m_ImageScoreRight);
	int nCount	 = (rect.right- m_ImageScoreLeft.GetWidth()+5+90+m_ImageScoreSeparator.GetWidth()*2+120-m_ImageScoreRight.GetWidth())/m_ImageScoreMid.GetWidth()+1 ;
	nImageWidth  = m_ImageScoreMid.GetWidth() ;
	nImageHeight = m_ImageScoreMid.GetHeight() ;
	nPosY		 = rect.bottom-nImageHeight-bRise ;
	for(BYTE i=0 ; i<nCount ; ++i)
	{
		nPosX		 = m_ImageScoreLeft.GetWidth()+5+90+m_ImageScoreSeparator.GetWidth()*2+120+i*nImageWidth;
		m_ImageScoreMid.BitBlt(pDC->m_hDC , nPosX , nPosY-20 , SRCCOPY);
	}

	//绘画右边
	nImageWidth  = m_ImageScoreRight.GetWidth() ;
	nImageHeight = m_ImageScoreRight.GetHeight() ;
	nPosX		 = rect.right-nImageWidth-5 ;
	nPosY		 = rect.bottom-nImageHeight -bRise;
	m_ImageScoreRight.BitBlt(pDC->m_hDC , nPosX , nPosY-20 , SRCCOPY);

	//显示文字
	pDC->SetTextColor(RGB(255,255,255)) ;
	UINT uOldAlign = pDC->SetTextAlign(TA_LEFT|TA_TOP);
	pDC->TextOut(37 ,  rect.bottom-nImageHeight+4-bRise-20 , "用户名" , lstrlen("用户名")) ;
    pDC->TextOut(158 , rect.bottom-nImageHeight+4-bRise-20, "亮牌" ,  lstrlen("亮牌")) ;
    
    CRect DrawRect ;
	DrawRect.left	= m_ImageScoreLeft.GetWidth()+5+90+m_ImageScoreSeparator.GetWidth()+120+6+10;
	DrawRect.top	= rect.bottom - m_ImageScoreSeparator.GetHeight()+4-bRise-20 ;
	DrawRect.bottom = rect.bottom - 97 ;
	DrawRect.right	= rect.right - m_ImageScoreRight.GetWidth() ;
	pDC->DrawText("分牌" , &DrawRect , DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP) ;
	
	pDC->SetTextAlign(uOldAlign);

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


	if(true==m_bShowCardHint) 
	{
		char HintText[]="选择要亮的牌然后点击亮牌按钮" ;
		pDC->SetTextColor(RGB(255 , 255 , 255)) ;
		DrawRect.top	= 20-bRise-20;
		DrawRect.left	= 20 ;
		DrawRect.bottom	= nHeight - 40 ;
		DrawRect.right	= nWidth - 20 ;

		pDC->SetTextAlign(TA_LEFT|TA_TOP) ;
		pDC->DrawText(HintText , lstrlen(HintText), &DrawRect , DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_SINGLELINE) ;

	}

	pDC->SelectObject(def_font);
	font.DeleteObject();	

    CPoint ShowCardPt[GAME_PLAYER] ;        //亮分位置
	CPoint ScorePt[GAME_PLAYER] ;           //得分位置

	for(BYTE i=0 ; i<GAME_PLAYER ; ++i)
	{
		ShowCardPt[i].x  = 120 ;
		ShowCardPt[i].y  = nHeight-21-(3-i)*18 -bRise;
		ScorePt[i].x     = 252 ;
		ScorePt[i].y     = nHeight-21-(3-i)*18 -bRise;
	}
	
	BYTE bCardColor ;
	BYTE bCardValue ;
	BYTE bIndex ;
	CGameLogic GameLogic;
	//绘画亮牌
	for(BYTE i=0 ; i<GAME_PLAYER ; ++i)	
	{
		for(BYTE j=0 ; j<m_bShowCardCount[i] ; ++j)
		{
			//数据验证
			ASSERT(0!=m_bShowCard[i][j]) ;
			if(0==m_bShowCard[i][j]) break ;
			bCardColor = GameLogic.GetCardColor(m_bShowCard[i][j]) ;
			bCardColor = bCardColor>>4 ;
			bCardValue = GameLogic.GetCardValue(m_bShowCard[i][j]) ;
			
			if(1==bCardValue)      bIndex = 12 ;
			else if(0==bCardColor) bIndex = 13 ;
			else if(1==bCardColor) bIndex = 15 ;
			else if(3==bCardColor) bIndex = 14 ;
			else                   bIndex = bCardValue-2 ;
			
			CImageHandle ImageShowCard(&m_ImageScore[bIndex]);
			m_ImageScore[bIndex].AlphaDrawImage(pDC,ShowCardPt[i].x+j*28,ShowCardPt[i].y, RGB(255,255,255));
		}
	}
	//绘画分数 
	for(BYTE i=0 ; i<GAME_PLAYER ; ++i)	
	{
		for(BYTE j=0 ; j<m_bScoreCardCount[i] ; ++j)
		{
			//数据验证
			ASSERT(0!=m_bScoreCard[i][j]) ;
			if(0==m_bScoreCard[i][j]) break ;
			bCardColor = GameLogic.GetCardColor(m_bScoreCard[i][j]) ;
			bCardColor = bCardColor>>4 ;
			bCardValue = GameLogic.GetCardValue(m_bScoreCard[i][j]) ;

			if(1==bCardValue)      bIndex = 12 ;
			else if(0==bCardColor) bIndex = 13 ;
			else if(1==bCardColor) bIndex = 15 ;
			else if(3==bCardColor) bIndex = 14 ;
			else                   bIndex = bCardValue-2 ;
			CImageHandle ImageShowCard(&m_ImageScore[bIndex]);
			m_ImageScore[bIndex].AlphaDrawImage(pDC,ScorePt[i].x+j*28,ShowCardPt[i].y,RGB(255,255,255));
		}
	}

	//绘画用户
	TCHAR szBuffer[32];
	BYTE m_UserReadCount=0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//变量定义
		WORD wUserTimer=GetUserTimer(i);
		const tagUserData * pUserData=GetUserInfo(i);

		//绘画用户
		if (pUserData!=NULL)
		{
			//用户名字
			
			pDC->SetTextColor((wUserTimer>0)?RGB(250,250,250):RGB(220,220,220));
			pDC->SetTextAlign(((i==1)&&(m_bDeasilOrder==true))||((i==3)&&(m_bDeasilOrder==false))?TA_RIGHT:TA_LEFT);
			pDC->TextOut(m_ptName[i].x,m_ptName[i].y,pUserData->szName,lstrlen(pUserData->szName));


			pDC->SetTextAlign(((i==1)&&(m_bDeasilOrder==true))||((i==3)&&(m_bDeasilOrder==false))?TA_RIGHT:TA_LEFT);
			DrawTextString(pDC,pUserData->szName,RGB(240,240,240),RGB(50,50,50),m_ptName[i].x,m_ptName[i].y);

			//亮牌框显示
			pDC->SetTextAlign(TA_LEFT) ;
			pDC->TextOut(50,nHeight-21-(3-i)*18-bRise-20,pUserData->szName,lstrlen(pUserData->szName));

			//剩余扑克
			if (pUserData->cbUserStatus>=US_PLAY)
			{
				pDC->SetTextAlign(((i==1)&&(m_bDeasilOrder==true))||((i==3)&&(m_bDeasilOrder==false))?TA_RIGHT:TA_LEFT);
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("（%ld 张）"),m_bCardCount[i]);
				pDC->TextOut(m_ptName[i].x,m_ptName[i].y+18,szBuffer,lstrlen(szBuffer));
			}
			//举手
			if (pUserData->cbUserStatus==US_READY)
			{
				if(i!=1)
					m_UserReadCount++;                    //一为当前玩家
				DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			}
			else
			{
				if(i!=1)
					m_UserReadCount=0;
			}
			//其他信息

			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);

			DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);
		}
		else
		{
			m_UserReadCount=0;
		}
	}
	//准备好了
	if(m_UserReadCount==(GAME_PLAYER-1) && m_bSendWaiMsg)
	{
		AfxGetMainWnd()->SendMessage(IDM_WAIT_START,0,0);
		m_bSendWaiMsg=false;
	}	

	return;
}

//用户顺序
void CGameClientView::SetUserOrder(bool bDeasilOrder)
{
	//状态判断
	if (m_bDeasilOrder==bDeasilOrder) return;

	//设置变量
	m_bDeasilOrder=bDeasilOrder;
	AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),m_bDeasilOrder?TRUE:FALSE);

	//设置界面
	CRect rcClient;
	GetClientRect(&rcClient);

	RectifyGameView(rcClient.Width(),rcClient.Height());

	//刷新界面
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
	else m_bCardCount[wChairID] = bCardCount;

	//更新界面
	UpdateGameView(NULL);

	return;
}
//////////////////////////////////////////////////////////////////////////

void CGameClientView::OnRButtonDown(UINT nFlags, CPoint point)
{
	AfxGetMainWnd()->SendMessage(IDM_OUT_CARD,1,1);
	CGameFrameView::OnRButtonDown(nFlags, point);
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

//亮牌按钮
void CGameClientView::OnShowCard()
{
	AfxGetMainWnd()->SendMessage(IDM_SHOW_CARD,1,1);
	return;
}

//不亮按钮
void CGameClientView::OnNotShowCard()
{
	AfxGetMainWnd()->SendMessage(IDM_NOT_SHOW_CARD,1,1);
	return;
}

//自动代打
void CGameClientView::OnAutoOutCard() 
{
	AfxGetMainWnd()->SendMessage(IDM_AUTO_OUT_CARD,1,1);
	return;
}

//取消代打
void CGameClientView::OnCancelAutoOutCard() 
{
	AfxGetMainWnd()->SendMessage(IDM_CANCEL_AUTO_OUT_CARD,1,1);
	return;
}


void CGameClientView::OnLButtonDblClk(UINT nFlags, CPoint point)
{

	//设置扑克
	m_btOutCard.EnableWindow(FALSE);
	m_HandCardControl.ShootAllCard(false);

	CGameFrameView::OnLButtonDblClk(nFlags, point);
}

//增加分数
void CGameClientView::AddScoreCard(WORD wChairID , BYTE bCardData[] , BYTE bCardCount) 
{
	//设置变量
	if(INVALID_CHAIR==wChairID)
	{
		ZeroMemory(m_bScoreCard,sizeof(m_bScoreCard));
		ZeroMemory(m_bScoreCardCount , sizeof(m_bScoreCardCount));
		return ;
	}
	CGameLogic GameLogic;
	for(BYTE i=0 ; i<bCardCount ; ++i)  
	{
		m_bScoreCard[wChairID][m_bScoreCardCount[wChairID]+i] = bCardData[i] ;
	}
	m_bScoreCardCount[wChairID] += bCardCount ;
    
	ASSERT(m_bScoreCardCount[wChairID]>0 && m_bScoreCardCount[wChairID] <=16) ;
	if(m_bScoreCardCount[wChairID]<=0 || m_bScoreCardCount[wChairID]>16) return ;

	//排列扑克
	GameLogic.SortCardList(m_bScoreCard[wChairID] , m_bScoreCardCount[wChairID]) ;

	//更新界面
	UpdateGameView(NULL);

	return ;
}


//设置亮牌
void CGameClientView::SetShowCard(WORD wChairID , BYTE bCardData[] , BYTE bCardCount)
{
	//设置变量
	if(INVALID_CHAIR==wChairID)
	{
		ZeroMemory(m_bShowCard , sizeof(m_bShowCard)) ;
		ZeroMemory(m_bShowCardCount , sizeof(m_bShowCardCount)) ;
		return ;
	}
	CGameLogic GameLogic;

	m_bShowCardCount[wChairID] = bCardCount ;
	for(BYTE i=0 ; i<bCardCount ; ++i)	m_bShowCard[wChairID][i] = bCardData[i] ;

	ASSERT(m_bShowCardCount[wChairID]>0 && m_bShowCardCount[wChairID] <=4) ;
	if(m_bShowCardCount[wChairID]<0 || m_bShowCardCount[wChairID]>4) return ;

	//排列扑克
	GameLogic.SortCardList(m_bShowCard[wChairID] , m_bShowCardCount[wChairID]) ;

	//更新界面
	UpdateGameView(NULL);

	return ;
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