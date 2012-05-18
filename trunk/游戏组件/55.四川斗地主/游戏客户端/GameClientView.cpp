#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////////
//��ť��ʶ 

#define IDC_START						100								//��ʼ��ť
#define IDC_ONE_SCORE					101								//1 �ְ�ť
#define IDC_TWO_SCORE					102								//2 �ְ�ť
#define IDC_THREE_SCORE					103								//3 �ְ�ť
#define IDC_GIVE_UP_SCORE				104								//������ť
#define IDC_OUT_CARD					105								//���ư�ť
#define IDC_PASS_CARD					106								//PASS��ť
#define IDC_AUTO_OUTCARD              107                           //��ʾ��Ŧ
#define IDC_AUTOPLAY_ON					108								//�йܰ�ť
#define IDC_AUTOPLAY_OFF				109								//ȡ����ť
#define IDC_SORT_CARD			        110                             //�йܰ�Ŧ

#define IDC_BACK_CATCH					111								//��ץ��ť
#define IDC_LOOK_CARD					112								//���ư�ť
#define IDC_CATCH						113								//��ץ��ť	
#define IDC_NO_CATCH					114								//����ť
#define IDC_ADD_SCORE					115								//���ư�ť
#define IDC_NO_ADD_SCORE				116								//������ť

//��ը��Ŀ
#define BOMB_EFFECT_COUNT				6								//��ը��Ŀ

//��ը��ʶ
#define IDI_BOMB_EFFECT					101								//��ը��ʶ
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_OUT_CARD, OnOutCard)
	ON_BN_CLICKED(IDC_PASS_CARD, OnPassCard)
	ON_BN_CLICKED(IDC_ONE_SCORE, OnOneScore)
	ON_BN_CLICKED(IDC_TWO_SCORE, OnTwoScore)
	ON_BN_CLICKED(IDC_THREE_SCORE, OnThreeScore)
	ON_BN_CLICKED(IDC_AUTO_OUTCARD, OnAutoOutCard)
	ON_BN_CLICKED(IDC_GIVE_UP_SCORE, OnGiveUpScore)
	ON_BN_CLICKED(IDC_AUTOPLAY_ON, OnAutoPlayerOn)
	ON_BN_CLICKED(IDC_AUTOPLAY_OFF,OnAutoPlayerOff)
	ON_BN_CLICKED(IDC_SORT_CARD, OnBnClickedSortCard)

	ON_BN_CLICKED(IDC_BACK_CATCH,	OnBackCatch)
	ON_BN_CLICKED(IDC_LOOK_CARD,	OnLookCard)
	ON_BN_CLICKED(IDC_CATCH,		OnCatch)
	ON_BN_CLICKED(IDC_NO_CATCH,		OnNoCatch)
	ON_BN_CLICKED(IDC_ADD_SCORE,	OnAddScore)
	ON_BN_CLICKED(IDC_NO_ADD_SCORE,	OnNoAddScore)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView(void) : CGameFrameView(true,24)
{
	//��Ϸ����
	m_bWaitType=0;
	m_lBaseScore=0;
	m_wBombTime=1;
	m_wLandUser=INVALID_CHAIR;
	memset(m_bSingleCard,0,sizeof(m_bSingleCard));
	memset(m_bUserScoreTag,0,sizeof(m_bUserScoreTag));
	memset(m_wUserScoreTimes,0,sizeof(m_wUserScoreTimes));

	//״̬����
	memset(m_bPass,0,sizeof(m_bPass));
	memset(m_bUserAction,0,sizeof(m_bUserAction));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bUserTrustee, 0, sizeof( m_bUserTrustee ) );

	//��ը����
	m_bBombEffect=false;
	m_cbBombFrameIndex=0;

	//λ����Ϣ
	memset(m_ptScore,0,sizeof(m_ptScore));
	memset(m_ptLand,0,sizeof(m_ptLand));
	memset(m_ptMingCard,0,sizeof(m_ptMingCard));
	memset(m_ptScoreTimes,0,sizeof(m_ptScoreTimes));
	memset(m_ptScoreTag,0,sizeof(m_ptScoreTag));
	memset(m_ptCountWarming,0,sizeof(m_ptCountWarming));
	m_bDeasilOrder = false;

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBomb.SetLoadInfo(IDB_BOMB,hInstance);
	m_ImageCard.SetLoadInfo(IDB_CARD,hInstance);
	m_ImageBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageCenter.SetLoadInfo(IDB_VIEW_CENTER,hInstance);
	m_ImageBombEffect.SetLoadInfo(IDB_BOMB_EFFECT,hInstance);
	m_ImageScore.LoadImage(hInstance, TEXT( "SCORE" ) );
	m_ImageLand.LoadImage(hInstance, TEXT( "BANKER" ) );
	m_ImageUserTrustee.LoadImage(hInstance,  TEXT("USER_TRUSTEE"));

	//����λ��
	m_LandSize.cx=m_ImageLand.GetWidth();
	m_LandSize.cy=m_ImageLand.GetHeight();

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

	//�����˿�
	for (WORD i=0;i<3;i++)
	{
		//�û��˿�
		m_UserCardControl[i].SetDirection(true);
		m_UserCardControl[i].SetDisplayFlag(true);	
		m_UserCardControl[i].SetCardSpace( 16, 0, 0 );

		m_UserCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,20+i);

		//�û��˿�
		if (i!=1)
		{
			m_HandCardControl[i].SetCardSpace(0,18,0);
			m_HandCardControl[i].SetDirection(false);
			m_HandCardControl[i].SetDisplayFlag(false);
			m_HandCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,30+i);
		}
		else
		{
			m_HandCardControl[i].SetDirection(true);
			m_HandCardControl[i].SetDisplayFlag(false);
			m_HandCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,30+i);
		}
	}

	//�����˿�
	m_BackCardControl.SetCardSpace(85,0,0);
	m_BackCardControl.SetDisplayFlag(false);
	m_HandCardControl[1].SetSinkWindow(AfxGetMainWnd());
	m_BackCardControl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,41);

	//������ť
	m_btStart.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_START);
	m_btOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_OUT_CARD);
	m_btPassCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_PASS_CARD);
	m_btBackCatch.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_BACK_CATCH);
	m_btLookCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_LOOK_CARD);
	m_btCatch.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_CATCH);
	m_btNoCatch.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_NO_CATCH);
	m_btAddScore.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_ADD_SCORE);
	m_btNoAddScore.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_NO_ADD_SCORE);
	m_btGiveUpScore.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_GIVE_UP_SCORE);
	m_btAutoOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_AUTO_OUTCARD);
	m_btAutoPlayOn.Create(TEXT(""), WS_CHILD,CreateRect,this,IDC_AUTOPLAY_ON);
	m_btAutoPlayOff.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_AUTOPLAY_OFF);
	m_btSortCard.Create(NULL,WS_CHILD|WS_DISABLED|WS_VISIBLE,CreateRect,this,IDC_SORT_CARD);

	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_START,hInstance,false);
	m_btOutCard.SetButtonImage(IDB_OUT_CARD,hInstance,false);
	m_btPassCard.SetButtonImage(IDB_PASS,hInstance,false);
	m_btBackCatch.SetButtonImage(IDB_BT_BACK_CATCH,hInstance,false);	
	m_btLookCard.SetButtonImage(IDB_BT_LOOK_CARD,hInstance,false);	
	m_btCatch.SetButtonImage(IDB_BT_CATCH,hInstance,false);		
	m_btNoCatch.SetButtonImage(IDB_BT_NO_CATCH,hInstance,false);	
	m_btAddScore.SetButtonImage(IDB_BT_ADD1,hInstance,false);	
	m_btNoAddScore.SetButtonImage(IDB_BT_NO_ADD1,hInstance,false);
	m_btGiveUpScore.SetButtonImage(IDB_GIVE_UP,hInstance,false);
	m_btAutoOutCard.SetButtonImage(IDB_AUTO_OUT_CARD,hInstance,false);
	m_btAutoPlayOn.SetButtonImage  (IDB_AUTOPLAY_ON,hInstance,false);
	m_btAutoPlayOff.SetButtonImage (IDB_AUTOPLAY_OFF,hInstance,false);
	m_btSortCard.SetButtonImage(IDB_COUNT_SORT,hInstance,false);

    m_btAutoPlayOn.ShowWindow(SW_SHOW);
    m_btAutoPlayOff.ShowWindow(SW_HIDE);

#ifdef VIDEO_GAME

	//������Ƶ
	for (WORD i=0; i<GAME_PLAYER; i++)
	{
		//������Ƶ
		m_DlgVedioService[i].Create(NULL,NULL,WS_CHILD|WS_VISIBLE,CreateRect,this,200+i);
		m_DlgVedioService[i].InitVideoService(i==1,i==1);

		//������Ƶ
		m_VedioServiceManager.SetVideoServiceControl(i,&m_DlgVedioService[i]);
	}

	m_HandCardControl[ 0 ].ShowWindow( SW_HIDE );
	m_HandCardControl[ 2 ].ShowWindow( SW_HIDE );

#endif

	return 0;
}

//�����Ϣ
void CGameClientView::OnRButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnRButtonUp(nFlags, Point);

	//�����˿�
		//�����˿�
	m_btOutCard.EnableWindow(FALSE);
	m_HandCardControl[1].ShootAllCard(false);

	return;
}

//���˫��
void CGameClientView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	__super::OnLButtonDblClk(nFlags, point);

	//�����˿�
	m_btOutCard.EnableWindow(FALSE);
	m_HandCardControl[1].ShootAllCard(false);

	return;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//��Ϸ����
	m_bWaitType=0;
	m_lBaseScore=0;
	m_wBombTime=1;
	m_wLandUser=INVALID_CHAIR;
	memset(m_bSingleCard,0,sizeof(m_bSingleCard));
	memset(m_bUserScoreTag,0,sizeof(m_bUserScoreTag));
	memset(m_wUserScoreTimes,0,sizeof(m_wUserScoreTimes));

	//��ը����
	m_bBombEffect=false;
	m_cbBombFrameIndex=0;

	//״̬����
	memset(m_bPass,0,sizeof(m_bPass));
	memset(m_bUserAction,0,sizeof(m_bUserAction));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bUserTrustee, 0, sizeof( m_bUserTrustee ) );
    
	//���ؿؼ�
	if(m_ScoreView.m_hWnd) m_ScoreView.ShowWindow(SW_HIDE);

	//���ذ�ť
	m_btStart.ShowWindow(SW_HIDE);
	m_btOutCard.ShowWindow(SW_HIDE);
	m_btPassCard.ShowWindow(SW_HIDE);
	m_btGiveUpScore.ShowWindow(SW_HIDE);
	m_btAutoOutCard.ShowWindow(SW_HIDE);
	m_btAutoPlayOn.ShowWindow( SW_SHOW );
	m_btAutoPlayOff.ShowWindow( SW_HIDE );

	//���ÿؼ�
	m_btOutCard.EnableWindow(FALSE);
	m_btPassCard.EnableWindow(FALSE);
	m_btAutoOutCard.EnableWindow(FALSE);
	m_btSortCard.EnableWindow(FALSE);
	m_btSortCard.SetButtonImage(IDB_COUNT_SORT,AfxGetInstanceHandle(),false);

	//�˿˿ؼ�
	m_BackCardControl.SetCardData(NULL,0);
	m_HandCardControl[1].SetPositively(false);
	m_HandCardControl[1].SetDisplayFlag(false);

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_UserCardControl[i].SetCardData(NULL,0);
		m_HandCardControl[i].SetCardData(NULL,0);
	}

#ifdef VIDEO_GAME
	//�����˿�
	m_HandCardControl[ 0 ].ShowWindow( SW_HIDE );
	m_HandCardControl[ 2 ].ShowWindow( SW_HIDE );
#endif

	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{

	//˳ʱ���
	if ( m_bDeasilOrder ) {
		//��������
		m_ptFace[2].x=m_nXBorder+5;
		m_ptFace[2].y=nHeight/2-m_nYFace;
		m_ptName[2].x=m_nXBorder+5;
		m_ptTimer[0].x=m_nXBorder+2+m_nXTimer/2;
		m_ptTimer[0].y=nHeight/2-m_nYFace-5-m_nYTimer/2;
		m_ptTimer[2].y=nHeight/2-m_nYFace-5-m_nYTimer;
		m_ptReady[2].x=m_nXBorder+5+m_nXTimer;
		m_ptReady[2].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_ptScore[2].x=m_nXBorder+m_nXFace+147-10;
		m_ptScore[2].y=nHeight/2-m_nYFace-30;
		m_ptLand[2].x=m_nXBorder-20;
		m_ptLand[2].y=nHeight/2+58;
		m_ptMingCard[2].x = m_nXBorder+10+m_nXFace;
		m_ptMingCard[2].y = nHeight/2-m_nYFace;
		m_ptScoreTimes[2].x = m_nXBorder+5;
		m_ptScoreTimes[2].y = nHeight/2+5+36;
		m_ptScoreTag[2].x = m_ptScoreTimes[2].x+60;
		m_ptScoreTag[2].y = m_ptScoreTimes[2].y-1;
		m_ptCountWarming[2].x = 80;
		m_ptCountWarming[2].y = nHeight/2+10;

		//��������
		m_ptFace[0].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptFace[0].y=nHeight/2-m_nYFace;
		m_ptName[0].x=nWidth-m_nXBorder-5;
		m_ptName[0].y=nHeight/2+5;
		m_ptTimer[2].x=nWidth-m_nXBorder-m_nXTimer/2-2;
		m_ptTimer[2].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_ptReady[0].x=nWidth-m_nXBorder-m_nXTimer-5;
		m_ptReady[0].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_ptScore[0].x=nWidth-m_nXBorder-m_nXFace-190-7;
		m_ptScore[0].y=nHeight/2-m_nYFace-30;
		m_ptLand[0].x=nWidth-m_nXBorder-85-5;
		m_ptLand[0].y=nHeight/2+58;
		m_ptMingCard[0].x = nWidth-28-m_nXFace;
		m_ptMingCard[0].y = nHeight/2-m_nYFace;
		m_ptScoreTimes[0].x = nWidth-m_nXBorder-5;
		m_ptScoreTimes[0].y = nHeight/2+5+36;
		m_ptScoreTag[0].x = m_ptScoreTimes[0].x-60;
		m_ptScoreTag[0].y = m_ptScoreTimes[0].y-1;
		m_ptCountWarming[0].x = nWidth-155;
		m_ptCountWarming[0].y = nHeight/2+15;

		//�û��˿�
		m_UserCardControl[0].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-137,nHeight/2-40,enXRight,enYCenter);
		m_UserCardControl[2].SetBenchmarkPos(m_nXBorder+m_nXFace+137,nHeight/2-40,enXLeft,enYCenter);
		m_HandCardControl[0].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-50,nHeight/2-50,enXRight,enYCenter);
		m_HandCardControl[2].SetBenchmarkPos(m_nXBorder+m_nXFace+50,nHeight/2-55,enXLeft,enYCenter);
	}
	else {
		//��������
		m_ptFace[0].x=m_nXBorder+5;
		m_ptFace[0].y=nHeight/2-m_nYFace;
		m_ptName[0].x=m_nXBorder+5;
		m_ptName[0].y=nHeight/2+5;
		m_ptTimer[0].x=m_nXBorder+2+m_nXTimer/2;
		m_ptTimer[0].y=nHeight/2-m_nYFace-5-m_nYTimer/2;
		m_ptReady[0].x=m_nXBorder+5+m_nXTimer;
		m_ptReady[0].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_ptScore[0].x=m_nXBorder+m_nXFace+147-10;
		m_ptScore[0].y=nHeight/2-m_nYFace-30;
		m_ptLand[0].x=m_nXBorder-20;
		m_ptLand[0].y=nHeight/2+58;
		m_ptMingCard[0].x = m_nXBorder+10+m_nXFace;
		m_ptMingCard[0].y = nHeight/2-m_nYFace;
		m_ptScoreTimes[0].x = m_nXBorder+5;
		m_ptScoreTimes[0].y = nHeight/2+5+36;
		m_ptScoreTag[0].x = m_ptScoreTimes[0].x+60;
		m_ptScoreTag[0].y = m_ptScoreTimes[0].y-1;
		m_ptCountWarming[0].x = 80;
		m_ptCountWarming[0].y = nHeight/2+10;

		//��������
		m_ptFace[2].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptFace[2].y=nHeight/2-m_nYFace;
		m_ptName[2].x=nWidth-m_nXBorder-5;
		m_ptName[2].y=nHeight/2+5;
		m_ptTimer[2].x=nWidth-m_nXBorder-m_nXTimer/2-2;
		m_ptTimer[2].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_ptReady[2].x=nWidth-m_nXBorder-m_nXTimer-5;
		m_ptReady[2].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_ptScore[2].x=nWidth-m_nXBorder-m_nXFace-190-7;
		m_ptScore[2].y=nHeight/2-m_nYFace-30;
		m_ptLand[2].x=nWidth-m_nXBorder-85-5;
		m_ptLand[2].y=nHeight/2+58;
		m_ptMingCard[2].x = nWidth-28-m_nXFace;
		m_ptMingCard[2].y = nHeight/2-m_nYFace;
		m_ptScoreTimes[2].x = nWidth-m_nXBorder-5;
		m_ptScoreTimes[2].y = nHeight/2+5+36;
		m_ptScoreTag[2].x = m_ptScoreTimes[2].x-60;
		m_ptScoreTag[2].y = m_ptScoreTimes[2].y-1;
		m_ptCountWarming[2].x = nWidth-155;
		m_ptCountWarming[2].y = nHeight/2+15;

		//�û��˿�
		m_UserCardControl[2].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-137,nHeight/2-40,enXRight,enYCenter);
		m_UserCardControl[0].SetBenchmarkPos(m_nXBorder+m_nXFace+137,nHeight/2-40,enXLeft,enYCenter);
		m_HandCardControl[2].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-50,nHeight/2-50,enXRight,enYCenter);
		m_HandCardControl[0].SetBenchmarkPos(m_nXBorder+m_nXFace+50,nHeight/2-55,enXLeft,enYCenter);
	}

	////////////////////////////////////////////////////////////////////////////
	//BYTE cbCardData[ 20 ] = { 0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01 };
	////
	//m_HandCardControl[ 1 ].SetCardData( cbCardData, sizeof( cbCardData ) ); 
	//m_HandCardControl[ 0 ].SetCardData( cbCardData, sizeof( cbCardData ) );
	//m_HandCardControl[ 2 ].SetCardData( cbCardData, sizeof( cbCardData ) );
	//m_UserCardControl[ 1 ].SetCardData( cbCardData, 8 ); 
	//m_UserCardControl[ 0 ].SetCardData( cbCardData, 8 );
	//m_UserCardControl[ 2 ].SetCardData( cbCardData, 8 );
	//for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID ) {
	//	m_bPass[ wChairID ] = true; 
	//}
	//if ( m_ScoreView.m_hWnd == NULL ) m_ScoreView.Create(IDD_GAME_SCORE,this);
	//m_ScoreView.ShowWindow( SW_SHOW );
	//for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID ) {
	//	m_ScoreView.SetGameScore( wChairID, "dkfjdkfjdf", 00 );
	//}
	//////////////////////////////////////////////////////////////////////////////

#ifdef VIDEO_GAME
	CRect VDRect;
	m_DlgVedioService[ 0 ].GetWindowRect( &VDRect );
	if ( m_bDeasilOrder ) {
		m_DlgVedioService[ 2 ].MoveWindow( m_nXBorder + 5, m_ptTimer[ 0 ].y - VDRect.Height() - m_nYTimer, VDRect.Width(), VDRect.Height() );
		m_DlgVedioService[ 0 ].MoveWindow( nWidth - VDRect.Width() - m_nXBorder - 5, m_ptTimer[ 2 ].y - VDRect.Height() - m_nYTimer, VDRect.Width(), VDRect.Height() );

		m_HandCardControl[0].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-140,nHeight/2-50,enXRight,enYCenter);
		m_HandCardControl[2].SetBenchmarkPos(m_nXBorder+m_nXFace+140,nHeight/2-55,enXLeft,enYCenter);
	}
	else {
		m_DlgVedioService[ 0 ].MoveWindow( m_nXBorder + 5, m_ptTimer[ 0 ].y - VDRect.Height() - m_nYTimer, VDRect.Width(), VDRect.Height() );
		m_DlgVedioService[ 2 ].MoveWindow( nWidth - VDRect.Width() - m_nXBorder - 5, m_ptTimer[ 2 ].y - VDRect.Height() - m_nYTimer, VDRect.Width(), VDRect.Height() );

		m_HandCardControl[2].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-140,nHeight/2-50,enXRight,enYCenter);
		m_HandCardControl[0].SetBenchmarkPos(m_nXBorder+m_nXFace+140,nHeight/2-55,enXLeft,enYCenter);
	}

	m_DlgVedioService[ 1 ].GetWindowRect( &VDRect );
	m_DlgVedioService[ 1 ].MoveWindow( m_nXBorder + 5, nHeight - VDRect.Height() - m_nYBorder - 50, VDRect.Width(), VDRect.Height() );
#endif
    
	//��������
	m_ptFace[1].x=nWidth/2-m_nXFace/2;
	m_ptFace[1].y=nHeight-m_nYBorder-m_nYFace-5;
	m_ptName[1].x=nWidth/2+5+m_nXFace/2;
	m_ptName[1].y=nHeight-m_nYBorder-m_nYFace-2;
	m_ptTimer[1].x=nWidth/2-m_nXFace/2-m_nXTimer/2-23;
	m_ptTimer[1].y=nHeight-m_nYBorder-m_nYTimer/2-2;
	m_ptReady[1].x=nWidth/2 + 10;
	m_ptReady[1].y=nHeight-217-m_nYBorder;
	m_ptScore[1].x=nWidth/2-33;
	m_ptScore[1].y=nHeight-m_nYBorder-m_nYFace-215;
	m_ptLand[1].x=nWidth/2-m_nXFace/2-150-5;
	m_ptLand[1].y=nHeight-m_nXBorder-m_LandSize.cy+2;
	m_ptMingCard[1].x = nWidth/2-m_nXFace-5;
	m_ptMingCard[1].y = nHeight-m_nYBorder-m_nYFace-8;
	m_ptScoreTimes[1].x = nWidth/2+80+m_nXFace/2;
	m_ptScoreTimes[1].y = nHeight-m_nYBorder-m_nYFace+16;
	m_ptScoreTag[1].x = m_ptScoreTimes[1].x+60;
	m_ptScoreTag[1].y = m_ptScoreTimes[1].y-3;
	m_ptCountWarming[1].x = nWidth/2+60;
	m_ptCountWarming[1].y = nHeight-150;

	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	m_btStart.GetWindowRect(&rcButton);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-217-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOutCard,NULL,nWidth/2-rcButton.Width()*3/2-10,nHeight-217-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btAutoOutCard,NULL,nWidth/2-rcButton.Width()/2,nHeight-217-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPassCard,NULL,nWidth/2+rcButton.Width()/2+10,nHeight-217-m_nYBorder,0,0,uFlags);
	INT nXpos=nWidth/2-rcButton.Width()*2+55;
	INT nXpos2=nWidth/2+15;
	INT nYpos=nHeight-210-m_nYBorder;
	DeferWindowPos(hDwp,m_btBackCatch,NULL,nXpos,nYpos,0,0,uFlags);
	DeferWindowPos(hDwp,m_btLookCard,NULL,nXpos2,nYpos,0,0,uFlags);
	DeferWindowPos(hDwp,m_btCatch,NULL,nXpos,nYpos,0,0,uFlags);
	DeferWindowPos(hDwp,m_btNoCatch,NULL,nXpos2,nYpos,0,0,uFlags);
	DeferWindowPos(hDwp,m_btAddScore,NULL,nXpos,nYpos,0,0,uFlags);
	DeferWindowPos(hDwp,m_btNoAddScore,NULL,nXpos2,nYpos,0,0,uFlags);
	DeferWindowPos(hDwp,m_btGiveUpScore,NULL,nWidth/2+30+rcButton.Width()-10,nHeight-217-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btSortCard,NULL,nWidth-m_nXBorder-rcButton.Width()-15,nHeight-rcButton.Height()*2-m_nYBorder-40,0,0,uFlags);
	EndDeferWindowPos(hDwp);

	CRect rcBtAutoPlay;
	m_btAutoPlayOn.GetWindowRect( rcBtAutoPlay );
	m_btAutoPlayOn .MoveWindow( nWidth - m_nXBorder - 5 - rcBtAutoPlay.Width() ,     nHeight-50, rcBtAutoPlay.Width(), rcBtAutoPlay.Height(), FALSE);
    m_btAutoPlayOff.MoveWindow( nWidth - m_nXBorder - 5 - rcBtAutoPlay.Width(),     nHeight-50, rcBtAutoPlay.Width(), rcBtAutoPlay.Height(), FALSE);
	m_btSortCard.MoveWindow( nWidth - m_nXBorder - 5 - rcBtAutoPlay.Width(), nHeight-55-rcBtAutoPlay.Height(), rcBtAutoPlay.Width(), rcBtAutoPlay.Height(), FALSE);

	//�ƶ��˿�
	m_BackCardControl.SetBenchmarkPos(nWidth/2,m_nYBorder+10,enXCenter,enYTop);
	m_HandCardControl[1].SetBenchmarkPos(nWidth/2,nHeight-m_nYFace-m_nYBorder-25,enXCenter,enYBottom);
	m_UserCardControl[1].SetBenchmarkPos(nWidth/2,nHeight-223-m_nYBorder,enXCenter,enYBottom);


	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageBack,enMode_Spread);
	DrawViewImage(pDC,m_ImageCenter,enMode_Centent);

	//�滭�û�
	TCHAR szBuffer[32];
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wUserTimer=GetUserTimer(i);
		const tagUserData * pUserData=GetUserInfo(i);

		/////////////////////////////////////////////////////

		//tagUserData * pUserData = new tagUserData;
		//strcpy( (char*)pUserData->szName, "dkjfdkfjdkfj" );
		//pUserData->cbUserStatus = US_READY;
		//wUserTimer = 10;
		//m_ImageLand.DrawImage(pDC,m_ptLand[i].x,m_ptLand[i].y);
		//m_wBombTime = 2;
		//m_wLandUser = 1;
		///////////////////////////////////////////////////////

		//�滭�û�
		if (pUserData!=NULL)
		{
			//�û�����
			pDC->SetTextAlign( ( ( i == 2 && ! m_bDeasilOrder ) || ( i == 0 && m_bDeasilOrder ) ) ? TA_RIGHT : TA_LEFT );
			pDC->SetTextColor((wUserTimer>0)?RGB(250,250,250):RGB(220,220,220));
			DrawTextString( pDC, pUserData->szName, RGB( 255, 255, 255 ), RGB( 0, 0, 0 ), m_ptName[i].x,m_ptName[i].y );

			//ʣ���˿�
		//	if (i!=1)
			{
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("ʣ�ࣺ%d ��"),m_bCardCount[i]);
				DrawTextString( pDC, szBuffer, RGB( 255, 255, 255 ), RGB( 0, 0, 0 ), m_ptName[i].x,m_ptName[i].y+18 );
			}

			//�滭����
			if(m_bCardCount[i]==1)
			{
				CPngImage ImageCardCount;
				ImageCardCount.LoadImage(AfxGetInstanceHandle(),TEXT("ONE_CARD_WARMING"));
				ImageCardCount.DrawImage(pDC,m_ptCountWarming[i].x,m_ptCountWarming[i].y);
			}
			else if(m_bCardCount[i]==2)
			{
				CPngImage ImageCardCount;
				ImageCardCount.LoadImage(AfxGetInstanceHandle(),TEXT("TWO_CARD_WARMING"));
				ImageCardCount.DrawImage(pDC,m_ptCountWarming[i].x,m_ptCountWarming[i].y);
			}

			//������Ϣ
			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);
		}
	}

	//�зֱ�־:��ץ,����,ץ,��,��,��,��,pass
	int nXImagePos,nImageWidth=m_ImageScore.GetWidth(),nImageHeight=m_ImageScore.GetHeight();
	for (WORD i=0;i<3;i++)
	{
		if ((m_bUserAction[i]!=0)||(m_bPass[i]==true))
		{
			if ( m_bPass[i]==true ) nXImagePos=m_ImageScore.GetWidth()/8*7;
			else if (m_bUserAction[i]==BACK_CATCH) nXImagePos=m_ImageScore.GetWidth()/8*0;
			else if (m_bUserAction[i]==LOOK_CARD) nXImagePos=m_ImageScore.GetWidth()/8*1;
			else if (m_bUserAction[i]==MING_CATCH) nXImagePos=m_ImageScore.GetWidth()/8*2;
			else if (m_bUserAction[i]==CALL_SCORE) 
			{
				 if(i!=m_wLandUser)nXImagePos=m_ImageScore.GetWidth()/8*3;
				 else nXImagePos=m_ImageScore.GetWidth()/8*4;
			}
			else if (m_bUserAction[i]==CALL_TWO_SCORE) 
			{
				 if(i!=m_wLandUser)nXImagePos=m_ImageScore.GetWidth()/8*5;
				 else nXImagePos=m_ImageScore.GetWidth()/8*6;
			}
			else if (m_bUserAction[i]==PASS_ACTION) nXImagePos=m_ImageScore.GetWidth()/8*7;
			m_ImageScore.DrawImage(pDC,m_ptScore[i].x,m_ptScore[i].y,nImageWidth/8,nImageHeight,nXImagePos,0);
		}
	}

	//ׯ��λ��
	if (m_wLandUser!=INVALID_CHAIR)
	{
		//��ȡ�û�
		const tagUserData * pUserData=GetUserInfo(m_wLandUser);
		if (pUserData!=NULL)
		{
			//��������
			TCHAR szBuffer[64];

			//���� DC
			pDC->SetTextAlign(TA_LEFT|TA_TOP);

			//�滭��Ϣ
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("������%s"),pUserData->szName);
			DrawTextString(pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),m_nXBorder+m_nXFace+10,m_nYBorder+8);

			//�滭ͷ��
			DrawUserFace(pDC,pUserData,m_nXBorder+5,m_nYBorder+5,false);
		}
		for(WORD i=0;i<GAME_PLAYER;i++)
		{
			if(m_wLandUser==i) m_ImageLand.DrawImage(pDC,m_ptLand[m_wLandUser].x,m_ptLand[m_wLandUser].y);
			else 
			{
				CPngImage ImageBoor;
				ImageBoor.LoadImage(AfxGetInstanceHandle(),TEXT("BOOR"));
				ImageBoor.DrawImage(pDC,m_ptLand[i].x,m_ptLand[i].y);
			}
		}
	}

	//��Ԫ����
	if(m_lBaseScore>0)
	{
		//��������
		TCHAR szBuffer[64];
		pDC->SetTextAlign(TA_LEFT|TA_TOP);

		//�滭����
		_sntprintf(szBuffer,sizeof(szBuffer),TEXT("��Ԫ���֣�%d"),m_lBaseScore);
		DrawTextString(pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),m_nXBorder+m_nXFace+10,m_nYBorder+25);
	}

	//�滭��ը
	if (m_bBombEffect==true)
	{
		//������Դ
		CImageHandle HandleBombEffect(&m_ImageBombEffect);

		//�滭Ч��
		INT nImageHeight=m_ImageBombEffect.GetHeight();
		INT nImageWidth=m_ImageBombEffect.GetWidth()/BOMB_EFFECT_COUNT;
		m_ImageBombEffect.AlphaDrawImage(pDC,(nWidth-nImageWidth)/2,0,nImageWidth,nImageHeight,
			nImageWidth*(m_cbBombFrameIndex%BOMB_EFFECT_COUNT),0,RGB(255,0,255));
	}

	//ը������
	if (m_wBombTime>1)
	{
		//ը����־
		CImageHandle ImageHandle(&m_ImageBomb);
		m_ImageBomb.AlphaDrawImage(pDC,m_nXBorder+5,nHeight-m_nYBorder-m_ImageBomb.GetHeight()-5,RGB(255,0,255));

		//���� DC
		pDC->SetTextColor(RGB(250,250,250));
		pDC->SetTextAlign(TA_LEFT|TA_TOP);

		//ը������
		TCHAR szBuffer[64];
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("ը����Ŀ��%d"),m_wBombTime-1);
		DrawTextString( pDC, szBuffer, RGB( 255, 255, 255 ), RGB( 0, 0, 0 ), m_nXBorder+m_ImageBomb.GetWidth()+10,nHeight-m_nYBorder-20 );
	//	pDC->TextOut(m_nXBorder+m_ImageBomb.GetWidth()+10,nHeight-m_nYBorder-20,szBuffer,lstrlen(szBuffer));
	}

	//�û�����
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		BYTE bTemp = m_bSingleCard[i];
		if(bTemp==0x4e || bTemp==0x4f)bTemp-=0x0d;
		if(bTemp>0)
		{
			CImageHandle ImageHandle(&m_ImageCard);
			INT nXSrc = (CCardControl::m_CardSize.cx)*((bTemp&0x0F)-1);
			INT nYSrc = (CCardControl::m_CardSize.cy)*((bTemp&0xF0)>>4); 
			m_ImageCard.AlphaDrawImage(pDC, m_ptMingCard[i].x,m_ptMingCard[i].y,18,35,nXSrc,nYSrc,RGB(255,0,255));
			break;
		}
	}

	//�ȴ���Ϣ
	if(m_bWaitType!=0)
	{
		//������Դ
		CFont InfoFont;
		InfoFont.CreateFont(-30,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("����"));

		//���� DC
		pDC->SetTextAlign(TA_CENTER|TA_TOP);
		CFont * pOldFont=pDC->SelectObject(&InfoFont);

		//�滭��Ϣ
		TCHAR strInfo[128] = TEXT("");
		switch(m_bWaitType)
		{
		case 1:
			{
				_sntprintf(strInfo,sizeof(strInfo),TEXT("�ȴ��û���ץ����"));
				break;
			}
		case 2:
			{
				_sntprintf(strInfo,sizeof(strInfo),TEXT("�ȴ��û���򲻴�"));
				break;
			}
		case 3:
			{
				_sntprintf(strInfo,sizeof(strInfo),TEXT("�ȴ��мҵ��򲻵�"));
				break;
			}
		case 4:
			{
				_sntprintf(strInfo,sizeof(strInfo),TEXT("�ȴ�ׯ��������"));
				break;
			}
		case 5:
			{
				_sntprintf(strInfo,sizeof(strInfo),TEXT("�ȴ��м��ݻ���"));
				break;
			}
		case 6:
			{
				_sntprintf(strInfo,sizeof(strInfo),TEXT("�ȴ�ׯ���ֻ���"));
				break;
			}
		}
		DrawTextString(pDC,strInfo,RGB(255,255,0),RGB(0,255,0),nWidth/2,200);

		//������Դ
		pDC->SelectObject(pOldFont);
		InfoFont.DeleteObject();
	}

	//������־
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_bUserScoreTag[i]==0)continue;

		//������Դ
		CFont InfoFont;
		InfoFont.CreateFont(-14,0,0,0,0,0,0,0,134,3,2,1,2,TEXT("����"));

		//���� DC
		pDC->SetTextAlign(TA_CENTER|TA_TOP);
		CFont * pOldFont=pDC->SelectObject(&InfoFont);

		//��Ϣ����
		TCHAR strInfo[128] = TEXT("");
		ASSERT(m_wLandUser<GAME_PLAYER);
		switch(m_bUserScoreTag[i])
		{
		case 1:
			{
				if(m_wLandUser==i)_sntprintf(strInfo,sizeof(strInfo),TEXT("��"));
				else _sntprintf(strInfo,sizeof(strInfo),TEXT("��"));
				break;
			}
		case 2:
			{
				if(m_wLandUser==i)_sntprintf(strInfo,sizeof(strInfo),TEXT("��"));
				else _sntprintf(strInfo,sizeof(strInfo),TEXT("��"));
				break;
			}
		}

		//���λ��
		DrawTextString(pDC,strInfo,RGB(255,220,0),RGB(0,0,0),m_ptScoreTag[i].x,m_ptScoreTag[i].y);

		//������Դ
		pDC->SelectObject(pOldFont);
		InfoFont.DeleteObject();
	}

	//�ƾֱ���
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
//#ifndef _DEBUG
//		if(i==1)
//#endif
		if(m_wUserScoreTimes[i]>0)
		{
			LONG lTemp = m_wUserScoreTimes[i];
			if(m_wBombTime>1)lTemp*=(INT)(pow(2,m_wBombTime-1));

			//�����Ϣ
			TCHAR strInfo[128]=TEXT("");
			_sntprintf(strInfo,sizeof(strInfo),TEXT("����:%d"),lTemp);
			pDC->SetTextAlign( ( ( i == 2 && ! m_bDeasilOrder ) || ( i == 0 && m_bDeasilOrder ) ) ? TA_RIGHT : TA_LEFT );
			DrawTextString(pDC,strInfo,RGB(255,255,255),RGB(0,0,0),m_ptScoreTimes[i].x,m_ptScoreTimes[i].y);
		}
	}

	//�йܱ�ʶ
	for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID ) {
		
		const tagUserData * pUserData=GetUserInfo( wChairID );
		if ( pUserData == NULL ) m_bUserTrustee[ wChairID ] = false;

		INT nXpos=0,nYpos=0;
		if ( m_bUserTrustee[ wChairID ] ) {
			if ( wChairID == 1 )
			{
				if(wChairID!=m_wLandUser)nXpos=m_ptLand[ wChairID ].x-10;
				else nXpos=m_ptLand[ wChairID ].x-m_ImageUserTrustee.GetWidth()+20;
				m_ImageUserTrustee.DrawImage( pDC, nXpos, m_ptLand[ wChairID ].y+20 );
			}
			else if ( wChairID == 0 && !m_bDeasilOrder || wChairID == 2 && m_bDeasilOrder )
			{
				if(wChairID!=m_wLandUser)nXpos=m_ptLand[ wChairID ].x+35;
				else nXpos=m_ptLand[ wChairID ].x+m_ImageUserTrustee.GetWidth()+5;
				m_ImageUserTrustee.DrawImage( pDC, nXpos, m_ptLand[ wChairID ].y + 70 );
			}
			else 
			{
				if(wChairID!=m_wLandUser)nXpos=m_ptLand[ wChairID ].x+40;
				else nXpos=m_ptLand[ wChairID ].x-m_ImageUserTrustee.GetWidth()+73;
				m_ImageUserTrustee.DrawImage( pDC, nXpos, m_ptLand[ wChairID ].y + 70 );
			}
		}
	}

	return;
}

//ը����Ŀ
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
void CGameClientView::SetLandUser(WORD wChairID)
{
	//���ñ���
	m_wLandUser=wChairID;

	//���½���
	UpdateGameView(NULL);

	return;
}

//�ڿӷ���
void CGameClientView::SetLandAction(WORD wChairID, BYTE bLandScore)
{
	//���ñ���
	if (wChairID!=INVALID_CHAIR) m_bUserAction[wChairID]=bLandScore;
	else memset(m_bUserAction,0,sizeof(m_bUserAction));

	//���½���
	UpdateGameView(NULL);

	return;
}

//��ʼ��ť
void CGameClientView::OnStart()
{

#ifdef VIDEO_GAME
	//�����˿�
	m_HandCardControl[ 0 ].ShowWindow( SW_HIDE );
	m_HandCardControl[ 2 ].ShowWindow( SW_HIDE );
#endif

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

//������ť
void CGameClientView::OnGiveUpScore()
{
	AfxGetMainWnd()->SendMessage(IDM_LAND_SCORE,255,255);
	return;
}

//������ʾ
void CGameClientView::OnAutoOutCard()
{
	AfxGetMainWnd()->SendMessage(IDM_AUTO_OUTCARD,0,0);
	return;
}

//�зְ�ť
void CGameClientView::OnThreeScore()
{
	AfxGetMainWnd()->SendMessage(IDM_LAND_SCORE,3,3);
	return;
}
//////////////////////////////////////////////////////////////////////////

// �Զ��й�
void CGameClientView::OnAutoPlayerOn()
{
    m_btAutoPlayOn.ShowWindow(SW_HIDE);
    m_btAutoPlayOff.ShowWindow(SW_SHOW);
	AfxGetMainWnd()->SendMessage(IDM_LAND_AUTOPLAY,1,1);
    return;
}

// ȡ���й�
void CGameClientView::OnAutoPlayerOff()
{
    m_btAutoPlayOn.ShowWindow(SW_SHOW);
    m_btAutoPlayOff.ShowWindow(SW_HIDE);
	AfxGetMainWnd()->SendMessage(IDM_LAND_AUTOPLAY,0,0);
    return;
}

// ��ץ��Ϣ
void CGameClientView::OnBackCatch()
{
	AfxGetMainWnd()->SendMessage(IDM_LOOK_OR_CATCH,1,1);
    return;
}

// ������Ϣ
void CGameClientView::OnLookCard()
{
	AfxGetMainWnd()->SendMessage(IDM_LOOK_OR_CATCH,0,0);
    return;
}

// ��ץ��Ϣ
void CGameClientView::OnCatch()
{
	AfxGetMainWnd()->SendMessage(IDM_CALL_BANKER,1,1);
    return;
}

// ��ץ��Ϣ
void CGameClientView::OnNoCatch()
{
	AfxGetMainWnd()->SendMessage(IDM_CALL_BANKER,0,0);
    return;
}

// ������Ϣ
void CGameClientView::OnAddScore()
{
	AfxGetMainWnd()->SendMessage(IDM_ADD_SCORE,1,1);
    return;
}

// ������Ϣ
void CGameClientView::OnNoAddScore()
{
	AfxGetMainWnd()->SendMessage(IDM_ADD_SCORE,0,0);
    return;
}

//����˳��
void CGameClientView::SetUserOrder(bool bDeasilOrder) {

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
}

//���ñ�ը
bool CGameClientView::SetBombEffect(bool bBombEffect)
{
	if (bBombEffect==true)
	{
		//���ñ���
		m_bBombEffect=true;
		m_cbBombFrameIndex=0;

		//����ʱ��
		SetTimer(IDI_BOMB_EFFECT,100,NULL);
	}
	else
	{
		//ֹͣ����
		if (m_bBombEffect==true)
		{
			//ɾ��ʱ��
			KillTimer(IDI_BOMB_EFFECT);

			//���ñ���
			m_bBombEffect=false;
			m_cbBombFrameIndex=0;

			//���½���
			UpdateGameView(NULL);
		}
	}

	return true;
}

//ʱ����Ϣ
VOID CGameClientView::OnTimer(UINT nIDEvent)
{
	//��ը����
	if (nIDEvent==IDI_BOMB_EFFECT)
	{
		//ֹͣ�ж�
		if (m_bBombEffect==false)
		{
			KillTimer(IDI_BOMB_EFFECT);
			return;
		}

		//���ñ���
		if ((m_cbBombFrameIndex+1)>=BOMB_EFFECT_COUNT)
		{
			//ɾ��ʱ��
			KillTimer(IDI_BOMB_EFFECT);

			//���ñ���
			m_bBombEffect=false;
			m_cbBombFrameIndex=0;
		}
		else m_cbBombFrameIndex++;

		//���½���
		UpdateGameView(NULL);

		return;
	}

	__super::OnTimer(nIDEvent);
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

//����й�
void CGameClientView::SetUserTrustee( WORD wChairID, bool bTrustee ) {

	//���ñ���
	if ( INVALID_CHAIR == wChairID ) {
		for ( WORD wChairIdx = 0; wChairIdx < GAME_PLAYER; wChairIdx++ ) {
			m_bUserTrustee[ wChairIdx ] = bTrustee;
		}
	}
	else {
		m_bUserTrustee[ wChairID ] = bTrustee;
	}

	//���½���
	UpdateGameView( NULL );
}

//�����˿�
void CGameClientView::SetSingleRandCard(WORD wChairID,BYTE bRandCard)
{
	//���ñ���
	m_bSingleCard[wChairID] = bRandCard;

	//���½���
	UpdateGameView( NULL );
}

//���ñ�־
void CGameClientView::SetUserWaitType(BYTE bWaitType)
{
	//���ñ���
	m_bWaitType = bWaitType;

	//���½���
	UpdateGameView( NULL );
}

//���ñ�־
void CGameClientView::SetUserScroeTag(WORD wChairID,BYTE bType)
{
	//���ñ���
	m_bUserScoreTag[wChairID] = bType;

	//���½���
	UpdateGameView(NULL);
}

//��������
void CGameClientView::SetUserScroeTimes(WORD wScroeTimes[])
{
	//���ñ���
	if(wScroeTimes!=NULL)for(WORD i=0;i<GAME_PLAYER;i++)m_wUserScoreTimes[i] = wScroeTimes[i];
	else for(WORD i=0;i<GAME_PLAYER;i++)m_wUserScoreTimes[i] = 0;

	//���½���
	UpdateGameView(NULL);
}

//����ť
VOID CGameClientView::OnBnClickedSortCard()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_SORT_HAND_CARD,0,0);

	return;
}
