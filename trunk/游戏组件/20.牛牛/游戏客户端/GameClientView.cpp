#include "StdAfx.h"
#include "Resource.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include "GameClientView.h"


//////////////////////////////////////////////////////////////////////////
//�궨��

//��ť��ʶ
#define IDC_HINT_OX						130								//��ʾ��ť
#define IDC_START						100								//��ʼ��ť
#define IDC_MAX_SCORE					104								//���ť
#define IDC_MIN_SCORE					105								//���ٰ�ť
#define IDC_ADD_SCORE					110								//��ע��ť

#define IDC_READY						111								//̯�ư�ť
#define IDC_RESORT						112								//���Ű�ť
#define IDC_OX							113								//ţţ��ť
#define IDC_ONE_SCORE					117								//��ע��ť
#define IDC_TWO_SCORE					114								//��ע��ť
#define IDC_THREE_SCORE					115								//��ע��ť
#define IDC_FOUR_SCORE					116								//��ע��ť
#define IDC_SHORTCUT					118								//��ݼ���ť
#define IDC_IDLER						119								//�мҰ�ť
#define IDC_BANKER						120								//ׯ�Ұ�ť

//��ʱ����ʶ
#define IDI_SEND_CARD					98								//���ƶ�ʱ��
#define IDI_GOLD						97								//���ƶ�ʱ��

//�ƶ��ٶ�
#define	SEND_PELS						80								//�����ٶ�	
#define	TIME_SENDSPEED					50								//����ٶ�
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)							  
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView() : CGameFrameView(true,24)
{
	//��������
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_bOxValue[i]=0xff;
		m_SendEndingPos[i].SetPoint(0,0);
	}
	m_wSendCount=0;
	m_wSendIndex=0;
	m_SendCardPos.SetPoint(0,0);
	m_SendCardItemArray.RemoveAll();

	//���ݱ���
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_bOpenCardUser,sizeof(m_bOpenCardUser));
	ZeroMemory(m_bShowSocre,sizeof(m_bShowSocre));
	m_wBankerUser=INVALID_CHAIR;
	m_bInvest=false;
	m_bCallUser=0xff;
	m_bShow=false;
	m_bLookOnUser=false;
	m_bOpenCard=false;

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageOxValue.SetLoadInfo(IDB_COW_VALUE,AfxGetInstanceHandle());
	m_ImageOxValueZero.SetLoadInfo(IDB_COW_VALUE_ZERO,AfxGetInstanceHandle());
	m_ImageDoulbeOx.SetLoadInfo(IDB_DOUBLE_COW,AfxGetInstanceHandle());
	m_ImageNumber.SetLoadInfo(IDB_NUMBER,AfxGetInstanceHandle());
	m_ImageScore.SetLoadInfo(IDB_SCORE,AfxGetInstanceHandle());
	m_ImageJeton.SetLoadInfo(IDB__MONEY,AfxGetInstanceHandle());
	m_ImageWaitCall.SetLoadInfo(IDB_CALL_BANKER,AfxGetInstanceHandle());
	m_ImageCard.SetLoadInfo(IDB_CARD,hInstance);
	m_ImageViewBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageViewCenter.SetLoadInfo(IDB_VIEW_FILL,hInstance);
	m_ImageBanker.SetLoadInfo(IDB_BANKER_FLAG,hInstance);

	return;
}

//��������
CGameClientView::~CGameClientView()
{
}

//��Ϣ����
BOOL CGameClientView::PreTranslateMessage(MSG * pMsg)
{
	m_ToolTipCtrl.RelayEvent(pMsg);
	return __super::PreTranslateMessage(pMsg);
}

//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	//m_GoldControl.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS,CRect(50,50,66,66),this,8);
	m_ScoreView.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,10);

	//������ť
	m_btHintOx.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_HINT_OX);
	m_btStart.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_START);
	m_btReSort.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_RESORT);
	m_btOpenCard.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_READY);
	m_btOx.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_OX);
	m_btShortcut.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_SHORTCUT);
	m_btOneScore.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_ONE_SCORE);
	m_btTwoScore.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_TWO_SCORE);
	m_btThreeScore.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_THREE_SCORE);
	m_btFourScore.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_FOUR_SCORE);
	m_btBanker.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_BANKER);
	m_btIdler.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_IDLER);

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btHintOx.SetButtonImage(IDB_HINT_OX,hInstance,false);
	m_btStart.SetButtonImage(IDB_START,hInstance,false);
	m_btReSort.SetButtonImage(IDB_RESOCT,hInstance,false);
	m_btOpenCard.SetButtonImage(IDB_OPENCARD,hInstance,false);
	m_btOx.SetButtonImage(IDB_COW,hInstance,false);
	m_btShortcut.SetButtonImage(IDB_ShortCut,hInstance,false);
	m_btOneScore.SetButtonImage(IDB__MONEY,hInstance,false);
	m_btTwoScore.SetButtonImage(IDB__MONEY,hInstance,false);
	m_btThreeScore.SetButtonImage(IDB__MONEY,hInstance,false);
	m_btFourScore.SetButtonImage(IDB__MONEY,hInstance,false);
	m_btBanker.SetButtonImage(IDB_BANKER,hInstance,false);
	m_btIdler.SetButtonImage(IDB_IDLER,hInstance,false);

	//�����ؼ�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_CardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,rcCreate,this,20+i);
		m_CardControlOx[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,rcCreate,this,20+GAME_PLAYER+i);
		m_CardControlOx[i].SetDrawHeight(30);
	}

    //��ť��ʾ
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btShortcut,TEXT("ţ��ʾ��ݼ�"));

#ifdef VIDEO_GAME
	//������Ƶ
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//������Ƶ
		m_DlgVideoService[i].Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,200+i);
		m_DlgVideoService[i].InitVideoService(i==2,i!=2);

		//������Ƶ
		g_VideoServiceManager.SetVideoServiceControl(i,&m_DlgVideoService[i]);
	}
#endif

	return 0;
}

//�����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_HINT_OX:			//��ʾ��ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_HINT_OX,0,0);
			return TRUE;
		}
	case IDC_START:				//��ʼ��ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_START,0,0);
			return TRUE;
		}
	case IDC_READY:				//̯�ư�ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_READY,0,0);
			return TRUE;
		}
	case IDC_OX:				//ţţ��ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_OX,0,0);
			return TRUE;
		}
	case IDC_RESORT:			//���Ű�ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_RESORT,0,0);
			return TRUE;
		}
	case IDC_ONE_SCORE:			//��ע��ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_ADD_SCORE,4,1);
			return TRUE;
		}
	case IDC_TWO_SCORE:			//��ע��ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_ADD_SCORE,3,2);
			return TRUE;
		}
	case IDC_THREE_SCORE:		//��ע��ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_ADD_SCORE,2,3);
			return TRUE;
		}
	case IDC_FOUR_SCORE:		//��ע��ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_ADD_SCORE,1,4);
			return TRUE;
		}
	case IDC_SHORTCUT:			//��ݼ���ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_SHORTCUT_KEY,0,0);
			return TRUE;
		}
	case IDC_BANKER:			//ׯ�Ұ�ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_BANKER,1,1);
			return TRUE;
		}
	case IDC_IDLER:				//�мҰ�ť
		{
			//������Ϣ
			AfxGetMainWnd()->SendMessage(IDM_BANKER,0,0);
			return TRUE;
		}
	}

	return __super::OnCommand(wParam, lParam);
}

//���ý���
void CGameClientView::ResetGameView()
{
	//��������
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_bOxValue[i]=0xff;
	}
	m_wSendCount=0;
	m_wSendIndex=0;
	m_SendCardPos.SetPoint(0,0);
	m_SendCardItemArray.RemoveAll();

	//���ݱ���
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_bOpenCardUser,sizeof(m_bOpenCardUser));
	ZeroMemory(m_bShowSocre,sizeof(m_bShowSocre));
	m_wBankerUser=INVALID_CHAIR;				
	m_bInvest=false;
	m_bCallUser=0xff;
	m_bShow=false;
	m_bLookOnUser=false;
	m_bOpenCard=false;

	//ɾ����ʱ��
	KillTimer(IDI_SEND_CARD);

	//���ذ�ť
	m_btHintOx.ShowWindow(SW_HIDE);
	m_btStart.ShowWindow(SW_HIDE);
	m_btReSort.ShowWindow(SW_HIDE);
	m_btOpenCard.ShowWindow(SW_HIDE);
	m_btOx.ShowWindow(SW_HIDE);
	m_btOneScore.ShowWindow(SW_HIDE);
	m_btTwoScore.ShowWindow(SW_HIDE);
	m_btThreeScore.ShowWindow(SW_HIDE);
	m_btFourScore.ShowWindow(SW_HIDE);
	m_btShortcut.ShowWindow(SW_HIDE);
	m_btBanker.ShowWindow(SW_HIDE);
	m_btIdler.ShowWindow(SW_HIDE);

	//���ؿؼ�
	m_ScoreView.ShowWindow(SW_HIDE);
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_CardControl[i].SetDisplayFlag(false);
		//m_CardControlOx[i].SetDisplayFlag(false);
		m_CardControl[i].SetCardData(NULL,0);
		m_CardControlOx[i].SetCardData(NULL,0);
	}

	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	CImageHandle ImageHandleBanker(&m_ImageBanker);

	//����λ��
	m_SendStartPos.SetPoint(nWidth/2,nHeight/2-30);

	m_ptFace[0].x=nWidth/2-m_nXFace/2;
	m_ptFace[0].y=nHeight/2-351;
	m_ptFace[2].x=nWidth/2-m_nXFace/2;
	m_ptFace[2].y=nHeight/2+352-m_nYFace;
	m_ptFace[1].x=nWidth/2+341;
	m_ptFace[1].y=nHeight/2-m_nYFace;
	m_ptFace[3].x=nWidth/2-373;
	m_ptFace[3].y=nHeight/2-m_nYFace;

	m_ptName[0].x=nWidth/2+21;
	m_ptName[0].y=nHeight/2-345;
	m_ptName[2].x=nWidth/2+21;
	m_ptName[2].y=nHeight/2+325;
	m_ptName[1].x=nWidth/2+183+155;
	m_ptName[1].y=nHeight/2-m_nYFace+8;
	m_ptName[3].x=nWidth/2-183-155;
	m_ptName[3].y=nHeight/2-m_nYFace+8;

	m_ptTimer[0].x=m_ptFace[0].x-m_nXTimer/2-5;
	m_ptTimer[0].y=nHeight/2-327;
	m_ptTimer[2].x=m_ptFace[2].x-m_nXTimer/2-5;
	m_ptTimer[2].y=nHeight/2+352-m_nYTimer/2;
	m_ptTimer[1].x=nWidth/2+188+164;
	m_ptTimer[1].y=nHeight/2-m_nYFace-m_nYTimer+20;
	m_ptTimer[3].x=nWidth/2-220-131;
	m_ptTimer[3].y=nHeight/2-m_nYFace+20-m_nYTimer;

	m_ptReady[0].x=nWidth/2-m_nXTimer-m_nXFace/2+195;
	m_ptReady[0].y=nHeight/2-331;
	m_ptReady[2].x=nWidth/2-m_nXTimer-m_nXFace/2-5;
	m_ptReady[2].y=nHeight/2+331;
	m_ptReady[1].x=nWidth/2+205+125;
	m_ptReady[1].y=nHeight/2-m_nYFace-m_nYTimer/2-5;
	m_ptReady[3].x=nWidth/2-205-125;
	m_ptReady[3].y=nHeight/2-m_nYFace-m_nYTimer/2-5;

	//��������
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		m_PointBanker[i].x=m_ptTimer[i].x-m_nXTimer/2;
		m_PointBanker[i].y=m_ptTimer[i].y-m_nYTimer/2;
	}
	m_PointBanker[1].x+=6;
	m_PointBanker[3].x+=2;
	m_PointBanker[2].y+=9;
	m_PointBanker[0].x	+=160;
	m_PointBanker[2].x	+=160;
	m_PointBanker[1].y	+=120;
	m_PointBanker[3].y	+=120;

	m_ptJeton[0].x=nWidth/2-32;
	m_ptJeton[0].y=nHeight/2-153;
	m_ptJeton[2].x=nWidth/2-32;
	m_ptJeton[2].y=nHeight/2+74;
	m_ptJeton[1].x=nWidth/2+52;
	m_ptJeton[1].y=nHeight/2-49;
	m_ptJeton[3].x=nWidth/2-116;
	m_ptJeton[3].y=nHeight/2-49;

	//�û��˿�;
	m_CardControl[0].SetBenchmarkPos(nWidth/2,nHeight/2-290,enXCenter,enYTop);
	m_CardControlOx[0].SetBenchmarkPos(nWidth/2,nHeight/2-320,enXCenter,enYTop);
	m_CardControl[2].SetBenchmarkPos(nWidth/2,nHeight/2+270,enXCenter,enYBottom);
	m_CardControlOx[2].SetBenchmarkPos(nWidth/2,nHeight/2+240,enXCenter,enYBottom);
	m_CardControl[1].SetBenchmarkPos(nWidth/2+293,nHeight/2-30,enXRight,enYCenter);
	m_CardControlOx[1].SetBenchmarkPos(nWidth/2+283,nHeight/2-60,enXRight,enYCenter);
	m_CardControl[3].SetBenchmarkPos(nWidth/2-293,nHeight/2-30,enXLeft,enYCenter);
	m_CardControlOx[3].SetBenchmarkPos(nWidth/2-283,nHeight/2-60,enXLeft,enYCenter);

	//����λ��
	m_SendEndingPos[0].x=m_CardControl[0].GetCardPos().x;
	m_SendEndingPos[0].y=m_CardControl[0].GetCardPos().y;
	m_SendEndingPos[1].x=m_CardControl[1].GetCardPos().x;
	m_SendEndingPos[1].y=m_CardControl[1].GetCardPos().y;
	m_SendEndingPos[2].x=m_CardControl[2].GetCardPos().x;
	m_SendEndingPos[2].y=m_CardControl[2].GetCardPos().y;
	m_SendEndingPos[3].x=m_CardControl[3].GetCardPos().x;
	m_SendEndingPos[3].y=m_CardControl[3].GetCardPos().y;

	//ţֵλ��
	m_ptValue[0].x=nWidth/2-138;
	m_ptValue[0].y=nHeight/2-250;
	m_ptValue[1].x=nWidth/2+200;
	m_ptValue[1].y=nHeight/2+30;
	m_ptValue[2].x=nWidth/2-138;
	m_ptValue[2].y=nWidth/2+158;
	m_ptValue[3].x=nWidth/2-280;
	m_ptValue[3].y=nHeight/2+30;

	//��ť�ؼ�
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//��ʼ��ť
	m_btStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStart,NULL,nWidth/2-32,nHeight/2+112,0,0,uFlags);

	//���ư�ť
	m_btReSort.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btReSort,NULL,nWidth/2-32,nHeight/2+112,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOpenCard,NULL,nWidth/2+25,nHeight/2+112,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOx,NULL,nWidth/2-118,nHeight/2+112,0,0,uFlags);
	DeferWindowPos(hDwp,m_btHintOx,NULL,nWidth/2-88,nHeight/2+112,0,0,uFlags);

	//������ť
	DeferWindowPos(hDwp,m_btShortcut,NULL,nWidth/2+330,nHeight/2+320,0,0,uFlags);

	//���밴ť
	DeferWindowPos(hDwp,m_btOneScore,NULL,nWidth/2-151,nHeight/2+163,0,0,uFlags);
	DeferWindowPos(hDwp,m_btTwoScore,NULL,nWidth/2-67,nHeight/2+163,0,0,uFlags);
	DeferWindowPos(hDwp,m_btThreeScore,NULL,nWidth/2+15,nHeight/2+163,0,0,uFlags);
	DeferWindowPos(hDwp,m_btFourScore,NULL,nWidth/2+98,nHeight/2+163,0,0,uFlags);

	//ѡׯ��ť
	DeferWindowPos(hDwp,m_btBanker,NULL,nWidth/2-76,nHeight/2+151,0,0,uFlags);
	DeferWindowPos(hDwp,m_btIdler,NULL,nWidth/2+14,nHeight/2+151,0,0,uFlags);

#ifdef VIDEO_GAME

	//��Ƶ����
	DeferWindowPos(hDwp,m_DlgVideoService[0],NULL,nWidth/2-162,nHeight/2-350,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_DlgVideoService[1],NULL,nWidth/2+287,nHeight/2-172,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_DlgVideoService[2],NULL,nWidth/2-290,nHeight/2+195,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_DlgVideoService[3],NULL,nWidth/2-375,nHeight/2-172,0,0,uFlags|SWP_NOSIZE);
#endif

	//�����ƶ�
	EndDeferWindowPos(hDwp);

	//��ע����
	//m_GoldControl.SetBasicPoint(nWidth-80-m_nXBorder-120,nHeight-109-m_nYBorder);

	//������ͼ
	CRect rcControl;
	m_ScoreView.GetWindowRect(&rcControl);
	m_ScoreView.SetWindowPos(NULL,nWidth/2-183,nHeight/2-153,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//��������
	SetFlowerControlInfo(3,m_ptFace[3].x,nHeight/2-BIG_FACE_HEIGHT/2);
	SetFlowerControlInfo(1,m_ptFace[1].x-BIG_FACE_HEIGHT/2,nHeight/2-BIG_FACE_HEIGHT/2);
	SetFlowerControlInfo(0,nWidth/2-BIG_FACE_WIDTH/2,m_ptFace[0].y
		,BIG_FACE_WIDTH, BIG_FACE_HEIGHT, 30);
	SetFlowerControlInfo(2,nWidth/2-BIG_FACE_WIDTH/2,m_ptFace[2].y-80
		,BIG_FACE_WIDTH, BIG_FACE_HEIGHT, 30);
	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageViewCenter,enMode_Spread);
	DrawViewImage(pDC,m_ImageViewBack,enMode_Centent);

	//�滭�û�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wUserTimer=GetUserTimer(i);
		const tagUserData * pUserData=GetUserInfo(i);

#ifdef _DEBUG
		////���Դ���
		//tagUserData Obj;
		//Obj.cbUserStatus = US_READY;
		//Obj.lScore = 999999999;
		//_sntprintf( Obj.szName,sizeof(Obj.szName),TEXT("�û�������"));
		//wUserTimer = i+1;
		//pUserData = &Obj;
		////ׯ�ұ�־
		//CImageHandle ImageHandleBanker(&m_ImageBanker);
		//m_ImageBanker.AlphaDrawImage(pDC,m_PointBanker[i].x,m_PointBanker[i].y,m_ImageBanker.GetWidth(),m_ImageBanker.GetHeight(),0,0,RGB(255,0,255));

#endif
	
		//�滭�û�
		if (pUserData!=NULL)
		{
			//�û�����
			pDC->SetTextAlign((i==1)?TA_RIGHT:TA_LEFT);
			pDC->SetTextColor((wUserTimer>0)?RGB(250,250,250):RGB(220,220,220));

			//�û����
			TCHAR szBuffer[64]=TEXT("");
			LONG lLeaveScore=pUserData->lScore-m_lTableScore[i];
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%ld"),lLeaveScore);
			int TempX=0;
			int TempY=0;
			if(i%2==1)
			{
				TempX=35;
				if(i==3)TempX*=-1;
				TempY=30;
			}

			//�û�����
			DrawTextString(pDC,pUserData->szName,RGB(255,255,255),RGB(0,0,0),m_ptName[i].x+TempX,m_ptName[i].y+TempY);

			//�û����
			DrawTextString(pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),m_ptName[i].x+TempX,m_ptName[i].y+16+TempY);

			//������Ϣ
			if (wUserTimer!=0)
			{
				if(m_bCallUser!=0xff)DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
				else if (pUserData->cbUserStatus==US_PLAY)DrawUserTimer(pDC,nWidth/2,nHeight/2-25,wUserTimer);
				else DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
			}
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);
		}
	}

	//�ȴ���ע
	if (m_bInvest==true)
	{
		//������Դ
		CFont InfoFont;
		InfoFont.CreateFont(-24,0,0,0,700,0,0,0,134,3,2,1,1,TEXT("����_GB2312"));

		//���� DC
		pDC->SetTextAlign(TA_CENTER|TA_TOP);
		CFont * pOldFont=pDC->SelectObject(&InfoFont);

		//�滭��Ϣ
		TCHAR zsInfo[128]=TEXT("");
		_sntprintf(zsInfo,sizeof(zsInfo),TEXT("�ȴ��м���ע"));
		DrawTextString(pDC,zsInfo,RGB(250,200,40),RGB(0,0,0),nWidth/2,nHeight/2);

		//������Դ
		pDC->SelectObject(pOldFont);
		InfoFont.DeleteObject();
	}

	//�ȴ�̯��
	if (m_bOpenCard==true)
	{
		//������Դ
		CFont InfoFont;
		InfoFont.CreateFont(-24,0,0,0,700,0,0,0,134,3,2,1,1,TEXT("����_GB2312"));

		//���� DC
		pDC->SetTextColor(RGB(250,200,40));
		pDC->SetTextAlign(TA_CENTER|TA_TOP);
		CFont * pOldFont=pDC->SelectObject(&InfoFont);

		//�滭��Ϣ
		TCHAR zsInfo[128]=TEXT("");
		_sntprintf(zsInfo,sizeof(zsInfo),TEXT("�ȴ����̯��"));
		DrawTextString(pDC,zsInfo,RGB(250,200,40),RGB(0,0,0),nWidth/2,nHeight/2);

		//������Դ
		pDC->SelectObject(pOldFont);
		InfoFont.DeleteObject();
	}

	//�ȴ���ׯ
	if(m_bCallUser!=0xff && (m_bCallUser!=2 || m_bLookOnUser))
	{
		CImageHandle HandleOx(&m_ImageWaitCall);
		int y=m_SendEndingPos[m_bCallUser].y+10;
		int x=nWidth/2 - m_ImageWaitCall.GetWidth()/2;
		if(m_bCallUser==1)x=nWidth/2+273-120;
		else if(m_bCallUser==3)x=nWidth/2-273-30;
		else if(m_bCallUser==0)y+=30;
		m_ImageWaitCall.AlphaDrawImage(pDC,x,y,m_ImageWaitCall.GetWidth(),
			m_ImageWaitCall.GetHeight(),0,0,RGB(255,0,255));
	}

	//ׯ����Ϣ
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		const tagUserData * pUserData=GetUserInfo(m_wBankerUser);
		if (pUserData!=NULL)
		{
			//ׯ�ұ�־
			CImageHandle ImageHandleBanker(&m_ImageBanker);
			m_ImageBanker.AlphaDrawImage(pDC,m_PointBanker[m_wBankerUser].x,m_PointBanker[m_wBankerUser].y,m_ImageBanker.GetWidth(),m_ImageBanker.GetHeight(),0,0,RGB(255,0,255));
		}
	}

	//�滭����
	for (BYTE i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserInfo(i);
		if (pUserData!=NULL &&(m_lTableScore[i]!=0L || m_bShowSocre[i]))
		{
			int iPos=1;
			if(m_bShowSocre[i])
			{
				if(m_lTableScore[i]>0L)iPos=0;
				else if(m_lTableScore[i]<=0L)iPos=2;
			}
			CImageHandle HandleScore(&m_ImageScore);
			m_ImageScore.AlphaDrawImage(pDC,m_ptJeton[i].x,m_ptJeton[i].y,m_ImageScore.GetWidth()/3,
				m_ImageScore.GetHeight(),m_ImageScore.GetWidth()/3*iPos,0,RGB(255,0,255));

			//��������
			CFont ViewFont;
			ViewFont.CreateFont(-12,0,0,0,700,0,0,0,134,3,2,1,1,TEXT("����"));

			//���� DC
			pDC->SetTextAlign(TA_CENTER);
			CFont * pOldFont=pDC->SelectObject(&ViewFont);

			//�滭��Ϣ
			TCHAR tc[NAME_LEN]=TEXT("");
			_snprintf(tc,sizeof(tc),"%ld",m_lTableScore[i]);
			DrawTextString(pDC,tc,RGB(250,250,250),RGB(0,0,0),m_ptJeton[i].x+32,m_ptJeton[i].y+8);

			//������Դ
			pDC->SelectObject(pOldFont);
			ViewFont.DeleteObject();
		}
	}

	//��ʾ̯�Ʊ�־
	for (BYTE i=0;i<GAME_PLAYER;i++)
	{
		if (m_bOpenCardUser[i]==TRUE)
		{
			//��������
			CFont ViewFont;
			ViewFont.CreateFont(-20,0,0,0,700,0,0,0,134,3,2,1,1,TEXT("���Ĳ���"));

			//���� DC
			pDC->SetTextAlign(TA_LEFT);
			CFont * pOldFont=pDC->SelectObject(&ViewFont);

			//�滭��Ϣ
			TCHAR tc[NAME_LEN]=TEXT("");
			_snprintf(tc,sizeof(tc),"̯");
			INT x,y;
			if(i==0)
			{
				x = m_ptValue[i].x+35;
				y = m_ptValue[i].y+40;
			}
			else if(i==1)
			{
				x = m_ptValue[i].x+97;
				y = m_ptValue[i].y-55;
			}
			else if(i==2)
			{
				x = m_ptValue[i].x+35;
				y = m_ptValue[i].y+40;
			}
			else 
			{
				x = m_ptValue[i].x-37;
				y = m_ptValue[i].y-55;
			}
			DrawTextString(pDC,tc,RGB(250,128,56),RGB(0,0,0),x,y);


			//������Դ
			pDC->SelectObject(pOldFont);
			ViewFont.DeleteObject();
		}
	}

	//�滭�˿�
	if (m_SendCardItemArray.GetCount()>0)
	{
		//��������
		CImageHandle HandleCard(&m_ImageCard);
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

		//��ȡ��С
		int nItemWidth=m_ImageCard.GetWidth()/13;
		int nItemHeight=m_ImageCard.GetHeight()/5;

		//�滭�˿�
		int nXPos=m_SendCardPos.x-nItemWidth/2;
		int nYPos=m_SendCardPos.y-nItemHeight/2+10;
		m_ImageCard.AlphaDrawImage(pDC,nXPos,nYPos,nItemWidth,nItemHeight,nItemWidth*2,nItemHeight*4,RGB(255,0,255));
	}

	//��ʾ����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_bOxValue[i]!=0xff)
		//if((m_bOxValue[i]=10) || true)
		{
			if(m_bShow || i==2)
			{
				if(m_bOxValue[i]!=0)
				{
					CImageHandle HandleOxValue(&m_ImageOxValue);
					INT nXpt = (i%2!=0)?(m_ptValue[i].x+18):(m_CardControl[i].GetCardPos().x-2-m_ImageOxValue.GetWidth());
					INT nypt = m_ptValue[i].y;
					m_ImageOxValue.AlphaDrawImage(pDC,nXpt,nypt,
						m_ImageOxValue.GetWidth(),m_ImageOxValue.GetHeight(),0,0,RGB(255,0,255));
					if(m_bOxValue[i]==10)
					{
						CImageHandle HandleDoulbeOx(&m_ImageDoulbeOx);
						m_ImageDoulbeOx.AlphaDrawImage(pDC,nXpt+33,nypt+6,
							m_ImageDoulbeOx.GetWidth(),m_ImageDoulbeOx.GetHeight(),0,0,RGB(255,0,255));
					}	
					else if(m_bOxValue[i]<10)
					{
						CImageHandle HandleNumber(&m_ImageNumber);
						m_ImageNumber.AlphaDrawImage(pDC,nXpt+35,nypt+7,
							m_ImageNumber.GetWidth()/9,m_ImageNumber.GetHeight(),
							m_ImageNumber.GetWidth()/9*((int)m_bOxValue[i]-1),0,RGB(255,0,255));
					}
				}
				else if(m_bOxValue[i]==0)
				{				
					INT nXpt = m_ptValue[i].x;
					INT nypt = m_ptValue[i].y;
					CImageHandle HandleOxValueZero(&m_ImageOxValueZero);
					m_ImageOxValueZero.AlphaDrawImage(pDC,nXpt+((i!=1)?31:5),nypt+1,
						m_ImageOxValueZero.GetWidth(),m_ImageOxValueZero.GetHeight(),0,0,RGB(255,0,255));
				}
			}
		}
	}

	//���Դ���----------------------------------------
	//BYTE bTemp[3]={1,2,3};
	//TCHAR szBuffer[32];

	//for (WORD i=0;i<GAME_PLAYER;i++)
	//{
	////	m_PlayerJeton[i][0].SetJeton(100);
	////	m_PlayerJeton[i][0].DrawJetonControl(pDC);
	//	//m_CardControl[i].SetDisplayHead(true);

	//	CImageHandle ImageHandleBanker(&m_ImageBanker);
	//	m_ImageBanker.AlphaDrawImage(pDC,m_PointBanker[i].x,m_PointBanker[i].y,m_ImageBanker.GetWidth(),m_ImageBanker.GetHeight(),0,0,RGB(255,0,255));
	//	//m_CardControl[i].SetCardData(bTemp,3);
	//	//׼��
	//	DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
	//	//�û�����
	//	pDC->SetTextColor(RGB(250,250,250));
	//	pDC->SetTextAlign((i==1)?TA_RIGHT:TA_LEFT);
	//	//�û����
	//	TCHAR szBuffer[64]=TEXT("");
	//	_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%ld"),565326L);
	//	int TempX=0;
	//	int TempY=0;
	//	if(i%2==1)
	//	{
	//		TempX=35;
	//		if(i==3)TempX*=-1;
	//		TempY=30;
	//	}
	//	pDC->TextOut(m_ptName[i].x+TempX,m_ptName[i].y+16+TempY,szBuffer,lstrlen(szBuffer));
	//	_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld9999"),i);
	//	pDC->TextOut(m_ptName[i].x+TempX,m_ptName[i].y+TempY,szBuffer,lstrlen(szBuffer));
	//	//pDC->TextOut(m_ptName[i].x,m_ptName[i].y,"���",strlen("���"));
	//	//ʣ����
	//	//_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%ld �ţ�"),27-i);
	//	CSize temp;
	//	temp.cx=((i==2)?24:0);
	//	temp.cy=((i==2)?0:18);
	//	//pDC->TextOut(m_ptName[i].x+temp.cx,m_ptName[i].y+temp.cy,szBuffer,lstrlen(szBuffer));	
	//	DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,i+50);
	//	DrawUserFace(pDC,55+i,m_ptFace[i].x,m_ptFace[i].y,false);
	//}

	return;
}

//�����ж�
bool CGameClientView::IsDispatchCard()
{
	return m_SendCardItemArray.GetCount()>0;
}

//��ɷ���
void CGameClientView::FinishDispatchCard()
{
	//����ж�
	if (m_SendCardItemArray.GetCount()==0) return;

	//��ɶ���
	for (INT_PTR i=0;i<m_SendCardItemArray.GetCount();i++)
	{
		//��ȡ����
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[i];

		//��ȡ�˿�
		BYTE cbCardData[MAX_COUNT];
		WORD wChairID=pSendCardItem->wChairID;
		BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

		//�����˿�
		if (cbCardCount<MAX_COUNT)
		{
			cbCardData[cbCardCount++]=pSendCardItem->cbCardData;
			m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);
		}
	}

	//ɾ������
	KillTimer(IDI_SEND_CARD);
	m_SendCardItemArray.RemoveAll();

	//���½���
	UpdateGameView(NULL);

	return;
}

//�����˿�
void CGameClientView::DispatchUserCard(WORD wChairID, BYTE cbCardData)
{
	//Ч�����
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return;

	//���ñ���
	tagSendCardItem SendCardItem;
	SendCardItem.cbCardData=cbCardData;
	SendCardItem.wChairID=wChairID;

	//��������
	m_SendCardItemArray.Add(SendCardItem);

	//��������
	if (m_SendCardItemArray.GetCount()==1) 
	{
		//��ȡλ��
		CRect rcClient;
		GetClientRect(&rcClient);

		//����λ��
		m_SendCardPos.x=m_SendStartPos.x;
		m_SendCardPos.y=m_SendStartPos.y;

		//λ�Ƽ���
		int nXCount=abs(m_SendEndingPos[SendCardItem.wChairID].x-m_SendStartPos.x)/SEND_PELS;
		int nYCount=abs(m_SendEndingPos[SendCardItem.wChairID].y-m_SendStartPos.y)/SEND_PELS;

		//���ô���
		m_wSendIndex=0;
		m_wSendCount=__max(1,__max(nXCount,nYCount));

		//���ö�ʱ��
		SetTimer(IDI_SEND_CARD,TIME_SENDSPEED,NULL);

		//��������
		CGameClientDlg * pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
		pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
	}

	return;
}

//������ע
void CGameClientView::SetUserTableScore(WORD wChairID, LONG lTableScore)
{
	//��������
	if (wChairID!=INVALID_CHAIR) m_lTableScore[wChairID]=lTableScore;
	else 
	{
		//ZeroMemory(m_tcBuffer,sizeof(m_tcBuffer));
		//for (WORD i=0;i<GAME_PLAYER;i++)m_ptKeepJeton[i].RemoveAll();
		ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	}

	//���½���
	UpdateGameView(NULL);
	return;
}

//ׯ�ұ�־
void CGameClientView::SetBankerUser(WORD wBankerUser)
{
	m_wBankerUser=wBankerUser;

	//���½���
	UpdateGameView(NULL);
	return;
}

//�ȴ���ׯ
void CGameClientView::SetWaitCall(BYTE bCallUser)
{
	m_bCallUser=bCallUser;

	//���½���
	UpdateGameView(NULL);

	return ;
}

//�ȴ���־
void CGameClientView::SetWaitInvest(bool bInvest)
{
	if(m_bInvest!=bInvest)
	{
		m_bInvest=bInvest;

		//���½���
		UpdateGameView(NULL);
	}
	return;
}

//������Ϣ
//void CGameClientView::SetScoreInfo(LONG lTurnMaxScore,LONG lTurnLessScore)
//{
//	m_lTurnMaxScore=lTurnMaxScore;
//	//m_lTurnLessScore=lTurnLessScore;
//
//	//���½���
//	UpdateGameView(NULL);
//	return;
//}

//��ʾ����
void CGameClientView::DisplayType(bool bShow)
{
	if(m_bShow!=bShow)
	{
		m_bShow=bShow;

		//���½���
		UpdateGameView(NULL);
	}

	return;
}

//��ʾ����
void CGameClientView::SetUserOxValue(WORD wChiarID,BYTE bValue)
{
	//ţţ����
	m_bOxValue[wChiarID]=bValue;

	//���½���
	UpdateGameView(NULL);

	return;
}

//��ʾ̯��
void CGameClientView::ShowOpenCard(WORD wChiarID,BOOL bShow)
{
	//̯���û�
	m_bOpenCardUser[wChiarID]=bShow;

	//���½���
	UpdateGameView(NULL);

	return;
}

//��ʾ����
void CGameClientView::ShowScore(WORD wChairID,bool bShow)
{
	//��ʾ��־
	m_bShowSocre[wChairID]=bShow;

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

//��������
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	pDC->SetTextColor(crFrame);
	CRect rcDraw;
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		rcDraw.CopyRect(lpRect);
		rcDraw.OffsetRect(nXExcursion[i],nYExcursion[i]);
		pDC->DrawText(pszString,nStringLength,&rcDraw,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}

	//�滭����
	rcDraw.CopyRect(lpRect);
	pDC->SetTextColor(crText);
	pDC->DrawText(pszString,nStringLength,&rcDraw,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	return;
}

//��ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{
	//���ʹ���
	switch(nIDEvent)
	{
	case IDI_SEND_CARD:		//�ƶ��˿�
		{
			//��������
			m_wSendIndex++;

			//����λ��
			tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];
			if(pSendCardItem->wChairID%2!=0)
				m_SendCardPos.x=m_SendStartPos.x+(m_SendEndingPos[pSendCardItem->wChairID].x-m_SendStartPos.x)*m_wSendIndex/m_wSendCount;
			else
				m_SendCardPos.y=m_SendStartPos.y+(m_SendEndingPos[pSendCardItem->wChairID].y-m_SendStartPos.y)*m_wSendIndex/m_wSendCount;

			//ֹͣ�ж�
			if (m_wSendIndex>=m_wSendCount)
			{
				//��ȡ�˿�
				BYTE cbCardData[MAX_COUNT];
				WORD wChairID=pSendCardItem->wChairID;
				BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

				//�����˿�
				cbCardData[cbCardCount++]=pSendCardItem->cbCardData;
				m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);

				//ɾ������
				m_SendCardItemArray.RemoveAt(0);

				//��������
				if (m_SendCardItemArray.GetCount()>0)
				{
					//��������
					tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

					//����λ��
					m_SendCardPos.x=m_SendStartPos.x;
					m_SendCardPos.y=m_SendStartPos.y;

					//λ�Ƽ���
					int nXCount=abs(m_SendEndingPos[pSendCardItem->wChairID].x-m_SendStartPos.x)/SEND_PELS;
					int nYCount=abs(m_SendEndingPos[pSendCardItem->wChairID].y-m_SendStartPos.y)/SEND_PELS;

					//���ô���
					m_wSendIndex=0;
					m_wSendCount=__max(1,__max(nXCount,nYCount));

					//��������
					CGameClientDlg * pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
					pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
				}
				else 
				{
					//��ɴ���
					KillTimer(IDI_SEND_CARD);
					AfxGetMainWnd()->SendMessage(IDM_SEND_CARD_FINISH,0,0);
				}
			}
			
			//���½���
			UpdateGameView(NULL);

			return ;
		}
	}

	__super::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////

