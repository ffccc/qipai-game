#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////
//��ť��ʶ

#define IDC_START						100								//��ʼ��ť
#define IDC_OUT_CARD					101								//���ư�ť
#define IDC_PASS_CARD					102								//PASS��ť
#define IDC_OUT_PROMPT					103								//��ʾ��ť

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_OUT_CARD, OnOutCard)
	ON_BN_CLICKED(IDC_PASS_CARD, OnPassCard)
	ON_BN_CLICKED(IDC_OUT_PROMPT, OnOutPrompt)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView(void) : CGameFrameView(true,24)
{
	//���ñ���
	m_bDeasilOrder=true;

	//״̬����
	memset(m_bPass,0,sizeof(m_bPass));
	memset(m_bCardCount,0,sizeof(m_bCardCount));

	//��ʷ����
	memset(m_lAllTurnScore,0,sizeof(m_lAllTurnScore));
	memset(m_lLastTurnScore,0,sizeof(m_lLastTurnScore));

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageBackGround.SetLoadInfo(IDB_BACK_GROUND,hInstance);
	m_ImageLc.SetLoadInfo(IDB_LC,hInstance);
	m_ImageRc.SetLoadInfo(IDB_RC,hInstance);
	m_ImageLogo.SetLoadInfo(IDB_KK_LOGO,hInstance);
	m_ImageHeadBg.SetLoadInfo(IDB_HEAD_BG,hInstance);
	m_ImageUserPass.SetLoadInfo(IDB_USER_PASS,hInstance);

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
	m_ScoreView.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,CreateRect,this,9);

	//�����˿�
	for (WORD i=0;i<4;i++)
	{
		m_UserCardControl[i].SetDirection(true);
		m_UserCardControl[i].SetDisplayFlag(true);
		m_UserCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,20+i);
	}
	for (WORD i=0;i<3;i++)
	{
		m_LeaveCardControl[i].SetDisplayFlag(true);
		m_LeaveCardControl[i].SetCardSpace(20,18,0);
		m_LeaveCardControl[i].SetDirection((i==0)?true:false);
		m_LeaveCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,30+i);
	}
	m_HandCardControl.SetSinkWindow(AfxGetMainWnd());
	m_HandCardControl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,40);

	//������ť
	m_btStart.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_START);
	m_btOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_OUT_CARD);
	m_btPassCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_PASS_CARD);
	//m_btOutPrompt.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_OUT_PROMPT);

	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_START,hInstance,false);
	m_btOutCard.SetButtonImage(IDB_OUT_CARD,hInstance,false);
	m_btPassCard.SetButtonImage(IDB_PASS,hInstance,false);
	//m_btOutPrompt.SetButtonImage(IDB_OUT_PROMPT,hInstance,false);

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
	//״̬����
	memset(m_bPass,0,sizeof(m_bPass));
	memset(m_bCardCount,0,sizeof(m_bCardCount));

	//��ʷ����
	memset(m_lAllTurnScore,0,sizeof(m_lAllTurnScore));
	memset(m_lLastTurnScore,0,sizeof(m_lLastTurnScore));

	//���ؿؼ�
	m_ScoreView.ShowWindow(SW_HIDE);

	//���ذ�ť
	m_btStart.ShowWindow(SW_HIDE);
	m_btOutCard.ShowWindow(SW_HIDE);
	m_btPassCard.ShowWindow(SW_HIDE);
	//m_btOutPrompt.ShowWindow(SW_HIDE);

	//���ÿؼ�
	m_btOutCard.EnableWindow(FALSE);
	m_btPassCard.EnableWindow(FALSE);
	//m_btOutPrompt.EnableWindow(FALSE);

	//�˿˿ؼ�
	m_HandCardControl.SetCardData(NULL,0);
	m_HandCardControl.SetPositively(false);
	m_HandCardControl.SetDisplayFlag(false);
	m_LeaveCardControl[0].SetCardData(NULL,0);
	m_LeaveCardControl[1].SetCardData(NULL,0);
	m_LeaveCardControl[2].SetCardData(NULL,0);
	for (WORD i=0;i<GAME_PLAYER;i++) m_UserCardControl[i].SetCardData(NULL,0);

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
		m_ptName[1].x=nWidth-m_nXBorder-m_nXFace-10;
		m_ptName[1].y=nHeight/2-m_nYFace+3;
		m_ptTimer[1].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptTimer[1].y=nHeight/2-m_nYFace-m_nYTimer-5;
		m_ptReady[1].x=nWidth-m_nXBorder-m_nXTimer-5;
		m_ptReady[1].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_PointUserPass[1].x=nWidth-m_nXBorder-m_nXFace-150;
		m_PointUserPass[1].y=nHeight/2-m_nYFace+5;

		m_ptFace[3].x=m_nXBorder+5;
		m_ptFace[3].y=nHeight/2-m_nYFace;
		m_ptName[3].x=m_nXBorder+m_nXFace+10;
		m_ptName[3].y=nHeight/2-m_nYFace+3;
		m_ptTimer[3].x=m_nXBorder+5;
		m_ptTimer[3].y=nHeight/2-m_nYFace-5-m_nYTimer;
		m_ptReady[3].x=m_nXBorder+5+m_nXTimer;
		m_ptReady[3].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_PointUserPass[3].x=m_nXBorder+m_nXFace+107;
		m_PointUserPass[3].y=nHeight/2-m_nYFace+5;
	}
	else
	{
		m_ptFace[3].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptFace[3].y=nHeight/2-m_nYFace;
		m_ptName[3].x=nWidth-m_nXBorder-m_nXFace-10;
		m_ptName[3].y=nHeight/2-m_nYFace+3;
		m_ptTimer[3].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptTimer[3].y=nHeight/2-m_nYFace-m_nYTimer-5;
		m_ptReady[3].x=nWidth-m_nXBorder-m_nXTimer-5;
		m_ptReady[3].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_PointUserPass[3].x=nWidth-m_nXBorder-m_nXFace-150;
		m_PointUserPass[3].y=nHeight/2-m_nYFace+5;

		m_ptFace[1].x=m_nXBorder+5;
		m_ptFace[1].y=nHeight/2-m_nYFace;
		m_ptName[1].x=m_nXBorder+m_nXFace+10;
		m_ptName[1].y=nHeight/2-m_nYFace+3;
		m_ptTimer[1].x=m_nXBorder+5;
		m_ptTimer[1].y=nHeight/2-m_nYFace-5-m_nYTimer;
		m_ptReady[1].x=m_nXBorder+5+m_nXTimer;
		m_ptReady[1].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
		m_PointUserPass[1].x=m_nXBorder+m_nXFace+107;
		m_PointUserPass[1].y=nHeight/2-m_nYFace+5;
	}

	m_ptFace[0].x=nWidth/2-m_nXFace/2;
	m_ptFace[0].y=m_nYBorder+5;
	m_ptName[0].x=nWidth/2+5+m_nXFace/2;
	m_ptName[0].y=m_nYBorder+2;
	m_ptTimer[0].x=nWidth/2-m_nXFace/2-m_nXTimer*2-5;
	m_ptTimer[0].y=m_nYBorder+10;
	m_ptReady[0].x=nWidth/2-m_nXTimer-m_nXFace/2-5;
	m_ptReady[0].y=m_nYBorder+m_nYTimer/2+10;
	m_PointUserPass[0].x=nWidth/2-21;
	m_PointUserPass[0].y=nHeight/2-140;

	m_ptFace[2].x=nWidth/2-m_nXFace/2;
	m_ptFace[2].y=nHeight-m_nYBorder-m_nYFace-5;
	m_ptName[2].x=nWidth/2+5+m_nXFace/2;
	m_ptName[2].y=nHeight-m_nYBorder-m_nYFace-2;
	m_ptTimer[2].x=nWidth/2-m_nXFace/2-m_nXTimer*2-5;
	m_ptTimer[2].y=nHeight-m_nYBorder-m_nYTimer-10;
	m_ptReady[2].x=nWidth/2-m_nXTimer-m_nXFace/2-5;
	m_ptReady[2].y=nHeight-m_nYBorder-m_nYTimer/2-10;
	m_PointUserPass[2].x=nWidth/2-21;
	m_PointUserPass[2].y=nHeight-m_nYBorder-m_nYFace-210;

	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(11);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	m_btStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-220-m_nYBorder,0,0,uFlags);

	m_btOutCard.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btOutCard,NULL,nWidth/2-rcButton.Width()*3/2+30,nHeight-240-m_nYBorder,0,0,uFlags);
	//DeferWindowPos(hDwp,m_btOutPrompt,NULL,nWidth/2-rcButton.Width()/2,nHeight-193-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPassCard,NULL,nWidth/2+rcButton.Width()/2-30,nHeight-240-m_nYBorder,0,0,uFlags);

	EndDeferWindowPos(hDwp);

	//�ƶ��˿�
	m_HandCardControl.SetBenchmarkPos(nWidth/2,nHeight-m_nYFace-m_nYBorder-10,enXCenter,enYBottom);
	m_UserCardControl[0].SetBenchmarkPos(nWidth/2,nHeight/2-208,enXCenter,enYTop);
	m_UserCardControl[2].SetBenchmarkPos(nWidth/2,nHeight-198-m_nYBorder,enXCenter,enYBottom);
	m_LeaveCardControl[0].SetBenchmarkPos(nWidth/2,m_nXBorder+50,enXCenter,enYTop);

	//�û��˿�
	if (m_bDeasilOrder==true)
	{
		m_UserCardControl[1].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-130,nHeight/2-30,enXRight,enYCenter);
		m_UserCardControl[3].SetBenchmarkPos(m_nXBorder+m_nXFace+130,nHeight/2-30,enXLeft,enYCenter);
		m_LeaveCardControl[1].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-50,nHeight/2-50,enXRight,enYCenter);
		m_LeaveCardControl[2].SetBenchmarkPos(m_nXBorder+m_nXFace+50,nHeight/2-50,enXLeft,enYCenter);
	}
	else 
	{
		m_UserCardControl[3].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-130,nHeight/2-30,enXRight,enYCenter);
		m_UserCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+130,nHeight/2-30,enXLeft,enYCenter);
		m_LeaveCardControl[2].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-50,nHeight/2-50,enXRight,enYCenter);
		m_LeaveCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+50,nHeight/2-50,enXLeft,enYCenter);
	}

	//������ͼ
	CRect rcControl;
	m_ScoreView.GetWindowRect(&rcControl);
	m_ScoreView.SetWindowPos(NULL,(nWidth-rcControl.Width())/2,nHeight/2-160,0,0,SWP_NOZORDER|SWP_NOSIZE);

	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageBackGround,enMode_Spread);
	DrawViewImage(pDC,m_ImageBack,enMode_Centent);

	CImageHandle HandleLc(&m_ImageLc);
	CImageHandle HandleRc(&m_ImageRc);
	CImageHandle HandleLogo(&m_ImageLogo);
	CImageHandle HandleHeadBg(&m_ImageHeadBg);

	m_ImageLogo.AlphaDrawImage(pDC,m_nXBorder+10,m_nYBorder,RGB(255,0,255));
	m_ImageLc.AlphaDrawImage(pDC,m_nXBorder,nHeight-m_ImageLc.GetHeight()-m_nYBorder,RGB(255,0,255));
	m_ImageRc.AlphaDrawImage(pDC,nWidth-m_ImageRc.GetWidth()-m_nXBorder,m_nYBorder,RGB(255,0,255));

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
			m_ImageHeadBg.BitBlt(pDC->m_hDC,m_ptFace[i].x-2,m_ptFace[i].y-2);
			//�û�����
			pDC->SetTextColor((wUserTimer>0)?RGB(250,250,250):RGB(220,220,220));
			pDC->SetTextAlign(((i==1)&&(m_bDeasilOrder==true))||((i==3)&&(m_bDeasilOrder==false))?TA_RIGHT:TA_LEFT);
			pDC->TextOut(m_ptName[i].x,m_ptName[i].y,pUserData->szName,lstrlen(pUserData->szName));

			//ʣ���˿�
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%ld �ţ�"),m_bCardCount[i]);
			pDC->TextOut(m_ptName[i].x,m_ptName[i].y+18,szBuffer,lstrlen(szBuffer));

			//������Ϣ
			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData->wFaceID,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);
		}
	}

	//������־
	CImageHandle ImageHnadleUserPass(&m_ImageUserPass);
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_bPass[i]==true) 
		{
			m_ImageUserPass.AlphaDrawImage(pDC,m_PointUserPass[i].x,m_PointUserPass[i].y,RGB(255,0,255));
		}
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

//��ʾ��ť
void CGameClientView::OnOutPrompt()
{
	AfxGetMainWnd()->SendMessage(IDM_OUT_PROMPT,0,0);
	return;
}

//////////////////////////////////////////////////////////////////////////

