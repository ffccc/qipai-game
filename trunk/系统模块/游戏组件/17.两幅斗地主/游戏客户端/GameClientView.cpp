#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////
//��ť��ʶ 

#define IDC_START						100								//��ʼ��ť
#define IDC_ONE_SCORE					101								//1 �ְ�ť
#define IDC_TWO_SCORE					102								//2 �ְ�ť
#define IDC_THREE_SCORE					103								//3 �ְ�ť
#define IDC_GIVE_UP_SCORE				104								//������ť
#define IDC_OUT_CARD					105								//���ư�ť
#define IDC_PASS_CARD					106								//PASS��ť
#define IDC_AUTO_OUTCARD                107                             //��ʾ��Ŧ
#define IDC_AUTOPLAY_ON					108								//�йܰ�ť
#define IDC_AUTOPLAY_OFF				109								//ȡ����ť
#define IDC_SORT_CARD			        110                             //�йܰ�Ŧ

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

	//״̬����
	m_bLandTitle=false;
	memset(m_bPass,0,sizeof(m_bPass));
	memset(m_bScore,0,sizeof(m_bScore));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bUserTrustee, 0, sizeof( m_bUserTrustee ) );

	//��ը����
	m_bBombEffect=false;
	m_cbBombFrameIndex=0;

	//λ����Ϣ
	memset(m_ptScore,0,sizeof(m_ptScore));
	memset(m_ptLand,0,sizeof(m_ptLand));
	m_bDeasilOrder = false;

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBomb.SetLoadInfo(IDB_BOMB,hInstance);
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
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//�û��˿�
		m_UserCardControl[i].SetDirection(true);
		m_UserCardControl[i].SetDisplayFlag(true);	
		m_UserCardControl[i].SetCardSpace( 16, 20, 0 );

		m_UserCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,50+i);

		if ( i == 0 || i == ME_VIEW_CHAIR )
			m_UserCardControl[i].SetDirection(true);
		else
			m_UserCardControl[i].SetDirection(false);

		if ( i ==ME_VIEW_CHAIR || i == 0 )
			m_HandCardControl[i].SetDirection(true);
		else
			m_HandCardControl[i].SetDirection(false);

		//�û��˿�
		if (i!=ME_VIEW_CHAIR)
		{
			m_HandCardControl[i].SetCardSpace(8,8,0);
			m_HandCardControl[i].SetDisplayFlag(false);
			m_HandCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,30+i);
		}
		else
		{
			m_HandCardControl[i].SetDisplayFlag(false);
			m_HandCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,30+i);
		}
	}

	//�����˿�
	m_BackCardControl.SetCardSpace(14,0,0);
	m_BackCardControl.SetDisplayFlag(false);
	m_HandCardControl[ME_VIEW_CHAIR].SetSinkWindow(AfxGetMainWnd());
	m_BackCardControl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,41);

	//������ť
	m_btStart.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_START);
	m_btOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_OUT_CARD);
	m_btPassCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_PASS_CARD);
	m_btOneScore.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_ONE_SCORE);
	m_btTwoScore.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_TWO_SCORE);
	m_btGiveUpScore.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_GIVE_UP_SCORE);
	m_btAutoOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_AUTO_OUTCARD);
	m_btThreeScore.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_THREE_SCORE);
	m_btAutoPlayOn.Create(TEXT(""), WS_CHILD,CreateRect,this,IDC_AUTOPLAY_ON);
	m_btAutoPlayOff.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_AUTOPLAY_OFF);
	m_btSortCard.Create(NULL,WS_CHILD|WS_DISABLED|WS_VISIBLE,CreateRect,this,IDC_SORT_CARD);
    

	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_START,hInstance,false);
	m_btOutCard.SetButtonImage(IDB_OUT_CARD,hInstance,false);
	m_btPassCard.SetButtonImage(IDB_PASS,hInstance,false);
	m_btOneScore.SetButtonImage(IDB_ONE_SCORE,hInstance,false);
	m_btTwoScore.SetButtonImage(IDB_TWO_SCORE,hInstance,false);
	m_btGiveUpScore.SetButtonImage(IDB_GIVE_UP,hInstance,false);
	m_btAutoOutCard.SetButtonImage(IDB_AUTO_OUT_CARD,hInstance,false);
	m_btThreeScore.SetButtonImage(IDB_THREE_SCORE,hInstance,false);
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
		m_DlgVedioService[i].InitVideoService(i==ME_VIEW_CHAIR,i==ME_VIEW_CHAIR);

		//������Ƶ
		m_VedioServiceManager.SetVideoServiceControl(i,&m_DlgVedioService[i]);
	}

	for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
	{
		if ( wChairID != ME_VIEW_CHAIR )
			m_HandCardControl[ wChairID ].ShowWindow( SW_HIDE );
	}

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
	m_HandCardControl[ME_VIEW_CHAIR].ShootAllCard(false);

	return;
}

//���˫��
void CGameClientView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	__super::OnLButtonDblClk(nFlags, point);

	//�����˿�
	m_btOutCard.EnableWindow(FALSE);
	m_HandCardControl[ME_VIEW_CHAIR].ShootAllCard(false);

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

	//��ը����
	m_bBombEffect=false;
	m_cbBombFrameIndex=0;

	//״̬����
	m_bLandTitle=false;
	memset(m_bPass,0,sizeof(m_bPass));
	memset(m_bScore,0,sizeof(m_bScore));
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
	m_btThreeScore.ShowWindow(SW_HIDE);
	m_btAutoPlayOn.ShowWindow(SW_SHOW);
	m_btAutoPlayOff.ShowWindow(SW_HIDE);

	//���ÿؼ�
	m_btOutCard.EnableWindow(FALSE);
	m_btPassCard.EnableWindow(FALSE);
	m_btAutoOutCard.EnableWindow(FALSE);
	m_btSortCard.EnableWindow(FALSE);
	m_btSortCard.SetButtonImage(IDB_COUNT_SORT,AfxGetInstanceHandle(),false);

	//�˿˿ؼ�
	m_BackCardControl.SetCardData(NULL,0);
	m_HandCardControl[ME_VIEW_CHAIR].SetPositively(false);
	m_HandCardControl[ME_VIEW_CHAIR].SetDisplayFlag(false);

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_UserCardControl[i].SetCardData(NULL,0);
		m_HandCardControl[i].SetCardData(NULL,0);
		if (i!=ME_VIEW_CHAIR)
			m_HandCardControl[i].SetCardSpace(8,8,0);
	}

#ifdef VIDEO_GAME
	//�����˿�
	m_HandCardControl[ 0 ].ShowWindow( SW_HIDE );
	m_HandCardControl[ 1 ].ShowWindow( SW_HIDE );
	m_HandCardControl[ 3 ].ShowWindow( SW_HIDE );
#endif

	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{

	int nVDPosY = 0;

#ifdef VIDEO_GAME
	nVDPosY = 50;
#endif

	//��������
	m_ptFace[0].x=nWidth/2-m_nXFace/2;
	m_ptFace[0].y=m_nYBorder+5;
	m_ptName[0].x=nWidth/2+5+m_nXFace/2;
	m_ptName[0].y=m_nYBorder+2;
	m_ptTimer[0].x=nWidth/2-m_nXFace/2-m_nXTimer/2-5;
	m_ptTimer[0].y=m_nYBorder+m_nYTimer/2+2;
	m_ptReady[0].x=nWidth/2+5+m_nXFace/2+120;
	m_ptReady[0].y=m_nYBorder+25;
	m_ptScore[0].x=nWidth/2-21;
	m_ptScore[0].y=m_nYBorder+m_nYFace+130;
	m_ptLand[0].x=nWidth/2-m_nXFace/2-m_LandSize.cx-m_nXTimer*2;
	m_ptLand[0].y=m_nXBorder+5;

	//��������
	m_ptFace[1].x=m_nXBorder+5;
	m_ptFace[1].y=nHeight/2-m_nYFace-nVDPosY;
	m_ptName[1].x=m_nXBorder+5;
	m_ptName[1].y=nHeight/2+5-nVDPosY;
	m_ptTimer[1].x=m_nXBorder+2+m_nXTimer/2;
	m_ptTimer[1].y=nHeight/2-m_nYFace-5-m_nYTimer/2-nVDPosY;
	m_ptReady[1].x=m_nXBorder+5+m_nXTimer;
	m_ptReady[1].y=nHeight/2-m_nYFace-m_nYTimer/2-5-nVDPosY;
	m_ptScore[1].x=m_nXBorder+m_nXFace+147-10;
	m_ptScore[1].y=nHeight/2-m_nYFace-30-nVDPosY;
	m_ptLand[1].x=m_nXBorder+5;
	m_ptLand[1].y=nHeight/2+40-nVDPosY;

	//��������
	m_ptFace[3].x=nWidth-m_nXBorder-m_nXFace-5;
	m_ptFace[3].y=nHeight/2-m_nYFace-nVDPosY;
	m_ptName[3].x=nWidth-m_nXBorder-5;
	m_ptName[3].y=nHeight/2+5-nVDPosY;
	m_ptTimer[3].x=nWidth-m_nXBorder-m_nXTimer/2-2;
	m_ptTimer[3].y=nHeight/2-m_nYFace-m_nYTimer/2-5-nVDPosY;
	m_ptReady[3].x=nWidth-m_nXBorder-m_nXTimer-5;
	m_ptReady[3].y=nHeight/2-m_nYFace-m_nYTimer/2-5-nVDPosY;
	m_ptScore[3].x=nWidth-m_nXBorder-m_nXFace-190-7;
	m_ptScore[3].y=nHeight/2-m_nYFace-30-nVDPosY;
	m_ptLand[3].x=nWidth-m_nXBorder-m_LandSize.cx-5;
	m_ptLand[3].y=nHeight/2+40-nVDPosY;

	//�û��˿�
	m_UserCardControl[3].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-147,nHeight/2-40,enXRight,enYCenter);
	m_UserCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+147,nHeight/2-40,enXLeft,enYCenter);
	m_UserCardControl[0].SetBenchmarkPos(nWidth/2,160,enXCenter,enYTop);
	m_HandCardControl[3].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-50,nHeight/2,enXRight,enYCenter);
	m_HandCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+50,nHeight/2,enXLeft,enYCenter);
	m_HandCardControl[0].SetBenchmarkPos(nWidth/2+40,100,enXCenter,enYCenter);	

	////////////////////////////////////////////////////////////////////////////
//	BYTE cbCardData[ ] = { 0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01 };
//	m_BackCardControl.SetCardData( cbCardData, 8 );
//	for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID ) {
//	//	if ( wChairID == ME_VIEW_CHAIR )
//		m_HandCardControl[ wChairID ].SetCardData( cbCardData, 25 );
//		m_HandCardControl[ wChairID ].SetDisplayFlag( true );
//		m_UserCardControl[ wChairID ].SetCardData( cbCardData, 10 ); 
//		m_bPass[ wChairID ] = true; 
//	}
//	
//	m_BackCardControl.SetDisplayFlag( true );
//	m_HandCardControl[ ME_VIEW_CHAIR ].SetDisplayFlag( true );
//	if ( m_ScoreView.m_hWnd == NULL ) m_ScoreView.Create(IDD_GAME_SCORE,this);
////	m_ScoreView.ShowWindow( SW_SHOW );
//	for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID ) {
//		m_ScoreView.SetGameScore( wChairID, "dkfjdkfjdf", 00 );
//	}
	//////////////////////////////////////////////////////////////////////////////

#ifdef VIDEO_GAME

	//��Ƶ���
	CRect VDRect;
	m_DlgVedioService[ 0 ].GetWindowRect( &VDRect );

	m_DlgVedioService[ 0 ].MoveWindow( nWidth - m_nXBorder - 5 - VDRect.Width(),m_nYBorder + 5 , VDRect.Width(), VDRect.Height() );
	m_DlgVedioService[ 1 ].MoveWindow( m_nXBorder + 5, nHeight / 2 - 10, VDRect.Width(), VDRect.Height() );
	m_DlgVedioService[ 3 ].MoveWindow( nWidth - VDRect.Width() - m_nXBorder - 5, nHeight / 2 - 10, VDRect.Width(), VDRect.Height() );

	m_DlgVedioService[ ME_VIEW_CHAIR ].GetWindowRect( &VDRect );
	m_DlgVedioService[ ME_VIEW_CHAIR ].MoveWindow( m_nXBorder + 5, nHeight - VDRect.Height() - m_nYBorder - 5, VDRect.Width(), VDRect.Height() );

#endif

	//��������
	m_ptFace[ME_VIEW_CHAIR].x=nWidth/2-m_nXFace/2;
	m_ptFace[ME_VIEW_CHAIR].y=nHeight-m_nYBorder-m_nYFace-5;
	m_ptName[ME_VIEW_CHAIR].x=nWidth/2+5+m_nXFace/2;
	m_ptName[ME_VIEW_CHAIR].y=nHeight-m_nYBorder-m_nYFace-2;
	m_ptTimer[ME_VIEW_CHAIR].x=nWidth/2-m_nXFace/2-m_nXTimer/2-5;
	m_ptTimer[ME_VIEW_CHAIR].y=nHeight-m_nYBorder-m_nYTimer/2-2;
	m_ptReady[ME_VIEW_CHAIR].x=nWidth/2 + 10;
	m_ptReady[ME_VIEW_CHAIR].y=nHeight-217-m_nYBorder;
	m_ptScore[ME_VIEW_CHAIR].x=nWidth/2-21;
	m_ptScore[ME_VIEW_CHAIR].y=nHeight-m_nYBorder-m_nYFace-210;
	m_ptLand[ME_VIEW_CHAIR].x=nWidth/2-m_nXFace/2-m_LandSize.cx-m_nXTimer*2;
	m_ptLand[ME_VIEW_CHAIR].y=nHeight-m_nXBorder-m_LandSize.cy-5;

	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	m_btStart.GetWindowRect(&rcButton);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-217-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOutCard,NULL,nWidth/2-rcButton.Width()*3/2-10,nHeight-217-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btAutoOutCard,NULL,nWidth/2-rcButton.Width()/2,nHeight-217-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPassCard,NULL,nWidth/2+rcButton.Width()/2+10,nHeight-217-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOneScore,NULL,nWidth/2-rcButton.Width()*2-12,nHeight-217-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btTwoScore,NULL,nWidth/2-rcButton.Width()-2,nHeight-217-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btThreeScore,NULL,nWidth/2+10,nHeight-217-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btGiveUpScore,NULL,nWidth/2+30+rcButton.Width()-10,nHeight-217-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btSortCard,NULL,nWidth-m_nXBorder-rcButton.Width()-15,nHeight-rcButton.Height()*2-m_nYBorder-40,0,0,uFlags);
	EndDeferWindowPos(hDwp);

	CRect rcBtAutoPlay;
	m_btAutoPlayOn.GetWindowRect( rcBtAutoPlay );
	m_btAutoPlayOn .MoveWindow( nWidth - m_nXBorder - 5 - rcBtAutoPlay.Width() , nHeight-5-rcBtAutoPlay.Height(), rcBtAutoPlay.Width(), rcBtAutoPlay.Height(), FALSE);
    m_btAutoPlayOff.MoveWindow( nWidth - m_nXBorder - 5 - rcBtAutoPlay.Width(), nHeight-5-rcBtAutoPlay.Height(), rcBtAutoPlay.Width(), rcBtAutoPlay.Height(), FALSE);
	m_btSortCard.MoveWindow( nWidth - m_nXBorder - 5 - rcBtAutoPlay.Width(), nHeight-10-2*rcBtAutoPlay.Height(), rcBtAutoPlay.Width(), rcBtAutoPlay.Height(), FALSE);

	//�ƶ��˿�
	m_BackCardControl.SetBenchmarkPos(m_nXBorder+5,m_nYBorder+5,enXLeft,enYTop);
	m_HandCardControl[ME_VIEW_CHAIR].SetBenchmarkPos(nWidth/2+15,nHeight-m_nYFace-m_nYBorder-25,enXCenter,enYBottom);
	m_UserCardControl[ME_VIEW_CHAIR].SetBenchmarkPos(nWidth/2,nHeight-223-m_nYBorder,enXCenter,enYBottom);

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

		//tagUserData *pUserData = new tagUserData;
		//strcpy( (char*)pUserData->szName, "dkjfdkfjdkfj" );
		//pUserData->cbUserStatus = US_READY;
		//wUserTimer = 10;
		//m_ImageLand.DrawImage(pDC,m_ptLand[i].x,m_ptLand[i].y);
		//m_wBombTime = 2;
		//m_bUserTrustee[ i ] = true;
		//m_wLandUser = 3;
		///////////////////////////////////////////////////////

		//�滭�û�
		if (pUserData!=NULL)
		{
			//�û�����
			pDC->SetTextAlign( ( ( i == 3 && ! m_bDeasilOrder ) || ( i == 0 && m_bDeasilOrder ) ) ? TA_RIGHT : TA_LEFT );
			pDC->SetTextColor((wUserTimer>0)?RGB(250,250,250):RGB(220,220,220));
			DrawTextString( pDC, pUserData->szName, RGB( 255, 255, 255 ), RGB( 0, 0, 0 ), m_ptName[i].x,m_ptName[i].y );

			//ʣ���˿�
		//	if (i!=ME_VIEW_CHAIR)
			{
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("ʣ�ࣺ%d ��"),m_bCardCount[i]);
				DrawTextString( pDC, szBuffer, RGB( 255, 255, 255 ), RGB( 0, 0, 0 ), m_ptName[i].x,m_ptName[i].y+18 );
			}

			//������Ϣ
			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);

			//�йܱ�ʶ
			if ( m_bUserTrustee[ i ] ) {
			if ( i == ME_VIEW_CHAIR )
				m_ImageUserTrustee.DrawImage( pDC, m_ptLand[ i ].x + 40, m_ptLand[ i ].y - 1 );
			else if ( i == 0 )
				m_ImageUserTrustee.DrawImage( pDC, m_ptLand[ i ].x + 40, m_ptFace[ i ].y + m_nYFace -
				m_ImageUserTrustee.GetHeight() + 3 );
			else if ( i == 1 )
				m_ImageUserTrustee.DrawImage( pDC, m_ptFace[ i ].x + m_nXFace + 8, m_ptFace[ i ].y + m_nYFace -
				m_ImageUserTrustee.GetHeight() + 3 );
			else
				m_ImageUserTrustee.DrawImage( pDC, m_ptFace[ i ].x - m_nXFace - 8, m_ptFace[ i ].y + m_nYFace -
				m_ImageUserTrustee.GetHeight() + 3 );
		}
		}
	}

	//�зֱ�־
	int nXImagePos,nImageWidth=m_ImageScore.GetWidth(),nImageHeight=m_ImageScore.GetHeight();
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if ((m_bScore[i]!=0)||(m_bPass[i]==true))
		{
			if ( m_bPass[i]==true ) nXImagePos=m_ImageScore.GetWidth()*4/5;
			else if ( m_bScore[i]==255 ) nXImagePos=m_ImageScore.GetWidth()*3/5;
			else nXImagePos=(m_bScore[i]-1)*m_ImageScore.GetWidth()/5;
			m_ImageScore.DrawImage(pDC,m_ptScore[i].x,m_ptScore[i].y,nImageWidth/5,nImageHeight,nXImagePos,0);
		}
	}

	//ׯ��λ��
	if (m_wLandUser!=INVALID_CHAIR)
	{
		//��ȡ�û�
		const tagUserData * pUserData=GetUserInfo(m_wLandUser);
		//tagUserData *pUserData = new tagUserData;
		//strcpy( (char*)pUserData->szName, "dkjfdkfjdkfj" );
		if (pUserData!=NULL)
		{
			//��������
			TCHAR szBuffer[64];

			//���� DC
			pDC->SetTextAlign(TA_LEFT|TA_TOP);
			pDC->SetTextColor(RGB(250,250,250));

			//�˿˸߶�
			CRect CardRect;
			m_BackCardControl.GetWindowRect( &CardRect );
			ScreenToClient( &CardRect );

			//�滭��Ϣ
			m_ImageLand.DrawImage(pDC,m_ptLand[m_wLandUser].x,m_ptLand[m_wLandUser].y);
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("������%s"),pUserData->szName);
			DrawTextString( pDC, szBuffer, RGB( 255, 255, 255 ), RGB( 0, 0, 0 ), m_nXBorder+10,CardRect.bottom+10 );

			//�滭ͷ��
		//	DrawUserFace(pDC,pUserData->wFaceID,m_nXBorder+5,m_nYBorder+5,false);

			//���Ʒ���
			LONG lScore=m_cbLandScore*m_lBaseScore;
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("������%ld ��"),lScore);
			DrawTextString( pDC, szBuffer, RGB( 255, 255, 255 ), RGB( 0, 0, 0 ), m_nXBorder+10,CardRect.bottom+30 );
		}
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
		//�˿˸߶�
		CRect CardRect;
		m_BackCardControl.GetWindowRect( &CardRect );
		ScreenToClient( &CardRect );

		//ը����־
		CImageHandle ImageHandle(&m_ImageBomb);
		m_ImageBomb.AlphaDrawImage(pDC,m_nXBorder+5,50+CardRect.bottom,RGB(255,0,255));

		//���� DC
		pDC->SetTextColor(RGB(250,250,250));
		pDC->SetTextAlign(TA_LEFT|TA_TOP);

		//ը������
		TCHAR szBuffer[64];
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("ը��������%d"),m_wBombTime);
		DrawTextString( pDC, szBuffer, RGB( 255, 255, 255 ), RGB( 0, 0, 0 ), m_nXBorder+m_ImageBomb.GetWidth()+10,CardRect.bottom+60 );
	}

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

#ifdef VIDEO_GAME
	//�����˿�
	m_HandCardControl[ 0 ].ShowWindow( SW_HIDE );
	m_HandCardControl[ 1 ].ShowWindow( SW_HIDE );
	m_HandCardControl[ 3 ].ShowWindow( SW_HIDE );
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

//����ť
VOID CGameClientView::OnBnClickedSortCard()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_SORT_HAND_CARD,0,0);

	return;
}