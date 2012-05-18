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
#define IDI_SHOWDISPATCH_CARD_TIP	103									//������ʾ
#define IDI_OPENCARD				104									//������ʾ
#define IDI_MOVECARD_END			105									//�Ƅ��ƽY��
#define IDI_POSTCARD				106									//������ʾ
#define IDI_OUT_CARD_TIME			107
#define IDI_JETTON_ANIMATION		108
#define IDI_HANDLELEAVE_ANIMATION	109  
#define IDI_OPENBOX_ANIMATION		110  
#define IDI_LEAVHANDLESOUND			111
#define IDI_PLEASEJETTONSOUND		112
#define IDI_SPEEKSOUND				113
#define IDI_FLASH_CARD				115									//������ʶ
#define IDI_FLASH_RAND_SIDE			116									//������ʶ
#define IDI_SHOW_CARD_RESULT		118

//��������ע,�������ж�ʱ����������(1000�Ժ�),ר�������ṩ
#define IDI_ANDROID_BET				1000								

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
#define IDC_JETTON_BUTTON_50000	    219									//��ť��ʶ
#define IDC_JETTON_BUTTON_500000	212									//��ť��ʶ
#define IDC_AUTO_OPEN_CARD			213									//��ť��ʶ
#define IDC_OPEN_CARD				214									//��ť��ʶ
#define IDC_BANK					215									//��ť��ʶ
#define IDC_CONTINUE_CARD			216									//��ť��ʶ
#define IDC_BANK_STORAGE			217									//��ť��ʶ
#define IDC_BANK_DRAW				218									//��ť��ʶ
#define IDC_UP						223									//��ť��ʶ
#define IDC_DOWN					224									//��ť��ʶ
#define IDC_RADIO					225									//��ť��ʶ
#define IDC_ADMIN					226									//��ť��ʶ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_ADMIN,OpenAdminWnd)
	ON_BN_CLICKED(IDC_APPY_BANKER, OnApplyBanker)
	ON_BN_CLICKED(IDC_CANCEL_BANKER, OnCancelBanker)
	ON_BN_CLICKED(IDC_SCORE_MOVE_L, OnScoreMoveL)
	ON_BN_CLICKED(IDC_SCORE_MOVE_R, OnScoreMoveR)
	ON_BN_CLICKED(IDC_OPEN_CARD, OnOpenCard)
	ON_BN_CLICKED(IDC_AUTO_OPEN_CARD,OnAutoOpenCard)
	ON_MESSAGE(WM_VIEWLBTUP,OnViLBtUp)
#ifdef  __BANKER___
	ON_BN_CLICKED(IDC_BANK_STORAGE, OnBankStorage)
	ON_BN_CLICKED(IDC_BANK_DRAW, OnBankDraw)
#endif

	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_DOWN, OnDown)


END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView() : CGameFrameView(true,24)
{
	//��ע��Ϣ
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ȫ����ע
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//ׯ����Ϣ
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_lBankerScore=0L;	
	m_lBankerWinScore=0L;
	m_lTmpBankerWinScore=0;
	m_Out_Bao_y = 0;
	m_CarIndex = 0;
	m_lRobotMaxJetton = 5000000l;


	//���ֳɼ�
	m_lMeCurGameScore=0L;	
	m_lMeCurGameReturnScore=0L;
	m_lBankerCurGameScore=0L;
	m_lGameRevenue=0L;

	m_JettonQueIndex = 0; 

	m_lLastJetton = 0;

	//״̬��Ϣ
	m_lCurrentJetton=0L;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=FALSE;
	m_bNeedSetGameRecord=FALSE;
	m_bWinTianMen=FALSE;
	m_bWinHuangMen=FALSE;
	m_bWinXuanMen=FALSE;
	m_bFlashResult=FALSE;
	m_blMoveFinish = FALSE;
	m_blAutoOpenCard = TRUE;
	m_bShowBao = FALSE;
	m_enDispatchCardTip=enDispatchCardTip_NULL;

	m_lMeCurGameScore=0L;			
	m_lMeCurGameReturnScore=0L;	
	m_lBankerCurGameScore=0L;	
	m_blCanStore=false;

	m_lAreaLimitScore=0L;	

	//λ����Ϣ
	m_nScoreHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;	
	m_CheckImagIndex = 0;
	m_ShowImageIndex  = 0;

	//��ʷ�ɼ�
	m_lMeStatisticScore=0;

	//�ؼ�����
	m_pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageViewFill.SetLoadInfo(IDB_VIEW_FILL,hInstance);
	m_ImageViewcentre.SetLoadInfo(IDB_CENTREBACK,hInstance);
	m_ImageBao.SetLoadInfo(IDB_CENTREBACK,hInstance);

	m_ImageWinFlags.LoadImage(hInstance,"IDB_IDB_FLAG_PNG");

	m_ImageJettonView.SetLoadInfo(IDB_JETTOM_VIEW,hInstance);
	m_ImageScoreNumber.SetLoadInfo(IDB_SCORE_NUMBER,hInstance);
	m_ImageMeScoreNumber.SetLoadInfo(IDB_ME_SCORE_NUMBER,hInstance);
	m_ViewBackPng.LoadImage(hInstance,"IDB_VIEW_BACK_PNG");
	m_idb_selPng.LoadImage(hInstance,"IDB_IDB_SEL_PNG");

	for (int i = 0;i<5;i++) m_ImageCardType[i].SetLoadInfo(IDB_CARDTYPE,hInstance);

	m_ImageGameEnd.SetLoadInfo( IDB_GAME_END, hInstance );
	m_ImageMeBanker.SetLoadInfo( IDB_ME_BANKER, hInstance );
	m_ImageChangeBanker.SetLoadInfo( IDB_CHANGE_BANKER, hInstance );
	m_ImageNoBanker.SetLoadInfo( IDB_NO_BANKER, hInstance );	
	m_ImageTimeFlagPng.LoadImage(hInstance,"IDB_TIME_PNG");
	m_TimerCount_png.LoadImage(hInstance,"IDB_TIMERCOUNT_PNG");
	m_ImageMeScoreNumberPng.LoadImage(hInstance,"IDB_MENUM_PNG");
	m_ImageScoreNumberPng.LoadImage(hInstance,"IDB_SCORE_PNG");
	m_pngGameEnd.LoadImage(hInstance,"IDB_GAME_END_PNG");

	//for (int i = 0;i<AREA_COUNT;i++) m_sT_ShowInfo.blShow[i]=false;

	TCHAR szBuffer[128]=TEXT("");
	//�����Ʊ���Դ
	for (int i = 0;i<7;i++)
	{
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("IDB_PUSHBOX%d"),i+1);
		m_PngPushBox[i].LoadImage(hInstance,szBuffer);
	}

	for (int i = 0;i<4;i++)
	{
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("IDB_JETTON%d"),i+1);
		m_PngShowJetton[i].LoadImage(hInstance,szBuffer);
	}

	for (int i = 0;i<4;i++)
	{
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("IDB_HANDLELEAVE%d"),i+1);
		m_PngShowLeaveHandle[i].LoadImage(hInstance,szBuffer);
	}


	for (int i = 0;i<4;i++)
	{
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("IDB_RESULT%d"),i+1);
		m_PngResult[i].LoadImage(hInstance,szBuffer);
	}

	m_bEnablePlaceJetton = false;

	int w = 40+30+10+5*4;
	w= 1*2*4;
	w = 1*2*4;
	w = 2*4;
	w = 1*4;
	w = 1*4;
	w = 1*3;

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

	//���밴ť
	m_btApplyBanker.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_APPY_BANKER);
	m_btCancelBanker.Create(NULL,WS_CHILD|WS_DISABLED,rcCreate,this,IDC_CANCEL_BANKER);

	m_btScoreMoveL.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_L);
	m_btScoreMoveR.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_R);

	m_btAutoOpenCard.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_AUTO_OPEN_CARD);
	m_btOpenCard.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_OPEN_CARD);

	m_btBank.Create(NULL,WS_CHILD,rcCreate,this,IDC_BANK);

	m_btBankerStorage.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BANK_STORAGE);
	m_btBankerDraw.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BANK_DRAW);

	m_btUp.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_UP);
	m_btDown.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_DOWN);



	//���ð�ť
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btJetton100.LoadRgnImage(hResInstance,IDB_BT_JETTON_100,RGB(255,0,255));
	m_btJetton1000.LoadRgnImage(hResInstance,IDB_BT_JETTON_1000,RGB(255,0,255));
	m_btJetton50000.LoadRgnImage(hResInstance,IDB_BT_JETTON_50000,RGB(255,0,255));
	m_btJetton10000.LoadRgnImage(hResInstance,IDB_BT_JETTON_10000,RGB(255,0,255));
	m_btJetton100000.LoadRgnImage(hResInstance,IDB_BT_JETTON_100000,RGB(255,0,255));
	m_btJetton500000.LoadRgnImage(hResInstance,IDB_BT_JETTON_500000,RGB(255,0,255));
	m_btJetton1000000.LoadRgnImage(hResInstance,IDB_BT_JETTON_1000000,RGB(255,0,255));
	m_btJetton5000000.LoadRgnImage(hResInstance,IDB_BT_JETTON_5000000,RGB(255,0,255));

	m_btApplyBanker.SetButtonImage(IDB_BT_APPLY_BANKER,hResInstance,false);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,hResInstance,false);


	m_btUp.SetButtonImage(IDB_BT_BT_S,hResInstance,false);
	m_btDown.SetButtonImage(IDB_BT_BT_X,hResInstance,false);

	m_btScoreMoveL.SetButtonImage(IDB_BT_SCORE_MOVE_L,hResInstance,false);
	m_btScoreMoveR.SetButtonImage(IDB_BT_SCORE_MOVE_R,hResInstance,false);

	m_btAutoOpenCard.SetButtonImage(IDB_BT_AUTO_OPEN_CARD,hResInstance,false);
	m_btOpenCard.SetButtonImage(IDB_BT_OPEN_CARD,hResInstance,false);

	m_btBank.SetButtonImage(IDB_BT_BANK,hResInstance,false);

	m_btBankerStorage.SetButtonImage(IDB_BT_STORAGE,hResInstance,false);
	m_btBankerDraw.SetButtonImage(IDB_BT_DRAW,hResInstance,false);

	m_btOpenAdmin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(4,4,11,11),this,IDC_ADMIN);
	m_btOpenAdmin.ShowWindow(SW_HIDE);

	SwitchToCheck();
	SetJettonHide(0);


	//����
#ifdef __SPECIAL___
	if(m_DlgBank.m_hWnd==NULL) m_DlgBank.Create(IDD_BANK_STORAGE,this);
#endif

	return 0;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//��ע��Ϣ
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ȫ����ע
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

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

	//״̬��Ϣ
	m_lCurrentJetton=0L;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=false;
	m_bNeedSetGameRecord=false;
	m_bWinTianMen=false;
	m_bWinHuangMen=false;
	m_bWinXuanMen=false;
	m_bFlashResult=false;
	m_bShowGameResult=false;
	m_enDispatchCardTip=enDispatchCardTip_NULL;

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

	//����б�
	m_ApplyUser.ClearAll();

	//�������
	CleanUserJetton();

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


	CSize Size;

	Size.cy = Size.cy/2;

	//CImageHandle ImageHandle(&m_ImageViewBack[0]);
	int iWidth =756;// m_ImageViewBack[0].GetWidth();
	int iHeight =705;// m_ImageViewBack[0].GetHeight();

	int LifeWidth = nWidth/2-iWidth/2;
	int TopHeight = nHeight/2-iHeight/2;



	for (int i = 0,	 j = 0;i<32;i++)
	{
		if(j ==0)
		{
			m_CarRect[i].top =nHeight/2-276-8;
			m_CarRect[i].left =nWidth/2-362-17+i*81-5;

		}else
		{
			if(j==1)
			{
				m_CarRect[i].top =nHeight/2-276-8+(i-8)*57;
				m_CarRect[i].left =nWidth/2-362-17+8*81-8;

			}else
			{
				if(j==2)
				{
					m_CarRect[i].top =nHeight/2-276-8+8*57;
					m_CarRect[i].left =nWidth/2-362-17+8*81-5-(i-(2*8))*81;

				}else
				{
					if(j==3)
					{
						m_CarRect[i].top =nHeight/2-276-8+8*57-(i-24)*57;
						m_CarRect[i].left =nWidth/2-362-17/*nWidth/2-362-17+8*81-8-(24-(2*8))*81*/;

					}
				}
			}
		}

		j = i/8;
	}


	for (int i = 0;i<8;i++)
	{
		if(i<4)
		{
			m_RectArea[i].top =nHeight/2-276-8+133;
			m_RectArea[i].left =nWidth/2-362-17+i*128-5+106-96-8+128;
			m_RectArea[i].bottom =m_RectArea[i].top+162;
			m_RectArea[i].right  = m_RectArea[i].left+128;

		}else
		{
			m_RectArea[i].top =nHeight/2-276-8+133+162;
			m_RectArea[i].left =nWidth/2-362-17+(i-4)*128-5+106-96-8+128;
			m_RectArea[i].bottom =m_RectArea[i].top+162;
			m_RectArea[i].right  = m_RectArea[i].left+128;

		}
	}

	int ExcursionY=10;
	for (int i = 0;i<AREA_COUNT;i++)
	{
		m_PointJettonNumber[i].SetPoint((m_RectArea[i].right+m_RectArea[i].left)/2, (m_RectArea[i].bottom+m_RectArea[i].top)/2-ExcursionY);
		m_PointJetton[i].SetPoint(m_RectArea[i].left, m_RectArea[i].top);
	}

	m_CardTypePoint[0] =CPoint(nWidth/2+125,nHeight/2-355+135+62); 

	//�ƶ��ؼ�
	HDWP hDwp=BeginDeferWindowPos(33);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	m_ApplyUser.m_viewHandle = m_hWnd;

	//�б�ؼ�
	DeferWindowPos(hDwp,m_ApplyUser,NULL,nWidth/2 + 148-10+50,nHeight/2-291-7-35,178,28,uFlags);
	DeferWindowPos(hDwp,m_btUp,NULL,LifeWidth+245+20+310+85,TopHeight+10+10+40,205/5,16,uFlags);
	DeferWindowPos(hDwp,m_btDown,NULL,LifeWidth+245+20+310+85+40,TopHeight+10+10+40,205/5,16,uFlags);

	m_btBankerStorage.EnableWindow(TRUE);

	m_MeInfoRect.top = TopHeight+10+12;
	m_MeInfoRect.left = LifeWidth+245+20+389;


	m_BaoPosion.top = TopHeight;
	m_BaoPosion.left = nWidth/2;


	//���밴ť
	CRect rcJetton;
	m_btJetton100.GetWindowRect(&rcJetton);
	int nYPos =TopHeight+40+434+5+16-2+100+20;
	int nXPos =  LifeWidth+182+4+1+10 ;
	int nSpace = 15;

	m_TopHeight = TopHeight;
	m_LifeWidth = LifeWidth;

	m_nWinFlagsExcursionX = LifeWidth+152+48+10;	
	m_nWinFlagsExcursionY = TopHeight+545+7+93;

	nSpace = 0;

	DeferWindowPos(hDwp,m_btJetton100,NULL,nXPos,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton1000,NULL,nXPos + nSpace + rcJetton.Width(),nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton10000,NULL,nXPos + nSpace * 2 + rcJetton.Width() * 2,nYPos,0,0,uFlags|SWP_NOSIZE);


	DeferWindowPos(hDwp,m_btJetton100000,NULL,nXPos + nSpace * 3 + rcJetton.Width() * 3,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton1000000,NULL,nXPos + nSpace * 4 + rcJetton.Width() * 4,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton5000000,NULL,nXPos + nSpace * 5 + rcJetton.Width() *5,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btApplyBanker,NULL,m_LifeWidth+609-3-41,m_TopHeight+35-3+22,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btCancelBanker,NULL,m_LifeWidth+609-3-41,m_TopHeight+35-3+22,0,0,uFlags|SWP_NOSIZE);


	DeferWindowPos(hDwp,m_btScoreMoveL,NULL,LifeWidth+123+70,TopHeight+562+21+86,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btScoreMoveR,NULL,LifeWidth+606-63,TopHeight+562+21+86,0,0,uFlags|SWP_NOSIZE);

	//���ư�ť
	DeferWindowPos(hDwp,m_btAutoOpenCard,NULL,LifeWidth+624+20,TopHeight+198,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btOpenCard,NULL,LifeWidth+624+20,TopHeight+198+30,0,0,uFlags|SWP_NOSIZE);
	//������ť
	DeferWindowPos(hDwp,m_btBank,NULL,nWidth/2+290,nHeight/2-340+74,0,0,uFlags|SWP_NOSIZE);
	//������ť
	DeferWindowPos(hDwp,m_btBankerStorage,NULL,LifeWidth+10-1+571,TopHeight+592-12+33,104,400,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btBankerDraw,NULL,LifeWidth+10-1+571,TopHeight+592+50-17+33,0,0,uFlags|SWP_NOSIZE);
	//�����ƶ�
	EndDeferWindowPos(hDwp);



	return;
}
void CGameClientView::SwitchToCheck()
{
	SwithToNormalView();
	return;

	m_btBank.ShowWindow(SW_SHOW);

	m_btAutoOpenCard.ShowWindow(SW_HIDE);
	m_btOpenCard.ShowWindow(SW_HIDE);
	m_ApplyUser.ShowWindow(SW_HIDE);
	m_btApplyBanker.ShowWindow(SW_HIDE);
	m_btCancelBanker.ShowWindow(SW_HIDE);
	m_btBank.ShowWindow(SW_HIDE);
	m_btScoreMoveL.ShowWindow(SW_HIDE);
	m_btScoreMoveR.ShowWindow(SW_HIDE);
	m_btJetton100.ShowWindow(SW_HIDE);
	m_btJetton1000.ShowWindow(SW_HIDE);
	m_btJetton10000.ShowWindow(SW_HIDE);
	m_btJetton50000.ShowWindow(SW_HIDE);
	m_btJetton100000.ShowWindow(SW_HIDE);
	m_btJetton500000.ShowWindow(SW_HIDE);
	m_btJetton1000000.ShowWindow(SW_HIDE);
	m_btJetton5000000.ShowWindow(SW_HIDE);
	m_DrawBack = false;
	SwithToNormalView();


}
void CGameClientView::SwithToNormalView()
{

	m_btBank.ShowWindow(SW_HIDE);

	m_btAutoOpenCard.ShowWindow(SW_HIDE);
	m_btOpenCard.ShowWindow(SW_HIDE);
	m_ApplyUser.ShowWindow(SW_SHOW);
	m_btApplyBanker.ShowWindow(SW_SHOW);
	m_btCancelBanker.ShowWindow(SW_SHOW);
	m_btBank.ShowWindow(SW_HIDE);
	m_btScoreMoveL.ShowWindow(SW_SHOW);
	m_btScoreMoveR.ShowWindow(SW_SHOW);

	m_btJetton100.ShowWindow(SW_SHOW);
	m_btJetton1000.ShowWindow(SW_SHOW);
	m_btJetton10000.ShowWindow(SW_SHOW);
	m_btJetton50000.ShowWindow(SW_HIDE);
	m_btJetton100000.ShowWindow(SW_SHOW);

	m_btJetton500000.ShowWindow(SW_HIDE);
	m_btJetton1000000.ShowWindow(SW_SHOW);
	m_btJetton1000000.ShowWindow(SW_SHOW);
	m_btJetton5000000.ShowWindow(SW_SHOW);


	m_DrawBack = true;

}
//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{

	//��ȡ���
	tagUserData const *pUserData = m_wBankerUser==INVALID_CHAIR ? NULL : GetUserInfo(m_wBankerUser);
	//�滭����
	DrawViewImage(pDC,m_ImageViewFill,enMode_Spread);


	m_ViewBackPng.DrawImage(pDC,(nWidth-m_ViewBackPng.GetWidth())/2,nHeight/2-m_ViewBackPng.GetHeight()/2+120,\
		m_ViewBackPng.GetWidth() ,m_ViewBackPng.GetHeight(),0,0,m_ViewBackPng.GetWidth() ,m_ViewBackPng.GetHeight());

	if(m_blRungingCar)
	{
		m_idb_selPng.DrawImage(pDC ,m_CarRect[m_CarIndex].left,m_CarRect[m_CarIndex].top,\
			m_idb_selPng.GetWidth()/8 ,m_idb_selPng.GetHeight(),(m_CarIndex%8)*(m_idb_selPng.GetWidth()/8),0,m_idb_selPng.GetWidth()/8 ,m_idb_selPng.GetHeight());

	}
	if(m_bFlashrandShow)
	{
		m_idb_selPng.DrawImage(pDC ,m_CarRect[m_CarIndex].left,m_CarRect[m_CarIndex].top,\
			m_idb_selPng.GetWidth()/8 ,m_idb_selPng.GetHeight(),(m_CarIndex%8)*(m_idb_selPng.GetWidth()/8),0,m_idb_selPng.GetWidth()/8 ,m_idb_selPng.GetHeight());

	}
	//��ȡ״̬
	BYTE cbGameStatus=m_pGameClientDlg->GetGameStatus();

	//״̬��ʾ
	CFont static InfoFont;
	InfoFont.CreateFont(-16,0,0,0,400,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("����"));
	CFont * pOldFont=pDC->SelectObject(&InfoFont);
	pDC->SetTextColor(RGB(255,255,0));

	pDC->SelectObject(pOldFont);
	InfoFont.DeleteObject();

	//ʱ����ʾ
	if(m_DrawBack)
	{

		int nTimeFlagWidth = m_ImageTimeFlagPng.GetWidth()/3;
		int nFlagIndex=0;
		if (cbGameStatus==GS_FREE) nFlagIndex=0;
		else if (cbGameStatus==GS_PLACE_JETTON) nFlagIndex=1;
		else if (cbGameStatus==GS_GAME_END) nFlagIndex=2;

		m_ImageTimeFlagPng.DrawImage(pDC,nWidth/2-348+215+30, m_TopHeight+566+15+7+20+5-441+7-16, nTimeFlagWidth, m_ImageTimeFlagPng.GetHeight(),
			nFlagIndex*nTimeFlagWidth,0);

		WORD wUserTimer = GetUserTimer(0);

		if (wUserTimer!=0)
		{
			DrawNumberString(pDC,wUserTimer,m_LifeWidth+424,m_TopHeight+177,true);
		}

	}
	CRect rcDispatchCardTips(m_LifeWidth+612, m_TopHeight+297,m_LifeWidth+612+200, m_TopHeight+297+115);

	//ʤ���߿�
	FlashJettonAreaFrame(nWidth,nHeight,pDC);

	//������Դ
	CImageHandle HandleJettonView(&m_ImageJettonView);
	CSize static SizeJettonItem(m_ImageJettonView.GetWidth()/6,m_ImageJettonView.GetHeight());

	//�滭����
	for (INT i=0;i<AREA_COUNT&&m_DrawBack;i++)
	{
		//��������
		LONGLONG lScoreCount=0L;
		LONGLONG static lScoreJetton[JETTON_COUNT]={100L,1000L,10000L,100000L,1000000L,5000000L};
		int static nJettonViewIndex=0;

		//�滭����
		for (INT_PTR j=0;j<m_JettonInfoArray[i].GetCount();j++)
		{
			//��ȡ��Ϣ
			tagJettonInfo * pJettonInfo=&m_JettonInfoArray[i][j];

			//�ۼ�����
			ASSERT(pJettonInfo->cbJettonIndex<JETTON_COUNT);
			lScoreCount+=lScoreJetton[pJettonInfo->cbJettonIndex];

			nJettonViewIndex=pJettonInfo->cbJettonIndex;

			//�滭����
			m_ImageJettonView.AlphaDrawImage(pDC,pJettonInfo->nXPos+m_PointJetton[i].x,
				pJettonInfo->nYPos+m_PointJetton[i].y,SizeJettonItem.cx,SizeJettonItem.cy,
				nJettonViewIndex*SizeJettonItem.cx,0,RGB(255,0,255));
		}
		//�滭����
		if (lScoreCount>0L)	DrawNumberString(pDC,lScoreCount,m_PointJettonNumber[i].x-5,m_PointJettonNumber[i].y);	
	}

	//�滭ׯ��
	if(m_DrawBack)
	{
		DrawBankerInfo(pDC,nWidth,nHeight);

		//�滭�û�
		DrawMeInfo(pDC,nWidth,nHeight);
	}


	//�л�ׯ��
	if ( m_bShowChangeBanker )
	{
		int	nXPos = nWidth / 2 - 130;
		int	nYPos = nHeight / 2 - 160+120;

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

	//������ʾ
	m_enDispatchCardTip = enDispatchCardTip_NULL;//��ʱ����
	if (enDispatchCardTip_NULL!=m_enDispatchCardTip)
	{
		if (m_ImageDispatchCardTip.IsNull()==false) m_ImageDispatchCardTip.Destroy();
		if (enDispatchCardTip_Dispatch==m_enDispatchCardTip)
			m_ImageDispatchCardTip.SetLoadInfo(IDB_DISPATCH_CARD,AfxGetInstanceHandle(),false);
		else
			m_ImageDispatchCardTip.SetLoadInfo(IDB_CONTINUE_CARD,AfxGetInstanceHandle(),false);
		CImageHandle ImageHandle(&m_ImageDispatchCardTip);
		m_ImageDispatchCardTip.BitBlt(pDC->GetSafeHdc(), (nWidth-m_ImageDispatchCardTip.GetWidth())/2, nHeight/2);
	}

	//�ҵ���ע
	DrawMeJettonNumber(pDC);
	//ʤ����־
	DrawWinFlags(pDC);


	if (m_pGameClientDlg->GetGameStatus()!=GS_FREE)
	{
		m_Out_Bao_y = 6;
	}
	if(m_blMoveFinish&&cbGameStatus==GS_GAME_END)
	{
		ShowGameResult(pDC, nWidth, nHeight);
	}
	if(m_DrawBack)
		DrawMoveInfo(pDC,m_rcTianMen);

	//�滭����
	if(m_bShowBao&&0)
	{
		if(m_bEnableSysBanker==0&&pUserData==NULL)
			;
		else
		{
			if(0<=m_Out_Bao_y&&m_Out_Bao_y<7)
			{
				if(m_Out_Bao_y>=6)
				{
					m_PngPushBox[m_Out_Bao_y].DrawImage(pDC,m_BaoPosion.left-m_PngPushBox[m_Out_Bao_y].GetWidth()/2 ,m_BaoPosion.top-120+6*45-50);

				}else
				{
					m_PngPushBox[m_Out_Bao_y].DrawImage(pDC,m_BaoPosion.left-m_PngPushBox[m_Out_Bao_y].GetWidth()/2 ,m_BaoPosion.top-120+m_Out_Bao_y*20-30);
				}
			}

		}
	}
	if(m_bShowJettonAn&&cbGameStatus!=GS_FREE&&0)
	{
		if(0<=m_bShowJettonIndex&&m_bShowJettonIndex<4)
			m_PngShowJetton[m_bShowJettonIndex].DrawImage(pDC,m_BaoPosion.left-m_PngShowJetton[m_bShowJettonIndex].GetWidth()/2 ,m_BaoPosion.top-120+6*20-30);
	}
	if(m_bShowLeaveHandleAn&&cbGameStatus==GS_GAME_END&&0)
	{
		if(0<=m_bShowLeaveHandleIndex&&m_bShowLeaveHandleIndex<4)
			m_PngShowLeaveHandle[m_bShowLeaveHandleIndex].DrawImage(pDC,m_BaoPosion.left-m_PngShowLeaveHandle[m_bShowLeaveHandleIndex].GetWidth()/2 ,m_BaoPosion.top-120+6*20-30);

	}
	return;
}

//������Ϣ
void CGameClientView::SetMeMaxScore(LONGLONG lMeMaxScore)
{
	if (m_lMeMaxScore!=lMeMaxScore)
	{
		//���ñ���
		m_lMeMaxScore=lMeMaxScore;
	}

	return;
}

//�����ע
void CGameClientView::SetAreaLimitScore(LONGLONG lAreaLimitScore)
{
	if ( m_lAreaLimitScore!= lAreaLimitScore )
	{
		//���ñ���
		m_lAreaLimitScore=lAreaLimitScore;
	}
}

//���ó���
void CGameClientView::SetCurrentJetton(LONGLONG lCurrentJetton)
{
	//���ñ���
	ASSERT(lCurrentJetton>=0L);
	m_lCurrentJetton=lCurrentJetton;
	if(lCurrentJetton==0)
	{
		SetJettonHide(0);
	}
	return;
}


//��ʷ��¼
void CGameClientView::SetGameHistory(BYTE *bcResulte)
{
	//��������
	BYTE bcResulteTmp[AREA_COUNT];
	memcpy(bcResulteTmp,bcResulte,AREA_COUNT);
	tagClientGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];


	for (int i = 1;i<=AREA_COUNT;i++)
	{

		if(bcResulteTmp[i-1]>0)
		{
			GameRecord.enOperateMen[i]=enOperateResult_Win;

		}else
		{
			GameRecord.enOperateMen[i]=enOperateResult_Lost;

		}
		/*if (0==m_lUserJettonScore[i]) GameRecord.enOperateMen[i]=enOperateResult_NULL;
		else if (m_lUserJettonScore[i] > 0 && (bcResulte[i-1]==4)) GameRecord.enOperateMen[i]=enOperateResult_Win;
		else if (m_lUserJettonScore[i] > 0 && (0==bcResulte[i-1])) GameRecord.enOperateMen[i]=enOperateResult_Lost;*/

	}
	//�ƶ��±�
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst )
	{
		m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
		if ( m_nScoreHead < m_nRecordFirst ) m_nScoreHead = m_nRecordFirst;
	}

	int nHistoryCount = (m_nRecordLast - m_nRecordFirst + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
	if ( MAX_FALG_COUNT < nHistoryCount ) m_btScoreMoveR.EnableWindow(TRUE);

	//�Ƶ����¼�¼
	if ( MAX_FALG_COUNT < nHistoryCount )
	{
		m_nScoreHead = (m_nRecordLast - MAX_FALG_COUNT + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
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
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ȫ����ע
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));


	ZeroMemory(m_JettonQue,sizeof(m_JettonQue));


	ZeroMemory(m_JettonQueArea,sizeof(m_JettonQueArea));




	m_JettonQueIndex = 0;





	//���½���
	UpdateGameView(NULL);

	return;
}

//������ע
void CGameClientView::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=AREA_COUNT);
	if (cbViewIndex>AREA_COUNT) return;

	m_lUserJettonScore[cbViewIndex]=lJettonCount;

	bool blHave = false;
	for (int i = 0;i<18;i++)
	{
		if(m_JettonQueArea[i]==cbViewIndex&&m_JettonQueIndex>i)
		{
			m_JettonQue[i] = m_lUserJettonScore[cbViewIndex];

			blHave= true;
			break;

		}
	}
	if(blHave==false)
	{
		if(m_JettonQueIndex>COUNT_DRAWINFO-1)
		{
			for (int i = 0;i<COUNT_DRAWINFO-1;i++)
			{
				m_JettonQueArea[i]= m_JettonQueArea[i+1];
				m_JettonQue[i] = m_JettonQue[i+1];

			}
			m_JettonQue[COUNT_DRAWINFO-1]=m_lUserJettonScore[cbViewIndex];
			m_JettonQueArea[COUNT_DRAWINFO-1] = cbViewIndex;
		}else 
		{
			m_JettonQueArea[m_JettonQueIndex]= cbViewIndex;
			m_JettonQue[m_JettonQueIndex++] = m_lUserJettonScore[cbViewIndex];

		}

		//m_JettonQueIndex = m_JettonQueIndex%16;

	}





	//���½���
	UpdateGameView(NULL);
}

//�����˿�
void CGameClientView::SetCardInfo(BYTE cbTableCardArray[1][1])
{
	if (cbTableCardArray!=NULL)
	{
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
void CGameClientView::PlaceUserJetton(BYTE cbViewIndex, LONGLONG lScoreCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=AREA_COUNT);
	if (cbViewIndex>AREA_COUNT) return;

	//��������
	bool bPlaceJetton = false;
	LONGLONG lScoreIndex[JETTON_COUNT] = {100L,1000L,10000L,100000L,1000000L,5000000L};

	//�߿���
	int nFrameWidth=0, nFrameHeight=0;
	int nBorderWidth=6;

	m_lAllJettonScore[cbViewIndex] += lScoreCount;
	nFrameWidth = m_RectArea [cbViewIndex-1].right-m_RectArea [cbViewIndex-1].left;
	nFrameHeight = m_RectArea[cbViewIndex-1].bottom-m_RectArea [cbViewIndex-1].top;


	//�����ж�
	bool bAddJetton = lScoreCount>0?true:false;

	if( lScoreCount < 0)
	{
		lScoreCount = -lScoreCount;
	}

	//���ӳ���
	for (BYTE i=0;i<CountArray(lScoreIndex);i++)
	{
		//������Ŀ
		BYTE cbScoreIndex = JETTON_COUNT-i-1;
		LONGLONG lCellCount = lScoreCount / lScoreIndex[cbScoreIndex];

		//�������
		if (lCellCount==0L) continue;

		//�������
		for (LONGLONG j=0;j<lCellCount;j++)
		{
			if (true == bAddJetton)
			{
				//�������
				tagJettonInfo JettonInfo;
				int nJettonSize=55;
				JettonInfo.cbJettonIndex=cbScoreIndex;
				int iWSize = nFrameWidth-nJettonSize-5;
				int iHSize =  nFrameHeight-nJettonSize-40;
				JettonInfo.nXPos=rand()%(iWSize);
				JettonInfo.nYPos=rand()%(iHSize);

				//��������
				bPlaceJetton = true;
				m_JettonInfoArray[cbViewIndex-1].Add(JettonInfo);
			}
			else
			{
				for (int nIndex=0; nIndex<m_JettonInfoArray[cbViewIndex-1].GetCount(); ++nIndex)
				{
					//�Ƴ��ж�
					tagJettonInfo &JettonInfo=m_JettonInfoArray[cbViewIndex-1][nIndex];
					if (JettonInfo.cbJettonIndex==cbScoreIndex)
					{
						m_JettonInfoArray[cbViewIndex-1].RemoveAt(nIndex);
						break;
					}
				}
			}
		}

		//������Ŀ
		lScoreCount -= lCellCount*lScoreIndex[cbScoreIndex];
	}

	//���½���
	if (bPlaceJetton==true) 
		UpdateGameView(NULL);

	return;
}

//��������ע
void CGameClientView::AndroidBet(BYTE cbViewIndex, LONGLONG lScoreCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=AREA_COUNT);
	if (cbViewIndex>AREA_COUNT) 
		return;

	if ( lScoreCount <= 0L )
		return;

	tagAndroidBet Androi;
	Androi.cbJettonArea = cbViewIndex;
	Androi.lJettonScore = lScoreCount;
	m_ArrayAndroid.Add(Androi);
	SetTimer(IDI_ANDROID_BET,100,NULL);
	int nHaveCount = 0;
	for ( int i = 0 ; i < m_ArrayAndroid.GetCount(); ++i)
	{
		if(m_ArrayAndroid[i].lJettonScore > 0)
			nHaveCount++;
	}
	UINT nElapse = 0;
	if ( nHaveCount <= 1 )
		nElapse = 260;
	else if ( nHaveCount <= 2 )
		nElapse = 160;
	else
		nElapse = 100;

	SetTimer(IDI_ANDROID_BET+m_ArrayAndroid.GetCount(),nElapse,NULL);
}

//���ֳɼ�
void CGameClientView::SetCurGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore, LONGLONG lGameRevenue)
{
	m_lMeCurGameScore=lMeCurGameScore;			
	m_lMeCurGameReturnScore=lMeCurGameReturnScore;			
	m_lBankerCurGameScore=lBankerCurGameScore;			
	m_lGameRevenue=lGameRevenue;					
}

//����ʤ��
void CGameClientView::SetWinnerSide(bool blWin[], bool bSet)
{	
	//����ʱ��
	for (int i= 0;i<AREA_COUNT;i++){

		m_bWinFlag[i]=blWin[i];
	}
	if (true==bSet)
	{
		//���ö�ʱ��
		SetTimer(IDI_FLASH_WINNER,500,NULL);

		//ȫʤ�ж�
		bool blWinAll = true;

		for (int i= 0;i<AREA_COUNT;i++){

			if(m_bWinFlag[i]==true){
				blWinAll = false;
			}
		}
		if (blWinAll){
			//������Դ
			HINSTANCE hInstance=AfxGetInstanceHandle();
		}
	}
	else 
	{
		//�����ʱ��
		KillTimer(IDI_FLASH_WINNER);

		//ȫʤ�ж�
		bool blWinAll = true;

		for (int i= 0;i<AREA_COUNT;i++){

			if(m_bWinFlag[i]==true){
				blWinAll = false;
			}
		}
		if (blWinAll){
		}
	}

	//���ñ���
	m_bFlashResult=bSet;
	m_bShowGameResult=bSet;
	m_cbAreaFlash=0xFF;

	//���½���
	UpdateGameView(NULL);

	return;
}

//��ȡ����
BYTE CGameClientView::GetJettonArea(CPoint MousePoint)
{
	for (int i = 0;i<AREA_COUNT;i++)
	{
		if(m_RectArea[i].PtInRect(MousePoint))
		{
			return i+1;
		}
	}
	return 0xFF;
}
//�滭����
void CGameClientView::DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos, bool blTimer,bool bMeScore)
{
	//������Դ
	CImageHandle HandleScoreNumber(&m_ImageScoreNumber);
	CImageHandle HandleMeScoreNumber(&m_ImageMeScoreNumber);
	CSize SizeScoreNumber(m_ImageScoreNumberPng.GetWidth()/11,m_ImageScoreNumberPng.GetHeight());

	if ( bMeScore ) SizeScoreNumber.SetSize(m_ImageMeScoreNumberPng.GetWidth()/11, m_ImageMeScoreNumberPng.GetHeight());

	if(blTimer)
	{
		SizeScoreNumber.SetSize(m_TimerCount_png.GetWidth()/10, m_TimerCount_png.GetHeight());
	}

	//������Ŀ
	LONGLONG lNumberCount=0;
	LONGLONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	LONGLONG tmpNum= lNumberCount+lNumberCount/4;

	//λ�ö���
	INT nYDrawPos=nYPos-SizeScoreNumber.cy/2;
	INT nXDrawPos=(INT)(nXPos+tmpNum*SizeScoreNumber.cx/2-SizeScoreNumber.cx);

	if (!blTimer)
		nXDrawPos = (INT)(nXPos+(SizeScoreNumber.cx-2)*lNumberCount/2+(SizeScoreNumber.cx-5)*(lNumberCount>3?0:(lNumberCount-1)/3)/2);

	//�滭����
	for (LONGLONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		if(i!=0&&i%3==0)
		{
			if(blTimer)
			{
			}
			else
			{
				nXDrawPos += 5;
				if ( bMeScore )
				{
					m_ImageMeScoreNumberPng.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
						10*SizeScoreNumber.cx,0);
				}
				else
				{
					m_ImageScoreNumberPng.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
						10*SizeScoreNumber.cx,0);
				}
				nXDrawPos-=(SizeScoreNumber.cx-2);
			}
		}
		LONG lCellNumber=(LONG)(lNumber%10);
		if(blTimer)
		{
			m_TimerCount_png.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
				lCellNumber*SizeScoreNumber.cx,0);

		}
		else
		{
			if ( bMeScore )
			{
				m_ImageMeScoreNumberPng.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
					lCellNumber*SizeScoreNumber.cx,0);
			}
			else
			{
				m_ImageScoreNumberPng.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
					lCellNumber*SizeScoreNumber.cx,0);
			}

		}
		//���ñ���
		lNumber/=10;

		if (blTimer)
			nXDrawPos-=SizeScoreNumber.cx;
		else
			nXDrawPos-=(SizeScoreNumber.cx-2);
	};

	return;
}

//�滭����
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos)
{
	LONGLONG lTmpNumber = lNumber;
	CString strNumber;
	CString strTmpNumber1;
	CString strTmpNumber2;
	bool blfirst = true;
	bool bLongNum = false;
	int nNumberCount = 0;

	strTmpNumber1.Empty();
	strTmpNumber2.Empty();
	strNumber.Empty();

	if ( lNumber == 0 )
		strNumber=TEXT("0");

	if ( lNumber < 0 ) 
		lNumber =- lNumber;

	if( lNumber >= 100 )
		bLongNum = true;

	while(lNumber > 0)
	{
		strTmpNumber1.Format(TEXT("%I64d"),lNumber%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if ( nNumberCount == 3 )
		{
			if(blfirst)
			{
				strTmpNumber2 += (TEXT("") +strNumber);
				blfirst = false;
			}
			else
			{
				strTmpNumber2 += (TEXT(",") +strNumber);
			}

			strNumber = strTmpNumber2;
			nNumberCount = 0;
			strTmpNumber2 = TEXT("");
		}
		lNumber /= 10;
	}

	if ( strTmpNumber2.IsEmpty() == FALSE )
	{
		if( bLongNum )
			strTmpNumber2 += (TEXT(",") +strNumber);
		else
			strTmpNumber2 += strNumber;

		strNumber = strTmpNumber2;
	}

	if ( lTmpNumber < 0 ) 
		strNumber = TEXT("-") + strNumber;
	//�������
	pDC->TextOut(nXPos,nYPos,strNumber);
}
//�滭����
CString CGameClientView::NumberStringWithSpace(LONGLONG lNumber)
{

	CString static strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;
	strTmpNumber1.Empty();
	strTmpNumber2.Empty();
	strNumber.Empty();
	if (lNumber==0) strNumber=TEXT("0");
	int nNumberCount=0;
	LONGLONG lTmpNumber=lNumber;
	if (lNumber<0) lNumber=-lNumber;
	bool blfirst = true;
	while (lNumber>0)
	{
		strTmpNumber1.Format(TEXT("%ld"),lNumber%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if (nNumberCount==3)
		{
			if(blfirst)
			{
				strTmpNumber2 +=strNumber;
				blfirst = false;

			}else
			{
				strTmpNumber2 += (TEXT(",") +strNumber);
			}

			strNumber=strTmpNumber2;
			nNumberCount=0;
			strTmpNumber2=TEXT("");
		}
		lNumber/=10;
	}

	if (strTmpNumber2.IsEmpty()==FALSE)
	{
		if(blfirst==false)
			strTmpNumber2 += (TEXT(",") +strNumber);
		else
			strTmpNumber2 += strNumber;

		strNumber=strTmpNumber2;
	}

	if (lTmpNumber<0) strNumber=TEXT("-")+strNumber;

	return  strNumber;


}
//�滭����
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, CRect rcRect, INT nFormat)
{
	LONGLONG lTmpNumber = lNumber;
	CString strNumber;
	CString strTmpNumber1;
	CString strTmpNumber2;
	bool blfirst = true;
	bool bLongNum = false;
	int nNumberCount = 0;

	strTmpNumber1.Empty();
	strTmpNumber2.Empty();
	strNumber.Empty();

	if ( lNumber == 0 )
		strNumber=TEXT("0");

	if ( lNumber < 0 ) 
		lNumber =- lNumber;

	if( lNumber >= 100 )
		bLongNum = true;

	while(lNumber > 0)
	{
		strTmpNumber1.Format(TEXT("%I64d"),lNumber%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if ( nNumberCount == 3 )
		{
			if(blfirst)
			{
				strTmpNumber2 += (TEXT("") +strNumber);
				blfirst = false;
			}
			else
			{
				strTmpNumber2 += (TEXT(",") +strNumber);
			}

			strNumber = strTmpNumber2;
			nNumberCount = 0;
			strTmpNumber2 = TEXT("");
		}
		lNumber /= 10;
	}

	if ( strTmpNumber2.IsEmpty() == FALSE )
	{
		if( bLongNum )
			strTmpNumber2 += (TEXT(",") +strNumber);
		else
			strTmpNumber2 += strNumber;

		strNumber = strTmpNumber2;
	}

	if ( lTmpNumber < 0 ) 
		strNumber = TEXT("-") + strNumber;
	//�������
	if (nFormat==-1) pDC->DrawText(strNumber,rcRect,DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);
	else pDC->DrawText(strNumber,rcRect,nFormat);
}
void CGameClientView::SetMoveCardTimer()
{
	KillTimer(IDI_POSTCARD);
	SetTimer(IDI_POSTCARD,300,NULL);
	SetTimer(IDI_DISPATCH_CARD,5000,NULL);

}
void CGameClientView::KillCardTime()
{
	KillTimer(IDI_FLASH_WINNER);
	KillTimer(IDI_POSTCARD);
	KillTimer(IDI_OPENCARD);
	KillTimer(IDI_DISPATCH_CARD);
	KillTimer(IDI_SHOWDISPATCH_CARD_TIP);

}
void CGameClientView::StartRunCar(int iTimer)
{
	m_bFlashrandShow = false;
	KillTimer(IDI_FLASH_RAND_SIDE);
	KillTimer(IDI_FLASH_CARD);
	KillTimer(IDI_SHOW_CARD_RESULT);
	iTimerStep = 400;
	SetTimer(IDI_FLASH_CARD,iTimer,NULL);
	ifirstTimer = 0;
	iOpenSide = m_cbTableCardArray[0][0];
	iTotoalRun = iOpenSide+32*3-2;
	iRunIndex = 0;
	m_CarIndex = 1;
	m_blRungingCar = true;
	SetTimer(IDI_SHOW_CARD_RESULT,12*1000,NULL);


}
void CGameClientView::RuningCar(int iTimer)
{
	if(iRunIndex<10)
	{
		iTimerStep-=43;

	}
	if(iRunIndex >= iTotoalRun-15)
	{
		iTimerStep+=47 /*8*(iRunIndex-iTotoalRun+15)*/;
	}
	if(iRunIndex==iTotoalRun)
	{
		KillTimer(IDI_FLASH_CARD);
		KillTimer(IDI_SHOW_CARD_RESULT);
		m_blRungingCar = false;
		m_blMoveFinish = true;
		//���ö�ʱ��
		this->DispatchCard();
		FinishDispatchCard();
		return ;

	}
	if(iTimerStep<0)
	{
		return ;
	}
	KillTimer(IDI_FLASH_CARD);
	SetTimer(IDI_FLASH_CARD,iTimer,NULL);

}
void CGameClientView::FlashWinOpenSide()
{

}
void CGameClientView::StartRandShowSide()
{
	KillTimer(IDI_FLASH_RAND_SIDE);
	iTimerStep = 100;
	m_bFlashrandShow = true;
	SetTimer(IDI_FLASH_RAND_SIDE,iTimerStep,NULL);


}
//��ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{
	if(IDI_FLASH_CARD == nIDEvent)
	{
		m_CarIndex = (m_CarIndex+1)%32;
		iRunIndex++;
		RuningCar(iTimerStep);
		::AfxGetMainWnd()->SendMessage(IDM_SOUND,3,3);

		//���½���
		UpdateGameView(NULL);
	}
	if(IDI_SHOW_CARD_RESULT==nIDEvent)
	{
		for (int i = iRunIndex;i<iTotoalRun;i++)
		{
			m_CarIndex = (m_CarIndex+1)%32;
			iRunIndex++;
			if(iRunIndex==iTotoalRun)
			{
				KillTimer(IDI_FLASH_CARD);
				KillTimer(IDI_SHOW_CARD_RESULT);
				m_blRungingCar = false;
				m_blMoveFinish = true;
				//���ö�ʱ��
				this->DispatchCard();
				FinishDispatchCard();
				return ;
			}
		}
	}

	if(IDI_FLASH_RAND_SIDE ==nIDEvent )
	{
		m_bFlashrandShow=!m_bFlashrandShow;
		m_CarIndex = rand()%32;
		//���½���
		UpdateGameView(NULL);
	}
	//����ʤ��
	if (nIDEvent==IDI_FLASH_WINNER)
	{
		//���ñ���
		m_bFlashResult=!m_bFlashResult;

		//���½���
		UpdateGameView(NULL);
		return;
	}
	//�ֻ�ׯ��
	else if ( nIDEvent == IDI_SHOW_CHANGE_BANKER )
	{
		ShowChangeBanker( false );
		return;
	}
	else if (nIDEvent==IDI_DISPATCH_CARD)
	{
		return;
	}
	else if (IDI_SHOWDISPATCH_CARD_TIP==nIDEvent)
	{
		SetDispatchCardTip(enDispatchCardTip_NULL);
	}
	else if ( nIDEvent == IDI_ANDROID_BET )
	{
		//���½���
		UpdateGameView(NULL);
		return;
	}
	else if ( nIDEvent >= (UINT)(IDI_ANDROID_BET + 1) && nIDEvent < (UINT)(IDI_ANDROID_BET + m_ArrayAndroid.GetCount() + 1) )
	{
		INT_PTR Index = nIDEvent - IDI_ANDROID_BET - 1;
		if (Index < 0 || Index >= m_ArrayAndroid.GetCount())
		{
			ASSERT(FALSE);
			KillTimer(nIDEvent);
			return;
		}

		if ( m_ArrayAndroid[Index].lJettonScore > 0 )
		{
			LONGLONG lScoreIndex[] = {5000000L,1000000L,100000L,10000L,1000L,100L};
			BYTE cbViewIndex = m_ArrayAndroid[Index].cbJettonArea;

			//���ӳ���
			for (BYTE i=0;i<CountArray(lScoreIndex);i++)
			{
				if(  lScoreIndex[i] > m_lRobotMaxJetton)
					continue;

				if ( m_ArrayAndroid[Index].lJettonScore >= lScoreIndex[i] )
				{
					m_ArrayAndroid[Index].lJettonScore -= lScoreIndex[i];
					m_lAllJettonScore[cbViewIndex] += lScoreIndex[i];

					tagJettonInfo JettonInfo;
					int iWSize = m_RectArea[cbViewIndex-1].right - m_RectArea[cbViewIndex-1].left - 60;
					int iHSize = m_RectArea[cbViewIndex-1].bottom - m_RectArea[cbViewIndex-1].top - 95;
					JettonInfo.nXPos=rand()%(iWSize);
					JettonInfo.nYPos=rand()%(iHSize);
					JettonInfo.cbJettonIndex = JETTON_COUNT - i - 1;

					//��������
					m_JettonInfoArray[cbViewIndex-1].Add(JettonInfo);
					//��������
					AfxGetMainWnd()->PostMessage(IDM_SOUND,7,7);
					break;
				}
			}
		}
		return;
	}

	__super::OnTimer(nIDEvent);
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	if (m_lCurrentJetton!=0L)
	{
		int iTimer = 1;
		//��ע����
		BYTE cbJettonArea=GetJettonArea(Point);

		//�����ע
		LONGLONG lMaxJettonScore=GetUserMaxJetton(cbJettonArea);

		if((m_lAllJettonScore[cbJettonArea]+m_lCurrentJetton)>m_lAreaLimitScore)
		{		
			return ;
		}

		//�Ϸ��ж�
		if (lMaxJettonScore < m_lCurrentJetton)
		{
			SetJettonHide(0);
			return ;
		}

		//������Ϣ
		if (cbJettonArea!=0xFF) AfxGetMainWnd()->SendMessage(IDM_PLACE_JETTON,cbJettonArea,(LPARAM)(&m_lCurrentJetton));
	}
	UpdateGameView(NULL);

	__super::OnLButtonDown(nFlags,Point);
}
void CGameClientView::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);
}
//���������ؼ���������Ϣ
LRESULT CGameClientView::OnViLBtUp(WPARAM wParam, LPARAM lParam)
{
	CPoint *pPoint = (CPoint*)lParam;

	ScreenToClient(pPoint);
	OnLButtonUp(1,*pPoint);
	return 1;
}
void CGameClientView::SetEnablePlaceJetton(bool bEnablePlaceJetton)
{
	m_bEnablePlaceJetton = bEnablePlaceJetton;
}
void CGameClientView::UpdataJettonButton()
{
	if(m_CurArea==0xFF||m_bEnablePlaceJetton == false)
	{
		return ;
	}
	//�������
	LONGLONG lCurrentJetton=GetCurrentJetton();
	LONGLONG lLeaveScore=m_lMeMaxScore;
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lLeaveScore -= m_lUserJettonScore[nAreaIndex];
	//�����ע
	LONGLONG lUserMaxJetton = 0;

	lUserMaxJetton = GetUserMaxJetton(m_CurArea);

	lLeaveScore = min((lLeaveScore),lUserMaxJetton); //�û����·� �����ֱȽ� �������屶 


	lCurrentJetton = m_lLastJetton ;

	//���ù��
	if (lCurrentJetton>lLeaveScore)
	{
		if (lLeaveScore>=5000000L) SetCurrentJetton(5000000L);
		else if (lLeaveScore>=1000000L) SetCurrentJetton(1000000L);
		else if (lLeaveScore>=100000L) SetCurrentJetton(100000L);
		else if (lLeaveScore>=10000L) SetCurrentJetton(10000L);
		else if (lLeaveScore>=1000L) SetCurrentJetton(1000L);
		else if (lLeaveScore>=100L) SetCurrentJetton(100L);
		else SetCurrentJetton(0L);
	}
	else
	{
		SetCurrentJetton(m_lLastJetton);
	}
}
//����ƶ���Ϣ
void CGameClientView::OnMouseMove(UINT nFlags, CPoint point)
{
	return __super::OnMouseMove(nFlags,point);
}
//�����Ϣ
void CGameClientView::OnRButtonDown(UINT nFlags, CPoint Point)
{
	//���ñ���
	m_lCurrentJetton=0L;

	if (m_pGameClientDlg->GetGameStatus()!=GS_GAME_END && m_cbAreaFlash!=0xFF)
	{
		m_cbAreaFlash=0xFF;
		SetJettonHide(0);
		UpdateGameView(NULL);
	}
	m_lLastJetton = 0L;

	__super::OnLButtonDown(nFlags,Point);
}
void CGameClientView::SetJettonHide(int ID)
{
	CRgnButton *btJetton[]={NULL,&m_btJetton100,&m_btJetton1000,&m_btJetton10000,&m_btJetton100000,&m_btJetton1000000,&m_btJetton5000000};

	if(1<=ID&&ID<=6)
	{
		for (int i = 1;i<=6;i++)
		{
			if(i!=ID)
				btJetton[i]->ShowWindow(SW_SHOW);
		}
		btJetton[ID]->ShowWindow(SW_HIDE);

		OutputDebugString("����ID\r\n");

	}else
	{
		for (int i = 1;i<=6;i++)
		{
			btJetton[i]->ShowWindow(SW_SHOW);	
		}
	}
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
		if ( m_cbAreaFlash != cbJettonArea )
		{
			m_cbAreaFlash = cbJettonArea;
			UpdateGameView(NULL);
		}

		bool bButtonArea = false;
		CRgnButton* pRgnButton[] = { &m_btJetton100, &m_btJetton1000, &m_btJetton10000, &m_btJetton100000, &m_btJetton1000000, &m_btJetton5000000};
		for (int i = 0 ; i < CountArray(pRgnButton) && cbJettonArea==0xFF; ++i)
		{
			CRgn rgn;
			CRect rect;
			rgn.CreateRectRgn(0,0,0,0);
			pRgnButton[i]->GetWindowRgn(rgn);
			pRgnButton[i]->GetWindowRect(rect);
			ScreenToClient(&rect);
			rgn.OffsetRgn(CPoint(rect.left,rect.top));
			if (rgn.PtInRegion(MousePoint))
			{
				rgn.DeleteObject();
				bButtonArea = true;
				break;
			}
			rgn.DeleteObject();
		}

		//�����ж�
		if ( cbJettonArea==0xFF && !bButtonArea)
		{
			SetCursor(LoadCursor(NULL,IDC_ARROW));
			SetJettonHide(0);
			return TRUE;
		}

		//�����ע
		LONGLONG lMaxJettonScore=GetUserMaxJetton(cbJettonArea);

		//�Ϸ��ж�
		int iTimer = 1;

		if((m_lAllJettonScore[cbJettonArea]+m_lCurrentJetton)>m_lAreaLimitScore  && !bButtonArea)
		{
			SetJettonHide(0);
			SetCursor(LoadCursor(NULL,IDC_NO));
			return TRUE;

		}
		if (lMaxJettonScore< m_lCurrentJetton*iTimer && !bButtonArea)
		{
			SetJettonHide(0);
			SetCursor(LoadCursor(NULL,IDC_NO));
			return TRUE;
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
			UpdateGameView(NULL);
		}
	}
	else
	{
		ClearAreaFlash();
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
void CGameClientView::SetBankerInfo(DWORD dwBankerUserID, LONGLONG lBankerScore) 
{
	//ׯ�����Ӻ�
	WORD wBankerUser=INVALID_CHAIR;

	//�������Ӻ�
	if (0!=dwBankerUserID)
	{
		for (WORD wChairID=0; wChairID<MAX_CHAIR; ++wChairID)
		{
			tagUserData const *pUserData=GetUserInfo(wChairID);
			if (NULL!=pUserData && dwBankerUserID==pUserData->dwUserID)
			{
				wBankerUser=wChairID;
				break;
			}
		}
	}

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
void CGameClientView::DrawMoveInfo(CDC* pDC,CRect rcRect)
{
	return ;
}
//�滭��ʶ
void CGameClientView::DrawWinFlags(CDC * pDC)
{
	//�ǿ��ж�
	//m_nRecordLast = 14;
	if (m_nRecordLast==m_nRecordFirst) return;

	//��Դ����

	int nIndex = m_nScoreHead;
	COLORREF static clrOld ;

	clrOld = pDC->SetTextColor(RGB(52,116,23));
	int nDrawCount=0;
	while ( nIndex != m_nRecordLast && ( m_nRecordLast!=m_nRecordFirst ) && nDrawCount < MAX_FALG_COUNT )
	{
		//ʤ����ʶ
		tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[nIndex];



		for (int i=1; i<=1; ++i)
		{
			//λ�ñ���
			int static nYPos=0,nXPos=0;
			nYPos=m_nWinFlagsExcursionY+i*27-i*1;
			nXPos=m_nWinFlagsExcursionX + ((nIndex - m_nScoreHead + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY) * (37);

			//ʤ����ʶ
			int static nFlagsIndex=0;

			for (int i = 1;i<=AREA_COUNT;i++)
			{

				if(ClientGameRecord.enOperateMen[i]==enOperateResult_Win)
				{
					nFlagsIndex = (i-1)*2;
					if(i==5)
					{
						nFlagsIndex = 1;

					}
					if(i==6)
					{
						nFlagsIndex = 3;

					}
					if(i==7)
					{
						nFlagsIndex = 5;

					}
					if(i==8)
					{
						nFlagsIndex = 7;

					}

				}
			}
			{
				//�滭��ʶ
				m_ImageWinFlags.DrawImage( pDC, nXPos, nYPos, m_ImageWinFlags.GetWidth()/8, 
					m_ImageWinFlags.GetHeight(),m_ImageWinFlags.GetWidth()/8 * nFlagsIndex, 0);

			}

		}

		//�ƶ��±�
		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
		nDrawCount++;
	}
	pDC->SetTextColor(clrOld);
}
//�ֹ�����
afx_msg void CGameClientView::OnOpenCard()
{
	::AfxGetMainWnd()->SendMessage(IDM_OPEN_CARD,0,0);

}
//�Զ�����
afx_msg void CGameClientView::OnAutoOpenCard()
{
	::AfxGetMainWnd()->SendMessage(IDM_AUTO_OPEN_CARD,0,0);

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
	if ( nHistoryCount == MAX_FALG_COUNT ) return;

	m_nScoreHead = ( m_nScoreHead + 1 ) % MAX_SCORE_HISTORY;
	if ( nHistoryCount-1 == MAX_FALG_COUNT ) m_btScoreMoveR.EnableWindow(FALSE);

	m_btScoreMoveL.EnableWindow(TRUE);

	//���½���
	UpdateGameView(NULL);
}

//��ʾ���
void CGameClientView::ShowGameResult(CDC *pDC, int nWidth, int nHeight)
{

	if (false==m_bShowGameResult) return;

	int	nXPos = nWidth / 2 - 200+20+30;
	int	nYPos = nHeight / 2 - 208;
	m_pngGameEnd.DrawImage( pDC, nXPos+2-20-30, nYPos+60,
		m_pngGameEnd.GetWidth(), m_pngGameEnd.GetHeight(),0, 0);
	pDC->SetTextColor(RGB(255,255,255));


	CRect rcMeWinScore, rcMeReturnScore;
	rcMeWinScore.left = nXPos+2 + 40+30;
	rcMeWinScore.top = nYPos+70 + 32+10+10;
	rcMeWinScore.right = rcMeWinScore.left + 111;
	rcMeWinScore.bottom = rcMeWinScore.top + 34;

	rcMeReturnScore.left = nXPos+2 + 150+50;
	rcMeReturnScore.top = nYPos+70 + 32+10+10;
	rcMeReturnScore.right = rcMeReturnScore.left + 111;
	rcMeReturnScore.bottom = rcMeReturnScore.top + 34;

	CString strMeGameScore, strMeReturnScore;
	DrawNumberStringWithSpace(pDC,m_lMeCurGameScore,rcMeWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	DrawNumberStringWithSpace(pDC,m_lMeCurGameReturnScore,rcMeReturnScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	CRect rcBankerWinScore;
	rcBankerWinScore.left = nXPos+2 + 40+30;
	rcBankerWinScore.top =  nYPos+70 + 32+10+35+10;
	rcBankerWinScore.right = rcBankerWinScore.left + 111;
	rcBankerWinScore.bottom = rcBankerWinScore.top + 34;

	CString strBankerCurGameScore;
	strBankerCurGameScore.Format(TEXT("%I64d"), m_lBankerCurGameScore);
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
void   CGameClientView::GetAllWinArea(BYTE bcWinArea[],BYTE bcAreaCount,BYTE InArea)
{
	if (InArea==0xFF)
	{
		return ;
	}
	ZeroMemory(bcWinArea,bcAreaCount);


	LONGLONG lMaxSocre = 0;

	for (int i = 0;i<32;i++)
	{
		BYTE bcOutCadDataWin[AREA_COUNT];
		BYTE bcData[1];
		bcData[0]=i+1;
		m_GameLogic.GetCardType(bcData,1,bcOutCadDataWin);
		for (int j= 0;j<AREA_COUNT;j++)
		{

			if(bcOutCadDataWin[j]>0&&j==InArea-1)
			{
				LONGLONG Score = 0; 
				for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
				{
					if(bcOutCadDataWin[nAreaIndex-1]>1)
					{
						Score += m_lAllJettonScore[nAreaIndex]*(bcOutCadDataWin[nAreaIndex-1]);
					}
				}
				if(Score>=lMaxSocre)
				{
					lMaxSocre = Score;
					CopyMemory(bcWinArea,bcOutCadDataWin,bcAreaCount);

				}
				break;
			}
		}
	}
}
//�����ע
LONGLONG CGameClientView::GetUserMaxJetton(BYTE cbJettonArea)
{
	if (cbJettonArea==0xFF)
		return 0;

	//����ע��
	LONGLONG lNowJetton = 0;
	ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lNowJetton += m_lUserJettonScore[nAreaIndex];
	//ׯ�ҽ��
	LONGLONG lBankerScore = 0x7fffffffffffffff;
	if (m_wBankerUser!=INVALID_CHAIR)
		lBankerScore = m_lBankerScore;

	BYTE bcWinArea[AREA_COUNT];
	LONGLONG LosScore = 0;
	LONGLONG WinScore = 0;

	GetAllWinArea(bcWinArea,AREA_COUNT,cbJettonArea);

	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
	{
		if(bcWinArea[nAreaIndex-1]>1)
		{
			LosScore+=m_lAllJettonScore[nAreaIndex]*(bcWinArea[nAreaIndex-1]);
		}
		else
		{
			if(bcWinArea[nAreaIndex-1]==0)
			{
				WinScore+=m_lAllJettonScore[nAreaIndex];
			}
		}
	}

	LONGLONG lTemp = lBankerScore;
	lBankerScore = lBankerScore + WinScore - LosScore;

	if ( lBankerScore < 0 )
	{
		if (m_wBankerUser!=INVALID_CHAIR)
		{
			lBankerScore = m_lBankerScore;
		}
		else
		{
			lBankerScore = 0x7fffffffffffffff;
		}
	}

	//��������
	LONGLONG lMeMaxScore;

	if((m_lMeMaxScore - lNowJetton)>m_lAreaLimitScore)
	{
		lMeMaxScore= m_lAreaLimitScore;
	}
	else
	{
		lMeMaxScore = m_lMeMaxScore-lNowJetton;
		lMeMaxScore = lMeMaxScore;
	}

	//ׯ������
	lMeMaxScore=min(lMeMaxScore,(lBankerScore)/(bcWinArea[cbJettonArea-1]));

	//��������
	lMeMaxScore = max(lMeMaxScore, 0);

	return lMeMaxScore;
}

//�ɼ�����
void CGameClientView::SetGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore)
{
	m_lMeCurGameScore=lMeCurGameScore;
	m_lMeCurGameReturnScore=lMeCurGameReturnScore;
	m_lBankerCurGameScore=lBankerCurGameScore;	
}

void CGameClientView::DrawMeJettonNumber(CDC *pDC)
{
	//�滭����
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
	{
		if (m_lUserJettonScore[nAreaIndex] > 0 )
			DrawNumberString(pDC,m_lUserJettonScore[nAreaIndex],m_PointJettonNumber[nAreaIndex-1].x-5,m_PointJettonNumber[nAreaIndex-1].y+25+50, false,true);
	}
}

//��ʼ����
void CGameClientView::DispatchCard()
{

	//���ñ�ʶ
	m_bNeedSetGameRecord=true;
}

//��������
void CGameClientView::FinishDispatchCard( bool bRecord /*= true*/ )
{
	//����ж�
	if (m_bNeedSetGameRecord==false) return;

	//���ñ�ʶ
	m_bNeedSetGameRecord=false;

	//ɾ����ʱ��
	KillTimer(IDI_DISPATCH_CARD);

	BYTE  bcResulteOut[AREA_COUNT];
	memset(bcResulteOut,0,AREA_COUNT);

	m_GameLogic.GetCardType(&m_cbTableCardArray[0][0],1,bcResulteOut);

	//�����¼
	if (bRecord)
	{
		SetGameHistory(bcResulteOut);
	}

	//�ۼƻ���
	m_lMeStatisticScore+=m_lMeCurGameScore;
	m_lBankerWinScore=m_lTmpBankerWinScore;


	bool blWin[AREA_COUNT];
	for (int i = 0;i<AREA_COUNT;i++){

		if(bcResulteOut[i]>0)
		{
			blWin[i]=true;
		}
		else
		{
			blWin[i]=false;
		}
	}

	//����Ӯ��
	SetWinnerSide(blWin, true);

	//��������
	if (m_lMeCurGameScore>0) 
	{
		::AfxGetMainWnd()->SendMessage(IDM_SOUND,5,5);

	}
	else if (m_lMeCurGameScore<0) 
	{
		::AfxGetMainWnd()->SendMessage(IDM_SOUND,4,4);

	}
	else 
	{
		::AfxGetMainWnd()->SendMessage(IDM_SOUND,4,4);

	}
}

//ʤ���߿�
void CGameClientView::FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC)
{
	//�Ϸ��ж�

	if(__TEST__)
	{
	}
	if(m_bFlashResult&&!m_blRungingCar)
	{
		m_idb_selPng.DrawImage(pDC ,m_CarRect[m_CarIndex].left,m_CarRect[m_CarIndex].top,\
			m_idb_selPng.GetWidth()/8 ,m_idb_selPng.GetHeight(),(m_CarIndex%8)*(m_idb_selPng.GetWidth()/8),0,m_idb_selPng.GetWidth()/8 ,m_idb_selPng.GetHeight());
	}

	if (m_cbAreaFlash==0xFF && false==m_bFlashResult) return;

	//��ע�ж�
	if (false==m_bFlashResult)
	{
		if (m_pGameClientDlg->GetGameStatus()==GS_PLACE_JETTON)
		{			
			pDC->Draw3dRect(m_RectArea[m_cbAreaFlash-1].left,m_RectArea[m_cbAreaFlash-1].top,m_RectArea[m_cbAreaFlash-1].Width()  ,m_RectArea[m_cbAreaFlash-1].Height(),RGB(255,255,0),RGB(255,255,0));
		}	
	}
	else
	{
		{
			for (int i = 0;i<AREA_COUNT;i++)
			{
				if(m_bWinFlag[i])
				{
					pDC->Draw3dRect(m_RectArea[i].left,m_RectArea[i].top,m_RectArea[i].Width()  ,m_RectArea[i].Height(),RGB(255,255,0),RGB(255,255,0));

				}
			}
		}
	}
}

//�ƶ�Ӯ��
void CGameClientView::DeduceWinner(bool bWinMen[])
{
	BYTE bcData = m_cbTableCardArray[0][0];
	if(1==bcData||bcData==2||bcData==1+8||bcData==2+8||bcData==1+2*8||bcData==2+2*8||bcData==1+3*8||bcData==2+3*8)
	{
		if(bcData%2==1)
			bWinMen[0]= true;
		else
			bWinMen[1]= true;

	}
	else if(3==bcData||bcData==4||bcData==3+8||bcData==4+8||bcData==3+2*8||bcData==4+2*8||bcData==3+3*8||bcData==4+3*8)
	{
		if(bcData%2==1)
			bWinMen[2]= true;
		else
			bWinMen[3]= true;



	}else if(5==bcData||bcData==6||bcData==5+8||bcData==6+8||bcData==5+2*8||bcData==6+2*8||bcData==5+3*8||bcData==6+3*8)
	{
		if(bcData%2==1)
			bWinMen[4]= true;
		else
			bWinMen[5]= true;

	}else if(7==bcData||bcData==8||bcData==7+8||bcData==8+8||bcData==7+2*8||bcData==8+2*8||bcData==7+3*8||bcData==8+3*8)
	{
		if(bcData%2==1)
			bWinMen[6]= true;
		else
			bWinMen[7]= true;

	}
}

//�ؼ�����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��ȡID
	WORD wControlID=LOWORD(wParam);

	//�ؼ��ж�
	switch (wControlID)
	{
	case IDC_JETTON_BUTTON_100:
		{
			//���ñ���
			m_lCurrentJetton=100L;
			break;
		}
	case IDC_JETTON_BUTTON_1000:
		{
			//���ñ���
			m_lCurrentJetton=1000L;
			break;
		}
	case IDC_JETTON_BUTTON_10000:
		{
			//���ñ���
			m_lCurrentJetton=10000L;
			break;
		}
	case IDC_JETTON_BUTTON_50000:
		{
			//���ñ���
			m_lCurrentJetton=50000L;
			break;
		}
	case IDC_JETTON_BUTTON_100000:
		{
			//���ñ���
			m_lCurrentJetton=100000L;
			break;
		}
	case IDC_JETTON_BUTTON_500000:
		{
			//���ñ���
			m_lCurrentJetton=500000L;
			break;
		}
	case IDC_JETTON_BUTTON_1000000:
		{
			//���ñ���
			m_lCurrentJetton=1000000L;
			break;
		}
	case IDC_JETTON_BUTTON_5000000:
		{
			//���ñ���
			m_lCurrentJetton=5000000L;
			break;
		}
	case IDC_AUTO_OPEN_CARD:
		{
			break;
		}
	case IDC_OPEN_CARD:
		{
			break;
		}
	case IDC_BANK:
		{
			break;
		}
	case IDC_CONTINUE_CARD:
		{
			SwithToNormalView();
			AfxGetMainWnd()->SendMessage(IDM_CONTINUE_CARD,0,0);
			break;
		}
	case IDC_RADIO:
		{
			m_CheckImagIndex = 0;

		}

		break;
	case IDC_RADIO+1:
		{
			m_CheckImagIndex = 1;
		}
		break;
	case IDC_RADIO+2:
		{
			m_CheckImagIndex = 2;

		}
		break;
	case IDC_RADIO+3:
		{
			m_CheckImagIndex = 3;

		}
		break;

	}

	m_lLastJetton = m_lCurrentJetton;

	return CGameFrameView::OnCommand(wParam, lParam);
}

//�ҵ�λ��
void CGameClientView::SetMeChairID(DWORD dwMeUserID)
{
	//�������Ӻ�
	for (WORD wChairID=0; wChairID<MAX_CHAIR; ++wChairID)
	{
		tagUserData const *pUserData=GetUserInfo(wChairID);
		if (NULL!=pUserData && dwMeUserID==pUserData->dwUserID)
		{
			m_wMeChairID=wChairID;
			break;
		}
	}
}

//������ʾ
void CGameClientView::SetDispatchCardTip(enDispatchCardTip DispatchCardTip)
{
	//���ñ���
	m_enDispatchCardTip=DispatchCardTip;

	//���ö�ʱ��
	if (enDispatchCardTip_NULL!=DispatchCardTip) SetTimer(IDI_SHOWDISPATCH_CARD_TIP,2*1000,NULL);		
	else KillTimer(IDI_SHOWDISPATCH_CARD_TIP);

	//���½���
	UpdateGameView(NULL);
}

//�滭ׯ��
void CGameClientView::DrawBankerInfo(CDC *pDC,int nWidth,int nHeight)
{
	//ׯ����Ϣ																											
	pDC->SetTextColor(RGB(255,255,0));

	//��ȡ���
	tagUserData const *pUserData = m_wBankerUser==INVALID_CHAIR ? NULL : GetUserInfo(m_wBankerUser);

	//λ����Ϣ
	m_LifeWidth;	
	m_TopHeight;

	CRect static StrRect;
	StrRect.left = m_LifeWidth+79-66;
	StrRect.top = m_TopHeight+84-66;
	StrRect.right = StrRect.left + 171;
	StrRect.bottom = StrRect.top + 15;

	//ׯ������
	TCHAR Text[256];
	if(m_bEnableSysBanker)
	{
		_snprintf(Text,256,_TEXT("��ǰׯ�ң�%s"),pUserData==NULL?(m_bEnableSysBanker?TEXT("ϵͳ��ׯ"):TEXT("ϵͳ��ׯ")):pUserData->szName);
		pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE );
	}
	else
	{
		_snprintf(Text,256,_TEXT("��ǰׯ�ң�%s"),pUserData==NULL?(m_bEnableSysBanker==false?TEXT("������ׯ"):TEXT("������ׯ")):pUserData->szName);
		pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE );
	}

	StrRect.top = StrRect.bottom;
	StrRect.bottom = StrRect.top + 15;
	_snprintf(Text,256,_TEXT("��ׯ������%d"),m_wBankerTime);
	pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE );

	StrRect.top = StrRect.bottom;
	StrRect.bottom = StrRect.top + 15;
	_snprintf(Text,256,_TEXT("ׯ�ҲƸ���%s"),NumberStringWithSpace(pUserData==NULL?0:pUserData->lScore));
	pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE);

	StrRect.top = StrRect.bottom;
	StrRect.bottom = StrRect.top + 15;
	_snprintf(Text,256,_TEXT("ׯ����Ӯ��%s"),NumberStringWithSpace(m_lBankerWinScore));
	pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE );
}
void CGameClientView::SetFirstShowCard(BYTE bcCard)
{


}
//�滭���
void CGameClientView::DrawMeInfo(CDC *pDC,int nWidth,int nHeight)
{
	//�Ϸ��ж�
	if (INVALID_CHAIR==m_wMeChairID) return;

	//ׯ����Ϣ																											
	pDC->SetTextColor(RGB(255,255,0));

	//��ȡ���
	tagUserData const *pUserData =  GetUserInfo(m_wMeChairID);

	//λ����Ϣ
	m_LifeWidth;	
	m_TopHeight;

	CRect static StrRect;
	StrRect.left = m_LifeWidth+79-66+17;
	StrRect.top = m_TopHeight+84-66+610;
	StrRect.right = StrRect.left + 144;
	StrRect.bottom = StrRect.top + 15;

	//ׯ������
	TCHAR Text[256];
	_snprintf(Text,256,_TEXT("�ǳƣ�%s"),pUserData->szName);
	pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE );

	StrRect.top = StrRect.bottom;
	StrRect.bottom = StrRect.top + 15;
	_snprintf(Text,256,_TEXT("��ң�%s"),NumberStringWithSpace(pUserData->lScore));
	pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE );

	StrRect.top = StrRect.bottom;
	StrRect.bottom = StrRect.top + 15;
	LONGLONG lMeJetton=0L;
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lMeJetton += m_lUserJettonScore[nAreaIndex];
	_snprintf(Text,256,_TEXT("���ã�%s"),NumberStringWithSpace(pUserData==NULL?0:pUserData->lScore-lMeJetton));
	pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE );

	StrRect.top = StrRect.bottom;
	StrRect.bottom = StrRect.top + 15;
	_snprintf(Text,256,_TEXT("�ɼ���%s"),NumberStringWithSpace(m_lMeStatisticScore) );
	pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE );
}
//////////////////////////////////////////////////////////////////////////
//���д��
void CGameClientView::OnBankStorage()
{
#ifdef __SPECIAL___
	//��ȡ�ӿ�
	CGameClientDlg *pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientDlg->GetClientKernel(IID_IClientKernel,VER_IClientKernel);

	if (NULL!=pIClientKernel)
	{
		if (NULL==m_DlgBank.m_hWnd) m_DlgBank.Create(IDD_BANK_STORAGE,this);

		if (!m_DlgBank.IsWindowVisible() || !m_DlgBank.m_bBankStorage)
		{
			m_DlgBank.SetSendInfo(pIClientKernel,pIClientKernel->GetMeUserInfo());
			m_DlgBank.SetBankerActionType(true);
			m_DlgBank.ShowWindow(SW_SHOW);

			CRect btRect;
			m_btBankerStorage.GetWindowRect(&btRect);
			m_DlgBank.SetPostPoint(CPoint(btRect.right,btRect.top));
		}
		else
			m_DlgBank.ShowWindow(SW_HIDE);
	}
#endif
}

//����ȡ��
void CGameClientView::OnBankDraw()
{
#ifdef __SPECIAL___
	//��ȡ�ӿ�
	CGameClientDlg *pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientDlg->GetClientKernel(IID_IClientKernel,VER_IClientKernel);

	if (NULL!=pIClientKernel)
	{
		if (NULL==m_DlgBank.m_hWnd) m_DlgBank.Create(IDD_BANK_STORAGE,this);

		if (!m_DlgBank.IsWindowVisible() || m_DlgBank.m_bBankStorage)
		{
			m_DlgBank.SetSendInfo(pIClientKernel,pIClientKernel->GetMeUserInfo());
			m_DlgBank.SetBankerActionType(false);
			m_DlgBank.ShowWindow(SW_SHOW);

			CRect btRect;
			m_btBankerDraw.GetWindowRect(&btRect);
			m_DlgBank.SetPostPoint(CPoint(btRect.right,btRect.top));
		}
		else
			m_DlgBank.ShowWindow(SW_HIDE);
	}
#endif
}

void CGameClientView::OnUp()
{
	m_ApplyUser.m_AppyUserList.SendMessage(WM_VSCROLL, MAKELONG(SB_LINEUP,0),NULL);
	m_ApplyUser.m_AppyUserList.Invalidate(TRUE);
	double nPos = m_ApplyUser.m_AppyUserList.GetScrollPos(SB_VERT);
	double nMax = m_ApplyUser.m_AppyUserList.GetScrollLimit(SB_VERT);

}
void CGameClientView::OnDown()
{
	double nPos = m_ApplyUser.m_AppyUserList.GetScrollPos(SB_VERT);
	if(nPos>m_ApplyUser.m_AppyUserList.GetItemCount()-3)
	{
		return ;
	}
	m_ApplyUser.m_AppyUserList.SendMessage(WM_VSCROLL, MAKELONG(SB_LINEDOWN,0),NULL);
	m_ApplyUser.m_AppyUserList.Invalidate(TRUE);
	double nMax = m_ApplyUser.m_AppyUserList.GetScrollLimit(SB_VERT);
};
void CGameClientView::StartMove()
{
	SetTimer(IDI_MOVECARD_END,250,NULL);
	m_Out_Bao_y = 0;
	m_bShowJettonIndex = 0;
	m_bShowLeaveHandleIndex = 0;
	m_bShowBao = TRUE;
	m_bShowJettonAn = false;
	m_bShowLeaveHandleAn = false;
	m_bOPenBoxAn = false;
	m_bOPenBoxIndex = 0;
	m_blShowLastResult = false;
	m_bShowResult = false;

}
void CGameClientView::StartJetton_AniMationN()
{
	SetTimer(IDI_JETTON_ANIMATION,400,NULL);
	m_bShowJettonAn = true;
	m_bShowJettonIndex = 0;


}

void CGameClientView::StartOPenBox()
{
	m_bOPenBoxAn = true;
	m_bOPenBoxIndex = 0;
	m_bShowBao = false;
	m_Out_Bao_y = 6;
	SetJettonHide(0);
	SetTimer(IDI_OPENBOX_ANIMATION,400,NULL);	

}

void CGameClientView::StartHandle_Leave()
{
	m_bShowLeaveHandleAn = true;
	m_bShowLeaveHandleIndex = 0;
	SetTimer(IDI_HANDLELEAVE_ANIMATION,400,NULL);

	::AfxGetMainWnd()->SendMessage(IDM_SOUND,0,1);


}
void CGameClientView::StarShowResult()
{
	m_bShowResult = true;

}
//�滭ʱ��
void CGameClientView::MyDrawUserTimer(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea)
{
	//������Դ
	CPngImage ImageTimeBack;
	CPngImage ImageTimeNumber;
	ImageTimeBack.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("TIME_BACK"));
	ImageTimeNumber.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("TIME_NUMBER"));

	//��ȡ����
	const INT nNumberHeight=ImageTimeNumber.GetHeight();
	const INT nNumberWidth=ImageTimeNumber.GetWidth()/10;

	//������Ŀ
	LONGLONG lNumberCount=0;
	WORD wNumberTemp=wTime;
	do
	{
		lNumberCount++;
		wNumberTemp/=10;
	} while (wNumberTemp>0L);

	//λ�ö���
	INT nYDrawPos=nYPos-nNumberHeight/2+1;
	INT nXDrawPos=static_cast<INT>(nXPos+(lNumberCount*nNumberWidth)/2-nNumberWidth);

	//�滭����
	CSize SizeTimeBack(ImageTimeBack.GetWidth(),ImageTimeBack.GetHeight());
	ImageTimeBack.DrawImage(pDC,nXPos-SizeTimeBack.cx/2,nYPos-SizeTimeBack.cy/2);

	//�滭����
	for (LONGLONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		WORD wCellNumber=wTime%10;
		ImageTimeNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,wCellNumber*nNumberWidth,0);

		//���ñ���
		wTime/=10;
		nXDrawPos-=nNumberWidth;
	};

	return;
}

//����Ա����
void CGameClientView::OpenAdminWnd()
{
	//��Ȩ��
	if((GetUserInfo(m_wMeChairID)->dwUserRight&UR_GAME_CONTROL)!=0)
	{
		if (NULL==m_AdminControl.m_hWnd) 
			m_AdminControl.Create(IDD_DIALOG_SYSTEM,this);

		if(!m_AdminControl.IsWindowVisible()) 
			m_AdminControl.ShowWindow(SW_SHOW);
		else 
			m_AdminControl.ShowWindow(SW_HIDE);
	}
}

//ִ��ʣ�����еĻ��嶯��
void CGameClientView::PerformAllBetAnimation()
{
	KillTimer(IDI_ANDROID_BET);
	for ( int i = 0 ; i < m_ArrayAndroid.GetCount(); ++i)
	{
		KillTimer(IDI_ANDROID_BET + i + 1);
		PlaceUserJetton(m_ArrayAndroid[i].cbJettonArea, m_ArrayAndroid[i].lJettonScore);
	}
	m_ArrayAndroid.RemoveAll();
}