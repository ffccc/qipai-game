#include "StdAfx.h"
#include "Math.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClientDlg.h"
#include ".\gameclientview.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FLASH_WINNER			100									//������ʶ
#define IDI_SHOW_CHANGE_BANKER		101									//�ֻ�ׯ��
#define IDI_DISPATCH_CARD			102									//���Ʊ�ʶ
#define IDI_BOMB_EFFECT				200									//��ը��ʶ


//��ť��ʶ
#define IDC_JETTON_BUTTON_100		200									//��ť��ʶ
#define IDC_JETTON_BUTTON_1000		201									//��ť��ʶ
#define IDC_JETTON_BUTTON_10000		202									//��ť��ʶ
#define IDC_JETTON_BUTTON_100000	203									//��ť��ʶ
#define IDC_JETTON_BUTTON_1000000	204									//��ť��ʶ
#define IDC_JETTON_BUTTON_5000000	205									//��ť��ʶ
#define IDC_APPY_BANKER				206									//��ť��ʶ
#define IDC_CANCEL_BANKER			207									//��ť��ʶ
#define IDC_SCORE_MOVE_L			209									//��ť��ʶ
#define IDC_SCORE_MOVE_R			210									//��ť��ʶ
#define IDC_VIEW_CHART				211									//��ť��ʶ
#define IDC_JETTON_BUTTON_50000		212									//��ť��ʶ
#define IDC_JETTON_BUTTON_500000	213									//��ť��ʶ

#define IDC_SHOUDONG				221
#define IDC_ZIDONG					222


//��ը��Ŀ
#define BOMB_EFFECT_COUNT			8									//��ը��Ŀ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_JETTON_BUTTON_100, OnJettonButton100)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_1000, OnJettonButton1000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_10000, OnJettonButton10000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_50000, OnJettonButton50000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_100000, OnJettonButton100000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_500000, OnJettonButton500000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_1000000, OnJettonButton1000000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_5000000, OnJettonButton5000000)
	ON_BN_CLICKED(IDC_APPY_BANKER, OnApplyBanker)
	ON_BN_CLICKED(IDC_CANCEL_BANKER, OnCancelBanker)
	ON_BN_CLICKED(IDC_SCORE_MOVE_L, OnScoreMoveL)
	ON_BN_CLICKED(IDC_SCORE_MOVE_R, OnScoreMoveR)

	ON_BN_CLICKED(IDC_SHOUDONG,OnShouDongModel)
	ON_BN_CLICKED(IDC_ZIDONG,OnZiDongModel)

	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView() : CGameFrameView(true,24)
{
	//��ע��Ϣ

	m_lMeTianScore=0L;						//��
	m_lMeDiScore=0L;						//��
	m_lMeXuanScore=0L;						//��
	m_lMeHuangScore=0L;					//��

	//���Ʊ���
	ZeroMemory(m_cbSendCount,sizeof(m_cbSendCount));


	ZeroMemory(m_cbNiuType,sizeof(m_cbNiuType));
	
	//ȫ����ע
	m_lAllTianScore=0L;					//��
	m_lAllDiScore=0L;					//��
	m_lAllXuanScore=0L;					//��
	m_lAllHuangScore=0L;				//��

	//ׯ����Ϣ
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_lBankerScore=0L;	
	m_lBankerWinScore=0L;
	m_lTmpBankerWinScore=0;

	//���ֳɼ�
	m_lMeCurGameScore=0L;	
	m_lMeCurGameReturnScore=0L;
	m_lBankerCurGameScore=0L;
	m_lGameRevenue=0L;
	m_bCanGo=false;
	m_bSet=false;
	m_bShowResult=false;

	//��������
	ZeroMemory(m_bBombEffect,sizeof(m_bBombEffect));
	ZeroMemory(m_cbBombFrameIndex,sizeof(m_cbBombFrameIndex));

	//״̬��Ϣ
	m_lCurrentJetton=0L;
	m_cbWinnerSide=0xFF;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=false;

	m_lMeCurGameScore=0L;			
	m_lMeCurGameReturnScore=0L;	
	m_lBankerCurGameScore=0L;		

	m_lAreaLimitScore=0L;	

	//λ����Ϣ
	m_nScoreHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;	

	//��ʷ�ɼ�
	m_lMeStatisticScore=0;

	//�ؼ�����
	m_pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageViewFill.SetLoadInfo(IDB_VIEW_FILL,hInstance);
	m_ImageViewBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageWinFlags.SetLoadInfo(IDB_WIN_FLAGS,hInstance);
	m_ImageJettonView.SetLoadInfo(IDB_JETTOM_VIEW,hInstance);
	m_ImageScoreNumber.SetLoadInfo(IDB_SCORE_NUMBER,hInstance);
	m_ImageMeScoreNumber.SetLoadInfo(IDB_ME_SCORE_NUMBER,hInstance);

	m_ImageFrameXianJia.SetLoadInfo( IDB_FRAME_XIAN_JIA, hInstance );
	m_ImageFrameZhuangJia.SetLoadInfo( IDB_FRAME_ZHUANG_JIA, hInstance );
	//m_ImageFrameXianTianWang.SetLoadInfo( IDB_FRAME_XIAN_TIAN_WANG, hInstance );
	//m_ImageFrameZhuangTianWang.SetLoadInfo( IDB_FRAME_ZHUANG_TIAN_WANG, hInstance );
	//m_ImageFramePingJia.SetLoadInfo( IDB_FRAME_PING_JIA, hInstance );
	//m_ImageFrameTongDianPing.SetLoadInfo( IDB_FRAME_TONG_DIAN_PING, hInstance );
	//m_ImageFramePlayerTwoPair.SetLoadInfo( IDB_PLAYER_TWO_PAIR, hInstance );
	//m_ImageFrameBankerTwoPair.SetLoadInfo( IDB_BANKER_TWO_PAIR, hInstance );


	m_ImageGameEnd.SetLoadInfo( IDB_GAME_END, hInstance );
	m_ImageGameEndFrame.SetLoadInfo( IDB_GAME_END_FRAME, hInstance );
	m_ImageGamePoint.SetLoadInfo( IDB_GAME_POINT, hInstance );

	m_ImageMeBanker.SetLoadInfo( IDB_ME_BANKER, hInstance );
	m_ImageChangeBanker.SetLoadInfo( IDB_CHANGE_BANKER, hInstance );
	m_ImageNoBanker.SetLoadInfo( IDB_NO_BANKER, hInstance );	

	m_ImageTimeFlag.SetLoadInfo(IDB_TIME_FLAG, hInstance);

	m_ImageNiu.SetLoadInfo(IDB_BMP_NIUNIU,hInstance);

	m_ImageBombEffect.LoadImage(hInstance,TEXT("FIRE_EFFECT"));

	m_bAutoModel=true;
	m_cbWaitPre=0;
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
	CRect rcCreate(0,0,0,0);
	m_GameRecord.Create(IDD_DLG_GAME_RECORD,this);
	m_ApplyUser.Create( IDD_DLG_GAME_RECORD	, this );
	m_GameRecord.ShowWindow(SW_HIDE);
	
	//��ע��ť
	m_btJetton100.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100);
	m_btJetton1000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1000);
	m_btJetton10000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_10000);
	m_btJetton50000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_50000);
	m_btJetton100000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100000);
	m_btJetton500000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_500000);
	m_btJetton1000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1000000);
	m_btJetton5000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_5000000);
//	m_btViewChart.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_VIEW_CHART);
		
	//���밴ť
	m_btApplyBanker.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_APPY_BANKER);
	m_btCancelBanker.Create(NULL,WS_CHILD|WS_DISABLED,rcCreate,this,IDC_CANCEL_BANKER);

	m_btScoreMoveL.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_L);
	m_btScoreMoveR.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_R);

//	m_btnShouDong.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_SHOUDONG);
//	m_btnZiDong.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_ZIDONG);
	//���ð�ť
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btJetton100.SetButtonImage(IDB_BT_JETTON_100,hResInstance,false);
	m_btJetton1000.SetButtonImage(IDB_BT_JETTON_1000,hResInstance,false);
	m_btJetton10000.SetButtonImage(IDB_BT_JETTON_10000,hResInstance,false);
	m_btJetton50000.SetButtonImage(IDB_BT_JETTON_50000,hResInstance,false);
	m_btJetton100000.SetButtonImage(IDB_BT_JETTON_100000,hResInstance,false);
	m_btJetton500000.SetButtonImage(IDB_BT_JETTON_500000,hResInstance,false);
	m_btJetton1000000.SetButtonImage(IDB_BT_JETTON_1000000,hResInstance,false);
	m_btJetton5000000.SetButtonImage(IDB_BT_JETTON_5000000,hResInstance,false);

//	m_btViewChart.SetButtonImage(IDB_BT_CHART,hResInstance,false);	

	m_btApplyBanker.SetButtonImage(IDB_BT_APPLY_BANKER,hResInstance,false);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,hResInstance,false);

	m_btScoreMoveL.SetButtonImage(IDB_BT_SCORE_MOVE_L,hResInstance,false);
	m_btScoreMoveR.SetButtonImage(IDB_BT_SCORE_MOVE_R,hResInstance,false);

//	m_btnShouDong.SetButtonImage(IDB_BMP_SHOUGONG,hResInstance,false);
//	m_btnZiDong.SetButtonImage(IDB_BMP_ZIDONG,hResInstance,false);

	//�˿˿ؼ�
	//m_CardControl[INDEX_BANKER].SetDisplayFlag(true);
	//m_CardControl[INDEX_PLAYER1].SetDisplayFlag(true);
	//m_CardControl[INDEX_PLAYER2].SetDisplayFlag(true);
	//m_CardControl[INDEX_PLAYER3].SetDisplayFlag(true);
	//m_CardControl[INDEX_PLAYER4].SetDisplayFlag(true);


	for(BYTE i=0;i<6;i++)
	{
		m_CardControl[i].SetParentWnd(this);
	}



	return 0;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//���Ʊ���
	ZeroMemory(m_cbSendCount,sizeof(m_cbSendCount));

	//����б�
	m_ApplyUser.ClearAll();

	//�������
	CleanUserJetton();

	//��������
	ZeroMemory(m_bBombEffect,sizeof(m_bBombEffect));
	ZeroMemory(m_cbBombFrameIndex,sizeof(m_cbBombFrameIndex));

	ZeroMemory(m_cbNiuType,sizeof(m_cbNiuType));



	//���ð�ť
	m_btApplyBanker.ShowWindow(SW_SHOW);
	m_btApplyBanker.EnableWindow(FALSE);
	m_btCancelBanker.ShowWindow(SW_HIDE);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,AfxGetInstanceHandle(),false);

	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//λ����Ϣ
	m_nWinFlagsExcursionX = nWidth/2-300;	
	m_nWinFlagsExcursionY = nHeight/2 + 227;
	m_nWinPointsExcursionX = nWidth/2-353;
	m_nWinPointsExcursionY = nHeight/2 - 267;	



	//����λ��
	int nCenterX=nWidth/2, nCenterY=nHeight/2;

	m_ptDrawNiu[0].x=nCenterX+80+10;
	m_ptDrawNiu[0].y=nCenterY-130;
	m_ptDrawNiu[1].x=nCenterX-300;
	m_ptDrawNiu[1].y=nCenterY+180;
	m_ptDrawNiu[2].x=nCenterX-120;
	m_ptDrawNiu[2].y=nCenterY+180;
	m_ptDrawNiu[3].x=nCenterX+50;
	m_ptDrawNiu[3].y=nCenterY+180;
	m_ptDrawNiu[4].x=nCenterX+220;
	m_ptDrawNiu[4].y=nCenterY+180;

	m_rcTian.left=nCenterX - 348;
	m_rcTian.top=nCenterY-88;
	m_rcTian.right=m_rcTian.left+169;
	m_rcTian.bottom=m_rcTian.top+123;
	
	m_rcDi.left=m_rcTian.right+5;
	m_rcDi.top=m_rcTian.top;
	m_rcDi.right=m_rcDi.left+169;
	m_rcDi.bottom=m_rcDi.top+123;

	m_rcXuan.left=m_rcDi.right+5;
	m_rcXuan.top=m_rcTian.top;
	m_rcXuan.right=m_rcXuan.left+169;
	m_rcXuan.bottom=m_rcXuan.top+123;

	m_rcHuang.left=m_rcXuan.right+5;
	m_rcHuang.top=m_rcTian.top;
	m_rcHuang.right=m_rcHuang.left+169;
	m_rcHuang.bottom=m_rcHuang.top+123;


	//��������
	int ExcursionY=10;
	m_PointJettonNumber[0].SetPoint((m_rcTian.right+m_rcTian.left)/2, (m_rcTian.bottom+m_rcTian.top)/2-ExcursionY);
	m_PointJettonNumber[1].SetPoint((m_rcDi.right+m_rcDi.left)/2, (m_rcDi.bottom+m_rcDi.top)/2-ExcursionY);
	m_PointJettonNumber[2].SetPoint((m_rcXuan.right+m_rcXuan.left)/2, (m_rcXuan.bottom+m_rcXuan.top)/2-ExcursionY);
	m_PointJettonNumber[3].SetPoint((m_rcHuang.right+m_rcHuang.left)/2, (m_rcHuang.bottom+m_rcHuang.top)/2-ExcursionY);
	
	//����λ��
	m_PointJetton[0].SetPoint(m_rcTian.left, m_rcTian.top);
	m_PointJetton[1].SetPoint(m_rcDi.left, m_rcDi.top);
	m_PointJetton[2].SetPoint(m_rcXuan.left, m_rcXuan.top);
	m_PointJetton[3].SetPoint(m_rcHuang.left, m_rcHuang.top);


	//�˿˿ؼ�
	m_CardControl[0].SetBenchmarkPos(CPoint(nWidth/2-40-50+30,nHeight/2-230),enXLeft,enYTop);
	m_CardControl[1].SetBenchmarkPos(CPoint(nWidth/2+25-350,nHeight/2+40),enXLeft,enYTop);
	m_CardControl[2].SetBenchmarkPos(CPoint(nWidth/2+25-200+30,nHeight/2+40),enXLeft,enYTop);
	m_CardControl[3].SetBenchmarkPos(CPoint(nWidth/2+25-50+50,nHeight/2+40),enXLeft,enYTop);
	m_CardControl[4].SetBenchmarkPos(CPoint(nWidth/2+25+100+70,nHeight/2+40),enXLeft,enYTop);

	m_CardControl[5].SetBenchmarkPos(CPoint(nWidth/2-45,nHeight/2-78),enXLeft,enYTop);

	//�ƶ��ؼ�
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	//�б�ؼ�
	DeferWindowPos(hDwp,m_ApplyUser,NULL,nWidth/2 -140,nHeight/2-330,320,83,uFlags);

	//���밴ť
	CRect rcJetton;
	m_btJetton100.GetWindowRect(&rcJetton);
	int nYPos = nHeight/2+220;
	int nXPos = nWidth/2+8+120;
	int nSpace = 5;

	DeferWindowPos(hDwp,m_btJetton100,NULL,nXPos,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton1000,NULL,nXPos + nSpace + rcJetton.Width(),nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton10000,NULL,nXPos + nSpace * 2 + rcJetton.Width() * 2,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton50000,NULL,nXPos + nSpace * 3 + rcJetton.Width() * 3,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton100000,NULL,nXPos ,nYPos+52,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton500000,NULL,nXPos + nSpace    + rcJetton.Width() ,nYPos+52,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton1000000,NULL,nXPos + nSpace*2  + rcJetton.Width()*2 ,nYPos+52,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton5000000,NULL,nXPos + nSpace*3 + rcJetton.Width()*3,nYPos+52,0,0,uFlags|SWP_NOSIZE);

//	DeferWindowPos(hDwp,m_btViewChart,NULL,nWidth/2-377,nHeight/2+180,0,0,uFlags|SWP_NOSIZE);

	//��ׯ��ť
	DeferWindowPos(hDwp,m_btApplyBanker,NULL,nWidth/2+240,nHeight/2-260,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btCancelBanker,NULL,nWidth/2+240,nHeight/2-260,0,0,uFlags|SWP_NOSIZE);

	DeferWindowPos(hDwp,m_btScoreMoveL,NULL,nWidth/2-203-100-17,nHeight/2+272-20,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btScoreMoveR,NULL,nWidth/2+302-200-16,nHeight/2+270-20,0,0,uFlags|SWP_NOSIZE);


//	DeferWindowPos(hDwp,m_btnShouDong,NULL,nWidth/2+240,nHeight/2-150,0,0,uFlags|SWP_NOSIZE);
//	DeferWindowPos(hDwp,m_btnZiDong,NULL,nWidth/2+240,nHeight/2-150-30,0,0,uFlags|SWP_NOSIZE);
	//�����ƶ�
	EndDeferWindowPos(hDwp);

	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageViewFill,enMode_Spread);
	DrawViewImage(pDC,m_ImageViewBack,enMode_Centent);

	//��ȡ״̬
	BYTE cbGameStatus=m_pGameClientDlg->GetGameStatus();

	//״̬��ʾ
	//CFont InfoFont;
	//InfoFont.CreateFont(-16,0,0,0,400,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("����"));
	//CFont * pOldFont=pDC->SelectObject(&InfoFont);
	//pDC->SetTextColor(RGB(255,234,0));
	//CRect rcDispatchCardTips(m_nWinPointsExcursionX, m_nWinPointsExcursionY+50, m_nWinPointsExcursionX + 220, m_nWinPointsExcursionY+115);
	//
	//if (m_pGameClientDlg->GetGameStatus()==GS_PLACE_JETTON)
	//	pDC->DrawText( TEXT("��Ϸ��ʼ�ˣ������·�"), rcDispatchCardTips, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_WORDBREAK );
	//else if (m_pGameClientDlg->GetGameStatus()==GS_GAME_END)
	//	pDC->DrawText( m_strDispatchCardTips, rcDispatchCardTips, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_WORDBREAK );

	//pDC->SelectObject(pOldFont);
	//InfoFont.DeleteObject();

	//ʱ����ʾ
	CImageHandle ImageHandleTimeFlag(&m_ImageTimeFlag);
	int nTimeFlagWidth = m_ImageTimeFlag.GetWidth()/3;
	int nFlagIndex=0;
	if (cbGameStatus==GS_FREE) nFlagIndex=0;
	else if (cbGameStatus==GS_PLACE_JETTON) nFlagIndex=1;
	else if (cbGameStatus==GS_GAME_END) nFlagIndex=2;
	m_ImageTimeFlag.BitBlt(pDC->GetSafeHdc(), nWidth/2-280, nHeight/2-130, nTimeFlagWidth, m_ImageTimeFlag.GetHeight(),
		nFlagIndex * nTimeFlagWidth, 0);


//--
	//�����ע
	pDC->SetTextColor(RGB(255,234,0));
	//LONG lLimitScore=GetMaxBankerScore();
	//DrawNumberStringWithSpace(pDC,lLimitScore,nWidth/2-300, nHeight/2 - 347);
	//lLimitScore=GetMaxPlayerScore();
	//DrawNumberStringWithSpace(pDC,lLimitScore,nWidth/2-300, nHeight/2 - 322);
	//lLimitScore=GetMaxTieScore();
	//DrawNumberStringWithSpace(pDC,lLimitScore,nWidth/2-300, nHeight/2 - 299);

	//pDC->FillSolidRect(m_rcTian.left,m_rcTian.top,m_rcTian.Width(),m_rcTian.Height(),RGB(255,0,255));
	//pDC->FillSolidRect(m_rcDi.left,m_rcDi.top,m_rcDi.Width(),m_rcDi.Height(),RGB(255,0,255));
	//pDC->FillSolidRect(m_rcXuan.left,m_rcXuan.top,m_rcXuan.Width(),m_rcXuan.Height(),RGB(255,0,255));
	//pDC->FillSolidRect(m_rcHuang.left,m_rcHuang.top,m_rcHuang.Width(),m_rcHuang.Height(),RGB(255,0,255));

	//ʤ���߿�
	FlashJettonAreaFrame(nWidth,nHeight,pDC);

	//������Դ
	CImageHandle HandleJettonView(&m_ImageJettonView);
	CSize SizeJettonItem(m_ImageJettonView.GetWidth()/JETTON_COUNT,m_ImageJettonView.GetHeight());

	//�滭����
	for (INT i=0;i<4;i++)
	{
		//��������
		LONG lScoreCount=0L;
		LONG lScoreJetton[JETTON_COUNT]={100L,500L,1000L,10000L,50000L,100000L,500000L,1000000L,5000000L};

		//�滭����
		for (INT_PTR j=0;j<m_JettonInfoArray[i].GetCount();j++)
		{
			//��ȡ��Ϣ
			tagJettonInfo * pJettonInfo=&m_JettonInfoArray[i][j];

			//�ۼ�����
			ASSERT(pJettonInfo->cbJettonIndex<JETTON_COUNT);
			lScoreCount+=lScoreJetton[pJettonInfo->cbJettonIndex];

			//�滭����
			m_ImageJettonView.AlphaDrawImage(pDC,pJettonInfo->nXPos+m_PointJetton[i].x,
				pJettonInfo->nYPos+m_PointJetton[i].y,SizeJettonItem.cx,SizeJettonItem.cy,
				pJettonInfo->cbJettonIndex*SizeJettonItem.cx,0,RGB(255,0,255));
		}

		//�滭����
		if (lScoreCount>0L)	DrawNumberString(pDC,lScoreCount,m_PointJettonNumber[i].x,m_PointJettonNumber[i].y);
	}
	//ׯ����Ϣ																											
	pDC->SetTextColor(RGB(255,234,0));

	//��ȡ���
	tagUserData const *pUserData = m_wBankerUser==INVALID_CHAIR ? NULL : GetUserInfo(m_wBankerUser);

	//λ����Ϣ
	CRect StrRect;
	StrRect.left = nWidth/2-85-210;
	StrRect.top = nHeight/2 - 330;
	StrRect.right = StrRect.left + 190;
	StrRect.bottom = StrRect.top + 15;

	//ׯ������
	pDC->DrawText(pUserData==NULL?(m_bEnableSysBanker?TEXT("ϵͳ��ׯ"):TEXT("������ׯ")):pUserData->szName, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_SINGLELINE );

	//ׯ���ܷ�
	CString strBankerTotalScore;
	StrRect.left = nWidth/2-85-210;
	StrRect.top = nHeight/2 - 306;
	StrRect.right = StrRect.left + 190;
	StrRect.bottom = StrRect.top + 15;
	DrawNumberStringWithSpace(pDC,pUserData==NULL?0:pUserData->lScore, StrRect);

	//ׯ�Ҿ���
	CString strBankerTime;
	strBankerTime.Format( "%d", m_wBankerTime );
	StrRect.left = nWidth/2-85-210;
	StrRect.top = nHeight/2 - 281;

	StrRect.right = StrRect.left + 190;
	StrRect.bottom = StrRect.top + 15;
	DrawNumberStringWithSpace(pDC,m_wBankerTime,StrRect);

	//ׯ�ҳɼ�
	CString strBankerScore;
	strBankerScore.Format( "%ld", m_lBankerWinScore);
	StrRect.left = nWidth/2-85-210;
	StrRect.top = nHeight/2 - 256;
	StrRect.right = StrRect.left + 190;
	StrRect.bottom = StrRect.top + 15;
	DrawNumberStringWithSpace(pDC,m_lBankerWinScore,StrRect);

	//�滭�û�
	if (m_wMeChairID!=INVALID_CHAIR)
	{
		const tagUserData *pMeUserData = GetUserInfo(m_wMeChairID);
		if ( pMeUserData != NULL )
		{
			//��Ϸ��Ϣ
			TCHAR szResultScore[16]=TEXT("");
			TCHAR szGameScore[16]=TEXT("");
			pDC->SetTextColor(RGB(255,255,255));
			LONG lMeJetton = m_lMeTianScore+m_lMeDiScore+m_lMeXuanScore+m_lMeHuangScore;
			//CRect rcAccount(CPoint(nWidth/2-297,nHeight/2),CPoint(nWidth/2-297+73,nHeight/2+10));
			//CRect rcGameScore(CPoint(nWidth/2-297,nHeight/2+294),CPoint(nWidth/2-297+73,nHeight/2+294+10));
			//CRect rcResultScore(CPoint(nWidth/2-297,nHeight/2+294+23),CPoint(nWidth/2-297+73,nHeight/2+294+10+23));
			CRect rcAccount(CPoint(nWidth/2+250,nHeight/2-327),CPoint(nWidth/2+250+73,nHeight/2-327+10));
			CRect rcGameScore(CPoint(nWidth/2+250,nHeight/2-304),CPoint(nWidth/2+250+73,nHeight/2-304+10));
			CRect rcResultScore(CPoint(nWidth/2+250,nHeight/2-281),CPoint(nWidth/2+250+73,nHeight/2-281+10));
			DrawNumberStringWithSpace(pDC,pMeUserData->lScore-lMeJetton,rcGameScore,DT_VCENTER|DT_SINGLELINE|DT_CENTER|DT_END_ELLIPSIS);
			DrawNumberStringWithSpace(pDC,m_lMeStatisticScore,rcResultScore,DT_VCENTER|DT_SINGLELINE|DT_CENTER|DT_END_ELLIPSIS);
			pDC->DrawText(pMeUserData->szName,lstrlen(pMeUserData->szName),rcAccount,DT_VCENTER|DT_SINGLELINE|DT_CENTER|DT_END_ELLIPSIS);
		}
	}

	//�л�ׯ��
	if ( m_bShowChangeBanker )
	{
		int	nXPos = nWidth / 2 - 130;
		int	nYPos = nHeight / 2 - 160;

		//������ׯ
		if ( m_wMeChairID == m_wBankerUser )
		{
			CImageHandle ImageHandleBanker(&m_ImageMeBanker);
			m_ImageMeBanker.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);
		}
		else if ( m_wBankerUser != INVALID_CHAIR )
		{
			CImageHandle ImageHandleBanker(&m_ImageChangeBanker);
			m_ImageChangeBanker.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);
		}
		else
		{
			CImageHandle ImageHandleBanker(&m_ImageNoBanker);
			m_ImageNoBanker.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);
		}
	}


	//�ҵ���ע
	DrawMeJettonNumber(pDC);

	//�滭ʱ��
	if (m_wMeChairID!=INVALID_CHAIR)
	{
		WORD wUserTimer=GetUserTimer(m_wMeChairID);
		if (wUserTimer!=0) DrawUserTimer(pDC,nWidth/2-255,nHeight/2-160,wUserTimer);
	}

	//����״̬
	if (cbGameStatus==GS_GAME_END)
	{
		//CImageHandle ImageHandleGameEndFrame(&m_ImageGameEndFrame);
		//int	nXPos = nWidth / 2 - 129;
		//int	nYPos = nHeight / 2 - 208;

		//CRect rcAlpha(nXPos, nYPos, nXPos+ m_ImageGameEndFrame.GetWidth(), nYPos+m_ImageGameEndFrame.GetHeight());
		//DrawAlphaRect(pDC, &rcAlpha, RGB(74,70,73), 0.8f);

		//�滭�߿�
		//m_ImageGameEndFrame.AlphaDrawImage(pDC, nXPos, nYPos, RGB(255,0,255));

		//�滭�˿�
		
		m_CardControl[INDEX_BANKER].DrawCardControl(pDC);
		m_CardControl[INDEX_PLAYER1].DrawCardControl(pDC);
		m_CardControl[INDEX_PLAYER2].DrawCardControl(pDC);
		m_CardControl[INDEX_PLAYER3].DrawCardControl(pDC);
		m_CardControl[INDEX_PLAYER4].DrawCardControl(pDC);
		m_CardControl[5].DrawCardControl(pDC);

		//�����˿�
/*		if (m_ptDispatchCard.x!=0 && m_ptDispatchCard.y!=0) 
		{
			m_DispatchCard.SetBenchmarkPos(m_ptDispatchCard.x,m_ptDispatchCard.y,enXCenter,enYCenter);
			m_DispatchCard.DrawCardControl(pDC);
		}	*/	
	}

	//ʤ����־
	DrawWinFlags(pDC);

	if(m_bShowResult)
	{
		//int	nXPos = nWidth / 2 - 129;
		//int	nYPos = nHeight / 2 - 208;	
		CImageHandle ImageHandleNiu(&m_ImageNiu);

		int nType=0;
		for(int i=0;i<5;i++)
		{
			if(m_cbNiuType[i]>11)
			{
				nType=11;
			}
			else
			{
				nType=m_cbNiuType[i];
			}
			m_ImageNiu.AlphaDrawImage(pDC, m_ptDrawNiu[i].x, m_ptDrawNiu[i].y-50, m_ImageNiu.GetWidth()/12, m_ImageNiu.GetHeight(),
				m_ImageNiu.GetWidth()/12*nType, 0, RGB(255, 0, 255));
		}
	}
	//��ʾ���
	ShowGameResult(pDC, nWidth, nHeight);	

	//�˿˵���
	//if (m_cbSendCount[INDEX_PLAYER1]+m_cbSendCount[INDEX_BANKER]!=0)
	//{
	//	//��������
	//	CImageHandle ImageHandleGamePoint(&m_ImageGamePoint);
	//	BYTE cbPlayerPoint, cbBankerPoint;
	//	int	nXPos = nWidth / 2 - 129;
	//	int	nYPos = nHeight / 2 - 208;		

	//	//�滭����
	//	int nPointWidth = m_ImageGamePoint.GetWidth() / 10;
	//	if (m_cbSendCount[INDEX_PLAYER1]!=0)
	//	{
	//		//�������
	//		cbPlayerPoint = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER1], m_cbSendCount[INDEX_PLAYER1]);
	//		m_ImageGamePoint.AlphaDrawImage(pDC, nXPos + 28, nYPos + 10, nPointWidth, m_ImageGamePoint.GetHeight(),
	//			cbPlayerPoint * nPointWidth, 0, RGB(255, 0, 255));
	//	}
	//	if (m_cbSendCount[INDEX_BANKER]!=0)
	//	{
	//		//�������
	//		cbBankerPoint = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER], m_cbSendCount[INDEX_BANKER]);
	//		m_ImageGamePoint.AlphaDrawImage(pDC, nXPos + 28 + 136, nYPos + 10, nPointWidth, m_ImageGamePoint.GetHeight(),
	//			cbBankerPoint * nPointWidth, 0, RGB(255, 0, 255));
	//	}
	//}

	//��ըЧ��
	DrawBombEffect(pDC);
	
	return;
}

//������Ϣ
void CGameClientView::SetMeMaxScore(LONG lMeMaxScore)
{
	if (m_lMeMaxScore!=lMeMaxScore)
	{
		//���ñ���
		m_lMeMaxScore=lMeMaxScore;
	}

	return;
}

//�����ע
void CGameClientView::SetAreaLimitScore(LONG lAreaLimitScore)
{
	if ( m_lAreaLimitScore!= lAreaLimitScore )
	{
		//���ñ���
		m_lAreaLimitScore=lAreaLimitScore;
	}
}

//���ó���
void CGameClientView::SetCurrentJetton(LONG lCurrentJetton)
{
	//���ñ���
	ASSERT(lCurrentJetton>=0L);
	m_lCurrentJetton=lCurrentJetton;

	return;
}

//��ʷ��¼
//void CGameClientView::SetGameHistory(enOperateResult OperateResult, BYTE cbPlayerCount, BYTE cbBankerCount, BYTE cbKingWinner, bool bPlayerTwoPair, bool bBankerTwoPair)
void CGameClientView::SetGameHistory(enOperateResult OperateResult,BYTE cbWinner)
{
	//��������
	tagClientGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.cbWinner=cbWinner;
	if(OperateResult==enOperateResult_Pleace)
	{
		if(m_lMeTianScore)
		{
			GameRecord.cbMePlaceRegion |=ID_TIAN_MARK;
		}
		if(m_lMeDiScore)
		{
			GameRecord.cbMePlaceRegion |=ID_DI_MARK;
		}
		if(m_lMeXuanScore)
		{
			GameRecord.cbMePlaceRegion |= ID_XUAN_MARK;
		}
		if(m_lMeHuangScore)
		{
			GameRecord.cbMePlaceRegion |=ID_HUANG_MARK;
		}
	}

	//GameRecord.enOperateFlags = OperateResult;
	//GameRecord.cbPlayerCount = cbPlayerCount;
	//GameRecord.cbBankerCount = cbBankerCount;
	//GameRecord.bPlayerTwoPair=bPlayerTwoPair;
	//GameRecord.bBankerTwoPair=bBankerTwoPair;
	//GameRecord.cbKingWinner=cbKingWinner;

	//�ƶ��±�
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst )
	{
		m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
		if ( m_nScoreHead < m_nRecordFirst ) m_nScoreHead = m_nRecordFirst;
	}

	int nHistoryCount = (m_nRecordLast - m_nRecordFirst + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
	if ( 14 < nHistoryCount ) m_btScoreMoveR.EnableWindow(TRUE);

	//�Ƶ����¼�¼
	if ( 14 < nHistoryCount )
	{
		m_nScoreHead = (m_nRecordLast - 14 + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
		m_btScoreMoveL.EnableWindow(TRUE);
		m_btScoreMoveR.EnableWindow(FALSE);
	}


	return;
}

//�������
void CGameClientView::CleanUserJetton()
{
	//��������
	for (BYTE i=0;i<CountArray(m_JettonInfoArray);i++)
	{
		m_JettonInfoArray[i].RemoveAll();
	}

	//��ע��Ϣ
	m_lMeTianScore=0L;
	m_lMeDiScore=0L;
	m_lMeXuanScore=0L;
	m_lMeHuangScore=0L;
	
	//ȫ����ע
	m_lAllTianScore=0L;
	m_lAllDiScore=0L;
	m_lAllXuanScore=0L;
	m_lAllHuangScore=0L;
	
	m_strDispatchCardTips=TEXT("");
	
	//���½���
	UpdateGameView(NULL);

	return;
}

//������ע
void CGameClientView::SetMePlaceJetton(BYTE cbViewIndex, LONG lJettonCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=ID_HUANG_MARK);
	if (cbViewIndex>ID_HUANG_MARK) return;

	if (cbViewIndex==ID_TIAN_MARK) m_lMeTianScore=lJettonCount;
	else if (cbViewIndex==ID_DI_MARK) m_lMeDiScore=lJettonCount;
	else if (cbViewIndex==ID_XUAN_MARK) m_lMeXuanScore=lJettonCount;
	else if (cbViewIndex==ID_HUANG_MARK) m_lMeHuangScore=lJettonCount;
	else {ASSERT(FALSE);return;}

	//���½���
	UpdateGameView(NULL);
}

//�����˿�
void CGameClientView::SetCardInfo(bool bHaveCard,BYTE cbTableCardArray[6][3])
{

	if (bHaveCard)
	{//12 3e 6 9

		CopyMemory(m_cbTableCardArray,cbTableCardArray,sizeof(m_cbTableCardArray));

		//��ʼ����
		DispatchCard();
	}
	else
	{
		ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));
	}
}

//���ó���
void CGameClientView::PlaceUserJetton(BYTE cbViewIndex, LONG lScoreCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=ID_HUANG_MARK);
	if (cbViewIndex>ID_HUANG_MARK) return;

	//����ը��
	//if (lScoreCount==5000000L) SetBombEffect(true,cbViewIndex);

	//��������
	bool bPlaceJetton=false;
	LONG lScoreIndex[JETTON_COUNT]={100L,500L,1000L,10000L,50000L,100000L,500000L,1000000L,5000000L};

	//�߿���
	int nFrameWidth=0, nFrameHeight=0;
	int nBorderWidth=6;

	switch (cbViewIndex)
	{
	case ID_TIAN_MARK:
		{ 
			m_lAllTianScore += lScoreCount;
			nFrameWidth = m_rcTian.right-m_rcTian.left;
			nFrameHeight = m_rcTian.bottom-m_rcTian.top;
			break;
		}
	case ID_DI_MARK:
		{
			m_lAllDiScore += lScoreCount;
			nFrameWidth = m_rcDi.right-m_rcDi.left;
			nFrameHeight = m_rcDi.bottom-m_rcDi.top;
			break;
		}
	case ID_XUAN_MARK:
		{ 
			m_lAllXuanScore += lScoreCount;
			nFrameWidth = m_rcXuan.right-m_rcXuan.left;
			nFrameHeight = m_rcXuan.bottom-m_rcXuan.top;
			break;
		}
	case ID_HUANG_MARK:
		{ 
			m_lAllHuangScore += lScoreCount;
			nFrameWidth = m_rcHuang.right-m_rcHuang.left;
			nFrameHeight = m_rcHuang.bottom-m_rcHuang.top;
			break;
		}
	default:
		{
			ASSERT(FALSE);
			return;
		}
	}

	nFrameWidth += nBorderWidth;
	nFrameHeight += nBorderWidth;

	//���ӳ���
	for (BYTE i=0;i<CountArray(lScoreIndex);i++)
	{
		//������Ŀ
		BYTE cbScoreIndex=JETTON_COUNT-i-1;
		LONG lCellCount=lScoreCount/lScoreIndex[cbScoreIndex];

		//�������
		if (lCellCount==0L) continue;

		//�������
		for (LONG j=0;j<lCellCount;j++)
		{
			//�������
			tagJettonInfo JettonInfo;
			int nJettonSize=68;
			JettonInfo.cbJettonIndex=cbScoreIndex;
			JettonInfo.nXPos=rand()%(nFrameWidth-nJettonSize);
			JettonInfo.nYPos=rand()%(nFrameHeight-nJettonSize);

			//��������
			bPlaceJetton=true;

			int nIndex=0;
			switch(cbViewIndex)
			{
			case ID_TIAN_MARK:
				nIndex=0;
				break;
			case ID_DI_MARK:
				nIndex=1;
				break;
			case ID_XUAN_MARK:
				nIndex=2;
				break;
			case ID_HUANG_MARK:
				nIndex=3;
				break;
			}


			m_JettonInfoArray[nIndex].Add(JettonInfo);
		}

		//������Ŀ
		lScoreCount-=lCellCount*lScoreIndex[cbScoreIndex];
	}

	//���½���
	if (bPlaceJetton==true) UpdateGameView(NULL);

	return;
}

//���ֳɼ�
void CGameClientView::SetCurGameScore(LONG lMeCurGameScore, LONG lMeCurGameReturnScore, LONG lBankerCurGameScore, LONG lGameRevenue)
{
	m_lMeCurGameScore=lMeCurGameScore;			
	m_lMeCurGameReturnScore=lMeCurGameReturnScore;			
	m_lBankerCurGameScore=lBankerCurGameScore;			
	m_lGameRevenue=lGameRevenue;					
}

//����ʤ��
void CGameClientView::SetWinnerSide(BYTE cbWinnerSide)
{
	//���ñ���
	m_cbWinnerSide=cbWinnerSide;
	m_cbAreaFlash=cbWinnerSide;

	//����ʱ��
	if (cbWinnerSide!=0xFF)
	{
		SetTimer(IDI_FLASH_WINNER,500,NULL);
	}
	else KillTimer(IDI_FLASH_WINNER);

	//���½���
	UpdateGameView(NULL);

	return;
}

//��ȡ����
BYTE CGameClientView::GetJettonArea(CPoint MousePoint)
{
	if (m_rcTian.PtInRect(MousePoint)) return ID_TIAN_MARK;
	if (m_rcDi.PtInRect(MousePoint)) return ID_DI_MARK;
	if (m_rcXuan.PtInRect(MousePoint)) return ID_XUAN_MARK;
	if (m_rcHuang.PtInRect(MousePoint)) return ID_HUANG_MARK;	
	return 0xFF;
}

//�滭����
void CGameClientView::DrawNumberString(CDC * pDC, LONG lNumber, INT nXPos, INT nYPos, bool bMeScore)
{
	//������Դ
	CImageHandle HandleScoreNumber(&m_ImageScoreNumber);
	CImageHandle HandleMeScoreNumber(&m_ImageMeScoreNumber);
	CSize SizeScoreNumber(m_ImageScoreNumber.GetWidth()/10,m_ImageScoreNumber.GetHeight());

	if ( bMeScore ) SizeScoreNumber.SetSize(m_ImageMeScoreNumber.GetWidth()/10, m_ImageMeScoreNumber.GetHeight());

	//������Ŀ
	LONG lNumberCount=0;
	LONGLONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	//λ�ö���
	INT nYDrawPos=nYPos-SizeScoreNumber.cy/2;
	INT nXDrawPos=nXPos+lNumberCount*SizeScoreNumber.cx/2-SizeScoreNumber.cx;

	//�滭����
	for (LONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		LONG lCellNumber=(LONG)(lNumber%10);
		if ( bMeScore )
		{
			m_ImageMeScoreNumber.AlphaDrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
				lCellNumber*SizeScoreNumber.cx,0,RGB(255,0,255));
		}
		else
		{
			m_ImageScoreNumber.AlphaDrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
				lCellNumber*SizeScoreNumber.cx,0,RGB(255,0,255));
		}

		//���ñ���
		lNumber/=10;
		nXDrawPos-=SizeScoreNumber.cx;
	};

	return;
}

//�滭����
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, LONG lNumber, INT nXPos, INT nYPos)
{
	CString strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;
	if (lNumber==0) strNumber=TEXT("0");
	int nNumberCount=0;
	LONG lTmpNumber=lNumber;
	if (lNumber<0) lNumber=-lNumber;
	while (lNumber>0)
	{
		strTmpNumber1.Format(TEXT("%ld"),lNumber%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if (nNumberCount==4)
		{
			strTmpNumber2 += (TEXT(" ") +strNumber);
			strNumber=strTmpNumber2;
			nNumberCount=0;
			strTmpNumber2=TEXT("");
		}
		lNumber/=10;
	}

	if (strTmpNumber2.IsEmpty()==FALSE)
	{
		strTmpNumber2 += (TEXT(" ") +strNumber);
		strNumber=strTmpNumber2;
	}

	if (lTmpNumber<0) strNumber=TEXT("-")+strNumber;

	//�������
	pDC->TextOut(nXPos,nYPos,strNumber);
}

//�滭����
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, LONG lNumber, CRect rcRect, INT nFormat)
{
	CString strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;
	if (lNumber==0) strNumber=TEXT("0");
	int nNumberCount=0;
	LONG lTmpNumber=lNumber;
	if (lNumber<0) lNumber=-lNumber;
	while (lNumber>0)
	{
		strTmpNumber1.Format(TEXT("%ld"),lNumber%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if (nNumberCount==4)
		{
			strTmpNumber2 += (TEXT(" ") +strNumber);
			strNumber=strTmpNumber2;
			nNumberCount=0;
			strTmpNumber2=TEXT("");
		}
		lNumber/=10;
	}

	if (strTmpNumber2.IsEmpty()==FALSE)
	{
		strTmpNumber2 += (TEXT(" ") +strNumber);
		strNumber=strTmpNumber2;
	}

	if (lTmpNumber<0) strNumber=TEXT("-")+strNumber;

	//�������
	if (nFormat==-1) pDC->DrawText(strNumber,rcRect,DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);
	else pDC->DrawText(strNumber,rcRect,nFormat);
}

//���밴ť
void CGameClientView::OnJettonButton100()
{
	//���ñ���
	m_lCurrentJetton=100L;

	return;
}

//���밴ť
void CGameClientView::OnJettonButton1000()
{
	//���ñ���
	m_lCurrentJetton=1000L;

	return;
}

//���밴ť
void CGameClientView::OnJettonButton10000()
{
	//���ñ���
	m_lCurrentJetton=10000L;

	return;
}

//���밴ť
void CGameClientView::OnJettonButton100000()
{
	//���ñ���
	m_lCurrentJetton=100000L;

	return;
}

//���밴ť
void CGameClientView::OnJettonButton1000000()
{
	//���ñ���
	m_lCurrentJetton=1000000L;

	return;
}
//���밴ť
void CGameClientView::OnJettonButton5000000()
{
	//���ñ���
	m_lCurrentJetton=5000000L;

	return;
}
void CGameClientView::OnJettonButton50000()
{
	//���ñ���
	m_lCurrentJetton=50000L;

	return;
}
void CGameClientView::OnJettonButton500000()
{
	//���ñ���
	m_lCurrentJetton=500000L;

	return;
}
//��ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{
	//����ʤ��
	if (nIDEvent==IDI_FLASH_WINNER)
	{
		//���ñ���
		if (m_cbAreaFlash!=m_cbWinnerSide)
		{
			m_cbAreaFlash=m_cbWinnerSide;
		}
		else 
		{
			m_cbAreaFlash=0xFF;
		}

		//���½���
		UpdateGameView(NULL);

		return;
	}

	//�ֻ�ׯ��
	if ( nIDEvent == IDI_SHOW_CHANGE_BANKER )
	{
		ShowChangeBanker( false );

		return;
	}

	else if (nIDEvent==IDI_DISPATCH_CARD)
	{
/*		bool bPlayerCard=false;
		if ((m_cbSendCount[0]+m_cbSendCount[1]+m_cbSendCount[2]+m_cbSendCount[3]+m_cbSendCount[4])<25)
		{
			//��������
			WORD wIndex=(m_cbSendCount[0]+m_cbSendCount[1]+m_cbSendCount[2]+m_cbSendCount[3]+m_cbSendCount[4])%5;
			if (wIndex==0) bPlayerCard=true;
		}
		else if (m_cbSendCount[INDEX_PLAYER1]<5) bPlayerCard=true;

		//�����ж�
		CRect rcView;
		GetClientRect(&rcView);
		int nEndPosX = 0;
		if (bPlayerCard) nEndPosX=rcView.Width()/2;
		else nEndPosX=rcView.Width()/2+110;
		int nEndPosY = rcView.Height()/2-185;
		if ( (m_ptDispatchCard.x > nEndPosX || m_ptDispatchCard.y < nEndPosY) &&
			m_ptDispatchCard.x != 0 && m_ptDispatchCard.y != 0 )
		{
			//�޸�λ��
			if (m_ptDispatchCard.y < nEndPosY) m_ptDispatchCard.y += 73; 
			else m_ptDispatchCard.x -= 115;

			//���½���
			UpdateGameView(NULL);

			return;
		}

		//ͣ���˿�
		static BYTE cbStopCount=0;
		if (cbStopCount<6)
		{			
			cbStopCount++;
			return;
		}
		else 
		{
			cbStopCount=0;			
		}

		if ( m_cbSendCount[INDEX_PLAYER1]+m_cbSendCount[INDEX_BANKER]<5 + 5-1 ) 
		{
			m_ptDispatchCard.x = rcView.Width()/2+220;
			m_ptDispatchCard.y = rcView.Height()/2-200;	
		}
		else
		{
			m_DispatchCard.SetCardData(NULL,0);
		}
*/
		//��������
		//m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("DISPATCH_CARD"));
		//::OutputDebugString("Is Anmi ing");
		if(m_cbWaitPre<4)
		{
			//				m_GameClientView.m_CardControl[4].SetCardData(m_cbTableCardArray[4],1);
			m_CardControl[5].SetCardData(m_cbTableCardArray[5],1);
			if(!m_cbWaitPre)m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("DISPATCH_CARD"));
			UpdateGameView(NULL);
			m_cbWaitPre++;
			return;
		}
		int logicvalue(0);
		if(m_cbTableCardArray[5][0]==0x4E || m_cbTableCardArray[5][0]==0x4F)
		{
			logicvalue=0;
		}
		else
		{
			logicvalue=m_GameLogic.GetCardValue(m_cbTableCardArray[5][0])-1;
		}
		m_CardControl[5].SetCardData(NULL,0);
		


		//�����˿�
		if ((m_cbSendCount[0]+m_cbSendCount[1]+m_cbSendCount[2]+m_cbSendCount[3]+m_cbSendCount[4])<15)
		{
			//��������
			WORD wIndex=(m_cbSendCount[0]+m_cbSendCount[1]+m_cbSendCount[2]+m_cbSendCount[3]+m_cbSendCount[4]+logicvalue)%5;

			//�����˿�
			m_cbSendCount[wIndex]++;
			m_CardControl[wIndex].SetCardData(m_cbTableCardArray[wIndex],m_cbSendCount[wIndex]);

			//������ʾ
			SetDispatchCardTips();

			//���½���
			UpdateGameView(NULL);
			m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("DISPATCH_CARD"));
			return;
		}

		if(!m_bCanGo)
		{
			if(!m_bSet)
			{
				m_bSet=true;
				SetTimer(123456,50,NULL);
			}
			return;
		}


		
		////�����˿�
		//if (m_cbSendCount[INDEX_PLAYER]<5)
		//{
		//	//�����˿�
		//	m_cbSendCount[INDEX_PLAYER]++;
		//	m_CardControl[INDEX_PLAYER].SetCardData(m_cbTableCardArray[INDEX_PLAYER],m_cbSendCount[INDEX_PLAYER]);

		//	//������ʾ
		//	SetDispatchCardTips();

		//	//���½���
		//	UpdateGameView(NULL);
		//	return;
		//}

		////ׯ���˿�
		//if (m_cbSendCount[INDEX_BANKER]<5)
		//{
		//	//�����˿�
		//	m_cbSendCount[INDEX_BANKER]++;
		//	m_CardControl[INDEX_BANKER].SetCardData(m_cbTableCardArray[INDEX_BANKER],m_cbSendCount[INDEX_BANKER]);

		//	//������ʾ
		//	SetDispatchCardTips();

		//	return;
		//}

		m_ptDispatchCard.x = 0;
		m_ptDispatchCard.y = 0;

		if(!m_bShowResult)
			m_bShowResult=true;

        //��ɷ���
		FinishDispatchCard();

		//���½���
		UpdateGameView(NULL);

		return;
	}
	else if(nIDEvent == 123456)
	{

		bool flag=false;
		/*for(int i=0;i<5;i++)
		{
			if(m_bAutoModel)
			{int nTemp=(i+1)%5;
				if(!m_CardControl[nTemp].TuiPai())
				{
					UpdateGameView(NULL);
					return;
				}
			}
			else
			{
				if(!m_CardControl[i].ShouTuiPai())
				{
					flag=false;
				}
			}

		}*/

		if(!m_bAutoModel && !flag)
		{
			if (m_wMeChairID!=INVALID_CHAIR)
			{
				WORD wUserTimer=GetUserTimer(m_wMeChairID);
				if(wUserTimer>7)return;
			}
			for(BYTE i=0;i<5;i++)
			{
				m_CardControl[i].ReSet();
			}
		}
		UpdateGameView(NULL);
		m_bCanGo=true;
		KillTimer(123456);
	}
	else if(nIDEvent==1111)
	{
		UpdateGameView(NULL);
	}
/*
	//��ը����
	if (nIDEvent<=IDI_BOMB_EFFECT+ID_BANKER_TWO_PAIR && IDI_BOMB_EFFECT+ID_XIAN_JIA<=nIDEvent)
	{
		WORD wIndex=nIDEvent-IDI_BOMB_EFFECT;
		//ֹͣ�ж�
		if (m_bBombEffect[wIndex]==false)
		{
			KillTimer(nIDEvent);
			return;
		}

		//���ñ���
		if ((m_cbBombFrameIndex[wIndex]+1)>=BOMB_EFFECT_COUNT)
		{
			//ɾ��ʱ��
			KillTimer(nIDEvent);

			//���ñ���
			m_bBombEffect[wIndex]=false;
			m_cbBombFrameIndex[wIndex]=0;
		}
		else m_cbBombFrameIndex[wIndex]++;

		//���½���
		UpdateGameView(NULL);

		return;
	}
*/
	__super::OnTimer(nIDEvent);
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	if(!m_bAutoModel)
	{

		for(int i=0;i<5;i++)
		{
			if(m_CardControl[i].PtInSecondCardRect(Point))
			{
				m_CardControl[i].OnLButtonDown(Point);
				
			}
		}
		SetTimer(1111,50,NULL);
		SetCapture();
	}

	if (m_lCurrentJetton!=0L)
	{
		BYTE cbJettonArea=GetJettonArea(Point);

		if (m_wMeChairID!=INVALID_CHAIR)
		{
			const tagUserData *pMeUserData = GetUserInfo(m_wMeChairID);
			if(pMeUserData->lScore<m_lCurrentJetton*10)
				return;
		}

		//��С�ж�
		switch( cbJettonArea )
		{
		case ID_TIAN_MARK:
			{
				LONG lMaxPlayerScore = GetMaxTianScore();
				if ( lMaxPlayerScore < m_lCurrentJetton )
					return ;
				break;
			}
		case ID_DI_MARK:
			{
				LONG lMaxTieScore  = GetMaxDiScore();
				if ( lMaxTieScore < m_lCurrentJetton )
					return ;
				break;
			}
		case ID_XUAN_MARK:
			{
				LONG lMaxBankerScore = GetMaxXuanScore();
				if ( lMaxBankerScore < m_lCurrentJetton )
					return ;
				break;
			}
		case ID_HUANG_MARK:
			{
				LONG lMaxTieKingScore= GetMaxHuangScore();
				if ( lMaxTieKingScore < m_lCurrentJetton )
					return ;
				break;
			}

		}
		if (cbJettonArea!=0xFF) AfxGetMainWnd()->SendMessage(IDM_PLACE_JETTON,cbJettonArea,m_lCurrentJetton);
	}

	__super::OnLButtonDown(nFlags,Point);
}

//�����Ϣ
void CGameClientView::OnRButtonDown(UINT nFlags, CPoint Point)
{
	//���ñ���
	m_lCurrentJetton=0L;

	if (m_pGameClientDlg->GetGameStatus()!=GS_GAME_END && m_cbAreaFlash!=0xFF)
	{
		m_cbAreaFlash=0xFF;
		UpdateGameView(NULL);
	}

	__super::OnLButtonDown(nFlags,Point);
}

//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	if (m_lCurrentJetton!=0L)
	{
		//��ȡ����
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);
		BYTE cbJettonArea=GetJettonArea(MousePoint);

		//���ñ���
		if ( m_cbAreaFlash!= cbJettonArea )
		{
			m_cbAreaFlash = cbJettonArea;
			UpdateGameView(NULL);
		}

		//�����ж�
		if (cbJettonArea==0xFF)
		{
			SetCursor(LoadCursor(NULL,IDC_NO));
			return TRUE;
		}

		//��С�ж�
		switch( cbJettonArea )
		{
		case ID_TIAN_MARK:
			{
				LONG lMaxTianScore = GetMaxTianScore();
				if ( lMaxTianScore < m_lCurrentJetton )
				{
					SetCursor(LoadCursor(NULL,IDC_NO));
					return TRUE;
				}
				break;
			}
		case ID_DI_MARK:
			{
				LONG lMaxDiScore  = GetMaxDiScore();
				if ( lMaxDiScore < m_lCurrentJetton )
				{
					SetCursor(LoadCursor(NULL,IDC_NO));
					return TRUE;
				}
				break;
			}
		case ID_XUAN_MARK:
			{
				LONG lMaxXuanScore = GetMaxXuanScore();
				if ( lMaxXuanScore < m_lCurrentJetton  )
				{
					SetCursor(LoadCursor(NULL,IDC_NO));
					return TRUE;
				};
				break;
			}
		case ID_HUANG_MARK:
			{
				LONG lMaxHuangScore= GetMaxHuangScore();
				if ( lMaxHuangScore < m_lCurrentJetton )
				{
					SetCursor(LoadCursor(NULL,IDC_NO));
					return TRUE;
				}
				break;
			}
		}


		//���ù��
		switch (m_lCurrentJetton)
		{
		case 100:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_100)));
				return TRUE;
			}
		case 1000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1000)));
				return TRUE;
			}
		case 10000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_10000)));
				return TRUE;
			}
		case 50000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_50000)));
				return TRUE;
			}
		case 100000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_100000)));
				return TRUE;
			}
		case 500000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_500000)));
				return TRUE;
			}
		case 1000000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1000000)));
				return TRUE;
			}
		case 5000000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_5000000)));
				return TRUE;
			}		
		}
	}

	return __super::OnSetCursor(pWnd, nHitTest, uMessage);
}

//�ֻ�ׯ��
void CGameClientView::ShowChangeBanker( bool bChangeBanker )
{
	//�ֻ�ׯ��
	if ( bChangeBanker )
	{
		SetTimer( IDI_SHOW_CHANGE_BANKER, 3000, NULL );
		m_bShowChangeBanker = true;
	}
	else
	{
		KillTimer( IDI_SHOW_CHANGE_BANKER );
		m_bShowChangeBanker = false ;
	}

	//���½���
	UpdateGameView(NULL);
}

//��ׯ��ť
void CGameClientView::OnApplyBanker()
{
	AfxGetMainWnd()->SendMessage(IDM_APPLY_BANKER,1,0);

}

//��ׯ��ť
void CGameClientView::OnCancelBanker()
{
	AfxGetMainWnd()->SendMessage(IDM_APPLY_BANKER,0,0);
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
//ׯ����Ϣ
void CGameClientView::SetBankerInfo(WORD wBankerUser, LONG lBankerScore) 
{
	//�л��ж�
	if (m_wBankerUser!=wBankerUser)
	{
		m_wBankerUser=wBankerUser;
		m_wBankerTime=0L;
		m_lBankerWinScore=0L;	
		m_lTmpBankerWinScore=0L;
	}
	m_lBankerScore=lBankerScore;
}

//�滭��ʶ
void CGameClientView::DrawWinFlags(CDC * pDC)
{
	CImageHandle ImageHandleWinFlags(&m_ImageWinFlags);
	int nIndex = m_nScoreHead;
	COLORREF clrOld ;
	clrOld = pDC->SetTextColor(RGB(255,234,0));
	CString strPoint;
	int nDrawCount = 0;
	while ( nIndex != m_nRecordLast && ( m_nRecordLast!=m_nRecordFirst ) && nDrawCount < 14 )
	{
		tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[nIndex];

		int nPosX = m_nWinFlagsExcursionX + ((nIndex - m_nScoreHead + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY) * 27;
		int nPosY=0;
		int nFlagsIndex = 0;
		BYTE RegionArea[4]={ID_TIAN_MARK,ID_DI_MARK,ID_XUAN_MARK,ID_HUANG_MARK};
		for(BYTE i=0;i<4;i++)
		{
			nPosY=m_nWinFlagsExcursionY+27*i;
			if(ClientGameRecord.cbWinner & RegionArea[i])
			{
				if(ClientGameRecord.cbMePlaceRegion & RegionArea[i])
				{
					nFlagsIndex=1;
				}
				else
				{
					nFlagsIndex=0;
				}
			}
			else
			{
				if(ClientGameRecord.cbMePlaceRegion & RegionArea[i])
				{
					nFlagsIndex=3;
				}
				else
				{
					nFlagsIndex=2;
				}
			}
			m_ImageWinFlags.AlphaDrawImage( pDC, nPosX, nPosY, m_ImageWinFlags.GetWidth()/4 , 
				m_ImageWinFlags.GetHeight(),m_ImageWinFlags.GetWidth()/4 * nFlagsIndex, 0, RGB(255, 0, 255) );
		}

		//int nYPos = 0;
		//if ( ClientGameRecord.cbBankerCount > ClientGameRecord.cbPlayerCount ) nYPos = m_nWinFlagsExcursionY ;
		//else if (ClientGameRecord.cbBankerCount < ClientGameRecord.cbPlayerCount ) nYPos = m_nWinFlagsExcursionY + 31;
		//else nYPos = m_nWinFlagsExcursionY + 2 * 31;

		//int nXPos = m_nWinFlagsExcursionX + ((nIndex - m_nScoreHead + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY) * 37;

		//int nFlagsIndex = 0;
		//if ( ClientGameRecord.enOperateFlags == enOperateResult_NULL ) nFlagsIndex = 0;
		//else if ( ClientGameRecord.enOperateFlags == enOperateResult_Win) nFlagsIndex = 1;
		//else if ( ClientGameRecord.enOperateFlags == enOperateResult_Lost) nFlagsIndex = 2;

		//m_ImageWinFlags.AlphaDrawImage( pDC, nXPos, nYPos, m_ImageWinFlags.GetWidth()/3 , 
		//	m_ImageWinFlags.GetHeight(),m_ImageWinFlags.GetWidth()/3 * nFlagsIndex, 0, RGB(255, 0, 255) );

		//strPoint.Format(TEXT("%d"), ClientGameRecord.cbBankerCount);
		//pDC->TextOut(m_nWinPointsExcursionX + nDrawCount * 17, m_nWinPointsExcursionY, strPoint);

		//strPoint.Format(TEXT("%d"), ClientGameRecord.cbPlayerCount);
		//pDC->TextOut(m_nWinPointsExcursionX + nDrawCount * 17, m_nWinPointsExcursionY + 21, strPoint);

		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
		nDrawCount++;
	}
	pDC->SetTextColor(clrOld);
/*
	//ͳ�Ƹ���
	nIndex = m_nRecordFirst;
	int nPlayerCount = 0, nBankerCount = 0, nTieCount = 0;
	while ( nIndex != m_nRecordLast && ( m_nRecordLast!=m_nRecordFirst ))
	{
		tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[nIndex];
		if ( ClientGameRecord.cbBankerCount < ClientGameRecord.cbPlayerCount ) nPlayerCount++;
		else if ( ClientGameRecord.cbBankerCount == ClientGameRecord.cbPlayerCount ) nTieCount++;
		else nBankerCount++;

		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
	}

	int nWidth, nHeight;
	CRect rcClient;
	GetClientRect(&rcClient);
	nWidth = rcClient.Width();
	nHeight = rcClient.Height();
	CFont font;
	VERIFY(font.CreateFont(25,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,"Arial"));                

	pDC->SetTextColor(RGB(255,255,255));
	CFont* def_font = pDC->SelectObject(&font);
	strPoint.Format(TEXT("%d"), nBankerCount);
	pDC->TextOut(nWidth/2-350+5*120+78, nHeight/2+257, strPoint);
	strPoint.Format(TEXT("%d"), nPlayerCount);
	pDC->TextOut(nWidth/2-350+5*120+78, nHeight/2+288, strPoint);
	strPoint.Format(TEXT("%d"), nTieCount);
	pDC->TextOut(nWidth/2-350+5*120+78, nHeight/2+320, strPoint);
	pDC->SelectObject(def_font);
	font.DeleteObject();*/
}

//�ƶ���ť
void CGameClientView::OnScoreMoveL()
{
	if ( m_nRecordFirst == m_nScoreHead ) return;

	m_nScoreHead = (m_nScoreHead - 1 + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
	if ( m_nScoreHead == m_nRecordFirst ) m_btScoreMoveL.EnableWindow(FALSE);

	m_btScoreMoveR.EnableWindow(TRUE);

	//���½���
	UpdateGameView(NULL);
}

//�ƶ���ť
void CGameClientView::OnScoreMoveR()
{
	int nHistoryCount = ( m_nRecordLast - m_nScoreHead + MAX_SCORE_HISTORY ) % MAX_SCORE_HISTORY;
	if ( nHistoryCount == 12 ) return;

	m_nScoreHead = ( m_nScoreHead + 1 ) % MAX_SCORE_HISTORY;
	if ( nHistoryCount-1 == 12 ) m_btScoreMoveR.EnableWindow(FALSE);

	m_btScoreMoveL.EnableWindow(TRUE);

	//���½���
	UpdateGameView(NULL);
}

void CGameClientView::OnShouDongModel()
{

	//const tagUserData *pMeUserData = GetUserInfo(m_wMeChairID);
	//if ( pMeUserData != NULL && pMeUserData->cbUserStatus == US_PLAY)
	if (m_pGameClientDlg->GetGameStatus()==GS_GAME_END)
	{
		//InsertSystemString("��Ϸ�в����л�����ģʽ��");
		m_pGameClientDlg->InsertShtickString("��ϵͳ��ʾ����Ϸ�в����л�����ģʽ��",RGB(255,0,0),true);
		return ;
	}
		m_bAutoModel=false;
		//InsertSystemString("��ǰģʽ���ֹ�����ģʽ");
		m_pGameClientDlg->InsertShtickString("��ϵͳ��ʾ����ǰΪ�ֹ�����ģʽ��",RGB(255,0,0),true);


}
void CGameClientView::OnZiDongModel()
{
//	const tagUserData *pMeUserData = GetUserInfo(m_wMeChairID);
	if (m_pGameClientDlg->GetGameStatus()==GS_GAME_END)
	{
		m_pGameClientDlg->InsertShtickString("��ϵͳ��ʾ����Ϸ�в����л�����ģʽ��",RGB(255,0,0),true);
		return;
	}

	m_pGameClientDlg->InsertShtickString("��ϵͳ��ʾ����ǰΪ�Զ�����ģʽ��",RGB(255,0,0),true);
	m_bAutoModel=true;
}
//��ʾ���
void CGameClientView::ShowGameResult(CDC *pDC, int nWidth, int nHeight)
{
	//��ʾ�ж�
	if (m_pGameClientDlg->GetGameStatus()!=GS_GAME_END || m_cbSendCount[INDEX_PLAYER1]+m_cbSendCount[INDEX_BANKER]!=0 ||
		m_ptDispatchCard.x!=0 || m_ptDispatchCard.y!=0) return;

	int	nXPos = nWidth / 2 - 129;
	int	nYPos = nHeight / 2 - 208;
	CImageHandle ImageHandleGameEnd(&m_ImageGameEnd);
//	CImageHandle ImageHandleGameEndFrame(&m_ImageGameEndFrame);	
//	CImageHandle ImageHandleGamePoint(&m_ImageGamePoint);

	//�滭�߿�
//	m_ImageGameEndFrame.AlphaDrawImage(pDC, nXPos, nYPos, RGB(255,0,255));

	CRect rcAlpha(nXPos+2, nYPos+130, nXPos+2 + m_ImageGameEnd.GetWidth(), nYPos+130+m_ImageGameEnd.GetHeight());
	DrawAlphaRect(pDC, &rcAlpha, RGB(74,70,73), 0.8f);
	m_ImageGameEnd.AlphaDrawImage(pDC, nXPos+2, nYPos+130, RGB(255,0,255));

	//�������
	//BYTE cbPlayerPoint, cbBankerPoint;
	//cbPlayerPoint = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER1], 5);

	////�������
	//cbBankerPoint = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER],5);

	////�滭����
	//int nPointWidth = m_ImageGamePoint.GetWidth() / 10;
	//m_ImageGamePoint.AlphaDrawImage(pDC, nXPos + 28, nYPos + 10, nPointWidth, m_ImageGamePoint.GetHeight(),
	//	cbPlayerPoint * nPointWidth, 0, RGB(255, 0, 255));
	//m_ImageGamePoint.AlphaDrawImage(pDC, nXPos + 28 + 136, nYPos + 10, nPointWidth, m_ImageGamePoint.GetHeight(),
	//	cbBankerPoint * nPointWidth, 0, RGB(255, 0, 255));

	pDC->SetTextColor(RGB(255,234,0));
	CRect rcMeWinScore, rcMeReturnScore;
	rcMeWinScore.left = nXPos+2 + 40;
	rcMeWinScore.top = nYPos+130 + 32;
	rcMeWinScore.right = rcMeWinScore.left + 111;
	rcMeWinScore.bottom = rcMeWinScore.top + 34;

	rcMeReturnScore.left = nXPos+2 + 150;
	rcMeReturnScore.top = nYPos+130 + 32;
	rcMeReturnScore.right = rcMeReturnScore.left + 111;
	rcMeReturnScore.bottom = rcMeReturnScore.top + 34;

	CString strMeGameScore, strMeReturnScore;
	DrawNumberStringWithSpace(pDC,m_lMeCurGameScore,rcMeWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	DrawNumberStringWithSpace(pDC,m_lMeCurGameReturnScore,rcMeReturnScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	CRect rcBankerWinScore;
	rcBankerWinScore.left = nXPos+2 + 40;
	rcBankerWinScore.top = nYPos+130 + 69;
	rcBankerWinScore.right = rcBankerWinScore.left + 111;
	rcBankerWinScore.bottom = rcBankerWinScore.top + 34;

	CString strBankerCurGameScore;
	strBankerCurGameScore.Format(TEXT("%ld"), m_lBankerCurGameScore);
	DrawNumberStringWithSpace(pDC,m_lBankerCurGameScore,rcBankerWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
}

//͸���滭
bool CGameClientView::DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha)
{
	ASSERT(pDC != 0 && lpRect != 0 && clr != CLR_NONE);
	if(pDC == 0 || lpRect == 0 || clr == CLR_NONE)
	{
		return false;
	}
	//ȫ͸��
	if( abs(fAlpha) <= 0.000001 )
	{
		return true;
	}

	for(LONG l=lpRect->top; l<lpRect->bottom; l++)
	{
		for(LONG k=lpRect->left; k<lpRect->right; k++)
		{
			COLORREF clrBk = pDC->GetPixel(k, l);
			COLORREF clrBlend = RGB(GetRValue(clrBk)*(1-fAlpha)+GetRValue(clr)*fAlpha, 
								GetGValue(clrBk)*(1-fAlpha)+GetGValue(clr)*fAlpha,
								GetBValue(clrBk)*(1-fAlpha)+GetBValue(clr)*fAlpha);
			pDC->SetPixel(k, l, clrBlend);
		}
	}

	return true;
}

//�����ע
LONG CGameClientView::GetMaxTianScore()
{	
	LONG lMaxPlayerScore = 0;

	//ׯ���ж�
	if (m_wBankerUser!=INVALID_CHAIR) 
	{
		//�����ע
		lMaxPlayerScore =  m_lBankerScore;
		lMaxPlayerScore -= (m_lAllTianScore+m_lAllDiScore + m_lAllXuanScore + m_lAllHuangScore)*10;
		lMaxPlayerScore = min(lMaxPlayerScore/10, (m_lAreaLimitScore-m_lAllTianScore));
	}
	else
	{
		lMaxPlayerScore=m_lAreaLimitScore-m_lAllTianScore;
	}

	return lMaxPlayerScore;
}

//�����ע
LONG CGameClientView::GetMaxDiScore()
{
	LONG lMaxPlayerScore = 0;
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//�����ע
		lMaxPlayerScore =  m_lBankerScore;
		lMaxPlayerScore -= (m_lAllTianScore+m_lAllDiScore + m_lAllXuanScore + m_lAllHuangScore)*10;
		lMaxPlayerScore = min(lMaxPlayerScore/10, (m_lAreaLimitScore-m_lAllDiScore));
	}
	else
	{
		lMaxPlayerScore=m_lAreaLimitScore-m_lAllDiScore;
	}

	return lMaxPlayerScore;
}

//�����ע
LONG CGameClientView::GetMaxXuanScore()
{
	LONG lMaxPlayerScore = 0;
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//�����ע
		lMaxPlayerScore =  m_lBankerScore;
		lMaxPlayerScore -= (m_lAllTianScore+m_lAllDiScore + m_lAllXuanScore + m_lAllHuangScore)*10;
		lMaxPlayerScore = min(lMaxPlayerScore/10, (m_lAreaLimitScore-m_lAllXuanScore));
	}
	else
	{
		lMaxPlayerScore=m_lAreaLimitScore-m_lAllXuanScore;
	}
	return lMaxPlayerScore;
}
//�����ע
LONG CGameClientView::GetMaxHuangScore()
{
	LONG lMaxPlayerScore = 0;
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//�����ע
		lMaxPlayerScore =  m_lBankerScore;
		lMaxPlayerScore -= (m_lAllTianScore+m_lAllDiScore + m_lAllXuanScore + m_lAllHuangScore)*10;
		lMaxPlayerScore = min(lMaxPlayerScore/10, (m_lAreaLimitScore-m_lAllHuangScore));
	}
	else
	{
		lMaxPlayerScore=m_lAreaLimitScore-m_lAllHuangScore;
	}
	return lMaxPlayerScore;
}
/*
//�����ע
LONG CGameClientView::GetMaxBankerScore()
{
	LONG lMaxPlayerScore = 0;
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//��������
		LONG lOtherAreaScore = m_lAllTieScore + m_lAllPlayerScore + m_lAllTieSamePointScore + m_lAllPlayerKingScore;

		//�����ע
		lMaxPlayerScore = lOtherAreaScore + m_lBankerScore;
		lMaxPlayerScore -= (m_lAllBankerScore + m_lAllBankerKingScore*2 + (m_lAllPlayerTwoPair+m_lAllBankerTwoPair)*RATE_TWO_PAIR);
		lMaxPlayerScore = min(lMaxPlayerScore, (m_lAreaLimitScore-m_lAllBankerScore));
	}
	else
	{
		lMaxPlayerScore = m_lAreaLimitScore-m_lAllBankerScore;
	}

	return lMaxPlayerScore;
}

//�����ע
LONG CGameClientView::GetMaxBankerKingScore()
{
	LONG lMaxPlayerScore = 0;
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//��������
		LONG lOtherAreaScore = m_lAllTieScore + m_lAllPlayerScore + m_lAllTieSamePointScore + m_lAllPlayerKingScore;

		//�����ע
		lMaxPlayerScore = lOtherAreaScore + m_lBankerScore;
		lMaxPlayerScore -= (m_lAllBankerScore + m_lAllBankerKingScore*2+(m_lAllPlayerTwoPair+m_lAllBankerTwoPair)*RATE_TWO_PAIR);
		lMaxPlayerScore = min(lMaxPlayerScore/2, (m_lAreaLimitScore-m_lAllBankerKingScore));
	}
	else
	{
		lMaxPlayerScore=m_lAreaLimitScore-m_lAllBankerKingScore;
	}

	return lMaxPlayerScore;
}

//�����ע
LONG CGameClientView::GetMaxTieScore()
{
	LONG lMaxTieScore = 0;
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//���ػ���
		LONG lReturnScore = m_lAllTieSamePointScore * 33 + m_lAllTieScore * 8+(m_lAllPlayerTwoPair+m_lAllBankerTwoPair)*RATE_TWO_PAIR;

		//���»���
		lMaxTieScore = min((m_lBankerScore - lReturnScore)/8, (m_lAreaLimitScore-m_lAllTieScore));
	}
	else
	{
		lMaxTieScore = m_lAreaLimitScore-m_lAllTieScore;
	}

	return lMaxTieScore;
}

//�����ע
LONG CGameClientView::GetMaxTieKingScore()
{
	LONG lMaxTieScore = 0;
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//���ػ���
		LONG lReturnScore = m_lAllTieSamePointScore * 33 + m_lAllTieScore * 8+(m_lAllPlayerTwoPair+m_lAllBankerTwoPair)*RATE_TWO_PAIR;

		//���»���
		lMaxTieScore = min(( m_lBankerScore - lReturnScore )/33, (m_lAreaLimitScore-m_lAllTieSamePointScore)) ;
	}
	else
	{
		lMaxTieScore = m_lAreaLimitScore-m_lAllTieSamePointScore;
	}

	return lMaxTieScore;
}
*/
//�ɼ�����
void CGameClientView::SetGameScore(LONG lMeCurGameScore, LONG lMeCurGameReturnScore, LONG lBankerCurGameScore)
{
	m_lMeCurGameScore=lMeCurGameScore;
	m_lMeCurGameReturnScore=lMeCurGameReturnScore;
	m_lBankerCurGameScore=lBankerCurGameScore;	
}

//�滭����
void CGameClientView::DrawMeJettonNumber(CDC *pDC)
{
	//�滭����
	if (0 < m_lMeTianScore) DrawNumberString(pDC,m_lMeTianScore,m_PointJettonNumber[0].x,m_PointJettonNumber[0].y+25, true);
	if (0 < m_lMeDiScore) DrawNumberString(pDC,m_lMeDiScore,m_PointJettonNumber[1].x,m_PointJettonNumber[1].y+25, true);
	if (0 < m_lMeXuanScore) DrawNumberString(pDC,m_lMeXuanScore,m_PointJettonNumber[2].x,m_PointJettonNumber[2].y+25, true);
	if (0 < m_lMeHuangScore) DrawNumberString(pDC,m_lMeHuangScore,m_PointJettonNumber[3].x,m_PointJettonNumber[3].y+25, true);
}

//��ʼ����
void CGameClientView::DispatchCard()
{
	//���ñ���
	//m_cbSendCount[INDEX_PLAYER]=0;
	//m_cbSendCount[INDEX_BANKER]=0;
	for(int i=0;i<5;i++)
	{
		m_cbSendCount[i]=0;
		m_CardControl[i].SetCardData(NULL,0);
	}

	//���ý���
	//m_CardControl[INDEX_PLAYER].SetCardData(NULL,0);
	//m_CardControl[INDEX_BANKER].SetCardData(NULL,0);

	//�����˿�
	BYTE cbCardData=0;
	m_DispatchCard.SetCardData(&cbCardData,1);

	//����λ��
	CRect rcView;
	GetClientRect(&rcView);
	m_ptDispatchCard.x = rcView.Width()/2+220;
	m_ptDispatchCard.y = rcView.Height()/2-200;

	//���ö�ʱ��
	SetTimer(IDI_DISPATCH_CARD,200,NULL);
}

//��������
void CGameClientView::FinishDispatchCard()
{
	//ɾ����ʱ��
	KillTimer(IDI_DISPATCH_CARD);

	//�����˿�
	//m_CardControl[INDEX_BANKER].SetCardData(m_cbTableCardArray[0],5);
	//m_CardControl[INDEX_PLAYER1].SetCardData(m_cbTableCardArray[1],5);
	//m_CardControl[INDEX_PLAYER2].SetCardData(m_cbTableCardArray[2],5);
	//m_CardControl[INDEX_PLAYER3].SetCardData(m_cbTableCardArray[3],5);
	//m_CardControl[INDEX_PLAYER4].SetCardData(m_cbTableCardArray[4],5);

	//bool bHaveNiu=false;

	//for(BYTE i=0;i<5;i++)
	//{
	//	bHaveNiu=m_GameLogic.GetOxCard(m_cbTableCardArray[i],5);
	//	m_CardControl[i].SetNiuCardData(m_cbTableCardArray[i],5,bHaveNiu);
	//}
	//
	m_DispatchCard.SetCardData(NULL,0);

	//�������
	ZeroMemory(m_cbSendCount,sizeof(m_cbSendCount));

	//���ü�¼
	if (m_pGameClientDlg->GetGameStatus()==GS_GAME_END)
	{
		//�˿˵���
//		BYTE cbPlayerPoint=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER1],5);
//		BYTE cbBankerPoint=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER],5);

		//��������
		//enOperateResult OperateResult = enOperateResult_NULL;
		//if (0 < m_lMeCurGameScore) OperateResult = enOperateResult_Win;
		//else if (m_lMeCurGameScore < 0) OperateResult = enOperateResult_Lost;
		//else OperateResult = enOperateResult_NULL;

		//�ƶ�Ӯ��
		BYTE cbWinner;
		DeduceWinner(cbWinner);


		for(BYTE i=0;i<5;i++)
		{
			m_cbNiuType[i]=GetIconNum(m_cbTableCardArray[i],3);
		}
		
		//�����¼
//		SetGameHistory(OperateResult, cbPlayerPoint, cbBankerPoint, cbKingWinner, bPlayerTwoPair,bBankerTwoPair);
		SetGameHistory(enOperateResult_Pleace,cbWinner);
		//�ۼƻ���
		m_lMeStatisticScore+=m_lMeCurGameScore;
		m_lBankerWinScore=m_lTmpBankerWinScore;
		

		//����Ӯ��
		SetWinnerSide(cbWinner);


		//��������
		if (m_lMeCurGameScore>0) m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_WIN"));
		else if (m_lMeCurGameScore<0) m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_LOST"));
		else m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_DRAW"));
	}
}

//ʤ���߿�
void CGameClientView::FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC)
{
	if (m_cbAreaFlash==0xFF) return;

	//������Դ
	CImageHandle HandleWinFlags(&m_ImageWinFlags);
	CSize SizeWinFlags(m_ImageWinFlags.GetWidth()/3,m_ImageWinFlags.GetHeight());

	//λ�ñ���
	int nXPos = 0;
	int nYPos = 0;

	//�мұ�־
	switch (m_cbAreaFlash)
	{
	case ID_TIAN_MARK:	
		{
			CImageHandle ImageHandleXianJia(&m_ImageFrameXianJia);
			m_ImageFrameXianJia.AlphaDrawImage(pDC,m_rcTian.left, m_rcTian.top, RGB(255,0,255));
			break;
		}
	case ID_DI_MARK:
		{
			CImageHandle ImageHandleXianTianWang(&m_ImageFrameXianJia);
			m_ImageFrameXianJia.AlphaDrawImage(pDC,m_rcDi.left, m_rcDi.top, RGB(255,0,255));
			break;
		}
	case ID_XUAN_MARK:
		{		
			CImageHandle ImageHandlePlayerTwoPair(&m_ImageFrameXianJia);
			m_ImageFrameXianJia.AlphaDrawImage(pDC,m_rcXuan.left, m_rcXuan.top, RGB(255,0,255));
			break;
		}
	case ID_HUANG_MARK:
		{
			CImageHandle ImageHandlePingJia(&m_ImageFrameXianJia);			
			m_ImageFrameXianJia.AlphaDrawImage(pDC,m_rcHuang.left, m_rcHuang.top, RGB(255,0,255));
			break;
		}
	}

		if(m_cbAreaFlash & ID_TIAN_MARK)
		{
			CImageHandle ImageHandleXianJia(&m_ImageFrameXianJia);
			m_ImageFrameXianJia.AlphaDrawImage(pDC,m_rcTian.left, m_rcTian.top, RGB(255,0,255));
		}
		if(m_cbAreaFlash &  ID_DI_MARK)
		{
			CImageHandle ImageHandleXianTianWang(&m_ImageFrameXianJia);
			m_ImageFrameXianJia.AlphaDrawImage(pDC,m_rcDi.left, m_rcDi.top, RGB(255,0,255));
		}
		if(m_cbAreaFlash & ID_XUAN_MARK)
		{		
			CImageHandle ImageHandlePlayerTwoPair(&m_ImageFrameXianJia);
			m_ImageFrameXianJia.AlphaDrawImage(pDC,m_rcXuan.left, m_rcXuan.top, RGB(255,0,255));
		}
		if(m_cbAreaFlash & ID_HUANG_MARK)
		{
			CImageHandle ImageHandlePingJia(&m_ImageFrameXianJia);			
			m_ImageFrameXianJia.AlphaDrawImage(pDC,m_rcHuang.left, m_rcHuang.top, RGB(255,0,255));
		}
		if(!(m_cbAreaFlash & ID_TIAN_MARK) && !(m_cbAreaFlash & ID_DI_MARK) &&
			!(m_cbAreaFlash & ID_XUAN_MARK) && !(m_cbAreaFlash & ID_HUANG_MARK))
		{
			CImageHandle ImageTongChi(&m_ImageFrameZhuangJia);			
			m_ImageFrameZhuangJia.AlphaDrawImage(pDC,m_rcTian.left, m_rcTian.top, RGB(255,0,255));
			m_ImageFrameZhuangJia.AlphaDrawImage(pDC,m_rcDi.left, m_rcDi.top, RGB(255,0,255));
			m_ImageFrameZhuangJia.AlphaDrawImage(pDC,m_rcXuan.left, m_rcXuan.top, RGB(255,0,255));
			m_ImageFrameZhuangJia.AlphaDrawImage(pDC,m_rcHuang.left, m_rcHuang.top, RGB(255,0,255));
		}
	

/*
	//�����ж�
	if (m_pGameClientDlg->GetGameStatus()==GS_GAME_END && m_cbSendCount[0]+m_cbSendCount[0]==0)
	{
		//�ƶ�Ӯ��
		BYTE cbWinner,cbKingWinner;
		bool bPlayerTwoPair,bBankerTwoPair;
		DeduceWinner(cbWinner, cbKingWinner, bPlayerTwoPair,bBankerTwoPair);

		//������
		if (cbKingWinner==ID_XIAN_TIAN_WANG)
		{
			CImageHandle ImageHandleXianTianWang(&m_ImageFrameXianTianWang);
			m_ImageFrameXianTianWang.AlphaDrawImage(pDC,m_rcPlayerKing.left, m_rcPlayerKing.top, RGB(255,0,255));
		}
		else if (cbKingWinner==ID_ZHUANG_TIAN_WANG)
		{
			CImageHandle ImageHandleZhuangTianWang(&m_ImageFrameZhuangTianWang);
			m_ImageFrameZhuangTianWang.AlphaDrawImage(pDC,m_rcBankerKing.left, m_rcBankerKing.top, RGB(255,0,255));
		}
		else if (cbKingWinner==ID_TONG_DIAN_PING)
		{
			CImageHandle ImageHandleTongDianPing(&m_ImageFrameTongDianPing);
			m_ImageFrameTongDianPing.AlphaDrawImage(pDC,m_rcTieKing.left, m_rcTieKing.top, RGB(255,0,255));
		}

		//����		
		if (bBankerTwoPair)
		{
			CImageHandle ImageHandleBankerTwoPair(&m_ImageFrameBankerTwoPair);
			m_ImageFrameBankerTwoPair.AlphaDrawImage(pDC,m_rcBankerTwoPair.left, m_rcBankerTwoPair.top, RGB(255,0,255));
		}
		if (bPlayerTwoPair)
		{
			CImageHandle ImageHandlePlayerTwoPair(&m_ImageFramePlayerTwoPair);
			m_ImageFramePlayerTwoPair.AlphaDrawImage(pDC,m_rcPlayerTwoPair.left, m_rcPlayerTwoPair.top, RGB(255,0,255));
		}
	}
	*/
}


//�ƶ�Ӯ��
void CGameClientView::DeduceWinner(BYTE &cbWinner)
{
	cbWinner = 0x00;

	if (m_GameLogic.IsBigger(m_cbTableCardArray[1], m_cbTableCardArray[0], 3, 3) )
		cbWinner |= 0x01; 
	if (m_GameLogic.IsBigger(m_cbTableCardArray[2], m_cbTableCardArray[0], 3, 3) )
		cbWinner |= 0x02;
	if (m_GameLogic.IsBigger(m_cbTableCardArray[3], m_cbTableCardArray[0], 3, 3) )
		cbWinner |= 0x04;
	if (m_GameLogic.IsBigger(m_cbTableCardArray[4], m_cbTableCardArray[0], 3, 3) )
		cbWinner |= 0x08;
	
	/*for(int i=INDEX_PLAYER1;i<=INDEX_PLAYER4;i++)
	{
		BYTE cbMarkType=0;
		switch(i)
		{
		case 1:
			cbMarkType=ID_TIAN_MARK;
			break;
		case 2:
			cbMarkType=ID_DI_MARK;
			break;
		case 3:
			cbMarkType=ID_XUAN_MARK;
			break;
		case 4:
			cbMarkType=ID_HUANG_MARK;
			break;
		}
		if (m_GameLogic.CompareCard(m_cbTableCardArray[i],m_cbTableCardArray[INDEX_BANKER],MAX_COUNT))
		{
			if(cbMarkType)
			{
				cbWinner|=cbMarkType;
			}
		}
		else
		{
			if(cbMarkType)
			{
				cbWinner = (cbWinner & ~cbMarkType);
			}

		}
	}*/
/*	cbKingWinner = 0;

	//�����Ƶ�
	BYTE cbPlayerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER],m_cbCardCount[INDEX_PLAYER]);
	BYTE cbBankerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER],m_cbCardCount[INDEX_BANKER]);

	//ʤ�����
	if (cbPlayerCount==cbBankerCount)
	{
		cbWinner=ID_PING_JIA;

		//ͬ��ƽ�ж�
		bool bAllPointSame = false;
		if ( m_cbCardCount[INDEX_PLAYER] == m_cbCardCount[INDEX_BANKER] )
		{
			for (WORD wCardIndex = 0; wCardIndex < m_cbCardCount[INDEX_PLAYER]; ++wCardIndex )
			{
				BYTE cbBankerValue = m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_BANKER][wCardIndex]);
				BYTE cbPlayerValue = m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_PLAYER][wCardIndex]);
				if ( cbBankerValue != cbPlayerValue ) break;
			}
			if ( wCardIndex == m_cbCardCount[INDEX_PLAYER] ) bAllPointSame = true;
		}
		if ( bAllPointSame ) cbKingWinner = ID_TONG_DIAN_PING;
	}
	else if (cbPlayerCount<cbBankerCount) 
	{
		cbWinner=ID_ZHUANG_JIA;

		//�����ж�
		if ( cbBankerCount == 8 || cbBankerCount == 9 ) cbKingWinner = ID_ZHUANG_TIAN_WANG;
	}
	else 
	{
		cbWinner=ID_XIAN_JIA;

		//�����ж�
		if ( cbPlayerCount == 8 || cbPlayerCount == 9 ) cbKingWinner = ID_XIAN_TIAN_WANG;
	}

	//�����ж�
	bPlayerTwoPair=false;
	bBankerTwoPair=false;
	if (m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_PLAYER][0])==m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_PLAYER][1]))
		bPlayerTwoPair=true;
	if (m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_BANKER][0])==m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_BANKER][1]))
		bBankerTwoPair=true;*/
}

//������ʾ
void CGameClientView::SetDispatchCardTips()
{
/*	if (m_cbSendCount[INDEX_PLAYER]+m_cbSendCount[INDEX_BANKER] < 4) return;

	//�������
	BYTE cbBankerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER],2);
	BYTE cbPlayerTwoCardCount = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER],2);

	//�мҲ���	
	if(cbPlayerTwoCardCount<=5 && cbBankerCount<8 && m_cbSendCount[INDEX_PLAYER]+m_cbSendCount[INDEX_BANKER]==4)
	{		
		CString strTips;
		strTips.Format(TEXT("��%d�㣬ׯ%d�㣬�м�������\n"), cbPlayerTwoCardCount, cbBankerCount);
		m_strDispatchCardTips = strTips;

		return;
	}

	if (m_cbSendCount[INDEX_BANKER]==3) return;

	BYTE cbPlayerThirdCardValue = 0 ;	//�������Ƶ���

	//�������
	cbPlayerThirdCardValue = m_GameLogic.GetCardPip(m_cbTableCardArray[INDEX_PLAYER][2]);

	//ׯ�Ҳ���
	CString strTips;
	if(cbPlayerTwoCardCount<8 && cbBankerCount<8)
	{
		switch(cbBankerCount)
		{
		case 0:
		case 1:
		case 2:
			strTips.Format(TEXT("��ǰ������%d�㣬ׯ%d�㣬ׯ��������\n"), cbPlayerTwoCardCount, cbBankerCount);
			m_strDispatchCardTips += strTips;
			break;

		case 3:
			if(m_cbCardCount[INDEX_PLAYER]==3 && cbPlayerThirdCardValue!=8)
			{
				strTips.Format(TEXT("�е�������%d�㣬ׯ%d�㣬ׯ��������\n"), cbPlayerThirdCardValue, cbBankerCount);
				m_strDispatchCardTips += strTips;
			}
			else if (m_cbCardCount[INDEX_PLAYER]==2) 
			{
				strTips.Format(TEXT("�в����ƣ�ׯ%d�㣬ׯ��������\n"), cbBankerCount);
				m_strDispatchCardTips += strTips;
			}			
			break;

		case 4:
			if(m_cbCardCount[INDEX_PLAYER]==3 && cbPlayerThirdCardValue!=1 && cbPlayerThirdCardValue!=8 && cbPlayerThirdCardValue!=9 && cbPlayerThirdCardValue!=0)
			{
				strTips.Format(TEXT("�е�������%d�㣬ׯ%d�㣬ׯ��������\n"), cbPlayerThirdCardValue, cbBankerCount);
				m_strDispatchCardTips += strTips;
			}
			else if ( m_cbCardCount[INDEX_PLAYER]==2) 
			{
				strTips.Format(TEXT("�в����ƣ�ׯ%d�㣬ׯ��������\n"), cbBankerCount);
				m_strDispatchCardTips += strTips;
			}
			break;

		case 5:
			if(m_cbCardCount[INDEX_PLAYER]==3 && cbPlayerThirdCardValue!=1 && cbPlayerThirdCardValue!=2 && cbPlayerThirdCardValue!=3  && cbPlayerThirdCardValue!=8 && cbPlayerThirdCardValue!=9 &&  cbPlayerThirdCardValue!=0)
			{
				strTips.Format(TEXT("�е�������%d�㣬ׯ%d�㣬ׯ��������\n"), cbPlayerThirdCardValue, cbBankerCount);
				m_strDispatchCardTips += strTips;
			}
			else if ( m_cbCardCount[INDEX_PLAYER]==2) 
			{
				strTips.Format(TEXT("�в����ƣ�ׯ%d�㣬ׯ��������\n"), cbBankerCount);
				m_strDispatchCardTips += strTips;
			}
			break;

		case 6:
			if(m_cbCardCount[INDEX_PLAYER]==3 && (cbPlayerThirdCardValue==6 || cbPlayerThirdCardValue==7))
			{
				strTips.Format(TEXT("�е�������%d�㣬ׯ%d�㣬ׯ��������\n"), cbPlayerThirdCardValue, cbBankerCount);
				m_strDispatchCardTips += strTips;
			}
			break;

			//���벹��
		case 7:
		case 8:
		case 9:
			break;
		default:
			break;
		}
	}
*/
	return ;
}

//���ñ�ը
bool CGameClientView::SetBombEffect(bool bBombEffect, WORD wAreaIndex)
{
	WORD wIndex=0;
	switch(wAreaIndex)
	{
	case ID_TIAN_MARK:
		wAreaIndex=0;
		break;
	case ID_DI_MARK:
		wAreaIndex=1;
		break;
	case ID_XUAN_MARK:
		wAreaIndex=2;
		break;
	case ID_HUANG_MARK:
		wAreaIndex=3;
		break;
	}
	if (bBombEffect==true)
	{
		//���ñ���
		m_bBombEffect[wIndex]=true;
		m_cbBombFrameIndex[wIndex]=0;

		//����ʱ��
		SetTimer(IDI_BOMB_EFFECT+wIndex,100,NULL);
	}
	else
	{
		//ֹͣ����
		if (m_bBombEffect[wIndex]==true)
		{
			//ɾ��ʱ��
			KillTimer(IDI_BOMB_EFFECT+wIndex);

			//���ñ���
			m_bBombEffect[wIndex]=false;
			m_cbBombFrameIndex[wIndex]=0;

			//���½���
			UpdateGameView(NULL);
		}
	}

	return true;
}

//�滭��ը
void CGameClientView::DrawBombEffect(CDC *pDC)
{
	//�滭��ը
	INT nImageHeight=m_ImageBombEffect.GetHeight();
	INT nImageWidth=m_ImageBombEffect.GetWidth()/BOMB_EFFECT_COUNT;
	if (m_bBombEffect[0]==true)
	{
		m_ImageBombEffect.DrawImage(pDC,m_PointJettonNumber[0].x-nImageWidth/2,m_PointJettonNumber[0].y,nImageWidth,nImageHeight,
			nImageWidth*(m_cbBombFrameIndex[0]%BOMB_EFFECT_COUNT),0);
	}
	if (m_bBombEffect[1]==true)
	{
		m_ImageBombEffect.DrawImage(pDC,m_PointJettonNumber[1].x-nImageWidth/2,m_PointJettonNumber[1].y,nImageWidth,nImageHeight,
			nImageWidth*(m_cbBombFrameIndex[1]%BOMB_EFFECT_COUNT),0);
	}
	if (m_bBombEffect[2]==true)
	{
		m_ImageBombEffect.DrawImage(pDC,m_PointJettonNumber[2].x-nImageWidth/2,m_PointJettonNumber[2].y,nImageWidth,nImageHeight,
			nImageWidth*(m_cbBombFrameIndex[2]%BOMB_EFFECT_COUNT),0);
	}
	if (m_bBombEffect[3]==true)
	{
		m_ImageBombEffect.DrawImage(pDC,m_PointJettonNumber[3].x-nImageWidth/2,m_PointJettonNumber[3].y,nImageWidth,nImageHeight,
			nImageWidth*(m_cbBombFrameIndex[3]%BOMB_EFFECT_COUNT),0);
	}
}
//////////////////////////////////////////////////////////////////////////


void CGameClientView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(!m_bAutoModel)
	{

		for(int i=0;i<5;i++)
		{
			m_CardControl[i].OnLButtonUp(point);
		}
		UpdateGameView(NULL);
		KillTimer(1111);
		ReleaseCapture();
	}

	CGameFrameView::OnLButtonUp(nFlags, point);
}

void CGameClientView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(!m_bAutoModel)
	{
		for(int i=0;i<5;i++)
		{
			//if(m_CardControl[i].GetShowState())
			{
				m_CardControl[i].OnMouseMove(point);
			}
		}
	}
	CGameFrameView::OnMouseMove(nFlags, point);
}

//��ȡ����ʱ���ͱ�־ͼ��
BYTE CGameClientView::GetIconNum(BYTE cbBufferData[], BYTE cbBufferCount)
{
	BYTE cbIconNum = 0;

	//��ȡ����
	BYTE cbCardType = m_GameLogic.GetCardType(cbBufferData, cbBufferCount);

	if(cbCardType == CARDTYPE_SAN_ZHANG)
		cbIconNum = 9;
	else if(cbCardType == CARDTYPE_SAN_GONG)
		cbIconNum = 10;
	else if(cbCardType == CARDTYPE_BI_SHI)
		cbIconNum = 11;
	else if(cbCardType == CARDTYPE_SAN_PAI)
	{
		//��ȡ�Ƶĵ���
		BYTE cbCardPoint = m_GameLogic.GetPoint(cbBufferData, cbBufferCount);
		if(cbCardPoint>0 && cbCardPoint<=9)
			cbIconNum = cbCardPoint - 1;
	}
	else
		cbIconNum = 0;

	return cbIconNum;

}
