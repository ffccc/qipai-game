#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"
#include ".\gameclientview.h"

//////////////////////////////////////////////////////////////////////////
//��ť��ʶ 

#define IDC_START						100								//��ʼ��ť
#define IDC_AUTO_ON						101								//����ON��ť
#define IDC_AUTO_OFF					102								//����OFF��ť
#define IDC_OPEN_DOOR					103								//���Ű�ť
#define IDC_OPEN_GOLD					104								//����ť
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_AUTO_ON,OnAutoOn)
	ON_BN_CLICKED(IDC_AUTO_OFF,OnAutoOff)
	ON_BN_CLICKED(IDC_OPEN_DOOR,OnOpenDoor)
	ON_BN_CLICKED(IDC_OPEN_GOLD,OnOpenGold)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//��־����
	m_bOutCard=false;
	m_bWaitOther=false;
	m_bHuangZhuang=false;
	m_bGoldCardShow=false;
	m_bGoldOutEnable=false;

	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		//������ʽ
		m_InsideHeapCard[i].SetArrangeMode(Arrange_Double);
		m_OutSideHeapCard[i].SetArrangeMode(Arrange_Single);
	}	

	//��Ϸ����
	m_lCellScore=0l;
	m_wBankerUser=INVALID_CHAIR;

	//���ӱ��� 
	m_wDoorDice = 0;
	m_wGoldDice = 0;


	//�û�״̬
	m_cbCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));	

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageWait.SetLoadInfo(IDB_WAIT_TIP,hInstance);
	m_ImageBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_BackGround.SetLoadInfo(IDB_BACK_VIEW1,hInstance);
	m_ImageUserFlag.SetLoadInfo(IDB_USER_FLAG,hInstance);
	m_ImageOutCard.SetLoadInfo(IDB_OUT_CARD_TIP,hInstance);
	m_ImageUserAction.SetLoadInfo(IDB_USER_ACTION,hInstance);
	m_ImageActionBack.SetLoadInfo(IDB_ACTION_BACK,hInstance);
	m_ImageBirdBack.SetLoadInfo(IDB_BIRD_BACK,hInstance);
	m_ImageHuangZhuang.SetLoadInfo(IDB_HUANG_ZHUANG,hInstance);
	m_ImageGold.SetLoadInfo( IDB_USER_GOLD, hInstance);
	m_ImageChuiFlag.LoadFromResource(hInstance,IDB_CHUI_FLAG);
	m_ImageGoldCard.SetLoadInfo(IDB_GOLDCARD_BACK,hInstance);
	CImageHandle HandleGlod( &m_ImageGold );
	m_csGlod.cx= m_ImageGold.GetWidth();
	m_csGlod.cy= m_ImageGold.GetHeight();

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

	//��������
	enDirection Direction[]={Direction_North,Direction_East,Direction_South,Direction_West};

	//���ÿؼ�
	for (WORD i=0;i<4;i++)
	{
		//�û��˿�
		m_InsideHeapCard[i].SetDirection(Direction[i]);
		m_OutSideHeapCard[i].SetDirection(Direction[i]);
		m_TableCard[i].SetDirection(Direction[i]);
		m_DiscardCard[i].SetDirection(Direction[i]);

		//����˿�
		m_WeaveCard[i][0].SetDisplayItem(true);
		m_WeaveCard[i][1].SetDisplayItem(true);
		m_WeaveCard[i][2].SetDisplayItem(true);
		m_WeaveCard[i][3].SetDisplayItem(true);
		m_WeaveCard[i][4].SetDisplayItem(true);
		m_WeaveCard[i][0].SetDirection(Direction[i]);
		m_WeaveCard[i][1].SetDirection(Direction[i]);
		m_WeaveCard[i][2].SetDirection(Direction[i]);
		m_WeaveCard[i][3].SetDirection(Direction[i]);
		m_WeaveCard[i][4].SetDirection(Direction[i]);
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
	//m_btAutoOn.Create(NULL,WS_CHILD,rcCreate,this,IDC_AUTO_ON);
	//m_btAutoOff.Create(NULL,WS_CHILD,rcCreate,this,IDC_AUTO_OFF);
	m_btOpenDoor.Create(NULL,WS_CHILD,rcCreate,this,IDC_OPEN_DOOR);
	m_btOpenGold.Create(NULL,WS_CHILD,rcCreate,this,IDC_OPEN_GOLD);


	m_btStart.SetButtonImage(IDB_BT_START,AfxGetInstanceHandle(),false);
	//m_btAutoOn.SetButtonImage(IDB_AUTO_ON,AfxGetInstanceHandle(),false);
	//m_btAutoOff.SetButtonImage(IDB_AUTO_OFF,AfxGetInstanceHandle(),false);
	m_btOpenDoor.SetButtonImage(IDB_OPENDOOR,AfxGetInstanceHandle(),false);
	m_btOpenGold.SetButtonImage(IDB_OPENGOLD,AfxGetInstanceHandle(),false);
	

	//m_btAutoOn.ShowWindow(SW_HIDE);
	//m_btAutoOn.EnableWindow(false);
	//m_btAutoOff.ShowWindow(SW_HIDE);
	m_btOpenDoor.ShowWindow(SW_HIDE);
	m_btOpenGold.ShowWindow(SW_HIDE);

	return 0;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//��־����
	m_bOutCard=false;
	m_bWaitOther=false;
	m_bHuangZhuang=false;
	m_bGoldCardShow = false;
	m_bGoldOutEnable=false;

	//��Ϸ����
	m_lCellScore=0l;
	m_wBankerUser=INVALID_CHAIR;

	//���ӱ��� 
	m_wDoorDice = 0;
	m_wGoldDice = 0;


	//�û�״̬
	m_cbCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));

	//��������
	m_btStart.ShowWindow(SW_HIDE);
	m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameScoreWnd.ShowWindow(SW_HIDE);



	//m_btAutoOn.ShowWindow(SW_SHOW);
	//m_btAutoOff.ShowWindow(SW_HIDE);
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		//������ʽ
		m_InsideHeapCard[i].SetArrangeMode(Arrange_Double);
		m_OutSideHeapCard[i].SetArrangeMode(Arrange_Single);
	}

	//�˿�����
	m_UserCard[0].SetCardData(0,false);
	m_UserCard[1].SetCardData(0,false);
	m_UserCard[2].SetCardData(0,false);
	m_HandCardControl.SetPositively(false);
	m_HandCardControl.SetDisplayItem(false);
	m_HandCardControl.SetCardData(NULL,0,0,NULL,0);
	
	m_GoldCardControl.SetPositively(false);
	m_GoldCardControl.SetDisplayItem(false);
	m_GoldCardControl.SetCardData(NULL,0,0,NULL,0);	

	//�˿�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_InsideHeapCard[i].SetCardData(0,0,0,0);
		m_OutSideHeapCard[i].SetCardData(0,0,0,0);
		m_TableCard[i].SetCardData(NULL,0);
		m_DiscardCard[i].SetCardData(NULL,0);
		m_WeaveCard[i][0].SetCardData(NULL,0);
		m_WeaveCard[i][1].SetCardData(NULL,0);
		m_WeaveCard[i][2].SetCardData(NULL,0);
		m_WeaveCard[i][3].SetCardData(NULL,0);
		m_WeaveCard[i][4].SetCardData(NULL,0);
	}	


	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//��������
	m_ptFace[0].x=nWidth/2-m_nXFace;
	m_ptFace[0].y=5+m_nYBorder;
	m_ptName[0].x=nWidth/2+5;
	m_ptName[0].y=5+m_nYBorder;
	m_ptTimer[0].x=nWidth/2-m_nXFace-m_nXTimer*2-5;
	m_ptTimer[0].y=5+m_nYBorder;
	m_ptReady[0].x=nWidth/2-m_nXFace-m_nXTimer-5;
	m_ptReady[0].y=5+m_nYBorder+m_nYTimer/2;
	m_UserFlagPos[0].x=nWidth/2-m_nXFace-m_nXTimer*2-32;
	m_UserFlagPos[0].y=5+m_nYBorder;

	m_ptFace[1].x=nWidth-m_nXBorder-m_nXFace-5;
	m_ptFace[1].y=nHeight/2-m_nYFace-50;
	m_ptName[1].x=nWidth-m_nXBorder-5;
	m_ptName[1].y=nHeight/2-45;
	m_ptTimer[1].x=nWidth-m_nXBorder-m_nXFace-5;
	m_ptTimer[1].y=nHeight/2-m_nYFace-55-m_nYTimer;
	m_ptReady[1].x=nWidth-m_nXBorder-m_nXTimer-5;
	m_ptReady[1].y=nHeight/2-m_nYFace-55-m_nYTimer/2;
	m_UserFlagPos[1].x=nWidth-m_nXBorder-28;
	m_UserFlagPos[1].y=nHeight/2-m_nYFace-83-m_nYTimer;

	m_ptFace[2].x=nWidth/2-m_nXFace/2;
	m_ptFace[2].y=nHeight-m_nYBorder-m_nYFace-5;
	m_ptName[2].x=nWidth/2+5+m_nXFace/2;
	m_ptName[2].y=nHeight-m_nYBorder-m_nYFace-2;
	m_ptTimer[2].x=nWidth/2-m_nXFace/2-m_nXTimer*2-5;
	m_ptTimer[2].y=nHeight-m_nYBorder-m_nYTimer-10;
	m_ptReady[2].x=nWidth/2-m_nXTimer-m_nXFace/2-5;
	m_ptReady[2].y=nHeight-m_nYBorder-m_nYTimer/2-10;
	m_UserFlagPos[2].x=nWidth/2-m_nXFace/2-m_nXTimer*2-32;
	m_UserFlagPos[2].y=nHeight-m_nYBorder-m_nYTimer-10;

	m_ptFace[3].x=m_nXBorder+5;
	m_ptFace[3].y=nHeight/2-m_nYFace-50;
	m_ptName[3].x=m_nXBorder+5;
	m_ptName[3].y=nHeight/2-45;
	m_ptTimer[3].x=m_nXBorder+5;
	m_ptTimer[3].y=nHeight/2-m_nYFace-55-m_nYTimer;
	m_ptReady[3].x=m_nXBorder+5+m_nXTimer;
	m_ptReady[3].y=nHeight/2-m_nYFace-55-m_nYTimer/2;
	m_UserFlagPos[3].x=m_nXBorder+8;
	m_UserFlagPos[3].y=nHeight/2-m_nYFace-83-m_nYTimer;

	//�û��˿�
	m_UserCard[0].SetControlPoint(nWidth/2-200,m_nYBorder+m_nYFace+10);
	m_UserCard[1].SetControlPoint(nWidth-m_nXBorder-m_nXFace-49,nHeight/2-255);
	m_UserCard[2].SetControlPoint(m_nXBorder+m_nXFace+30,nHeight/2+177);
	m_HandCardControl.SetBenchmarkPos(nWidth/2-20,nHeight-m_nYFace-m_nYBorder-15,enXCenter,enYBottom);
	m_GoldCardControl.SetBenchmarkPos(nWidth/2+80,nHeight/2-27,enXRight,enYBottom);
	

	//�����˿�
	m_TableCard[0].SetControlPoint(nWidth/2-179,m_nYBorder+m_nYFace+10);
	m_TableCard[1].SetControlPoint(nWidth-m_nXBorder-m_nXFace-30,nHeight/2-197);
	m_TableCard[2].SetControlPoint(nWidth/2+185,nHeight-m_nYFace-m_nYBorder-15);
	m_TableCard[3].SetControlPoint(m_nXBorder+m_nXFace+30,nHeight/2+97);

	//�ѻ��˿�
	int nXCenter=nWidth/2;
	int nYCenter=nHeight/2-40;
	m_InsideHeapCard[0].SetControlPoint(nXCenter-60,nYCenter-80);
	m_OutSideHeapCard[0].SetControlPoint(nXCenter-182,nYCenter-150);
	m_InsideHeapCard[1].SetControlPoint(nXCenter+70,nYCenter-48);
	m_OutSideHeapCard[1].SetControlPoint(nXCenter+175,nYCenter-128);
	m_InsideHeapCard[2].SetControlPoint(nXCenter-60,nYCenter+63);
	m_OutSideHeapCard[2].SetControlPoint(nXCenter-182,nYCenter+163);
	m_InsideHeapCard[3].SetControlPoint(nXCenter-90,nYCenter-55);
	m_OutSideHeapCard[3].SetControlPoint(nXCenter-185,nYCenter-128);
	/*m_HeapCard[0].SetControlPoint(nXCenter-60,nYCenter-82);
	m_HeapCard[1].SetControlPoint(nXCenter+71,nYCenter-47);
	m_HeapCard[2].SetControlPoint(nXCenter-162,nYCenter+52);
	m_HeapCard[3].SetControlPoint(nXCenter-93,nYCenter-145);*/
		
	//�����˿�
	m_DiscardCard[0].SetControlPoint(nXCenter-98,nYCenter-130);
	m_DiscardCard[1].SetControlPoint(nXCenter+145,nYCenter-68);
	m_DiscardCard[2].SetControlPoint(nXCenter+71,nYCenter+172);
	m_DiscardCard[3].SetControlPoint(nXCenter-156,nYCenter+99);
	/*m_DiscardCard[0].SetControlPoint(nXCenter-60,nYCenter-82);
	m_DiscardCard[1].SetControlPoint(nXCenter+95,nYCenter-47);
	m_DiscardCard[2].SetControlPoint(nXCenter+41,nYCenter+91);
	m_DiscardCard[3].SetControlPoint(nXCenter-96,nYCenter+42);*/

	//����˿�
	m_WeaveCard[0][0].SetControlPoint(nWidth/2+215,m_nYBorder+m_nYFace+52);
	m_WeaveCard[0][1].SetControlPoint(nWidth/2+125,m_nYBorder+m_nYFace+52);
	m_WeaveCard[0][2].SetControlPoint(nWidth/2+35,m_nYBorder+m_nYFace+52);
	m_WeaveCard[0][3].SetControlPoint(nWidth/2-55,m_nYBorder+m_nYFace+52);
	m_WeaveCard[0][4].SetControlPoint(nWidth/2-145,m_nYBorder+m_nYFace+52);

	//����˿�
	m_WeaveCard[1][0].SetControlPoint(nWidth-m_nXBorder-m_nXFace-83,nHeight/2+150);
	m_WeaveCard[1][1].SetControlPoint(nWidth-m_nXBorder-m_nXFace-83,nHeight/2+70);
	m_WeaveCard[1][2].SetControlPoint(nWidth-m_nXBorder-m_nXFace-83,nHeight/2-10);
	m_WeaveCard[1][3].SetControlPoint(nWidth-m_nXBorder-m_nXFace-83,nHeight/2-90);
	m_WeaveCard[1][4].SetControlPoint(nWidth-m_nXBorder-m_nXFace-83,nHeight/2-170);

	//����˿�
	m_WeaveCard[2][0].SetControlPoint(nWidth/2-250,nHeight-m_nYFace-m_nYBorder-68);
	m_WeaveCard[2][1].SetControlPoint(nWidth/2-160,nHeight-m_nYFace-m_nYBorder-68);
	m_WeaveCard[2][2].SetControlPoint(nWidth/2-70,nHeight-m_nYFace-m_nYBorder-68);
	m_WeaveCard[2][3].SetControlPoint(nWidth/2+20,nHeight-m_nYFace-m_nYBorder-68);
	m_WeaveCard[2][4].SetControlPoint(nWidth/2+110,nHeight-m_nYFace-m_nYBorder-68);

	//����˿�
	m_WeaveCard[3][0].SetControlPoint(m_nXBorder+m_nXFace+82,nHeight/2-230);
	m_WeaveCard[3][1].SetControlPoint(m_nXBorder+m_nXFace+82,nHeight/2-150);
	m_WeaveCard[3][2].SetControlPoint(m_nXBorder+m_nXFace+82,nHeight/2-70);
	m_WeaveCard[3][3].SetControlPoint(m_nXBorder+m_nXFace+82,nHeight/2+10);
	m_WeaveCard[3][4].SetControlPoint(m_nXBorder+m_nXFace+82,nHeight/2+90);

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
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-135-m_nYBorder,0,0,uFlags);

	//m_btAutoOn.GetWindowRect(&rcButton);
	//DeferWindowPos(hDwp,m_btAutoOn,NULL,nWidth-(rcButton.Width()+m_nXBorder+10),nHeight-140-m_nYBorder,0,0,uFlags);
	//DeferWindowPos(hDwp,m_btAutoOff,NULL,nWidth-(rcButton.Width()+m_nXBorder+10),nHeight-100-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOpenDoor,NULL,nWidth/2,nHeight/2,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOpenGold,NULL,nWidth/2,nHeight/2,0,0,uFlags);

	
	//�����ƶ�
	EndDeferWindowPos(hDwp);

	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_BackGround,enMode_Spread);
	DrawViewImage(pDC,m_ImageBack,enMode_Centent);

	//�滭�û�
	pDC->SetTextColor(RGB(255,255,0));
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		const tagUserData * pUserData=GetUserInfo(i);
		if (pUserData!=NULL)
		{
			SetImageGold( pDC, i, pUserData);
			//�û�����
			pDC->SetTextAlign((i==1)?(TA_RIGHT|TA_TOP):(TA_LEFT|TA_TOP));
			DrawTextString(pDC,pUserData->szName,RGB(240,240,240),RGB(50,50,50),m_ptName[i].x,m_ptName[i].y);

			//������Ϣ
			WORD wUserTimer=GetUserTimer(i);
			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
			if (pUserData->cbUserStatus==US_READY)DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);
			
			
		}
	}

	//���Ʊ���	
	if(m_bGoldCardShow)
	{
		CImageHandle GoldCardHandle(&m_ImageGoldCard);
		m_ImageGoldCard.AlphaDrawImage(pDC,nWidth/2-50,nHeight/2-110,m_ImageGoldCard.GetWidth(),m_ImageGoldCard.GetHeight(),0,0,RGB(255,0,255));
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
			//m_ImageUserFlag.BitBlt();
			m_ImageUserFlag.AlphaDrawImage(pDC/*->m_hDC*/,m_UserFlagPos[i].x,m_UserFlagPos[i].y,nImageWidth,nImageHeight,nImageWidth*wIndex,0,RGB(255,0,255));
		}
	}

	//�����˿�
	for (WORD i=0;i<4;i++)
	{
		m_TableCard[i].DrawCardControl(pDC);
		m_DiscardCard[i].DrawCardControl(pDC);
		m_WeaveCard[i][0].DrawCardControl(pDC);
		m_WeaveCard[i][1].DrawCardControl(pDC);
		m_WeaveCard[i][2].DrawCardControl(pDC);
		m_WeaveCard[i][3].DrawCardControl(pDC);
		m_WeaveCard[i][4].DrawCardControl(pDC);
	}

	//��˹��
	/*for(WORD i=0;i<4;i++)
	{
		m_InsideHeapCard[i].SetCardData(0,0,INSIDE_HEAP_FULL_COUNT,0);		
		

	}

	m_OutSideHeapCard[0].SetCardData(0,0,OUTSIDE_HEAP_FULL_COUNT,0);
	m_OutSideHeapCard[1].SetCardData(0,0,OUTSIDE_HEAP_FULL_COUNT,0);
	m_OutSideHeapCard[2].SetCardData(0,0,OUTSIDE_HEAP_FULL_COUNT,0);
	m_OutSideHeapCard[3].SetCardData(0,0,OUTSIDE_HEAP_FULL_COUNT,0);*/
	

	//�ѻ��˿�
	m_InsideHeapCard[3].DrawCardControl(pDC);
	m_OutSideHeapCard[3].DrawCardControl(pDC);
	m_InsideHeapCard[0].DrawCardControl(pDC);
	m_OutSideHeapCard[0].DrawCardControl(pDC);
	m_InsideHeapCard[1].DrawCardControl(pDC);
	m_OutSideHeapCard[1].DrawCardControl(pDC);
	m_InsideHeapCard[2].DrawCardControl(pDC);
	m_OutSideHeapCard[2].DrawCardControl(pDC);	

	//�û��˿�
	m_UserCard[0].DrawCardControl(pDC);
	m_UserCard[1].DrawCardControl(pDC);
	m_UserCard[2].DrawCardControl(pDC);
	m_HandCardControl.DrawCardControl(pDC);
	m_GoldCardControl.DrawCardControl(pDC);
	

	//������ʾ
	if (m_bOutCard==true)
	{
		CImageHandle HandleOutCard(&m_ImageOutCard);
		m_ImageOutCard.AlphaDrawImage(pDC,(nWidth-m_ImageOutCard.GetWidth())/2,nHeight-150,RGB(255,0,255));
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
					nYPos=nHeight/2-71;
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
				else if (m_cbUserAction[i]&WIK_GANG) nXImagePos=nItemWidth*3;
				else if (m_cbUserAction[i]&WIK_CHI_HU) nXImagePos=nItemWidth*4;				

				//�滭����
				int nItemHeight=m_ImageUserAction.GetHeight();
				m_ImageUserAction.BitBlt(pDC->m_hDC,nXPos+13,nYPos+15,nItemWidth,nItemHeight,nXImagePos,0);
			}
			else
			{
				//�滭�˿�
				g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,m_cbCardData,nXPos+18,nYPos+15);
			}
			
		}
	}

	/*if(m_bAutoEnable==false)
	{
		m_btAutoOn.EnableWindow(true);
		m_btAutoOn.ShowWindow(SW_SHOW);
		m_btAutoOff.EnableWindow(false);
		m_btAutoOff.ShowWindow(SW_HIDE);
	}
	else
	{
		m_btAutoOff.ShowWindow(SW_SHOW);
		m_btAutoOff.EnableWindow(true);
		m_btAutoOn.ShowWindow(SW_HIDE);
		m_btAutoOn.EnableWindow(false);
	}*/

    TCHAR	szString[30];
	pDC->SetTextColor(RGB(0,0,0));

	//���ŵ���
	if(m_wDoorDice!=0)
	{
		_snprintf(szString,sizeof(szString),TEXT("���ŵ�����%d"),m_wDoorDice);
		pDC->TextOut(5,nHeight-20,szString);
	}

	//�������
	if(m_wGoldDice!=0)
	{
		_snprintf(szString,sizeof(szString),TEXT("���������%d"),m_wGoldDice);
		pDC->TextOut(5,nHeight-35,szString);
	}


	

	return;
}

//��Ϣ����
BOOL CGameClientView::PreTranslateMessage(MSG * pMsg)
{
	m_ToolTipCtrl.RelayEvent(pMsg);
	return __super::PreTranslateMessage(pMsg);
}

//��������
void CGameClientView::SetCellScore(LONG lCellScore)
{
	//�����˿�
	if (lCellScore!=m_lCellScore)
	{
		//���ñ���
		m_lCellScore=lCellScore;

		//���½���
		UpdateGameView(NULL);
	}

	return;
}

//�����˿�
void CGameClientView::SetHuangZhuang(bool bHuangZhuang)
{
	//�����˿�
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

	//��ȡ�˿�
	//BYTE cbHoverCard=m_HandCardControl.GetHoverCard();
	tagCardItem CardItem;
	memset(&CardItem,0,sizeof(CardItem));
	m_HandCardControl.GetHoverCardItem(CardItem);
	BYTE cbHoverCard=CardItem.cbCardData;
	bool bGoldFlag = CardItem.bGoldFlag;

	if ((cbHoverCard!=0)&&((bGoldFlag==false)||(m_bGoldOutEnable==true))) 
		AfxGetMainWnd()->PostMessage(IDM_OUT_CARD,cbHoverCard,cbHoverCard);
	else if((cbHoverCard!=0)&&(bGoldFlag==true)&&(m_bGoldOutEnable==false))
	{
		//������ʾ
		m_ToolTipCtrl.Create(this);
		m_ToolTipCtrl.Activate(TRUE);
		m_ToolTipCtrl.SetWindowText(TEXT("���ǽ��ƣ�ֻ����˫�λ����ε�ʱ����ܴ����"));
	}

	return;
}

//��ʼ��ť
void CGameClientView::OnStart()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_START,0,0);

	return;
}

//����ť
void CGameClientView::OnAutoOn()
{
	//���ð�ť
	m_bAutoEnable=true;
	/*m_btAutoOff.ShowWindow(SW_SHOW);
	m_btAutoOff.EnableWindow();
	m_btAutoOn.ShowWindow(SW_HIDE);
	m_btAutoOn.EnableWindow(false);*/

	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_AUTO,1,0);

	return;
}
//����ť
void CGameClientView::OnAutoOff()
{
	//���ð�ť
	m_bAutoEnable=false;
	/*m_btAutoOn.ShowWindow(SW_SHOW);
	m_btAutoOn.EnableWindow(true);
	m_btAutoOff.ShowWindow(SW_HIDE);
	m_btAutoOff.EnableWindow(false);*/

	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_AUTO,0,0);

	return;
}
void CGameClientView::SetImageGold( CDC *pDC, BYTE byPlayer, const tagUserData* pUserData )
{
	CImageHandle HandleImage( &m_ImageGold );
	if(byPlayer==1)
	{
		m_ImageGold.AlphaDrawImage( pDC, m_ptFace[byPlayer].x+6, m_ptName[byPlayer].y+10, m_csGlod.cx, m_csGlod.cy,0, 0, RGB(255,0,255));
	}
	else
		m_ImageGold.AlphaDrawImage( pDC, m_ptName[byPlayer].x, m_ptName[byPlayer].y+20, m_csGlod.cx, m_csGlod.cy,0, 0, RGB(255,0,255));
	char str[100];	
	if(pUserData->lScore)
	{
		sprintf( str, "%ld",  pUserData->lScore);
		switch(byPlayer)
		{
		case 0:
			{
				SetArtText( pDC, m_ptName[byPlayer].x+m_csGlod.cx+10, m_ptName[byPlayer].y+20, RGB(0,0,120),RGB(222, 233,2), str );
				break;
			}
		case 1:
			{
				SetArtText( pDC, m_ptFace[byPlayer].x-15, m_ptName[byPlayer].y+m_csGlod.cy+20, RGB(0,0,120),RGB(222, 233,2), str );
				break;
			}
		case 2:
			{
				SetArtText( pDC,  m_ptName[byPlayer].x+m_csGlod.cx+40, m_ptName[byPlayer].y+20, RGB(0,0,120),RGB(222, 233,2), str );
				break;
			}
		case 3:
			{
				SetArtText( pDC, m_ptFace[byPlayer].x, m_ptName[byPlayer].y+m_csGlod.cy+20, RGB(0,0,120),RGB(222, 233,2), str );
				break;
			}
			
		}
	}
	if(pUserData->lGameGold)
	{
		sprintf( str, "%ld",  pUserData->lGameGold);
		switch(byPlayer)
		{
		case 0:
			{
				SetArtText( pDC, m_ptName[byPlayer].x+m_csGlod.cx+10, m_ptName[byPlayer].y+20, RGB(0,0,120),RGB(222, 233,2), str );
				break;
			}
		case 1:
			{
				SetArtText( pDC, m_ptFace[byPlayer].x-15, m_ptName[byPlayer].y+m_csGlod.cy+20, RGB(0,0,120),RGB(222, 233,2), str );
				break;
			}
		case 2:
			{
				SetArtText( pDC,  m_ptName[byPlayer].x+m_csGlod.cx+40, m_ptName[byPlayer].y+20, RGB(0,0,120),RGB(222, 233,2), str );
				break;
			}
		case 3:
			{
				SetArtText( pDC, m_ptFace[byPlayer].x, m_ptName[byPlayer].y+m_csGlod.cy+20, RGB(0,0,120),RGB(222, 233,2), str );
				break;
			}

		}
	}
	/*if( pUserData->lGold)
	{
		sprintf( str, "%ld",  pUserData->lGold);
		SetArtText( pDC, m_ptFace[byPlayer].x+40+m_csGlod.cx+10, m_ptFace[byPlayer].y+20, RGB(0,0,120),RGB(222, 233,2), str );
	}
	else if( pUserData->lScore)
	{
		sprintf( str, "%ld",  pUserData->lScore);
		SetArtText( pDC, m_ptFace[byPlayer].x+40+m_csGlod.cx+10, m_ptFace[byPlayer].y+20, RGB(0,0,120),RGB(222, 233,2), str );
	}*/
}

void CGameClientView::SetArtText( CDC *pDC, int x, int y, COLORREF nColor, COLORREF nBkColor, LPCTSTR lpszString )
{
	pDC->SetTextCharacterExtra( 3 );
	pDC->SetTextColor( nBkColor );
	//pDC->TextOut( x, y, lpszString );
	pDC->TextOut( x, y+1, lpszString );
	pDC->TextOut( x, y-1, lpszString );
	pDC->TextOut( x+1, y, lpszString );
	pDC->TextOut( x+1, y-1, lpszString );
	pDC->TextOut( x+1, y+1, lpszString );
	pDC->TextOut( x-1, y+1, lpszString );
	pDC->TextOut( x-1, y, lpszString );
	pDC->TextOut( x-1, y-1, lpszString );

	pDC->SetTextColor( nColor );
	pDC->TextOut( x, y, lpszString );
}

// ������ʾ
void CGameClientView::SetImageTing( CDC *pDC, BYTE byPlayer)
{	
	char str[100];		
	sprintf( str, "��");
	switch(byPlayer)
	{
		case 0:
			{
				SetArtText( pDC, m_ptName[byPlayer].x+m_csGlod.cx+10, m_ptName[byPlayer].y+20, RGB(0,0,120),RGB(222, 233,2), str );
				break;
			}
		case 1:
			{
				SetArtText( pDC, m_ptFace[byPlayer].x-15, m_ptName[byPlayer].y+m_csGlod.cy+20, RGB(0,0,120),RGB(222, 233,2), str );
				break;
			}
		case 2:
			{
				SetArtText( pDC,  m_ptName[byPlayer].x+m_csGlod.cx+40, m_ptName[byPlayer].y+20, RGB(0,0,120),RGB(222, 233,2), str );
				break;
			}
		case 3:
			{
				SetArtText( pDC, m_ptFace[byPlayer].x, m_ptName[byPlayer].y+m_csGlod.cy+20, RGB(0,0,120),RGB(222, 233,2), str );
				break;
			}
			
	}

}

//������Ϣ
void CGameClientView::OnOpenDoor()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_OPEN_DOOR,0,0);

	return;
}

//������Ϣ
void CGameClientView::OnOpenGold()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_OPEN_GOLD,0,0);
	return;
}

//���ý�����ʾ
void CGameClientView::SetGoldCardShow(bool bGoldCardShow)
{
	if(m_bGoldCardShow!=bGoldCardShow)m_bGoldCardShow=bGoldCardShow;
	return;
}
