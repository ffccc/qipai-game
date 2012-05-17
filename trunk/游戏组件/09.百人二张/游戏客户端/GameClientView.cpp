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
#define IDC_JETTON_BUTTON_500000	212									//��ť��ʶ
#define IDC_AUTO_OPEN_CARD			213									//��ť��ʶ
#define IDC_OPEN_CARD				214									//��ť��ʶ
#define IDC_BANK					215									//��ť��ʶ
#define IDC_CONTINUE_CARD			216									//��ť��ʶ
#define IDC_BANK_STORAGE			217									//��ť��ʶ
#define IDC_BANK_DRAW				218									//��ť��ʶ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_APPY_BANKER, OnApplyBanker)
	ON_BN_CLICKED(IDC_CANCEL_BANKER, OnCancelBanker)
	ON_BN_CLICKED(IDC_SCORE_MOVE_L, OnScoreMoveL)
	ON_BN_CLICKED(IDC_SCORE_MOVE_R, OnScoreMoveR)
	ON_BN_CLICKED(IDC_OPEN_CARD, OnOpenCard)
	ON_BN_CLICKED(IDC_AUTO_OPEN_CARD,OnAutoOpenCard)
	ON_MESSAGE(WM_VIEWLBTUP,OnViLBtUp)

	ON_BN_CLICKED(IDC_BANK_STORAGE, OnBankStorage)
	ON_BN_CLICKED(IDC_BANK_DRAW, OnBankDraw)


END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView() : CGameFrameView(true,24)
{

	//��ע��Ϣ
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ȫ����ע
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//��������
	ZeroMemory(&m_ApplyBankerCondition,sizeof(m_ApplyBankerCondition));

	//���Ʊ���
	m_bOpenCard=true;

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
	m_bWinShunMen=false;
	m_bWinDuiMen=false;
	m_bWinDaoMen=false;
	m_bFlashResult=false;
	m_blMoveFinish = false;
	m_blAutoOpenCard = true;
	m_enDispatchCardTip=enDispatchCardTip_NULL;

	m_lMeCurGameScore=0L;			
	m_lMeCurGameReturnScore=0L;	
	m_lBankerCurGameScore=0L;		

	m_lAreaLimitScore=0L;	

	//λ����Ϣ
	m_nScoreHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;	

	b_IsMaxJetton=false;
	m_cbTimeLeave=0;

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

	//�߿���Դ
	m_ImageFrameShunMen.SetLoadInfo(IDB_FRAME_SHUN_MEN_EX, hInstance);
	m_ImageFrameJiaoL.SetLoadInfo(IDB_FRAME_JIAO_L_EX, hInstance);
	m_ImageFrameQiao.SetLoadInfo(IDB_FRAME_QIAO_EX, hInstance);
	m_ImageFrameDuiMen.SetLoadInfo(IDB_FRAME_DUI_MEN_EX, hInstance);
	m_ImageFrameDaoMen.SetLoadInfo(IDB_FRAME_DAO_EMN_EX, hInstance);
	m_ImageFrameJiaoR.SetLoadInfo(IDB_FRAME_JIAO_R_EX, hInstance);

	m_ImageGameEnd.SetLoadInfo( IDB_GAME_END, hInstance );

	m_ImageMeBanker.SetLoadInfo( IDB_ME_BANKER, hInstance );
	m_ImageChangeBanker.SetLoadInfo( IDB_CHANGE_BANKER, hInstance );
	m_ImageNoBanker.SetLoadInfo( IDB_NO_BANKER, hInstance );	

	m_ImageTimeFlag.SetLoadInfo(IDB_TIME_FLAG, hInstance);

	m_ImageXianZhu.LoadImage(hInstance,TEXT("XIAZHU"));
	m_ImageXianZhuNum.LoadImage(hInstance,TEXT("XIAZHU_NUM"));
	m_MyBanker.LoadImage(hInstance,TEXT("IDB_MEBANKER"));
	m_ChangeBanker.LoadImage(hInstance,TEXT("IDB_CHANGEBANKER"));
	m_NoBanker.LoadImage(hInstance,TEXT("IDB_NOBANKER"));

	

	/*const BYTE CGameLogic::m_cbCardListData[CARD_COUNT]=
	{
			0x0C,0x2C,
			0x0B,0x2B,
			0x0A,0x1A,0x2A,0x3A,
			0x09,0x29,
			0x07,0x17,0x27,0x37,
			0x08,0x18,0x28,0x38,
			0x06,0x16,0x26,0x36,
			0x05,0x25,
			0x04,0x14,0x24,0x34,
			0x02,0x22,
			0x42,
			0x23

			QQ��22��������3����8�ԣ���4�ԣ���10�ԡ���6�ԡ���4�ԣ�����3��Ϊһ����
			����10�� ��6�ԡ���7�ԡ���J�� ������4��һ���󣩺�8�ԡ���7�ԡ���5�ԡ���Ŷԣ�����4��һ����
			����һ������� ��ô���ܵ�����Ϊׯ�Ҵ� Q+ 9��϶��㣾  Q+8 ��϶���(������8����) ��2��8��϶��㣨������8�� 
			>Q��2����8����4����10=��6=��4=��10=��6=��7=��J=��8=��7=��5=���>����==3
	};*/
	m_cbTableCardArray[SHUN_MEN_INDEX ][0]=0x0A;
	m_cbTableCardArray[SHUN_MEN_INDEX ][1]=0x2A;
	m_cbTableCardArray[ BANKER_INDEX][0]=0x1A;
	m_cbTableCardArray[BANKER_INDEX][1]=0x3A;
	bool bWinShunMen=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[SHUN_MEN_INDEX],2)==1?true:false;

	return;
}

//��������
CGameClientView::~CGameClientView(void)
{
}
void CGameClientView::SetMoveCardTimer()
{
	KillTimer(IDI_POSTCARD);
	m_OpenCardIndex = 1;
	m_bcShowCount = 0;
	m_PostStartIndex =  m_GameLogic.GetCardValue(m_bcfirstShowCard);
	if(m_PostStartIndex>1)
		m_PostStartIndex  = (m_PostStartIndex-1)%4;
	m_PostCardIndex=m_PostStartIndex;
	for (int i = 0;i<4;i++)
	{
		m_CardControl[i].m_blAdd = true;
		m_CardControl[i].m_blAuto = m_blAutoOpenCard;
		m_CardControl[i].m_blGameEnd = true;	
	}
	SetTimer(IDI_POSTCARD,500,NULL);
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
	m_btJetton100000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100000);
	m_btJetton500000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_500000);
	m_btJetton1000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1000000);
	m_btJetton5000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_5000000);
		
	//���밴ť
	m_btApplyBanker.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_APPY_BANKER);
	m_btCancelBanker.Create(NULL,WS_CHILD|WS_DISABLED,rcCreate,this,IDC_CANCEL_BANKER);

	m_btScoreMoveL.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_L);
	m_btScoreMoveR.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_R);

	//m_btAutoOpenCard.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_AUTO_OPEN_CARD);
	//m_btOpenCard.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_OPEN_CARD);

	m_btBank.Create(NULL,WS_CHILD,rcCreate,this,IDC_BANK);
	m_btContinueCard.Create(NULL,WS_CHILD,rcCreate,this,IDC_CONTINUE_CARD);

	m_btBankerStorage.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BANK_STORAGE);
	m_btBankerDraw.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BANK_DRAW);

//	m_btBankerStorage.EnableWindow(TRUE);

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

	//m_btAutoOpenCard.SetButtonImage(IDB_BT_AUTO_OPEN_CARD,hResInstance,false);
	//m_btOpenCard.SetButtonImage(IDB_BT_OPEN_CARD,hResInstance,false);

	m_btBank.SetButtonImage(IDB_BT_BANK,hResInstance,false);
	m_btContinueCard.SetButtonImage(IDB_BT_CONTINUE_CARD,hResInstance,false);

	m_btBankerStorage.SetButtonImage(IDB_BT_STORAGE,hResInstance,false);
	m_btBankerDraw.SetButtonImage(IDB_BT_DRAW,hResInstance,false);

	//�˿˿ؼ�
	for (int i=0; i<CountArray(m_CardControl); ++i) m_CardControl[i].SetDisplayFlag(true);
	return 0;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//��ע��Ϣ
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ȫ����ע
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//��������
	ZeroMemory(&m_ApplyBankerCondition,sizeof(m_ApplyBankerCondition));

	//���Ʊ���
	m_bOpenCard=true;

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
	m_bWinShunMen=false;
	m_bWinDuiMen=false;
	m_bWinDaoMen=false;
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

	b_IsMaxJetton=false;
	m_cbTimeLeave=0;

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
	m_nWinFlagsExcursionX = nWidth/2-260;	
	m_nWinFlagsExcursionY = nHeight/2 + 246;

	//����λ��
	int nCenterX=nWidth/2, nCenterY=nHeight/2, nBorderWidth=4;

	m_ptMaxJetton.x=nCenterX-200;
	m_ptMaxJetton.y=nCenterY-200;

	m_ptBanker.x=nCenterX-130;
	m_ptBanker.y=nCenterY-160;

	m_ptDispatchCardTip.x=nCenterX-130;
	m_ptDispatchCardTip.y=nCenterY;

	m_rcShunMen.left=nCenterX - 258-94;
	m_rcShunMen.top=nCenterY-227-8;
	m_rcShunMen.right=m_rcShunMen.left+216;
	m_rcShunMen.bottom=m_rcShunMen.top+262;

	m_rcJiaoL.left=m_rcShunMen.left;
	m_rcJiaoL.top=m_rcShunMen.bottom-nBorderWidth;
	m_rcJiaoL.right=m_rcShunMen.right;
	m_rcJiaoL.bottom=m_rcJiaoL.top+104;

	m_rcQiao.left=m_rcShunMen.right-nBorderWidth;
	m_rcQiao.top=m_rcShunMen.top;
	m_rcQiao.right=m_rcQiao.left+272;
	m_rcQiao.bottom=m_rcQiao.top+150;

	m_rcDuiMen.left=m_rcQiao.left;
	m_rcDuiMen.top=m_rcQiao.bottom-nBorderWidth;
	m_rcDuiMen.right=m_rcQiao.right;
	m_rcDuiMen.bottom=m_rcJiaoL.bottom;

	m_rcDaoMen.left=m_rcQiao.right-nBorderWidth;
	m_rcDaoMen.top=m_rcQiao.top;
	m_rcDaoMen.right=m_rcDaoMen.left+216;
	m_rcDaoMen.bottom=m_rcDaoMen.top+262;

	m_rcJiaoR.left=m_rcDaoMen.left;
	m_rcJiaoR.top=m_rcDaoMen.bottom-nBorderWidth;
	m_rcJiaoR.right=m_rcDaoMen.right;
	m_rcJiaoR.bottom=m_rcDuiMen.bottom;
	
	//��������
	int ExcursionY=10;
	m_PointJettonNumber[ID_SHUN_MEN-1].SetPoint((m_rcShunMen.right+m_rcShunMen.left)/2, (m_rcShunMen.bottom+m_rcShunMen.top)/2-ExcursionY);
	m_PointJettonNumber[ID_JIAO_L-1].SetPoint((m_rcJiaoL.right+m_rcJiaoL.left)/2, (m_rcJiaoL.bottom+m_rcJiaoL.top)/2-ExcursionY);
	m_PointJettonNumber[ID_QIAO-1].SetPoint((m_rcQiao.right+m_rcQiao.left)/2, (m_rcQiao.bottom+m_rcQiao.top)/2-ExcursionY);
	m_PointJettonNumber[ID_DUI_MEN-1].SetPoint((m_rcDuiMen.right+m_rcDuiMen.left)/2, (m_rcDuiMen.bottom+m_rcDuiMen.top)/2-ExcursionY);
	m_PointJettonNumber[ID_DAO_MEN-1].SetPoint((m_rcDaoMen.right+m_rcDaoMen.left)/2, (m_rcDaoMen.bottom+m_rcDaoMen.top)/2-ExcursionY);
	m_PointJettonNumber[ID_JIAO_R-1].SetPoint((m_rcJiaoR.right+m_rcJiaoR.left)/2, (m_rcJiaoR.bottom+m_rcJiaoR.top)/2-ExcursionY);
	
	//����λ��
	m_PointJetton[ID_SHUN_MEN-1].SetPoint(m_rcShunMen.left+35, m_rcShunMen.top+70);
	m_PointJetton[ID_JIAO_L-1].SetPoint(m_rcJiaoL.left+43, m_rcJiaoL.top+30);
	m_PointJetton[ID_QIAO-1].SetPoint(m_rcQiao.left+50, m_rcQiao.top+30);
	m_PointJetton[ID_DUI_MEN-1].SetPoint(m_rcDuiMen.left+50, m_rcDuiMen.top+45);
	m_PointJetton[ID_DAO_MEN-1].SetPoint(m_rcDaoMen.left+35, m_rcDaoMen.top+70);
	m_PointJetton[ID_JIAO_R-1].SetPoint(m_rcJiaoR.left+43, m_rcJiaoR.top+30);

	//�˿˿ؼ�
	m_CardControl[0].SetBenchmarkPos(CPoint(nWidth/2,nHeight/2-355),enXCenter,enYTop);
	m_CardControl[1].SetBenchmarkPos(CPoint(nWidth/2-360,nHeight/2-152),enXLeft,enYTop);
	m_CardControl[2].SetBenchmarkPos(CPoint(nWidth/2,nHeight/2+30),enXCenter,enYTop);
	m_CardControl[3].SetBenchmarkPos(CPoint(nWidth/2+270-30,nHeight/2-152),enXLeft,enYTop);
	m_CardControl[4].SetBenchmarkPos(CPoint(nWidth/2,nHeight/2-152),enXCenter,enYTop);

	//�ƶ��ؼ�
	HDWP hDwp=BeginDeferWindowPos(34);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	m_ApplyUser.m_viewHandle = m_hWnd;

	//�б�ؼ�
	DeferWindowPos(hDwp,m_ApplyUser,NULL,nWidth/2 + 148-10,nHeight/2-291,206,50,uFlags);

	//���밴ť
	CRect rcJetton;
	m_btJetton100.GetWindowRect(&rcJetton);
	int nYPos = nHeight/2+175;
	int nXPos = nWidth/2-64-124;
	int nSpace = 13;

	DeferWindowPos(hDwp,m_btJetton100,NULL,nXPos,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton1000,NULL,nXPos + nSpace + rcJetton.Width(),nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton10000,NULL,nXPos + nSpace * 2 + rcJetton.Width() * 2,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton100000,NULL,nXPos + nSpace * 3 + rcJetton.Width() * 3,nYPos,0,0,uFlags|SWP_NOSIZE);	
	DeferWindowPos(hDwp,m_btJetton500000,NULL,nXPos + nSpace * 4 + rcJetton.Width() * 4,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton1000000,NULL,nXPos + nSpace * 5 + rcJetton.Width() * 5,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton5000000,NULL,nXPos + nSpace * 6 + rcJetton.Width() * 6,nYPos,0,0,uFlags|SWP_NOSIZE);

	//��ׯ��ť
	DeferWindowPos(hDwp,m_btApplyBanker,NULL,nWidth/2+277,nHeight/2-340+5,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btCancelBanker,NULL,nWidth/2+277,nHeight/2-340+5,0,0,uFlags|SWP_NOSIZE);

	DeferWindowPos(hDwp,m_btScoreMoveL,NULL,nWidth/2-291+60,nHeight/2+262,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btScoreMoveR,NULL,nWidth/2+334,nHeight/2+260,0,0,uFlags|SWP_NOSIZE);

	int nXOpenCard=nWidth/2+270;
	int nYOpenCard=nHeight/2+133-10;

	//DeferWindowPos(hDwp,m_btAutoOpenCard,NULL,nXOpenCard,nYOpenCard,0,0,uFlags|SWP_NOSIZE);
	//DeferWindowPos(hDwp,m_btOpenCard,NULL,nWidth/2+270,nHeight/2+143-10,0,0,uFlags|SWP_NOSIZE);

	//������ť
	DeferWindowPos(hDwp,m_btBank,NULL,nWidth/2-345,nHeight/2+143,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btContinueCard,NULL,/*nWidth/2+185*/nWidth/2+270,nHeight/2+143-10,0,0,uFlags|SWP_NOSIZE);

	//������ť
	DeferWindowPos(hDwp,m_btBankerStorage,NULL,nWidth/2-355+60+5+565,nHeight/2+143+19,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btBankerDraw,NULL,nWidth/2-355+60+5+565,nHeight/2+143+33+26,0,0,uFlags|SWP_NOSIZE);


	

	//�����ƶ�
	EndDeferWindowPos(hDwp);

	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	HINSTANCE hInstance=AfxGetInstanceHandle();

	//�滭����
	DrawViewImage(pDC,m_ImageViewFill,enMode_Spread);
	DrawViewImage(pDC,m_ImageViewBack,enMode_Centent);

	//��ȡ״̬
	BYTE cbGameStatus=m_pGameClientDlg->GetGameStatus();

	//״̬��ʾ
	CFont static InfoFont;
	InfoFont.CreateFont(-16,0,0,0,400,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("����"));
	CFont * pOldFont=pDC->SelectObject(&InfoFont);
	pDC->SetTextColor(RGB(255,234,0));
	
	pDC->SelectObject(pOldFont);
	InfoFont.DeleteObject();

	//ʱ����ʾ
	CImageHandle ImageHandleTimeFlag(&m_ImageTimeFlag);
	int nTimeFlagWidth = m_ImageTimeFlag.GetWidth()/3;
	int nFlagIndex=0;
	if (cbGameStatus==GS_FREE) nFlagIndex=0;
	else if (cbGameStatus==GS_PLACE_JETTON) nFlagIndex=1;
	else if (cbGameStatus==GS_GAME_END) nFlagIndex=2;
	m_ImageTimeFlag.BitBlt(pDC->GetSafeHdc(), nWidth/2-348+190, nHeight/2+225-500, nTimeFlagWidth, m_ImageTimeFlag.GetHeight(),
		nFlagIndex * nTimeFlagWidth, 0);

	

	//Dlg�����SetGameTimer(GetMeChairID()��������õ�SwitchViewChairID��������������8ʱʵ����ViewIDת��Ϊ0��
	WORD wUserTimer=GetUserTimer(0);
	if (wUserTimer!=0) DrawUserTimer(pDC,nWidth/2-323+204,nHeight/2+197-500,wUserTimer);

	//ʤ���߿�
	FlashJettonAreaFrame(nWidth,nHeight,pDC);

	//������Դ
	CImageHandle HandleJettonView(&m_ImageJettonView);
	CSize static SizeJettonItem(m_ImageJettonView.GetWidth()/9,m_ImageJettonView.GetHeight());

	//�滭����
	for (INT i=0;i<AREA_COUNT;i++)
	{
		//��������
		LONG lScoreCount=0L;
		LONGLONG lMaxScore=GetUserMaxJetton();

		////ׯ�ҽ��
		//LONGLONG lBankerScore=2147483647;
		//if (m_wBankerUser!=INVALID_CHAIR) lBankerScore=m_lBankerScore;
		//for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lBankerScore-=m_lAllJettonScore[nAreaIndex];

		////������ѹ��ׯ�ҵ�ȫ��������
		//if(lMaxScore==lBankerScore) lMaxScore/=2;
		//if(lMaxScore>100000000L) lMaxScore=100000000L;
		//lMaxScore=lMaxScore/100*100;

		LONG static lScoreJetton[JETTON_COUNT]={100L,1000L,10000L,100000L,500000L,1000000L,5000000L};
		int static nJettonViewIndex=0;

		//�滭����
		for (INT_PTR j=0;j<m_JettonInfoArray[i].GetCount();j++)
		{
			//��ȡ��Ϣ
			tagJettonInfo * pJettonInfo=&m_JettonInfoArray[i][j];

			//�ۼ�����
			ASSERT(pJettonInfo->cbJettonIndex<JETTON_COUNT);
			lScoreCount+=lScoreJetton[pJettonInfo->cbJettonIndex];

			//ͼƬ����
			if (1==pJettonInfo->cbJettonIndex || 2==pJettonInfo->cbJettonIndex) nJettonViewIndex=pJettonInfo->cbJettonIndex+1;
			else if (0==pJettonInfo->cbJettonIndex) nJettonViewIndex=pJettonInfo->cbJettonIndex;
			else nJettonViewIndex=pJettonInfo->cbJettonIndex+2;

			//�滭����
			m_ImageJettonView.AlphaDrawImage(pDC,pJettonInfo->nXPos+m_PointJetton[i].x,
				pJettonInfo->nYPos+m_PointJetton[i].y,SizeJettonItem.cx,SizeJettonItem.cy,
				nJettonViewIndex*SizeJettonItem.cx,0,RGB(255,0,255));
		}

		//�滭����
		if (lScoreCount>0L)	DrawNumberString(pDC,lScoreCount,m_PointJettonNumber[i].x,m_PointJettonNumber[i].y);
	}

	//�滭ׯ��
	DrawBankerInfo(pDC,nWidth,nHeight);

	//�滭�û�
	DrawMeInfo(pDC,nWidth,nHeight);

	DrawWinInfo(pDC,nWidth,nHeight);

	//��ע��ʾ
	if (b_IsMaxJetton)
	{
		m_ImageXianZhu.DrawImage(pDC,m_ptMaxJetton.x,m_ptMaxJetton.y);
		m_ImageXianZhuNum.DrawImage(pDC,m_ptMaxJetton.x+197,m_ptMaxJetton.y-5,m_ImageXianZhuNum.GetWidth()/4,
									  m_ImageXianZhuNum.GetHeight(),m_cbTimeLeave*38,0);
	}

	//�л�ׯ��
	if ( m_bShowChangeBanker )
	{
		int	nXPos = nWidth / 2 - 130;
		int	nYPos = nHeight / 2 - 160;

		//������ׯ
		if ( m_wMeChairID == m_wBankerUser )
		{
			/*CImageHandle ImageHandleBanker(&m_ImageMeBanker);
			m_ImageMeBanker.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);*/
			m_MyBanker.DrawImage(pDC,m_ptBanker.x,m_ptBanker.y);
		}
		else if ( m_wBankerUser != INVALID_CHAIR )
		{
			/*CImageHandle ImageHandleBanker(&m_ImageChangeBanker);
			m_ImageChangeBanker.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);*/
			m_ChangeBanker.DrawImage(pDC,m_ptBanker.x,m_ptBanker.y);
		}
		else
		{
			/*CImageHandle ImageHandleBanker(&m_ImageNoBanker);
			m_ImageNoBanker.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);*/
			m_NoBanker.DrawImage(pDC,m_ptBanker.x,m_ptBanker.y);
		}
	}

    //������ʾ
	if (enDispatchCardTip_NULL!=m_enDispatchCardTip)
	{
		if (m_DispatchCard.IsNull()==false) m_DispatchCard.DestroyImage();
		if (enDispatchCardTip_Dispatch==m_enDispatchCardTip)
			m_DispatchCard.LoadImage(hInstance,TEXT("IDB_DISPATCHCARD"));
		else
			m_DispatchCard.LoadImage(hInstance,TEXT("IDB_CONTINUECARD"));
		/*CImageHandle ImageHandle(&m_ImageDispatchCardTip);
		m_ImageDispatchCardTip.AlphaDrawImage(pDC,(nWidth-m_ImageDispatchCardTip.GetWidth())/2, nHeight/2,RGB(255,255,255));*/
		m_DispatchCard.DrawImage(pDC,m_ptDispatchCardTip.x,m_ptDispatchCardTip.y);
	}

	//�ҵ���ע
	DrawMeJettonNumber(pDC);

	//����״̬
	if (cbGameStatus==GS_GAME_END)
	{
		//�滭�˿�
		for (int i=0; i<CountArray(m_CardControl); ++i) m_CardControl[i].DrawCardControl(pDC);

		
	

	}
	//ʤ����־
	DrawWinFlags(pDC);


	if(m_blMoveFinish)
	{
		
		//��ʾ���
		ShowGameResult(pDC, nWidth, nHeight);	

		
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

	return;
}

//��ʷ��¼
void CGameClientView::SetGameHistory(bool bWinShunMen, bool bWinDaoMen, bool bWinDuiMen)
{
	//��������
	tagClientGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.bWinShunMen=bWinShunMen;
	GameRecord.bWinDuiMen=bWinDuiMen;
	GameRecord.bWinDaoMen=bWinDaoMen;

	//��������
	if (0==m_lUserJettonScore[ID_SHUN_MEN]) GameRecord.enOperateShunMen=enOperateResult_NULL;
	else if (m_lUserJettonScore[ID_SHUN_MEN] > 0 && true==bWinShunMen) GameRecord.enOperateShunMen=enOperateResult_Win;
	else if (m_lUserJettonScore[ID_SHUN_MEN] > 0 && false==bWinShunMen) GameRecord.enOperateShunMen=enOperateResult_Lost;

	if (0==m_lUserJettonScore[ID_DAO_MEN]) GameRecord.enOperateDaoMen=enOperateResult_NULL;
	else if (m_lUserJettonScore[ID_DAO_MEN] > 0 && true==bWinDaoMen) GameRecord.enOperateDaoMen=enOperateResult_Win;
	else if (m_lUserJettonScore[ID_DAO_MEN] >0 && false==bWinDaoMen) GameRecord.enOperateDaoMen=enOperateResult_Lost;

	if (0==m_lUserJettonScore[ID_DUI_MEN]) GameRecord.enOperateDuiMen=enOperateResult_NULL;
	else if (m_lUserJettonScore[ID_DUI_MEN] > 0 && true==bWinDuiMen) GameRecord.enOperateDuiMen=enOperateResult_Win;
	else if (m_lUserJettonScore[ID_DUI_MEN] >0 && false==bWinDuiMen) GameRecord.enOperateDuiMen=enOperateResult_Lost;

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

	//���½���
	UpdateGameView(NULL);

	return;
}

//������ע
void CGameClientView::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=ID_JIAO_R);
	if (cbViewIndex>ID_JIAO_R) return;

	m_lUserJettonScore[cbViewIndex]=lJettonCount;

	//���½���
	UpdateGameView(NULL);
}

//�����˿�
void CGameClientView::SetCardInfo(BYTE cbTableCardArray[4][2])
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
	ASSERT(cbViewIndex<=ID_JIAO_R);
	if (cbViewIndex>ID_JIAO_R) return;

	//��������
	bool bPlaceJetton=false;
	LONG lScoreIndex[JETTON_COUNT]={100L,1000L,10000L,100000L,500000L,1000000L,5000000L};

	//�߿���
	int nFrameWidth=0, nFrameHeight=0;
	int nBorderWidth=6;

	switch (cbViewIndex)
	{
	case ID_SHUN_MEN:
		{ 
			m_lAllJettonScore[ID_SHUN_MEN] += lScoreCount;
			nFrameWidth = m_rcShunMen.right-m_rcShunMen.left-70;
			nFrameHeight = m_rcShunMen.bottom-m_rcShunMen.top-140;
			break;
		}
	case ID_JIAO_L:
		{
			m_lAllJettonScore[ID_JIAO_L] += lScoreCount;
			nFrameWidth = m_rcJiaoL.right-m_rcJiaoL.left-86;
			nFrameHeight = m_rcJiaoL.bottom-m_rcJiaoL.top-40;
			break;
		}
	case ID_QIAO:
		{ 
			m_lAllJettonScore[ID_QIAO] += lScoreCount;
			nFrameWidth = m_rcQiao.right-m_rcQiao.left-100;
			nFrameHeight = m_rcQiao.bottom-m_rcQiao.top-60;
			break;
		}
	case ID_DUI_MEN:
		{ 
			m_lAllJettonScore[ID_DUI_MEN] += lScoreCount;
			nFrameWidth = m_rcDuiMen.right-m_rcDuiMen.left-100;
			nFrameHeight = m_rcDuiMen.bottom-m_rcDuiMen.top-90;
			break;
		}
	case ID_DAO_MEN:
		{ 
			m_lAllJettonScore[ID_DAO_MEN] += lScoreCount;
			nFrameWidth = m_rcDaoMen.right-m_rcDaoMen.left-70;
			nFrameHeight = m_rcDaoMen.bottom-m_rcDaoMen.top-140;
			break;
		}
	case ID_JIAO_R:
		{ 
			m_lAllJettonScore[ID_JIAO_R] += lScoreCount;
			nFrameWidth = m_rcJiaoR.right-m_rcJiaoR.left-86;
			nFrameHeight = m_rcJiaoR.bottom-m_rcJiaoR.top-40;
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

	//�����ж�
	bool bAddJetton=lScoreCount>0?true:false;
	lScoreCount=(int)abs((int)lScoreCount);

	//���ӳ���
	for (BYTE i=0;i<CountArray(lScoreIndex);i++)
	{
		//������Ŀ
		BYTE cbScoreIndex=JETTON_COUNT-i-1;
		LONGLONG lCellCount=lScoreCount/lScoreIndex[cbScoreIndex];

		//�������
		if (lCellCount==0L) continue;

		//�������
		for (LONG j=0;j<lCellCount;j++)
		{
			if (true==bAddJetton)
			{
				//�������
				tagJettonInfo JettonInfo;
				int nJettonSize=68;
				JettonInfo.cbJettonIndex=cbScoreIndex;
				JettonInfo.nXPos=rand()%(nFrameWidth-nJettonSize);
				JettonInfo.nYPos=rand()%(nFrameHeight-nJettonSize);

				//��������
				bPlaceJetton=true;
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
		lScoreCount-=lCellCount*lScoreIndex[cbScoreIndex];
	}

	//���½���
	if (bPlaceJetton==true) UpdateGameView(NULL);

	return;
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
void CGameClientView::SetWinnerSide(bool bWinShunMen, bool bWinDuiMen, bool bWinDaoMen, bool bSet)
{	
	//����ʱ��
	if (true==bSet)
	{
		//���ö�ʱ��
		SetTimer(IDI_FLASH_WINNER,500,NULL);

		//ȫʤ�ж�
		if (false==bWinShunMen && false==bWinDuiMen && false==bWinDaoMen)
		{
			//�����Դ
			m_ImageFrameShunMen.Destroy();
			m_ImageFrameJiaoL.Destroy();
			m_ImageFrameQiao.Destroy();
			m_ImageFrameDuiMen.Destroy();
			m_ImageFrameDaoMen.Destroy();
			m_ImageFrameJiaoR.Destroy();

			//������Դ
			HINSTANCE hInstance=AfxGetInstanceHandle();
			m_ImageFrameShunMen.SetLoadInfo(IDB_FRAME_SHUN_MEN, hInstance);
			m_ImageFrameJiaoL.SetLoadInfo(IDB_FRAME_JIAO_L, hInstance);
			m_ImageFrameQiao.SetLoadInfo(IDB_FRAME_QIAO, hInstance);
			m_ImageFrameDuiMen.SetLoadInfo(IDB_FRAME_DUI_MEN, hInstance);
			m_ImageFrameDaoMen.SetLoadInfo(IDB_FRAME_DAO_EMN, hInstance);
			m_ImageFrameJiaoR.SetLoadInfo(IDB_FRAME_JIAO_R, hInstance);
		}
	}
	else 
	{
		//�����ʱ��
		KillTimer(IDI_FLASH_WINNER);

		//ȫʤ�ж�
		if (false==m_bWinDaoMen && false==m_bWinShunMen && false==m_bWinDuiMen)
		{
			//�����Դ
			m_ImageFrameShunMen.Destroy();
			m_ImageFrameJiaoL.Destroy();
			m_ImageFrameQiao.Destroy();
			m_ImageFrameDuiMen.Destroy();
			m_ImageFrameDaoMen.Destroy();
			m_ImageFrameJiaoR.Destroy();

			//������Դ
			HINSTANCE hInstance=AfxGetInstanceHandle();
			m_ImageFrameShunMen.SetLoadInfo(IDB_FRAME_SHUN_MEN_EX, hInstance);
			m_ImageFrameJiaoL.SetLoadInfo(IDB_FRAME_JIAO_L_EX, hInstance);
			m_ImageFrameQiao.SetLoadInfo(IDB_FRAME_QIAO_EX, hInstance);
			m_ImageFrameDuiMen.SetLoadInfo(IDB_FRAME_DUI_MEN_EX, hInstance);
			m_ImageFrameDaoMen.SetLoadInfo(IDB_FRAME_DAO_EMN_EX, hInstance);
			m_ImageFrameJiaoR.SetLoadInfo(IDB_FRAME_JIAO_R_EX, hInstance);
		}
	}

	//���ñ���
	m_bWinShunMen=bWinShunMen;
	m_bWinDuiMen=bWinDuiMen;
	m_bWinDaoMen=bWinDaoMen;
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
	if (m_rcShunMen.PtInRect(MousePoint)) return ID_SHUN_MEN;
	if (m_rcJiaoL.PtInRect(MousePoint)) return ID_JIAO_L;
	if (m_rcQiao.PtInRect(MousePoint)) return ID_QIAO;
	if (m_rcDuiMen.PtInRect(MousePoint)) return ID_DUI_MEN;	
	if (m_rcDaoMen.PtInRect(MousePoint)) return ID_DAO_MEN;	
	if (m_rcJiaoR.PtInRect(MousePoint)) return ID_JIAO_R;

	return 0xFF;
}

//�滭����
void CGameClientView::DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos, bool bMeScore)
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
	INT nYDrawPos=nYPos-SizeScoreNumber.cy/2-24;
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
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos)
{
	CString strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;
	if (lNumber==0) strNumber=TEXT("0");
	int nNumberCount=0;
	LONGLONG lTmpNumber=lNumber;
	if (lNumber<0) lNumber=-lNumber;
	while (lNumber>0)
	{
		strTmpNumber1.Format(TEXT("%I64d"),lNumber%10);
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
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, CRect rcRect, INT nFormat)
{
	CString static strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;
	strTmpNumber1.Empty();
	strTmpNumber2.Empty();
	strNumber.Empty();
	if (lNumber==0) strNumber=TEXT("0");
	int nNumberCount=0;
	LONGLONG lTmpNumber=lNumber;
	if (lNumber<0) lNumber=-lNumber;
	while (lNumber>0)
	{
		strTmpNumber1.Format(TEXT("%I64d"),lNumber%10);
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

void CGameClientView::DrawNumberStringWithPercentage(CDC * pDC, LONGLONG lNumber, CRect rcRect, INT nFormat)
{
	CString static strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;
	strTmpNumber1.Empty();
	strTmpNumber2.Empty();
	strNumber.Empty();
	if (lNumber==0) strNumber=TEXT("0");

	strTmpNumber1.Format(TEXT("%I64d"),lNumber%100);
	strTmpNumber2.Format(TEXT("%I64d"),lNumber/100);

	strTmpNumber1+=TEXT("%");
	strTmpNumber2+=TEXT(".");

	strNumber=strTmpNumber2+strTmpNumber1;

	//�������
	if (nFormat==-1) pDC->DrawText(strNumber,rcRect,DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);
	else pDC->DrawText(strNumber,rcRect,nFormat);
}

//��ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{
	//����ʤ��
	if (nIDEvent==IDI_FLASH_WINNER)
	{
		//���ñ���
		m_bFlashResult=!m_bFlashResult;

		//���½���
		UpdateGameView(NULL);

		return;
	}
	if(IDI_POSTCARD ==nIDEvent)
	{

		if(m_bcShowCount<2)
		{
			if(m_CardControl[4].m_CardTempItemArray.GetCount()>0)
			{
				m_CardControl[4].m_blhideOneCard = false;
				m_CardControl[4].m_blGameEnd = true;
				m_CardControl[4].SetDisplayFlag(true);
				m_CardControl[4].ShowCardControl(true);
				m_CardControl[4].m_CardItemArray.SetSize(1);
				m_CardControl[4].m_CardItemArray[0]=m_CardControl[4].m_CardTempItemArray[0];
				m_CardControl[4].m_CardItemArray[0].cbCardData = m_bcfirstShowCard;
				m_CardControl[4].m_CardItemArray[0].bShoot = false;
			}
		
		}else
		{
			m_CardControl[4].m_CardItemArray.SetSize(0);

			bool blKillTimer = true;
			for (int i = 0;i<4;i++)
			{
				if(m_CardControl[i].m_CardItemArray.GetCount()!=m_CardControl[i].m_CardTempItemArray.GetCount())
				{
					blKillTimer = false;
					break;
				}
			}
			m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("DISPATCH_CARD"));
			m_PostCardIndex = m_PostCardIndex%4;
			m_CardControl[m_PostCardIndex].OnCopyCard();
			m_PostCardIndex++;
			if(blKillTimer)
			{
					
				KillTimer(IDI_POSTCARD);
				SetTimer(IDI_OPENCARD,50,NULL);
			}
			UpdateGameView(NULL);

		}
		m_bcShowCount++;
	
	
	}
	if(IDI_OPENCARD==nIDEvent)
	{
		if(m_blAutoOpenCard == false)
		{
			while (1)
			{
				bool blCopy = false;
				for (int i = 0;i<4;i++)
				{
					if(m_CardControl[i].m_CardItemArray.GetCount()!=m_CardControl[i].m_CardTempItemArray.GetCount())
					{
						blCopy = true;
						break;
					}
				}
				if(false == blCopy)
				{
					break;
				}else
				{
					m_CardControl[i].OnCopyCard();
				}
			}

			for (int i = 0;i<4;i++)
			{
				m_CardControl[i].m_blGameEnd = false;
				m_CardControl[i].m_blhideOneCard = false;
			}
			//���½���
			UpdateGameView(NULL);
			KillTimer(IDI_OPENCARD);

			SetTimer(IDI_DISPATCH_CARD,10000,NULL);

			return ;

		}
		{
			if(m_CardControl[m_OpenCardIndex%4].m_MovePoint.x<40&&m_CardControl[m_OpenCardIndex%4].m_blAdd)
			{
				m_CardControl[m_OpenCardIndex%4].m_blGameEnd = false;
				m_CardControl[m_OpenCardIndex%4].m_MovePoint.x+=2;
				m_CardControl[m_OpenCardIndex%4].m_blhideOneCard = false;
				//���½���
				UpdateGameView(NULL);


			}else
			{

				m_CardControl[m_OpenCardIndex%4].m_blAdd = false;
				m_CardControl[m_OpenCardIndex%4].m_blGameEnd = false;
				if(m_CardControl[m_OpenCardIndex%4].m_MovePoint.x>10)
				m_CardControl[m_OpenCardIndex%4].m_MovePoint.x-=2;
				m_CardControl[m_OpenCardIndex%4].m_blhideOneCard = false;
				UpdateGameView(NULL);
				if(m_CardControl[m_OpenCardIndex%4].m_MovePoint.x<=10)
				{
					m_CardControl[m_OpenCardIndex%4].m_tMoveCard.m_blMoveCard = true;
					m_CardControl[m_OpenCardIndex%4].m_tMoveCard.m_blMoveFinish = true;
					m_CardControl[m_OpenCardIndex%4].m_MovePoint.x = 0;
					m_CardControl[m_OpenCardIndex%4].m_MovePoint.y = 0;
					m_CardControl[m_OpenCardIndex%4].m_blAdd = true;
					m_CardControl[m_OpenCardIndex%4].m_blGameEnd = true;
					m_OpenCardIndex++;
					if(m_OpenCardIndex==5)
					{
						for (int i = 0;i<4;i++)
						{
							m_CardControl[i].m_blGameEnd = true;
						}
						KillTimer(IDI_OPENCARD);
						UpdateGameView(NULL);
						//������ʾ�ƶ�ʱ��
						SetTimer(IDI_DISPATCH_CARD,30,NULL);
					}

				}

			}
		}
	}
	//�ֻ�ׯ��
	else if ( nIDEvent == IDI_SHOW_CHANGE_BANKER )
	{
		ShowChangeBanker( false );

		return;
	}
	else if (nIDEvent==IDI_DISPATCH_CARD)
	{
		//��ɷ���
		FinishDispatchCard();
		{
			for (int i = 0;i<4;i++)
			{
				m_CardControl[i].m_blGameEnd = true;
				m_CardControl[i].m_blhideOneCard = false;
			}
			KillTimer(IDI_OPENCARD);
			UpdateGameView(NULL);

		}
		while (1)
		{
			bool blCopy = false;
			for (int i = 0;i<4;i++)
			{
				if(m_CardControl[i].m_CardItemArray.GetCount()!=m_CardControl[i].m_CardTempItemArray.GetCount())
				{
					blCopy = true;
					break;
				}
			}
			if(false == blCopy)
			{
				break;
			}else
			{
				m_CardControl[i].OnCopyCard();
			}
		}
		m_blMoveFinish = true;
		//���½���
		UpdateGameView(NULL);

		return;
	}
	else if (IDI_SHOWDISPATCH_CARD_TIP==nIDEvent)
	{
		SetDispatchCardTip(enDispatchCardTip_NULL);

	}else if(nIDEvent==IDI_MOVECARD_END)
	{
		KillTimer(IDI_MOVECARD_END);
	}

	__super::OnTimer(nIDEvent);
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	for (int i = 0;i<4;i++)
	{
		if(!m_CardControl[i].m_blAuto)
		{
			m_CardControl[i].OnLButtonDown(nFlags, Point);
		}
	}
		
	if (m_lCurrentJetton!=0L)
	{
		//��ע����
		BYTE cbJettonArea=GetJettonArea(Point);
	
		//�����ע
		LONGLONG lMaxJettonScore=GetUserMaxJetton();

		//�Ϸ��ж�
		if (lMaxJettonScore < m_lCurrentJetton) return ;

		//������Ϣ
		if (cbJettonArea!=0xFF) AfxGetMainWnd()->SendMessage(IDM_PLACE_JETTON,cbJettonArea,(LONG)m_lCurrentJetton);
	}
	UpdateGameView(NULL);

	__super::OnLButtonDown(nFlags,Point);
}
void CGameClientView::OnLButtonUp(UINT nFlags, CPoint Point)
{
	
	for (int i = 0;i<4;i++)
	{
		if(!m_CardControl[i].m_blAuto)
		{
			m_CardControl[i].OnLButtonUp(nFlags, Point);
			UpdateGameView(NULL);
		}
		
	}
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
//����ƶ���Ϣ
void CGameClientView::OnMouseMove(UINT nFlags, CPoint point)
{
	for (int i = 0;i<4;i++)
	{
		if(!m_CardControl[i].m_blAuto)
		{
			m_CardControl[i].OnMouseMove(nFlags, point);
			UpdateGameView(NULL);

		}

	}
	__super::OnMouseMove(nFlags,point);
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

		//�����ע
		LONGLONG lMaxJettonScore=GetUserMaxJetton();

		////ׯ�ҽ��
		//LONGLONG lBankerScore=2147483647;
		//if (m_wBankerUser!=INVALID_CHAIR) lBankerScore=m_lBankerScore;
		//for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lBankerScore-=m_lAllJettonScore[nAreaIndex];

		////������ѹ��ׯ�ҵ�ȫ��������
		//if(lMaxJettonScore==lBankerScore) lMaxJettonScore/=2;
		//if(lMaxJettonScore>100000000L) lMaxJettonScore=100000000L;
		//lMaxJettonScore=lMaxJettonScore/100*100;

		//�Ϸ��ж�
		if (lMaxJettonScore < m_lCurrentJetton)
		{
			SetCursor(LoadCursor(NULL,IDC_NO));
			return TRUE;
		}
	
		////ȫѹ���ù��
		//if(m_lCurrentJetton==lMaxJettonScore)
		//{
		//	SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_500000)));
		//	return TRUE;
		//}

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

//�滭��ʶ
void CGameClientView::DrawWinFlags(CDC * pDC)
{
	//�ǿ��ж�
	if (m_nRecordLast==m_nRecordFirst) return;

	//��Դ����
	CImageHandle ImageHandleWinFlags(&m_ImageWinFlags);
	int nIndex = m_nScoreHead;
	COLORREF static clrOld ;

	clrOld = pDC->SetTextColor(RGB(255,234,0));
	int nDrawCount=0;
	while ( nIndex != m_nRecordLast && ( m_nRecordLast!=m_nRecordFirst ) && nDrawCount < MAX_FALG_COUNT )
	{
		//ʤ����ʶ
		tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[nIndex];
		bool static bWinMen[3];
		bWinMen[0]=ClientGameRecord.bWinShunMen;
		bWinMen[1]=ClientGameRecord.bWinDaoMen;
		bWinMen[2]=ClientGameRecord.bWinDuiMen;

		//�������
		enOperateResult static OperateResult[3];
		OperateResult[0]=ClientGameRecord.enOperateShunMen;
		OperateResult[1]=ClientGameRecord.enOperateDaoMen;
		OperateResult[2]=ClientGameRecord.enOperateDuiMen;

		for (int i=0; i<3; ++i)
		{
			//λ�ñ���
			int static nYPos=0,nXPos=0;
			nYPos=m_nWinFlagsExcursionY+i*32;
			nXPos=m_nWinFlagsExcursionX + ((nIndex - m_nScoreHead + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY) * 39;

			//ʤ����ʶ
			int static nFlagsIndex=0;
			if (false==bWinMen[i])
			{
				nFlagsIndex=2;
			}
			else
			{
				nFlagsIndex=0;
			}
			if (OperateResult[i] != enOperateResult_NULL)
			{			
				nFlagsIndex+=1;

				if(nFlagsIndex>=4)
				{
					nFlagsIndex=3;
				}
			}
			//�滭��ʶ
			m_ImageWinFlags.AlphaDrawImage( pDC, nXPos+55, nYPos, m_ImageWinFlags.GetWidth()/4 , 
				m_ImageWinFlags.GetHeight(),m_ImageWinFlags.GetWidth()/4 * nFlagsIndex, 0, RGB(255, 0, 255) );
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
	//��ʾ�ж�
	//if (m_pGameClientDlg->GetGameStatus()!=GS_GAME_END) return;
	if (false==m_bShowGameResult) return;

	int	nXPos = nWidth / 2 - 129;
	int	nYPos = nHeight / 2 - 208;
	CImageHandle ImageHandleGameEnd(&m_ImageGameEnd);

	CRect rcAlpha(nXPos+2, nYPos+70, nXPos+2 + m_ImageGameEnd.GetWidth(), nYPos+70+m_ImageGameEnd.GetHeight());
	DrawAlphaRect(pDC, &rcAlpha, RGB(74,70,73), 0.8f);
	m_ImageGameEnd.AlphaDrawImage(pDC, nXPos+2, nYPos+70, RGB(255,0,255));

	pDC->SetTextColor(RGB(255,234,0));
	CRect rcMeWinScore, rcMeReturnScore;
	rcMeWinScore.left = nXPos+2 + 40;
	rcMeWinScore.top = nYPos+70 + 32;
	rcMeWinScore.right = rcMeWinScore.left + 111;
	rcMeWinScore.bottom = rcMeWinScore.top + 34;

	rcMeReturnScore.left = nXPos+2 + 150;
	rcMeReturnScore.top = nYPos+70 + 32;
	rcMeReturnScore.right = rcMeReturnScore.left + 111;
	rcMeReturnScore.bottom = rcMeReturnScore.top + 34;

	CString strMeGameScore, strMeReturnScore;
	DrawNumberStringWithSpace(pDC,m_lMeCurGameScore,rcMeWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	DrawNumberStringWithSpace(pDC,m_lMeCurGameReturnScore,rcMeReturnScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	CRect rcBankerWinScore;
	rcBankerWinScore.left = nXPos+2 + 40;
	rcBankerWinScore.top = nYPos+70 + 69;
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

//�����ע
LONGLONG CGameClientView::GetUserMaxJetton()
{
	//����ע��
	LONGLONG lNowJetton = 0;
	ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lNowJetton += m_lUserJettonScore[nAreaIndex];

	//ׯ�ҽ��
	LONGLONG lBankerScore=2147483647;
	if (m_wBankerUser!=INVALID_CHAIR) lBankerScore=m_lBankerScore;
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lBankerScore-=m_lAllJettonScore[nAreaIndex];

	//��������
	LONGLONG lMeMaxScore=min(m_lMeMaxScore-lNowJetton,m_lAreaLimitScore);

	//ׯ������
	lMeMaxScore=min(lMeMaxScore,lBankerScore);

	//��������
	ASSERT(lMeMaxScore >= 0);
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

//�滭����
void CGameClientView::DrawMeJettonNumber(CDC *pDC)
{
	//�滭����
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
	{
		if (m_lUserJettonScore[nAreaIndex] > 0 )
			DrawNumberString(pDC,m_lUserJettonScore[nAreaIndex],m_PointJettonNumber[nAreaIndex-1].x,m_PointJettonNumber[nAreaIndex-1].y+25, true);
	}
}

//��ʼ����
void CGameClientView::DispatchCard()
{
	//���ý���
	for (int i=0; i<CountArray(m_CardControl); ++i) m_CardControl[i].m_CardItemArray.SetSize(0);
	//���ñ�ʶ
	m_bNeedSetGameRecord=true;
}

//��������
void CGameClientView::FinishDispatchCard()
{
	//����ж�
	if (m_bNeedSetGameRecord==false) return;

	//���ñ�ʶ
	m_bNeedSetGameRecord=false;

	//ɾ����ʱ��
	KillTimer(IDI_DISPATCH_CARD);

	////�����˿�
	//for (int i=0; i<CountArray(m_CardControl); ++i) m_CardControl[i].SetCardData(m_cbTableCardArray[i],2);

	//�ƶ�Ӯ��
	bool bWinShunMen, bWinDuiMen, bWinDaoMen;
	DeduceWinner(bWinShunMen, bWinDuiMen, bWinDaoMen);	

	//�����¼
	SetGameHistory(bWinShunMen, bWinDaoMen, bWinDuiMen);

	//�ۼƻ���
	m_lMeStatisticScore+=m_lMeCurGameScore;
	m_lBankerWinScore=m_lTmpBankerWinScore;

	//����Ӯ��
	SetWinnerSide(bWinShunMen, bWinDuiMen, bWinDaoMen, true);

	//��������
	if (m_lMeCurGameScore>0) m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_WIN"));
	else if (m_lMeCurGameScore<0) m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_LOST"));
	else m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_DRAW"));
}

//ʤ���߿�
void CGameClientView::FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC)
{
	//�Ϸ��ж�
	if (m_cbAreaFlash==0xFF && false==m_bFlashResult) return;

	//��ע�ж�
	if (false==m_bFlashResult)
	{
		switch (m_cbAreaFlash)
		{
		case ID_SHUN_MEN:	
			{
				//�滭ͼƬ
				CImageHandle ImageHandle(&m_ImageFrameShunMen);
				m_ImageFrameShunMen.AlphaDrawImage(pDC,m_rcShunMen.left, m_rcShunMen.top, RGB(255,0,255));
				break;
			}
		case ID_JIAO_L:
			{
				//�滭ͼƬ
				CImageHandle ImageHandle(&m_ImageFrameJiaoL);
				m_ImageFrameJiaoL.AlphaDrawImage(pDC,m_rcJiaoL.left, m_rcJiaoL.top, RGB(255,0,255));
				break;
			}
		case ID_QIAO:
			{	
				//�滭ͼƬ
				CImageHandle ImageHandle(&m_ImageFrameQiao);
				m_ImageFrameQiao.AlphaDrawImage(pDC,m_rcQiao.left, m_rcQiao.top, RGB(255,0,255));
				break;
			}
		case ID_DUI_MEN:
			{
				//�滭ͼƬ
				CImageHandle ImageHandle(&m_ImageFrameDuiMen);	
				m_ImageFrameDuiMen.AlphaDrawImage(pDC,m_rcDuiMen.left, m_rcDuiMen.top, RGB(255,0,255));
				break;
			}
		case ID_DAO_MEN:
			{
				//�滭ͼƬ
				CImageHandle ImageHandle(&m_ImageFrameDaoMen);	
				m_ImageFrameDaoMen.AlphaDrawImage(pDC,m_rcDaoMen.left, m_rcDaoMen.top, RGB(255,0,255));
				break;
			}
		case ID_JIAO_R:
			{
				//�滭ͼƬ
				CImageHandle ImageHandle(&m_ImageFrameJiaoR);
				m_ImageFrameJiaoR.AlphaDrawImage(pDC,m_rcJiaoR.left, m_rcJiaoR.top, RGB(255,0,255));
				break;
			}
		}
	}
	else
	{
		//ׯȫʤ�ж�
		if (false==m_bWinDaoMen && false==m_bWinShunMen && false==m_bWinDuiMen)
		{
			//������Դ
			CImageHandle ImageHandleShunMen(&m_ImageFrameShunMen);
			CImageHandle ImageHandleJiaoL(&m_ImageFrameJiaoL);
			CImageHandle ImageHandleQiao(&m_ImageFrameQiao);
			CImageHandle ImageHandleDuiMen(&m_ImageFrameDuiMen);	
			CImageHandle ImageHandleDaoMen(&m_ImageFrameDaoMen);	
			CImageHandle ImageHandleJiaoR(&m_ImageFrameJiaoR);

			//�滭ͼƬ
			m_ImageFrameShunMen.AlphaDrawImage(pDC,m_rcShunMen.left, m_rcShunMen.top, RGB(255,0,255));
			m_ImageFrameJiaoL.AlphaDrawImage(pDC,m_rcJiaoL.left, m_rcJiaoL.top, RGB(255,0,255));
			m_ImageFrameQiao.AlphaDrawImage(pDC,m_rcQiao.left, m_rcQiao.top, RGB(255,0,255));
			m_ImageFrameDuiMen.AlphaDrawImage(pDC,m_rcDuiMen.left, m_rcDuiMen.top, RGB(255,0,255));
			m_ImageFrameDaoMen.AlphaDrawImage(pDC,m_rcDaoMen.left, m_rcDaoMen.top, RGB(255,0,255));
			m_ImageFrameJiaoR.AlphaDrawImage(pDC,m_rcJiaoR.left, m_rcJiaoR.top, RGB(255,0,255));
		}
		else
		{
			//���ж�
			if (true==m_bWinDaoMen)
			{
				CImageHandle ImageHandleDaoMen(&m_ImageFrameDaoMen);
				m_ImageFrameDaoMen.AlphaDrawImage(pDC,m_rcDaoMen.left, m_rcDaoMen.top, RGB(255,0,255));
			}
			if (true==m_bWinShunMen)
			{
				CImageHandle ImageHandleShunMen(&m_ImageFrameShunMen);
				m_ImageFrameShunMen.AlphaDrawImage(pDC,m_rcShunMen.left, m_rcShunMen.top, RGB(255,0,255));
			}
			if (true==m_bWinDuiMen)
			{
				CImageHandle ImageHandleDuiMen(&m_ImageFrameDuiMen);
				m_ImageFrameDuiMen.AlphaDrawImage(pDC,m_rcDuiMen.left, m_rcDuiMen.top, RGB(255,0,255));
			}

			//���ж�
			if (true==m_bWinShunMen && true==m_bWinDuiMen)
			{
				CImageHandle ImageHandleJiaoL(&m_ImageFrameJiaoL);
				m_ImageFrameJiaoL.AlphaDrawImage(pDC,m_rcJiaoL.left, m_rcJiaoL.top, RGB(255,0,255));
			}
			if (true==m_bWinShunMen && true==m_bWinDaoMen)
			{
				CImageHandle ImageHandleQiao(&m_ImageFrameQiao);
				m_ImageFrameQiao.AlphaDrawImage(pDC,m_rcQiao.left, m_rcQiao.top, RGB(255,0,255));
			}
			if (true==m_bWinDuiMen && true==m_bWinDaoMen)
			{
				CImageHandle ImageHandleJiaoR(&m_ImageFrameJiaoR);
				m_ImageFrameJiaoR.AlphaDrawImage(pDC,m_rcJiaoR.left, m_rcJiaoR.top, RGB(255,0,255));
			}
		}
	}
}


//�ƶ�Ӯ��
void CGameClientView::DeduceWinner(bool &bWinShunMen, bool &bWinDuiMen, bool &bWinDaoMen)
{
	//��С�Ƚ�
	bWinShunMen=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[SHUN_MEN_INDEX],2)==1?true:false;
	bWinDuiMen=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[DUI_MEN_INDEX],2)==1?true:false;
	bWinDaoMen=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[DAO_MEN_INDEX],2)==1?true:false;
}

//�ؼ�����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��ȡID
	WORD wControlID=LOWORD(wParam);

	//LONGLONG lMaxScore=GetUserMaxJetton();

	////ׯ�ҽ��
	//LONGLONG lBankerScore=2147483647;
	//if (m_wBankerUser!=INVALID_CHAIR) lBankerScore=m_lBankerScore;
	//for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lBankerScore-=m_lAllJettonScore[nAreaIndex];

	////������ѹ��ׯ�ҵ�ȫ��������
	//if(lMaxScore==lBankerScore) lMaxScore/=2;
	//if(lMaxScore>100000000L) lMaxScore=100000000L;
	//lMaxScore=lMaxScore/100*100;

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
			AfxGetMainWnd()->SendMessage(IDM_CONTINUE_CARD,0,0);
			break;
		}
	}

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
	pDC->SetTextColor(RGB(255,234,0));

	//��ȡ���
	tagUserData const *pUserData = m_wBankerUser==INVALID_CHAIR ? NULL : GetUserInfo(m_wBankerUser);

	//λ����Ϣ
	CRect static StrRect;
	StrRect.left = nWidth/2-298;
	StrRect.top = nHeight/2 - 328;
	StrRect.right = StrRect.left + 190;
	StrRect.bottom = StrRect.top + 15;

	//ׯ������
	pDC->DrawText(pUserData==NULL?(m_bEnableSysBanker?TEXT("ϵͳ��ׯ"):TEXT("������ׯ")):pUserData->szName, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_SINGLELINE );

	//ׯ���ܷ�
	StrRect.left = nWidth/2-298;
	StrRect.top = nHeight/2 - 306;
	StrRect.right = StrRect.left + 190;
	StrRect.bottom = StrRect.top + 15;
	DrawNumberStringWithSpace(pDC,pUserData==NULL?0:pUserData->lScore, StrRect);

	//ׯ�ҳɼ�
	StrRect.left = nWidth/2-298;
	StrRect.top = nHeight/2 - 265;
	StrRect.right = StrRect.left + 190;
	StrRect.bottom = StrRect.top + 15;
	DrawNumberStringWithSpace(pDC,m_wBankerTime,StrRect);

	//ׯ�Ҿ���
	StrRect.left = nWidth/2-298;
	StrRect.top = nHeight/2 - 287;
	StrRect.right = StrRect.left + 190;
	StrRect.bottom = StrRect.top + 15;
	DrawNumberStringWithSpace(pDC,m_lBankerWinScore,StrRect);
}
void CGameClientView::SetFirstShowCard(BYTE bcCard)
{
	m_bcfirstShowCard =  bcCard;
	m_CardControl[4].SetCardData(NULL,0);
	m_CardControl[4].SetCardData(&bcCard,1);
	m_CardControl[4].m_blGameEnd = true;
	m_CardControl[4].SetDisplayFlag(true);
	m_CardControl[4].ShowCardControl(true);
}
//�滭���
void CGameClientView::DrawMeInfo(CDC *pDC,int nWidth,int nHeight)
{
	//�Ϸ��ж�
	if (INVALID_CHAIR==m_wMeChairID) return;

	const tagUserData *pMeUserData = GetUserInfo(m_wMeChairID);

	LONGLONG lNowJetton = 0;
	ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lNowJetton += m_lUserJettonScore[nAreaIndex];

	if ( pMeUserData != NULL )
	{
		//������ɫ
		pDC->SetTextColor(RGB(255,255,255));

		//�ܹ���ע
		LONGLONG lMeJetton=0L;
		for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lMeJetton += m_lUserJettonScore[nAreaIndex];

		//����λ��
		CRect static rcDraw;
		rcDraw.left=nWidth/2-218-85;
		rcDraw.top=nHeight/2+178-13;
		rcDraw.right=rcDraw.left+140;
		rcDraw.bottom=rcDraw.top+15;

		//�滭�ʺ�
		pDC->DrawText(pMeUserData->szName,lstrlen(pMeUserData->szName),rcDraw,DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);

		//�滭����
		rcDraw.top=nHeight/2+178+20-13;
		rcDraw.bottom=rcDraw.top+15;
		DrawNumberStringWithSpace(pDC,pMeUserData->lScore-lMeJetton,rcDraw,DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);

		//�滭�ɼ�
		rcDraw.top=nHeight/2+178+2*19-13;
		rcDraw.bottom=rcDraw.top+15;
		DrawNumberStringWithSpace(pDC,m_lMeStatisticScore,rcDraw,DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);

		//������ע
		rcDraw.top=nHeight/2+178+3*19-13;
		rcDraw.bottom=rcDraw.top+15;
		DrawNumberStringWithSpace(pDC,lNowJetton,rcDraw,DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);

	}
}
//////////////////////////////////////////////////////////////////////////
//���д��
void CGameClientView::OnBankStorage()
{
	//��ȡ�ӿ�
	CGameClientDlg *pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientDlg->GetClientKernel(IID_IClientKernel,VER_IClientKernel);

	if (NULL!=pIClientKernel)
	{
		if (NULL==m_DlgBank.m_hWnd) m_DlgBank.Create(IDD_BANK_STORAGE,this);
		m_DlgBank.SetSendInfo(pIClientKernel,pIClientKernel->GetMeUserInfo());
		m_DlgBank.SetBankerActionType(true);
		m_DlgBank.ShowWindow(SW_SHOW);

		CPoint Point;
		GetCursorPos(&Point);
		CRect Rect;
		CRect btRect;
		m_btBankerStorage.GetWindowRect(&btRect);
		m_DlgBank.GetWindowRect(&Rect);
		m_DlgBank.SetPostPoint(CPoint(btRect.right,btRect.top ));
		//m_DlgBank.SetWindowPos(NULL,)
	//	m_DlgBank.CenterWindow();
	}
}

//����ȡ��
void CGameClientView::OnBankDraw()
{
	//��ȡ�ӿ�
	CGameClientDlg *pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientDlg->GetClientKernel(IID_IClientKernel,VER_IClientKernel);

	if (NULL!=pIClientKernel)
	{
		if (NULL==m_DlgBank.m_hWnd) m_DlgBank.Create(IDD_BANK_STORAGE,this);
		m_DlgBank.SetSendInfo(pIClientKernel,pIClientKernel->GetMeUserInfo());
		m_DlgBank.SetBankerActionType(false);
		m_DlgBank.ShowWindow(SW_SHOW);
		CPoint Point;
		GetCursorPos(&Point);
		CRect Rect;
		CRect btRect;
		m_btBankerDraw.GetWindowRect(&btRect);
		m_DlgBank.GetWindowRect(&Rect);
		m_DlgBank.SetPostPoint(CPoint(btRect.right,btRect.top));
		//m_DlgBank.CenterWindow();
	}
}

//����ʤ��
void CGameClientView::DrawWinInfo(CDC *pDC,int nWidth,int nHeight)
{
	/*float fShunMenWin=0.0f;
	float fDuiMenWin=0.0f;
	float fDaoMenWin=0.0f;*/

	int iShunMenWin=0;
	int iDuiMenWin=0;
	int iDaoMenWin=0;

	int iWinShunMenCount=0;
	int iWinDuiMenCount=0;
	int iWinDaoMenCount=0;
	int nRecordCount=(m_nRecordLast - m_nRecordFirst + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;

	for(int i=0;i<nRecordCount;i++)
	{
		if(m_GameRecordArrary[i].bWinShunMen) iWinShunMenCount++;
		if(m_GameRecordArrary[i].bWinDaoMen) iWinDaoMenCount++;
		if(m_GameRecordArrary[i].bWinDuiMen) iWinDuiMenCount++;
	}

	iShunMenWin=(float)iWinShunMenCount/nRecordCount*10000;
	iDaoMenWin=(float)iWinDaoMenCount/nRecordCount*10000;
	iDuiMenWin=(float)iWinDuiMenCount/nRecordCount*10000;

	pDC->SetTextColor(RGB(255,234,0));

	CRect static rcDraw;
	rcDraw.left=nWidth/2-218-83;
	rcDraw.top=nHeight/2+268-13;
	rcDraw.right=rcDraw.left+140;
	rcDraw.bottom=rcDraw.top+15;

	DrawNumberStringWithPercentage(pDC,iShunMenWin,rcDraw,DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);

	rcDraw.top=nHeight/2+178+2*19-13+70+15;
	rcDraw.bottom=rcDraw.top+15;
	DrawNumberStringWithPercentage(pDC,iDaoMenWin,rcDraw,DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);

	rcDraw.top=nHeight/2+178+3*19-13+70+28;
	rcDraw.bottom=rcDraw.top+15;
	DrawNumberStringWithPercentage(pDC,iDuiMenWin,rcDraw,DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);

}