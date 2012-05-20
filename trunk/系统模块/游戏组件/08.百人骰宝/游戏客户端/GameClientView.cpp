#include "StdAfx.h"
#include "Math.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FLASH_WINNER			100									//������ʶ
#define IDI_SHOW_CHANGE_BANKER		101									//�ֻ�ׯ��
#define IDI_SHOW_CHANGE_SICBO       102                                 // ����ҡ���ӷ�ʽ
#define IDI_DISPATCH_CARD			103									//���Ʊ�ʶ
#define IDI_CHANGE_AZIMUTH          104                                 // ���ķ�λ
#define IDI_BOMB_EFFECT				200									//��ը��ʶ


//��ť��ʶ
#define IDC_JETTON_BUTTON_100		200									//��ť��ʶ
#define IDC_JETTON_BUTTON_1000		201									//��ť��ʶ
#define IDC_JETTON_BUTTON_10000		202									//��ť��ʶ
#define IDC_JETTON_BUTTON_100000	203									//��ť��ʶ
#define IDC_JETTON_BUTTON_500000	204									//��ť��ʶ
#define IDC_JETTON_BUTTON_1000000	205									//��ť��ʶ
#define IDC_JETTON_BUTTON_5000000	206									//��ť��ʶ
#define IDC_APPY_BANKER				207									//��ť��ʶ
#define IDC_CANCEL_BANKER			208									//��ť��ʶ
#define IDC_SCORE_MOVE_L			209									//��ť��ʶ
#define IDC_SCORE_MOVE_R			210									//��ť��ʶ
#define IDC_BANKER_MOVE_U           211                                 // �ƶ�ׯ��
#define IDC_BANKER_MOVE_D           212                                 // �ƶ�ׯ��

#define IDC_BT_SICBO_DESKTOP		220									//��ť��ʶ
#define IDC_BT_SICBO_INMIDAIR		221									//��ť��ʶ
#define IDC_BT_SICBO_INTHEAIR		223									//��ť��ʶ

#define IDC_BT_OPEN_BANK		    230									//��ť��ʶ
#define IDC_TC_PROMPT		        231									//��ť��ʶ

//��ը��Ŀ
#define BOMB_EFFECT_COUNT			8									// ��ը��Ŀ
#define SICBO_EFFECT_COUNT			55//35								// ҡ���ӵ���Ŀ
#define SICBO_EFFECT_FRAME_COUNT	35									// ҡ���ӵ���Ŀ
#define SICBO_FRAME_COUNT_NORMAL    10                                  // ��������ʹ�õ�֡��

#define SHOW_APPLY_BANKER         6                                      // ׯ����Ŀ

//////////////////////////////////////////////////////////////////////////
TCHAR *g_szPrompt[COUNT_AZIMUTH]={
	_T("��Χ���⣬�������ӵĵ�\n����Ϊ11���11������"),
_T("��Χ���⣬�������ӵĵ�\n����Ϊ10���10������"),
_T("��Χ���⣬�������ӵĵ�\n����Ϊ����"),
_T("��Χ���⣬�������ӵĵ�\n����Ϊż��"),
_T("��Χ���⣬�������ӵĵ�\n����Ϊ4"),
_T("��Χ���⣬�������ӵĵ�\n����Ϊ5"),
_T("��Χ���⣬�������ӵĵ�\n����Ϊ6"),
_T("��Χ���⣬�������ӵĵ�\n����Ϊ7"),
_T("��Χ���⣬�������ӵĵ�\n����Ϊ8"),
_T("��Χ���⣬�������ӵĵ�\n����Ϊ9"),
_T("��Χ���⣬�������ӵĵ�\n����Ϊ10"),
_T("��Χ���⣬�������ӵĵ�\n����Ϊ11"),
_T("��Χ���⣬�������ӵĵ�\n����Ϊ12"),
_T("��Χ���⣬�������ӵĵ�\n����Ϊ13"),
_T("��Χ���⣬�������ӵĵ�\n����Ϊ14"),
_T("��Χ���⣬�������ӵĵ�\n����Ϊ15"),
_T("��Χ���⣬�������ӵĵ�\n����Ϊ16"),
_T("��Χ���⣬�������ӵĵ�\n����Ϊ17"),
_T("���������г���1�����\n�ӣ������2������˫����\n3������3ɸ"),
_T("���������г���2�����\n�ӣ������2������˫����\n3������3ɸ"),
_T("���������г���3�����\n�ӣ������2������˫����\n3������3ɸ"),
_T("���������г���4�����\n�ӣ������2������˫����\n3������3ɸ"),
_T("���������г���5�����\n�ӣ������2������˫����\n3������3ɸ"),
_T("���������г���6�����\n�ӣ������2������˫����\n3������3ɸ"),
_T("��Χ���⣬����������ͬ\nʱ����1���2��"),
_T("��Χ���⣬����������ͬ\nʱ����1���3��"),
_T("��Χ���⣬����������ͬ\nʱ����1���4��"),
_T("��Χ���⣬����������ͬ\nʱ����1���5��"),
_T("��Χ���⣬����������ͬ\nʱ����1���6��"),
_T("��Χ���⣬����������ͬ\nʱ����2���3��"),
_T("��Χ���⣬����������ͬ\nʱ����2���4��"),
_T("��Χ���⣬����������ͬ\nʱ����2���5��"),
_T("��Χ���⣬����������ͬ\nʱ����2���6��"),
_T("��Χ���⣬����������ͬ\nʱ����3���4��"),
_T("��Χ���⣬����������ͬ\nʱ����3���5��"),
_T("��Χ���⣬����������ͬ\nʱ����3���6��"),
_T("��Χ���⣬����������ͬ\nʱ����4���5��"),
_T("��Χ���⣬����������ͬ\nʱ����4���6��"),
_T("��Χ���⣬����������ͬ\nʱ����5���6��"),
_T("��Χ���⣬����������ͬ\nʱ��������1��"),
_T("��Χ���⣬����������ͬ\nʱ��������2��"),
_T("��Χ���⣬����������ͬ\nʱ��������3��"),
_T("��Χ���⣬����������ͬ\nʱ��������4��"),
_T("��Χ���⣬����������ͬ\nʱ��������5��"),
_T("��Χ���⣬����������ͬ\nʱ��������6��"),
_T("�������ӵĵ�������1"),
_T("�������ӵĵ�������2"),
_T("�������ӵĵ�������3"),
_T("�������ӵĵ�������4"),
_T("�������ӵĵ�������5"),
_T("�������ӵĵ�������6"),
_T("�������ӵĵ�����ͬ")};

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_JETTON_BUTTON_100, OnJettonButton100)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_1000, OnJettonButton1000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_10000, OnJettonButton10000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_100000, OnJettonButton100000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_500000, OnJettonButton500000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_1000000, OnJettonButton1000000)
	ON_BN_CLICKED(IDC_JETTON_BUTTON_5000000, OnJettonButton5000000)

	ON_BN_CLICKED(IDC_BANKER_MOVE_U, OnBtnBankerMoveUp)
	ON_BN_CLICKED(IDC_BANKER_MOVE_D, OnBtnBankerMoveDown)
	ON_BN_CLICKED(IDC_APPY_BANKER, OnApplyBanker)
	ON_BN_CLICKED(IDC_CANCEL_BANKER, OnCancelBanker)
	ON_BN_CLICKED(IDC_SCORE_MOVE_L, OnScoreMoveL)
	ON_BN_CLICKED(IDC_SCORE_MOVE_R, OnScoreMoveR)
	ON_BN_CLICKED(IDC_BT_SICBO_DESKTOP, OnBtnSicboDesktop)
	ON_BN_CLICKED(IDC_BT_SICBO_INMIDAIR, OnBtnSicboInMidair)
	ON_BN_CLICKED(IDC_BT_SICBO_INTHEAIR, OnBtnSicboInTheAir)
	ON_BN_CLICKED(IDC_BT_OPEN_BANK, OnBtnOpenBank)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView() : CGameFrameView(true,24)
{
	//������ע
	ZeroMemory(m_lMeScore, sizeof(m_lMeScore));
	
	//ȫ����ע
	ZeroMemory(m_lAllScore, sizeof(m_lAllScore));

	//ׯ����Ϣ
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_lBankerScore=0L;	
	m_lBankerWinScore=0L;
	m_lTmpBankerWinScore=0L;

	//���ֳɼ�
	m_lMeCurGameScore=0L;	
	m_lMeCurGameReturnScore=0L;
	m_lBankerCurGameScore=0L;
	m_lGameRevenue=0L;

	//��������
	ZeroMemory(m_bBombEffect,sizeof(m_bBombEffect));
	ZeroMemory(m_cbBombFrameIndex,sizeof(m_cbBombFrameIndex));
	m_bSicboEffect = false;
	m_bySicboFrameIndex = 0;
	ZeroMemory(&m_SicboAnimInfo,sizeof(m_SicboAnimInfo));
	ZeroMemory(m_bySicboDraw,sizeof(m_bySicboDraw));  // �����ӵ�˳��

	ZeroMemory(m_enCards,sizeof(m_enCards));
	ZeroMemory(m_bWinner,sizeof(m_bWinner));   // ��Ӯ���

	//״̬��Ϣ
	m_lCurrentJetton=0L;
	m_cbWinnerSide=0xFF;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=false;
	m_bShowChangeSicbo = false;
	m_bShowSicboEffectBack = false;

	m_lMeCurGameScore=0L;			
	m_lMeCurGameReturnScore=0L;	
	m_lBankerCurGameScore=0L;		

	m_lAreaLimitScore=0L;	
	m_lMeMaxScore = 0L;
	m_lAllMaxScore = 0L;

	//λ����Ϣ
	m_nScoreHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;	
	m_enSicboType = enSicboType_Sicbo;
	m_enSelArea = enCardType_Illegal;

	//��ʷ�ɼ�
	m_lMeStatisticScore=0;

	//�ؼ�����
	m_pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
	m_ApplyUserArray.RemoveAll();
	m_iApplyFirst = 0;


	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageViewFill.SetLoadInfo(IDB_VIEW_FILL,hInstance);
	m_ImageViewBack.SetLoadInfo(IDB_VIEW_BACK,hInstance);
	m_ImageWinFlags.SetLoadInfo(IDB_WIN_FLAGS,hInstance);
	m_ImageScoreNumber.SetLoadInfo(IDB_SCORE_NUMBER,hInstance);
	m_ImageMeScoreNumber.SetLoadInfo(IDB_ME_SCORE_NUMBER,hInstance);
	m_ImageCardDot.SetLoadInfo(IDB_SICBO_NUMBER_DOT, hInstance);
	m_ImageCardType.SetLoadInfo( IDB_CARD_TYPE, hInstance );

	m_ImageMeBanker.SetLoadInfo( IDB_ME_BANKER, hInstance );
	m_ImageChangeBanker.SetLoadInfo( IDB_CHANGE_BANKER, hInstance );
	m_ImageNoBanker.SetLoadInfo( IDB_NO_BANKER, hInstance );	

	m_ImageTimeFlag.SetLoadInfo(IDB_TIME_FLAG, hInstance);
	m_ImageSicboPoint.SetLoadInfo(IDB_SICBO_POINT, hInstance);

	m_ImageBombEffect.LoadImage(hInstance,TEXT("FIRE_EFFECT"));
	m_ImgSicboEffectNormal.LoadImage(hInstance, TEXT("SICBO_EFFECT_NORMAL"));
	m_ImgSicboEffectDesktop.LoadImage(hInstance, TEXT("SICBO_EFFECT_DESKTOP"));
	m_ImgSicboEffectInMidair.LoadImage(hInstance, TEXT("SICBO_EFFECT_INMIDAIR"));
	m_ImgSicboEffectInTheAir.LoadImage(hInstance, TEXT("SICBO_EFFECT_INTHEAIR"));
	m_ImgSicboEffectResult.LoadImage(hInstance, TEXT("SICBO_EFFECT_RESULT"));
	m_ImgSicboEffectBack.LoadImage(hInstance, TEXT("SICBO_EFFECT_BACK"));
	m_ImgSicboGiftInTheAir.LoadImage(hInstance, TEXT("GIFT_INTHEAIR"));

	m_ImgSicboEPlan.LoadImage(hInstance, TEXT("SICBO_EPLAN"));			//����ͼƬ
	m_ImgSicboPlan.LoadImage(hInstance, TEXT("SICBO_PLAN"));			    //����ͼƬ
	m_ImgSicboNum.LoadImage(hInstance, TEXT("SICBO_NUM"));			    //����ͼƬ
	m_PngGameEnd.LoadImage(hInstance,TEXT("GAME_END"));
	m_ImageJettonView.LoadImage(hInstance, TEXT("JETTON_VIEW"));
	m_ImageFrameWin.LoadImage(hInstance, TEXT("FRAME_WIN"));
	m_TCPrompt.m_PngBack.LoadImage(hInstance, TEXT("USER_PROMPT"));
	for (int i=0; i<COUNT_AZIMUTH; ++i)
	{		
		TCHAR szPngName[_MAX_PATH]={0};
		_sntprintf(szPngName, sizeof(szPngName),_T("SICBO_AREA_%02d"), i);
		m_PngSicboArea[i].LoadImage(hInstance,szPngName);
		if (i<SICBO_ANIM_FRAME_COUNT)
		{
			_sntprintf(szPngName, sizeof(szPngName),_T("ANIM_SICBO_%02d"), i+1);
			m_ImgSicboAnim[i].LoadImage(hInstance,szPngName);
		}
	}

	m_ImageSicboDesktop.SetLoadInfo( IDB_SICBO_DESKTOP, hInstance );                // ����ҡ����
	m_ImageSicboInMidair.SetLoadInfo( IDB_SICBO_IN_MIDAIR, hInstance );             // ���ҡ����
	m_ImageSicboInTheAir.SetLoadInfo( IDB_SICBO_IN_THE_AIR, hInstance );            // ����ҡ����
	m_bSicboShow = false;
	m_lSelAreaMax = 0L;
	return;
}

//��������
CGameClientView::~CGameClientView(void)
{
}

BOOL CGameClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	//cs.style = cs.style & ~WS_CLIPCHILDREN;
	return __super::PreCreateWindow(cs);
}

//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	
	//��ע��ť
	m_btJetton100.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100);
	m_btJetton1000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1000);
	m_btJetton10000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_10000);
	m_btJetton100000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100000);
	m_btJetton500000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_500000);
	m_btJetton1000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1000000);
	m_btJetton5000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_5000000);
		
	//���밴ť
	m_btApplyBanker.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_APPY_BANKER);
	m_btCancelBanker.Create(NULL,WS_CHILD|WS_DISABLED,rcCreate,this,IDC_CANCEL_BANKER);
	m_btScoreMoveL.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_L);
	m_btScoreMoveR.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_R);

	m_btBankerMoveUp.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BANKER_MOVE_U);
	m_btBankerMoveDown.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BANKER_MOVE_D);

	m_btSicboDesktop.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BT_SICBO_DESKTOP);                     // ����ҡ����
	m_btSicboInMidair.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BT_SICBO_INMIDAIR);                   // ���ҡ����
	m_btSicboInTheAir.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BT_SICBO_INTHEAIR);                   // ����ҡ����
	
	m_btSicboDesktop.ShowWindow(SW_HIDE);
	m_btSicboInMidair.ShowWindow(SW_HIDE);
	m_btSicboInTheAir.ShowWindow(SW_HIDE);
	
	m_btOpenBank.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BT_OPEN_BANK);                   // ����ҡ����

	//���ð�ť
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btJetton100.SetButtonImage(IDB_BT_JETTON_100,hResInstance,false);
	m_btJetton1000.SetButtonImage(IDB_BT_JETTON_1000,hResInstance,false);
	m_btJetton10000.SetButtonImage(IDB_BT_JETTON_10000,hResInstance,false);
	m_btJetton100000.SetButtonImage(IDB_BT_JETTON_100000,hResInstance,false);
	m_btJetton500000.SetButtonImage(IDB_BT_JETTON_500000,hResInstance,false);
	m_btJetton1000000.SetButtonImage(IDB_BT_JETTON_1000000,hResInstance,false);
	m_btJetton5000000.SetButtonImage(IDB_BT_JETTON_5000000,hResInstance,false);

	m_btApplyBanker.SetButtonImage(IDB_BT_APPLY_BANKER,hResInstance,false);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,hResInstance,false);

	m_btScoreMoveL.SetButtonImage(IDB_BT_SCORE_MOVE_L,hResInstance,false);
	m_btScoreMoveR.SetButtonImage(IDB_BT_SCORE_MOVE_R,hResInstance,false);

	m_btBankerMoveUp.SetButtonImage(IDB_BT_UP,hResInstance,false);
	m_btBankerMoveDown.SetButtonImage(IDB_BT_DOWN,hResInstance,false);

	m_btSicboDesktop.SetButtonImage(IDB_BT_SICBO_DESKTOP,hResInstance,false);
	m_btSicboInMidair.SetButtonImage(IDB_BT_SICBO_INMIDAIR,hResInstance,false);
	m_btSicboInTheAir.SetButtonImage(IDB_BT_SICBO_INTHEAIR,hResInstance,false);
	m_btOpenBank.SetButtonImage(IDB_OPEN_BANK, hResInstance, false);
	m_TCPrompt.Create(NULL,WS_CHILD|WS_VISIBLE|SS_CENTERIMAGE,rcCreate,this,IDC_TC_PROMPT);  // ��ʾ
	m_TCPrompt.ShowWindow(SW_HIDE);	
	return 0;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//������ע
	ZeroMemory(m_lMeScore, sizeof(m_lMeScore));

	//ȫ����ע
	ZeroMemory(m_lAllScore, sizeof(m_lAllScore));

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

	//��������
	ZeroMemory(m_bBombEffect,sizeof(m_bBombEffect));
	ZeroMemory(m_cbBombFrameIndex,sizeof(m_cbBombFrameIndex));
	m_bSicboEffect = false;                             // ҡ����Ч��
	m_bySicboFrameIndex = 0L;
	ZeroMemory(&m_SicboAnimInfo,sizeof(m_SicboAnimInfo));
	ZeroMemory(m_bySicboDraw,sizeof(m_bySicboDraw));
	ZeroMemory(m_enCards,sizeof(m_enCards));;
	ZeroMemory(m_bWinner,sizeof(m_bWinner));   // ��Ӯ���

	//״̬��Ϣ
	m_lCurrentJetton=0L;
	m_cbWinnerSide=0xFF;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=false;
	m_bShowChangeSicbo =false;
	m_bShowSicboEffectBack = false;
	m_bSicboShow=false;

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
	m_ApplyUserArray.RemoveAll();
	m_iApplyFirst = 0;
	m_lSelAreaMax = 0L;

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
	//����λ��
	int nCenterX=nWidth/2, nCenterY=nHeight/2;

	//λ����Ϣ
	m_nWinFlagsExcursionX = nCenterX-234;	
	m_nWinFlagsExcursionY = nCenterY + 263;

	// 52 ������
	int iX = nCenterX+223;
	int iY = nCenterY-233;
	m_rcUser[enCardType_Big]=CRect(iX,iY,iX+141,iY+156);
	iX = nCenterX-365;
	m_rcUser[enCardType_Small]=CRect(iX,iY,iX+141,iY+156);
	iY = nCenterY+110;
	m_rcUser[enCardType_Single]=CRect(iX,iY,iX+140,iY+102);
	iX = nCenterX+224;
	m_rcUser[enCardType_Double]=CRect(iX,iY,iX+140,iY+102);
	int iW = 53;
	int iH = 99;
	iX = nCenterX-365;
	iY = nCenterY-78;
	m_rcUser[enCardType_NumberFour]=CRect(iX,iY,iX+iW,iY+iH);
	iX = nCenterX-313;
	m_rcUser[enCardType_NumberFive]=CRect(iX,iY,iX+iW,iY+iH);
	iX = nCenterX-261;
	m_rcUser[enCardType_NumberSix]=CRect(iX,iY,iX+iW,iY+iH);
	iX = nCenterX-209;
	m_rcUser[enCardType_NumberSeven]=CRect(iX,iY,iX+iW,iY+iH);
	iX = nCenterX-157;
	m_rcUser[enCardType_NumberEight]=CRect(iX,iY,iX+iW,iY+iH);
	iX = nCenterX-105;
	m_rcUser[enCardType_NumberNine]=CRect(iX,iY,iX+iW,iY+iH);
	iX = nCenterX-53;
	m_rcUser[enCardType_NumberTen]=CRect(iX,iY,iX+iW,iY+iH);
	iX = nCenterX-1;
	m_rcUser[enCardType_NumberEleven]=CRect(iX,iY,iX+iW,iY+iH);
	iX = nCenterX+51;
	m_rcUser[enCardType_NumberTwelve]=CRect(iX,iY,iX+iW,iY+iH);
	iX = nCenterX+103;
	m_rcUser[enCardType_NumberThirteen]=CRect(iX,iY,iX+iW,iY+iH);
	iX = nCenterX+155;
	m_rcUser[enCardType_NumberFourteen]=CRect(iX,iY,iX+iW,iY+iH);
	iX = nCenterX+207;
	m_rcUser[enCardType_NumberFifteen]=CRect(iX,iY,iX+iW,iY+iH);
	iX = nCenterX+259;
	m_rcUser[enCardType_NumberSixteen]=CRect(iX,iY,iX+iW,iY+iH);
	iX = nCenterX+311;
	m_rcUser[enCardType_NumberSeventeen]=CRect(iX,iY,iX+iW,iY+iH);
	//////////////////////////////////////////////////////////////////////////
	iX=nCenterX-226;
	iY = nCenterY+110;
	iW = 76;
	iH = 54;
	m_rcUser[enCardType_SicboOne]=CRect(iX,iY,iX+iW,iY+iH);
	iX=nCenterX-151;
	m_rcUser[enCardType_SicboTwo]=CRect(iX,iY,iX+iW,iY+iH);
	iX=nCenterX-76;
	m_rcUser[enCardType_SicboThree]=CRect(iX,iY,iX+iW,iY+iH);
	iX=nCenterX-1;
	m_rcUser[enCardType_SicboFour]=CRect(iX,iY,iX+iW,iY+iH);
	iX=nCenterX+74;
	m_rcUser[enCardType_SicboFive]=CRect(iX,iY,iX+iW,iY+iH);
	iX=nCenterX+149;
	m_rcUser[enCardType_SicboSix]=CRect(iX,iY,iX+iW,iY+iH);
	iX=nCenterX-361;
	iY = nCenterY+20;
	iW = 49;
	iH = 91;
	m_rcUser[enCardType_SicboOneAndTwo]=CRect(iX,iY,iX+iW,iY+iH);
	iX=nCenterX-313;
	m_rcUser[enCardType_SicboOneAndThree]=CRect(iX,iY,iX+iW,iY+iH);
	iX=nCenterX-265;
	m_rcUser[enCardType_SicboOneAndFour]=CRect(iX,iY,iX+iW,iY+iH);
	iX=nCenterX-217;
	m_rcUser[enCardType_SicboOneAndFive]=CRect(iX,iY,iX+iW,iY+iH);
	iX=nCenterX-169;
	m_rcUser[enCardType_SicboOneAndSix]=CRect(iX,iY,iX+iW,iY+iH);
	iX=nCenterX-121;
	m_rcUser[enCardType_SicboTwoAndThree]=CRect(iX,iY,iX+iW,iY+iH);
	iX=nCenterX-73;
	m_rcUser[enCardType_SicboTwoAndFour]=CRect(iX,iY,iX+iW,iY+iH);
	iX=nCenterX-25;
	m_rcUser[enCardType_SicboTwoAndFive]=CRect(iX,iY,iX+iW,iY+iH);
	iX=nCenterX+23;
	m_rcUser[enCardType_SicboTwoAndSix]=CRect(iX,iY,iX+iW,iY+iH);
	iX=nCenterX+71;
	m_rcUser[enCardType_SicboThreeAndFour]=CRect(iX,iY,iX+iW,iY+iH);
	iX=nCenterX+119;
	m_rcUser[enCardType_SicboThreeAndFive]=CRect(iX,iY,iX+iW,iY+iH);
	iX=nCenterX+167;
	m_rcUser[enCardType_SicboThreeAndSix]=CRect(iX,iY,iX+iW,iY+iH);
	iX=nCenterX+215;
	m_rcUser[enCardType_SicboFourAndFive]=CRect(iX,iY,iX+iW,iY+iH);
	iX=nCenterX+263;
	m_rcUser[enCardType_SicboFourAndSix]=CRect(iX,iY,iX+iW,iY+iH);
	iX=nCenterX+311;
	m_rcUser[enCardType_SicboFiveAndSix]=CRect(iX,iY,iX+iW,iY+iH);
	iW = 78;
	iH = 46;
	iX=nCenterX-225;
	iY = nCenterY-213;
	m_rcUser[enCardType_SicboDoubleOne]=CRect(iX,iY,iX+iW,iY+iH);
	iY = nCenterY-168;
	m_rcUser[enCardType_SicboDoubleTwo]=CRect(iX,iY,iX+iW,iY+iH);
	iY = nCenterY-123;
	m_rcUser[enCardType_SicboDoubleThree]=CRect(iX,iY,iX+iW,iY+iH);
	iX=nCenterX+146;
	iY = nCenterY-213;
	m_rcUser[enCardType_SicboDoubleFour]=CRect(iX,iY,iX+iW,iY+iH);
	iY = nCenterY-168;
	m_rcUser[enCardType_SicboDoubleFive]=CRect(iX,iY,iX+iW,iY+iH);
	iY = nCenterY-123;
	m_rcUser[enCardType_SicboDoubleSix]=CRect(iX,iY,iX+iW,iY+iH);

	iW = 50;
	iH = 46;
	iX=nCenterX-148;
	iY = nCenterY-213;
	m_rcUser[enCardType_SicboThreeOne]=CRect(iX,iY,iX+iW,iY+iH);
	iX=nCenterX-99;
	m_rcUser[enCardType_SicboThreeTwo]=CRect(iX,iY,iX+iW,iY+iH);
	iX=nCenterX-50;
	m_rcUser[enCardType_SicboThreeThree]=CRect(iX,iY,iX+iW,iY+iH);
	iX=nCenterX-1;
	m_rcUser[enCardType_SicboThreeFour]=CRect(iX,iY,iX+iW,iY+iH);
	iX=nCenterX+48;
	m_rcUser[enCardType_SicboThreeFive]=CRect(iX,iY,iX+iW,iY+iH);
	iX=nCenterX+97;
	m_rcUser[enCardType_SicboThreeSix]=CRect(iX,iY,iX+iW,iY+iH);

	iW = 295;
	iH = 91;
	iX=nCenterX-148;
	iY = nCenterY-168;
	m_rcUser[enCardType_SicboThreeSame]=CRect(iX,iY,iX+iW,iY+iH);

	// �������ĵ�
	m_ptSicboAnim[enSicboType_Normal].SetPoint(nWidth/2, nHeight/2-30);
	m_ptSicboAnim[enSicboType_Desktop].SetPoint(nWidth/2-245, nHeight/2+95);
	m_ptSicboAnim[enSicboType_InMidair].SetPoint(nWidth/2-245, nHeight/2+95);
	m_ptSicboAnim[enSicboType_InTheAir].SetPoint(nWidth/2, nHeight/2-40);
	m_ptSicboAnim[enSicboType_Sicbo].SetPoint(nWidth/2+15, nHeight/2-100);

	m_ptApplyListPos.SetPoint(nWidth/2 -110,nHeight/2-333);

	int ExcursionY=10;
	for (BYTE i=0; i<COUNT_AZIMUTH; ++i)
	{
		//��������
		m_PointJettonNumber[i].SetPoint((m_rcUser[i].right+m_rcUser[i].left)/2, (m_rcUser[i].bottom+m_rcUser[i].top)/2-ExcursionY);

		//����λ��
		m_PointJetton[i].SetPoint((m_rcUser[i].left + (m_rcUser[i].right-m_rcUser[i].left)/2) , (m_rcUser[i].top + (m_rcUser[i].bottom-m_rcUser[i].top)/2));
	}

	//�ƶ��ؼ�
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	//���밴ť
	CRect rcJetton;
	m_btJetton100.GetWindowRect(&rcJetton);
	int nYPos = nHeight/2+194;
	int nXPos = nWidth/2-218;
	int nSpace = 3;

	DeferWindowPos(hDwp,m_btJetton100,NULL,nXPos,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton1000,NULL,nXPos + nSpace + rcJetton.Width(),nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton10000,NULL,nXPos + nSpace * 2 + rcJetton.Width() * 2,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton100000,NULL,nXPos + nSpace * 3 + rcJetton.Width() * 3,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton500000,NULL,nXPos + nSpace * 4 + rcJetton.Width() * 4,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton1000000,NULL,nXPos + nSpace * 5 + rcJetton.Width() * 5,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton5000000,NULL,nXPos + nSpace * 6 + rcJetton.Width() * 6,nYPos,0,0,uFlags|SWP_NOSIZE);

	//��ׯ��ť
	DeferWindowPos(hDwp,m_btApplyBanker,NULL,nWidth/2+271,nHeight/2-266,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btCancelBanker,NULL,nWidth/2+271,nHeight/2-266,0,0,uFlags|SWP_NOSIZE);

	DeferWindowPos(hDwp,m_btScoreMoveL,NULL,nWidth/2-265,nHeight/2+261,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btScoreMoveR,NULL,nWidth/2+240,nHeight/2+261,0,0,uFlags|SWP_NOSIZE);

	DeferWindowPos(hDwp,m_btBankerMoveUp,NULL,nWidth/2+119,nHeight/2-333,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btBankerMoveDown,NULL,nWidth/2+119,nHeight/2-280,0,0,uFlags|SWP_NOSIZE);

	DeferWindowPos(hDwp,m_btOpenBank,NULL,nWidth/2-358,nHeight/2+262,0,0,uFlags|SWP_NOSIZE);

	CRect rcSicboDesktop;
	m_btSicboDesktop.GetClientRect(&rcSicboDesktop);
	DeferWindowPos(hDwp,m_btSicboDesktop,NULL,nWidth/2+50,nHeight/2+115,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btSicboInMidair,NULL,nWidth/2+50 + rcSicboDesktop.Width() + 10,nHeight/2+115,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btSicboInTheAir,NULL,nWidth/2+50 + (rcSicboDesktop.Width() + 10)*2,nHeight/2+115,0,0,uFlags|SWP_NOSIZE);

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
	if (enCardType_Illegal != m_enSelArea)
	{
		m_PngSicboArea[m_enSelArea].DrawImage(pDC, m_rcUser[m_enSelArea].left+2, m_rcUser[m_enSelArea].top+2);
	}


	//ʱ����ʾ
	CImageHandle ImageHandleTimeFlag(&m_ImageTimeFlag);
	int nTimeFlagWidth = m_ImageTimeFlag.GetWidth()/3;
	int nFlagIndex=0;
	if (cbGameStatus==GS_FREE) nFlagIndex=0;
	else if (cbGameStatus==GS_PLACE_JETTON) nFlagIndex=1;
	else if (cbGameStatus==GS_GAME_END) nFlagIndex=2;
	m_ImageTimeFlag.AlphaDrawImage(pDC, nWidth/2+285, nHeight/2+300, nTimeFlagWidth, m_ImageTimeFlag.GetHeight(),
		nFlagIndex * nTimeFlagWidth, 0,RGB(255,0,255));

	//�����ע
	pDC->SetTextColor(RGB(255,234,0));
	//ʤ���߿�
	FlashJettonAreaFrame(nWidth,nHeight,pDC);

	//������Դ
	CSize SizeJettonItem(m_ImageJettonView.GetWidth()/JETTON_COUNT,m_ImageJettonView.GetHeight());

	// �滭����
	for (INT i=0;i<COUNT_AZIMUTH;i++)
	{
		//��������
		__int64 lScoreCount=0L;
		__int64 lScoreJetton[JETTON_COUNT]={100L,500L,1000L,10000L,50000L,100000L,500000L,1000000L,5000000L};

		//�滭����
		for (INT_PTR j=0;j<m_JettonInfoArray[i].GetCount();j++)
		{
			//��ȡ��Ϣ
			tagJettonInfo * pJettonInfo=&m_JettonInfoArray[i][j];

			//�ۼ�����
			ASSERT(pJettonInfo->cbJettonIndex<JETTON_COUNT);
			lScoreCount+=lScoreJetton[pJettonInfo->cbJettonIndex];

			//�滭����
			m_ImageJettonView.DrawImage(pDC, pJettonInfo->nXPos+m_PointJetton[i].x-SizeJettonItem.cx/2,
				pJettonInfo->nYPos+m_PointJetton[i].y-SizeJettonItem.cy/2,
				SizeJettonItem.cx,SizeJettonItem.cy,
				pJettonInfo->cbJettonIndex*SizeJettonItem.cx,0);
		}

		//�滭����
		//if (lScoreCount>0L)	DrawNumberString(pDC,lScoreCount,m_PointJettonNumber[i].x,m_PointJettonNumber[i].y);
	}

	//ׯ����Ϣ
	pDC->SetTextColor(RGB(255,234,0));

	//��ȡ���
	tagUserData const *pUserData = m_wBankerUser==INVALID_CHAIR ? NULL : GetUserInfo(m_wBankerUser);

	//λ����Ϣ
	CRect StrRect;
	StrRect.left = nWidth/2-300;
	StrRect.top = nHeight/2 - 332;
	StrRect.right = StrRect.left + 190;
	StrRect.bottom = StrRect.top + 13;
	

	//ׯ������
	pDC->DrawText(pUserData==NULL?(m_bEnableSysBanker?TEXT("ϵͳ��ׯ"):TEXT("������ׯ")):pUserData->szName, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_SINGLELINE );

	//ׯ���ܷ�
	CString strBankerTotalScore;
	StrRect.left = nWidth/2-300;
	StrRect.top = nHeight/2 - 310;
	StrRect.right = StrRect.left + 190;
	StrRect.bottom = StrRect.top + 13;
	DrawNumberStringWithSpace(pDC,pUserData==NULL?0:pUserData->lScore, StrRect);

	//ׯ�Ҿ���
	CString strBankerTime;
	strBankerTime.Format( "%d", m_wBankerTime );
	StrRect.left = nWidth/2-300;
	StrRect.top = nHeight/2 - 287;
	StrRect.right = StrRect.left + 190;
	StrRect.bottom = StrRect.top + 13;
	DrawNumberStringWithSpace(pDC,m_wBankerTime,StrRect);

	//ׯ�ҳɼ�
	StrRect.left = nWidth/2-300;
	StrRect.top = nHeight/2 - 264;
	StrRect.right = StrRect.left + 190;
	StrRect.bottom = StrRect.top + 13;
	DrawNumberStringWithSpace(pDC,m_lBankerWinScore,StrRect);

	// ����ׯ���б�
	for (int i=0; i<SHOW_APPLY_BANKER; ++i)
	{
		int iIndex = m_iApplyFirst +i;
		if ((iIndex<0) || (iIndex>=m_ApplyUserArray.GetCount()))
		{
			break;
		}
		// λ��
		CRect rc;
		rc.left = m_ptApplyListPos.x;
		rc.top = m_ptApplyListPos.y + i*14;
		rc.right = rc.left + 100;
		rc.bottom = rc.top + 14;
		pDC->DrawText(m_ApplyUserArray[iIndex].strUserName, rc, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_SINGLELINE );
		rc.left = rc.right + 5;
		rc.right = rc.left + 120;
		DrawNumberStringWithSpace(pDC,m_ApplyUserArray[iIndex].lUserScore,rc);
	}

	//�滭�û�
	if (m_wMeChairID!=INVALID_CHAIR)
	{
		const tagUserData *pMeUserData = GetUserInfo(m_wMeChairID);
		if ( pMeUserData != NULL )
		{
			//��Ϸ��Ϣ
			TCHAR szResultScore[16]=TEXT("");
			TCHAR szGameScore[16]=TEXT("");
			__int64 lMeJetton = 0;
			for (BYTE i=0; i<COUNT_AZIMUTH; ++i)
			{
				lMeJetton += m_lMeScore[i];
			}

			CRect rcAccount(CPoint(nWidth/2+206,nHeight/2 - 334),CPoint(nWidth/2+206+190 ,nHeight/2 - 334+13));
			CRect rcGameScore(CPoint(nWidth/2+206, nHeight/2 - 309),CPoint(nWidth/2+206+190, nHeight/2 - 309+13));
			CRect rcResultScore(CPoint(nWidth/2+206,nHeight/2 - 287),CPoint(nWidth/2+206+190,nHeight/2 - 287+13));
			
			DrawNumberStringWithSpace(pDC,pMeUserData->lScore-lMeJetton,rcGameScore);
			DrawNumberStringWithSpace(pDC,m_lMeStatisticScore,rcResultScore);

			pDC->DrawText(pMeUserData->szName,lstrlen(pMeUserData->szName),rcAccount,DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_SINGLELINE);
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

	if ( m_bShowChangeSicbo )
	{
		int	nXPos = nWidth / 2 - 130;
		int	nYPos = nHeight / 2 - 160;

		//������ׯ
		if ( enSicboType_Desktop == m_enSicboType )
		{
			CImageHandle ImageHandleDesktop(&m_ImageSicboDesktop);
			m_ImageSicboDesktop.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);
		}
		else if ( enSicboType_InMidair == m_enSicboType )
		{
			CImageHandle ImageHandleInMidair(&m_ImageSicboInMidair);
			m_ImageSicboInMidair.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);
		}
		else if ( enSicboType_InTheAir == m_enSicboType )
		{
			CImageHandle ImageHandleInTheAir(&m_ImageSicboInTheAir);
			m_ImageSicboInTheAir.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);
		}
	}


	//�ҵ���ע
	//DrawMeJettonNumber(pDC);

	//�滭ʱ��
	if (m_wMeChairID!=INVALID_CHAIR)
	{
		WORD wUserTimer=GetUserTimer(m_wMeChairID);
		if (wUserTimer!=0) DrawUserTimer(pDC,nWidth/2+323,nHeight/2+278,wUserTimer);
	}

	//ʤ����־
	DrawWinFlags(pDC);

	//��ʾ���
	ShowGameResult(pDC, nWidth, nHeight);
	
	//��ըЧ��
	DrawBombEffect(pDC);

	// ҡ���Ӷ���
	DrawSicboAnim(pDC);
	m_TCPrompt.Invalidate();
	//m_TCPrompt.ShowWindow(SW_SHOW);
	return;
}

//������Ϣ
void CGameClientView::SetGameMaxScore(__int64 lMeMaxScore, __int64 lAllMaxScore)
{
	if (m_lMeMaxScore!=lMeMaxScore)
	{
		//���ñ���
		m_lMeMaxScore=lMeMaxScore;
	}
	if (m_lAllMaxScore != lAllMaxScore)
	{
		m_lAllMaxScore = lAllMaxScore;
	}
	return;
}

//�����ע
void CGameClientView::SetAreaLimitScore(__int64 lAreaLimitScore)
{
	if ( m_lAreaLimitScore!= lAreaLimitScore )
	{
		//���ñ���
		m_lAreaLimitScore=lAreaLimitScore;
	}
}

//���ó���
void CGameClientView::SetCurrentJetton(__int64 lCurrentJetton)
{
	//���ñ���
	ASSERT(lCurrentJetton>=0L);
	if ((m_lCurrentJetton>0)||(0 == lCurrentJetton))
	{
		m_lCurrentJetton=lCurrentJetton;
	}	
	return;
}

//��ʷ��¼
void CGameClientView::SetGameHistory(tagClientGameRecord &gameRecord)
{
	//��������
	tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[m_nRecordLast];
	ClientGameRecord.bFlags = gameRecord.bFlags;
	for (BYTE i=0; i<MAX_COUNT_SICBO; ++i)
	{
		ClientGameRecord.iCards[i] = gameRecord.iCards[i];
	}

	//�ƶ��±�
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst )
	{
		m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
		if ( m_nScoreHead < m_nRecordFirst ) m_nScoreHead = m_nRecordFirst;
	}

	int nHistoryCount = (m_nRecordLast - m_nRecordFirst + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
	if ( SHOW_SCORE_HISTORY < nHistoryCount )
	{
		m_btScoreMoveR.EnableWindow(TRUE);

		m_nScoreHead = (m_nRecordLast - SHOW_SCORE_HISTORY + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
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

	//������ע
	ZeroMemory(m_lMeScore, sizeof(m_lMeScore));

	//ȫ����ע
	ZeroMemory(m_lAllScore, sizeof(m_lAllScore));

	m_strDispatchCardTips=TEXT("");
	
	//���½���
	UpdateGameView(NULL);

	return;
}

//������ע
void CGameClientView::SetMePlaceJetton(BYTE cbViewIndex, __int64 lJettonCount)
{
	//Ч�����
	ASSERT(cbViewIndex<COUNT_AZIMUTH);
	if (cbViewIndex>=COUNT_AZIMUTH) return;

	m_lMeScore[cbViewIndex]=lJettonCount;

	//���½���
	UpdateGameView(NULL);
}

//�����˿�
void CGameClientView::SetCardInfo(E_SICBO_NUMBER enCards[MAX_COUNT_SICBO],bool bWinner[COUNT_AZIMUTH])
{
	m_cbWinnerSide=0xFF;
	if (NULL != enCards)
	{
		CopyMemory(m_enCards,enCards,sizeof(m_enCards));
		CopyMemory(m_bWinner,bWinner, sizeof(m_bWinner));
		m_bSicboShow = false;

		//��ʼ����
		DispatchCard();
	}
	else
	{
		ZeroMemory(m_enCards,sizeof(m_enCards));
		ZeroMemory(m_bWinner,sizeof(m_bWinner));
		UpdateGameView(NULL);
	}
}

//���ó���
void CGameClientView::PlaceUserJetton(BYTE cbViewIndex, __int64 lScoreCount)
{
	//Ч�����
	ASSERT(cbViewIndex<COUNT_AZIMUTH);
	if (cbViewIndex>=COUNT_AZIMUTH) return;

	//����ը��
	if (lScoreCount==5000000L)
	{
		SetBombEffect(true,cbViewIndex);
	}

	//��������
	bool bPlaceJetton=false;
	__int64 lScoreIndex[JETTON_COUNT]={100L,500L,1000L,10000L,50000L,100000L,500000L,1000000L,5000000L};

	//�߿���
	int nFrameWidth=0, nFrameHeight=0;
	int nBorderWidth=6;
	m_lAllScore[cbViewIndex] += lScoreCount;
	nFrameWidth = m_rcUser[cbViewIndex].right-m_rcUser[cbViewIndex].left;
	nFrameHeight = m_rcUser[cbViewIndex].bottom-m_rcUser[cbViewIndex].top;
	nFrameWidth -= nBorderWidth;
	nFrameHeight -=  nBorderWidth;

	//���ӳ���
	for (BYTE i=0;i<CountArray(lScoreIndex);i++)
	{
		//������Ŀ
		BYTE cbScoreIndex=JETTON_COUNT-i-1;
		__int64 lCellCount=lScoreCount/lScoreIndex[cbScoreIndex];

		//�������
		if (lCellCount==0L) continue;

		//�������
		for (__int64 j=0;j<lCellCount;j++)
		{
			//�������
			tagJettonInfo JettonInfo;
			int nJettonSize=48;
			JettonInfo.cbJettonIndex=cbScoreIndex;
			int iRand = nFrameWidth-nJettonSize;
			if (iRand<1)
			{
				iRand= 1;
			}
			JettonInfo.nXPos=rand()%iRand - iRand/2;

			iRand = nFrameHeight-nJettonSize-15;
			if (iRand<1)
			{
				iRand= 1;
			}
			JettonInfo.nYPos=rand()%iRand - iRand/2;

			//��������
			bPlaceJetton=true;
			m_JettonInfoArray[cbViewIndex].Add(JettonInfo);
		}

		//������Ŀ
		lScoreCount-=lCellCount*lScoreIndex[cbScoreIndex];
	}

	//���½���
	if (bPlaceJetton==true) UpdateGameView(NULL);

	return;
}

//���ֳɼ�
void CGameClientView::SetCurGameScore(__int64 lMeCurGameScore, __int64 lMeCurGameReturnScore, __int64 lBankerCurGameScore, __int64 lGameRevenue)
{
	m_lMeCurGameScore=lMeCurGameScore;			
	m_lMeCurGameReturnScore=lMeCurGameReturnScore;			
	m_lBankerCurGameScore=lBankerCurGameScore;			
	m_lGameRevenue=lGameRevenue;					
}

//����ʤ��
void CGameClientView::SetWinnerSide(BYTE cbWinnerSide)
{
	m_cbWinnerSide=cbWinnerSide;
	//����ʱ��
	if (cbWinnerSide!=0xFF)
	{
		SetTimer(IDI_FLASH_WINNER,500,NULL);
	}
	else
	{
		KillTimer(IDI_FLASH_WINNER);
		m_cbAreaFlash = 0xFF;
	}

	//���½���
	UpdateGameView(NULL);

	return;
}

//��ȡ����
BYTE CGameClientView::GetJettonArea(CPoint MousePoint)
{
	for (BYTE i=0; i<COUNT_AZIMUTH; ++i)
	{
		if (m_rcUser[i].PtInRect(MousePoint))
		{
			return i;
		}
	}
	return 0xFF;
}

//�滭����
void CGameClientView::DrawNumberString(CDC * pDC, __int64 lNumber, INT nXPos, INT nYPos, bool bMeScore)
{
	//������Դ
	CImageHandle HandleScoreNumber(&m_ImageScoreNumber);
	CImageHandle HandleMeScoreNumber(&m_ImageMeScoreNumber);
	CSize SizeScoreNumber(m_ImageScoreNumber.GetWidth()/10,m_ImageScoreNumber.GetHeight());

	if ( bMeScore ) SizeScoreNumber.SetSize(m_ImageMeScoreNumber.GetWidth()/10, m_ImageMeScoreNumber.GetHeight());

	//������Ŀ
	LONG lNumberCount=0;
	__int64 lNumberTemp=lNumber;
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
		int lCellNumber=(int)(lNumber%10);
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
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, __int64 lNumber, INT nXPos, INT nYPos)
{
	CString strNumber=TEXT("");
	strNumber.Format(TEXT("%I64d"),lNumber>0?lNumber:-lNumber);
	//int p=strNumber.GetLength()-4;
	//while(p>0)
	//{
	//	strNumber.Insert(p," ");
	//	p-=4;
	//}
	if(lNumber<0)
	{
		strNumber=TEXT("-")+strNumber;
	}
	//�������
	pDC->TextOut(nXPos,nYPos,strNumber);
}

//�滭����
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, __int64 lNumber, CRect rcRect, INT nFormat)
{
	CString strNumber=TEXT("");

	strNumber.Format(TEXT("%I64d"),lNumber>0?lNumber:-lNumber);
	//int p=strNumber.GetLength()-4;
	//while(p>0)
	//{
	//	strNumber.Insert(p," ");
	//	p-=4;
	//}
	if(lNumber<0)
	{
		strNumber=TEXT("-")+strNumber;
	}

	//�������
	if (nFormat==-1)
	{
		pDC->DrawText(strNumber,rcRect,DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);
	}
	else
	{
		pDC->DrawText(strNumber,rcRect,nFormat);
	}
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
void CGameClientView::OnJettonButton500000()
{
	//���ñ���
	m_lCurrentJetton=500000L;

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
// �ƶ�ׯ��
void CGameClientView::OnBtnBankerMoveUp()
{
	CSingleLock sigle(&m_ApplyBankerLock);
	sigle.Lock();
	if (m_iApplyFirst>0)
	{
		m_iApplyFirst --;
		UpdateGameView(NULL);
	}
	m_btBankerMoveUp.EnableWindow((m_iApplyFirst>0));
	m_btBankerMoveDown.EnableWindow(((m_ApplyUserArray.GetCount()-m_iApplyFirst)>SHOW_APPLY_BANKER));
}

void CGameClientView::OnBtnBankerMoveDown()
{
	CSingleLock sigle(&m_ApplyBankerLock);
	sigle.Lock();
	if ((m_ApplyUserArray.GetCount()-m_iApplyFirst)>SHOW_APPLY_BANKER)
	{
		m_iApplyFirst ++;
		UpdateGameView(NULL);
	}
	m_btBankerMoveUp.EnableWindow((m_iApplyFirst>0));
	m_btBankerMoveDown.EnableWindow(((m_ApplyUserArray.GetCount()-m_iApplyFirst)>SHOW_APPLY_BANKER));
}

//��ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{
	//����ʤ��
	if (nIDEvent==IDI_FLASH_WINNER)
	{
		//���ñ���
		if (0xFF != m_cbWinnerSide)
		{
			if (0x01 != m_cbAreaFlash)
			{
				m_cbAreaFlash = 0x01;
			}
			else
			{
				m_cbAreaFlash = 0x00;
			}
		}
		else
		{
			m_cbAreaFlash = 0xFF;
			KillTimer(IDI_FLASH_WINNER);
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
	// ����ҡ���ӷ�ʽ
	if (IDI_SHOW_CHANGE_SICBO == nIDEvent)
	{
		KillTimer(nIDEvent);
		m_bShowChangeSicbo = false;
		UpdateGameView(NULL);
		return ;
	}
	else if (IDI_DISPATCH_CARD == nIDEvent)
	{
		//ֹͣ�ж�
		if (m_bSicboEffect==false)
		{
			KillTimer(nIDEvent);
			return;
		}
		int iCountFrame = ((enSicboType_Normal==m_enSicboType)
			||(enSicboType_InTheAir == m_enSicboType))?SICBO_EFFECT_COUNT:SICBO_EFFECT_FRAME_COUNT;
		if (enSicboType_Sicbo == m_enSicboType)
		{
			iCountFrame = 40;
		}
		if ((m_bySicboFrameIndex+1)>=iCountFrame)
		{
			//ɾ��ʱ��
			KillTimer(nIDEvent);
			m_bShowSicboEffectBack = false;
			FinishDispatchCard();
		}
		else
		{
			++m_bySicboFrameIndex;
		}

		//���½���
		UpdateGameView(NULL);

		return;
	}

	//��ը����
	if ((nIDEvent > IDI_BOMB_EFFECT+enCardType_Illegal)
		&& (nIDEvent < IDI_BOMB_EFFECT+COUNT_AZIMUTH))
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
		else
		{
			++m_cbBombFrameIndex[wIndex];
		}

		//���½���
		UpdateGameView(NULL);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	if (m_lCurrentJetton!=0L)
	{
		BYTE cbJettonArea=GetJettonArea(Point);

		//��С�ж�
		__int64 lMaxPlayerScore = GetMaxPlayerScore(cbJettonArea);
		if ( lMaxPlayerScore < m_lCurrentJetton )
		{
			return ;
		}
		
		if (cbJettonArea!=0xFF) 
			AfxGetMainWnd()->SendMessage(IDM_PLACE_JETTON,cbJettonArea,(LPARAM)m_lCurrentJetton);
	}
	__super::OnLButtonDown(nFlags,Point);
}

//�����Ϣ
void CGameClientView::OnRButtonDown(UINT nFlags, CPoint Point)
{
	//���ñ���
	m_lCurrentJetton=0L;
	m_enSelArea = enCardType_Illegal;

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
	if (NULL == m_pGameClientDlg)
	{
		m_enSelArea = enCardType_Illegal;
		m_lSelAreaMax = 0L;
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_NORMAL)));
		return true;
	}
	E_CARD_TYPE enArea = enCardType_Illegal;
	for (int i=0;i<COUNT_AZIMUTH; ++i)
	{
		//��ȡ����
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);
		if (m_rcUser[i].PtInRect(MousePoint))
		{
			enArea = (E_CARD_TYPE)(i);
			break;
		}
	}	
	if (enArea != m_enSelArea)
	{
		m_TCPrompt.ShowWindow(SW_HIDE);
		m_TCPrompt.MoveWindow(0,0,0,0);
		m_enSelArea = enArea;		
		UpdateGameView(NULL);
	}
	if ((enCardType_Illegal != m_enSelArea) && (GS_PLACE_JETTON == m_pGameClientDlg->GetGameStatus()))
	{
		m_lSelAreaMax = GetMaxPlayerScore(m_enSelArea);
		if (m_lSelAreaMax < 0)
		{
			m_lSelAreaMax = 0L;
		}		
		UpdateGameView(NULL);
	}
	if (enCardType_Illegal != m_enSelArea)
	{
		INT iSrcX = m_TCPrompt.m_PngBack.GetWidth()/2;
		INT iXPos = m_rcUser[m_enSelArea].left + m_rcUser[m_enSelArea].Width()/2 -10;
		INT iYPos = m_rcUser[m_enSelArea].top - m_TCPrompt.m_PngBack.GetHeight() + 20;
		switch (m_enSelArea)
		{
		case enCardType_Big:              case enCardType_Double:           case enCardType_NumberThirteen:
		case enCardType_NumberFourteen:   case enCardType_NumberFifteen:    case enCardType_NumberSixteen:
		case enCardType_NumberSeventeen:  case enCardType_SicboSix:         case enCardType_SicboFive:
		case enCardType_SicboThreeAndFour:case enCardType_SicboThreeAndFive:case enCardType_SicboThreeAndSix:
		case enCardType_SicboFourAndFive: case enCardType_SicboFourAndSix:  case enCardType_SicboFiveAndSix:
		case enCardType_SicboDoubleFour:  case enCardType_SicboDoubleFive:  case enCardType_SicboDoubleSix:
		case enCardType_SicboThreeFive:   case enCardType_SicboThreeSix:
			{
				iXPos = m_rcUser[m_enSelArea].left - iSrcX +m_rcUser[m_enSelArea].Width()/2 + 10;
				iSrcX = 0; break;				
			}
		default: break;
		}

		TCHAR szText[_MAX_PATH]={0};
		if (GS_PLACE_JETTON == m_pGameClientDlg->GetGameStatus())
		{		
			_sntprintf(szText, sizeof(szText), _T("���·�  ��%I64d\r\n���·�  ��%I64d\r\n�ҵ��·֣�%I64d"),
				m_lSelAreaMax, m_lAllScore[m_enSelArea], m_lMeScore[m_enSelArea]);
			m_TCPrompt.SetWindowText(szText);
		}
		else
		{
			m_TCPrompt.SetWindowText(g_szPrompt[m_enSelArea]);
		}
		CRect rc;
		rc = CRect(iXPos,iYPos, iXPos + m_TCPrompt.m_PngBack.GetWidth()/2,iYPos + m_TCPrompt.m_PngBack.GetHeight());
		m_TCPrompt.m_iXPos = iSrcX;
		m_TCPrompt.MoveWindow(iXPos,iYPos, m_TCPrompt.m_PngBack.GetWidth()/2, m_TCPrompt.m_PngBack.GetHeight(), TRUE);	
		m_TCPrompt.ShowWindow(SW_SHOW);
	}

	if (m_lCurrentJetton!=0L)
	{
		//�����ж�
		if (enCardType_Illegal == m_enSelArea)
		{
			SetCursor(LoadCursor(NULL,IDC_NO));
			return TRUE;
		}

		//��С�ж�
		OUTPUT_DEBUG_STRING("client %d  ���� �������� %I64d", m_enSelArea, m_lSelAreaMax);
		if ( m_lSelAreaMax < m_lCurrentJetton )
		{
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
		default: break;
		}
	}
	else
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_NORMAL)));
		return TRUE;
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

//�ı�ҡ���ӷ�ʽ
void CGameClientView::SetSicboType(E_SICBO_TYPE enSicboType)
{
	m_enSicboType = enSicboType;
	if (enSicboType_Normal != m_enSicboType)
	{
		SetTimer( IDI_SHOW_CHANGE_SICBO, 3000, NULL );
		m_bShowChangeSicbo = true;
	}
	UpdateGameView(NULL);
}

void CGameClientView::AddApplyBanker(tagApplyUser &applyUser)
{
	CSingleLock sigle(&m_ApplyBankerLock);
	sigle.Lock();
	int iFind = -1;
	for (int i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		if (0 == m_ApplyUserArray[i].strUserName.Compare(applyUser.strUserName))
		{
			iFind = i;
			break;
		}
	}
	if (iFind > -1)  // �Ѿ��ڶ���������Ҫ����
	{
		return ;
	}
//#ifdef _DEBUG
//	static int iTEs = 0;
//	if (0 == iTEs)
//	{
//		iTEs ++;
//		for (int i=0; i<20; ++i)
//		{
//			tagApplyUser at;
//			at.strUserName.Format("%02d er��rjL�ˡ�f", i);
//			at.lUserScore= 8015221665796676762;
//			AddApplyBanker(at);
//		}
//	}
//#endif
	m_ApplyUserArray.Add(applyUser);
	UpdateGameView(NULL);
	m_btBankerMoveUp.EnableWindow((m_iApplyFirst>0));
	m_btBankerMoveDown.EnableWindow(((m_ApplyUserArray.GetCount()-m_iApplyFirst)>SHOW_APPLY_BANKER));
}

void CGameClientView::CancelApplyBanker(tagApplyUser &applyUser)
{
	CSingleLock sigle(&m_ApplyBankerLock);
	sigle.Lock();
	int iFind = -1;
	for (int i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		if (0 == m_ApplyUserArray[i].strUserName.Compare(applyUser.strUserName))
		{
			m_ApplyUserArray.RemoveAt(i);
			iFind = i;
			break;
		}
	}
	if (iFind < 0)  // û��ɾ��
	{
		return ;
	}
	// ɾ��������ʾ��Χ
	if (m_ApplyUserArray.GetCount() <= SHOW_APPLY_BANKER)
	{
		m_iApplyFirst = 0;
	}
	else
	{
		if ((m_ApplyUserArray.GetCount()-m_iApplyFirst) < SHOW_APPLY_BANKER)
		{
			m_iApplyFirst = (int)(m_ApplyUserArray.GetCount() - SHOW_APPLY_BANKER);
		}
		if (m_iApplyFirst < 0)
		{
			m_iApplyFirst = 0;
		}
	}
	UpdateGameView(NULL);
	m_btBankerMoveUp.EnableWindow((m_iApplyFirst>0));
	m_btBankerMoveDown.EnableWindow(((m_ApplyUserArray.GetCount()-m_iApplyFirst)>SHOW_APPLY_BANKER));
}

void CGameClientView::UpdateApplyBanker(tagApplyUser &applyUser)
{
	for (int i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		if (0 == m_ApplyUserArray[i].strUserName.Compare(applyUser.strUserName))
		{
			m_ApplyUserArray[i].lUserScore = applyUser.lUserScore;
			UpdateGameView(NULL);
			break;
		}
	}
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
void CGameClientView::SetBankerInfo(WORD wBankerUser, __int64 lBankerScore) 
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
	CImageHandle ImageHandleSicboNumber(&m_ImageCardDot);
	int nIndex = m_nScoreHead;
	COLORREF clrOld ;
	clrOld = pDC->SetTextColor(RGB(255,234,0));
	int iHistoryCount = abs((m_nRecordLast - m_nRecordFirst + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY);
	iHistoryCount = iHistoryCount>=SHOW_SCORE_HISTORY?0:(SHOW_SCORE_HISTORY-iHistoryCount);
	CString strPoint;
	int nDrawCount = 0;
	while ( nIndex != m_nRecordLast && ( m_nRecordLast!=m_nRecordFirst ) && nDrawCount < SHOW_SCORE_HISTORY )
	{
		tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[nIndex];

		int nXPos = m_nWinFlagsExcursionX + (((nIndex - m_nScoreHead + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY) + iHistoryCount) * 30;
		int nYPos = 0;
		int nFlagsIndex = 0;
		int iCardDot = 0;
		// ��ʾ3������
		for (BYTE i=0; i<MAX_COUNT_SICBO; ++i)
		{
			nYPos = m_nWinFlagsExcursionY + ((m_ImageWinFlags.GetHeight()/2 - 3)*i);
			nFlagsIndex = ClientGameRecord.iCards[i]-1;
			//OUTPUT_DEBUG_STRING("���� %d  index %d",ClientGameRecord.iCards[i], i);
			iCardDot += ClientGameRecord.iCards[i];

			m_ImageWinFlags.AlphaDrawImage(pDC, nXPos, nYPos, m_ImageWinFlags.GetWidth()/6 , 
				m_ImageWinFlags.GetHeight()/2,m_ImageWinFlags.GetWidth()/6 * nFlagsIndex,
				ClientGameRecord.bFlags?0:(m_ImageWinFlags.GetHeight()/2), RGB(255, 0, 255));
		}
		nYPos += 23;
		nYPos += 2;
		m_ImageCardDot.AlphaDrawImage(pDC, nXPos, nYPos,m_ImageCardDot.GetWidth()/2, m_ImageCardDot.GetHeight(),
			ClientGameRecord.bFlags?0:m_ImageCardDot.GetWidth()/2, 0, RGB(255, 0, 255));
		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
		nDrawCount++;
	}

	pDC->SetTextColor(clrOld);
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
	if ( SHOW_SCORE_HISTORY == nHistoryCount)
	{
		return;
	}

	m_nScoreHead = ( m_nScoreHead + 1 ) % MAX_SCORE_HISTORY;
	if ( SHOW_SCORE_HISTORY == (nHistoryCount-1) )
	{
		m_btScoreMoveR.EnableWindow(FALSE);
	}

	m_btScoreMoveL.EnableWindow(TRUE);

	//���½���
	UpdateGameView(NULL);
}

// ����ҡ����
void CGameClientView::OnBtnSicboDesktop()
{
	AfxGetMainWnd()->SendMessage(IDM_CHANGE_SICBO_TYPE,enSicboType_Desktop,0);
}
// ���ҡ����
void CGameClientView::OnBtnSicboInMidair()
{
	AfxGetMainWnd()->SendMessage(IDM_CHANGE_SICBO_TYPE,enSicboType_InMidair,0);
}
// ����ҡ����
void CGameClientView::OnBtnSicboInTheAir()
{
	AfxGetMainWnd()->SendMessage(IDM_CHANGE_SICBO_TYPE,enSicboType_InTheAir,0);
}

// ���в���
void CGameClientView::OnBtnOpenBank()
{
	AfxGetMainWnd()->SendMessage(IDM_OPEN_BANK,0,0);
}
//��ʾ���
void CGameClientView::ShowGameResult(CDC *pDC, int nWidth, int nHeight)
{
	//��ʾ�ж�
	if (m_pGameClientDlg->GetGameStatus()!=GS_GAME_END)
		return;

	CImageHandle ImageHandleSicbo(&m_ImageSicboPoint);
	CImageHandle ImageHandleCardsType(&m_ImageCardType);

	if (m_bSicboShow)
	{
		//// �����ӵ���
		INT nImageHeight=m_ImgSicboEffectResult.GetHeight()/4;
		INT nImageWidth=m_ImgSicboEffectResult.GetWidth()/6;

		// y������С���Ȼ���x����С��Ҳ�Ȼ�				
		for (BYTE j=0; j<MAX_COUNT_SICBO; ++j)
		{
			m_ImgSicboEffectResult.DrawImage(pDC, 
				m_ptSicboAnim[enSicboType_Sicbo].x+m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.x-nImageWidth/2,
				m_ptSicboAnim[enSicboType_Sicbo].y+m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.y-nImageHeight/2 + 100,
				nImageWidth, nImageHeight,
				m_SicboAnimInfo[m_bySicboDraw[j]].byResultX*nImageWidth,
				m_SicboAnimInfo[m_bySicboDraw[j]].byResultY*nImageHeight);
		}
	}

	//�����ж�,���ҷ�������
	if ((m_pGameClientDlg->GetGameStatus()==GS_GAME_END) && m_bSicboShow)
	{
		int	nXPos = nWidth / 2 - 129;
		int	nYPos = nHeight / 2 - 220;
		m_PngGameEnd.DrawImage(pDC, nXPos,nYPos);

		pDC->SetTextColor(RGB(255,234,0));
		CRect rcMeWinScore, rcMeReturnScore;
		rcMeWinScore.left = nXPos+25 + 40;
		rcMeWinScore.top = nYPos+20 + 32;
		rcMeWinScore.right = rcMeWinScore.left + 111;
		rcMeWinScore.bottom = rcMeWinScore.top + 34;

		rcMeReturnScore.left = nXPos+25 + 150;
		rcMeReturnScore.top = nYPos+20 + 32;
		rcMeReturnScore.right = rcMeReturnScore.left + 111;
		rcMeReturnScore.bottom = rcMeReturnScore.top + 34;

		CString strMeGameScore, strMeReturnScore;
		DrawNumberStringWithSpace(pDC,m_lMeCurGameScore,rcMeWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
		DrawNumberStringWithSpace(pDC,m_lMeCurGameReturnScore,rcMeReturnScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);

		CRect rcBankerWinScore;
		rcBankerWinScore.left = nXPos+25 + 40;
		rcBankerWinScore.top = nYPos+20 + 69;
		rcBankerWinScore.right = rcBankerWinScore.left + 111;
		rcBankerWinScore.bottom = rcBankerWinScore.top + 34;
		DrawNumberStringWithSpace(pDC,m_lBankerCurGameScore,rcBankerWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	}
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
__int64 CGameClientView::GetMaxPlayerScore(int iArea)
{
	if ((iArea < 0) || (iArea>=COUNT_AZIMUTH))
	{
		return 0L;
	}
	__int64 i64UserAllScore = 0; // ����Լ��Ѿ���ע�ķ�
	__int64 i64AllUserAllScore=0;// ���������ע�� 
	__int64 i64AllLossScore=0;   // ׯ��������
	__int64 i64LoseAreaScore=0;
	for (int i=0; i<COUNT_AZIMUTH; ++i)
	{
		i64UserAllScore += m_lMeScore[i];
		i64AllUserAllScore += m_lAllScore[i];
	}

	for (int i=1; i<=MAX_SICBO_NUMBER; ++i)
	{
		for (int j=i; j<=MAX_SICBO_NUMBER; ++j)
		{
			for (int h=j; h<=MAX_SICBO_NUMBER; ++h)
			{
				E_SICBO_NUMBER enBuffer[]={(E_SICBO_NUMBER)i,(E_SICBO_NUMBER)j,(E_SICBO_NUMBER)h};
				E_CARD_TYPE enCardType[COUNT_AZIMUTH];
				int iCount = 0;
				m_GameLogic.GetCardType(enBuffer,enCardType, iCount);
				__int64 i64AllLossScoreTemp=0;
				__int64 i64LoseAreaScoreTemp=0;
				bool bFind = false;
				for (int l=0; l<iCount; ++l)
				{
					if (enCardType_Illegal == enBuffer[l])
					{
						continue;
					}
					if (iArea == enCardType[l])
					{
						bFind = true;
					}
				}
				if (!bFind)
				{
					continue;
				}

				// ��������������Ǯ
				for (int l=0; l<iCount; ++l)
				{
					if (enCardType_Illegal == enCardType[l])
					{
						continue;
					}
					i64LoseAreaScoreTemp += m_lAllScore[enCardType[l]];
					if ((enCardType[l]<enCardType_SicboOne) ||(enCardType[l]>enCardType_SicboSix))
					{
						i64AllLossScoreTemp += (m_lAllScore[enCardType[l]] * m_GameLogic.m_i64Loss_Percent[enCardType[l]]);
					}
					else
					{
						// ��Ҫ�������
						E_SICBO_NUMBER enSicboNum = (E_SICBO_NUMBER)(enCardType[l]-enCardType_SicboOne+1);
						int iCountNum = m_GameLogic.GetSicboCountByNumber(enBuffer, enSicboNum);
						int iMuti[]={0,2,3,4};
						i64AllLossScoreTemp += (m_lAllScore[enCardType[l]] * iMuti[iCountNum]);
					}
				}

				if (i64AllLossScoreTemp > i64AllLossScore)
				{
					i64AllLossScore = i64AllLossScoreTemp;
				}
				if (i64LoseAreaScoreTemp>i64LoseAreaScore)
				{
					i64LoseAreaScore = i64LoseAreaScoreTemp;
				}
			}
		}
	}
	//OUTPUT_DEBUG_STRING(_T("client %d������עׯ������� %I64d"),iArea, i64AllLossScore);

	// ��ȡ��������
	__int64 lAllMaxScore = m_lAllMaxScore;
	__int64 lUserMaxScore = m_lMeMaxScore;

	// ��ȥ�Ѿ���ע��
	lUserMaxScore -= i64UserAllScore;
	if (lUserMaxScore<=0)
	{
		return 0L;
	}

	// ׯ�ҿ������Ǯ
	lAllMaxScore = lAllMaxScore - i64LoseAreaScore + i64AllUserAllScore;
	lAllMaxScore -= i64AllLossScore;
	if ((iArea<enCardType_SicboOne) ||(iArea>enCardType_SicboSix))
	{
		lAllMaxScore = lAllMaxScore/(m_GameLogic.m_i64Loss_Percent[iArea] -1);
	}
	else
	{
		lAllMaxScore = lAllMaxScore/3; // ���������
	}
	if (lAllMaxScore<=0)
	{
		return 0L;
	}
	lUserMaxScore = min(lAllMaxScore, lUserMaxScore);
	return lUserMaxScore;
}

//�ɼ�����
void CGameClientView::SetGameScore(__int64 lMeCurGameScore, __int64 lMeCurGameReturnScore, __int64 lBankerCurGameScore)
{
	m_lMeCurGameScore=lMeCurGameScore;
	m_lMeCurGameReturnScore=lMeCurGameReturnScore;
	m_lBankerCurGameScore=lBankerCurGameScore;	
}

//�滭����
void CGameClientView::DrawMeJettonNumber(CDC *pDC)
{
	//�滭����
	for (BYTE i=0; i<COUNT_AZIMUTH; ++i)
	{
		if (0 < m_lMeScore[i])
		{
			DrawNumberString(pDC,m_lMeScore[i],m_PointJettonNumber[i].x,m_PointJettonNumber[i].y+25, true);
		}
	}
}

//��ʼ����
void CGameClientView::DispatchCard()
{
	if ((enSicboType_Desktop == m_enSicboType)||(enSicboType_InMidair == m_enSicboType))
	{
		m_bSicboEffect=true;
		m_bShowSicboEffectBack = true;
		m_bySicboFrameIndex=0;
		// �����߸�����ÿ�����߸����������
		CRect rect[7];
		rect[0].left = -4;
		rect[0].top = -3;
		rect[0].right = rect[0].left + 3;
		rect[0].bottom = rect[0].top + 3;

		rect[1].left = -21;
		rect[1].top = -3;
		rect[1].right = rect[1].left + 3;
		rect[1].bottom = rect[1].top + 3;

		rect[2].left = +12;
		rect[2].top = -3;
		rect[2].right = rect[2].left + 3;
		rect[2].bottom = rect[2].top + 3;

		rect[3].left = -12;
		rect[3].top = -13;
		rect[3].right = rect[3].left + 3;
		rect[3].bottom = rect[3].top + 3;

		rect[4].left = +6;
		rect[4].top = -13;
		rect[4].right = rect[4].left + 3;
		rect[4].bottom = rect[4].top + 3;

		rect[5].left = -12;
		rect[5].top = +8;
		rect[5].right = rect[5].left + 3;
		rect[5].bottom = rect[5].top + 3;

		rect[6].left = +6;
		rect[6].top = +8;
		rect[6].right = rect[6].left + 3;
		rect[6].bottom = rect[6].top + 3;

		for (BYTE i=0; i<MAX_COUNT_SICBO; ++i)
		{
			m_bySicboDraw[i]=i;
			m_SicboAnimInfo[i].byAnimX = rand()%6 + 3;
			m_SicboAnimInfo[i].byAnimY = rand()%6;
			m_SicboAnimInfo[i].byResultX = (m_enCards[i]-1)+(6*(rand()%10));
			m_SicboAnimInfo[i].byResultY = 0;
			BYTE byTemp = rand()%(7-i);
			m_SicboAnimInfo[i].ptEnd.x = rect[byTemp].left + (rand()%(rect[byTemp].Width()+1));
			m_SicboAnimInfo[i].ptEnd.y = rect[byTemp].top  + (rand()%(rect[byTemp].Height()+1));
			if ((7-i-1) != byTemp)
			{
				CRect rectTemp = rect[byTemp];
				rect[byTemp] = rect[7-i-1];
				rect[7-i-1] = rectTemp; 
			}
		}

		// �����ӵ�˳��
		for (BYTE i=1; i<MAX_COUNT_SICBO; ++i)
		{
			for (BYTE j=0; j<i; ++j)
			{
				if (m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.y>m_SicboAnimInfo[m_bySicboDraw[i]].ptEnd.y)
				{
					BYTE byTemp = m_bySicboDraw[j];
					m_bySicboDraw[j] = m_bySicboDraw[i];
					m_bySicboDraw[i] = byTemp;
				}
				else if ((m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.y==m_SicboAnimInfo[m_bySicboDraw[i]].ptEnd.y)
					&& (m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.x>m_SicboAnimInfo[m_bySicboDraw[i]].ptEnd.x))
				{
					BYTE byTemp = m_bySicboDraw[j];
					m_bySicboDraw[j] = m_bySicboDraw[i];
					m_bySicboDraw[i] = byTemp;
				}
			}
		}
		//���ö�ʱ��
		SetTimer(IDI_DISPATCH_CARD,50,NULL);
	}
	else if (enSicboType_InTheAir == m_enSicboType)
	{
		//���ñ���
		m_bSicboEffect=true;
		m_bShowSicboEffectBack = true;
		m_bySicboFrameIndex=0;
		// �����߸�����ÿ�����߸����������
		CRect rect[7];
		rect[0].left = -20;
		rect[0].top = -25;
		rect[0].right = rect[0].left + 40;
		rect[0].bottom = rect[0].top + 40;

		rect[1].left = -135;
		rect[1].top = -30;
		rect[1].right = rect[1].left + 50;
		rect[1].bottom = rect[1].top +50;

		rect[2].left = +100;
		rect[2].top = -35;
		rect[2].right = rect[2].left + 50;
		rect[2].bottom = rect[2].top + 50;

		rect[3].left = -125;
		rect[3].top = -100;
		rect[3].right = rect[3].left + 60;
		rect[3].bottom = rect[3].top + 60;

		rect[4].left = +80;
		rect[4].top = -105;
		rect[4].right = rect[4].left + 60;
		rect[4].bottom = rect[4].top + 60;

		rect[5].left = -125;
		rect[5].top = +70;
		rect[5].right = rect[5].left + 50;
		rect[5].bottom = rect[5].top + 50;

		rect[6].left = +85;
		rect[6].top = +70;
		rect[6].right = rect[6].left + 60;
		rect[6].bottom = rect[6].top + 60;

		for (BYTE i=0; i<MAX_COUNT_SICBO; ++i)
		{
			m_bySicboDraw[i]=i;
			m_SicboAnimInfo[i].byAnimX = rand()%6 + 3;
			m_SicboAnimInfo[i].byAnimY = rand()%6;
			m_SicboAnimInfo[i].byResultX = m_enCards[i]-1;
			m_SicboAnimInfo[i].byResultY = rand()%4;
			BYTE byTemp = rand()%(7-i);
			m_SicboAnimInfo[i].ptEnd.x = rect[byTemp].left + (rand()%(rect[byTemp].Width()+1));
			m_SicboAnimInfo[i].ptEnd.y = rect[byTemp].top  + (rand()%(rect[byTemp].Height()+1));
			if ((7-i-1) != byTemp)
			{
				CRect rectTemp = rect[byTemp];
				rect[byTemp] = rect[7-i-1];
				rect[7-i-1] = rectTemp; 
			}
		}

		// �����ӵ�˳��
		for (BYTE i=1; i<MAX_COUNT_SICBO; ++i)
		{
			for (BYTE j=0; j<i; ++j)
			{
				if (m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.y>m_SicboAnimInfo[m_bySicboDraw[i]].ptEnd.y)
				{
					BYTE byTemp = m_bySicboDraw[j];
					m_bySicboDraw[j] = m_bySicboDraw[i];
					m_bySicboDraw[i] = byTemp;
				}
				else if ((m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.y==m_SicboAnimInfo[m_bySicboDraw[i]].ptEnd.y)
					&& (m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.x>m_SicboAnimInfo[m_bySicboDraw[i]].ptEnd.x))
				{
					BYTE byTemp = m_bySicboDraw[j];
					m_bySicboDraw[j] = m_bySicboDraw[i];
					m_bySicboDraw[i] = byTemp;
				}
			}
		}
		//���ö�ʱ��
		SetTimer(IDI_DISPATCH_CARD,20,NULL);
	}
	else if (enSicboType_Sicbo == m_enSicboType)
	{
		DispatchCardSicbo();
	}
	else
	{
		DispatchCardNormal();
	}
}
void CGameClientView::DispatchCardSicbo()
{
	//���ñ���
	m_bSicboEffect=true;
	m_bShowSicboEffectBack = true;
	m_bySicboFrameIndex=0;
	// �����߸�����ÿ�����߸����������
	CRect rect[7];
	rect[0].left = -5;
	rect[0].top = -15;
	rect[0].right = rect[0].left + 10;
	rect[0].bottom = rect[0].top + 10;

	rect[1].left = -65;
	rect[1].top = -15;
	rect[1].right = rect[1].left + 10;
	rect[1].bottom = rect[1].top +10;

	rect[2].left = +45;
	rect[2].top = -15;
	rect[2].right = rect[2].left + 10;
	rect[2].bottom = rect[2].top + 10;

	rect[3].left = -45;
	rect[3].top = -52;
	rect[3].right = rect[3].left + 10;
	rect[3].bottom = rect[3].top + 10;

	rect[4].left = +22;
	rect[4].top = -55;
	rect[4].right = rect[4].left + 10;
	rect[4].bottom = rect[4].top + 10;

	rect[5].left = -45;
	rect[5].top = +14;
	rect[5].right = rect[5].left + 10;
	rect[5].bottom = rect[5].top + 10;

	rect[6].left = +22;
	rect[6].top = +14;
	rect[6].right = rect[6].left + 10;
	rect[6].bottom = rect[6].top + 10;

	for (BYTE i=0; i<MAX_COUNT_SICBO; ++i)
	{
		m_bySicboDraw[i]=i;
		m_SicboAnimInfo[i].byAnimX = rand()%6 + 3;
		m_SicboAnimInfo[i].byAnimY = rand()%6;
		m_SicboAnimInfo[i].byResultX = m_enCards[i]-1;
		m_SicboAnimInfo[i].byResultY = rand()%4;
		BYTE byTemp = rand()%(7-i);
		m_SicboAnimInfo[i].ptEnd.x = rect[byTemp].left + (rand()%(rect[byTemp].Width()+1));
		m_SicboAnimInfo[i].ptEnd.y = rect[byTemp].top  + (rand()%(rect[byTemp].Height()+1));
		if ((7-i-1) != byTemp)
		{
			CRect rectTemp = rect[byTemp];
			rect[byTemp] = rect[7-i-1];
			rect[7-i-1] = rectTemp; 
		}
	}

	// �����ӵ�˳��
	for (BYTE i=1; i<MAX_COUNT_SICBO; ++i)
	{
		for (BYTE j=0; j<i; ++j)
		{
			if (m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.y>m_SicboAnimInfo[m_bySicboDraw[i]].ptEnd.y)
			{
				BYTE byTemp = m_bySicboDraw[j];
				m_bySicboDraw[j] = m_bySicboDraw[i];
				m_bySicboDraw[i] = byTemp;
			}
			else if ((m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.y==m_SicboAnimInfo[m_bySicboDraw[i]].ptEnd.y)
				&& (m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.x>m_SicboAnimInfo[m_bySicboDraw[i]].ptEnd.x))
			{
				BYTE byTemp = m_bySicboDraw[j];
				m_bySicboDraw[j] = m_bySicboDraw[i];
				m_bySicboDraw[i] = byTemp;
			}
		}
	}
	SetTimer(IDI_DISPATCH_CARD,80,NULL);
}

void CGameClientView::DispatchCardNormal()
{
	//���ñ���
	m_bSicboEffect=true;
	m_bShowSicboEffectBack = true;
	m_bySicboFrameIndex=0;
	// �����߸�����ÿ�����߸����������
	CRect rect[7];
	rect[0].left = -5;
	rect[0].top = -15;
	rect[0].right = rect[0].left + 10;
	rect[0].bottom = rect[0].top + 10;

	rect[1].left = -65;
	rect[1].top = -15;
	rect[1].right = rect[1].left + 10;
	rect[1].bottom = rect[1].top +10;

	rect[2].left = +45;
	rect[2].top = -15;
	rect[2].right = rect[2].left + 10;
	rect[2].bottom = rect[2].top + 10;

	rect[3].left = -45;
	rect[3].top = -52;
	rect[3].right = rect[3].left + 10;
	rect[3].bottom = rect[3].top + 10;

	rect[4].left = +22;
	rect[4].top = -55;
	rect[4].right = rect[4].left + 10;
	rect[4].bottom = rect[4].top + 10;

	rect[5].left = -45;
	rect[5].top = +14;
	rect[5].right = rect[5].left + 10;
	rect[5].bottom = rect[5].top + 10;

	rect[6].left = +22;
	rect[6].top = +14;
	rect[6].right = rect[6].left + 10;
	rect[6].bottom = rect[6].top + 10;

	for (BYTE i=0; i<MAX_COUNT_SICBO; ++i)
	{
		m_bySicboDraw[i]=i;
		m_SicboAnimInfo[i].byAnimX = rand()%6 + 3;
		m_SicboAnimInfo[i].byAnimY = rand()%6;
		m_SicboAnimInfo[i].byResultX = m_enCards[i]-1;
		m_SicboAnimInfo[i].byResultY = rand()%4;
		BYTE byTemp = rand()%(7-i);
		m_SicboAnimInfo[i].ptEnd.x = rect[byTemp].left + (rand()%(rect[byTemp].Width()+1));
		m_SicboAnimInfo[i].ptEnd.y = rect[byTemp].top  + (rand()%(rect[byTemp].Height()+1));
		if ((7-i-1) != byTemp)
		{
			CRect rectTemp = rect[byTemp];
			rect[byTemp] = rect[7-i-1];
			rect[7-i-1] = rectTemp; 
		}
	}

	// �����ӵ�˳��
	for (BYTE i=1; i<MAX_COUNT_SICBO; ++i)
	{
		for (BYTE j=0; j<i; ++j)
		{
			if (m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.y>m_SicboAnimInfo[m_bySicboDraw[i]].ptEnd.y)
			{
				BYTE byTemp = m_bySicboDraw[j];
				m_bySicboDraw[j] = m_bySicboDraw[i];
				m_bySicboDraw[i] = byTemp;
			}
			else if ((m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.y==m_SicboAnimInfo[m_bySicboDraw[i]].ptEnd.y)
				&& (m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.x>m_SicboAnimInfo[m_bySicboDraw[i]].ptEnd.x))
			{
				BYTE byTemp = m_bySicboDraw[j];
				m_bySicboDraw[j] = m_bySicboDraw[i];
				m_bySicboDraw[i] = byTemp;
			}
		}
	}
	SetTimer(IDI_DISPATCH_CARD,20,NULL);
}

//��������
void CGameClientView::FinishDispatchCard()
{
	//ɾ����ʱ��
	KillTimer(IDI_CHANGE_AZIMUTH);
	KillTimer(IDI_DISPATCH_CARD);	

	m_bSicboEffect = false;
	m_bySicboFrameIndex = 0;
	m_bShowSicboEffectBack = false;
	m_bSicboShow = true;

	//�����¼
	tagClientGameRecord gameRecord;
	ZeroMemory(&gameRecord, sizeof(gameRecord));
	int iCardDot = 0;
	for (BYTE i=0; i<MAX_COUNT_SICBO; ++i)
	{
		gameRecord.iCards[i] = m_enCards[i];
		iCardDot += gameRecord.iCards[i];
	}
	if (iCardDot>enCardType_NumberTen)
	{
		gameRecord.bFlags = true;
	}
	else
	{
		gameRecord.bFlags = false;
	}
		
	SetGameHistory(gameRecord);

	// �ۼƻ���
	m_lMeStatisticScore+=m_lMeCurGameScore;
	m_lBankerWinScore=m_lTmpBankerWinScore;

	//����Ӯ��
	SetWinnerSide(0);

	//��������
	if (m_lMeCurGameScore>0) m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_WIN"));
	else if (m_lMeCurGameScore<0) m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_LOST"));
	else m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_DRAW"));
}

//ʤ���߿�
void CGameClientView::FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC)
{
	if (m_cbAreaFlash==0xFF) return;
	
	// �����ж�,���ҷ�������
	if ((m_pGameClientDlg->GetGameStatus()==GS_GAME_END) && m_bSicboShow)
	{		
		// �鿴˭��Ӯ��		
		INT iWidth = m_ImageFrameWin.GetWidth()/8;
		INT iHeight = m_ImageFrameWin.GetHeight()/2;
		INT iSrcHeight = 0;
		if (0 != m_cbAreaFlash)
		{
			iSrcHeight = iHeight;
		}

		if ((iWidth<3) || (iHeight<3))
		{
			return ;
		}
		for (int i=0; i<COUNT_AZIMUTH; ++i)
		{
			if (m_bWinner[i])
			{
				CRect rcTemp(m_rcUser[i]);
				rcTemp.left = rcTemp.left - 5;
				rcTemp.top = rcTemp.top - 5;
				rcTemp.right = rcTemp.right + 5;
				rcTemp.bottom = rcTemp.bottom + 5;
				
				// ����Ӯ�ı߿�
				// ����,�ײ�
				for (INT j=0; j<(rcTemp.Width()-2*iWidth); j+=iWidth)
				{
					INT iXPos = rcTemp.left+iWidth + j;
					INT iXSrc = iWidth * 4;
					INT iWidthTemp = (rcTemp.Width()-2*iWidth)-j;
					if (iWidthTemp>iWidth)
					{
						iWidthTemp = iWidth;
					}
					m_ImageFrameWin.DrawImage(pDC,iXPos,rcTemp.top,iWidthTemp, iHeight, iXSrc,iSrcHeight,iWidthTemp, iHeight);
					INT iYPos = rcTemp.bottom - iHeight;
					iXSrc = iWidth * 6;
					m_ImageFrameWin.DrawImage(pDC,iXPos,iYPos,iWidthTemp, iHeight, iXSrc,iSrcHeight,iWidthTemp, iHeight);
				}

				// ����
				for (INT j=0; j<(rcTemp.Height()-2*iHeight); j+=iHeight)
				{
					INT iXPos = rcTemp.left;
					INT iYPos = rcTemp.top + iHeight +j;
					INT iXSrc = iWidth * 5;

					INT iHeightTemp = (rcTemp.Height()-2*iHeight)-j;
					if (iHeightTemp>iHeight)
					{
						iHeightTemp = iHeight;
					}

					m_ImageFrameWin.DrawImage(pDC,iXPos,iYPos,iWidth, iHeightTemp, iXSrc,iSrcHeight,iWidth, iHeightTemp);
					
					iXPos = rcTemp.right-iWidth;
					iXSrc = iWidth * 7;
					m_ImageFrameWin.DrawImage(pDC,iXPos,iYPos,iWidth, iHeightTemp, iXSrc,iSrcHeight,iWidth, iHeightTemp);
				}

				// ���ĸ���
				INT iXPos =  rcTemp.left;
				INT iYPos =  rcTemp.top;
				INT iXSrc = 0;
				m_ImageFrameWin.DrawImage(pDC,iXPos,iYPos,iWidth, iHeight, iXSrc,iSrcHeight,iWidth, iHeight);

				iXPos =rcTemp.right-iWidth;
				iYPos = rcTemp.top;
				iXSrc = iWidth;
				m_ImageFrameWin.DrawImage(pDC,iXPos,iYPos,iWidth, iHeight, iXSrc,iSrcHeight,iWidth, iHeight);

				iXPos =rcTemp.left;
				iYPos = rcTemp.bottom - iHeight;
				iXSrc = iWidth*2;
				m_ImageFrameWin.DrawImage(pDC,iXPos,iYPos,iWidth, iHeight, iXSrc,iSrcHeight,iWidth, iHeight);

				iXPos =rcTemp.right-iWidth;
				iYPos = rcTemp.bottom - iHeight;
				iXSrc = iWidth*3;
				m_ImageFrameWin.DrawImage(pDC,iXPos,iYPos,iWidth, iHeight, iXSrc,iSrcHeight,iWidth, iHeight);
			}
		}
	}
}

//���ñ�ը
bool CGameClientView::SetBombEffect(bool bBombEffect, int iAreaIndex)
{
	if (bBombEffect==true)
	{
		//���ñ���
		m_bBombEffect[iAreaIndex]=true;
		m_cbBombFrameIndex[iAreaIndex]=0;

		//����ʱ��
		SetTimer(IDI_BOMB_EFFECT+iAreaIndex,100,NULL);
	}
	else
	{
		//ֹͣ����
		if (m_bBombEffect[iAreaIndex]==true)
		{
			//ɾ��ʱ��
			KillTimer(IDI_BOMB_EFFECT+iAreaIndex);

			//���ñ���
			m_bBombEffect[iAreaIndex]=false;
			m_cbBombFrameIndex[iAreaIndex]=0;

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
	for (int i=0; i<COUNT_AZIMUTH; ++i)
	{
		if (m_bBombEffect[i])
		{
			m_ImageBombEffect.DrawImage(pDC,m_PointJettonNumber[i].x-nImageWidth/2,m_PointJettonNumber[i].y-10,nImageWidth,nImageHeight,
				nImageWidth*(m_cbBombFrameIndex[i]%BOMB_EFFECT_COUNT),0);
		}
	}
}

void CGameClientView::DrawSicboAnim(CDC *pDC)
{
	if (enSicboType_Desktop == m_enSicboType)
	{
		DrawSicboAnimDesktop(pDC);
	}
	else if (enSicboType_InMidair == m_enSicboType)
	{
		DrawSicboAnimInMidair(pDC);
	}
	else if (enSicboType_InTheAir == m_enSicboType)
	{
		DrawSicboAnimInTheAir(pDC);
	}
	else if (enSicboType_Sicbo == m_enSicboType)
	{
		DrawSicboAnimSicbo(pDC);
	}
	else
	{
		DrawSicboAnimNormal(pDC);
	}
}

void CGameClientView::DrawSicboAnimNormal(CDC *pDC)
{
	// ���ӵĵ���
	INT nImageHeight=0;
	INT nImageWidth=0;
	// ���ϱ���
	if (m_bShowSicboEffectBack)
	{
		nImageHeight=m_ImgSicboEffectBack.GetHeight();
		nImageWidth=m_ImgSicboEffectBack.GetWidth();
		m_ImgSicboEffectBack.DrawImage(pDC, m_ptSicboAnim[enSicboType_Normal].x-nImageWidth/2,m_ptSicboAnim[enSicboType_Normal].y-nImageHeight/2,
			nImageWidth, nImageHeight, 0,0);

		// �����Ѿ�ҡ���
		if (m_bSicboEffect)
		{
			nImageHeight=m_ImgSicboEffectResult.GetHeight()/4;
			nImageWidth=m_ImgSicboEffectResult.GetWidth()/6;
			BYTE bySicbo = m_bySicboFrameIndex/SICBO_FRAME_COUNT_NORMAL;
			if (bySicbo >= MAX_COUNT_SICBO)
			{
				// y������С���Ȼ���x����С��Ҳ�Ȼ�				
				for (BYTE j=0; j<MAX_COUNT_SICBO; ++j)
				{
					m_ImgSicboEffectResult.DrawImage(pDC, 
						m_ptSicboAnim[enSicboType_Normal].x+m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.x-nImageWidth/2,
						m_ptSicboAnim[enSicboType_Normal].y+m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.y-nImageHeight/2,
						nImageWidth, nImageHeight,
						m_SicboAnimInfo[m_bySicboDraw[j]].byResultX*nImageWidth,
						m_SicboAnimInfo[m_bySicboDraw[j]].byResultY*nImageHeight);
				}
				return ;
			}

			for (BYTE j=0; j<MAX_COUNT_SICBO; ++j)
			{
				if (m_bySicboDraw[j] < bySicbo)
				{
					m_ImgSicboEffectResult.DrawImage(pDC,
						m_ptSicboAnim[enSicboType_Normal].x+m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.x-nImageWidth/2,
						m_ptSicboAnim[enSicboType_Normal].y+m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.y-nImageHeight/2,
						nImageWidth, nImageHeight,
						m_SicboAnimInfo[m_bySicboDraw[j]].byResultX*nImageWidth,
						m_SicboAnimInfo[m_bySicboDraw[j]].byResultY*nImageHeight);
				}
			}

			CPoint ptAnimStart = m_ptSicboAnim[enSicboType_Normal]-CPoint(0,210);
			CPoint ptAnim = ptAnimStart;
			BYTE byIndex = m_bySicboFrameIndex%SICBO_FRAME_COUNT_NORMAL;
			if (byIndex < (SICBO_FRAME_COUNT_NORMAL-1))  // ǰ��4֡���ƶ�������
			{
				nImageHeight=m_ImgSicboEffectNormal.GetHeight()/6;
				nImageWidth=m_ImgSicboEffectNormal.GetWidth()/9;

				ptAnim.x += (((m_ptSicboAnim[enSicboType_Normal].x+m_SicboAnimInfo[bySicbo].ptEnd.x -ptAnimStart.x)/(SICBO_FRAME_COUNT_NORMAL))*byIndex);
				ptAnim.y += (((m_ptSicboAnim[enSicboType_Normal].y+m_SicboAnimInfo[bySicbo].ptEnd.y -ptAnimStart.y)/SICBO_FRAME_COUNT_NORMAL)*byIndex);

				// ���ƶ�̬����
				m_ImgSicboEffectNormal.DrawImage(pDC, ptAnim.x-nImageWidth/2,
					ptAnim.y -nImageHeight/2, nImageWidth, nImageHeight,
					byIndex*nImageWidth, m_SicboAnimInfo[bySicbo].byAnimY*nImageHeight);
			}
			else
			{
				// ���ƾ�̬����
				m_ImgSicboEffectResult.DrawImage(pDC,
					m_ptSicboAnim[enSicboType_Normal].x+m_SicboAnimInfo[bySicbo].ptEnd.x-nImageWidth/2,
					m_ptSicboAnim[enSicboType_Normal].y+m_SicboAnimInfo[bySicbo].ptEnd.y-nImageHeight/2,
					nImageWidth, nImageHeight,
					m_SicboAnimInfo[bySicbo].byResultX*nImageWidth,
					m_SicboAnimInfo[bySicbo].byResultY*nImageHeight);
			}
		}		
	}
}

void CGameClientView::DrawSicboAnimDesktop(CDC *pDC)
{
	INT nImageHeight=m_ImgSicboEffectDesktop.GetHeight();
	INT nImageWidth=m_ImgSicboEffectDesktop.GetWidth()/SICBO_EFFECT_FRAME_COUNT;

	// ���ӵĵ���
	if (m_bSicboEffect)
	{
		if (m_bShowSicboEffectBack && m_bySicboFrameIndex<8)
		{
			INT nImgHeight=m_ImgSicboPlan.GetHeight();
			INT nImgWidth=m_ImgSicboPlan.GetWidth();
			m_ImgSicboPlan.DrawImage(pDC, m_ptSicboAnim[enSicboType_Desktop].x-nImgWidth/2-5,m_ptSicboAnim[enSicboType_Desktop].y-nImgHeight/2+50,
				nImgWidth, nImgHeight, 0,0);
		}
		else if (m_bShowSicboEffectBack && m_bySicboFrameIndex>22)
		{
			INT nImgHeight=m_ImgSicboEPlan.GetHeight();
			INT nImgWidth=m_ImgSicboEPlan.GetWidth();
			m_ImgSicboEPlan.DrawImage(pDC, m_ptSicboAnim[enSicboType_Desktop].x-nImgWidth/2-8,m_ptSicboAnim[enSicboType_Desktop].y-nImgHeight/2+46,
				nImgWidth, nImgHeight, 0,0);

			// ������
			nImgHeight=m_ImgSicboNum.GetHeight();
			nImgWidth=m_ImgSicboNum.GetWidth()/60;

			// y������С���Ȼ���x����С��Ҳ�Ȼ�				
			for (BYTE j=0; j<MAX_COUNT_SICBO; ++j)
			{
				m_ImgSicboNum.DrawImage(pDC, 
					m_ptSicboAnim[enSicboType_Desktop].x-8+m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.x-nImgWidth/2,
					m_ptSicboAnim[enSicboType_Desktop].y+46+m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.y-nImgHeight/2,
					nImgWidth, nImgHeight,
					m_SicboAnimInfo[m_bySicboDraw[j]].byResultX*nImgWidth,
					m_SicboAnimInfo[m_bySicboDraw[j]].byResultY*nImgHeight);
			}
		}

		m_ImgSicboEffectDesktop.DrawImage(pDC,m_ptSicboAnim[enSicboType_Desktop].x-nImageWidth/2,
			m_ptSicboAnim[enSicboType_Desktop].y-nImageHeight/2,nImageWidth,nImageHeight,
			nImageWidth*(m_bySicboFrameIndex%SICBO_EFFECT_FRAME_COUNT),0);				
	}
}

void CGameClientView::DrawSicboAnimInMidair(CDC *pDC)
{
	INT nImageHeight=m_ImgSicboEffectInMidair.GetHeight();
	INT nImageWidth=m_ImgSicboEffectInMidair.GetWidth()/SICBO_EFFECT_FRAME_COUNT;

	// ���ӵĵ���
	if (m_bSicboEffect)
	{
		if (m_bShowSicboEffectBack && m_bySicboFrameIndex<8)
		{
			INT nImgHeight=m_ImgSicboPlan.GetHeight();
			INT nImgWidth=m_ImgSicboPlan.GetWidth();
			m_ImgSicboPlan.DrawImage(pDC, m_ptSicboAnim[enSicboType_InMidair].x-nImgWidth/2-5,m_ptSicboAnim[enSicboType_InMidair].y-nImgHeight/2+50,
				nImgWidth, nImgHeight, 0,0);
		}
		else if (m_bShowSicboEffectBack && m_bySicboFrameIndex>24)
		{
			INT nImgHeight=m_ImgSicboEPlan.GetHeight();
			INT nImgWidth=m_ImgSicboEPlan.GetWidth();
			m_ImgSicboEPlan.DrawImage(pDC, m_ptSicboAnim[enSicboType_InMidair].x-nImgWidth/2-8,m_ptSicboAnim[enSicboType_InMidair].y-nImgHeight/2+42,
				nImgWidth, nImgHeight, 0,0);

			// ������
			nImgHeight=m_ImgSicboNum.GetHeight();
			nImgWidth=m_ImgSicboNum.GetWidth()/60;

			// y������С���Ȼ���x����С��Ҳ�Ȼ�				
			for (BYTE j=0; j<MAX_COUNT_SICBO; ++j)
			{
				m_ImgSicboNum.DrawImage(pDC, 
					m_ptSicboAnim[enSicboType_InMidair].x-8+m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.x-nImgWidth/2,
					m_ptSicboAnim[enSicboType_InMidair].y+42+m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.y-nImgHeight/2,
					nImgWidth, nImgHeight,
					m_SicboAnimInfo[m_bySicboDraw[j]].byResultX*nImgWidth,
					m_SicboAnimInfo[m_bySicboDraw[j]].byResultY*nImgHeight);
			}
		}

		m_ImgSicboEffectInMidair.DrawImage(pDC,m_ptSicboAnim[enSicboType_InMidair].x-nImageWidth/2,
			m_ptSicboAnim[enSicboType_InMidair].y-nImageHeight/2,nImageWidth,nImageHeight,
			nImageWidth*(m_bySicboFrameIndex%SICBO_EFFECT_FRAME_COUNT),0);				
	}
}

void CGameClientView::DrawSicboAnimInTheAir(CDC *pDC)
{
	// ���ӵĵ���
	INT nImageHeight=0;
	INT nImageWidth=0;
	// ���ϱ���
	if (m_bShowSicboEffectBack)
	{
		// �����Ѿ�ҡ���
		// ���ӵĵ��� m_ptSicboEffect

		if (m_bSicboEffect)
		{
			nImageHeight=m_ImgSicboEffectResult.GetHeight()/4;
			nImageWidth=m_ImgSicboEffectResult.GetWidth()/6;
			BYTE bySicbo = m_bySicboFrameIndex/SICBO_FRAME_COUNT_NORMAL;
			if (bySicbo >= MAX_COUNT_SICBO)
			{
				// y������С���Ȼ���x����С��Ҳ�Ȼ�				
				for (BYTE j=0; j<MAX_COUNT_SICBO; ++j)
				{
					m_ImgSicboEffectResult.DrawImage(pDC, 
						m_ptSicboAnim[enSicboType_InTheAir].x+m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.x-nImageWidth/2,
						m_ptSicboAnim[enSicboType_InTheAir].y+m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.y-nImageHeight/2,
						nImageWidth, nImageHeight,
						m_SicboAnimInfo[m_bySicboDraw[j]].byResultX*nImageWidth,
						m_SicboAnimInfo[m_bySicboDraw[j]].byResultY*nImageHeight);
				}
				return ;
			}

			if ((m_bySicboFrameIndex%SICBO_FRAME_COUNT_NORMAL)<6)
			{
				INT nImgHeight=m_ImgSicboGiftInTheAir.GetHeight();
				INT nImgWidth=m_ImgSicboGiftInTheAir.GetWidth()/3;
				m_ImgSicboGiftInTheAir.DrawImage(pDC, m_ptSicboAnim[enSicboType_InTheAir].x-nImgWidth/2,m_ptSicboAnim[enSicboType_InTheAir].y-nImgHeight/2,
					nImgWidth, nImgHeight, nImgWidth*(m_bySicboFrameIndex%SICBO_FRAME_COUNT_NORMAL),0);
			}

			for (BYTE j=0; j<MAX_COUNT_SICBO; ++j)
			{
				if (m_bySicboDraw[j] < bySicbo)
				{
					m_ImgSicboEffectResult.DrawImage(pDC,
						m_ptSicboAnim[enSicboType_InTheAir].x+m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.x-nImageWidth/2,
						m_ptSicboAnim[enSicboType_InTheAir].y+m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.y-nImageHeight/2,
						nImageWidth, nImageHeight,
						m_SicboAnimInfo[m_bySicboDraw[j]].byResultX*nImageWidth,
						m_SicboAnimInfo[m_bySicboDraw[j]].byResultY*nImageHeight);
				}
			}

			CPoint ptAnimStart = m_ptSicboAnim[enSicboType_InTheAir]-CPoint(0,210);
			CPoint ptAnim = ptAnimStart;
			BYTE byIndex = m_bySicboFrameIndex%SICBO_FRAME_COUNT_NORMAL;
			if (byIndex < (SICBO_FRAME_COUNT_NORMAL-1))  // ǰ��4֡���ƶ�������
			{
				if (byIndex <3)
				{
					return ;
				}
				nImageHeight=m_ImgSicboEffectNormal.GetHeight()/6;
				nImageWidth=m_ImgSicboEffectNormal.GetWidth()/9;

				ptAnim.x += (((m_ptSicboAnim[enSicboType_InTheAir].x+m_SicboAnimInfo[bySicbo].ptEnd.x -ptAnimStart.x)/(SICBO_FRAME_COUNT_NORMAL))*byIndex);
				ptAnim.y += (((m_ptSicboAnim[enSicboType_InTheAir].y+m_SicboAnimInfo[bySicbo].ptEnd.y -ptAnimStart.y)/SICBO_FRAME_COUNT_NORMAL)*byIndex);

				// ���ƶ�̬����
				m_ImgSicboEffectNormal.DrawImage(pDC, ptAnim.x-nImageWidth/2,
					ptAnim.y -nImageHeight/2, nImageWidth, nImageHeight,
					(byIndex-3)*nImageWidth, m_SicboAnimInfo[bySicbo].byAnimY*nImageHeight);
			}
			else
			{
				// ���ƾ�̬����
				m_ImgSicboEffectResult.DrawImage(pDC,
					m_ptSicboAnim[enSicboType_InTheAir].x+m_SicboAnimInfo[bySicbo].ptEnd.x-nImageWidth/2,
					m_ptSicboAnim[enSicboType_InTheAir].y+m_SicboAnimInfo[bySicbo].ptEnd.y-nImageHeight/2,
					nImageWidth, nImageHeight,
					m_SicboAnimInfo[bySicbo].byResultX*nImageWidth,
					m_SicboAnimInfo[bySicbo].byResultY*nImageHeight);
			}
		}
	}
}

void CGameClientView::DrawSicboAnimSicbo(CDC *pDC)
{
	// ���ӵĵ���
	if (m_bSicboEffect)
	{
		if (m_bySicboFrameIndex>35)
		{
			//INT nImgHeight=m_ImgSicboEPlan.GetHeight();
			//INT nImgWidth=m_ImgSicboEPlan.GetWidth();
			//m_ImgSicboEPlan.DrawImage(pDC, m_ptSicboAnim[enSicboType_Sicbo].x-nImgWidth/2-8,m_ptSicboAnim[enSicboType_Sicbo].y-nImgHeight/2+42,
			//	nImgWidth, nImgHeight, 0,0);

			INT nImageHeight=m_ImgSicboEffectResult.GetHeight()/4;
			INT nImageWidth=m_ImgSicboEffectResult.GetWidth()/6;

			// y������С���Ȼ���x����С��Ҳ�Ȼ�				
			for (BYTE j=0; j<MAX_COUNT_SICBO; ++j)
			{
				m_ImgSicboEffectResult.DrawImage(pDC, 
					m_ptSicboAnim[enSicboType_Sicbo].x+m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.x-nImageWidth/2,
					m_ptSicboAnim[enSicboType_Sicbo].y+m_SicboAnimInfo[m_bySicboDraw[j]].ptEnd.y-nImageHeight/2 + 100,
					nImageWidth, nImageHeight,
					m_SicboAnimInfo[m_bySicboDraw[j]].byResultX*nImageWidth,
					m_SicboAnimInfo[m_bySicboDraw[j]].byResultY*nImageHeight);
			}
		}
		// ǰ��16֡�ظ�����
		if (m_bySicboFrameIndex>=5 && m_bySicboFrameIndex<(16+5))
		{
			INT iIndex = m_bySicboFrameIndex-5;
			INT nImageHeight=m_ImgSicboAnim[iIndex].GetHeight();
			INT nImageWidth=m_ImgSicboAnim[iIndex].GetWidth();
			m_ImgSicboAnim[iIndex].DrawImage(pDC,m_ptSicboAnim[enSicboType_Sicbo].x-nImageWidth/2,
				m_ptSicboAnim[enSicboType_Sicbo].y-nImageHeight/2,nImageWidth,nImageHeight,0,0);
		}
		else if (m_bySicboFrameIndex>=(16+5) && m_bySicboFrameIndex<10 + (16+5))
		{
			INT iIndex = m_bySicboFrameIndex-(16+5)+7;
			INT nImageHeight=m_ImgSicboAnim[iIndex].GetHeight();
			INT nImageWidth=m_ImgSicboAnim[iIndex].GetWidth();
			m_ImgSicboAnim[iIndex].DrawImage(pDC,m_ptSicboAnim[enSicboType_Sicbo].x-nImageWidth/2,
				m_ptSicboAnim[enSicboType_Sicbo].y-nImageHeight/2,nImageWidth,nImageHeight,0,0);
		}
		else if (m_bySicboFrameIndex>=(10 + (16+5)) && m_bySicboFrameIndex<(15 + (16+5)))
		{
			INT iIndex = 16;
			INT nImageHeight=m_ImgSicboAnim[iIndex].GetHeight();
			INT nImageWidth=m_ImgSicboAnim[iIndex].GetWidth();
			m_ImgSicboAnim[iIndex].DrawImage(pDC,m_ptSicboAnim[enSicboType_Sicbo].x-nImageWidth/2,
				m_ptSicboAnim[enSicboType_Sicbo].y-nImageHeight/2,nImageWidth,nImageHeight,0,0);
		}
		else if (m_bySicboFrameIndex>(10 + (16+5)) && m_bySicboFrameIndex<38)
		{
			INT iIndex = 17;
			INT nImageHeight=m_ImgSicboAnim[iIndex].GetHeight();
			INT nImageWidth=m_ImgSicboAnim[iIndex].GetWidth();
			m_ImgSicboAnim[iIndex].DrawImage(pDC,m_ptSicboAnim[enSicboType_Sicbo].x-nImageWidth/2,
				m_ptSicboAnim[enSicboType_Sicbo].y-nImageHeight/2,nImageWidth,nImageHeight,0,0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void CGameClientView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (0L == m_lCurrentJetton)
	{
		for (int i=0; i<COUNT_AZIMUTH; ++i)
		{
			if (m_rcUser[i].PtInRect(point))
			{			
				// ������Ϣ
				AfxGetMainWnd()->SendMessage(IDM_USE_WINNER,i,USER_WINNER);
				break;
			}
		}
	}

	CGameFrameView::OnLButtonDblClk(nFlags, point);
}