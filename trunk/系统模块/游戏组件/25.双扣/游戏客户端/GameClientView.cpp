#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClient.h"
//////////////////////////////////////////////////////////////////////////
//��ť��ʶ

#define IDC_START						100								//��ʼ��ť
#define IDC_OUT_CARD					101								//���ư�ť
#define IDC_PASS_CARD					102								//PASS��ť
#define IDC_OUT_PROMPT					103								//��ʾ��ť
#define IDC_SORT_BY_COLOR				104								//��ɫ����
#define IDC_SORT_BY_VALUE				105								//��С����
#define IDC_TRUSTEE_CONTROL				106								//�йܿ���
//#define IDC_SHOWVIDEO					107								//��Ƶ��ʾ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_OUT_CARD, OnOutCard)
	ON_BN_CLICKED(IDC_PASS_CARD, OnPassCard)
	ON_BN_CLICKED(IDC_OUT_PROMPT, OnOutPrompt)
	ON_BN_CLICKED(IDC_SORT_BY_COLOR, OnSortByColor)
	ON_BN_CLICKED(IDC_SORT_BY_VALUE, OnSortByValue)
	ON_BN_CLICKED(IDC_TRUSTEE_CONTROL,OnStusteeControl)
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
	ZeroMemory(m_wWinOrder,sizeof(m_wWinOrder));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageViewFill.SetLoadInfo(IDB_VIEW_FILL,hInstance);
	m_ImageViewBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageUserPass.SetLoadInfo(IDB_USER_PASS,hInstance);
	ImageWinOrder.SetLoadInfo(IDB_WIN_ORDER,hInstance);
	m_ImageTrustee.LoadImage(hInstance,TEXT("TRUSTEE"));

	//����λ��
	m_PointWinOrder[0].SetPoint(0,0);
	m_PointWinOrder[1].SetPoint(0,0);
	m_PointWinOrder[2].SetPoint(0,0);
	m_PointWinOrder[3].SetPoint(0,0);

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
		//�û��˿�
		if ((i%2)==0)
		{
			m_UserCardControl[i].SetDirection(true);
			m_UserCardControl[i].SetDisplayFlag(true);
			m_UserCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,20+i);
		}
		else
		{
			m_UserCardControl[i].SetDirection(false);
			m_UserCardControl[i].SetDisplayFlag(true);
			m_UserCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,20+i);
		}

		//�û��˿�
		if ((i%2)==0)
		{
			m_HandCardControl[i].SetDirection(true);
			m_HandCardControl[i].SetDisplayFlag(false);
			m_HandCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,30+i);
		}
		else
		{
			m_HandCardControl[i].SetDirection(false);
			m_HandCardControl[i].SetDisplayFlag(false);
			m_HandCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,30+i);
		}
	}

	//�û��˿�
	m_HandCardControl[2].SetSinkWindow(AfxGetMainWnd());

	//������ť
	m_btStart.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_START);
	m_btOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_OUT_CARD);
	m_btPassCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_PASS_CARD);
	//m_btOutPrompt.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_OUT_PROMPT);
	m_btSortByColor.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_SORT_BY_COLOR);
	m_btSortByValue.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_SORT_BY_VALUE);
	m_btStusteeControl.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,CreateRect,this,IDC_TRUSTEE_CONTROL);
	//m_btShowVideo.Create(TEXT(""),WS_CHILD|WS_VISIBLE/*|WS_DISABLED*/,CreateRect,this,IDC_SHOWVIDEO);

	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_START,hInstance,false);
	m_btOutCard.SetButtonImage(IDB_OUT_CARD,hInstance,false);
	m_btPassCard.SetButtonImage(IDB_PASS,hInstance,false);
	//m_btOutPrompt.SetButtonImage(IDB_OUT_PROMPT,hInstance,false);
	m_btSortByColor.SetButtonImage(IDB_SORT_BY_COLOR,hInstance,false);
	m_btSortByValue.SetButtonImage(IDB_SORT_BY_ORDER,hInstance,false);
	m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,hInstance,false);
	//m_btShowVideo.SetButtonImage(IDB_START,hInstance,false);

	//��ȡ����
	m_bDeasilOrder=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),FALSE)?true:false;

#ifdef VIDEO_GAME
	//������Ƶ
	for (WORD i=0;i<4;i++)
	{
		//������Ƶ
		m_DlgVedioService[i].Create(NULL,NULL,WS_CHILD|WS_VISIBLE,CreateRect,this,200+i);
		m_DlgVedioService[i].InitVideoService(i==2,i==2);

		//������Ƶ
		g_VedioServiceManager.SetVideoServiceControl(i,&m_DlgVedioService[i]);
	}
#endif


	return 0;
}

//�����Ϣ
void CGameClientView::OnRButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnRButtonUp(nFlags, Point);

	//�����˿�
	m_btOutCard.EnableWindow(FALSE);
	m_HandCardControl[2].ShootAllCard(false);

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
	//״̬����
	memset(m_bPass,0,sizeof(m_bPass));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	ZeroMemory(m_wWinOrder,sizeof(m_wWinOrder));

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
	m_btStusteeControl.EnableWindow(FALSE);


	//�˿˿ؼ�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_UserCardControl[i].SetCardData(NULL,0);
		m_HandCardControl[i].SetCardData(NULL,0);
		m_HandCardControl[i].SetPositively(false);
		m_HandCardControl[i].SetDisplayFlag(false);
	}

	//����λ��
	m_PointWinOrder[0].SetPoint(0,0);
	m_PointWinOrder[1].SetPoint(0,0);
	m_PointWinOrder[2].SetPoint(0,0);
	m_PointWinOrder[3].SetPoint(0,0);

	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//��������
	int nViewHeight=nHeight-m_nYBorder;

#ifdef VIDEO_GAME
	
	CRect rect ;
	m_DlgVedioService[0].GetWindowRect(&rect) ;

	//��������
	if (m_bDeasilOrder==true)
	{
		m_ptFace[1].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptFace[1].y=nViewHeight/2-m_nYFace-35;
		m_ptName[1].x=nWidth-m_nXBorder-5-m_nXFace;
		m_ptName[1].y=m_ptFace[1].y+5;
		m_ptTimer[1].x=nWidth-m_nXBorder-m_nXTimer/2-5;
		m_ptTimer[1].y=m_ptFace[1].y-m_nYTimer+10;
		m_ptReady[1].x=nWidth-m_nXBorder-m_nXFace-155;
		m_ptReady[1].y=m_ptFace[1].y + m_nYFace/2 - 5;
		m_PointWinOrder[1].x=nWidth-m_nXBorder-m_nXTimer-45;
		m_PointWinOrder[1].y=m_ptFace[1].y-m_nYTimer*2;
		m_PointUserPass[1].x=nWidth-m_nXBorder-rect.Width()-120;
		m_PointUserPass[1].y=nHeight/2-rect.Height()/2+5;
		m_PointTrustee[1].x=nWidth-m_nXBorder-m_nXTimer*2-5;
		m_PointTrustee[1].y=m_ptFace[1].y-m_nYTimer;

		m_ptFace[3].x=m_nXBorder+5;
		m_ptFace[3].y=nViewHeight/2-m_nYFace-35;
		m_ptName[3].x=m_nXBorder+5+m_nXFace;
		m_ptName[3].y=m_ptFace[3].y+5;
		m_ptTimer[3].x=m_nXBorder+m_nXTimer/2+5;
		m_ptTimer[3].y=m_ptFace[1].y-m_nYTimer+10;
		m_ptReady[3].x=m_ptFace[3].x + m_nXFace +155;
		m_ptReady[3].y=m_ptFace[3].y+m_nYFace/2-5;
		m_PointWinOrder[3].x=m_nXBorder+m_nXTimer/2+5;
		m_PointWinOrder[3].y=m_ptFace[3].y-m_nYTimer*2;
		m_PointUserPass[3].x=m_nXBorder+rect.Width()+80;
		m_PointUserPass[3].y=nHeight/2-rect.Height()/2+5;
		m_PointTrustee[3].x=m_nXBorder+m_nXTimer*3/2+5;
		m_PointTrustee[3].y=m_ptFace[3].y-m_nYTimer;

	}
	else
	{
		m_ptFace[3].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptFace[3].y=nViewHeight/2-m_nYFace-35;
		m_ptName[3].x=nWidth-m_nXBorder-5-m_nXFace;
		m_ptName[3].y=m_ptFace[3].y+5;
		m_ptTimer[3].x=nWidth-m_nXBorder-m_nXTimer/2-5;
		m_ptTimer[3].y=m_ptName[3].y-m_nYTimer+10;
		m_ptReady[3].x=nWidth-m_nXBorder-m_nXFace-155;
		m_ptReady[3].y=m_ptFace[3].y + m_nYFace/2 - 5;
		m_PointWinOrder[3].x=nWidth-m_nXBorder-m_nXTimer-45;
		m_PointWinOrder[3].y=m_ptFace[1].y-m_nYTimer*2;
		m_PointUserPass[3].x=nWidth-m_nXBorder-rect.Width()-120;
		m_PointUserPass[3].y=nHeight/2-rect.Height()/2+5;
		m_PointTrustee[3].x=nWidth-m_nXBorder-m_nXTimer*2-5;
		m_PointTrustee[3].y=m_ptFace[3].y-m_nYTimer;

		m_ptFace[1].x=m_nXBorder+5;
		m_ptFace[1].y=nViewHeight/2-m_nYFace-35;
		m_ptName[1].x=m_nXBorder+5+m_nXFace;
		m_ptName[1].y=m_ptFace[1].y+5;
		m_ptTimer[1].x=m_nXBorder+m_nXTimer/2+5;
		m_ptTimer[1].y=m_ptFace[1].y-m_nYTimer+10;
		m_ptReady[1].x=m_ptFace[1].x + m_nXFace + 155;
		m_ptReady[1].y=m_ptFace[1].y+m_nYFace/2-5;
		m_PointWinOrder[1].x=m_nXBorder+m_nXTimer/2+5;
		m_PointWinOrder[1].y=m_ptFace[1].y-m_nYTimer*2;
		m_PointUserPass[1].x=m_nXBorder+rect.Width()+80;
		m_PointUserPass[1].y=nHeight/2-rect.Height()/2+5;
		m_PointTrustee[1].x=m_nXBorder+m_nXTimer*3/2+5;
		m_PointTrustee[1].y=m_ptFace[1].y-m_nYTimer;

	}

	m_ptFace[0].x=nWidth/2-m_nXFace/2;
	m_ptFace[0].y=m_nYBorder+5;
	m_ptName[0].x=nWidth/2+5+m_nXFace/2;
	m_ptName[0].y=m_nYBorder+5;
	m_ptTimer[0].x=m_ptName[0].x+100;
	m_ptTimer[0].y=m_nYBorder+m_nYTimer/2+5;
	m_ptReady[0].x=nWidth/2;
	m_ptReady[0].y=m_nYBorder+m_nYFace+35;
	m_PointWinOrder[0].x=m_ptFace[0].x-100;
	m_PointWinOrder[0].y=m_nYBorder+5;
	m_PointUserPass[0].x=nWidth/2-21;
	m_PointUserPass[0].y=nHeight/2-140;
	m_PointTrustee[0].x=m_ptName[0].x+100+m_nXTimer;
	m_PointTrustee[0].y=m_nYBorder+5;

	m_ptFace[2].x=nWidth/2-m_nXFace/2;
	m_ptFace[2].y=nViewHeight-m_nYFace-5;
	m_ptName[2].x=nWidth/2+5+m_nXFace/2;
	m_ptName[2].y=nViewHeight-m_nYFace-2;
	m_ptTimer[2].x=m_ptName[2].x+100;
	m_ptTimer[2].y=nViewHeight-m_nYTimer/2;
	m_ptReady[2].x=nWidth/2;
	m_ptReady[2].y=nViewHeight-m_nYBorder-220;
	m_PointWinOrder[2].x=m_ptFace[2].x-100;
	m_PointWinOrder[2].y=nViewHeight-m_nYFace-5;
	m_PointUserPass[2].x=nWidth/2-21;
	m_PointUserPass[2].y=nHeight-m_nYBorder-m_nYFace-210;
	m_PointTrustee[2].x=m_ptName[2].x+100+m_nXTimer;
	m_PointTrustee[2].y=nViewHeight-m_nYFace-5;


	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(11);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	m_btStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-193-m_nYBorder,0,0,uFlags);

	m_btOutCard.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btOutCard,NULL,nWidth/2-rcButton.Width()-40,nHeight-m_nYFace-m_nYBorder-180,0,0,uFlags);
	//DeferWindowPos(hDwp,m_btOutPrompt,NULL,nWidth/2-rcButton.Width()/2,nHeight-193-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPassCard,NULL,nWidth/2+40,nHeight-m_nYFace-m_nYBorder-180,0,0,uFlags);

	//�йܰ�ť
	m_btStusteeControl.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStusteeControl,NULL,nWidth-m_nXBorder-(rcButton.Width()+5),nHeight-m_nYBorder-5-rcButton.Height(),0,0,uFlags);

	//��Ƶ����
	CRect rcAVDlg;
	m_DlgVedioService[0].GetWindowRect(&rcAVDlg);
	if( m_bDeasilOrder==false )
	{
		DeferWindowPos(hDwp,m_DlgVedioService[3],NULL,nWidth-m_nXBorder-5-rcAVDlg.Width(),nHeight/2-3,0,0,uFlags);
		DeferWindowPos(hDwp,m_DlgVedioService[1],NULL,m_nXBorder+5,nHeight/2-3,0,0,uFlags);
	}else
	{
		DeferWindowPos(hDwp,m_DlgVedioService[1],NULL,nWidth-m_nXBorder-5-rcAVDlg.Width(),nHeight/2-3,0,0,uFlags);
		DeferWindowPos(hDwp,m_DlgVedioService[3],NULL,m_nXBorder+5,nHeight/2-3,0,0,uFlags);
	}
	DeferWindowPos(hDwp,m_DlgVedioService[0],NULL,nWidth-m_nXBorder-5-rcAVDlg.Width(),5,0,0,uFlags);
	m_DlgVedioService[2].GetWindowRect(&rcAVDlg);
	DeferWindowPos(hDwp,m_DlgVedioService[2],NULL,m_nXBorder+5,nHeight-m_nYBorder-3-rcAVDlg.Height(),0,0,uFlags);

	EndDeferWindowPos(hDwp);

	//�ƶ��˿�
	m_HandCardControl[0].SetBenchmarkPos(nWidth/2,m_nYBorder+m_nYFace,enXCenter,enYTop);
	m_HandCardControl[2].SetBenchmarkPos(nWidth/2,nHeight-m_nYFace-m_nYBorder-20,enXCenter,enYBottom);
	m_UserCardControl[0].SetBenchmarkPos(nWidth/2,nHeight/2-150,enXCenter,enYTop);
	m_UserCardControl[2].SetBenchmarkPos(nWidth/2,nHeight-205-m_nYBorder,enXCenter,enYBottom);

	//�ƶ���ť
	m_btSortByColor.MoveWindow(nWidth-m_nXBorder-70,nHeight-m_nYBorder-27,60,22);
	m_btSortByValue.MoveWindow(nWidth-m_nXBorder-70,nHeight-m_nYBorder-57,60,22);

	//�û��˿�
	if (m_bDeasilOrder==true)
	{
		m_UserCardControl[1].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-160,nHeight/2-15,enXRight,enYCenter);
		m_UserCardControl[3].SetBenchmarkPos(m_nXBorder+m_nXFace+160,nHeight/2-15,enXLeft,enYCenter);
		m_HandCardControl[1].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-70,nHeight/2-50,enXRight,enYCenter);
		m_HandCardControl[3].SetBenchmarkPos(m_nXBorder+m_nXFace+70,nHeight/2-50,enXLeft,enYCenter);
	}
	else 
	{
		m_UserCardControl[3].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-160,nHeight/2-15,enXRight,enYCenter);
		m_UserCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+160,nHeight/2-15,enXLeft,enYCenter);
		m_HandCardControl[3].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-70,nHeight/2-50,enXRight,enYCenter);
		m_HandCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+70,nHeight/2-50,enXLeft,enYCenter);
	}

	//������ͼ
	CRect rcControl;
	m_ScoreView.GetWindowRect(&rcControl);
	m_ScoreView.SetWindowPos(NULL,(nWidth-rcControl.Width())/2,(nHeight-rcControl.Height()-100)/2,0,0,SWP_NOZORDER|SWP_NOSIZE);
	//m_ScoreView.SetWindowPos(NULL,(nWidth)/2,(nHeight)/2,0,0,SWP_NOZORDER|SWP_NOSIZE);

#else
	//��������
	if (m_bDeasilOrder==true)
	{
		m_ptFace[1].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptFace[1].y=nViewHeight/2-m_nYFace-35;
		m_ptName[1].x=nWidth-m_nXBorder-5;
		m_ptName[1].y=nHeight/2-20;
		m_ptTimer[1].x=nWidth-m_nXBorder-m_nXTimer/2-5;
		m_ptTimer[1].y=m_ptFace[1].y-m_nYTimer+5;
		m_ptReady[1].x=nWidth-m_nXBorder-m_nXFace-155;
		m_ptReady[1].y=m_ptFace[1].y + m_nYFace/2 - 5;
		m_PointWinOrder[1].x=nWidth-m_nXBorder-m_nXTimer-45;
		m_PointWinOrder[1].y=m_ptFace[1].y-m_nYTimer*2;
		m_PointUserPass[1].x=nWidth-m_nXBorder-m_nXFace-205;
		m_PointUserPass[1].y=nHeight/2-m_nYFace+5;
		m_PointTrustee[1].x=nWidth-m_nXBorder-5-m_nXFace;
		m_PointTrustee[1].y=m_ptFace[1].y+m_nYTimer*2;


		m_ptFace[3].x=m_nXBorder+5;
		m_ptFace[3].y=nViewHeight/2-m_nYFace-35;
		m_ptName[3].x=m_nXBorder+10;
		m_ptName[3].y=nHeight/2-20;
		m_ptTimer[3].x=m_nXBorder+m_nXTimer/2+5;
		m_ptTimer[3].y=m_ptFace[1].y-m_nYTimer+5;
		m_ptReady[3].x=m_ptFace[3].x + m_nXFace +155;
		m_ptReady[3].y=m_ptFace[3].y+m_nYFace/2-5;
		m_PointWinOrder[3].x=m_nXBorder+m_nXTimer/2+5;
		m_PointWinOrder[3].y=m_ptFace[3].y-m_nYTimer*2;
		m_PointUserPass[3].x=m_nXBorder+m_nXFace+137;
		m_PointUserPass[3].y=nHeight/2-m_nYFace+5;
		m_PointTrustee[3].x=m_nXBorder+5;
		m_PointTrustee[3].y=m_ptFace[3].y+m_nYTimer*2;

	}
	else
	{
		m_ptFace[3].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptFace[3].y=nViewHeight/2-m_nYFace-35;
		m_ptName[3].x=nWidth-m_nXBorder-5;
		m_ptName[3].y=nHeight/2-20;
		m_ptTimer[3].x=nWidth-m_nXBorder-m_nXTimer/2-5;
		m_ptTimer[3].y=m_ptFace[3].y+-m_nYTimer+5;
		m_ptReady[3].x=nWidth-m_nXBorder-m_nXFace-155;
		m_ptReady[3].y=m_ptFace[3].y + m_nYFace/2 - 5;
		m_PointWinOrder[3].x=nWidth-m_nXBorder-m_nXTimer-45;
		m_PointWinOrder[3].y=m_ptFace[1].y-m_nYTimer*2;
		m_PointUserPass[3].x=nWidth-m_nXBorder-m_nXFace-205;
		m_PointUserPass[3].y=nHeight/2-m_nYFace+5;
		m_PointTrustee[3].x=nWidth-m_nXBorder-5-m_nXFace;
		m_PointTrustee[3].y=m_ptFace[3].y+m_nYTimer*2;

		m_ptFace[1].x=m_nXBorder+5;
		m_ptFace[1].y=nViewHeight/2-m_nYFace-35;
		m_ptName[1].x=m_nXBorder+10;
		m_ptName[1].y=nHeight/2-20;
		m_ptTimer[1].x=m_nXBorder+m_nXTimer/2+5;
		m_ptTimer[1].y=m_ptFace[1].y-m_nYTimer+5;
		m_ptReady[1].x=m_ptFace[1].x + m_nXFace + 155;
		m_ptReady[1].y=m_ptFace[1].y+m_nYFace/2-5;
		m_PointWinOrder[1].x=m_nXBorder+m_nXTimer/2+5;
		m_PointWinOrder[1].y=m_ptFace[1].y-m_nYTimer*2;
		m_PointUserPass[1].x=m_nXBorder+m_nXFace+137;
		m_PointUserPass[1].y=nHeight/2-m_nYFace+5;
		m_PointTrustee[1].x=m_nXBorder+5;
		m_PointTrustee[1].y=m_ptFace[1].y+m_nYTimer*2;
	}

	m_ptFace[0].x=nWidth/2-m_nXFace/2;
	m_ptFace[0].y=m_nYBorder+5;
	m_ptName[0].x=nWidth/2+5+m_nXFace/2;
	m_ptName[0].y=m_nYBorder+5;
	m_ptTimer[0].x=m_ptName[0].x+100;
	m_ptTimer[0].y=m_nYBorder+m_nYTimer/2+5;
	m_ptReady[0].x=nWidth/2;
	m_ptReady[0].y=m_nYBorder+m_nYFace+35;
	m_PointWinOrder[0].x=m_ptFace[0].x-100;
	m_PointWinOrder[0].y=m_nYBorder+5;
	m_PointUserPass[0].x=nWidth/2-21;
	m_PointUserPass[0].y=nHeight/2-140;
	m_PointTrustee[0].x=m_ptName[0].x+100+m_nXTimer;
	m_PointTrustee[0].y=m_nYBorder+5;

	m_ptFace[2].x=nWidth/2-m_nXFace/2;
	m_ptFace[2].y=nViewHeight-m_nYFace-5;
	m_ptName[2].x=nWidth/2+5+m_nXFace/2;
	m_ptName[2].y=nViewHeight-m_nYFace-2;
	m_ptTimer[2].x=m_ptName[2].x+100;
	m_ptTimer[2].y=nViewHeight-m_nYTimer/2;
	m_ptReady[2].x=nWidth/2;
	m_ptReady[2].y=nViewHeight-m_nYBorder-220;
	m_PointWinOrder[2].x=m_ptFace[2].x-100;
	m_PointWinOrder[2].y=nViewHeight-m_nYFace-5;
	m_PointUserPass[2].x=nWidth/2-21;
	m_PointUserPass[2].y=nHeight-m_nYBorder-m_nYFace-210;
	m_PointTrustee[2].x=m_ptName[2].x+100+m_nXTimer;
	m_PointTrustee[2].y=nViewHeight-m_nYFace-5;

	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(11);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	m_btStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-193-m_nYBorder,0,0,uFlags);

	m_btOutCard.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btOutCard,NULL,nWidth/2-rcButton.Width()-40,nHeight-m_nYFace-m_nYBorder-180,0,0,uFlags);
	//DeferWindowPos(hDwp,m_btOutPrompt,NULL,nWidth/2-rcButton.Width()/2,nHeight-193-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPassCard,NULL,nWidth/2+40,nHeight-m_nYFace-m_nYBorder-180,0,0,uFlags);

	//�йܰ�ť
	m_btStusteeControl.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStusteeControl,NULL,nWidth-m_nXBorder-(rcButton.Width()+5),nHeight-m_nYBorder-5-rcButton.Height(),0,0,uFlags);

	EndDeferWindowPos(hDwp);

	//�ƶ��˿�
	m_HandCardControl[0].SetBenchmarkPos(nWidth/2,m_nXBorder+50,enXCenter,enYTop);
	m_HandCardControl[2].SetBenchmarkPos(nWidth/2,nHeight-m_nYFace-m_nYBorder-20,enXCenter,enYBottom);
	m_UserCardControl[0].SetBenchmarkPos(nWidth/2,nHeight/2-190,enXCenter,enYTop);
	m_UserCardControl[2].SetBenchmarkPos(nWidth/2,nHeight-198-m_nYBorder,enXCenter,enYBottom);

	//�ƶ���ť
	m_btSortByColor.MoveWindow(nWidth-m_nXBorder-70,nHeight-m_nYBorder-27,60,22);
	m_btSortByValue.MoveWindow(nWidth-m_nXBorder-70,nHeight-m_nYBorder-57,60,22);


	//�û��˿�
	if (m_bDeasilOrder==true)
	{
		m_UserCardControl[1].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-140,nHeight/2-30,enXRight,enYCenter);
		m_UserCardControl[3].SetBenchmarkPos(m_nXBorder+m_nXFace+140,nHeight/2-30,enXLeft,enYCenter);
		m_HandCardControl[1].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-50,nHeight/2-50,enXRight,enYCenter);
		m_HandCardControl[3].SetBenchmarkPos(m_nXBorder+m_nXFace+50,nHeight/2-50,enXLeft,enYCenter);
	}
	else 
	{
		m_UserCardControl[3].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-140,nHeight/2-30,enXRight,enYCenter);
		m_UserCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+140,nHeight/2-30,enXLeft,enYCenter);
		m_HandCardControl[3].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-50,nHeight/2-50,enXRight,enYCenter);
		m_HandCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+50,nHeight/2-50,enXLeft,enYCenter);
	}

	//������ͼ
	CRect rcControl;
	m_ScoreView.GetWindowRect(&rcControl);
	m_ScoreView.SetWindowPos(NULL,(nWidth-rcControl.Width())/2,(nHeight-rcControl.Height()-100)/2,0,0,SWP_NOZORDER|SWP_NOSIZE);
	//m_ScoreView.SetWindowPos(NULL,(nWidth)/2,(nHeight)/2,0,0,SWP_NOZORDER|SWP_NOSIZE);



#endif

	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageViewFill,enMode_Spread);
	DrawViewImage(pDC,m_ImageViewBack,enMode_Centent);

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
			pDC->SetTextColor((wUserTimer>0)?RGB(255,255,255):RGB(0,0,0));
			pDC->SetTextAlign(((i==1)&&(m_bDeasilOrder==true))||((i==3)&&(m_bDeasilOrder==false))?TA_RIGHT:TA_LEFT);
			DrawTextString(pDC,pUserData->szName,RGB(255,255,255),RGB(0,0,0),m_ptName[i].x,m_ptName[i].y);

			//ʣ���˿�
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("ʣ�ࣺ%d ��"),m_bCardCount[i]);
			pDC->TextOut(m_ptName[i].x,m_ptName[i].y+18,szBuffer,lstrlen(szBuffer));


			//������Ϣ
			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);

			//�й�
			if(m_bTrustee[i])
			{

				m_ImageTrustee.DrawImage(pDC,m_PointTrustee[i].x,m_PointTrustee[i].y);
			}
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

	//�滭����
	CImageHandle ImageHnadleWinOrder(&ImageWinOrder);
	CSize SizeWinOrder(ImageWinOrder.GetWidth()/4,ImageWinOrder.GetHeight());

	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_wWinOrder[i]!=0)
		{
			//�滭��Ϣ
			ImageWinOrder.AlphaDrawImage(pDC,m_PointWinOrder[i].x,m_PointWinOrder[i].y,ImageWinOrder.GetWidth()/4,ImageWinOrder.GetHeight(),(m_wWinOrder[i]-1)*SizeWinOrder.cx,0,RGB(255,0,255));
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

//��������
bool CGameClientView::SetUserWinOrder(WORD wChairID, WORD wWinOrder)
{
	//���ñ���
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) m_wWinOrder[i]=wWinOrder;
	}
	else
	{
		ASSERT(wChairID<GAME_PLAYER);
		m_wWinOrder[wChairID]=wWinOrder;
	}

	//���½���
	UpdateGameView(NULL);

	return true;
}
//�����й�
void CGameClientView::SetTrustee(WORD wTrusteeUser,bool bTrustee)
{
	//У������ 
	ASSERT(wTrusteeUser>=0&&wTrusteeUser<GAME_PLAYER);

	if(m_bTrustee[wTrusteeUser] !=bTrustee)	
	{
		//��������
		m_bTrustee[wTrusteeUser]=bTrustee;

		//���½���
		UpdateGameView(NULL);
	}
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

//��ɫ����
void CGameClientView::OnSortByColor()
{
	AfxGetMainWnd()->SendMessage(IDM_SORT_BY_COLOR,0,0);
	return;
}

//��С����
void CGameClientView::OnSortByValue()
{
	AfxGetMainWnd()->SendMessage(IDM_SORT_BY_VALUE,0,0);
	return;
}
//�Ϲܿ���
void CGameClientView::OnStusteeControl()
{
	AfxGetMainWnd()->SendMessage(IDM_TRUSTEE_CONTROL,0,0);
	return;
}
//��Ƶ��ʾ
/*void CGameClientView::OnShowVideo()
{
ASSERT(m_AVSendDlg.GetSafeHwnd());
if(m_AVSendDlg.IsWindowVisible())
return;
else
m_AVSendDlg.ShowWindow(SW_SHOW);
}*/
//////////////////////////////////////////////////////////////////////////

