#include "StdAfx.h"
#include "Resource.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////
//��ť��ʶ

//��Ϸ��ť
#define IDC_START						100								//��ʼ��ť
#define IDC_OUT_CARD					101								//���ư�ť
#define IDC_OUT_PROMPT					102								//��ʾ��ť
#define IDC_SEND_CONCEAL				103								//���װ�ť
#define IDC_KOU							104								//������ť
#define IDC_GIVE_UP						105								//������ť

//���ܰ�ť
#define IDC_CONCEAL_CARD				200								//�鿴����
#define IDC_REQUEST_LEAVE				201								//�����뿪
#define IDC_LAST_TURN_CARD				202								//�����˿�
#define IDC_TRUSTEE_CONTROL				203								//�йܿ���

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_OUT_CARD, OnOutCard)
	ON_BN_CLICKED(IDC_OUT_PROMPT, OnOutPrompt)
	ON_BN_CLICKED(IDC_SEND_CONCEAL, OnSendConceal)
	ON_BN_CLICKED(IDC_CONCEAL_CARD, OnConcealCard)
	ON_BN_CLICKED(IDC_REQUEST_LEAVE, OnRuquestLeave)
	ON_BN_CLICKED(IDC_LAST_TURN_CARD, OnLastTurnCard)
	ON_BN_CLICKED(IDC_TRUSTEE_CONTROL, OnStusteeControl)
	ON_BN_CLICKED(IDC_KOU,OnKouWang)
	ON_BN_CLICKED(IDC_GIVE_UP,OnGiveUp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView() : CGameFrameView()
{
	//��ʾ����
	m_bDeasilOrder=true;
	m_bWaitConceal=false;
	m_bLastTurnCard=false;

	//��Ϸ����
	m_wCardScore=0xFFFF;
	m_wBankerUser=INVALID_CHAIR;
	m_cbHandCardCount=0;
	m_cbKouCardCount=0;

	//������Ϣ
	m_cbCallCount=0;
	m_cbCallColor=COLOR_ERROR;
	m_cbMainValue=VALUE_ERROR;
	m_cbValueOrder[0]=VALUE_ERROR;
	m_cbValueOrder[1]=VALUE_ERROR;
	m_wCallCardUser=INVALID_CHAIR;

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBanker.SetLoadInfo(IDB_BANKER,hInstance);
	m_ImageBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageBackGround.SetLoadInfo(IDB_BACK_GROUND,hInstance);
	m_ImageColorFlag.SetLoadInfo(IDB_COLOR_FLAG,hInstance);
	m_ImageLastTurn.SetLoadInfo(IDB_LAST_TURN_TIP,hInstance);
	m_ImageWaitConceal.SetLoadInfo(IDB_WAIT_CONCEAL,hInstance);
	m_ImageStationBack.SetLoadInfo(IDB_STATION_BACK,hInstance);
	m_ImageStationTitle.SetLoadInfo(IDB_STATION_TITLE,hInstance);

	//������Դ
	CImageHandle ImageHandleBanker(&m_ImageBanker);
	CImageHandle ImageHandleStationBack(&m_ImageStationBack);
	CImageHandle ImageHandleBackGround(&m_ImageBackGround);

	//��ȡ��С
	m_SizeBanker.cx=m_ImageBanker.GetWidth();
	m_SizeBanker.cy=m_ImageBanker.GetHeight();
	m_SizeStation.cx=m_ImageStationBack.GetWidth();
	m_SizeStation.cy=m_ImageStationBack.GetHeight();

	return;
}

//��������
CGameClientView::~CGameClientView()
{
}

//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	m_ScoreView.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,10);
	m_CallCardWnd.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,11);
	m_ConcealCardView.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,12);

	//�����˿�
	for (WORD i=0;i<4;i++)
	{
		m_UserCardControl[i].SetDirection(true);
		m_UserCardControl[i].SetDisplayFlag(true);
		m_UserCardControl[i].SetCardSpace(DEF_CARD_H_SPACE,DEF_CARD_V_SPACE,0);
		m_UserCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,20+i);
	}
	m_CardScore.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,rcCreate,this,50);
	m_HandCardControl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,60);
	m_KouCardControl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,70);

	//�����˿�
	m_CardScore.SetDisplayFlag(true);
	m_CardScore.SetCardSpace(DEF_CARD_H_SPACE,0,0);
	m_HandCardControl.SetSinkWindow(AfxGetMainWnd());
	m_KouCardControl.SetSinkWindow(AfxGetMainWnd());

	//������ť
	m_btStart.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_START);
	m_btOutCard.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_OUT_CARD);
	m_btOutPrompt.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_OUT_PROMPT);
	m_btSendConceal.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_SEND_CONCEAL);
	m_btKouWang.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_KOU);
	m_btGiveUp.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_GIVE_UP);

	//���ܰ�ť
	m_btConcealCard.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_CONCEAL_CARD);
	//m_btRuquestLeave.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_REQUEST_LEAVE);
	m_btLastTurnCard.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_LAST_TURN_CARD);
	//m_btStusteeControl.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_TRUSTEE_CONTROL);

	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_START,hInstance,false);
	m_btOutCard.SetButtonImage(IDB_OUT_CARD,hInstance,false);
	m_btKouWang.SetButtonImage(IDB_OK,hInstance,false);
	m_btGiveUp.SetButtonImage(IDB_GIVE_UP,hInstance,false);
	m_btOutPrompt.SetButtonImage(IDB_OUT_PROMPT,hInstance,false);
	m_btSendConceal.SetButtonImage(IDB_SEND_CONCEAL,hInstance,false);

	//���ܰ�ť
	m_btConcealCard.SetButtonImage(IDB_CONCEAL_CARD,hInstance,false);
	//m_btRuquestLeave.SetButtonImage(IDB_REQUEST_LEAVE,hInstance,false);
	m_btLastTurnCard.SetButtonImage(IDB_LAST_TURN_CARD,hInstance,false);
	//m_btStusteeControl.SetButtonImage(IDB_START_TRUSTEE,hInstance,false);

	//������ʾ
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btConcealCard,TEXT("�鿴����"));
	//m_ToolTipCtrl.AddTool(&m_btRuquestLeave,TEXT("�鿴����"));
	m_ToolTipCtrl.AddTool(&m_btLastTurnCard,TEXT("��ʾ��һ�ֳ���"));
	//m_ToolTipCtrl.AddTool(&m_btStusteeControl,TEXT("��ʼ��ֹͣ���й�"));

	//��ȡ����
	m_bDeasilOrder=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),FALSE)?true:false;

	return 0;
}

//��Ϣ����
BOOL CGameClientView::PreTranslateMessage(MSG * pMsg)
{
	m_ToolTipCtrl.RelayEvent(pMsg);
	return __super::PreTranslateMessage(pMsg);
}

//���ý���
void CGameClientView::ResetGameView()
{
	//��ʾ����
	m_bWaitConceal=false;
	m_bLastTurnCard=false;

	//��Ϸ����
	m_wCardScore=0xFFFF;
	m_wBankerUser=INVALID_CHAIR;
	m_cbHandCardCount=0;
	m_cbKouCardCount=0;

	//������Ϣ
	m_cbCallCount=0;
	m_cbCallColor=COLOR_ERROR;
	m_cbMainValue=VALUE_ERROR;
	m_cbValueOrder[0]=VALUE_ERROR;
	m_cbValueOrder[1]=VALUE_ERROR;
	m_wCallCardUser=INVALID_CHAIR;

	//���ؿؼ�
	m_btStart.ShowWindow(SW_HIDE);
	m_btOutCard.ShowWindow(SW_HIDE);
	m_btKouWang.ShowWindow(SW_HIDE);
	m_btGiveUp.ShowWindow(SW_HIDE);
	m_btOutPrompt.ShowWindow(SW_HIDE);
	m_btSendConceal.ShowWindow(SW_HIDE);

	//���ð�ť
	m_btConcealCard.EnableWindow(FALSE);
	//m_btRuquestLeave.EnableWindow(FALSE);
	m_btLastTurnCard.EnableWindow(FALSE);
	//m_btStusteeControl.EnableWindow(FALSE);
	//m_btStusteeControl.SetButtonImage(IDB_START_TRUSTEE,AfxGetInstanceHandle(),false);

	//�����˿�
	m_CardScore.SetCardData(NULL,0);
	m_HandCardControl.SetCardData(NULL,0);
	m_HandCardControl.SetPositively(false);
	m_HandCardControl.SetDisplayFlag(false);

	m_KouCardControl.SetCardData(NULL,0);
	m_KouCardControl.SetPositively(false);
	m_KouCardControl.SetDisplayFlag(false);

	for (WORD i=0;i<CountArray(m_UserCardControl);i++) m_UserCardControl[i].SetCardData(NULL,0);

	//�ؼ����
	m_ScoreView.ShowWindow(SW_HIDE);
	m_CallCardWnd.ShowWindow(SW_HIDE);
	m_ConcealCardView.ShowWindow(SW_HIDE);


	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//��������
	int nViewHeight=nHeight-m_SizeStation.cy-m_nYBorder;

	//��������
	if (m_bDeasilOrder==false)
	{
		m_ptFace[3].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptFace[3].y=nViewHeight/2-m_nYFace-25;
		m_ptName[3].x=nWidth-m_nXBorder-m_nXFace-10;
		m_ptName[3].y=nViewHeight/2-m_nYFace-23;
		m_ptTimer[3].x=nWidth-m_nXBorder-m_nXTimer*2-5;
		m_ptTimer[3].y=nViewHeight/2-m_nYFace-m_nYTimer-30;
		m_ptReady[3].x=nWidth-m_nXBorder-m_nXTimer-5;
		m_ptReady[3].y=nViewHeight/2-m_nYFace-m_nYTimer/2-30;
		m_PointBanker[3].x=nWidth-m_nXBorder-m_SizeBanker.cx-3;
		m_PointBanker[3].y=nViewHeight/2-20;

		m_ptFace[1].x=m_nXBorder+25;
		m_ptFace[1].y=nViewHeight/2-m_nYFace-25;
		m_ptName[1].x=m_nXBorder+m_nXFace+30;
		m_ptName[1].y=nViewHeight/2-m_nYFace-23;
		m_ptTimer[1].x=m_nXBorder+25;
		m_ptTimer[1].y=nViewHeight/2-m_nYFace-m_nYTimer-30;
		m_ptReady[1].x=m_nXBorder+25+m_nXTimer;
		m_ptReady[1].y=nViewHeight/2-m_nYFace-m_nYTimer/2-30;
		m_PointBanker[1].x=m_nXBorder+23;
		m_PointBanker[1].y=nViewHeight/2-20;
	}
	else
	{
		m_ptFace[1].x=nWidth-m_nXBorder-m_nXFace-5;
		m_ptFace[1].y=nViewHeight/2-m_nYFace-25;
		m_ptName[1].x=nWidth-m_nXBorder-m_nXFace-10;
		m_ptName[1].y=nViewHeight/2-m_nYFace-23;
		m_ptTimer[1].x=nWidth-m_nXBorder-m_nXTimer*2-5;
		m_ptTimer[1].y=nViewHeight/2-m_nYFace-m_nYTimer-30;
		m_ptReady[1].x=nWidth-m_nXBorder-m_nXTimer-5;
		m_ptReady[1].y=nViewHeight/2-m_nYFace-m_nYTimer/2-30;
		m_PointBanker[1].x=nWidth-m_nXBorder-m_SizeBanker.cx-3;
		m_PointBanker[1].y=nViewHeight/2-20;

		m_ptFace[3].x=m_nXBorder+5;
		m_ptFace[3].y=nViewHeight/2-m_nYFace-25;
		m_ptName[3].x=m_nXBorder+m_nXFace+10;
		m_ptName[3].y=nViewHeight/2-m_nYFace-23;
		m_ptTimer[3].x=m_nXBorder+5;
		m_ptTimer[3].y=nViewHeight/2-m_nYFace-m_nYTimer-30;
		m_ptReady[3].x=m_nXBorder+5+m_nXTimer;
		m_ptReady[3].y=nViewHeight/2-m_nYFace-m_nYTimer/2-30;
		m_PointBanker[3].x=m_nXBorder+3;
		m_PointBanker[3].y=nViewHeight/2-20;
	}

	m_ptFace[0].x=nWidth/2-m_nXFace/2;
	m_ptFace[0].y=m_nYBorder+20;
	m_ptName[0].x=nWidth/2+5+m_nXFace/2;
	m_ptName[0].y=m_nYBorder+30;
	m_ptTimer[0].x=nWidth/2-m_nXFace/2-m_nXTimer*2-5;
	m_ptTimer[0].y=m_nYBorder+10;
	m_ptReady[0].x=nWidth/2-m_nXTimer-m_nXFace/2-5;
	m_ptReady[0].y=m_nYBorder+m_nYTimer/2+25;
	m_PointBanker[0].x=nWidth/2-m_nXFace/2-m_SizeBanker.cx-m_nXTimer*2-10;
	m_PointBanker[0].y=m_nYBorder+20;

	m_ptFace[2].x=nWidth/2-m_nXFace/2;
	m_ptFace[2].y=nViewHeight-m_nYFace-25;
	m_ptName[2].x=nWidth/2+5+m_nXFace/2;
	m_ptName[2].y=nViewHeight-m_nYFace-20;
	m_ptTimer[2].x=nWidth/2-m_nXFace/2-m_nXTimer*2-5;
	m_ptTimer[2].y=nViewHeight-m_nYTimer-30;
	m_ptReady[2].x=nWidth/2-m_nXTimer-m_nXFace/2-5;
	m_ptReady[2].y=nViewHeight-m_nYTimer/2-30;
	m_PointBanker[2].x=nWidth/2-m_nXFace/2-m_SizeBanker.cx-m_nXTimer*2-10;
	m_PointBanker[2].y=nViewHeight-m_SizeBanker.cy-23;

	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//��ʼ��ť
	m_btStart.GetWindowRect(&rcButton);
	//DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nViewHeight-225,0,0,uFlags);
	DeferWindowPos(hDwp,m_btStart,NULL,nWidth/2+70,nViewHeight-60,0,0,uFlags);

	//���װ�ť
	m_btSendConceal.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btSendConceal,NULL,(nWidth-rcButton.Width())/2,nViewHeight-220,0,0,uFlags);

	//���ư�ť
	m_btOutCard.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btOutCard,NULL,nWidth/2+10,nViewHeight-210,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOutPrompt,NULL,nWidth/2-rcButton.Width()-10,nViewHeight-210,0,0,uFlags);

	//������ť
	m_btKouWang.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btKouWang,NULL,nWidth/2+10,nViewHeight-220,0,0,uFlags);
	
	//������ť
	m_btGiveUp.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btGiveUp,NULL,nWidth/2-rcButton.Width()-10,nViewHeight-220,0,0,uFlags);
	
	//���ܰ�ť
	m_btConcealCard.GetWindowRect(&rcButton);
	//DeferWindowPos(hDwp,m_btStusteeControl,NULL,nWidth-m_nXBorder-(rcButton.Width()+5),nViewHeight-5-rcButton.Height(),0,0,uFlags);
	//DeferWindowPos(hDwp,m_btRuquestLeave,NULL,nWidth-m_nXBorder-(rcButton.Width()+5)*2,nViewHeight-5-rcButton.Height(),0,0,uFlags);
	DeferWindowPos(hDwp,m_btConcealCard,NULL,nWidth-m_nXBorder-(rcButton.Width()+5)*2,nViewHeight-5-rcButton.Height(),0,0,uFlags);
	DeferWindowPos(hDwp,m_btLastTurnCard,NULL,nWidth-m_nXBorder-(rcButton.Width()+5),nViewHeight-5-rcButton.Height(),0,0,uFlags);

	//�ƶ��ؼ�
	EndDeferWindowPos(hDwp);

	//�ƶ��˿�
	m_CardScore.SetBenchmarkPos(m_nXBorder+200,nViewHeight-15,enXLeft,enYTop);
	m_UserCardControl[0].SetBenchmarkPos(nWidth/2,m_nYBorder+m_nYFace+25,enXCenter,enYTop);
	m_UserCardControl[2].SetBenchmarkPos(nWidth/2,nViewHeight-210-m_nYBorder,enXCenter,enYBottom);
	m_HandCardControl.SetBenchmarkPos(nWidth/2,nViewHeight-m_nYFace-m_nYBorder-30,enXCenter,enYBottom);
	m_KouCardControl.SetBenchmarkPos(nWidth/2,nViewHeight/2-40,enXCenter,enYBottom);

	//�û��˿�
	if (m_bDeasilOrder==true)
	{
		m_UserCardControl[1].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-80,nViewHeight/2-71,enXRight,enYCenter);
		m_UserCardControl[3].SetBenchmarkPos(m_nXBorder+m_nXFace+80,nViewHeight/2-71,enXLeft,enYCenter);
	}
	else 
	{
		m_UserCardControl[3].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-80,nViewHeight/2-71,enXRight,enYCenter);
		m_UserCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+80,nViewHeight/2-71,enXLeft,enYCenter);
	}

	//���ƴ���
	CRect rcCallCard;
	m_CallCardWnd.GetWindowRect(&rcCallCard);
	m_CallCardWnd.SetWindowPos(NULL,nWidth/2+20,nViewHeight-m_nYBorder-rcCallCard.Height()-162,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//������ͼ
	CRect rcScore;
	m_ScoreView.GetWindowRect(&rcScore);
	m_ScoreView.SetWindowPos(NULL,(nWidth-rcScore.Width())/2,nViewHeight/2-160,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//���ƴ���
	CRect rcConceal;
	m_ConcealCardView.GetWindowRect(&rcConceal);
	m_ConcealCardView.SetWindowPos(NULL,(nWidth-rcConceal.Width())/2,nViewHeight/2-160,0,0,SWP_NOZORDER|SWP_NOSIZE);

	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	CImageHandle CHImageBackGround(&m_ImageBackGround);
	DrawViewImage(pDC,m_ImageBackGround,enMode_Spread);
	DrawViewImage(pDC,m_ImageBack,enMode_Spread);
	BYTE MOVEPOS=24;

	//�滭�û�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wUserTimer=GetUserTimer(i);
		const tagUserData * pUserData=GetUserInfo(i);

		//�滭�û�
		if (pUserData!=NULL)
		{
			//�û�����
			pDC->SetTextAlign(((i==1)&&(m_bDeasilOrder==true))||((i==3)&&(m_bDeasilOrder==false))?TA_RIGHT:TA_LEFT);
			DrawTextString(pDC,pUserData->szName,RGB(240,240,240),RGB(50,50,50),m_ptName[i].x,m_ptName[i].y);

			//������Ϣ
			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);
		}
	}

	//״̬����
	CImageHandle ImageHandleStationBack(&m_ImageStationBack);
	CImageHandle ImageHandleStationTitle(&m_ImageStationTitle);
	for (int nXPos=0;nXPos<nWidth;nXPos+=m_ImageStationBack.GetWidth())
	{
		m_ImageStationBack.BitBlt(pDC->m_hDC,nXPos,nHeight-m_ImageStationBack.GetHeight()-m_nYBorder);
	}
	m_ImageStationTitle.BitBlt(pDC->m_hDC,m_nXBorder+20,nHeight-m_ImageStationTitle.GetHeight()-m_nYBorder-50);

	//������Ϣ
	if (m_cbMainValue!=VALUE_ERROR)
	{
		//��������
		LPCTSTR szValue[]=
		{
			TEXT("A"),TEXT("2"),TEXT("3"),TEXT("4"),TEXT("5"),
			TEXT("6"),TEXT("7"),TEXT("8"),TEXT("9"),TEXT("10"),
			TEXT("J"),TEXT("Q"),TEXT("K")
		};

		//�����ַ�
		TCHAR szString[32]=TEXT("");
		_snprintf(szString,CountArray(szString),TEXT("%s"),szValue[m_cbMainValue-1]);

		//�����Ϣ
		pDC->SetTextAlign(TA_LEFT|TA_TOP);
		pDC->SetTextColor(RGB(240,240,240));
		DrawTextString(pDC,szString,RGB(240,240,240),RGB(50,50,50),m_nXBorder+85,nHeight-m_nYBorder-m_SizeStation.cy+14+MOVEPOS-50);

		//�滭��ɫ
		if (m_cbCallColor!=COLOR_ERROR)
		{
			//������Դ
			CImageHandle HandleColorFlag(&m_ImageColorFlag);
			int nColorImageWidth=m_ImageColorFlag.GetWidth()/6;
			int nColorImageHeight=m_ImageColorFlag.GetHeight();

			//�滭��־
			for (BYTE i=0;i<m_cbCallCount;i++) 
			{
				int nXPos=m_nXBorder+105+i*(nColorImageWidth+4);
				int nYPos=nHeight-m_nYBorder-m_SizeStation.cy+12+MOVEPOS-50;
				m_ImageColorFlag.AlphaDrawImage(pDC,nXPos,nYPos,nColorImageWidth,nColorImageHeight,
					nColorImageWidth*(m_cbCallColor>>4),0,RGB(255,0,255));
			}
		}
		else
		{
			lstrcpyn(szString,TEXT("[ ��ɫδ�� ]"),CountArray(szString));
			DrawTextString(pDC,szString,RGB(240,240,240),RGB(50,50,50),m_nXBorder+105,nHeight-m_nYBorder-m_SizeStation.cy+14+MOVEPOS-50);
		}
	}

	//������Ϣ
/*	if ((m_cbValueOrder[0]!=VALUE_ERROR)&&(m_cbValueOrder[1]!=VALUE_ERROR))
	{
		TCHAR szString[32]=TEXT("");
		pDC->SetTextAlign(TA_LEFT|TA_TOP);
		pDC->SetTextColor(RGB(240,240,240));
		_snprintf(szString,CountArray(szString),TEXT("�Լ� %ld���Լ� %ld"),m_cbValueOrder[0],m_cbValueOrder[1]);
		DrawTextString(pDC,szString,RGB(240,240,240),RGB(50,50,50),m_nXBorder+65,nHeight-m_nYBorder-m_SizeStation.cy+14);
	}*/

	//ׯ����Ϣ
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		const tagUserData * pUserData=GetUserInfo(m_wBankerUser);
		if (pUserData!=NULL)
		{
			//ׯ������
			pDC->SetTextAlign(TA_LEFT|TA_TOP);
			DrawTextString(pDC,pUserData->szName,RGB(240,240,240),RGB(50,50,50),m_nXBorder+85,nHeight-m_nYBorder-m_SizeStation.cy+37+MOVEPOS-50);

			//ׯ�ұ�־
			CImageHandle ImageHandleBanker(&m_ImageBanker);
			m_ImageBanker.AlphaDrawImage(pDC,m_PointBanker[m_wBankerUser].x,m_PointBanker[m_wBankerUser].y,m_SizeBanker.cx,m_SizeBanker.cy,0,0,RGB(255,0,255));
		}
	}

	//�����û�
	if (m_wCallCardUser!=INVALID_CHAIR)
	{
		//��ȡ�û�
		const tagUserData * pUserData=GetUserInfo(m_wCallCardUser);
		if (pUserData!=NULL)
		{
			pDC->SetTextAlign(TA_LEFT|TA_TOP);
			DrawTextString(pDC,pUserData->szName,RGB(240,240,240),RGB(50,50,50),m_nXBorder+85,nHeight-m_nYBorder-m_SizeStation.cy+60+MOVEPOS-50);
		}
	}

	//��Ϸ�÷�
	if (m_wCardScore!=0xFFFF)
	{
		TCHAR szCardScore[16]=TEXT("");
		pDC->SetTextAlign(TA_LEFT|TA_TOP);
		pDC->SetTextColor(RGB(240,240,240));
		_snprintf(szCardScore,CountArray(szCardScore),TEXT("%ld ��"),m_wCardScore);
		DrawTextString(pDC,szCardScore,RGB(240,240,240),RGB(50,50,50),m_nXBorder+85,nHeight-m_nYBorder-m_SizeStation.cy+81+MOVEPOS-50);
	}

	//���Ʊ�־
	if (m_bWaitConceal==true)
	{
		//������Դ
		CImageHandle HandleWaitConceal(&m_ImageWaitConceal);

		//�滭��־
		int nImageWidth=m_ImageWaitConceal.GetWidth();
		int nImageHegith=m_ImageWaitConceal.GetHeight();
		m_ImageWaitConceal.AlphaDrawImage(pDC,(nWidth-nImageWidth)/2,(nHeight-m_SizeStation.cy-nImageHegith)/2-50,RGB(255,0,255));
	}

	//���ֱ�־
	if (m_bLastTurnCard==true)
	{
		//������Դ
		CImageHandle HandleLastTurn(&m_ImageLastTurn);

		//�滭��־
		int nImageWidth=m_ImageLastTurn.GetWidth();
		int nImageHegith=m_ImageLastTurn.GetHeight();
		m_ImageLastTurn.AlphaDrawImage(pDC,(nWidth-nImageWidth)/2,(nHeight-m_SizeStation.cy-nImageHegith)/2-65,RGB(255,0,255));
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

//���õ÷�
void CGameClientView::SetCardScore(WORD wCardScore)
{
	//�����û�
	if (wCardScore!=m_wCardScore)
	{
		//���ñ���
		m_wCardScore=wCardScore;

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

//������ʾ
void CGameClientView::SetWaitConceal(bool bWaitConceal)
{
	//���ñ���
	if (bWaitConceal!=m_bWaitConceal)
	{
		//���ñ���
		m_bWaitConceal=bWaitConceal;

		//���½���
		UpdateGameView(NULL);
	}

	return;
}

//��������
void CGameClientView::SetLastTurnCard(bool bLastTurnCard)
{
	//���ñ���
	if (bLastTurnCard!=m_bLastTurnCard)
	{
		//���ñ���
		m_bLastTurnCard=bLastTurnCard;

		//���½���
		UpdateGameView(NULL);
	}

	return;
}

//��������
void CGameClientView::SetValueOrder(BYTE cbMainValue, BYTE cbValueMySelf, BYTE cbValueOther)
{
	//���ñ���
	m_cbMainValue=cbMainValue;
	m_cbValueOrder[0]=cbValueMySelf;
	m_cbValueOrder[1]=cbValueOther;

	//���½���
	UpdateGameView(NULL);

	return;
}

//��������
void CGameClientView::SetCallCardInfo(WORD wCallCardUser, BYTE cbCallColor, BYTE cbCallCount)
{
	//���ñ���
	m_cbCallColor=cbCallColor;
	m_cbCallCount=cbCallCount;
	m_wCallCardUser=wCallCardUser;

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

//��ʾ��ť
void CGameClientView::OnOutPrompt()
{
	AfxGetMainWnd()->SendMessage(IDM_OUT_PROMPT,0,0);
	return;
}

//���װ�ť
void CGameClientView::OnSendConceal()
{
	AfxGetMainWnd()->SendMessage(IDM_SEND_CONCEAL,0,0);
	return;
}

//�鿴����
void CGameClientView::OnConcealCard()
{
	AfxGetMainWnd()->SendMessage(IDM_CONCEAL_CARD,0,0);
	return;
}

//����
void CGameClientView::OnKouWang()
{
	AfxGetMainWnd()->SendMessage(IDM_KOU,0,0);
	return;
}
//����
void CGameClientView::OnGiveUp()
{
	AfxGetMainWnd()->SendMessage(IDM_GIVE_UP,0,0);
	return;
}
//�����뿪
void CGameClientView::OnRuquestLeave()
{
	AfxGetMainWnd()->SendMessage(IDM_REQUEST_LEAVE,0,0);
	return;
}

//�����˿�
void CGameClientView::OnLastTurnCard()
{
	AfxGetMainWnd()->SendMessage(IDM_LAST_TURN_CARD,0,0);
	return;
}

//�Ϲܿ���
void CGameClientView::OnStusteeControl()
{
	AfxGetMainWnd()->SendMessage(IDM_TRUSTEE_CONTROL,0,0);
	return;
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
	m_btSendConceal.EnableWindow(FALSE);
	m_HandCardControl.ShootAllCard(false);
	m_KouCardControl.ShootAllCard(false);
	return;
}

//////////////////////////////////////////////////////////////////////////
