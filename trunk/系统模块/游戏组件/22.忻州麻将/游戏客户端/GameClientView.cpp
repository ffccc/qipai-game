#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"

#include "Struct.h"

#include "GameClient.h"

//Ӧ�ó������
extern CGameClientApp theApp;
extern UINT MyThreadProc( LPVOID pParam );
//////////////////////////////////////////////////////////////////////////
//��ť��ʶ 

#define IDC_START						100								//��ʼ��ť
#define IDC_TING_CARD					102									//���ư�ť

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_TING_CARD,OnTingCard)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	xOffInt=0;
	yOffInt=0;
	//��־����
	m_bOutCard=false;
	m_bWaitOther=false;
	m_bHuangZhuang=false;

	//��Ϸ����
	m_lCellScore=0l;
	m_wBankerUser=INVALID_CHAIR;

	m_nDiceCount2= -1;
	m_nDiceCount1= -1;
	//�û�״̬
	m_cbCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageWait.SetLoadInfo(IDB_WAIT_TIP,hInstance);
	m_ImageBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageUserFlag.SetLoadInfo(IDB_USER_FLAG,hInstance);
	m_ImageOutCard.SetLoadInfo(IDB_OUT_CARD_TIP,hInstance);
	m_ImageUserAction.SetLoadInfo(IDB_USER_ACTION,hInstance);
	m_ImageActionBack.SetLoadInfo(IDB_ACTION_BACK,hInstance);
	m_ImageHuangZhuang.SetLoadInfo(IDB_HUANG_ZHUANG,hInstance);

	m_ImageTingBack.SetLoadInfo(IDB_TING_BACK,hInstance);
	//����-����
	m_ActionDice1.SetLoadInfo( IDB_ACTION_DICE, hInstance);
	CImageHandle HandleDice( &m_ActionDice1);
	m_csActionDice1.cx=m_ActionDice1.GetWidth()/13;
	m_csActionDice1.cy=m_ActionDice1.GetHeight();

	m_ActionDice2.SetLoadInfo( IDB_ACTION_DICE2, hInstance);
	CImageHandle HandleDice2( &m_ActionDice2);
	m_csActionDice2.cx=m_ActionDice2.GetWidth()/6;
	m_csActionDice2.cy=m_ActionDice2.GetHeight();
	return;
}

//��������
CGameClientView::~CGameClientView(void)
{
}

//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//�����ؼ�����
	CRect CreateRect( 0, 0, 40, 30 );
	//��������
	enDirection Direction[]={Direction_North,Direction_East,Direction_South,Direction_West};

	//���ÿؼ�
	for (WORD i=0;i<4;i++)
	{
		//�û��齫
		m_HeapCard[i].SetDirection(Direction[i]);
		m_TableCard[i].SetDirection(Direction[i]);
		m_DiscardCard[i].SetDirection(Direction[i]);

		//����齫
		m_WeaveCard[i][0].SetDisplayItem(true);
		m_WeaveCard[i][1].SetDisplayItem(true);
		m_WeaveCard[i][2].SetDisplayItem(true);
		m_WeaveCard[i][3].SetDisplayItem(true);
		m_WeaveCard[i][0].SetDirection(Direction[i]);
		m_WeaveCard[i][1].SetDirection(Direction[i]);
		m_WeaveCard[i][2].SetDirection(Direction[i]);
		m_WeaveCard[i][3].SetDirection(Direction[i]);
	}

	//���ÿؼ�
	m_UserCard[0].SetDirection(Direction_North);
	m_UserCard[1].SetDirection(Direction_East);
	m_UserCard[2].SetDirection(Direction_West);

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	m_GameScoreWnd.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,100);
	m_ControlWnd.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,10,NULL);

	//�����ؼ�
	m_btStart.Create(NULL,WS_CHILD,rcCreate,this,IDC_START);
	m_btStart.SetButtonImage(IDB_BT_START,AfxGetInstanceHandle(),false);

	//����ť
	m_btTingCard.Create(NULL,WS_CHILD,rcCreate,this,IDC_TING_CARD);
	m_btTingCard.SetButtonImage(IDB_BT_TING_CARD,AfxGetInstanceHandle(),false);
	//ʱ�ӿؼ�
	m_Timer.SetBitmap( IDB_TIMERBACK, IDB_TIMERARROW );
	m_Timer.SetSinkWindow( AfxGetMainWnd() );
	m_Timer.SetDesOrder( false );
	m_Timer.Create( NULL, NULL, WS_VISIBLE|WS_CHILD, CreateRect, this, 20 );
	m_Timer.ShowWindow( SW_HIDE);
	return 0;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//��־����
	m_bOutCard=false;
	m_bWaitOther=false;
	m_bHuangZhuang=false;

	//��Ϸ����
	m_lCellScore=0l;
	m_wBankerUser=INVALID_CHAIR;

	//�û�״̬
	m_cbCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));

	//��������
	m_btStart.ShowWindow(SW_HIDE);
	m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameScoreWnd.ShowWindow(SW_HIDE);

	//�齫����
	m_UserCard[0].SetCardData(0,false);
	m_UserCard[1].SetCardData(0,false);
	m_UserCard[2].SetCardData(0,false);
	m_HandCardControl.SetPositively(false);
	m_HandCardControl.SetDisplayItem(false);
	m_HandCardControl.SetCardData(NULL,0,0);

	//�齫����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_HeapCard[i].SetCardData(0,0,0);
		m_TableCard[i].SetCardData(NULL,0);
		m_DiscardCard[i].SetCardData(NULL,0);
		m_WeaveCard[i][0].SetCardData(NULL,0);
		m_WeaveCard[i][1].SetCardData(NULL,0);
		m_WeaveCard[i][2].SetCardData(NULL,0);
		m_WeaveCard[i][3].SetCardData(NULL,0);
	}

	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	int xInt;
	int yInt;
	if (nWidth > 0 && nWidth < 1500)
	{
		if (xOffInt != nWidth)
		{
			xOffInt=nWidth;
			yOffInt=nHeight;
			AfxGetMainWnd()->SendMessage(IDM_RESET_UI, nWidth, nHeight);
		}
	}//End if
	//��������
	m_ptFace[0].x=nWidth/2-m_nXFace;
	m_ptFace[0].y=5+m_nYBorder + 30;//������
	m_ptNameNew[0].x=nWidth/2+5;
	m_ptName[0].y=5+m_nYBorder;
	m_ptTimer[0].x=nWidth/2-m_nXFace-m_nXTimer*2-5;
	m_ptTimer[0].y=5+m_nYBorder;
	m_ptReady[0].x=nWidth/2-m_nXFace-m_nXTimer-5;
	m_ptReady[0].y=5+m_nYBorder+m_nYTimer/2;
	m_UserFlagPos[0].x=nWidth/2-m_nXFace-m_nXTimer*2-32;
	m_UserFlagPos[0].y=5+m_nYBorder;

	xInt = -20;//������λ
	m_ptFace[1].x=nWidth-m_nXBorder-m_nXFace-5;
	m_ptFace[1].x+=xInt;
	m_ptFace[1].y=nHeight/2-m_nYFace-50;
	m_ptNameNew[1].x=nWidth-m_nXBorder-5;
	m_ptNameNew[1].x+=xInt;
	m_ptName[1].y=nHeight/2-45;
	m_ptTimer[1].x=nWidth-m_nXBorder-m_nXFace-5;
	m_ptTimer[1].x+=xInt;
	m_ptTimer[1].y=nHeight/2-m_nYFace-55-m_nYTimer;
	m_ptReady[1].x=nWidth-m_nXBorder-m_nXTimer-5;
	m_ptReady[1].x+=xInt;
	m_ptReady[1].y=nHeight/2-m_nYFace-55-m_nYTimer/2;
	m_UserFlagPos[1].x=nWidth-m_nXBorder-28;
	m_UserFlagPos[1].x+=xInt;
	m_UserFlagPos[1].y=nHeight/2-m_nYFace-83-m_nYTimer;

	m_ptFace[2].x=nWidth/2-m_nXFace/2;
	m_ptFace[2].y=nHeight-m_nYBorder-m_nYFace-5;
	m_ptNameNew[2].x=nWidth/2+5+m_nXFace/2;
	m_ptName[2].y=nHeight-m_nYBorder-m_nYFace-2;
	m_ptTimer[2].x=nWidth/2-m_nXFace/2-m_nXTimer*2-5;
	m_ptTimer[2].y=nHeight-m_nYBorder-m_nYTimer-10;
	m_ptReady[2].x=nWidth/2-m_nXTimer-m_nXFace/2-5;
	m_ptReady[2].y=nHeight-m_nYBorder-m_nYTimer/2-10;
	m_UserFlagPos[2].x=nWidth/2-m_nXFace/2-m_nXTimer*2-32;
	m_UserFlagPos[2].y=nHeight-m_nYBorder-m_nYTimer-10;

	m_ptFace[3].x=m_nXBorder+5 + 30;//������
	m_ptFace[3].y=nHeight/2-m_nYFace-50 -20;//������
	m_ptNameNew[3].x=m_nXBorder+5;
	m_ptName[3].y=nHeight/2-45;
	int yTopInt = 35;
	m_ptTimer[3].x=m_nXBorder+5;
	m_ptTimer[3].y=nHeight/2-m_nYFace-55-m_nYTimer;
	m_ptTimer[3].y+= yTopInt;


	m_ptReady[3].x=m_nXBorder+5+m_nXTimer;
	m_ptReady[3].y=nHeight/2-m_nYFace-55-m_nYTimer/2;
	m_ptReady[3].y += yTopInt;

	m_UserFlagPos[3].x=m_nXBorder+8;
	m_UserFlagPos[3].y=nHeight/2-m_nYFace-83-m_nYTimer;
	m_UserFlagPos[3].y += yTopInt;

	int nXCenter=nWidth/2;
	int nYCenter=nHeight/2-40;

	int mjW = 20;
	int topOffX = -150;
	int topOffY = -(m_nYFace+10);
	//�û��齫
	//TOP
	xInt = nXCenter-200 +topOffX;
	yInt = m_nYBorder;//m_nYBorder+m_nYFace+10;
	m_UserCard[0].SetControlPoint( xInt, yInt);

	//RIGHT
	int rightOffY = 80;
	xInt = nWidth-m_nXBorder-m_nXFace-49;
	yInt = nHeight/2-255 + rightOffY;
	m_UserCard[1].SetControlPoint(xInt, yInt);

	//LEFT
	int leftOffX = VIDEO_PIC_W -(m_nXFace+30);
	int leftOffY = 0;

	xInt = m_nXBorder+VIDEO_PIC_W;//m_nXBorder+m_nXFace+30;
	yInt = nHeight/2+177;
	m_UserCard[2].SetControlPoint(xInt, yInt);

	//DOWN
	xInt = nXCenter;
	yInt = nHeight-m_nYFace-m_nYBorder-15;
	m_HandCardControl.SetBenchmarkPos( xInt, yInt,enXCenter,enYBottom);
	//�����齫
	m_TableCard[0].SetControlPoint(nWidth/2-179,m_nYBorder+m_nYFace+10);
	m_TableCard[1].SetControlPoint(nWidth-m_nXBorder-m_nXFace-30,nHeight/2-197);
	m_TableCard[2].SetControlPoint(nWidth/2+185,nHeight-m_nYFace-m_nYBorder-15);
	m_TableCard[3].SetControlPoint(m_nXBorder+m_nXFace+30,nHeight/2+97);

	//�ѻ��齫
	int djMJOffX = 0;//40-17;
	//Top
	xInt =nXCenter-98-djMJOffX;
	yInt =nYCenter-108;
	m_HeapCard[0].SetControlPoint(xInt, yInt);

	//rigtht
	xInt =nXCenter+101-djMJOffX;
	yInt =nYCenter-68;
	m_HeapCard[1].SetControlPoint(xInt, yInt);

	//left
	xInt =nXCenter-192-djMJOffX;
	yInt =nYCenter+73;
	m_HeapCard[2].SetControlPoint(xInt, yInt);

	//down
	xInt =nXCenter-123-djMJOffX;
	yInt =nYCenter-172;
	m_HeapCard[3].SetControlPoint(xInt, yInt);

	//�����齫
	//TOP
	xInt =nXCenter-98;
	yInt =nYCenter-110;
	m_DiscardCard[0].SetControlPoint(xInt, yInt);

	//RIGTHT
	xInt =nXCenter+125 + 50;
	yInt =nYCenter-68;
	m_DiscardCard[1].SetControlPoint(xInt, yInt);

	//LEFT
	xInt =nXCenter+71;
	yInt =nYCenter+112;
	m_DiscardCard[2].SetControlPoint(xInt, yInt);

	//DOWN
	xInt =nXCenter-126;
	yInt =nYCenter+59;
	m_DiscardCard[3].SetControlPoint(xInt, yInt);

	//����齫
	//TOP
	xInt = topOffX;
	yInt =m_nYBorder+m_nYFace+52 + topOffY;
	m_WeaveCard[0][0].SetControlPoint(nWidth/2+145+xInt,yInt);
	m_WeaveCard[0][1].SetControlPoint(nWidth/2+55+xInt,yInt);
	m_WeaveCard[0][2].SetControlPoint(nWidth/2-35+xInt,yInt);
	m_WeaveCard[0][3].SetControlPoint(nWidth/2-125+xInt,yInt);

	//����齫
	//right
	xInt =nWidth-m_nXBorder-m_nXFace-83;
	yInt =rightOffY;
	m_WeaveCard[1][0].SetControlPoint(xInt,nHeight/2+100+yInt);
	m_WeaveCard[1][1].SetControlPoint(xInt,nHeight/2+20+yInt);
	m_WeaveCard[1][2].SetControlPoint(xInt,nHeight/2-60+yInt);
	m_WeaveCard[1][3].SetControlPoint(xInt,nHeight/2-140+yInt);

	//����齫
	//down
	xInt = 0;
	yInt =nHeight-m_nYFace-m_nYBorder-68;
	m_WeaveCard[2][0].SetControlPoint(nWidth/2-190+xInt,yInt);
	m_WeaveCard[2][1].SetControlPoint(nWidth/2-100+xInt,yInt);
	m_WeaveCard[2][2].SetControlPoint(nWidth/2-10+xInt,yInt);
	m_WeaveCard[2][3].SetControlPoint(nWidth/2+80+xInt,yInt);

	//����齫
	//left
	xInt =m_nXBorder+m_nXFace+82+leftOffX;
	yInt =0;
	m_WeaveCard[3][0].SetControlPoint(xInt,nHeight/2-200+yInt);
	m_WeaveCard[3][1].SetControlPoint(xInt,nHeight/2-120+yInt);
	m_WeaveCard[3][2].SetControlPoint(xInt,nHeight/2-40+yInt);
	m_WeaveCard[3][3].SetControlPoint(xInt,nHeight/2+40+yInt);

	//���ƴ���
	m_ControlWnd.SetBenchmarkPos(nWidth/2+298,nHeight-m_nYBorder-120);

	//�����ɼ�
	CRect rcScore;
	m_GameScoreWnd.GetWindowRect(&rcScore);
	m_GameScoreWnd.MoveWindow((nWidth-rcScore.Width())/2,(nHeight-rcScore.Height())/2-30,rcScore.Width(),rcScore.Height());

	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	m_btStart.GetWindowRect(&rcButton);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//�ƶ�����
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-155-m_nYBorder,0,0,uFlags);

	//�ƶ�����
	m_btTingCard.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btTingCard,NULL,(nWidth-rcButton.Width() - m_nXBorder),nHeight-105-m_nYBorder,0,0,uFlags);
	//�����ƶ�
	EndDeferWindowPos(hDwp);

	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sDrawGameView.log",strTime);

	if (nWidth > 0 && nWidth < 1500)
	{
		if (xOffInt != nWidth)
		{
			xOffInt=nWidth;
			yOffInt=nHeight;

			AfxGetMainWnd()->SendMessage(IDM_RESET_UI, nWidth, nHeight);
		}

	}
	//�滭����
	DrawViewImage(pDC,m_ImageBack,enMode_Spread);
	int iXPos;
	int iYPos;
	for (int i=0; i < GAME_PLAYER; i++)
	{
		iXPos=	m_PtVideo[i].x;
		iYPos=m_PtVideo[i].y;

		if ( 1 == m_uVideoInt[i])
		DrawOtherVideoPic(  pDC,  iXPos,  iYPos);
	}//End for

	//�Լ�
	int myX,myY,myW,myH;
	myW= MYSLEF_VIDEO_W;
	myH= MYSLEF_VIDEO_H;
	myX=  10-3;//
	myY=  nHeight-myH-10-35; 
	DrawMyselfVideoPic( pDC,  myX,  myY);
	//�滭�û�
	pDC->SetTextColor(RGB(255,255,0));
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		const tagUserData * pUserData=GetUserInfo(i);
		if (pUserData!=NULL)
		{
			//�û�����
			pDC->SetTextAlign((i==1)?(TA_RIGHT|TA_TOP):(TA_LEFT|TA_TOP));
			DrawTextString(pDC,pUserData->szName,RGB(240,240,240),RGB(50,50,50),m_ptNameNew[i].x,m_ptName[i].y);

			if( m_tDice>0 && m_tDice<12)
			{
				ActionDice1( pDC,m_tDice);

				strTemp.Format("ActionDice1 ");
				theApp.WriteLog(strFile, strTemp);

			}
			else
			{
				if( m_tDice== 12)
				{
					ActionDice2( pDC,m_nDiceCount1,m_nDiceCount2);
					strTemp.Format("ActionDice2  m_nDiceCount1=%d,m_nDiceCount2=%d", m_nDiceCount1,m_nDiceCount2);
					theApp.WriteLog(strFile, strTemp);
		//	m_tDice++;
				}//End if
			}//End if
			//������Ϣ
			WORD wUserTimer=GetUserTimer(i);
			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			//DrawUserFace(pDC,pUserData->wFaceID,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);
			//�������
			if( m_bTingFlag[i] )
			{
				ShowTingPic( pDC, m_ptFace[i].x,m_ptFace[i].y);
			}//End if
		}
	}

	//�û���־
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//����λͼ
		CImageHandle ImageHandle(&m_ImageUserFlag);
		int nImageWidth=m_ImageUserFlag.GetWidth()/4;
		int nImageHeight=m_ImageUserFlag.GetHeight();

		//�滭��־
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			WORD wIndex=((i+GAME_PLAYER)-m_wBankerUser)%GAME_PLAYER;
			m_ImageUserFlag.BitBlt(pDC->m_hDC,m_UserFlagPos[i].x,m_UserFlagPos[i].y,nImageWidth,nImageHeight,nImageWidth*wIndex,0);
		}
	}

	//�����齫
	for (WORD i=0;i<4;i++)
	{
		m_TableCard[i].DrawCardControl(pDC);
		m_DiscardCard[i].DrawCardControl(pDC);
		m_WeaveCard[i][0].DrawCardControl(pDC);
		m_WeaveCard[i][1].DrawCardControl(pDC);
		m_WeaveCard[i][2].DrawCardControl(pDC);
		m_WeaveCard[i][3].DrawCardControl(pDC);
	}

	//�ѻ��齫
	m_HeapCard[3].DrawCardControl(pDC);
	m_HeapCard[0].DrawCardControl(pDC);
	m_HeapCard[1].DrawCardControl(pDC);
	m_HeapCard[2].DrawCardControl(pDC);

	//�û��齫
	m_UserCard[0].DrawCardControl(pDC);
	m_UserCard[1].DrawCardControl(pDC);
	m_UserCard[2].DrawCardControl(pDC);
	m_HandCardControl.DrawCardControl(pDC);

	//������ʾ
	if (m_bOutCard==true)
	{
		CImageHandle HandleOutCard(&m_ImageOutCard);
		m_ImageOutCard.AlphaDrawImage(pDC,(nWidth-m_ImageOutCard.GetWidth())/2,nHeight-150,RGB(255,0,255));
		//��ʾ����
		if ( !m_bTingFlag[2] )
		{
			m_btTingCard.ShowWindow(SW_SHOW);
		}
		else
		{
			m_btTingCard.ShowWindow(SW_HIDE);
		}
	}
	else
	{
		m_btTingCard.ShowWindow(SW_HIDE);
	}

	//�ȴ���ʾ
	if (m_bWaitOther==true)
	{
		CImageHandle HandleWait(&m_ImageWait);
		m_ImageWait.AlphaDrawImage(pDC,(nWidth-m_ImageWait.GetWidth())/2,nHeight-150,RGB(255,0,255));
	}

	//��ׯ��־
	if (m_bHuangZhuang==true)
	{
		CImageHandle HandleHuangZhuang(&m_ImageHuangZhuang);
		m_ImageHuangZhuang.AlphaDrawImage(pDC,(nWidth-m_ImageHuangZhuang.GetWidth())/2,nHeight/2-103,RGB(255,0,255));
	}

	//�û�״̬
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if ((m_wOutCardUser==i)||(m_cbUserAction[i]!=0))
		{
			//����λ��
			int nXPos=0,nYPos=0;
			switch (i)
			{
			case 0:	//����
				{
					nXPos=nWidth/2-32;
					nYPos=m_nYBorder+95;
					break;
				}
			case 1:	//����
				{
					nXPos=nWidth-m_nXBorder-170;
					nYPos=nHeight/2-71;
					break;
				}
			case 2:	//����
				{
					nXPos=nWidth/2-32;
					nYPos=nHeight-m_nYBorder-220;
					break;
				}
			case 3:	//����
				{
					nXPos=m_nXBorder+115;
					nYPos=nHeight/2-71 -45;
					break;
				}
			}

			//��������
			CImageHandle ImageHandle(&m_ImageActionBack);
			m_ImageActionBack.AlphaDrawImage(pDC,nXPos,nYPos,m_ImageActionBack.GetWidth()/4,m_ImageActionBack.GetHeight(),
				i*m_ImageActionBack.GetWidth()/4,0,RGB(255,0,255));

			//�滭����
			if (m_cbUserAction[i]!=WIK_NULL)
			{
				//��������
				int nXImagePos=0;
				CImageHandle ImageHandle(&m_ImageUserAction);
				int nItemWidth=m_ImageUserAction.GetWidth()/7;

				//����λ��
				if (m_cbUserAction[i]&WIK_PENG) nXImagePos=nItemWidth;
				else if (m_cbUserAction[i]&WIK_FILL) nXImagePos=nItemWidth*2;
				else if (m_cbUserAction[i]&WIK_GANG) nXImagePos=nItemWidth*3;
				else if (m_cbUserAction[i]&WIK_CHI_HU) nXImagePos=nItemWidth*4;

				//�滭����
				int nItemHeight=m_ImageUserAction.GetHeight();
				m_ImageUserAction.BitBlt(pDC->m_hDC,nXPos+13,nYPos+15,nItemWidth,nItemHeight,nXImagePos,0);
			}
			else
			{
				//����Ѿ����ˣ����Ҵ��˵�һ�ų�ȥ
				if ( m_IntFirstTingFlag[i]==1 && m_bTingFlag[i] )
				{
					//�滭�齫
					g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,0,nXPos+18,nYPos+15);
				}
				else
				{
					//�滭�齫
					g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,m_cbCardData,nXPos+18,nYPos+15);
				}
			}
		}
	}

	return;
}

//��������
void CGameClientView::SetCellScore(LONG lCellScore)
{
	//�����齫
	if (lCellScore!=m_lCellScore)
	{
		//���ñ���
		m_lCellScore=lCellScore;

		//���½���
		UpdateGameView(NULL);
	}

	return;
}

//�����齫
void CGameClientView::SetHuangZhuang(bool bHuangZhuang)
{
	//�����齫
	if (bHuangZhuang!=m_bHuangZhuang)
	{
		//���ñ���
		m_bHuangZhuang=bHuangZhuang;

		//���½���
		UpdateGameView(NULL);
	}

	return;
}

//ׯ���û�
void CGameClientView::SetBankerUser(WORD wBankerUser)
{
	//�����û�
	if (wBankerUser!=m_wBankerUser)
	{
		//���ñ���
		m_wBankerUser=wBankerUser;

		//���½���
		UpdateGameView(NULL);
	}

	return;
}

//״̬��־
void CGameClientView::SetStatusFlag(bool bOutCard, bool bWaitOther)
{
	//���ñ���
	m_bOutCard=bOutCard;
	m_bWaitOther=bWaitOther;

	//���½���
	UpdateGameView(NULL);

	return;
}

//������Ϣ
void CGameClientView::SetOutCardInfo(WORD wViewChairID, BYTE cbCardData)
{
	//���ñ���
	m_cbCardData=cbCardData;
	m_wOutCardUser=wViewChairID;

	//���½���
	UpdateGameView(NULL);

	return;
}

//������Ϣ
void CGameClientView::SetUserAction(WORD wViewChairID, BYTE bUserAction)
{
	//���ñ���
	if (wViewChairID<GAME_PLAYER) m_cbUserAction[wViewChairID]=bUserAction;
	else ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));

	//���½���
	UpdateGameView(NULL);

	return;
}

//��������
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	pDC->SetTextColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//�滭����
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);

	return;
}

//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//�������
	bool bRePaint=false;
	bool bHandle=m_HandCardControl.OnEventSetCursor(MousePoint,bRePaint);

	//�ػ�����
	if (bRePaint==true) UpdateGameView(NULL);

	//������
	if (bHandle==false) __super::OnSetCursor(pWnd,nHitTest,uMessage);

	return TRUE;
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//��ȡ�齫
	BYTE cbHoverCard=m_HandCardControl.GetHoverCard();
	if (cbHoverCard!=0) AfxGetMainWnd()->PostMessage(IDM_OUT_CARD,cbHoverCard,cbHoverCard);

	return;
}

//��ʼ��ť
void CGameClientView::OnStart()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_START,0,0);

	return;
}

//���ư�ť
void CGameClientView::OnTingCard()
{
	AfxGetMainWnd()->SendMessage(IDM_TING_CARD,0,0);
	return;
}
///////////////////////////////////////////////����/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���Ӷ���
void CGameClientView::ActionDice1( CDC *pDC, int nFrame )
{
	CImageHandle ImageHandle( &m_ActionDice1);
	LONG lWidth, lHeight;
	lWidth = m_csActionDice1.cx;
	lHeight = m_csActionDice1.cy;

	m_ActionDice1.AlphaDrawImage( pDC, PS_BX_DICE, PS_BY_DICE, lWidth, lHeight, lWidth*nFrame, 0, COLORTRANS);
}

//���Ӷ���2
void CGameClientView::ActionDice2( CDC *pDC,int pos1, int pos2 )
{
	CImageHandle ImageHandle( &m_ActionDice2);
	LONG lWidth, lHeight;
	lWidth = m_csActionDice2.cx;
	lHeight = m_csActionDice2.cy;

	m_ActionDice2.AlphaDrawImage( pDC, PS_BX_DICE1, PS_BY_DICE1, lWidth, lHeight, lWidth*pos1, 0, COLORTRANS);
	m_ActionDice2.AlphaDrawImage( pDC, PS_BX_DICE2, PS_BY_DICE2, lWidth, lHeight, lWidth*pos2, 0, COLORTRANS);
}
//�������
void CGameClientView::ShowTingPic( CDC *pDC,int x, int y )
{
	CImageHandle ImageHandle( &m_ImageTingBack);
	LONG lWidth, lHeight;
	lWidth = 44;
	lHeight = 44;

	m_ImageTingBack.AlphaDrawImage( pDC, x, y, lWidth, lHeight, 0, 0, COLORTRANS);

}
