#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"
#include ".\gameclientview.h"
#include "GameClient.h"

//////////////////////////////////////////////////////////////////////////
//��ť��ʶ 

#define IDC_START						100								//��ʼ��ť
#define IDC_TRUSTEE_CONTROL				104								//�йܿ���

//������ʶ
#define IDI_BOMB_EFFECT					101								//������ʶ
//������Ŀ
#define BOMB_EFFECT_COUNT				12								//������Ŀ
#define DISC_EFFECT_COUNT				8								//����Ч��		

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_TRUSTEE_CONTROL,OnStusteeControl)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//��־����
	m_bOutCard=false;
	m_bWaitOther=false;
	m_bHuangZhuang=false;
	ZeroMemory(m_bListenStatus,sizeof(m_bListenStatus));

	//��Ϸ����
	m_lCellScore=0L;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//��������
	m_bBombEffect=false;
	m_cbBombFrameIndex=0;

	//����Ч��
	m_wDiscUser=INVALID_CHAIR;
	m_cbDiscFrameIndex=0;

	//�û�״̬
	m_cbCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageWait.SetLoadInfo(IDB_WAIT_TIP,hInstance);
	m_ImageBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageUserFlag.SetLoadInfo(IDB_USER_FLAG,hInstance);
	m_ImageOutCard.SetLoadInfo(IDB_OUT_CARD_TIP,hInstance);
	m_ImageUserAction.SetLoadInfo(IDB_USER_ACTION,hInstance);
	m_ImageActionBack.SetLoadInfo(IDB_ACTION_BACK,hInstance);
	m_ImageHuangZhuang.SetLoadInfo(IDB_HUANG_ZHUANG,hInstance);
	m_ImageListenStatusH.SetLoadInfo(IDB_LISTEN_FLAG_H,hInstance);
	m_ImageListenStatusV.SetLoadInfo(IDB_LISTEN_FLAG_V,hInstance);
	m_ImageTrustee.LoadImage(hInstance,TEXT("TRUSTEE"));
	m_ImageActionAni.LoadImage(AfxGetInstanceHandle(),TEXT("ActionAni"));
	m_ImageDisc.LoadImage(AfxGetInstanceHandle(),TEXT("DISC"));
	m_ImageArrow.LoadImage(AfxGetInstanceHandle(),TEXT("ARROW"));
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

	//��������
	enDirection Direction[]={Direction_North,Direction_East,Direction_South,Direction_West};

	//���ÿؼ�
	for (WORD i=0;i<4;i++)
	{
		//�û��˿�
		m_HeapCard[i].SetDirection(Direction[i]);
		m_TableCard[i].SetDirection(Direction[i]);
		m_DiscardCard[i].SetDirection(Direction[i]);

		//����˿�
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
	m_ScoreControl.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rcCreate,this,200);
	m_ControlWnd.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,10,NULL);

	//�����ؼ�
	m_btStart.Create(NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,IDC_START);
	m_btStart.SetButtonImage(IDB_BT_START,AfxGetInstanceHandle(),false);

	//�йܰ�ť
	m_btStusteeControl.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_TRUSTEE_CONTROL);
	m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false);
	//������Ƶ
	for (WORD i=0;i<4;i++)
	{
		//������Ƶ
		m_DlgVedioService[i].Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,300+i);
		m_DlgVedioService[i].InitVideoService(i==2,true);

		//������Ƶ
		g_VedioServiceManager.SetVideoServiceControl(i,&m_DlgVedioService[i]);
	}

	SetTimer(IDI_DISC_EFFECT,250,NULL);

	return 0;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//��־����
	m_bOutCard=false;
	m_bWaitOther=false;
	m_bHuangZhuang=false;
	ZeroMemory(m_bListenStatus,sizeof(m_bListenStatus));

	//��Ϸ����
	m_lCellScore=0L;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//��������
	m_bBombEffect=false;
	m_cbBombFrameIndex=0;

	//����Ч��
	m_wDiscUser=INVALID_CHAIR;
	m_cbDiscFrameIndex=0;


	//�û�״̬
	m_cbCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));

	//��������
	m_btStart.ShowWindow(SW_HIDE);
	m_ControlWnd.ShowWindow(SW_HIDE);
	m_ScoreControl.RestorationData();
	
	//���ÿؼ�
	m_btStusteeControl.EnableWindow(FALSE);


	//�˿�����
	m_UserCard[0].SetCardData(0,false);
	m_UserCard[1].SetCardData(0,false);
	m_UserCard[2].SetCardData(0,false);
	m_HandCardControl.SetPositively(false);
	m_HandCardControl.SetDisplayItem(false);
	m_HandCardControl.SetCardData(NULL,0,0);

	//�˿�����
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

	//���ٶ�ʱ��
	KillTimer(IDI_DISC_EFFECT);
	KillTimer(IDI_BOMB_EFFECT);

	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//��������
	m_ptFace[0].x=nWidth/2-m_nXFace;
	m_ptFace[0].y=5+m_nYBorder;
	m_ptName[0].x=nWidth/2+5;
	m_ptName[0].y=20+m_nYBorder;
	m_ptTimer[0].x=nWidth/2-m_nXFace-m_nXTimer-2;
	m_ptTimer[0].y=17+m_nYBorder;
	m_ptReady[0].x=nWidth/2-m_nXFace-m_nXTimer*2-5;
	m_ptReady[0].y=5+m_nYBorder+m_nYTimer/2;
	m_UserFlagPos[0].x=nWidth/2-m_nXFace-m_nXTimer-32;
	m_UserFlagPos[0].y=5+m_nYBorder;
	m_UserListenPos[0].x=nWidth/2;
	m_UserListenPos[0].y=m_nYBorder+100;
	m_PointTrustee[0].x=nWidth/2-m_nXFace-20-m_nXFace/2;
	m_PointTrustee[0].y=5+m_nYBorder;


	m_ptFace[1].x=nWidth-m_nXBorder-m_nXFace-5;
	m_ptFace[1].y=nHeight/2-m_nYFace-50;
	m_ptName[1].x=nWidth-m_nXBorder-5;
	m_ptName[1].y=nHeight/2-45;
	m_ptTimer[1].x=nWidth-m_nXBorder-m_nXFace+5;
	m_ptTimer[1].y=nHeight/2-m_nYFace-27-m_nYTimer;
	m_ptReady[1].x=nWidth-m_nXBorder-m_nXTimer+3;
	m_ptReady[1].y=nHeight/2-m_nYFace-55-m_nYTimer/2+m_nYFace*2+40;
	m_UserFlagPos[1].x=nWidth-m_nXBorder-m_nXTimer+8;
	m_UserFlagPos[1].y=nHeight/2-m_nYFace-83-m_nYTimer;
	m_UserListenPos[1].x=nWidth-m_nXBorder-93;
	m_UserListenPos[1].y=nHeight/2-75;
	m_PointTrustee[1].x=nWidth-m_nXBorder-m_nXTimer+8;
	m_PointTrustee[1].y=nHeight/2-m_nYFace-50-35;

	m_ptFace[2].x=nWidth/2-m_nXFace;
	m_ptFace[2].y=nHeight-m_nYBorder-m_nYFace-5;
	m_ptName[2].x=nWidth/2+5+m_nXFace/2;
	m_ptName[2].y=nHeight-m_nYBorder-m_nYFace+8;
	m_ptTimer[2].x=nWidth/2-m_nXFace/2-m_nXTimer-2;
	m_ptTimer[2].y=nHeight-m_nYBorder-m_nYTimer-8+40;
	m_ptReady[2].x=nWidth/2-m_nXTimer*2-m_nXFace/2-5;
	m_ptReady[2].y=nHeight-m_nYBorder-m_nYTimer/2-10;
	m_UserFlagPos[2].x=nWidth/2-m_nXFace-m_nXTimer-32;
	m_UserFlagPos[2].y=nHeight-m_nYBorder-35;
	m_UserListenPos[2].x=nWidth/2;
	m_UserListenPos[2].y=nHeight-m_nYBorder-123;
	m_PointTrustee[2].x=nWidth/2-m_nXFace-20-m_nXFace/2;
	m_PointTrustee[2].y=nHeight-m_nYBorder-m_nYFace-5;

	m_ptFace[3].x=m_nXBorder+5;
	m_ptFace[3].y=nHeight/2-m_nYFace-50;
	m_ptName[3].x=m_nXBorder+5;
	m_ptName[3].y=nHeight/2-45;
	m_ptTimer[3].x=m_nXBorder+22;
	m_ptTimer[3].y=nHeight/2-m_nYFace-27-m_nYTimer;
	m_ptReady[3].x=m_nXBorder-2+m_nXTimer;
	m_ptReady[3].y=nHeight/2-m_nYFace-55-m_nYTimer/2+m_nYFace*2+40;
	m_UserFlagPos[3].x=m_nXBorder+8;
	m_UserFlagPos[3].y=nHeight/2-m_nYFace-83-m_nYTimer;
	m_UserListenPos[3].x=m_nXBorder+91;
	m_UserListenPos[3].y=nHeight/2-80;
	m_PointTrustee[3].x=m_nXBorder+5;
	m_PointTrustee[3].y=nHeight/2-m_nYFace-50-35;


	//�û��˿�
	m_UserCard[0].SetControlPoint(nWidth/2-200,m_nYBorder+m_nYFace+10);
	m_UserCard[1].SetControlPoint(nWidth/2+245,nHeight/2-255);
	m_UserCard[2].SetControlPoint(nWidth/2-265,nHeight/2+177);
	m_HandCardControl.SetBenchmarkPos(nWidth/2+40,nHeight-m_nYFace-m_nYBorder-5,enXCenter,enYBottom);

	//�����˿�
	m_TableCard[0].SetControlPoint(nWidth/2-179,m_nYBorder+m_nYFace+10);
	m_TableCard[1].SetControlPoint(nWidth/2+275,nHeight/2-197);
	m_TableCard[2].SetControlPoint(nWidth/2+185,nHeight-m_nYFace-m_nYBorder-8);
	m_TableCard[3].SetControlPoint(nWidth/2-275,nHeight/2+97);

	//�ѻ��˿�
	int nXCenter=nWidth/2;
	int nYCenter=nHeight/2-40;
	m_HeapCard[0].SetControlPoint(nXCenter-150,nYCenter-200);
	m_HeapCard[1].SetControlPoint(nXCenter+205,nYCenter-145);
	m_HeapCard[2].SetControlPoint(nXCenter-150,nYCenter+213);
	m_HeapCard[3].SetControlPoint(nXCenter-220,nYCenter-145);

	//�����˿�
	//m_DiscardCard[0].SetControlPoint(nXCenter-103,nYCenter-100);
	m_DiscardCard[0].SetControlPoint(nXCenter-125,nYCenter-100);
	m_DiscardCard[1].SetControlPoint(nXCenter+103,nYCenter-63);
	//m_DiscardCard[2].SetControlPoint(nXCenter+100,nYCenter+112);
	m_DiscardCard[2].SetControlPoint(nXCenter+125,nYCenter+112);
	m_DiscardCard[3].SetControlPoint(nXCenter-125,nYCenter+74);

	//����˿�
	m_WeaveCard[0][0].SetControlPoint(nWidth/2+195,m_nYBorder+m_nYFace+15);
	m_WeaveCard[0][1].SetControlPoint(nWidth/2+110,m_nYBorder+m_nYFace+15);
	m_WeaveCard[0][2].SetControlPoint(nWidth/2+25,m_nYBorder+m_nYFace+15);
	m_WeaveCard[0][3].SetControlPoint(nWidth/2-60,m_nYBorder+m_nYFace+15);

	//����˿�
	m_WeaveCard[1][0].SetControlPoint(nWidth/2+275,nHeight/2+90);
	m_WeaveCard[1][1].SetControlPoint(nWidth/2+275,nHeight/2+5);
	m_WeaveCard[1][2].SetControlPoint(nWidth/2+275,nHeight/2-80);
	m_WeaveCard[1][3].SetControlPoint(nWidth/2+275,nHeight/2-165);

	//����˿�
	m_WeaveCard[2][0].SetControlPoint(nWidth/2-240,nHeight-m_nYFace-m_nYBorder-5);
	m_WeaveCard[2][1].SetControlPoint(nWidth/2-140,nHeight-m_nYFace-m_nYBorder-5);
	m_WeaveCard[2][2].SetControlPoint(nWidth/2-40,nHeight-m_nYFace-m_nYBorder-5);
	m_WeaveCard[2][3].SetControlPoint(nWidth/2+60,nHeight-m_nYFace-m_nYBorder-5);

	//����˿�
	m_WeaveCard[3][0].SetControlPoint(nWidth/2-275,nHeight/2-200);
	m_WeaveCard[3][1].SetControlPoint(nWidth/2-275,nHeight/2-115);
	m_WeaveCard[3][2].SetControlPoint(nWidth/2-275,nHeight/2-20);
	m_WeaveCard[3][3].SetControlPoint(nWidth/2-275,nHeight/2+75);

	//���ƴ���
	m_ControlWnd.SetBenchmarkPos(nWidth/2+298,nHeight-m_nYBorder-120);


	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	m_btStart.GetWindowRect(&rcButton);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//�ƶ�����
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-125-m_nYBorder,0,0,uFlags);
	//�ƶ�����
	DeferWindowPos(hDwp,m_btStusteeControl,NULL,nWidth-m_nXBorder-(rcButton.Width()+5),nHeight-m_nYBorder-rcButton.Height(),0,0,uFlags);
	//�ƶ��ɼ�
	CRect rcScoreControl;
	m_ScoreControl.GetWindowRect(&rcScoreControl);
	DeferWindowPos(hDwp,m_ScoreControl,NULL,(nWidth-rcScoreControl.Width())/2,(nHeight-rcScoreControl.Height())*2/5,0,0,uFlags);

	//��Ƶ����
	CRect rcAVDlg;
	m_DlgVedioService[0].GetWindowRect(&rcAVDlg);
	DeferWindowPos(hDwp,m_DlgVedioService[1],NULL,nWidth-m_nXBorder-5-rcAVDlg.Width(),nHeight/2+30,0,0,uFlags);
	DeferWindowPos(hDwp,m_DlgVedioService[3],NULL,m_nXBorder+5,nHeight/2+30,0,0,uFlags);
	DeferWindowPos(hDwp,m_DlgVedioService[0],NULL,nWidth-m_nXBorder-5-rcAVDlg.Width(),5,0,0,uFlags);
	m_DlgVedioService[2].GetWindowRect(&rcAVDlg);
	DeferWindowPos(hDwp,m_DlgVedioService[2],NULL,m_nXBorder+5,nHeight-m_nYBorder-3-rcAVDlg.Height(),0,0,uFlags);

	//�����ƶ�
	EndDeferWindowPos(hDwp);

	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageBack,enMode_Spread);
	DrawViewImage(pDC,m_ImageCenter,enMode_Centent);


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
			DrawTextString(pDC,pUserData->szName,RGB(255,255,255),RGB(0,0,0),m_ptName[i].x,m_ptName[i].y);

			//������Ϣ
			WORD wUserTimer=GetUserTimer(i);
			if ((wUserTimer!=0)&&(m_wCurrentUser!=INVALID_CHAIR))
			{
				DrawUserTimer(pDC,nWidth/2,nHeight/2,wUserTimer);
				if(m_wCurrentUser==0)
					m_ImageArrow.DrawImage(pDC,nWidth/2-15,nHeight/2-m_ImageArrow.GetHeight()*2,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*m_wCurrentUser,0);
				if(m_wCurrentUser==1)
					m_ImageArrow.DrawImage(pDC,nWidth/2+m_ImageArrow.GetWidth()/4,nHeight/2-15,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*m_wCurrentUser,0);
				if(m_wCurrentUser==2)
					m_ImageArrow.DrawImage(pDC,nWidth/2-15,nHeight/2+m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*m_wCurrentUser,0);
				if(m_wCurrentUser==3)
					m_ImageArrow.DrawImage(pDC,nWidth/2-m_ImageArrow.GetWidth()/4*2,nHeight/2-15,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*m_wCurrentUser,0);
			}
			if((wUserTimer!=0)&&(m_wCurrentUser==INVALID_CHAIR))
			{
				DrawUserTimer(pDC,nWidth/2,nHeight/2,wUserTimer);
				if(i==0)
					m_ImageArrow.DrawImage(pDC,nWidth/2-15,nHeight/2-m_ImageArrow.GetHeight()*2,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*i,0);
				if(i==1)
					m_ImageArrow.DrawImage(pDC,nWidth/2+m_ImageArrow.GetWidth()/4,nHeight/2-15,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*i,0);
				if(i==2)
					m_ImageArrow.DrawImage(pDC,nWidth/2-15,nHeight/2+m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*i,0);
				if(i==3)
					m_ImageArrow.DrawImage(pDC,nWidth/2-m_ImageArrow.GetWidth()/4*2,nHeight/2-15,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*i,0);

			}

			if (pUserData->cbUserStatus==US_READY) 
				DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);

			//�й�
			if(m_bTrustee[i])
			{
			
				m_ImageTrustee.DrawImage(pDC,m_PointTrustee[i].x,m_PointTrustee[i].y);
			}
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
			if(wIndex == 1) wIndex=3; //�ϱ�����
			else if(wIndex == 3) wIndex=1;
			else wIndex =wIndex;
			m_ImageUserFlag.AlphaDrawImage(pDC,m_UserFlagPos[i].x,m_UserFlagPos[i].y,nImageWidth,nImageHeight,nImageWidth*wIndex,0,RGB(255,0,255));
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
	}

	//�ѻ��˿�
	m_HeapCard[3].DrawCardControl(pDC);
	m_HeapCard[0].DrawCardControl(pDC);
	m_HeapCard[1].DrawCardControl(pDC);
	m_HeapCard[2].DrawCardControl(pDC);

	//�û��˿�
	m_UserCard[0].DrawCardControl(pDC);
	m_UserCard[1].DrawCardControl(pDC);
	m_UserCard[2].DrawCardControl(pDC);
	m_HandCardControl.DrawCardControl(pDC);

	//������ʾ
	if (m_bOutCard==true)
	{
		CImageHandle HandleOutCard(&m_ImageOutCard);
		m_ImageOutCard.AlphaDrawImage(pDC,(nWidth-m_ImageOutCard.GetWidth())/2,nHeight-145,RGB(255,0,255));
	}

	//�ȴ���ʾ
	if (m_bWaitOther==true)
	{
		CImageHandle HandleWait(&m_ImageWait);
		m_ImageWait.AlphaDrawImage(pDC,(nWidth-m_ImageWait.GetWidth())/2,nHeight-145,RGB(255,0,255));
	}

	//��ׯ��־
	if (m_bHuangZhuang==true)
	{
		CImageHandle HandleHuangZhuang(&m_ImageHuangZhuang);
		m_ImageHuangZhuang.AlphaDrawImage(pDC,(nWidth-m_ImageHuangZhuang.GetWidth())/2,nHeight/2-103,RGB(255,0,255));
	}

	//���Ʊ�־
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_bListenStatus[i]==true)
		{
			//������Դ
			CImageHandle HandleListenStatus(((i%2)==0)?&m_ImageListenStatusH:&m_ImageListenStatusV);

			//��ȡ��Ϣ
			int nImageWidth=HandleListenStatus->GetWidth();
			int nImageHeight=HandleListenStatus->GetHeight();

			//�滭��־
			HandleListenStatus->AlphaDrawImage(pDC,m_UserListenPos[i].x-nImageWidth/2,m_UserListenPos[i].y-nImageHeight/2-10,RGB(255,0,255));
		}
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
					nYPos=nHeight-m_nYBorder-240;
					break;
				}
			case 3:	//����
				{
					nXPos=m_nXBorder+115;
					nYPos=nHeight/2-71;
					break;
				}
			}

			//�滭����
			if (m_cbUserAction[i]!=WIK_NULL)
			{	

				//�滭����
				if (m_bBombEffect==true)
				{

					//�滭Ч��
					INT nItemHeight=m_ImageActionAni.GetHeight()/6;
					INT nItemWidth=m_ImageActionAni.GetWidth()/BOMB_EFFECT_COUNT;

					//�滭����
					int nYImagePos=0;
					if (m_cbUserAction[i]&WIK_PENG) nYImagePos=nItemHeight;
					else if (m_cbUserAction[i]&WIK_GANG) nYImagePos=nItemHeight*2;
					else if (m_cbUserAction[i]&WIK_LISTEN) nYImagePos=nItemHeight*3;
					else if (m_cbUserAction[i]&WIK_CHI_HU) nYImagePos=nItemHeight*4;
					else nYImagePos=0;
					m_ImageActionAni.DrawImage(pDC,nXPos-nItemWidth/2+54,nYPos+42-nItemHeight/2,nItemWidth,nItemHeight,
						nItemWidth*(m_cbBombFrameIndex%BOMB_EFFECT_COUNT),nYImagePos,nItemWidth,nItemHeight);
				}
			}
			else
			{	
				//��������
				CImageHandle ImageHandle(&m_ImageActionBack);
				m_ImageActionBack.BlendDrawImage(pDC,nXPos,nYPos,m_ImageActionBack.GetWidth(),m_ImageActionBack.GetHeight(),
					0,0,RGB(255,0,255),180);


				//�滭�˿�
				g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,m_cbCardData,nXPos+15,nYPos+13);
			}

		}
	}

	//����Ч��
	if(m_wDiscUser!=INVALID_CHAIR)
	{
		//������Դ
		//CImageHandle HandleDisc(&m_ImageDisc);

		////��ȡλ��
		//int nItemWidth=m_ImageDisc.GetWidth()/8;
		//int nItemHeight=m_ImageDisc.GetHeight();
		//CPoint pt=m_DiscardCard[m_wDiscUser].GetLastCardPosition();

		//m_ImageDisc.BlendDrawImage(pDC,pt.x,pt.y,nItemWidth,nItemHeight,nItemWidth*(m_cbDiscFrameIndex%BOMB_EFFECT_COUNT),0,RGB(255,0,255),200);
		//m_ImageDisc.AlphaDrawImage(pDC,100,100,nItemWidth,nItemHeight,nItemWidth*(m_cbDiscFrameIndex%BOMB_EFFECT_COUNT),0,RGB(255,0,255));


		CSize SizeDisc(m_ImageDisc.GetWidth()/DISC_EFFECT_COUNT,m_ImageDisc.GetHeight());
		CPoint pt=m_DiscardCard[m_wDiscUser].GetLastCardPosition();
		//�滭��Ϣ
		m_ImageDisc.DrawImage(pDC,pt.x,pt.y,SizeDisc.cx,SizeDisc.cy,
			m_cbDiscFrameIndex*SizeDisc.cx,0,SizeDisc.cx,SizeDisc.cy);

	}
	return;
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
	if (wViewChairID<GAME_PLAYER)
	{
		m_cbUserAction[wViewChairID]=bUserAction;
		SetBombEffect(true);
	}
	else 
	{
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		if(m_bBombEffect)
			SetBombEffect(false);
	}

	//���½���
	UpdateGameView(NULL);

	return;
}
//���Ʊ�־
void CGameClientView::SetUserListenStatus(WORD wViewChairID, bool bListenStatus)
{
	//���ñ���
	if (wViewChairID<GAME_PLAYER)
	{
		SetBombEffect(true);
		m_cbUserAction[wViewChairID]=WIK_LISTEN;
		m_bListenStatus[wViewChairID]=bListenStatus;
	}
	else 
		ZeroMemory(m_bListenStatus,sizeof(m_bListenStatus));

	//���½���
	UpdateGameView(NULL);

	return;
}
//���ö���
bool CGameClientView::SetBombEffect(bool bBombEffect)
{
	if (bBombEffect==true)
	{
		//���ñ���
		m_bBombEffect=true;
		m_cbBombFrameIndex=0;

		//����ʱ��
		SetTimer(IDI_BOMB_EFFECT,250,NULL);
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
//�����û�
void CGameClientView::SetDiscUser(WORD wDiscUser)
{
	if(m_wDiscUser != wDiscUser)
	{
		//���±���
		m_wDiscUser=wDiscUser;

		//���½���
		UpdateGameView(NULL);
	}
	return;
}
//��ʱ���
void CGameClientView::SetCurrentUser(WORD wCurrentUser)
{
	if (m_wCurrentUser != wCurrentUser)
	{
		//���±��� 
		m_wCurrentUser=wCurrentUser;
		
		//���½���
		UpdateGameView(NULL);
	}
	return;
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
	if (bRePaint==true)
		UpdateGameView(NULL);

	//������
	if (bHandle==false)
		__super::OnSetCursor(pWnd,nHitTest,uMessage);

	return TRUE;
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//��ȡ�˿�
	BYTE cbHoverCard=m_HandCardControl.GetHoverCard();
	if (cbHoverCard!=0) 
		AfxGetMainWnd()->PostMessage(IDM_OUT_CARD,cbHoverCard,cbHoverCard);

	return;
}

//��ʼ��ť
void CGameClientView::OnStart()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_START,0,0);

	return;
}
//�Ϲܿ���
void CGameClientView::OnStusteeControl()
{
	AfxGetMainWnd()->SendMessage(IDM_TRUSTEE_CONTROL,0,0);
	return;
}
//////////////////////////////////////////////////////////////////////////
void CGameClientView::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	//��������
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
	if (nIDEvent==IDI_DISC_EFFECT)
	{
		//���ñ���
		if ((m_cbDiscFrameIndex+1)>=DISC_EFFECT_COUNT)
		{
			m_cbDiscFrameIndex=0;
		}
		else m_cbDiscFrameIndex++;

		//���½���
		UpdateGameView(NULL);

		return;

	}

	__super::OnTimer(nIDEvent);
}
