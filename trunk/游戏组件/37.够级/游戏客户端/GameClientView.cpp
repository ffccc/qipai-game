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

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()

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
	
	//���ñ���
	m_bDeasilOrder=true;

	//״̬����
	memset(m_bPass,0,sizeof(m_bPass));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(HaveEnd,0,sizeof(HaveEnd));

	//λ����Ϣ
	memset(m_ptHeart,0,sizeof(m_ptHeart));

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageCenter.SetLoadInfo(IDB_VIEW_CENTER,hInstance);
	
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

	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_START,hInstance,false);
	m_btOutCard.SetButtonImage(IDB_OUT_CARD,hInstance,false);
	m_btPassCard.SetButtonImage(IDB_PASS,hInstance,false);
	m_btAutoOutCard.SetButtonImage(IDB_AUTO_OUT_CARD,hInstance,false);

	//��ȡ����
	m_bDeasilOrder=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),FALSE)?true:false;

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

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//��������
	if (m_bDeasilOrder==true)
	{
		m_ptFace[1].x=nWidth-m_nXBorder-m_nXFace-30;
		m_ptFace[1].y=nHeight/3-m_nYFace;
		m_ptName[1].x=m_ptFace[1].x+10;
		m_ptName[1].y=nHeight/3+10;
		m_ptTimer[1].x=nWidth-m_nXBorder-m_nXFace+10;
		m_ptTimer[1].y=nHeight/3-m_nYFace-m_nYTimer-5;
		m_ptReady[1].x=nWidth-m_nXBorder-m_nXTimer-5;
		m_ptReady[1].y=nHeight/3-m_nYFace-m_nYTimer/2-5;
		m_ptHeart[1].x=m_ptName[1].x;
		m_ptHeart[1].y=m_ptName[1].y+20;

		m_ptFace[2].x=nWidth-m_nXBorder-m_nXFace-30;
		m_ptFace[2].y=nHeight*2/3-m_nYFace;
		m_ptName[2].x=m_ptFace[2].x+10;
		m_ptName[2].y=nHeight*2/3+10;
		m_ptTimer[2].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptTimer[2].y=nHeight*2/3-m_nYFace-m_nYTimer-5;
		m_ptReady[2].x=nWidth-m_nXBorder-m_nXTimer-5;
		m_ptReady[2].y=nHeight*2/3-m_nYFace-m_nYTimer/2-5;
		m_ptHeart[2].x=m_ptName[2].x;
		m_ptHeart[2].y=m_ptName[2].y+20;

		m_ptFace[4].x=m_nXBorder+45;
		m_ptFace[4].y=nHeight*2/3-m_nYFace;
		m_ptName[4].x=m_ptFace[4].x+10;
		m_ptName[4].y=nHeight*2/3+10;
		m_ptTimer[4].x=m_nXBorder+5;
		m_ptTimer[4].y=nHeight*2/3-m_nYFace-5-m_nYTimer;
		m_ptReady[4].x=m_nXBorder+5+m_nXTimer;
		m_ptReady[4].y=nHeight*2/3-m_nYFace-m_nYTimer/2-5;
		m_ptHeart[4].x=m_ptName[4].x;
		m_ptHeart[4].y=m_ptName[4].y+20;

		m_ptFace[5].x=m_nXBorder+45;
		m_ptFace[5].y=nHeight/3-m_nYFace;
		m_ptName[5].x=m_ptFace[5].x+10;
		m_ptName[5].y=nHeight/3+10;
		m_ptTimer[5].x=m_nXBorder+5;
		m_ptTimer[5].y=nHeight/3-m_nYFace-5-m_nYTimer;
		m_ptReady[5].x=m_nXBorder+5+m_nXTimer;
		m_ptReady[5].y=nHeight/3-m_nYFace-m_nYTimer/2-5;
		m_ptHeart[5].x=m_ptName[5].x;
		m_ptHeart[5].y=m_ptName[5].y+20;
		
	}
	else
	{
		m_ptFace[5].x=nWidth-m_nXBorder-m_nXFace-30;
		m_ptFace[5].y=nHeight/3-m_nYFace;
		m_ptName[5].x=m_ptFace[5].x+10;
		m_ptName[5].y=nHeight/3+10;
		m_ptTimer[5].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptTimer[5].y=nHeight/3-m_nYFace-m_nYTimer-5;
		m_ptReady[5].x=nWidth-m_nXBorder-m_nXTimer-5;
		m_ptReady[5].y=nHeight/3-m_nYFace-m_nYTimer/2-5;
		m_ptHeart[5].x=m_ptName[5].x;
		m_ptHeart[5].y=m_ptName[5].y+20;

		m_ptFace[4].x=nWidth-m_nXBorder-m_nXFace-30;
		m_ptFace[4].y=nHeight*2/3-m_nYFace;
		m_ptName[4].x=m_ptFace[4].x+10;
		m_ptName[4].y=nHeight*2/3+10;
		m_ptTimer[4].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptTimer[4].y=nHeight*2/3-m_nYFace-m_nYTimer-5;
		m_ptReady[4].x=nWidth-m_nXBorder-m_nXTimer-5;
		m_ptReady[4].y=nHeight*2/3-m_nYFace-m_nYTimer/2-5;
		m_ptHeart[4].x=m_ptName[4].x;
		m_ptHeart[4].y=m_ptName[4].y+20;

		m_ptFace[2].x=m_nXBorder+45;
		m_ptFace[2].y=nHeight*2/3-m_nYFace;
		m_ptName[2].x=m_ptFace[2].x+10;
		m_ptName[2].y=nHeight*2/3+10;
		m_ptTimer[2].x=m_ptFace[2].x;
		m_ptTimer[2].y=nHeight*2/3-m_nYFace-5-m_nYTimer;
		m_ptReady[2].x=m_ptFace[2].x+m_nXTimer;
		m_ptReady[2].y=nHeight*2/3-m_nYFace-m_nYTimer/2-5;
		m_ptHeart[2].x=m_ptName[2].x;
		m_ptHeart[2].y=m_ptName[2].y+20;

		m_ptFace[1].x=m_nXBorder+45;
		m_ptFace[1].y=nHeight/3-m_nYFace;
		m_ptName[1].x=m_ptFace[1].x+10;
		m_ptName[1].y=nHeight/3+10;
		m_ptTimer[1].x=m_ptFace[1].x;
		m_ptTimer[1].y=nHeight/3-m_nYFace-5-m_nYTimer;
		m_ptReady[1].x=m_ptFace[1].x+m_nXTimer;
		m_ptReady[1].y=nHeight/3-m_nYFace-m_nYTimer/2-5;
		m_ptHeart[1].x=m_ptName[1].x;
		m_ptHeart[1].y=m_ptName[1].y+20;
		
	}

	m_ptFace[3].x=50;
	m_ptFace[3].y=nHeight*13/14-m_nYBorder-m_nYFace-35;
	m_ptName[3].x=m_ptFace[3].x;
	m_ptName[3].y=m_ptFace[3].y+m_nYFace+2;
	m_ptTimer[3].x=m_ptFace[3].x+m_nXTimer+25;
	m_ptTimer[3].y=m_ptFace[3].y;
	m_ptReady[3].x=m_ptFace[3].x;
	m_ptReady[3].y=m_ptFace[3].y-30;
	m_ptHeart[3].x=m_ptFace[3].x+m_nXFace+10;
	m_ptHeart[3].y=m_ptFace[3].y+10;

	m_ptFace[0].x=nWidth/2-m_nXFace/2;
	m_ptFace[0].y=m_nYFace;
	m_ptName[0].x=nWidth/2+5+m_nXFace/2;
	m_ptName[0].y=m_nYFace;
	m_ptTimer[0].x=nWidth/2-m_nXFace/2-m_nXTimer*2-5;
	m_ptTimer[0].y=m_nYFace+m_nXTimer+5;
	m_ptReady[0].x=nWidth/2-m_nXTimer-m_nXFace/2-5;
	m_ptReady[0].y=m_nYFace;
	m_ptHeart[0].x=m_ptFace[0].x+m_nXFace+10;
	m_ptHeart[0].y=m_nYFace;
	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	m_btStart.GetWindowRect(&rcButton);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
		
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight*13/14-195-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOutCard,NULL,nWidth/2-rcButton.Width()*3/2-10,nHeight*13/14-195-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btAutoOutCard,NULL,nWidth/2-rcButton.Width()/2,nHeight*13/14-195-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPassCard,NULL,nWidth/2+rcButton.Width()/2+10,nHeight*13/14-195-m_nYBorder,0,0,uFlags);
	EndDeferWindowPos(hDwp);

	//�ƶ��˿�
	m_HandCardControl.SetBenchmarkPos(nWidth/2,nHeight-m_nYFace-m_nYBorder-10,enXCenter,enYBottom);
	m_UserCardControl[3].SetBenchmarkPos(nWidth/2,nHeight*13/14-210-m_nYBorder,enXCenter,enYBottom);
	m_UserCardControl[0].SetBenchmarkPos(nWidth/2,m_nYFace+80,enXLeft,enYCenter);
	//�û��˿�
	if (m_bDeasilOrder==true)
	{
		m_UserCardControl[1].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-150,nHeight/3-30,enXRight,enYCenter);
		m_UserCardControl[2].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-150,nHeight*2/3-30,enXRight,enYCenter);
		m_UserCardControl[4].SetBenchmarkPos(m_nXBorder+m_nXFace+150,nHeight*2/3-80,enXLeft,enYCenter);
		m_UserCardControl[5].SetBenchmarkPos(m_nXBorder+m_nXFace+150,nHeight/3-30,enXLeft,enYCenter);


	}
	else 
	{
		m_UserCardControl[5].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-150,nHeight/3-30,enXRight,enYCenter);
		m_UserCardControl[4].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-150,nHeight*2/3-30,enXRight,enYCenter);
		m_UserCardControl[2].SetBenchmarkPos(m_nXBorder+m_nXFace+150,nHeight*2/3-100,enXLeft,enYCenter);
		m_UserCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+150,nHeight/3-30,enXLeft,enYCenter);


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
				pDC->TextOut(m_ptName[i].x,m_ptName[i].y+18,szBuffer,lstrlen(szBuffer));
			}

			//������Ϣ
			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData->wFaceID,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);
		
			CString ss;
			ss.Format("%s",HaveEnd[i]);
			pDC->SetBkMode(1);
			if (ss.IsEmpty()==FALSE)
				pDC->TextOut(m_ptName[i].x,m_ptName[i].y+30,ss);
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

//��ʼ��ť
void CGameClientView::OnStart()
{
	memset(HaveEnd,0,sizeof(HaveEnd));
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

//////////////////////////////////////////////////////////////////////////

