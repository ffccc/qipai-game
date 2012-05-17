#include "StdAfx.h"
#include "Resource.h"
#include "..\��Ϸ������\GameLogic.h"
#include "GameClientView.h"
#include "GameClient.h"

//////////////////////////////////////////////////////////////////////////
//��ť��ʶ

//��Ϸ��ť
#define IDC_START						100								//��ʼ��ť
#define IDC_OUT_CARD					101								//���ư�ť
#define IDC_OUT_PROMPT					102								//��ʾ��ť
#define IDC_SEND_CONCEAL				103								//���װ�ť

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
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView() : CGameFrameView()
{
	//��ʾ����
	m_bWaitConceal=false;
	m_bLastTurnCard=false;

	//��Ϸ����
	m_wCardScore=0xFFFF;
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_bStrustee,sizeof(m_bStrustee));

	//������Ϣ
	m_cbCallCount=0;
	m_cbCallColor=COLOR_ERROR;
	m_cbMainValue=VALUE_ERROR;
	m_cbValueOrder[0]=VALUE_ERROR;
	m_cbValueOrder[1]=VALUE_ERROR;
	m_wCallCardUser=INVALID_CHAIR;
	m_cbScoreCardCount = 0;
	ZeroMemory(m_cbScoreCardData,sizeof(m_cbScoreCardData));

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBanker.SetLoadInfo(IDB_BANKER,hInstance);
	m_ImageBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageCenter.SetLoadInfo(IDB_VIEW_CENTER,hInstance);
	m_ImageColorFlag.SetLoadInfo(IDB_COLOR_FLAG,hInstance);
	m_ImageLastTurn.SetLoadInfo(IDB_LAST_TURN_TIP,hInstance);
	m_ImageWaitConceal.SetLoadInfo(IDB_WAIT_CONCEAL,hInstance);
	m_ImageStatusInfo.SetLoadInfo(IDB_STATUS_INFO,hInstance);
	m_ImageScoreCard.SetLoadInfo(IDB_SCORE_CARD,hInstance);
	//m_ImageStrustee.SetLoadInfo(IDB_STRUSTEE,hInstance);
	m_PngStrustee.LoadImage(hInstance,(LPCTSTR)IDP_STRUSTEE);

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
	m_ScoreView.Create(NULL,NULL,WS_CHILD/*|WS_CLIPSIBLINGS|WS_CLIPCHILDREN*/,rcCreate,this,10);
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
	m_HandCardControl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,60);

	//�����˿�
	m_HandCardControl.SetSinkWindow(AfxGetMainWnd());

	//������ť
	m_btStart.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_START);
	m_btOutCard.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_OUT_CARD);
	m_btOutPrompt.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_OUT_PROMPT);
	m_btSendConceal.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_SEND_CONCEAL);

	//���ܰ�ť
	m_btConcealCard.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE|WS_CLIPSIBLINGS,rcCreate,this,IDC_CONCEAL_CARD);
	m_btRuquestLeave.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE|WS_CLIPSIBLINGS,rcCreate,this,IDC_REQUEST_LEAVE);
	m_btLastTurnCard.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE|WS_CLIPSIBLINGS,rcCreate,this,IDC_LAST_TURN_CARD);
	m_btStusteeControl.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE|WS_CLIPSIBLINGS,rcCreate,this,IDC_TRUSTEE_CONTROL);

	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_START,hInstance,false);
	m_btOutCard.SetButtonImage(IDB_OUT_CARD,hInstance,false);
	m_btOutPrompt.SetButtonImage(IDB_OUT_PROMPT,hInstance,false);
	m_btSendConceal.SetButtonImage(IDB_SEND_CONCEAL,hInstance,false);

	//���ܰ�ť
	m_btConcealCard.SetButtonImage(IDB_CONCEAL_CARD,hInstance,false);
	m_btRuquestLeave.SetButtonImage(IDB_REQUEST_LEAVE,hInstance,false);
	m_btLastTurnCard.SetButtonImage(IDB_LAST_TURN_CARD,hInstance,false);
	m_btStusteeControl.SetButtonImage(IDB_START_TRUSTEE,hInstance,false);

	//������ʾ
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btConcealCard,TEXT("�鿴����"));
	m_ToolTipCtrl.AddTool(&m_btRuquestLeave,TEXT("�����뿪"));
	m_ToolTipCtrl.AddTool(&m_btLastTurnCard,TEXT("��ʾ��һ�ֳ���"));
	m_ToolTipCtrl.AddTool(&m_btStusteeControl,TEXT("��ʼ��ֹͣ���й�"));

	//������Ƶ
#ifdef VIDEO_GAME
	for (WORD i=0;i<4;i++)
	{
		//������Ƶ
		m_DlgVedioService[i].Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,70+i);
		m_DlgVedioService[i].InitVideoService(i==2,i==2);

		//������Ƶ
		g_VedioServiceManager.SetVideoServiceControl(i,&m_DlgVedioService[i]);
	}
#endif

	//test
	//m_btStart.ShowWindow(SW_SHOW);
	//m_btOutCard.ShowWindow(SW_SHOW);
	//m_btOutPrompt.ShowWindow(SW_SHOW);
	//m_btSendConceal.ShowWindow(SW_SHOW);
	//BYTE cbCard[MAX_COUNT] = {
	//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,/*255,*/
	//	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	
	//	0x21,0x22,0x23,0x24,0x25
	//};
	////BYTE cbCardCount = 10;
	//m_HandCardControl.SetCardData(cbCard,28);
	//m_HandCardControl.SetDisplayFlag(true);
	//m_HandCardControl.SetPositively(true);
	//for( BYTE i = 0; i < GAME_PLAYER; i++ )
	//{
	//	m_UserCardControl[i].SetCardData(cbCard,10);
	//}
	////m_UserCardControl[2].SetCardData(NULL,0);
	//tagScoreViewInfo info;
	//info.cbConcealCount = 8;
	//CopyMemory(info.cbConcealCard,cbCard,sizeof(BYTE)*info.cbConcealCount);
	//info.wConcealScore = 5;
	//info.wConcealTime = 1;
	//info.wGameScore = 1;
	//m_ScoreView.SetScoreViewInfo(info);
	//for( i = 0; i < GAME_PLAYER; i++ )
	//{
	//	m_ScoreView.SetGameScore(i,TEXT("SSSSS"),3);
	//}
	//m_ScoreView.ShowWindow(SW_SHOW);
	//BYTE cbScoreCard[] = 
	//{
	//	0x05,0x0a,0x0d,
	//	0x15,0x1a,0x1d,
	//	0x25,0x2a,0x2d,
	//	0x35,0x2a,0x2d,
	//	0x05,0x0a,0x0d,
	//	0x15,0x1a,0x1d,
	//	0x25,0x2a,0x2d,
	//	0x35,0x2a,0x2d
	//};
	//SetCardScore(100);
	//SetScoreData(cbScoreCard,24);
	//SetBankerUser(1);
	//m_cbValueOrder[0] = 1;
	//m_cbValueOrder[1] = 2;
	//m_cbMainValue = 2;
	//m_wCallCardUser = 0;
	//m_cbCallColor = 0x40;
	//m_cbCallCount = 2;
	//SetStrustee(INVALID_CHAIR,true);
	//m_CallCardWnd.ShowWindow(SW_SHOW);
	//m_CallCardWnd.UpdateCallControl(0xff);
	//end test

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
	ZeroMemory(m_bStrustee,sizeof(m_bStrustee));

	//������Ϣ
	m_cbCallCount=0;
	m_cbCallColor=COLOR_ERROR;
	m_cbMainValue=VALUE_ERROR;
	m_cbValueOrder[0]=VALUE_ERROR;
	m_cbValueOrder[1]=VALUE_ERROR;
	m_wCallCardUser=INVALID_CHAIR;
	m_cbScoreCardCount = 0;
	ZeroMemory(m_cbScoreCardData,sizeof(m_cbScoreCardData));

	//���ؿؼ�
	m_btStart.ShowWindow(SW_HIDE);
	m_btOutCard.ShowWindow(SW_HIDE);
	m_btOutPrompt.ShowWindow(SW_HIDE);
	m_btSendConceal.ShowWindow(SW_HIDE);

	//���ð�ť
	m_btConcealCard.EnableWindow(FALSE);
	m_btRuquestLeave.EnableWindow(FALSE);
	m_btLastTurnCard.EnableWindow(FALSE);
	m_btStusteeControl.EnableWindow(FALSE);
	m_btStusteeControl.SetButtonImage(IDB_START_TRUSTEE,AfxGetInstanceHandle(),false);

	//�����˿�
	m_HandCardControl.SetCardData(NULL,0);
	m_HandCardControl.SetPositively(false);
	m_HandCardControl.SetDisplayFlag(false);
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
#ifdef VIDEO_GAME
	//��������
	int nViewHeight=nHeight-m_nYBorder;

	//��������
	m_ptFace[3].x=nWidth-m_nXBorder-m_nXFace-5;
	m_ptFace[3].y=nViewHeight/2-m_nYFace-25;
	m_ptName[3].x=nWidth-m_nXBorder-5;
	m_ptName[3].y=m_ptFace[3].y+m_nYFace+5;
	m_ptTimer[3].x=nWidth-m_nXBorder-m_nXTimer/2-5;
	m_ptTimer[3].y=m_ptFace[3].y-5-m_nYTimer/2;
	m_ptReady[3].x=nWidth-m_nXBorder-m_nXFace-55;
	m_ptReady[3].y=m_ptFace[3].y + m_nYFace/2 - 5;
	m_PointBanker[3].x=nWidth-m_nXBorder-43;
	m_PointBanker[3].y=m_ptFace[3].y-97;
	m_ptStrustee[3].x = m_ptFace[3].x-50;
	m_ptStrustee[3].y = m_ptFace[3].y-50;

	m_ptFace[1].x=m_nXBorder+5;
	m_ptFace[1].y=nViewHeight/2-m_nYFace-25;
	m_ptName[1].x=m_nXBorder+5;
	m_ptName[1].y=m_ptFace[1].y+m_nYFace+5;
	m_ptTimer[1].x=m_nXBorder+m_nXTimer/2+5;
	m_ptTimer[1].y=m_ptFace[1].y-5-m_nYTimer/2;
	m_ptReady[1].x=m_ptFace[1].x + m_nXFace + 55;
	m_ptReady[1].y=m_ptFace[1].y+m_nYFace/2-5;
	m_PointBanker[1].x=m_nXBorder+5;
	m_PointBanker[1].y=m_ptFace[1].y-97;
	m_ptStrustee[1].x = m_ptFace[1].x+m_nXFace+20;
	m_ptStrustee[1].y = m_ptFace[1].y-50;

	m_ptFace[0].x=nWidth/2-m_nXFace/2;
	m_ptFace[0].y=m_nYBorder+5;
	m_ptName[0].x=nWidth/2+5+m_nXFace/2;
	m_ptName[0].y=m_nYBorder+5;
	m_ptTimer[0].x=m_ptName[0].x+90;
	m_ptTimer[0].y=m_nYBorder+m_nYTimer/2+5;
	m_ptReady[0].x=nWidth/2;
	m_ptReady[0].y=m_nYBorder+m_nYFace+35;
	m_PointBanker[0].x=nWidth/2-m_nXFace/2-80;
	m_PointBanker[0].y=m_nYBorder+5;
	m_ptStrustee[0].x = m_PointBanker[0].x+45;
	m_ptStrustee[0].y = m_ptName[0].y;

	m_ptFace[2].x=nWidth/2-m_nXFace/2;
	m_ptFace[2].y=nViewHeight-m_nYFace-5;
	m_ptName[2].x=nWidth/2+5+m_nXFace/2;
	m_ptName[2].y=nViewHeight-m_nYFace-2;
	m_ptTimer[2].x=m_ptName[2].x+90;
	m_ptTimer[2].y=nViewHeight-m_nYTimer/2;
	m_ptReady[2].x=nWidth/2;
	m_ptReady[2].y=nViewHeight-m_nYBorder-220;
	m_PointBanker[2].x=nWidth/2-m_nXFace/2-80;
	m_PointBanker[2].y=nViewHeight-42;
	m_ptStrustee[2].x = m_PointBanker[2].x+45;
	m_ptStrustee[2].y = m_ptName[2].y-3;

	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//��ʼ��ť
	m_btStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nViewHeight-195,0,0,uFlags);

	//���װ�ť
	m_btSendConceal.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btSendConceal,NULL,(nWidth-rcButton.Width())/2,nViewHeight-210,0,0,uFlags);

	//���ư�ť
	m_btOutCard.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btOutPrompt,NULL,nWidth/2+15,nViewHeight-210,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOutCard,NULL,nWidth/2-rcButton.Width()-15,nViewHeight-210,0,0,uFlags);

	//���ܰ�ť
	m_btConcealCard.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStusteeControl,NULL,nWidth-m_nXBorder-(rcButton.Width()+5),nViewHeight-5-rcButton.Height(),0,0,uFlags);
	DeferWindowPos(hDwp,m_btRuquestLeave,NULL,nWidth-m_nXBorder-(rcButton.Width()+5)*2,nViewHeight-5-rcButton.Height(),0,0,uFlags);
	DeferWindowPos(hDwp,m_btConcealCard,NULL,nWidth-m_nXBorder-(rcButton.Width()+5)*3,nViewHeight-5-rcButton.Height(),0,0,uFlags);
	DeferWindowPos(hDwp,m_btLastTurnCard,NULL,nWidth-m_nXBorder-(rcButton.Width()+5)*4,nViewHeight-5-rcButton.Height(),0,0,uFlags);

	//��Ƶ����
	CRect rcAVDlg;
	m_DlgVedioService[0].GetWindowRect(&rcAVDlg);
	DeferWindowPos(hDwp,m_DlgVedioService[3],NULL,nWidth-m_nXBorder-5-rcAVDlg.Width(),nHeight/2-3,0,0,uFlags);
	DeferWindowPos(hDwp,m_DlgVedioService[1],NULL,m_nXBorder+5,nHeight/2-3,0,0,uFlags);
	DeferWindowPos(hDwp,m_DlgVedioService[0],NULL,nWidth-m_nXBorder-5-rcAVDlg.Width(),5,0,0,uFlags);
	m_DlgVedioService[2].GetWindowRect(&rcAVDlg);
	DeferWindowPos(hDwp,m_DlgVedioService[2],NULL,m_nXBorder+5,nHeight-m_nYBorder-3-rcAVDlg.Height(),0,0,uFlags);

	//�ƶ��ؼ�
	EndDeferWindowPos(hDwp);

	//�ƶ��˿�
	m_UserCardControl[0].SetBenchmarkPos(nWidth/2,m_nYBorder+m_nYFace+60,enXCenter,enYTop);
	m_UserCardControl[2].SetBenchmarkPos(nWidth/2,nViewHeight-221-m_nYBorder,enXCenter,enYBottom);
	m_HandCardControl.SetBenchmarkPos(nWidth/2,nViewHeight-m_nYFace-m_nYBorder-20,enXCenter,enYBottom);

	//�û��˿�
	m_UserCardControl[3].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-110,nViewHeight/2-71,enXRight,enYCenter);
	m_UserCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+110,nViewHeight/2-71,enXLeft,enYCenter);

	//���ƴ���
	CRect rcCallCard;
	m_CallCardWnd.GetWindowRect(&rcCallCard);
	m_CallCardWnd.SetWindowPos(NULL,nWidth/2-5,nViewHeight-m_nYBorder-rcCallCard.Height()-182,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//������ͼ
	CRect rcScore;
	m_ScoreView.GetWindowRect(&rcScore);
	m_ScoreView.SetWindowPos(NULL,(nWidth-rcScore.Width())/2,nViewHeight/2-200,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//���ƴ���
	CRect rcConceal;
	m_ConcealCardView.GetWindowRect(&rcConceal);
	m_ConcealCardView.SetWindowPos(NULL,(nWidth-rcConceal.Width())/2,nViewHeight/2-160,0,0,SWP_NOZORDER|SWP_NOSIZE);

#else
	//��������
	int nViewHeight=nHeight-m_nYBorder;

	//��������
	m_ptFace[3].x=nWidth-m_nXBorder-m_nXFace-5;
	m_ptFace[3].y=nViewHeight/2-m_nYFace-25;
	m_ptName[3].x=nWidth-m_nXBorder-5;
	m_ptName[3].y=m_ptFace[3].y+m_nYFace+5;
	m_ptTimer[3].x=nWidth-m_nXBorder-m_nXTimer/2-5;
	m_ptTimer[3].y=m_ptName[3].y+45;
	m_ptReady[3].x=nWidth-m_nXBorder-m_nXFace-55;
	m_ptReady[3].y=m_ptFace[3].y + m_nYFace/2 - 5;
	m_PointBanker[3].x=nWidth-m_nXBorder-43;
	m_PointBanker[3].y=m_ptFace[3].y-50;
	m_ptStrustee[3].x = m_ptFace[3].x-45;
	m_ptStrustee[3].y = m_ptFace[3].y-50;

	m_ptFace[1].x=m_nXBorder+5;
	m_ptFace[1].y=nViewHeight/2-m_nYFace-25;
	m_ptName[1].x=m_nXBorder+5;
	m_ptName[1].y=m_ptFace[1].y+m_nYFace+5;
	m_ptTimer[1].x=m_nXBorder+m_nXTimer/2+5;
	m_ptTimer[1].y=m_ptName[1].y+45;
	m_ptReady[1].x=m_ptFace[1].x + m_nXFace + 55;
	m_ptReady[1].y=m_ptFace[1].y+m_nYFace/2-5;
	m_PointBanker[1].x=m_nXBorder+5;
	m_PointBanker[1].y=m_ptFace[1].y-50;
	m_ptStrustee[1].x = m_ptFace[1].x+m_nXFace+15;
	m_ptStrustee[1].y = m_ptFace[1].y-50;

	m_ptFace[0].x=nWidth/2-m_nXFace/2;
	m_ptFace[0].y=m_nYBorder+5;
	m_ptName[0].x=nWidth/2+5+m_nXFace/2;
	m_ptName[0].y=m_nYBorder+5;
	m_ptTimer[0].x=m_ptName[0].x+90;
	m_ptTimer[0].y=m_nYBorder+m_nYTimer/2+5;
	m_ptReady[0].x=nWidth/2;
	m_ptReady[0].y=m_nYBorder+m_nYFace+35;
	m_PointBanker[0].x=nWidth/2-m_nXFace/2-80;
	m_PointBanker[0].y=m_nYBorder+5;
	m_ptStrustee[0].x = m_PointBanker[0].x+45;
	m_ptStrustee[0].y = m_ptName[0].y;

	m_ptFace[2].x=nWidth/2-m_nXFace/2;
	m_ptFace[2].y=nViewHeight-m_nYFace-5;
	m_ptName[2].x=nWidth/2+5+m_nXFace/2;
	m_ptName[2].y=nViewHeight-m_nYFace-2;
	m_ptTimer[2].x=m_ptName[2].x+90;
	m_ptTimer[2].y=nViewHeight-m_nYTimer/2;
	m_ptReady[2].x=nWidth/2;
	m_ptReady[2].y=nViewHeight-m_nYBorder-220;
	m_PointBanker[2].x=nWidth/2-m_nXFace/2-80;
	m_PointBanker[2].y=nViewHeight-42;
	m_ptStrustee[2].x = m_PointBanker[2].x+45;
	m_ptStrustee[2].y = m_ptName[2].y-3;

	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//��ʼ��ť
	m_btStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nViewHeight-195,0,0,uFlags);

	//���װ�ť
	m_btSendConceal.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btSendConceal,NULL,(nWidth-rcButton.Width())/2,nViewHeight-210,0,0,uFlags);

	//���ư�ť
	m_btOutCard.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btOutPrompt,NULL,nWidth/2+15,nViewHeight-210,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOutCard,NULL,nWidth/2-rcButton.Width()-15,nViewHeight-210,0,0,uFlags);

	//���ܰ�ť
	m_btConcealCard.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStusteeControl,NULL,nWidth-m_nXBorder-(rcButton.Width()+5),nViewHeight-5-rcButton.Height(),0,0,uFlags);
	DeferWindowPos(hDwp,m_btRuquestLeave,NULL,nWidth-m_nXBorder-(rcButton.Width()+5)*2,nViewHeight-5-rcButton.Height(),0,0,uFlags);
	DeferWindowPos(hDwp,m_btConcealCard,NULL,nWidth-m_nXBorder-(rcButton.Width()+5)*3,nViewHeight-5-rcButton.Height(),0,0,uFlags);
	DeferWindowPos(hDwp,m_btLastTurnCard,NULL,nWidth-m_nXBorder-(rcButton.Width()+5)*4,nViewHeight-5-rcButton.Height(),0,0,uFlags);

	//�ƶ��ؼ�
	EndDeferWindowPos(hDwp);

	//�ƶ��˿�
	m_UserCardControl[0].SetBenchmarkPos(nWidth/2,m_nYBorder+m_nYFace+60,enXCenter,enYTop);
	m_UserCardControl[2].SetBenchmarkPos(nWidth/2,nViewHeight-221-m_nYBorder,enXCenter,enYBottom);
	m_HandCardControl.SetBenchmarkPos(nWidth/2,nViewHeight-m_nYFace-m_nYBorder-20,enXCenter,enYBottom);

	//�û��˿�
	m_UserCardControl[3].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-110,nViewHeight/2-71,enXRight,enYCenter);
	m_UserCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+110,nViewHeight/2-71,enXLeft,enYCenter);

	//���ƴ���
	CRect rcCallCard;
	m_CallCardWnd.GetWindowRect(&rcCallCard);
	m_CallCardWnd.SetWindowPos(NULL,nWidth/2+20,nViewHeight-m_nYBorder-rcCallCard.Height()-182,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//������ͼ
	CRect rcScore;
	m_ScoreView.GetWindowRect(&rcScore);
	m_ScoreView.SetWindowPos(NULL,(nWidth-rcScore.Width())/2,nViewHeight/2-200,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//���ƴ���
	CRect rcConceal;
	m_ConcealCardView.GetWindowRect(&rcConceal);
	m_ConcealCardView.SetWindowPos(NULL,(nWidth-rcConceal.Width())/2,nViewHeight/2-160,0,0,SWP_NOZORDER|SWP_NOSIZE);

#endif		//VIDEO_GAME

	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageBack,enMode_Spread);
	//DrawViewImage(pDC,m_ImageCenter,enMode_Centent);
	CImageHandle HandleCenter(&m_ImageCenter);
	m_ImageCenter.BitBlt(pDC->m_hDC,nWidth/2-m_ImageCenter.GetWidth()/2,
		nHeight/2-m_ImageCenter.GetHeight()/2-20);

	//�滭�û�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wUserTimer=GetUserTimer(i);
		const tagUserData * pUserData=GetUserInfo(i);
		//test
		//WORD wUserTimer = 30;
		//tagUserData *pUserData = new tagUserData;
		//lstrcpy(pUserData->szName,TEXT("�������"));
		//pUserData->lScore = 10000L;
		//pUserData->cbUserStatus = US_READY;
		//�滭�û�
		if (pUserData!=NULL)
		{
			//�û�����
			pDC->SetTextAlign(i==3?TA_RIGHT:TA_LEFT);
			DrawTextString(pDC,pUserData->szName,RGB(255,255,255),RGB(0,0,0),m_ptName[i].x,m_ptName[i].y);

			//������Ϣ
			if (wUserTimer!=0) DrawUserTimer(pDC,m_ptTimer[i].x,m_ptTimer[i].y,wUserTimer);
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);
			//�йܱ�־
			if( m_bStrustee[i] )
				m_PngStrustee.DrawImage(pDC,m_ptStrustee[i].x,m_ptStrustee[i].y);
		}
		//delete pUserData;
	}
	
	//�÷��˿�
	CImageHandle HandleScoreBack(&m_ImageStatusInfo);
	int nXPos = m_nXBorder+3;
	int nYPos = m_nYBorder + 3;
	m_ImageStatusInfo.BitBlt(pDC->m_hDC,nXPos,nYPos);
	if (m_cbScoreCardCount>0)
	{
		//������Դ
		CImageHandle HandleScoreCard(&m_ImageScoreCard);
		CSize SizeScoreCard(m_ImageScoreCard.GetWidth()/3,m_ImageScoreCard.GetHeight()/4);

		//�滭�˿�
		for (WORD i=0;i<m_cbScoreCardCount;i++)
		{
			//��ȡλ��
			INT nXImagePos=0;
			INT nYImagePos=0;

			//��ȡλ��
			switch (m_cbScoreCardData[i]&0x0F)
			{
			case 5:
				{
					nXImagePos=0;
					nYImagePos=((m_cbScoreCardData[i]&0xF0)>>4)*SizeScoreCard.cy;
					break;
				}
			case 10:
				{
					nXImagePos=SizeScoreCard.cx;
					nYImagePos=((m_cbScoreCardData[i]&0xF0)>>4)*SizeScoreCard.cy;
					break;
				}
			case 13:
				{
					nXImagePos=SizeScoreCard.cx*2;
					nYImagePos=((m_cbScoreCardData[i]&0xF0)>>4)*SizeScoreCard.cy;
					break;
				}
			}

			//�滭�˿�
			INT nXPos1=nXPos+14*(i%15)+3;
			INT nYPos1=nYPos+25*(i/15)+3;
			m_ImageScoreCard.AlphaDrawImage(pDC,nXPos1,nYPos1,SizeScoreCard.cx,SizeScoreCard.cy,nXImagePos,nYImagePos,RGB(17,66,87));
		}
	}

	nXPos += 64;
	nYPos += 81;
	CRect rcDraw(nXPos,nYPos,nXPos+163,nYPos+17);
	//������Ϣ
	if ((m_cbValueOrder[0]!=VALUE_ERROR)&&(m_cbValueOrder[1]!=VALUE_ERROR))
	{
		//��������
		LPCTSTR szValue[]=
		{
			TEXT("A"),TEXT("2"),TEXT("3"),TEXT("4"),TEXT("5"),
			TEXT("6"),TEXT("7"),TEXT("8"),TEXT("9"),TEXT("10"),
			TEXT("J"),TEXT("Q"),TEXT("K")
		};

		//������Ϣ
		TCHAR szValueOrder[128]=TEXT("");
		_snprintf(szValueOrder,CountArray(szValueOrder),TEXT("%s  [ ������ %s���Է��� %s ]"),szValue[m_cbMainValue-1],
			szValue[m_cbValueOrder[0]-1],szValue[m_cbValueOrder[1]-1]);

		//�滭��Ϣ
		pDC->SetTextAlign(TA_LEFT);
		DrawTextString(pDC,szValueOrder,RGB(255,255,255),RGB(0,0,0),rcDraw);
	}

	rcDraw.OffsetRect(0,23);
	rcDraw.right = rcDraw.left+78;
	//�����û�
	if (m_wCallCardUser!=INVALID_CHAIR)
	{
		//��ȡ�û�
		const tagUserData *pUserData = GetUserInfo(m_wCallCardUser);
		//test
		//tagUserData *pUserData = new tagUserData;
		//_snprintf(pUserData->szName,CountArray(pUserData->szName),TEXT("%s"),TEXT("SSSSSS"));

		//�滭�û�
		if (pUserData!=NULL)
		{
			pDC->SetTextAlign(TA_LEFT);
			DrawTextString(pDC,pUserData->szName,RGB(255,255,255),RGB(0,0,0),rcDraw);

			//���ƻ�ɫ
			if (m_cbCallCount>0)
			{
				//������Դ
				CSize SizeCallColor;
				CImageHandle HandleCallColor(&m_ImageColorFlag);
				SizeCallColor.SetSize(m_ImageColorFlag.GetWidth()/5,m_ImageColorFlag.GetHeight());

				//���ƻ�ɫ
				for (BYTE i=0;i<m_cbCallCount;i++)
				{
					INT nXPos1=rcDraw.right+46+i*SizeCallColor.cx-(m_cbCallCount*SizeCallColor.cx)/2;
					m_ImageColorFlag.AlphaDrawImage(pDC,nXPos1,rcDraw.top,SizeCallColor.cx,SizeCallColor.cy,
						SizeCallColor.cx*((m_cbCallColor&0xF0)>>4),0,RGB(255,0,255));
				}
			}
		}
		//delete pUserData;
	}

	rcDraw.OffsetRect(0,23);
	//��ǰ�÷�
	if (m_wCardScore!=0xFFFF)
	{
		//������Ϣ
		TCHAR szCardScore[16]=TEXT("");
		_snprintf(szCardScore,CountArray(szCardScore),TEXT("%d"),m_wCardScore);

		//�滭��Ϣ
		pDC->SetTextAlign(TA_LEFT);
		DrawTextString(pDC,szCardScore,RGB(255,255,255),RGB(0,0,0),&rcDraw);
	}

	//ׯ����Ϣ
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//ׯ�ұ�־
		CImageHandle ImageHandleBanker(&m_ImageBanker);
		m_ImageBanker.AlphaDrawImage(pDC,m_PointBanker[m_wBankerUser].x,m_PointBanker[m_wBankerUser].y,m_ImageBanker.GetWidth(),m_ImageBanker.GetHeight()
			,0,0,RGB(255,0,255));
	}

	//���Ʊ�־
	if (m_bWaitConceal==true)
	{
		//������Դ
		CImageHandle HandleWaitConceal(&m_ImageWaitConceal);

		//�滭��־
		int nImageWidth=m_ImageWaitConceal.GetWidth();
		int nImageHegith=m_ImageWaitConceal.GetHeight();
		m_ImageWaitConceal.AlphaDrawImage(pDC,(nWidth-nImageWidth)/2,(nHeight-nImageHegith)/2-50,RGB(255,0,255));
	}

	//���ֱ�־
	if (m_bLastTurnCard==true)
	{
		//������Դ
		CImageHandle HandleLastTurn(&m_ImageLastTurn);

		//�滭��־
		int nImageWidth=m_ImageLastTurn.GetWidth();
		int nImageHegith=m_ImageLastTurn.GetHeight();
		m_ImageLastTurn.AlphaDrawImage(pDC,(nWidth-nImageWidth)/2,(nHeight-nImageHegith)/2-65,RGB(255,0,255));
	}

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

	//�����˿�
	m_btOutCard.EnableWindow(FALSE);
	m_btSendConceal.EnableWindow(FALSE);
	m_HandCardControl.ShootAllCard(false);

	//test
	//AfxGetMainWnd()->SendMessage(IDM_TEST);
	//TCHAR szBuffer[16]=TEXT("");
	//_snprintf(szBuffer,CountArray(szBuffer),TEXT("(%d,%d)"),Point.x,Point.y);
	//MessageBox(szBuffer);
	//end test

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

	return;
}

//���õ÷���
void CGameClientView::SetScoreData( const BYTE cbScoreCard[],BYTE cbCardCount )
{
	if( cbCardCount == 0 || cbScoreCard == NULL )
	{
		m_cbScoreCardCount = 0;
		ZeroMemory(m_cbScoreCardData,sizeof(m_cbScoreCardData));
	}else
	{
		m_cbScoreCardCount = cbCardCount;
		CopyMemory(m_cbScoreCardData,cbScoreCard,sizeof(BYTE)*cbCardCount);
	}
	UpdateGameView(NULL);
}

//�����йܱ�־
void CGameClientView::SetStrustee( WORD wChairId,bool bStrustee )
{
	if( INVALID_CHAIR == wChairId )
	{
		memset(m_bStrustee,bStrustee,sizeof(m_bStrustee));
	}else
	{
		m_bStrustee[wChairId] = bStrustee;
	}
	UpdateGameView(NULL);
	return;
}

//////////////////////////////////////////////////////////////////////////
