#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"

//#define DEBUG_GAME
//////////////////////////////////////////////////////////////////////////
//��ť��ʶ 

#define IDC_START						100								//��ʼ��ť
#define IDC_GIVE_UP_SCORE				104								//������ť
#define IDC_OUT_CARD					105								//���ư�ť
#define IDC_PASS_CARD					106								//PASS��ť
#define IDC_AUTO_OUTCARD                107                             //��ʾ��Ŧ
#define IDC_ENTHRONEMENT				108								//��λ
#define IDC_DEMISE						109								//��λ
#define IDC_BYONESELF					200								//����
#define IDC_NOTBYONESELF				201								//������
#define IDC_REBALLED					202								//�췴
#define IDC_NOTREBALLED					203								//���췴
#define IDC_KEEP						204								//����
#define IDC_HIDEKEEP					205								//����

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_ENTHRONEMENT, OnEnthronement)
	ON_BN_CLICKED(IDC_DEMISE, OnDemise)
	ON_BN_CLICKED(IDC_BYONESELF, OnByOneself)
	ON_BN_CLICKED(IDC_NOTBYONESELF, OnNotByOneself)
	ON_BN_CLICKED(IDC_REBALLED, OnReballed)
	ON_BN_CLICKED(IDC_NOTREBALLED, OnNotReballed)
	ON_BN_CLICKED(IDC_KEEP,OnKeep)
	ON_BN_CLICKED(IDC_HIDEKEEP,OnHideKeep)

	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_OUT_CARD, OnOutCard)
	ON_BN_CLICKED(IDC_PASS_CARD, OnPassCard)
	ON_BN_CLICKED(IDC_AUTO_OUTCARD, OnAutoOutCard)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView(void) : CGameFrameView(true,24)
{
	//��Ϸ����
	m_wCurrentEnthronement = -1;				
	m_wCurrentHousecarl = -1;		
	//���ñ���
	m_bDeasilOrder=true;

	//״̬����
	memset(m_bPass,0,sizeof(m_bPass));
	memset(m_bCardCount,0,sizeof(m_bCardCount));

	//λ����Ϣ
	memset(m_ptHeart,0,sizeof(m_ptHeart));
	memset(m_ptEmp_Hou,0,sizeof(m_ptEmp_Hou));

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageCenter.SetLoadInfo(IDB_VIEW_CENTER,hInstance);
	m_ImageHousecarl.SetLoadInfo(IDB_HOUSECARL,hInstance);
	m_ImageEmperor.SetLoadInfo(IDB_EMPEROR,hInstance);
	m_ImageFarmer.SetLoadInfo(IDB_FARMER,hInstance);

	m_wChairID = -1;
	m_bDif = false;
	m_bGamePlaying = false;
	m_bOutHousecarl = false;
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
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_UserCardControl[i].SetDirection(true);
		m_UserCardControl[i].SetDisplayFlag(true);
		m_UserCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,20+i);
	}
	for (WORD i=0;i<2;i++) 
	{
		//m_FaceCardControl[i].SetDirection(false);
		//m_FaceCardControl[i].SetDisplayFlag(false);
		//m_FaceCardControl[i].SetCardSpace(0,18,0)
		//m_LeaveCardControl[i].SetDirection(false);
		//m_LeaveCardControl[i].SetDisplayFlag(false);
		//m_LeaveCardControl[i].SetCardSpace(0,18,0);
	//	m_LeaveCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,30+i);
	}
	m_HandCardControl.SetSinkWindow(AfxGetMainWnd());
	m_HandCardControl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,40);

	//������ť
	m_btStart.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_START);
	m_btOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_OUT_CARD);
	m_btPassCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_PASS_CARD);
	m_btAutoOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_AUTO_OUTCARD);

	m_btEnthronement.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_ENTHRONEMENT);
	m_btDemise.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_DEMISE);
	m_btByOneself.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_BYONESELF);
	m_btNotByOneself.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_NOTBYONESELF);
	m_btRebelled.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_REBALLED);
	m_btNotRebelled.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_NOTREBALLED);
	m_btKeep.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_KEEP);
	m_btHideKeep.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_HIDEKEEP);


	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btEnthronement.SetButtonImage(IDB_START,hInstance,false);
	m_btDemise.SetButtonImage(IDB_START,hInstance,false);
	m_btByOneself.SetButtonImage(IDB_START,hInstance,false);
	m_btNotByOneself.SetButtonImage(IDB_START,hInstance,false);
	m_btRebelled.SetButtonImage(IDB_START,hInstance,false);
	m_btNotRebelled.SetButtonImage(IDB_START,hInstance,false);
	m_btKeep.SetButtonImage(IDB_START,hInstance,false);
	m_btHideKeep.SetButtonImage(IDB_START,hInstance,false);

	m_btStart.SetButtonImage(IDB_START,hInstance,false);
	m_btOutCard.SetButtonImage(IDB_OUT_CARD,hInstance,false);
	m_btPassCard.SetButtonImage(IDB_PASS,hInstance,false);
	m_btAutoOutCard.SetButtonImage(IDB_AUTO_OUT_CARD,hInstance,false);

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

	//��ȡ����
	//m_dwCardHSpace=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("CardSpace"),DEFAULT_PELS);
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
	m_HandCardControl.ShootAllCard(false);

	return;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//��Ϸ����
	
	//״̬����
	memset(m_bPass,0,sizeof(m_bPass));
	memset(m_bCardCount,0,sizeof(m_bCardCount));

	//���ؿؼ�
	m_ScoreView.ShowWindow(SW_HIDE);

	//���ذ�ť
	m_btStart.ShowWindow(SW_HIDE);
	m_btPassCard.ShowWindow(SW_HIDE);
	m_btAutoOutCard.ShowWindow(SW_HIDE);

	//���ÿؼ�
	m_btOutCard.EnableWindow(FALSE);
	m_btPassCard.EnableWindow(FALSE);
	m_btAutoOutCard.EnableWindow(FALSE);

	//�˿˿ؼ�
	m_HandCardControl.SetCardData(NULL,0);
	m_HandCardControl.SetPositively(false);
	m_HandCardControl.SetDisplayFlag(false);
	//m_LeaveCardControl[0].SetCardData(NULL,0);
	//m_LeaveCardControl[1].SetCardData(NULL,0);
	for (WORD i=0;i<GAME_PLAYER;i++) m_UserCardControl[i].SetCardData(NULL,0);

	return;
}


//#ifdef VIDEO_GAME
//	CRect VDRect;
//	m_DlgVedioService[ 0 ].GetWindowRect( &VDRect );
//	if ( m_bDeasilOrder ) {
//		m_DlgVedioService[ 2 ].MoveWindow( m_nXBorder + 5, m_ptTimer[ 0 ].y - VDRect.Height() - m_nYTimer, VDRect.Width(), VDRect.Height() );
//		m_DlgVedioService[ 0 ].MoveWindow( nWidth - VDRect.Width() - m_nXBorder - 5, m_ptTimer[ 2 ].y - VDRect.Height() - m_nYTimer, VDRect.Width(), VDRect.Height() );
//
//		m_HandCardControl[0].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-140,nHeight/2-50,enXRight,enYCenter);
//		m_HandCardControl[2].SetBenchmarkPos(m_nXBorder+m_nXFace+140,nHeight/2-55,enXLeft,enYCenter);
//	}
//	else {
//		m_DlgVedioService[ 0 ].MoveWindow( m_nXBorder + 5, m_ptTimer[ 0 ].y - VDRect.Height() - m_nYTimer, VDRect.Width(), VDRect.Height() );
//		m_DlgVedioService[ 2 ].MoveWindow( nWidth - VDRect.Width() - m_nXBorder - 5, m_ptTimer[ 2 ].y - VDRect.Height() - m_nYTimer, VDRect.Width(), VDRect.Height() );
//
//		m_HandCardControl[2].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-140,nHeight/2-50,enXRight,enYCenter);
//		m_HandCardControl[0].SetBenchmarkPos(m_nXBorder+m_nXFace+140,nHeight/2-55,enXLeft,enYCenter);
//	}
//
//	m_DlgVedioService[ 1 ].GetWindowRect( &VDRect );
//	m_DlgVedioService[ 1 ].MoveWindow( m_nXBorder + 5, nHeight - VDRect.Height() - m_nYBorder - 50, VDRect.Width(), VDRect.Height() );


//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//��������
	if (m_bDeasilOrder==true)
	{
		m_ptFace[0].x=nWidth-m_nXBorder-m_nXFace-30;
		m_ptFace[0].y=nHeight/4-m_nYFace;
		m_ptName[0].x=m_ptFace[0].x+10;
		m_ptName[0].y=nHeight/4+10;
		m_ptTimer[0].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptTimer[0].y=nHeight/4-m_nYFace-m_nYTimer-5;
		m_ptReady[0].x=nWidth-m_nXBorder-m_nXTimer-5;
		m_ptReady[0].y=nHeight/4-m_nYFace-m_nYTimer/2-5;
		m_ptHeart[0].x=m_ptName[0].x;
		m_ptHeart[0].y=m_ptName[0].y+20;

		m_ptFace[1].x=nWidth-m_nXBorder-m_nXFace-30;
		m_ptFace[1].y=nHeight/2-m_nYFace;
		m_ptName[1].x=m_ptFace[1].x+10;
		m_ptName[1].y=nHeight/2+10;
		m_ptTimer[1].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptTimer[1].y=nHeight/2-m_nYFace-m_nYTimer-5;
		m_ptReady[1].x=nWidth-m_nXBorder-m_nXTimer-5;
		m_ptReady[1].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_ptHeart[1].x=m_ptName[1].x;
		m_ptHeart[1].y=m_ptName[1].y+20;

		m_ptFace[3].x=m_nXBorder+25;
		m_ptFace[3].y=nHeight/2-m_nYFace;
		m_ptName[3].x=m_ptFace[3].x;
		m_ptName[3].y=nHeight/2+10;
		m_ptTimer[3].x=m_nXBorder+5;
		m_ptTimer[3].y=nHeight/2-m_nYFace-5-m_nYTimer;
		m_ptReady[3].x=m_nXBorder+5+m_nXTimer;
		m_ptReady[3].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_ptHeart[3].x=m_ptName[3].x-20;
		m_ptHeart[3].y=m_ptName[3].y+20;

		m_ptFace[4].x=m_nXBorder+25;
		m_ptFace[4].y=nHeight/4-m_nYFace;
		m_ptName[4].x=m_ptFace[4].x;
		m_ptName[4].y=nHeight/4+10;
		m_ptTimer[4].x=m_ptFace[4].x;
		m_ptTimer[4].y=nHeight/4-m_nYFace-5-m_nYTimer;
		m_ptReady[4].x=m_ptFace[4].x+m_nXTimer;
		m_ptReady[4].y=nHeight/4-m_nYFace-m_nYTimer/2-5;
		m_ptHeart[4].x=m_ptName[4].x-20;
		m_ptHeart[4].y=m_ptName[4].y+20;
		
	}
	else
	{
		m_ptFace[4].x=nWidth-m_nXBorder-m_nXFace-30;
		m_ptFace[4].y=nHeight/4-m_nYFace;
		m_ptName[4].x=m_ptFace[4].x+10;
		m_ptName[4].y=nHeight/4+10;
		m_ptTimer[4].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptTimer[4].y=nHeight/4-m_nYFace-m_nYTimer-5;
		m_ptReady[4].x=nWidth-m_nXBorder-m_nXTimer-5;
		m_ptReady[4].y=nHeight/4-m_nYFace-m_nYTimer/2-5;
		m_ptHeart[4].x=m_ptName[4].x-20;
		m_ptHeart[4].y=m_ptName[4].y+20;

		m_ptFace[3].x=nWidth-m_nXBorder-m_nXFace-30;
		m_ptFace[3].y=nHeight/2-m_nYFace;
		m_ptName[3].x=m_ptFace[3].x+10;
		m_ptName[3].y=nHeight/2+10;
		m_ptTimer[3].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptTimer[3].y=nHeight/2-m_nYFace-m_nYTimer-5;
		m_ptReady[3].x=nWidth-m_nXBorder-m_nXTimer-5;
		m_ptReady[3].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_ptHeart[3].x=m_ptName[3].x-20;
		m_ptHeart[3].y=m_ptName[3].y+20;

		m_ptFace[1].x=m_nXBorder+25;
		m_ptFace[1].y=nHeight/2-m_nYFace;
		m_ptName[1].x=m_ptFace[1].x;
		m_ptName[1].y=nHeight/2+10;
		m_ptTimer[1].x=m_ptFace[1].x;
		m_ptTimer[1].y=nHeight/2-m_nYFace-5-m_nYTimer;
		m_ptReady[1].x=m_ptFace[1].x+m_nXTimer;
		m_ptReady[1].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_ptHeart[1].x=m_ptName[1].x;
		m_ptHeart[1].y=m_ptName[1].y+20;

		m_ptFace[0].x=m_nXBorder+25;
		m_ptFace[0].y=nHeight/4-m_nYFace;
		m_ptName[0].x=m_ptFace[0].x;
		m_ptName[0].y=nHeight/4+10;
		m_ptTimer[0].x=m_ptFace[0].x;
		m_ptTimer[0].y=nHeight/4-m_nYFace-5-m_nYTimer;
		m_ptReady[0].x=m_ptFace[0].x+m_nXTimer;
		m_ptReady[0].y=nHeight/4-m_nYFace-m_nYTimer/2-5;
		m_ptHeart[0].x=m_ptName[0].x;
		m_ptHeart[0].y=m_ptName[0].y+20;
		
	}

	m_ptFace[2].x=nWidth/2-m_nXFace/2;
	m_ptFace[2].y=nHeight*13/14-m_nYBorder-m_nYFace+5;
	m_ptName[2].x=nWidth/2+5+m_nXFace/2+20;
	m_ptName[2].y=nHeight-m_nYBorder-m_nYFace-30;
	m_ptTimer[2].x=nWidth/2-m_nXFace/2-m_nXTimer*2-5;
	m_ptTimer[2].y=nHeight-m_nYBorder-m_nYTimer-10;
	m_ptReady[2].x=nWidth/2-m_nXTimer-m_nXFace/2-5;
	m_ptReady[2].y=nHeight-m_nYBorder-m_nYTimer/2-50;
	m_ptHeart[2].x=m_ptFace[2].x+m_nXFace+5;
	m_ptHeart[2].y=m_ptFace[2].y-10;

	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	m_btStart.GetWindowRect(&rcButton);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	DeferWindowPos(hDwp,m_btEnthronement,NULL,(nWidth-rcButton.Width())/2 - 50,nHeight-195-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btDemise,NULL,(nWidth-rcButton.Width())/2 + 50,nHeight-195-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btByOneself,NULL,(nWidth-rcButton.Width())/2 - 50,nHeight-195-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btNotByOneself,NULL,(nWidth-rcButton.Width())/2 + 50,nHeight-195-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btRebelled,NULL,(nWidth-rcButton.Width())/2 - 50,nHeight-195-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btNotRebelled,NULL,(nWidth-rcButton.Width())/2 + 50,nHeight-195-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btKeep,NULL,(nWidth-rcButton.Width())/2 - 50,nHeight-195-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btHideKeep,NULL,(nWidth-rcButton.Width())/2 + 50,nHeight-195-m_nYBorder,0,0,uFlags);
	
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight*13/14-195-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOutCard,NULL,nWidth/2-rcButton.Width()*3/2-10,nHeight*13/14-195-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btAutoOutCard,NULL,nWidth/2-rcButton.Width()/2,nHeight*13/14-195-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPassCard,NULL,nWidth/2+rcButton.Width()/2+10,nHeight*13/14-195-m_nYBorder,0,0,uFlags);
	EndDeferWindowPos(hDwp);

	//�ƶ��˿�
	m_HandCardControl.SetBenchmarkPos(nWidth/2,nHeight*13/14-m_nYFace-m_nYBorder-10,enXCenter,enYBottom);
	m_UserCardControl[2].SetBenchmarkPos(nWidth/2,nHeight-210-m_nYBorder,enXCenter,enYBottom);

	//�û��˿�
	if (m_bDeasilOrder==true)
	{
		m_UserCardControl[0].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-150,nHeight/3-140,enXRight,enYCenter);
		m_UserCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+150,nHeight*2/3-150,enXLeft,enYCenter);
		m_UserCardControl[3].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-150,nHeight*2/3-150,enXRight,enYCenter);
		m_UserCardControl[4].SetBenchmarkPos(m_nXBorder+m_nXFace+150,nHeight/3-140,enXLeft,enYCenter);

	}
	else 
	{
		m_UserCardControl[0].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-150,nHeight/3-140,enXRight,enYCenter);
		m_UserCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+150,nHeight*2/3-150,enXLeft,enYCenter);
		m_UserCardControl[3].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-150,nHeight*2/3-150,enXRight,enYCenter);
		m_UserCardControl[4].SetBenchmarkPos(m_nXBorder+m_nXFace+150,nHeight/3-140,enXLeft,enYCenter);

	}

	//������ͼ
	CRect rcControl;
	m_ScoreView.GetWindowRect(&rcControl);
	m_ScoreView.SetWindowPos(NULL,(nWidth-rcControl.Width())/2,(nHeight-rcControl.Height()-100)/2,0,0,SWP_NOZORDER|SWP_NOSIZE);

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

		//�滭�û�
		if (pUserData!=NULL)
		{
			//�û�����
			pDC->SetTextColor((wUserTimer>0)?RGB(250,250,250):RGB(220,220,220));
			pDC->SetTextAlign(((i==0)&&(m_bDeasilOrder==true))||((i==2)&&(m_bDeasilOrder==false))?TA_RIGHT:TA_LEFT);
			pDC->TextOut(m_ptName[i].x,m_ptName[i].y,pUserData->szName,lstrlen(pUserData->szName));

			//ʣ���˿�
			if (pUserData->cbUserStatus>=US_PLAY && (m_bCardCount[i] <= 10) && (m_bCardCount[i] != 0))
			{
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%ld �ţ�"),m_bCardCount[i]);
				pDC->TextOut(m_ptHeart[i].x,m_ptHeart[i].y+25,szBuffer,lstrlen(szBuffer));
			}

			//������Ϣ
			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);
			if (m_bGamePlaying)
			{
				if(i==m_wCurrentEnthronement)
				{
					CImageHandle ImageHandle(&m_ImageEmperor);
					m_ImageEmperor.BitBlt(pDC->m_hDC,m_ptHeart[i].x,m_ptHeart[i].y);

				}
				
			}
			if (m_bOutHousecarl)
			{
				if (i == m_wCurrentHousecarl)
				{
					CImageHandle ImageHandle(&m_ImageHousecarl);
					m_ImageHousecarl.BitBlt(pDC->m_hDC,m_ptHeart[i].x+30,m_ptHeart[i].y);

				}
			}
		}
	}

	
#ifdef DEBUG_GAME
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		////��������
		//WORD wUserTimer=GetUserTimer(i);
		//const tagUserData * pUserData=GetUserInfo(i);

		//�滭�û�
		//if (pUserData!=NULL)
		{
			//�û�����
			pDC->SetTextColor(RGB(250,250,250));
			pDC->SetTextAlign(((i==0)&&(m_bDeasilOrder==true))||((i==2)&&(m_bDeasilOrder==false))?TA_RIGHT:TA_LEFT);
			pDC->TextOut(m_ptName[i].x,m_ptName[i].y,"��Ϸ���");

			//ʣ���˿�
			/*if (pUserData->cbUserStatus>=US_PLAY)*/
			{
				//_snprintf(szBuffer,sizeof(szBuffer),TEXT("�� �ţ�"),15);
			//	pDC->TextOut(m_ptName[i].x,m_ptName[i].y+18,"15��");
			}

			//������Ϣ
			/*if (wUserTimer!=0)*/ DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,15);
			/*if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);*/
			DrawUserFace(pDC,5,m_ptFace[i].x,m_ptFace[i].y,true);
		}
	}

#endif
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

//���ܰ�ť����
void CGameClientView::SetBT(WORD wChairID,BYTE bIndex)
{
	m_wChairID = wChairID;
	switch( bIndex)
	{
	case 1:
	case 2:
		m_btEnthronement.ShowWindow(SW_SHOW);
		m_btDemise.ShowWindow(SW_SHOW);
		break;
	case 3:
	case 4:
		m_bDif = true;
		m_btEnthronement.ShowWindow(SW_SHOW);
		m_btDemise.ShowWindow(SW_SHOW);

		break;
	case 5:
	case 6:
		m_btRebelled.ShowWindow(SW_SHOW);
		m_btNotRebelled.ShowWindow(SW_SHOW);
		break;
	case 7:
	case 8:
		m_btKeep.ShowWindow(SW_SHOW);
		m_btHideKeep.ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}
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
	m_HandCardControl.ShootAllCard(false);
	return;
}
//������ʾ
void CGameClientView::OnAutoOutCard()
{
	AfxGetMainWnd()->SendMessage(IDM_AUTO_OUTCARD,0,0);
	return;
}

//��λ
void CGameClientView::OnDemise()
{
	m_btEnthronement.ShowWindow(SW_HIDE);
	m_btDemise.ShowWindow(SW_HIDE);
	AfxGetMainWnd()->SendMessage(IDM_EMPEROR,1,0);
	if (m_bDif)
	{
		m_btByOneself.ShowWindow(SW_SHOW);
		m_btNotByOneself.ShowWindow(SW_SHOW);
		m_bDif = false;
	}

}
//��λ
void CGameClientView::OnEnthronement()
{
	m_btEnthronement.ShowWindow(SW_HIDE);
	m_btDemise.ShowWindow(SW_HIDE);
	AfxGetMainWnd()->SendMessage(IDM_EMPEROR,1,1);
	if (m_bDif)
	{
		m_btByOneself.ShowWindow(SW_SHOW);
		m_btNotByOneself.ShowWindow(SW_SHOW);
		m_bDif = false;
	}

}
//����
void CGameClientView::OnByOneself()
{
	m_btByOneself.ShowWindow(SW_HIDE);
	m_btNotByOneself.ShowWindow(SW_HIDE);
	AfxGetMainWnd()->SendMessage(IDM_HOUSECARL,0,1);
}
//������
void CGameClientView::OnNotByOneself()
{
	m_btByOneself.ShowWindow(SW_HIDE);
	m_btNotByOneself.ShowWindow(SW_HIDE);
	AfxGetMainWnd()->SendMessage(IDM_HOUSECARL,0,0);
}
//�췴
void CGameClientView::OnReballed()
{
	m_btRebelled.ShowWindow(SW_HIDE);
	m_btNotRebelled.ShowWindow(SW_HIDE);
}
//���췴
void CGameClientView::OnNotReballed()
{
	m_btRebelled.ShowWindow(SW_HIDE);
	m_btNotRebelled.ShowWindow(SW_HIDE);
}
//����
void CGameClientView::OnKeep()
{
	m_btKeep.ShowWindow(SW_HIDE);
	m_btHideKeep.ShowWindow(SW_HIDE);
}
//����
void CGameClientView::OnHideKeep()
{
	m_btKeep.ShowWindow(SW_HIDE);
	m_btHideKeep.ShowWindow(SW_HIDE);
}
//////////////////////////////////////////////////////////////////////////

