#include "StdAfx.h"
#include "Resource.h"
#include "GameClient.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////
//��ť��ʶ 

#define IDC_START						100								//��ʼ��ť
#define IDC_ONE_SCORE					101								//1 �ְ�ť
#define IDC_TWO_SCORE					102								//2 �ְ�ť
#define IDC_THREE_SCORE					103								//3 �ְ�ť
#define IDC_SHOW_CARD					104								//���ư�ť
#define IDC_GIVE_UP_SCORE				105								//������ť
#define IDC_OUT_CARD					106								//���ư�ť
#define IDC_PASS_CARD					107								//PASS��ť
#define IDC_SORT_BY_NUM					108								//����������ť
#define IDC_SORT_BY_SIZE				109								//����С����ť

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_OUT_CARD, OnOutCard)
	ON_BN_CLICKED(IDC_PASS_CARD, OnPassCard)
	ON_BN_CLICKED(IDC_ONE_SCORE, OnOneScore)
	ON_BN_CLICKED(IDC_TWO_SCORE, OnTwoScore)
	ON_BN_CLICKED(IDC_THREE_SCORE, OnThreeScore)
	ON_BN_CLICKED(IDC_SHOW_CARD, OnShowCard)
	ON_BN_CLICKED(IDC_GIVE_UP_SCORE, OnGiveUpScore)
	ON_BN_CLICKED(IDC_SORT_BY_NUM, OnSortByNum)
	ON_BN_CLICKED(IDC_SORT_BY_SIZE, OnSortBySize)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView(void) : CGameFrameView(true,24)
{
	//��Ϸ����
	m_lBaseScore=0;
	m_wBombTime=1;
	m_cbLandScore=0;
	m_wLandUser=INVALID_CHAIR;

	//���ñ���
	m_bDeasilOrder=true;

	//״̬����
	m_bLandTitle=false;
	m_bMaxBombCountShow= false;
	memset(m_bPass,0,sizeof(m_bPass));
	memset(m_bScore,0,sizeof(m_bScore));
	memset(m_bCardCount,0,sizeof(m_bCardCount));

	//λ����Ϣ
	memset(m_ptScore,0,sizeof(m_ptScore));
	memset(m_ptLand,0,sizeof(m_ptLand));

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBomb.SetLoadInfo(IDB_BOMB,hInstance);
	m_ImageBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageFill.SetLoadInfo(IDB_VIEW_FILL,hInstance);

	//����λ��
	CPngImage ImageLand;
	ImageLand.LoadImage(AfxGetInstanceHandle(),TEXT("BANKER"));
	m_LandSize.cx=ImageLand.GetWidth();
	m_LandSize.cy=ImageLand.GetHeight();

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

	//�����ؼ�
	CRect CreateRect(0,0,0,0);
	m_ScoreView.Create(IDD_GAME_SCORE,this);

	//�����˿�
	for (WORD i=0;i<4;i++)
	{
		m_UserCardControl[i].SetDirection(true);
		m_UserCardControl[i].SetDisplayFlag(true);
		m_UserCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,20+i);
	}
	for (WORD i=0;i<4;i++)
	{
		m_UserCardControl[i].SetlargCard();
		m_UserCardControl[i].SetCardSpace(15,0,0);
		m_HandCardControl[i].SetDisplayFlag(true);
		m_HandCardControl[i].SetCardSpace(14,13,0);
		m_HandCardControl[i].SetDirection(((i==0)||(i==2))?true:false);
		m_HandCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,30+i);
	}
	m_BackCardControl.Create(NULL, NULL,WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,CreateRect,this, 70);
	m_BackCardControl.SetCardSpace(13,0,0);
	m_BackCardControl.SetDisplayFlag(true);
	m_HandCardControl[2].SetSinkWindow(AfxGetMainWnd());

	//���ô���
	m_HandCardControl[2].SetlargCard();

	//������ť
	m_btStart.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_START);
	m_btOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_OUT_CARD);
	m_btPassCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_PASS_CARD);
	m_btOneScore.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_ONE_SCORE);
	m_btTwoScore.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_TWO_SCORE);
	m_btThreeScore.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_THREE_SCORE);
	m_btShowCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_SHOW_CARD);
	m_btGiveUpScore.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_GIVE_UP_SCORE);

	m_btSortByNum.Create(TEXT(""),WS_CHILD, CreateRect, this, IDC_SORT_BY_NUM);
	m_btSortBySize.Create(TEXT(""), WS_CHILD, CreateRect, this, IDC_SORT_BY_SIZE);

	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_START,hInstance,false);
	m_btOutCard.SetButtonImage(IDB_OUT_CARD,hInstance,false);
	m_btPassCard.SetButtonImage(IDB_PASS,hInstance,false);
	m_btOneScore.SetButtonImage(IDB_ONE_SCORE,hInstance,false);
	m_btTwoScore.SetButtonImage(IDB_TWO_SCORE,hInstance,false);
	m_btThreeScore.SetButtonImage(IDB_THREE_SCORE,hInstance,false);
	m_btShowCard.SetButtonImage(IDB_BT_SHOW_CARD,hInstance,false);
	m_btGiveUpScore.SetButtonImage(IDB_GIVE_UP,hInstance,false);
	//����������ť
	m_btSortByNum.SetButtonImage(IDB_BT_2, hInstance, false);
	//����С����ť
	m_btSortBySize.SetButtonImage(IDB_BT_1, hInstance, false);

	//��ȡ����
	m_bDeasilOrder=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),FALSE)?true:false;

#ifdef VIDEO_GAME
	//������Ƶ
	for (WORD i=0;i<4;i++)
	{
		//������Ƶ
		m_DlgVideoService[i].Create(NULL,NULL,WS_CHILD|WS_VISIBLE,CreateRect,this,200+i);
		m_DlgVideoService[i].InitVideoService(i==2,i==2);

		//������Ƶ
		g_VideoServiceManager.SetVideoServiceControl(i,&m_DlgVideoService[i]);
	}
		
	for (WORD i=0;i<4;i++)
	{
		m_HandCardControl[i].SetCardSpace(10,10,0);
		if(i==2) m_HandCardControl[i].SetCardSpace(15,12,0);
	}
#endif

	return 0;
}

//�����Ϣ
void CGameClientView::OnRButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnRButtonUp(nFlags, Point);

	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_OUT_CARD,1,1);

	return;
}

//���˫��
void CGameClientView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	__super::OnLButtonDblClk(nFlags, point);

	//�����˿�
	m_btOutCard.EnableWindow(FALSE);
	m_HandCardControl[2].ShootAllCard(false);

	return;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//��Ϸ����
	m_lBaseScore=0;
	m_wBombTime=1;
	m_cbLandScore=0;
	m_wLandUser=INVALID_CHAIR;

	//״̬����
	m_bLandTitle=false;
	memset(m_bPass,0,sizeof(m_bPass));
	memset(m_bScore,0,sizeof(m_bScore));
	memset(m_bCardCount,0,sizeof(m_bCardCount));

	//���ؿؼ�
	m_ScoreView.ShowWindow(SW_HIDE);

	//���ذ�ť
	m_btStart.ShowWindow(SW_HIDE);
	m_btOutCard.ShowWindow(SW_HIDE);
	m_btPassCard.ShowWindow(SW_HIDE);
	m_btThreeScore.ShowWindow(SW_HIDE);
	m_btGiveUpScore.ShowWindow(SW_HIDE);
	m_btShowCard.ShowWindow(SW_HIDE);

	m_btSortByNum.ShowWindow(SW_HIDE);
	m_btSortBySize.ShowWindow(SW_HIDE);

	//���ÿؼ�
	m_btOutCard.EnableWindow(FALSE);
	m_btPassCard.EnableWindow(FALSE);

	//�˿˿ؼ�
	m_BackCardControl.SetCardData(NULL,0);
	m_HandCardControl[2].SetPositively(false);
	m_HandCardControl[2].SetDisplayFlag(false);
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_HandCardControl[1].SetCardData(NULL,0);
		m_UserCardControl[i].SetCardData(NULL,0);
	}

	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//��������
	if (m_bDeasilOrder==true)
	{
		m_ptFace[1].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptFace[1].y=nHeight/2-m_nYFace;
		m_ptName[1].x=m_ptFace[1].x+8;
		m_ptName[1].y=nHeight/2-m_nYFace+3+m_nXFace;
		m_ptTimer[1].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptTimer[1].y=nHeight/2-m_nYFace-m_nYTimer-5;
		m_ptReady[1].x=nWidth-m_nXBorder-m_nXTimer-5;
		m_ptReady[1].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_ptScore[1].x=nWidth-m_nXBorder-m_nXFace-180;
		m_ptScore[1].y=nHeight/2-m_nYFace+5;
		m_ptLand[1].x=nWidth-m_nXBorder-m_LandSize.cx-3;
		m_ptLand[1].y=nHeight/2+5-m_nYFace-80;
		m_ptBomb[1].x =m_ptLand[1].x;
		m_ptBomb[1].y =m_ptLand[1].y;

		m_ptFace[3].x=m_nXBorder+5;
		m_ptFace[3].y=nHeight/2-m_nYFace;
		m_ptName[3].x=m_ptFace[3].x;
		m_ptName[3].y=nHeight/2-m_nYFace+3+m_nXFace;
		m_ptTimer[3].x=m_nXBorder+5;
		m_ptTimer[3].y=nHeight/2-m_nYFace-5-m_nYTimer;
		m_ptReady[3].x=m_nXBorder+5+m_nXTimer;
		m_ptReady[3].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_ptScore[3].x=m_nXBorder+m_nXFace+137;
		m_ptScore[3].y=nHeight/2-m_nYFace+5;
		m_ptLand[3].x=m_nXBorder+3;
		m_ptLand[3].y=nHeight/2+5-m_nYFace-80;
		m_ptBomb[3].x =m_ptLand[3].x;
		m_ptBomb[3].y =m_ptLand[3].y;
	}
	else
	{
		m_ptFace[3].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptFace[3].y=nHeight/2-m_nYFace-15;
		m_ptName[3].x=m_ptFace[3].x+m_nYFace+5;
		m_ptName[3].y=nHeight/2-m_nYFace+20;
		m_ptTimer[3].x=nWidth-m_nXBorder-m_nXTimer/2;
		m_ptTimer[3].y=nHeight/2-m_nYFace-m_nYTimer;
		m_ptReady[3].x=nWidth-m_nXBorder-m_nXTimer-50;
		m_ptReady[3].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_ptScore[3].x=nWidth-m_nXBorder-m_nXFace-240;
		m_ptScore[3].y=nHeight/2-m_nYFace+5;
		m_ptLand[3].x=nWidth-m_nXBorder-m_LandSize.cx-3;
		m_ptLand[3].y=nHeight/2-m_nYFace-m_nYTimer-30-m_LandSize.cy;
		m_ptBomb[3].x =m_ptLand[3].x-5;
		m_ptBomb[3].y =m_ptLand[3].y;

		m_ptFace[1].x=m_nXBorder+5;
		m_ptFace[1].y=nHeight/2-m_nYFace-15;
		m_ptName[1].x=m_ptFace[1].x-5;
		m_ptName[1].y=nHeight/2-m_nYFace+20;
		m_ptTimer[1].x=m_nXBorder+m_nXTimer/2;
		m_ptTimer[1].y=nHeight/2-m_nYFace-m_nYTimer;
		m_ptReady[1].x=m_nXBorder+5+m_nXTimer+50;
		m_ptReady[1].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_ptScore[1].x=m_nXBorder+m_nXFace+197;
		m_ptScore[1].y=nHeight/2-m_nYFace+5;
		m_ptLand[1].x=m_nXBorder+3;
		m_ptLand[1].y=nHeight/2-m_nYFace-m_nYTimer-30-m_LandSize.cy;
		m_ptBomb[1].x =m_ptLand[1].x;
		m_ptBomb[1].y =m_ptLand[1].y;
	}

	m_ptFace[0].x=nWidth/2-m_nXFace/2;
	m_ptFace[0].y=m_nYBorder+5;
	m_ptName[0].x=nWidth/2+5+m_nXFace/2;
	m_ptName[0].y=m_nYBorder+5;
	m_ptTimer[0].x=nWidth/2-m_nXFace/2-m_nXTimer/2-5;
	m_ptTimer[0].y=m_nYBorder+m_nYTimer/2;
	m_ptReady[0].x=nWidth/2-m_nXFace/2+15;
	m_ptReady[0].y=m_nYBorder+m_nYTimer/2+80;
	m_ptScore[0].x=nWidth/2-21;
	m_ptScore[0].y=m_nYBorder+m_nYFace+120;
	m_ptLand[0].x=nWidth/2-m_nXFace/2-m_LandSize.cx-m_nXTimer-8;
	m_ptLand[0].y=m_nXBorder+5;
	m_ptBomb[0].x =m_ptLand[0].x;
	m_ptBomb[0].y =m_ptLand[0].y;

	m_ptFace[2].x=nWidth/2-m_nXFace/2;
	m_ptFace[2].y=nHeight-m_nYBorder-m_nYFace-5;
	m_ptName[2].x=nWidth/2+5+m_nXFace/2;
	m_ptName[2].y=nHeight-m_nYBorder-m_nYFace/2;
	m_ptTimer[2].x=nWidth/2-m_nXFace/2-m_nXTimer/2-5;
	m_ptTimer[2].y=nHeight-m_nYBorder-m_nYTimer/2;
	m_ptReady[2].x=nWidth/2-m_nXFace/2+15;
	m_ptReady[2].y=nHeight-m_nYBorder-m_nYTimer/2-80;
	m_ptScore[2].x=nWidth/2-21;
	m_ptScore[2].y=nHeight-m_nYBorder-m_nYFace-210;
	m_ptLand[2].x=nWidth/2-m_nXFace/2-m_LandSize.cx-m_nXTimer-8;
	m_ptLand[2].y=nHeight-m_nXBorder-m_LandSize.cy-5;
	m_ptBomb[2].x =m_ptLand[2].x;
	m_ptBomb[2].y =m_ptLand[2].y;

	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	m_btStart.GetWindowRect(&rcButton);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-210-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOutCard,NULL,nWidth/2-rcButton.Width()-5,nHeight-210-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPassCard,NULL,nWidth/2+5,nHeight-210-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOneScore,NULL,nWidth/2-rcButton.Width()*5/2-10,nHeight-195-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btTwoScore,NULL,nWidth/2-rcButton.Width()*3/2-5,nHeight-195-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btThreeScore,NULL,nWidth/2-rcButton.Width()/2,nHeight-195-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btShowCard,NULL,nWidth/2+rcButton.Width()/2+5,nHeight-195-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btGiveUpScore,NULL,nWidth/2+rcButton.Width()*3/2+10,nHeight-195-m_nYBorder,0,0,uFlags);
	m_btSortByNum.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btSortByNum,NULL,nWidth-rcButton.Width()-8-rcButton.Width(),nHeight-rcButton.Height()-2-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btSortBySize,NULL,nWidth-rcButton.Width()-8,nHeight-rcButton.Height()-2-m_nYBorder,0,0,uFlags);
	EndDeferWindowPos(hDwp);

	//�ƶ��˿�
	m_BackCardControl.SetBenchmarkPos(nWidth/2-80,nHeight/2-70,enXLeft,enYTop);
	m_HandCardControl[2].SetBenchmarkPos(nWidth/2,nHeight-m_nYFace-m_nYBorder-20,enXCenter,enYBottom);
	m_UserCardControl[0].SetBenchmarkPos(nWidth/2,nHeight/2-208,enXCenter,enYTop);
	m_UserCardControl[2].SetBenchmarkPos(nWidth/2,nHeight-208-m_nYBorder,enXCenter,enYBottom);
	m_HandCardControl[0].SetBenchmarkPos(nWidth/2,m_nXBorder+50,enXCenter,enYTop);

	//�û��˿�
	if (m_bDeasilOrder==true)
	{
		m_UserCardControl[1].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-130,nHeight/2-30,enXRight,enYCenter);
		m_UserCardControl[3].SetBenchmarkPos(m_nXBorder+m_nXFace+130,nHeight/2-30,enXLeft,enYCenter);
		m_HandCardControl[1].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-50,nHeight/2-30,enXRight,enYCenter);
		m_HandCardControl[3].SetBenchmarkPos(m_nXBorder+m_nXFace+50,nHeight/2-30,enXLeft,enYCenter);
	}
	else 
	{
		m_UserCardControl[3].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-130,nHeight/2-30,enXRight,enYCenter);
		m_UserCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+130,nHeight/2-30,enXLeft,enYCenter);
		m_HandCardControl[3].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-50,nHeight/2-30,enXRight,enYCenter);
		m_HandCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+50,nHeight/2-30,enXLeft,enYCenter);
	}

	//������ͼ
	CRect rcControl;
	m_ScoreView.GetWindowRect(&rcControl);
	m_ScoreView.SetWindowPos(NULL,(nWidth)/2-30,(nHeight)/2,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//��Ƶλ������
#ifdef VIDEO_GAME
	RectifyVideoView(nWidth,nHeight);
#endif

		
	//m_ScoreView.ShowWindow(1);

	return;
}


#ifdef VIDEO_GAME
//������Ƶλ��
void CGameClientView::RectifyVideoView(int nWidth, int nHeight)
{
	//BYTE card[]={	1,1,1,1,1,1,1,1,1,1,
	//				1,1,1,1,1,1,1,1,1,1,
	//				1,1,1,1,1,1,1,1,1,1,
	//				1,1,1,};
	//for(WORD i=0;i<4;i++)
	//{
	//	m_UserCardControl[i].SetCardData(card,10);
	//	m_UserCardControl[i].ShowWindow(1);
	//}
	//m_HandCardControl[0].SetCardData(card,30);
	//m_HandCardControl[1].SetCardData(card,30);
	//m_HandCardControl[2].SetCardData(card,30);
	//m_HandCardControl[3].SetCardData(card,30);


	m_UserCardControl[1].SetDirection(0);
	m_UserCardControl[1].SetCardSpace(10,15,0);
	m_UserCardControl[3].SetDirection(0);
	m_UserCardControl[3].SetCardSpace(10,15,0);

	m_HandCardControl[2].ShowWindow(SW_SHOW);
	
	m_HandCardControl[0].ShowWindow(SW_HIDE);
	m_HandCardControl[1].ShowWindow(SW_HIDE);
	m_HandCardControl[3].ShowWindow(SW_HIDE);

	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//��Ƶ����
	CRect rcAVDlg;
	m_DlgVideoService[0].GetWindowRect(&rcAVDlg);
	if( m_bDeasilOrder==false)
	{
		DeferWindowPos(hDwp,m_DlgVideoService[3],NULL,nWidth-m_nXBorder-5-rcAVDlg.Width(),nHeight/2-10,0,0,uFlags);
		DeferWindowPos(hDwp,m_DlgVideoService[1],NULL,m_nXBorder+5,nHeight/2-10,0,0,uFlags);
	}else
	{
		DeferWindowPos(hDwp,m_DlgVideoService[1],NULL,nWidth-m_nXBorder-5-rcAVDlg.Width(),nHeight/2-rcAVDlg.Height()/2,0,0,uFlags);
		DeferWindowPos(hDwp,m_DlgVideoService[3],NULL,m_nXBorder+5,nHeight/2-rcAVDlg.Height()/2,0,0,uFlags);
	}
	DeferWindowPos(hDwp,m_DlgVideoService[0],NULL,nWidth-rcAVDlg.Width()-5,5,0,0,uFlags);
	m_DlgVideoService[2].GetWindowRect(&rcAVDlg);
	DeferWindowPos(hDwp,m_DlgVideoService[2],NULL,m_nXBorder+5,nHeight-m_nYBorder-3-rcAVDlg.Height(),0,0,uFlags);

	CRect rcButton;
	m_btOneScore.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btOneScore,NULL,nWidth/2-rcButton.Width()*3/2-5-30,nHeight-195-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btTwoScore,NULL,nWidth/2-rcButton.Width()/2-30,nHeight-195-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btThreeScore,NULL,nWidth/2+rcButton.Width()/2+5-30,nHeight-195-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btGiveUpScore,NULL,nWidth/2+rcButton.Width()*3/2+10-30,nHeight-195-m_nYBorder,0,0,uFlags);
	m_btShowCard.ShowWindow(SW_HIDE);
	EndDeferWindowPos(hDwp);

	//�û��˿�
	m_HandCardControl[2].SetBenchmarkPos(nWidth/2,nHeight-m_nYFace-m_nYBorder-20,enXCenter,enYBottom);
	if (m_bDeasilOrder==true)
	{
		m_UserCardControl[1].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-130,nHeight/2-30,enXRight,enYCenter);
		m_UserCardControl[3].SetBenchmarkPos(m_nXBorder+m_nXFace+130,nHeight/2-30,enXLeft,enYCenter);
		m_HandCardControl[1].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-145,nHeight/2-20,enXRight,enYCenter);
		m_HandCardControl[3].SetBenchmarkPos(m_nXBorder+m_nXFace+145,nHeight/2-20,enXLeft,enYCenter);
	}
	else 
	{
		m_UserCardControl[3].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-150,nHeight/2-30,enXRight,enYCenter);
		m_UserCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+150,nHeight/2-30,enXLeft,enYCenter);
		m_HandCardControl[3].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-145,nHeight/2-20,enXRight,enYCenter);
		m_HandCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+145,nHeight/2-20,enXLeft,enYCenter);
	}

	//��������
	int nViewHeight=nHeight-m_nYBorder;
	//��������
	if (m_bDeasilOrder==true)
	{
		m_ptFace[1].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptFace[1].y=nViewHeight/2-m_nYFace-25;
		m_ptName[1].x=nWidth-m_nXBorder-5;
		m_ptName[1].y=m_ptFace[1].y+m_nYFace+5;
		m_ptTimer[1].x=nWidth-m_nXBorder-m_nXTimer/2-5;
		m_ptTimer[1].y=m_ptFace[1].y-5-m_nYTimer/2;
		m_ptReady[1].x=nWidth-m_nXBorder-m_nXFace-55;
		m_ptReady[1].y=m_ptFace[1].y + m_nYFace/2 - 5;
		m_ptScore[1].x=nWidth-m_nXBorder-m_nXFace-180;
		m_ptScore[1].y=nHeight/2-m_nYFace+5;
		m_ptLand[1].x=nWidth-m_nXBorder-m_LandSize.cx-3;
		m_ptLand[1].y=nHeight/2+5-m_nYFace-80;
		m_ptBomb[1].x =m_ptLand[1].x;
		m_ptBomb[1].y =m_ptLand[1].y;

		m_ptFace[3].x=m_nXBorder+5;
		m_ptFace[3].y=nViewHeight/2-m_nYFace-25;
		m_ptName[3].x=m_nXBorder+5;
		m_ptName[3].y=m_ptFace[3].y+m_nYFace+5;
		m_ptTimer[3].x=m_nXBorder+m_nXTimer/2+5;
		m_ptTimer[3].y=m_ptFace[3].y-5-m_nYTimer/2;
		m_ptReady[3].x=m_ptFace[3].x + m_nXFace + 55;
		m_ptReady[3].y=m_ptFace[3].y+m_nYFace/2-5;
		m_ptScore[3].x=m_nXBorder+m_nXFace+137;
		m_ptScore[3].y=nHeight/2-m_nYFace+5;
		m_ptLand[3].x=m_nXBorder+3;
		m_ptLand[3].y=nHeight/2+5-m_nYFace-80;
		m_ptBomb[3].x =m_ptLand[3].x;
		m_ptBomb[3].y =m_ptLand[3].y;
	}
	else
	{
		m_ptFace[3].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptFace[3].y=nHeight/2-m_nYFace-15;
		m_ptName[3].x=m_ptFace[3].x-5;
		m_ptName[3].y=nHeight/2-m_nYFace+5;
		m_ptTimer[3].x=nWidth-m_nXBorder-m_nXTimer/2;
		m_ptTimer[3].y=nHeight/2-m_nYFace-m_nYTimer;
		m_ptReady[3].x=nWidth-m_nXBorder-m_nXTimer-50;
		m_ptReady[3].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_ptScore[3].x=nWidth-m_nXBorder-m_nXFace-240;
		m_ptScore[3].y=nHeight/2-m_nYFace+5;
		m_ptLand[3].x=nWidth-m_nXBorder-m_LandSize.cx-3;
		m_ptLand[3].y=nHeight/2-m_nYFace-m_nYTimer-30-m_LandSize.cy;
		m_ptBomb[3].x =m_ptLand[3].x-5;
		m_ptBomb[3].y =m_ptLand[3].y;

		m_ptFace[1].x=m_nXBorder+5;
		m_ptFace[1].y=nHeight/2-m_nYFace-15;
		m_ptName[1].x=m_ptFace[1].x+m_nXFace+5;
		m_ptName[1].y=nHeight/2-m_nYFace+5;
		m_ptTimer[1].x=m_nXBorder+m_nXTimer/2;
		m_ptTimer[1].y=nHeight/2-m_nYFace-m_nYTimer;
		m_ptReady[1].x=m_nXBorder+5+m_nXTimer+50;
		m_ptReady[1].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_ptScore[1].x=m_nXBorder+m_nXFace+197;
		m_ptScore[1].y=nHeight/2-m_nYFace+5;
		m_ptLand[1].x=m_nXBorder+3;
		m_ptLand[1].y=nHeight/2-m_nYFace-m_nYTimer-30-m_LandSize.cy;
		m_ptBomb[1].x =m_ptLand[1].x;
		m_ptBomb[1].y =m_ptLand[1].y;
	}

	m_ptFace[0].x=nWidth/2-m_nXFace/2;
	m_ptFace[0].y=m_nYBorder+5;
	m_ptName[0].x=nWidth/2+5+m_nXFace/2;
	m_ptName[0].y=m_nYBorder+5;
	m_ptTimer[0].x=nWidth/2-m_nXFace/2-m_nXTimer/2-5;
	m_ptTimer[0].y=m_nYBorder+m_nYTimer/2;
	m_ptReady[0].x=nWidth/2-m_nXFace/2+15;
	m_ptReady[0].y=m_nYBorder+m_nYTimer/2+80;
	m_ptScore[0].x=nWidth/2-21;
	m_ptScore[0].y=m_nYBorder+m_nYFace+120;
	m_ptLand[0].x=nWidth/2-m_nXFace/2-m_LandSize.cx-m_nXTimer-8;
	m_ptLand[0].y=m_nXBorder+5;
	m_ptBomb[0].x =m_ptLand[0].x;
	m_ptBomb[0].y =m_ptLand[0].y;

	m_ptFace[2].x=nWidth/2-m_nXFace/2;
	m_ptFace[2].y=nHeight-m_nYBorder-m_nYFace-5;
	m_ptName[2].x=nWidth/2+5+m_nXFace/2;
	m_ptName[2].y=nHeight-m_nYBorder-m_nYFace/2;
	m_ptTimer[2].x=nWidth/2-m_nXFace/2-m_nXTimer/2-5;
	m_ptTimer[2].y=nHeight-m_nYBorder-m_nYTimer/2;
	m_ptReady[2].x=nWidth/2-m_nXFace/2+15;
	m_ptReady[2].y=nHeight-m_nYBorder-m_nYTimer/2-80;
	m_ptScore[2].x=nWidth/2-21;
	m_ptScore[2].y=nHeight-m_nYBorder-m_nYFace-210;
	m_ptLand[2].x=nWidth/2-m_nXFace/2-m_LandSize.cx-m_nXTimer-8;
	m_ptLand[2].y=nHeight-m_nXBorder-m_LandSize.cy-5;
	m_ptBomb[2].x =m_ptLand[2].x;
	m_ptBomb[2].y =m_ptLand[2].y;
}
#endif

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageFill,enMode_Spread);
	DrawViewImage(pDC,m_ImageBack,enMode_Centent);

	
	//�滭�û�
	TCHAR szBuffer[32];
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wUserTimer=GetUserTimer(i);
		const tagUserData * pUserData=GetUserInfo(i);

		///////////////////////////////////////////////
		////test
//		WORD wUserTimer = 30;
//		tagUserData *pUserData = new tagUserData;
//		lstrcpy(pUserData->szName,TEXT("�������"));
//		pUserData->lScore = 10000L;
//		pUserData->cbUserStatus = US_PLAY;
//		m_wLandUser=1;
//		m_bOutBombCount[1]=2;
//		m_bMaxBombCountShow=1;
//		m_bScore[i]=1;
//		m_bPass[i]=true;
//		m_bCardCount[i]=30;
		///////////////////////////////////////////////

		//�滭�û�
		if (pUserData!=NULL)
		{
			//�û�����
			pDC->SetTextColor((wUserTimer>0)?RGB(250,250,250):RGB(220,220,220));
			pDC->SetTextAlign(((i==1)&&(m_bDeasilOrder==true))||((i==3)&&(m_bDeasilOrder==false))?TA_RIGHT:TA_LEFT);
			pDC->TextOut(m_ptName[i].x,m_ptName[i].y,pUserData->szName,lstrlen(pUserData->szName));

			//ʣ���˿�
			if (pUserData->cbUserStatus>=US_PLAY)
			{
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%ld �ţ�"),m_bCardCount[i]);
				
				if(i==0)	pDC->TextOut(m_ptName[i].x,m_ptName[i].y+20,szBuffer,lstrlen(szBuffer));
				if(i==1)	
				{
#ifdef VIDEO_GAME
					pDC->TextOut(m_ptName[i].x+5,m_ptName[i].y-20,szBuffer,lstrlen(szBuffer));
#else
					pDC->TextOut(m_ptName[i].x+5,m_ptName[i].y+15,szBuffer,lstrlen(szBuffer));
#endif
				}
				if(i==2)	pDC->TextOut(m_ptName[i].x,m_ptName[i].y-20,szBuffer,lstrlen(szBuffer));
				if(i==3)	
				{
#ifdef VIDEO_GAME
					pDC->TextOut(m_ptName[i].x+5,m_ptName[i].y-20,szBuffer,lstrlen(szBuffer));
#else
					pDC->TextOut(m_ptName[i].x,m_ptName[i].y+15,szBuffer,lstrlen(szBuffer));
#endif
				}

				if(m_bMaxBombCountShow)
				{
					//if(i==2 || i== 0){
					if(m_bMaxBombCount[i] !=255){

						//_snprintf(szBuffer,sizeof(szBuffer),TEXT("%s�����Գ�%ld��ը����"),szBuffer, m_bMaxBombCount[i]);
						//ը����־
						CImageHandle ImageHandle(&m_ImageBomb);
						m_ImageBomb.AlphaDrawImage(pDC,m_ptBomb[i].x,m_ptBomb[i].y,RGB(255,0,255));

						//���� DC
						pDC->SetTextColor(RGB(255,255,0));
						pDC->SetTextAlign(TA_LEFT|TA_TOP);

						//ը������
						TCHAR szBuffer[8];
						_snprintf(szBuffer,sizeof(szBuffer),TEXT("%d"),m_bMaxBombCount[i]-m_bOutBombCount[i]);
						pDC->TextOut(m_ptBomb[i].x+30,m_ptBomb[i].y+10,szBuffer,lstrlen(szBuffer));
					}
					//} 
				}

				//if(m_bMaxBombCountShow){
				//	if(i==1 || i ==3){
				//		if(m_bMaxBombCount[i] ==255){
				//			_snprintf(szBuffer,sizeof(szBuffer),TEXT("������"));
				//		} else{
				//			_snprintf(szBuffer,sizeof(szBuffer),TEXT("������") );
				//		}
				//		pDC->TextOut(m_ptName[i].x,m_ptName[i].y+18+18,szBuffer,lstrlen(szBuffer));
				//		if(m_bMaxBombCount[i] ==255){
				//			_snprintf(szBuffer,sizeof(szBuffer),TEXT("�����"));
				//		} else{
				//			_snprintf(szBuffer,sizeof(szBuffer),TEXT("����%ld��"), m_bMaxBombCount[i]);
				//		}
				//		pDC->TextOut(m_ptName[i].x,m_ptName[i].y+18+18+18,szBuffer,lstrlen(szBuffer));
				//		if(m_bMaxBombCount[i] ==255){
				//			_snprintf(szBuffer,sizeof(szBuffer),TEXT("ը����"));
				//		} else{
				//			_snprintf(szBuffer,sizeof(szBuffer),TEXT("ը����"));
				//		}
				//		pDC->TextOut(m_ptName[i].x,m_ptName[i].y+18+18+18+18,szBuffer,lstrlen(szBuffer));
				//	}
				//}
			}


			//������Ϣ
			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);
		}
		
	}

	//�зֱ�־
	CPngImage ImageScore;
	ImageScore.LoadImage(AfxGetInstanceHandle(),TEXT("SCORE"));
	int nXImagePos,nImageWidth=ImageScore.GetWidth(),nImageHeight=ImageScore.GetHeight();
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if ((m_bScore[i]!=0))
		{
			if ((m_bPass[i]==true)||(m_bScore[i]==255)) nXImagePos=ImageScore.GetWidth()*3/5;
			else nXImagePos=(m_bScore[i]-1)*ImageScore.GetWidth()/5;
			ImageScore.DrawImage(pDC,m_ptScore[i].x,m_ptScore[i].y,nImageWidth/5,nImageHeight,nXImagePos,0);
		}
		if(m_bPass[i]==true)
		{
			if ((m_bPass[i]==true)||(m_bScore[i]==255)) nXImagePos=ImageScore.GetWidth()*4/5;
			else nXImagePos=(m_bScore[i]-1)*ImageScore.GetWidth()/5;
			ImageScore.DrawImage(pDC,m_ptScore[i].x,m_ptScore[i].y,nImageWidth/5,nImageHeight,nXImagePos,0);	
		}
	}

	//test
	//tagUserData *pUserData = new tagUserData;
	//lstrcpy(pUserData->szName,TEXT("�������"));
	//pUserData->lScore = 10000L;
	//pUserData->cbUserStatus = US_READY;


	//ׯ��λ��
	if (m_wLandUser!=INVALID_CHAIR)
	{
		//��ȡ�û�
		const tagUserData * pUserData=GetUserInfo(m_wLandUser);
		if (pUserData!=NULL)
		{
			//��������
			TCHAR szBuffer[64];
			CPngImage ImageLand;
			ImageLand.LoadImage(AfxGetInstanceHandle(),TEXT("BANKER"));

			//���� DC
			pDC->SetTextAlign(TA_LEFT|TA_TOP);
			pDC->SetTextColor(RGB(250,250,250));

			//�滭��Ϣ
			ImageLand.DrawImage(pDC,m_ptLand[m_wLandUser].x,m_ptLand[m_wLandUser].y);
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("������%s"),pUserData->szName);
			pDC->TextOut(m_nXBorder+m_nXFace+10,m_nYBorder+8,szBuffer,lstrlen(szBuffer));
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("������%d ��"),m_cbLandScore);
			pDC->TextOut(m_nXBorder+m_nXFace+10,m_nYBorder+25,szBuffer,lstrlen(szBuffer));
			DrawUserFace(pDC,pUserData,m_nXBorder+5,m_nYBorder+5,false);
		}
	}

	//ը������
	//if (m_wBombTime>1)
	//{
	//	//ը����־
	//	CImageHandle ImageHandle(&m_ImageBomb);
	//	m_ImageBomb.AlphaDrawImage(pDC,m_nXBorder+5,nHeight-m_nYBorder-m_ImageBomb.GetHeight()-5,RGB(255,0,255));

	//	//���� DC
	//	pDC->SetTextColor(RGB(250,250,250));
	//	pDC->SetTextAlign(TA_LEFT|TA_TOP);

	//	//ը������
	//	TCHAR szBuffer[64];
	//	_snprintf(szBuffer,sizeof(szBuffer),TEXT("�ѳ�ը����%d"),m_wBombTime);
	//	pDC->TextOut(m_nXBorder+m_ImageBomb.GetWidth()+10,nHeight-m_nYBorder-20,szBuffer,lstrlen(szBuffer));
	//}

	//������Ϣ
	if (m_bLandTitle==true)
	{
		//������Դ
		CFont InfoFont;
		InfoFont.CreateFont(-36,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("����"));

		//���� DC
		pDC->SetTextColor(RGB(201,229,133));
		pDC->SetTextAlign(TA_CENTER|TA_TOP);
		CFont * pOldFont=pDC->SelectObject(&InfoFont);

		//�滭��Ϣ
		pDC->TextOut(nWidth/2,200,"���ȴ��û��з�");

		//������Դ
		pDC->SelectObject(pOldFont);
		InfoFont.DeleteObject();
	}

	return;
}

//�û�˳��
void CGameClientView::SetUserOrder(bool bDeasilOrder)
{
	//״̬�ж�
	if (m_bDeasilOrder==bDeasilOrder) return;

	//���ñ���
	m_bDeasilOrder=bDeasilOrder;
	AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),m_bDeasilOrder?TRUE:FALSE);

	//���ý���
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyGameView(rcClient.Width(),rcClient.Height());

	//ˢ�½���
	UpdateGameView(NULL);

	return;
}

//ը������
void CGameClientView::SetBombTime(WORD wBombTime)
{
	//���ñ���
	m_wBombTime=wBombTime;

	//ˢ�½���
	UpdateGameView(NULL);

	return;
}

//��������
void CGameClientView::SetBaseScore(LONG lBaseScore)
{
	//���ñ���
	m_lBaseScore=lBaseScore;

	//ˢ�½���
	UpdateGameView(NULL);

	return;
}

//��ʾ��ʾ
void CGameClientView::ShowLandTitle(bool bLandTitle)
{
	//���ñ���
	m_bLandTitle=bLandTitle;

	//ˢ�½���
	UpdateGameView(NULL);

	return;
}

//���÷���
void CGameClientView::SetPassFlag(WORD wChairID, bool bPass)
{
	//���ñ���
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) m_bPass[i]=bPass;
	}
	else m_bPass[wChairID]=bPass;

	//���½���
	UpdateGameView(NULL);

	return;
}

//�˿���Ŀ
void CGameClientView::SetCardCount(WORD wChairID, BYTE bCardCount)
{
	//���ñ���
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) m_bCardCount[i]=bCardCount;
	}
	else m_bCardCount[wChairID]=bCardCount;

	//���½���
	UpdateGameView(NULL);

	return;
}

//�ڿӷ���
void CGameClientView::SetLandUser(WORD wChairID, BYTE bLandScore)
{
	//���ñ���
	m_wLandUser=wChairID;
	m_cbLandScore=bLandScore;

	//���½���
	UpdateGameView(NULL);

	return;
}

//�ڿӷ���
void CGameClientView::SetLandScore(WORD wChairID, BYTE bLandScore)
{
	//���ñ���
	if (wChairID!=INVALID_CHAIR) m_bScore[wChairID]=bLandScore;
	else memset(m_bScore,0,sizeof(m_bScore));

	//���½���
	UpdateGameView(NULL);

	return;
}

//��ʼ��ť
void CGameClientView::OnStart()
{
	AfxGetMainWnd()->SendMessage(IDM_START,0,0);
	return;
}

//���ư�ť
void CGameClientView::OnOutCard()
{
	AfxGetMainWnd()->SendMessage(IDM_OUT_CARD,1,1);
	return;
}

//������ť
void CGameClientView::OnPassCard()
{
	AfxGetMainWnd()->SendMessage(IDM_PASS_CARD,1,1);
	return;
}

//�зְ�ť
void CGameClientView::OnOneScore()
{
	AfxGetMainWnd()->SendMessage(IDM_LAND_SCORE,1,1);
	return;
}

//�зְ�ť
void CGameClientView::OnTwoScore()
{
	AfxGetMainWnd()->SendMessage(IDM_LAND_SCORE,2,2);
	return;
}

//�зְ�ť
void CGameClientView::OnThreeScore()
{
	AfxGetMainWnd()->SendMessage(IDM_LAND_SCORE,3,3);
	return;
}

//�зְ�ť
void CGameClientView::OnShowCard()
{
	AfxGetMainWnd()->SendMessage(IDM_LAND_SCORE,6,6);
	return;
}

//������ť
void CGameClientView::OnGiveUpScore()
{
	AfxGetMainWnd()->SendMessage(IDM_LAND_SCORE,255,255);
	return;
}

//����С����ť
void CGameClientView::OnSortByNum()
{
	AfxGetMainWnd()->SendMessage(IDM_SORT_BY_NUM,0,0);
	return;
}

//����������ť
void CGameClientView::OnSortBySize()
{
	
	AfxGetMainWnd()->SendMessage(IDM_SORT_BY_SIZE,0,0);
	return;
}
//////////////////////////////////////////////////////////////////////////

