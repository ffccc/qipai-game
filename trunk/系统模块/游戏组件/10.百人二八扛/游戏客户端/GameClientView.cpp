#include "StdAfx.h"
#include "Math.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClientDlg.h"
#include ".\gameclientview.h"

//////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_FLASH_WINNER			100									//闪动标识
#define IDI_SHOW_CHANGE_BANKER		101									//轮换庄家
#define IDI_DISPATCH_CARD			102									//发牌标识
#define IDI_SHOWDISPATCH_CARD_TIP	103									//发牌提示
#define IDI_OPENCARD				104									//发牌提示
#define IDI_MOVECARD_END			105									//移動牌結束
#define IDI_POSTCARD				106									//发牌提示
#define IDI_SICE					107									//发牌提示
#define IDI_OPENCARD_2				108									//发牌提示

//按钮标识
#define IDC_JETTON_BUTTON_100		200									//按钮标识
#define IDC_JETTON_BUTTON_1000		201									//按钮标识
#define IDC_JETTON_BUTTON_10000		202									//按钮标识
#define IDC_JETTON_BUTTON_100000	203									//按钮标识
#define IDC_JETTON_BUTTON_1000000	204									//按钮标识
#define IDC_JETTON_BUTTON_5000000	205									//按钮标识
#define IDC_APPY_BANKER				206									//按钮标识
#define IDC_CANCEL_BANKER			207									//按钮标识
#define IDC_SCORE_MOVE_L			209									//按钮标识
#define IDC_SCORE_MOVE_R			210									//按钮标识
#define IDC_VIEW_CHART				211									//按钮标识
#define IDC_JETTON_BUTTON_50000	    219									//按钮标识
#define IDC_JETTON_BUTTON_500000	212									//按钮标识
#define IDC_AUTO_OPEN_CARD			213									//按钮标识
#define IDC_OPEN_CARD				214									//按钮标识
#define IDC_BANK					215									//按钮标识
#define IDC_CONTINUE_CARD			216									//按钮标识
#define IDC_BANK_STORAGE			217									//按钮标识
#define IDC_BANK_DRAW				218									//按钮标识
#define IDC_CONTROL_BUTTON			230									//按钮标识
#define IDC_UP						223									//按钮标识
#define IDC_DOWN					224									//按钮标识
#define IDC_WIN_RATE				225									//按钮标识

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
	ON_BN_CLICKED(IDC_AUTO_OPEN_CARD, OnAutoOpenCard)
	ON_MESSAGE(WM_VIEWLBTUP, OnViLBtUp)
	ON_BN_CLICKED(IDC_CONTROL_BUTTON, OnControl)

	ON_BN_CLICKED(IDC_BANK, OnBankStorage)
	ON_BN_CLICKED(IDC_BANK_DRAW, OnBankDraw)

	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	ON_BN_CLICKED(IDC_WIN_RATE, OnControl)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientView::CGameClientView() : CGameFrameView(true, 24)
{
	//下注信息
	ZeroMemory(m_lUserJettonScore, sizeof(m_lUserJettonScore));

	//全体下注
	ZeroMemory(m_lAllJettonScore, sizeof(m_lAllJettonScore));

	m_bShowLeft = false;						//显示左挂角
	m_bShowRight = false;					//显示右挂角


	//庄家信息
	m_wBankerUser = INVALID_CHAIR;
	m_wBankerTime = 0;
	m_lBankerScore = 0L;
	m_lBankerWinScore = 0L;
	m_lTmpBankerWinScore = 0;

	//当局成绩
	m_lMeCurGameScore = 0L;
	m_lMeCurGameReturnScore = 0L;
	m_lBankerCurGameScore = 0L;
	m_lGameRevenue = 0L;

	//状态信息
	m_lCurrentJetton = 0L;
	m_cbAreaFlash = 0xFF;
	m_wMeChairID = INVALID_CHAIR;
	m_bShowChangeBanker = false;
	m_bNeedSetGameRecord = false;
	m_bWinTianMen = false;
	m_bWinXuanMen = false;
	m_bFlashResult = false;
	m_blMoveFinish = false;
	m_blAutoOpenCard = true;
	m_enDispatchCardTip = enDispatchCardTip_NULL;

	m_lMeCurGameScore = 0L;
	m_lMeCurGameReturnScore = 0L;
	m_lBankerCurGameScore = 0L;

	m_lAreaLimitScore = 0L;

	//位置信息
	m_nScoreHead = 0;
	m_nRecordFirst = 0;
	m_nRecordLast = 0;

	//历史成绩
	m_lMeStatisticScore = 0;

	//控件变量
	m_pGameClientDlg = CONTAINING_RECORD(this, CGameClientDlg, m_GameClientView);

	//加载位图
	HINSTANCE hInstance = AfxGetInstanceHandle();
	m_ImageViewFill.SetLoadInfo(IDB_VIEW_FILL, hInstance);
	m_ImageViewBack.SetLoadInfo(IDB_VIEW_BACK, hInstance);
	m_ImageWinFlags.SetLoadInfo(IDB_WIN_FLAGS, hInstance);
	m_ImageJettonView.SetLoadInfo(IDB_JETTOM_VIEW, hInstance);
	m_ImageScoreNumber.SetLoadInfo(IDB_SCORE_NUMBER, hInstance);
	m_ImageMeScoreNumber.SetLoadInfo(IDB_ME_SCORE_NUMBER, hInstance);

	m_ImageBackLeft01.SetLoadInfo(APPLY_USER_BACK_LEFT_1, hInstance);
	m_ImageBackLeft02.SetLoadInfo(APPLY_USER_BACK_LEFT_2, hInstance);
	m_ImageBackRight01.SetLoadInfo(APPLY_USER_BACK_1, hInstance);
	m_ImageBackRight02.SetLoadInfo(APPLY_USER_BACK_2, hInstance);

	m_ImageFrameTianMen.SetLoadInfo(IDB_FRAME_TIAN_EMN, hInstance);
	m_ImageFrameDiMen.SetLoadInfo(IDB_FRAME_DI_EMN, hInstance);
	m_ImageFrameXuanMen.SetLoadInfo(IDB_FRAME_XUAN_EMN, hInstance);

	m_ImageFrameQiao.SetLoadInfo(IDB_FRAME_QIAO_EX, hInstance);
	m_ImageFrameJiaoR.SetLoadInfo(IDB_FRAME_JIAO_R_EX, hInstance);

	m_ImageGameEnd.SetLoadInfo(IDB_GAME_END, hInstance);
	m_ImageCodeBack.SetLoadInfo(CODE_BACK, hInstance);
	m_ImageCardSmall.SetLoadInfo(CARD_SMALL, hInstance);
	m_ImageCurrentJoinCount.SetLoadInfo(CURRENT_JOIN_COUNT, hInstance);
	if (m_ImageGameBegin.IsNull())
		m_ImageGameBegin.LoadImage(hInstance, "GAME_BEGIN");

	m_ImageMeBanker.SetLoadInfo(IDB_ME_BANKER, hInstance);
	m_ImageChangeBanker.SetLoadInfo(IDB_CHANGE_BANKER, hInstance);
	m_ImageNoBanker.SetLoadInfo(IDB_NO_BANKER, hInstance);

	m_ImageTimeFlag.SetLoadInfo(IDB_TIME_FLAG, hInstance);
	m_ImageWinRate.SetLoadInfo(IDB_BT_WIN_RATE, hInstance);

	//通杀通陪
	if (m_ImageTP.IsNull())
		m_ImageTP.LoadImage(hInstance, "T_P");
	if (m_ImageTS.IsNull())
		m_ImageTS.LoadImage(hInstance, "T_S");

	return;
}

//析构函数
CGameClientView::~CGameClientView(void)
{
}

void CGameClientView::SetMoveCardTimer()
{
	KillTimer(IDI_POSTCARD);
	m_OpenCardIndex = 1;
	m_bcShowCount = 0;
	m_PostStartIndex =  m_GameLogic.GetCardValue(m_bcfirstShowCard[0]) + m_GameLogic.GetCardValue(m_bcfirstShowCard[1]);
	if (m_PostStartIndex > 1)
		m_PostStartIndex  = (m_PostStartIndex - 1) % CARD_LOCATION_COUNT;

	m_PostCardIndex = m_PostStartIndex;
	for (int i = 0; i < CARD_LOCATION_COUNT; i++)
	{
		m_CardControl[i].m_blAdd = true;
		m_CardControl[i].m_blAuto = m_blAutoOpenCard;
		m_CardControl[i].m_blGameEnd = true;
		m_CardControl[i].m_blShowResult = false;
		m_CardControl[i].m_blShowLineResult = false;
	}
	SetTimer(IDI_POSTCARD, 300, NULL);
	SetTimer(IDI_DISPATCH_CARD, 25000, NULL);
}

//建立消息
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1) return -1;

	//创建控件
	CRect rcCreate(0, 0, 0, 0);
	m_GameRecord.Create(IDD_DLG_GAME_RECORD, this);
	m_ApplyUser.Create(IDD_DLG_GAME_RECORD	, this);
	m_GameRecord.ShowWindow(SW_HIDE);

	//下注按钮
//	m_btJetton100.Create(NULL, WS_CHILD | WS_VISIBLE | WS_DISABLED, rcCreate, this, IDC_JETTON_BUTTON_100);
	m_btJetton1000.Create(NULL, WS_CHILD | WS_VISIBLE | WS_DISABLED, rcCreate, this, IDC_JETTON_BUTTON_1000);
	m_btJetton10000.Create(NULL, WS_CHILD | WS_VISIBLE | WS_DISABLED, rcCreate, this, IDC_JETTON_BUTTON_10000);
	m_btJetton50000.Create(NULL, WS_CHILD | WS_VISIBLE | WS_DISABLED, rcCreate, this, IDC_JETTON_BUTTON_50000);
	m_btJetton100000.Create(NULL, WS_CHILD | WS_VISIBLE | WS_DISABLED, rcCreate, this, IDC_JETTON_BUTTON_100000);

	m_btJetton500000.Create(NULL, WS_CHILD | WS_VISIBLE | WS_DISABLED, rcCreate, this, IDC_JETTON_BUTTON_500000);
	m_btJetton1000000.Create(NULL, WS_CHILD | WS_VISIBLE | WS_DISABLED, rcCreate, this, IDC_JETTON_BUTTON_1000000);
//	m_btJetton5000000.Create(NULL, WS_CHILD | WS_VISIBLE | WS_DISABLED, rcCreate, this, IDC_JETTON_BUTTON_5000000);

	//申请按钮
	m_btApplyBanker.Create(NULL, WS_CHILD | WS_VISIBLE | WS_DISABLED, rcCreate, this, IDC_APPY_BANKER);
	m_btCancelBanker.Create(NULL, WS_CHILD | WS_DISABLED, rcCreate, this, IDC_CANCEL_BANKER);

	m_btScoreMoveL.Create(NULL, WS_CHILD/* | WS_VISIBLE | WS_DISABLED*/, rcCreate, this, IDC_SCORE_MOVE_L);
	m_btScoreMoveR.Create(NULL, WS_CHILD/* | WS_VISIBLE | WS_DISABLED*/, rcCreate, this, IDC_SCORE_MOVE_R);

	m_btAutoOpenCard.Create(NULL, WS_CHILD/* | WS_VISIBLE*/, rcCreate, this, IDC_AUTO_OPEN_CARD);
	m_btOpenCard.Create(NULL, WS_CHILD/* | WS_VISIBLE*/ | WS_DISABLED, rcCreate, this, IDC_OPEN_CARD);

	m_btBank.Create(NULL, WS_CHILD, rcCreate, this, IDC_BANK);
	m_btContinueCard.Create(NULL, WS_CHILD, rcCreate, this, IDC_CONTINUE_CARD);

	m_btBankerStorage.Create(NULL, WS_CHILD | WS_VISIBLE | WS_DISABLED, rcCreate, this, IDC_BANK_STORAGE);
	m_btBankerDraw.Create(NULL, WS_CHILD | WS_VISIBLE, rcCreate, this, IDC_BANK_DRAW);

	m_btUp.Create(NULL, WS_CHILD, rcCreate, this, IDC_UP);
	m_btDown.Create(NULL, WS_CHILD, rcCreate, this, IDC_DOWN);
	m_btWinRate.Create(NULL, WS_CHILD/* | WS_VISIBLE*/, rcCreate, this, IDC_WIN_RATE);

//	m_btBankerStorage.EnableWindow(TRUE);

	//设置按钮
	HINSTANCE hResInstance = AfxGetInstanceHandle();
//	m_btJetton100.SetButtonImage(IDB_BT_JETTON_100, hResInstance, false);
	m_btJetton1000.SetButtonImage(IDB_BT_JETTON_1000, hResInstance, false);
	m_btJetton50000.SetButtonImage(IDB_BT_JETTON_50000, hResInstance, false);
	m_btJetton10000.SetButtonImage(IDB_BT_JETTON_10000, hResInstance, false);
	m_btJetton100000.SetButtonImage(IDB_BT_JETTON_100000, hResInstance, false);
	m_btJetton500000.SetButtonImage(IDB_BT_JETTON_500000, hResInstance, false);
	m_btJetton1000000.SetButtonImage(IDB_BT_JETTON_1000000, hResInstance, false);
//	m_btJetton5000000.SetButtonImage(IDB_BT_JETTON_5000000, hResInstance, false);

	m_btApplyBanker.SetButtonImage(IDB_BT_APPLY_BANKER, hResInstance, false);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY, hResInstance, false);


	m_btUp.SetButtonImage(IDB_BT_BT_S, hResInstance, false);
	m_btDown.SetButtonImage(IDB_BT_BT_X, hResInstance, false);

	m_btScoreMoveL.SetButtonImage(IDB_BT_SCORE_MOVE_L, hResInstance, false);
	m_btScoreMoveR.SetButtonImage(IDB_BT_SCORE_MOVE_R, hResInstance, false);

	m_btAutoOpenCard.SetButtonImage(IDB_BT_AUTO_OPEN_CARD, hResInstance, false);
	m_btOpenCard.SetButtonImage(IDB_BT_OPEN_CARD, hResInstance, false);

	m_btBank.SetButtonImage(IDB_BT_BANK, hResInstance, false);
	m_btContinueCard.SetButtonImage(IDB_BT_CONTINUE_CARD, hResInstance, false);

	m_btBankerStorage.SetButtonImage(IDB_BT_STORAGE, hResInstance, false);
	m_btBankerDraw.SetButtonImage(IDB_BT_DRAW, hResInstance, false);
	m_btWinRate.SetButtonImage(IDB_BT_WIN_RATE, hResInstance, false);

	//tagApplyUser ApplyUser;
	//ApplyUser.strUserName="fdsfds";
	//m_ApplyUser.InserUser(ApplyUser);
	//ApplyUser.strUserName="fdsfds1";
	//m_ApplyUser.InserUser(ApplyUser);
	//ApplyUser.strUserName="fdsfds2";
	//m_ApplyUser.InserUser(ApplyUser);
	//ApplyUser.strUserName="fdsfds3";
	//m_ApplyUser.InserUser(ApplyUser);
	//ApplyUser.strUserName="fdsfds4";
	//m_ApplyUser.InserUser(ApplyUser);
	//ApplyUser.strUserName="fdsfds5";
	//m_ApplyUser.InserUser(ApplyUser);

	//扑克控件
	for (int i = 0; i < CountArray(m_CardControl); ++i)
		m_CardControl[i].SetDisplayFlag(false);

	return 0;
}

//重置界面
void CGameClientView::ResetGameView()
{
	//下注信息
	ZeroMemory(m_lUserJettonScore, sizeof(m_lUserJettonScore));

	//游戏记录（麻将）
	ZeroMemory(m_CardHistoryArray, sizeof(m_CardHistoryArray));

	//全体下注
	ZeroMemory(m_lAllJettonScore, sizeof(m_lAllJettonScore));

	//庄家信息
	m_wBankerUser = INVALID_CHAIR;
	m_wBankerTime = 0;
	m_lBankerScore = 0L;
	m_lBankerWinScore = 0L;
	m_lTmpBankerWinScore = 0;

	//当局成绩
	m_lMeCurGameScore = 0L;
	m_lMeCurGameReturnScore = 0L;
	m_lBankerCurGameScore = 0L;
	m_lGameRevenue = 0L;

	//状态信息
	m_lCurrentJetton = 0L;
	m_cbAreaFlash = 0xFF;
	m_wMeChairID = INVALID_CHAIR;
	m_bShowChangeBanker = false;
	m_bNeedSetGameRecord = false;
	m_bWinTianMen = false;
	m_bWinXuanMen = false;
	m_bFlashResult = false;
	m_bShowGameResult = false;
	m_enDispatchCardTip = enDispatchCardTip_NULL;

	m_lMeCurGameScore = 0L;
	m_lMeCurGameReturnScore = 0L;
	m_lBankerCurGameScore = 0L;

	m_lAreaLimitScore = 0L;

	//位置信息
	m_nScoreHead = 0;
	m_nRecordFirst = 0;
	m_nRecordLast = 0;

	//历史成绩
	m_lMeStatisticScore = 0;

	//清空列表
	m_ApplyUser.ClearAll();

	//清除桌面
	CleanUserJetton();

	//设置按钮
	m_btApplyBanker.ShowWindow(SW_SHOW);
	m_btApplyBanker.EnableWindow(FALSE);
	m_btCancelBanker.ShowWindow(SW_HIDE);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY, AfxGetInstanceHandle(), false);

	return;
}

//调整控件
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//位置信息
	m_nWinFlagsExcursionX = nWidth / 2 - 260 - 41;
	m_nWinFlagsExcursionY = nHeight / 2 + 246 - 21;

	CSize Size;
	m_CardControl[0].GetControlSize(Size, 2);
	Size.cy = Size.cy / 2;

	CImageHandle ImageHandle(&m_ImageViewBack);
	int iWidth = m_ImageViewBack.GetWidth();

	int LifeWidth = nWidth / 2 - iWidth / 2;
	int TopHeight = nHeight / 2 - m_ImageViewBack.GetHeight() / 2;


	//区域位置
	int nCenterX = nWidth / 2, nCenterY = nHeight / 2, nBorderWidth = 4;

	m_Sice[0].SetPointBase(LifeWidth + 110, TopHeight + 200);
	m_Sice[0].SetSize(450, 100);
	m_Sice[1].SetPointBase(LifeWidth + 110, TopHeight + 200);
	m_Sice[1].SetSize(450, 100);

	m_rcTianMen.x = nWidth / 2 - 330;
	m_rcTianMen.y = nHeight / 2 + 56;

	m_rcDimen.x = nWidth / 2 - 156;
	m_rcDimen.y = nHeight / 2 + 96;

	m_rcXuanMen.x = nWidth / 2 + 46;
	m_rcXuanMen.y = nHeight / 2 + 56;

//////////////////////////////////////////////////////////////////////////
	CPoint ptPolygon[5];
	ptPolygon[0].x = nWidth / 2 - 328;
	ptPolygon[0].y = nHeight / 2 + 58;

	ptPolygon[1].x = nWidth / 2 - 132;
	ptPolygon[1].y = nHeight / 2 + 58;

	ptPolygon[2].x = nWidth / 2 - 62;
	ptPolygon[2].y = nHeight / 2 + 101;

	ptPolygon[3].x = nWidth / 2 - 153;
	ptPolygon[3].y = nHeight / 2 + 217;

	ptPolygon[4].x = nWidth / 2 - 328;
	ptPolygon[4].y = nHeight / 2 + 58;
	m_rgnTianMen.CreatePolygonRgn(ptPolygon, 5, ALTERNATE);

//////////////////////////////////////////////////////////////////////////
	ptPolygon[0].x = nWidth / 2 - 64;
	ptPolygon[0].y = nHeight / 2 + 101;

	ptPolygon[1].x = nWidth / 2 + 64;
	ptPolygon[1].y = nHeight / 2 + 101;

	ptPolygon[2].x = nWidth / 2 + 141;
	ptPolygon[2].y = nHeight / 2 + 216;

	ptPolygon[3].x = nWidth / 2 - 141;
	ptPolygon[3].y = nHeight / 2 + 216;

	ptPolygon[4].x = nWidth / 2 - 64;
	ptPolygon[4].y = nHeight / 2 + 101;
	m_rgnDimen.CreatePolygonRgn(ptPolygon, 5, ALTERNATE);

//////////////////////////////////////////////////////////////////////////
	ptPolygon[0].x = nWidth / 2 + 328;
	ptPolygon[0].y = nHeight / 2 + 58;

	ptPolygon[1].x = nWidth / 2 + 132;
	ptPolygon[1].y = nHeight / 2 + 58;

	ptPolygon[2].x = nWidth / 2 + 62;
	ptPolygon[2].y = nHeight / 2 + 101;

	ptPolygon[3].x = nWidth / 2 + 153;
	ptPolygon[3].y = nHeight / 2 + 217;

	ptPolygon[4].x = nWidth / 2 + 328;
	ptPolygon[4].y = nHeight / 2 + 58;
	m_rgnXuanmen.CreatePolygonRgn(ptPolygon, 5, ALTERNATE);
//////////////////////////////////////////////////////////////////////////

// 	m_rcHuangMen.left = LifeWidth + 117 + 25 + 175 + 175 + 173;
// 	m_rcHuangMen.top = TopHeight + 254 + 7;
// 	m_rcHuangMen.right = m_rcHuangMen.left + 167;
// 	m_rcHuangMen.bottom = m_rcHuangMen.top + 138;

	//筹码数字
	int ExcursionY = 10;
	m_PointJettonNumber[ID_TIAN_MEN-1].SetPoint(nWidth / 2 - 195, nHeight / 2 + 119 + ExcursionY);
	m_PointJettonNumber[ID_DI_MEN -1].SetPoint(nWidth / 2, nHeight / 2 + 165 + ExcursionY);
//	m_PointJettonNumber[ID_HUANG_MEN-1].SetPoint((m_rcHuangMen.right + m_rcHuangMen.left) / 2, (m_rcHuangMen.bottom + m_rcHuangMen.top) / 2 - ExcursionY);
	m_PointJettonNumber[ID_XUAN_MEN-1].SetPoint(nWidth / 2 + 195, nHeight / 2 + 119 + ExcursionY);

	//筹码位置
	m_PointJetton[ID_TIAN_MEN-1].SetPoint(m_rcTianMen.x + 50, m_rcTianMen.y);
	m_PointJetton[ID_DI_MEN-1].SetPoint(m_rcDimen.x + 65, m_rcDimen.y + 15);
//	m_PointJetton[ID_HUANG_MEN -1].SetPoint(m_rcHuangMen.left, m_rcHuangMen.top);
	m_PointJetton[ID_XUAN_MEN-1].SetPoint(m_rcXuanMen.x + 50, m_rcXuanMen.y);


	//扑克控件
	m_CardControl[0].SetBenchmarkPos(CPoint(nWidth / 2 - m_CardControl[0].GetCardSize().cx / 2 - 19, nHeight / 2 - 160), enXCenter, enYTop);
	m_CardControl[1].SetBenchmarkPos(CPoint(nWidth / 2 - 300, nHeight / 2 - 20), enXCenter, enYTop);
	m_CardControl[2].SetBenchmarkPos(CPoint(nWidth / 2 - m_CardControl[0].GetCardSize().cx / 2 - 19, nHeight / 2 - 20), enXCenter, enYTop);
	m_CardControl[3].SetBenchmarkPos(CPoint(nWidth / 2 + 300 - 80, nHeight / 2 - 20), enXCenter, enYTop);


	for (int i = 0; i < CARD_LOCATION_COUNT; i++)
	{
		m_CardControl[i].SetCardSpace(17, 17, 25);
	}

	//移动控件
	HDWP hDwp = BeginDeferWindowPos(33);
	const UINT uFlags = SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOCOPYBITS;

	m_ApplyUser.m_viewHandle = m_hWnd;


	//列表控件, int nHeight
	if (m_bShowRight)
	{
		if (m_ApplyUser.GetItemCount() > 7)
		{
			m_btUp.ShowWindow(SW_SHOW);
			m_btDown.ShowWindow(SW_SHOW);
		}
		else
		{
			m_btUp.ShowWindow(SW_HIDE);
			m_btDown.ShowWindow(SW_HIDE);
		}

		DeferWindowPos(hDwp, m_ApplyUser, NULL, nWidth - LifeWidth - 215, TopHeight + 17, 206, 126, uFlags);
		DeferWindowPos(hDwp, m_btUp, NULL, nWidth - LifeWidth - 24, TopHeight + 18, 75 / 5, 16, uFlags);
		DeferWindowPos(hDwp, m_btDown, NULL, nWidth - LifeWidth - 24, TopHeight + 126, 75 / 5, 16, uFlags);
	}
	else
	{
		m_btUp.ShowWindow(SW_HIDE);
		m_btDown.ShowWindow(SW_HIDE);

		DeferWindowPos(hDwp, m_ApplyUser, NULL, nWidth - LifeWidth - 215 + 1, TopHeight + 17, 206, 17, uFlags);
		//DeferWindowPos(hDwp, m_btUp, NULL, iWidth - 221 + 206 - 20, TopHeight + 20, 75 / 5, 16, uFlags);
		//DeferWindowPos(hDwp, m_btDown, NULL, iWidth - 221 + 206 - 20, TopHeight + 20 + 88 - 20, 75 / 5, 16, uFlags);
	}

	/*if(1)
	{
		m_btUp.ShowWindow(SW_SHOW);
		m_btUp.EnableWindow(true);
		m_btDown.ShowWindow(SW_SHOW);
		m_btDown.EnableWindow(true);
	}*/

	m_MeInfoRect.top = TopHeight + 10 + 12;
	m_MeInfoRect.left = LifeWidth + 245 + 20 + 389;

	//筹码按钮
	CRect rcJetton;
	m_btJetton1000.GetWindowRect(&rcJetton);

	int nSpace = 4;

//	DeferWindowPos(hDwp, m_btJetton100, NULL, nWidth / 2 - (nSpace * 4 + rcJetton.Width() * 4), nHeight / 2 + 230, 0, 0, uFlags | SWP_NOSIZE);
	DeferWindowPos(hDwp, m_btJetton1000, NULL, nWidth / 2 - (nSpace * 3 + rcJetton.Width() * 3), nHeight / 2 + 240, 0, 0, uFlags | SWP_NOSIZE);
	DeferWindowPos(hDwp, m_btJetton10000, NULL, nWidth / 2 - (nSpace * 2 + rcJetton.Width() * 2), nHeight / 2 + 250, 0, 0, uFlags | SWP_NOSIZE);
	DeferWindowPos(hDwp, m_btJetton50000, NULL, nWidth / 2 - (nSpace * 1 + rcJetton.Width() * 1), nHeight / 2 + 260, 0, 0, uFlags | SWP_NOSIZE);

	DeferWindowPos(hDwp, m_btJetton100000, NULL, nWidth / 2 + (nSpace * 0 + rcJetton.Width() * 0), nHeight / 2 + 260, 0, 0, uFlags | SWP_NOSIZE);
	DeferWindowPos(hDwp, m_btJetton500000, NULL, nWidth / 2 + (nSpace * 1 + rcJetton.Width() * 1), nHeight / 2 + 250, 0, 0, uFlags | SWP_NOSIZE);
	DeferWindowPos(hDwp, m_btJetton1000000, NULL, nWidth / 2 + (nSpace * 2 + rcJetton.Width() * 2), nHeight / 2 + 240, 0, 0, uFlags | SWP_NOSIZE);
//	DeferWindowPos(hDwp, m_btJetton5000000, NULL, nWidth / 2 + (nSpace * 3 + rcJetton.Width() * 3), nHeight / 2 + 230, 0, 0, uFlags | SWP_NOSIZE);

	DeferWindowPos(hDwp, m_btWinRate, NULL, 0, 0, 0, 0, uFlags | SWP_NOSIZE);


	//上庄按钮
	DeferWindowPos(hDwp, m_btApplyBanker, NULL, nWidth / 2 + 270, nHeight / 2 + 200, 0, 0, uFlags | SWP_NOSIZE);
	DeferWindowPos(hDwp, m_btCancelBanker, NULL, nWidth / 2 + 270, nHeight / 2 + 200, 0, 0, uFlags | SWP_NOSIZE);
	//银行按钮
	DeferWindowPos(hDwp, m_btBank, NULL, nWidth / 2 + 273, nHeight / 2 + 287, 0, 0, uFlags | SWP_NOSIZE);

	//继续发牌
	DeferWindowPos(hDwp, m_btContinueCard, NULL, LifeWidth + 624 + 20, TopHeight + 198 - 30, 0, 0, uFlags | SWP_NOSIZE);

	DeferWindowPos(hDwp, m_btScoreMoveL, NULL, LifeWidth + 58 + 23 + 4, TopHeight + 594 + 9, 0, 0, uFlags | SWP_NOSIZE);
	DeferWindowPos(hDwp, m_btScoreMoveR, NULL, LifeWidth + 58 + 28 + 403 + 2, TopHeight + 594 + 9, 0, 0, uFlags | SWP_NOSIZE);

	//开牌按钮
	DeferWindowPos(hDwp, m_btAutoOpenCard, NULL, LifeWidth + 624 + 20, TopHeight + 198, 0, 0, uFlags | SWP_NOSIZE);
	DeferWindowPos(hDwp, m_btOpenCard, NULL, LifeWidth + 624 + 20, TopHeight + 198 + 30, 0, 0, uFlags | SWP_NOSIZE);


	m_btBank.ShowWindow(SW_SHOW);
//	m_btContinueCard.ShowWindow(1);

	//其他按钮
	DeferWindowPos(hDwp, m_btBankerStorage, NULL, nWidth / 2 - 355 + 60 + 5, TopHeight + 143, 0, 0, uFlags | SWP_NOSIZE);
	DeferWindowPos(hDwp, m_btBankerDraw, NULL, nWidth / 2 - 415 + 50 + 5, TopHeight + 143, 0, 0, uFlags | SWP_NOSIZE);

	m_btBankerStorage.ShowWindow(SW_HIDE);
	m_btBankerDraw.ShowWindow(SW_HIDE);
	//结束移动
	EndDeferWindowPos(hDwp);


	return;
}

//绘画界面
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//绘画背景
	DrawViewImage(pDC, m_ImageViewFill, enMode_Spread);
	DrawViewImage(pDC, m_ImageViewBack, enMode_Centent);
	CImageHandle ImageCodeBackHandle(&m_ImageCodeBack);
	m_ImageCodeBack.AlphaDrawImage(pDC, nWidth / 2 + 406 - m_ImageCodeBack.GetWidth(), nHeight / 2 - 352,
	                               m_ImageCodeBack.GetWidth(), m_ImageCodeBack.GetHeight(), 0, 0, RGB(255, 0, 255));

	//获取状态
	BYTE cbGameStatus = m_pGameClientDlg->GetGameStatus();

	//状态提示
	CFont static InfoFont;
	/*InfoFont.CreateFont(-16,0,0,0,400,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("宋体"));
	CFont * pOldFont=pDC->SelectObject(&InfoFont);
	pDC->SetTextColor(RGB(255,234,0));

	pDC->SelectObject(pOldFont);
	InfoFont.DeleteObject();*/

	//时间提示
	CImageHandle ImageHandleTimeFlag(&m_ImageTimeFlag);
	int nTimeFlagWidth = m_ImageTimeFlag.GetWidth() / 4;
	int nFlagIndex = 0;
	if (cbGameStatus == GS_FREE) nFlagIndex = 0;
	else if (cbGameStatus == GS_PLACE_JETTON) nFlagIndex = 1;
	else if (cbGameStatus == GS_GAME_END) nFlagIndex = 2;
	m_ImageTimeFlag.AlphaDrawImage(pDC, nWidth / 2 - 395, nHeight / 2 + 270, nTimeFlagWidth, m_ImageTimeFlag.GetHeight(),
	                               nFlagIndex * nTimeFlagWidth, 0, RGB(15, 84, 87));

	//退出游戏
	if (cbGameStatus == GS_FREE)
	{
		m_ImageGameBegin.DrawImage(pDC, nWidth / 2 - m_ImageGameBegin.GetWidth() / 2, nHeight / 2 - 88, m_ImageGameBegin.GetWidth(),
		                           m_ImageGameBegin.GetHeight(), 0, 0, m_ImageGameBegin.GetWidth(), m_ImageGameBegin.GetHeight());
	}
	//当前可下注
	else if (cbGameStatus == GS_PLACE_JETTON)
	{
		CImageHandle ImageCurrentJoinCountHandle(&m_ImageCurrentJoinCount);
		m_ImageCurrentJoinCount.AlphaDrawImage(pDC, nWidth / 2 - 180, nHeight / 2 - 88,
		                                       m_ImageCurrentJoinCount.GetWidth(), m_ImageCurrentJoinCount.GetHeight(), 0, 0, RGB(15, 84, 87));

		LONGLONG llMeScore = 0;
		for (int i = 0; i < AREA_COUNT + 1; i++)
		{
			llMeScore += m_lAllJettonScore[i];
		}
		DrawNumberString(pDC, m_lBankerScore - llMeScore, nWidth / 2 - 180 + m_ImageCurrentJoinCount.GetWidth() + 75, nHeight / 2 - 70, false);
	}


	//Dlg里面的SetGameTimer(GetMeChairID()）里面调用的SwitchViewChairID在桌子人数大于8时实际是ViewID转化为0了
	WORD wUserTimer = GetUserTimer(0);
	if (wUserTimer != 0)
		DrawUserTimer(pDC, nWidth / 2 - 330, nHeight / 2 + 285 - 40, wUserTimer);

	//显示挂角
	CImageHandle ImageBackLeft01_Handle(&m_ImageBackLeft01);					//左挂角01
	CImageHandle ImageBackLeft02_Handle(&m_ImageBackLeft02);					//左挂角02
	CImageHandle ImageBackRight01_Handle(&m_ImageBackRight01);					//右挂角01
	CImageHandle ImageBackRight02_Handle(&m_ImageBackRight02);					//右挂

	if (m_bShowLeft)
	{
		m_ImageBackLeft01.AlphaDrawImage(pDC, nWidth / 2 - 407, nHeight / 2 - 352, m_ImageBackLeft01.GetWidth(), m_ImageBackLeft01.GetHeight(), 0, 0, RGB(255, 0, 255));
		CImageHandle ImageCardSmall_Handle(&m_ImageCardSmall);
		int nXArray[10] = {nWidth / 2 - 381 - 4, nWidth / 2 - 366 - 4, nWidth / 2 - 342 - 4, nWidth / 2 - 327 - 4, nWidth / 2 - 302 - 4, nWidth / 2 - 289 - 4, nWidth / 2 - 262 - 4, nWidth / 2 - 247 - 4, nWidth / 2 - 221 - 4, nWidth / 2 - 206 - 4};
		int nYArray[4] = {nHeight / 2 - 330 + 3, nHeight / 2 - 303 + 6, nHeight / 2 - 276 + 9, nHeight / 2 - 249 + 12};
		for (int i = 0; i < 5; i++)
		{
			if (m_CardHistoryArray[i][0])
			{
				for (int j = 0; j < 8 / 2; j++)
				{
					m_ImageCardSmall.AlphaDrawImage(pDC, nXArray[i * 2], nYArray[j], 15, 25, m_CardHistoryArray[i][j * 2] * 15, 0, RGB(255, 0, 255));
					m_ImageCardSmall.AlphaDrawImage(pDC, nXArray[i * 2 + 1], nYArray[j], 15, 25, m_CardHistoryArray[i][j * 2 + 1] * 15, 0, RGB(255, 0, 255));
				}
			}
			else
				break;
		}
	}
	else
		m_ImageBackLeft02.AlphaDrawImage(pDC, nWidth / 2 - 407, nHeight / 2 - 352, m_ImageBackLeft02.GetWidth(), m_ImageBackLeft02.GetHeight(), 0, 0, RGB(255, 0, 255));

	if (m_bShowRight)
		m_ImageBackRight01.AlphaDrawImage(pDC, nWidth / 2 + 407 - m_ImageBackRight01.GetWidth(), nHeight / 2 - 352, m_ImageBackRight01.GetWidth(), m_ImageBackRight01.GetHeight(), 0, 0, RGB(255, 0, 255));
	else
		m_ImageBackRight02.AlphaDrawImage(pDC, nWidth / 2 + 407 - m_ImageBackRight02.GetWidth(), nHeight / 2 - 352, m_ImageBackRight02.GetWidth(), m_ImageBackRight02.GetHeight(), 0, 0, RGB(255, 0, 255));

	//胜利边框
	FlashJettonAreaFrame(nWidth, nHeight, pDC);

	//筹码资源
	CImageHandle HandleJettonView(&m_ImageJettonView);
	CSize static SizeJettonItem(m_ImageJettonView.GetWidth() / 9, m_ImageJettonView.GetHeight());

	//绘画筹码
	for (INT i = 0; i < AREA_COUNT; i++)
	{
		//变量定义
		LONG lScoreCount = 0L;
		LONG static lScoreJetton[JETTON_COUNT] = {10000L, 100000L, 1000000L, 5000000L, 10000000L, 50000000L};
		int static nJettonViewIndex = 0;

		//绘画筹码
		for (INT_PTR j = 0; j < m_JettonInfoArray[i].GetCount(); j++)
		{
			//获取信息
			tagJettonInfo * pJettonInfo = &m_JettonInfoArray[i][j];

			//累计数字
			ASSERT(pJettonInfo->cbJettonIndex < JETTON_COUNT);
			lScoreCount += lScoreJetton[pJettonInfo->cbJettonIndex];

			//图片索引
			if (1 == pJettonInfo->cbJettonIndex || 2 == pJettonInfo->cbJettonIndex) nJettonViewIndex = pJettonInfo->cbJettonIndex + 1;
			else if (0 == pJettonInfo->cbJettonIndex) nJettonViewIndex = pJettonInfo->cbJettonIndex;
			else nJettonViewIndex = pJettonInfo->cbJettonIndex + 1;

			//绘画界面
			m_ImageJettonView.AlphaDrawImage(pDC, pJettonInfo->nXPos + m_PointJetton[i].x,
			                                 pJettonInfo->nYPos + m_PointJetton[i].y, SizeJettonItem.cx, SizeJettonItem.cy,
			                                 nJettonViewIndex*SizeJettonItem.cx, 0, RGB(255, 0, 255));
		}

		//绘画数字
		if (lScoreCount > 0L)
			DrawNumberString(pDC, lScoreCount, m_PointJettonNumber[i].x, m_PointJettonNumber[i].y);
	}

	//绘画庄家
	DrawBankerInfo(pDC, nWidth, nHeight);

	//绘画用户
	DrawMeInfo(pDC, nWidth, nHeight);

	//切换庄家
	if (m_bShowChangeBanker)
	{
		int	nXPos = nWidth / 2 - 130;
		int	nYPos = nHeight / 2 - 160;

		//由我做庄
		if (m_wMeChairID == m_wBankerUser)
		{
			CImageHandle ImageHandleBanker(&m_ImageMeBanker);
			m_ImageMeBanker.AlphaDrawImage(pDC, nXPos, nYPos, m_ImageMeBanker.GetWidth(), m_ImageMeBanker.GetHeight(), 0, 0, RGB(15, 84, 87));
		}
		else if (m_wBankerUser != INVALID_CHAIR)
		{
			CImageHandle ImageHandleBanker(&m_ImageChangeBanker);
			m_ImageChangeBanker.AlphaDrawImage(pDC, nXPos, nYPos, m_ImageChangeBanker.GetWidth(), m_ImageChangeBanker.GetHeight(), 0, 0, RGB(15, 84, 87));
		}
		else
		{
			CImageHandle ImageHandleBanker(&m_ImageNoBanker);
			m_ImageNoBanker.AlphaDrawImage(pDC, nXPos, nYPos, m_ImageNoBanker.GetWidth(), m_ImageNoBanker.GetHeight(), 0, 0, RGB(15, 84, 87));
		}
	}

// 	//发牌提示: 继续发牌
// 	if (enDispatchCardTip_NULL != m_enDispatchCardTip)
// 	{
// 		if (m_ImageDispatchCardTip.IsNull() == false) m_ImageDispatchCardTip.Destroy();
// 		if (enDispatchCardTip_Dispatch == m_enDispatchCardTip)
// 			m_ImageDispatchCardTip.SetLoadInfo(IDB_DISPATCH_CARD, AfxGetInstanceHandle(), false);
// 		else
// 			m_ImageDispatchCardTip.SetLoadInfo(IDB_CONTINUE_CARD, AfxGetInstanceHandle(), false);
//
// 		CImageHandle ImageHandle(&m_ImageDispatchCardTip);
// 		m_ImageDispatchCardTip.BitBlt(pDC->GetSafeHdc(), (nWidth - m_ImageDispatchCardTip.GetWidth()) / 2, nHeight / 2);
// 	}

	//我的下注
	DrawMeJettonNumber(pDC);


	//绘画扑克
	for (int i = 0; i < CountArray(m_CardControl); ++i)
		m_CardControl[i].DrawCardControl(pDC);

	//胜利标志 -- 历史列表
	//DrawWinFlags(pDC);


	if (m_blMoveFinish)
	{
		//显示结果
		DrawType(pDC, 0);
	}

// 	//结束状态
// 	if (cbGameStatus == GS_GAME_END)
// 	{
// 		//绘画扑克
// 		for (int i = 0; i < CountArray(m_CardControl); ++i)
// 			m_CardControl[i].DrawCardControl(pDC);
// 	}

	if (m_Sice[0].IsVisable())
		m_Sice[0].DrawCardControl(pDC);
	if (m_Sice[1].IsVisable())
		m_Sice[1].DrawCardControl(pDC);

	if (m_blMoveFinish)
	{
		ShowGameResult(pDC, nWidth, nHeight);
	}
	return;
}

//设置信息
void CGameClientView::SetMeMaxScore(LONGLONG lMeMaxScore)
{
	if (m_lMeMaxScore != lMeMaxScore)
	{
		//设置变量
		m_lMeMaxScore = lMeMaxScore;
	}

	return;
}

//最大下注
void CGameClientView::SetAreaLimitScore(LONGLONG lAreaLimitScore)
{
	if (m_lAreaLimitScore != lAreaLimitScore)
	{
		//设置变量
		m_lAreaLimitScore = lAreaLimitScore;
	}
}

//设置筹码
void CGameClientView::SetCurrentJetton(LONGLONG lCurrentJetton)
{
	//设置变量
	ASSERT(lCurrentJetton >= 0L);
	m_lCurrentJetton = lCurrentJetton;

	return;
}

//历史记录
void CGameClientView::SetGameHistory(bool bWinTian, bool bWinDi, bool bWinXuan)
{
	//设置数据
	tagClientGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.bWinTianMen = bWinTian;
	GameRecord.bWinDiMen = bWinDi;
	GameRecord.bWinXuanMen = bWinXuan;

	//操作类型
	if (0 == m_lUserJettonScore[ID_TIAN_MEN]) GameRecord.enOperateTianMen = enOperateResult_NULL;
	else if (m_lUserJettonScore[ID_TIAN_MEN] > 0 && true == bWinTian) GameRecord.enOperateTianMen = enOperateResult_Win;
	else if (m_lUserJettonScore[ID_TIAN_MEN] > 0 && false == bWinTian) GameRecord.enOperateTianMen = enOperateResult_Lost;

	if (0 == m_lUserJettonScore[ID_DI_MEN]) GameRecord.enOperateDiMen = enOperateResult_NULL;
	else if (m_lUserJettonScore[ID_DI_MEN] > 0 && true == bWinDi) GameRecord.enOperateDiMen = enOperateResult_Win;
	else if (m_lUserJettonScore[ID_DI_MEN] > 0 && false == bWinDi) GameRecord.enOperateDiMen = enOperateResult_Lost;

	if (0 == m_lUserJettonScore[ID_XUAN_MEN]) GameRecord.enOperateXuanMen = enOperateResult_NULL;
	else if (m_lUserJettonScore[ID_XUAN_MEN] > 0 && true == bWinXuan) GameRecord.enOperateXuanMen = enOperateResult_Win;
	else if (m_lUserJettonScore[ID_XUAN_MEN] > 0 && false == bWinXuan) GameRecord.enOperateXuanMen = enOperateResult_Lost;


// 	if (0 == m_lUserJettonScore[ID_HUANG_MEN]) GameRecord.enOperateHuangMen = enOperateResult_NULL;
// 	else if (m_lUserJettonScore[ID_HUANG_MEN] > 0 && true == bWinHuang) GameRecord.enOperateHuangMen = enOperateResult_Win;
// 	else if (m_lUserJettonScore[ID_HUANG_MEN] > 0 && false == bWinHuang) GameRecord.enOperateHuangMen = enOperateResult_Lost;

	//移动下标
	m_nRecordLast = (m_nRecordLast + 1) % MAX_SCORE_HISTORY;
	if (m_nRecordLast == m_nRecordFirst)
	{
		m_nRecordFirst = (m_nRecordFirst + 1) % MAX_SCORE_HISTORY;
		if (m_nScoreHead < m_nRecordFirst) m_nScoreHead = m_nRecordFirst;
	}

	int nHistoryCount = (m_nRecordLast - m_nRecordFirst + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
	if (MAX_FALG_COUNT < nHistoryCount) m_btScoreMoveR.EnableWindow(TRUE);

	//移到最新记录
	if (MAX_FALG_COUNT < nHistoryCount)
	{
		m_nScoreHead = (m_nRecordLast - MAX_FALG_COUNT + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
//		m_btScoreMoveL.EnableWindow(TRUE);
//		m_btScoreMoveR.EnableWindow(FALSE);
	}

	return;
}

//清理筹码
void CGameClientView::CleanUserJetton()
{
	//清理数组
	for (BYTE i = 0; i < CountArray(m_JettonInfoArray); i++)
	{
		m_JettonInfoArray[i].RemoveAll();
	}

	//下注信息
	ZeroMemory(m_lUserJettonScore, sizeof(m_lUserJettonScore));

	//全体下注
	ZeroMemory(m_lAllJettonScore, sizeof(m_lAllJettonScore));

	//更新界面
	UpdateGameView(NULL);

	return;
}

//个人下注
void CGameClientView::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	//效验参数
	ASSERT(cbViewIndex <= ID_XUAN_MEN);
	if (cbViewIndex > ID_XUAN_MEN) return;

	m_lUserJettonScore[cbViewIndex] = lJettonCount;

	//更新界面
	UpdateGameView(NULL);
}

//设置扑克
void CGameClientView::SetCardInfo(BYTE cbTableCardArray[CARD_LOCATION_COUNT][CARD_COUNT_OF_ONE])
{
	if (cbTableCardArray != NULL)
	{
		CopyMemory(m_cbTableCardArray, cbTableCardArray, sizeof(m_cbTableCardArray));

		//开始发牌
		DispatchCard();
	}
	else
	{
		ZeroMemory(m_cbTableCardArray, sizeof(m_cbTableCardArray));
	}
}

//设置筹码
void CGameClientView::PlaceUserJetton(BYTE cbViewIndex, LONGLONG lScoreCount)
{
	//效验参数
	ASSERT(cbViewIndex <= ID_XUAN_MEN);
	if (cbViewIndex > ID_XUAN_MEN) return;

	//变量定义
	bool bPlaceJetton = false;
	LONGLONG lScoreIndex[JETTON_COUNT] = {10000L, 100000L, 1000000L, 5000000L, 10000000L, 50000000L};

	//边框宽度
	int nFrameWidth = 0, nFrameHeight = 0;
	int nBorderWidth = 6;

	switch (cbViewIndex)
	{
		case ID_TIAN_MEN:
		{
			m_lAllJettonScore[ID_TIAN_MEN] += lScoreCount;
			nFrameWidth = 184;
			nFrameHeight = 143;
			break;
		}
		case ID_DI_MEN:
		{
			m_lAllJettonScore[ID_DI_MEN] += lScoreCount;
			nFrameWidth = 180;
			nFrameHeight = 118;
			break;
		}
// 		case ID_HUANG_MEN :
// 		{
// 			m_lAllJettonScore[ID_HUANG_MEN] += lScoreCount;
// 			nFrameWidth = m_rcHuangMen.right - m_rcHuangMen.left;
// 			nFrameHeight = m_rcHuangMen.bottom - m_rcHuangMen.top;
// 			break;
// 		}
		case ID_XUAN_MEN:
		{
			m_lAllJettonScore[ID_XUAN_MEN] += lScoreCount;
			nFrameWidth = 184;
			nFrameHeight = 143;
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

	//增加判断
	bool bAddJetton = lScoreCount > 0 ? true : false;
	lScoreCount = abs((int)lScoreCount);

	//增加筹码
	for (BYTE i = 0; i < CountArray(lScoreIndex); i++)
	{
		//计算数目
		BYTE cbScoreIndex = JETTON_COUNT - i - 1;
		LONGLONG lCellCount = lScoreCount / lScoreIndex[cbScoreIndex];

		//插入过虑
		if (lCellCount == 0L) continue;

		//加入筹码
		for (LONG j = 0; j < lCellCount; j++)
		{
			if (true == bAddJetton)
			{
				//构造变量
				tagJettonInfo JettonInfo;
				int nJettonSize = 68;
				JettonInfo.cbJettonIndex = cbScoreIndex;
				JettonInfo.nXPos = rand() % (nFrameWidth - nJettonSize);
				JettonInfo.nYPos = rand() % (nFrameHeight - nJettonSize);

				//插入数组
				bPlaceJetton = true;
				m_JettonInfoArray[cbViewIndex-1].Add(JettonInfo);
			}
			else
			{
				for (int nIndex = 0; nIndex < m_JettonInfoArray[cbViewIndex-1].GetCount(); ++nIndex)
				{
					//移除判断
					tagJettonInfo &JettonInfo = m_JettonInfoArray[cbViewIndex-1][nIndex];
					if (JettonInfo.cbJettonIndex == cbScoreIndex)
					{
						m_JettonInfoArray[cbViewIndex-1].RemoveAt(nIndex);
						break;
					}
				}
			}
		}

		//减少数目
		lScoreCount -= lCellCount * lScoreIndex[cbScoreIndex];
	}

	//更新界面
	if (bPlaceJetton == true) UpdateGameView(NULL);

	return;
}

//当局成绩
void CGameClientView::SetCurGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore, LONGLONG lGameRevenue)
{
	m_lMeCurGameScore = lMeCurGameScore;
	m_lMeCurGameReturnScore = lMeCurGameReturnScore;
	m_lBankerCurGameScore = lBankerCurGameScore;
	m_lGameRevenue = lGameRevenue;
}

//设置胜方
void CGameClientView::SetWinnerSide(bool bWinTian, bool bWinDi, bool bWinXuan, bool bSet)
{
	//设置时间
	if (true == bSet)
	{
		//设置定时器
		SetTimer(IDI_FLASH_WINNER, 500, NULL);

		//全胜判断
		if (false == bWinTian && false == bWinDi && false == bWinXuan)
		{
			//清除资源
			m_ImageFrameTianMen.Destroy();
			m_ImageFrameDiMen.Destroy();
			m_ImageFrameXuanMen.Destroy();
			//重设资源
			HINSTANCE hInstance = AfxGetInstanceHandle();

			//边框资源
			m_ImageFrameTianMen.SetLoadInfo(IDB_FRAME_TIAN_MEN_EX, hInstance);
			m_ImageFrameDiMen.SetLoadInfo(IDB_FRAME_DI_MEN_EX, hInstance);
			m_ImageFrameXuanMen.SetLoadInfo(IDB_FRAME_XUAN_MEN_EX, hInstance);
		}
	}
	else
	{
		//清楚定时器
		KillTimer(IDI_FLASH_WINNER);

		//全胜判断
		if (false == m_bWinXuanMen && false == m_bWinTianMen)
		{
			//清除资源
			m_ImageFrameTianMen.Destroy();
			m_ImageFrameDiMen.Destroy();
			m_ImageFrameXuanMen.Destroy();
			//重设资源
			HINSTANCE hInstance = AfxGetInstanceHandle();
			m_ImageFrameTianMen.SetLoadInfo(IDB_FRAME_TIAN_EMN, hInstance);
			m_ImageFrameDiMen.SetLoadInfo(IDB_FRAME_DI_EMN, hInstance);
			m_ImageFrameXuanMen.SetLoadInfo(IDB_FRAME_XUAN_EMN, hInstance);
		}
	}

	//设置变量
	m_bWinTianMen = bWinTian;
	m_bWinXuanMen = bWinXuan;
	m_bWinDiMen = bWinDi;
	m_bFlashResult = bSet;
	m_bShowGameResult = bSet;
	m_cbAreaFlash = 0xFF;

	//更新界面
	UpdateGameView(NULL);

	return;
}

//获取区域
BYTE CGameClientView::GetJettonArea(CPoint MousePoint)
{
	if (m_rgnTianMen.PtInRegion(MousePoint)) return ID_TIAN_MEN;
	if (m_rgnDimen.PtInRegion(MousePoint)) return ID_DI_MEN;
//	if (m_rcHuangMen.PtInRect(MousePoint)) return ID_XUAN_MEN;
	if (m_rgnXuanmen.PtInRegion(MousePoint)) return ID_XUAN_MEN ;
	return 0xFF;
}
void CGameClientView::DrawType(CDC* pDC, WORD wChairID)
{
	if (false == m_bShowGameResult) return;

	CImageHandle HandleImageCardType[5];
	for (int i = 0; i < 5; i++)
	{
		int iIndex = 0;
		//iIndex = CT_SPECIAL_NIUNIUXW-2;
		//m_lUserCardType[IDC_BANK]
		if (m_lUserCardType[i] != CT_POINT)
		{
			iIndex = m_lUserCardType[i] - 2;
			if (iIndex >= 13)
			{
				if (iIndex == 13)
				{
					iIndex = 12;
					//break;

				}
				if (iIndex == 14)
				{
					iIndex = 11;
					//break;

				}
				if (iIndex == 15)
				{
					iIndex = 13;
					//	break;

				}

			}
			else
			{
				if (iIndex > 10)
				{
					iIndex = 10;
				}
				if (iIndex < 0)
				{
					iIndex = 0;
				}

			}

			//iIndex = 11;

		}
	}
}

//绘画数字
void CGameClientView::DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos, bool bMeScore)
{
	//加载资源
	CImageHandle HandleScoreNumber(&m_ImageScoreNumber);
	CImageHandle HandleMeScoreNumber(&m_ImageMeScoreNumber);
	CSize SizeScoreNumber(m_ImageScoreNumber.GetWidth() / 10, m_ImageScoreNumber.GetHeight());

	if (bMeScore) SizeScoreNumber.SetSize(m_ImageMeScoreNumber.GetWidth() / 10, m_ImageMeScoreNumber.GetHeight());

	//计算数目
	LONGLONG lNumberCount = 0;
	LONGLONG lNumberTemp = lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp /= 10;
	}
	while (lNumberTemp > 0);

	//位置定义
	INT nYDrawPos = INT(nYPos - SizeScoreNumber.cy / 2);
	INT nXDrawPos = INT(nXPos + lNumberCount * SizeScoreNumber.cx / 2 - SizeScoreNumber.cx);

	//绘画桌号
	for (LONG i = 0; i < lNumberCount; i++)
	{
		//绘画号码
		LONGLONG lCellNumber = (LONGLONG)(lNumber % 10);
		if (bMeScore)
		{
			m_ImageMeScoreNumber.AlphaDrawImage(pDC, nXDrawPos, nYDrawPos, SizeScoreNumber.cx, SizeScoreNumber.cy,
			                                    (INT)lCellNumber*SizeScoreNumber.cx, 0, RGB(255, 0, 255));
		}
		else
		{
			m_ImageScoreNumber.AlphaDrawImage(pDC, nXDrawPos, nYDrawPos, SizeScoreNumber.cx, SizeScoreNumber.cy,
			                                  (INT)lCellNumber*SizeScoreNumber.cx, 0, RGB(255, 0, 255));
		}

		//设置变量
		lNumber /= 10;
		nXDrawPos -= SizeScoreNumber.cx;
	};

	return;
}

//绘画数字
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos)
{
	CString strNumber = TEXT(""), strTmpNumber1, strTmpNumber2;
	if (lNumber == 0) strNumber = TEXT("0");
	int nNumberCount = 0;
	LONGLONG lTmpNumber = lNumber;
	if (lNumber < 0) lNumber = -lNumber;
	while (lNumber > 0)
	{
		strTmpNumber1.Format(TEXT("%I64d"), lNumber % 10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1 + strTmpNumber2;

		if (nNumberCount == 4)
		{
			strTmpNumber2 += (TEXT(" ") + strNumber);
			strNumber = strTmpNumber2;
			nNumberCount = 0;
			strTmpNumber2 = TEXT("");
		}
		lNumber /= 10;
	}

	if (strTmpNumber2.IsEmpty() == FALSE)
	{
		strTmpNumber2 += (TEXT(" ") + strNumber);
		strNumber = strTmpNumber2;
	}

	if (lTmpNumber < 0) strNumber = TEXT("-") + strNumber;

	//输出数字
	pDC->TextOut(nXPos, nYPos, strNumber);
}

//绘画数字
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, CRect rcRect, INT nFormat)
{
	CString static strNumber = TEXT(""), strTmpNumber1, strTmpNumber2;
	strTmpNumber1.Empty();
	strTmpNumber2.Empty();
	strNumber.Empty();
	if (lNumber == 0) strNumber = TEXT("0");
	int nNumberCount = 0;
	LONGLONG lTmpNumber = lNumber;
	if (lNumber < 0) lNumber = -lNumber;
	while (lNumber > 0)
	{
		strTmpNumber1.Format(TEXT("%I64d"), lNumber % 10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1 + strTmpNumber2;

		if (nNumberCount == 4)
		{
			strTmpNumber2 += (TEXT(" ") + strNumber);
			strNumber = strTmpNumber2;
			nNumberCount = 0;
			strTmpNumber2 = TEXT("");
		}
		lNumber /= 10;
	}

	if (strTmpNumber2.IsEmpty() == FALSE)
	{
		strTmpNumber2 += (TEXT(" ") + strNumber);
		strNumber = strTmpNumber2;
	}

	if (lTmpNumber < 0) strNumber = TEXT("-") + strNumber;

	//输出数字
	if (nFormat == -1) pDC->DrawText(strNumber, rcRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP | DT_SINGLELINE);
	else pDC->DrawText(strNumber, rcRect, nFormat);
}

void CGameClientView::KillCardTime()
{
	KillTimer(IDI_SICE);
	KillTimer(IDI_FLASH_WINNER);
	KillTimer(IDI_POSTCARD);
	KillTimer(IDI_OPENCARD);
	KillTimer(IDI_DISPATCH_CARD);
	KillTimer(IDI_SHOWDISPATCH_CARD_TIP);
	for (int i = 0; i < CARD_LOCATION_COUNT; i++)
	{
		m_CardControl[i].m_blShowResult = false;
		m_CardControl[i].m_blShowLineResult = false;
	}
}

//定时器消息
void CGameClientView::OnTimer(UINT nIDEvent)
{
	//闪动胜方
	if (nIDEvent == IDI_FLASH_WINNER)
	{
		//设置变量
		m_bFlashResult = !m_bFlashResult;

		//更新界面
		UpdateGameView(NULL);

		return;
	}

	if (IDI_SICE == nIDEvent)
	{
		m_Sice[0].SetVisable(true);
		m_Sice[1].SetVisable(true);
		UpdateGameView(NULL);
		if (m_Sice[0].GetFrameIndex() == 0)
		{
			m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(), TEXT("DICE_GO"));
		}
		m_Sice[0].SetFrameIndex(m_Sice[0].GetFrameIndex() + 1);
		m_Sice[1].SetFrameIndex(m_Sice[1].GetFrameIndex() + 1);

		static int Timer = 0;
		Timer++;
		if (m_Sice[0].IsFinish())
		{
			if (Timer >= 10)
			{
				KillTimer(IDI_SICE);	//杀掉自己
				//不再显示骰子
				m_Sice[0].SetVisable(false);
				m_Sice[1].SetVisable(false);
				//开始发牌
				SetMoveCardTimer();
			}
		}
		else
		{
			Timer = 0;
		}
	}

	if (IDI_POSTCARD == nIDEvent)
	{
		bool blKillTimer = true;
		for (int i = 0; i < CARD_LOCATION_COUNT; i++)
		{
			if (m_CardControl[i].m_CardItemArray.GetCount() != m_CardControl[i].m_CardTempItemArray.GetCount())
			{
				blKillTimer = false;
				break;
			}
		}
		m_PostCardIndex = m_PostCardIndex % CARD_LOCATION_COUNT;
		m_CardControl[m_PostCardIndex].OnCopyCard();
		m_PostCardIndex++;
		if (blKillTimer)
		{
			KillTimer(IDI_POSTCARD);
			SetTimer(IDI_OPENCARD, 25, NULL);
		}
		UpdateGameView(NULL);

		m_bcShowCount++;
	}

	if (IDI_OPENCARD == nIDEvent)
	{
		if (m_CardControl[m_OpenCardIndex % CARD_LOCATION_COUNT].m_MovePoint.x < 40 && m_CardControl[m_OpenCardIndex % CARD_LOCATION_COUNT].m_blAdd)
		{
			m_CardControl[m_OpenCardIndex % CARD_LOCATION_COUNT].m_blGameEnd = false;
			m_CardControl[m_OpenCardIndex % CARD_LOCATION_COUNT].m_MovePoint.x += 2;
			m_CardControl[m_OpenCardIndex % CARD_LOCATION_COUNT].m_blhideOneCard = false;
			//更新界面
			UpdateGameView(NULL);
		}
		else
		{
			m_CardControl[m_OpenCardIndex % CARD_LOCATION_COUNT].m_blAdd = false;
			m_CardControl[m_OpenCardIndex % CARD_LOCATION_COUNT].m_blGameEnd = false;
			if (m_CardControl[m_OpenCardIndex % CARD_LOCATION_COUNT].m_MovePoint.x > 10)
				m_CardControl[m_OpenCardIndex % CARD_LOCATION_COUNT].m_MovePoint.x -= 2;
			m_CardControl[m_OpenCardIndex % CARD_LOCATION_COUNT].m_blhideOneCard = false;
			UpdateGameView(NULL);
			if (m_CardControl[m_OpenCardIndex % CARD_LOCATION_COUNT].m_MovePoint.x <= 10)
			{
				m_CardControl[m_OpenCardIndex % CARD_LOCATION_COUNT].m_tMoveCard.m_blMoveCard = true;
				m_CardControl[m_OpenCardIndex % CARD_LOCATION_COUNT].m_tMoveCard.m_blMoveFinish = true;
				m_CardControl[m_OpenCardIndex % CARD_LOCATION_COUNT].m_MovePoint.x = 0;
				m_CardControl[m_OpenCardIndex % CARD_LOCATION_COUNT].m_MovePoint.y = 0;
				m_CardControl[m_OpenCardIndex % CARD_LOCATION_COUNT].m_blAdd = true;
				m_CardControl[m_OpenCardIndex % CARD_LOCATION_COUNT].m_blGameEnd = true;


				m_CardControl[m_OpenCardIndex % CARD_LOCATION_COUNT].SetCardData(m_cbTableCardArray[m_OpenCardIndex % CARD_LOCATION_COUNT], CARD_COUNT_OF_ONE, false);
				m_CardControl[m_OpenCardIndex % CARD_LOCATION_COUNT].m_blShowResult = true;
				m_OpenCardIndex++;

				UpdateGameView(NULL);

				if (m_OpenCardIndex == CARD_LOCATION_COUNT)
				{
					KillTimer(IDI_OPENCARD);
					UpdateGameView(NULL);

					//设置显示牌定时器
					SetTimer(IDI_OPENCARD_2, 100, NULL);
				}
			}
		}
	}


	//开牌动画
	else if (nIDEvent == IDI_OPENCARD_2)
	{
		bool bFinish = true;
		for (int i = 0; i < CARD_LOCATION_COUNT; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				if (m_CardControl[i].m_nOpenIndex[j] < OPEN_CARD_MAX_INDEX)
				{
					m_CardControl[i].m_nOpenIndex[j]++;
					bFinish = false;
					break;
				}
			}
			if (!bFinish) break;
		}

		if (!bFinish)
		{
			UpdateGameView(NULL);
		}
		if (bFinish)
		{
			KillTimer(IDI_OPENCARD_2);
			UpdateGameView(NULL);

			//设置显示牌定时器
			SetTimer(IDI_DISPATCH_CARD, 30, NULL);
		}
	}

	//轮换庄家
	else if (nIDEvent == IDI_SHOW_CHANGE_BANKER)
	{
		ShowChangeBanker(false);

		return;
	}
	else if (nIDEvent == IDI_DISPATCH_CARD)
	{
		//完成发牌
		FinishDispatchCard();

		for (int i = 0; i < CARD_LOCATION_COUNT; i++)
		{
			m_CardControl[i].m_blGameEnd = true;
			m_CardControl[i].m_blhideOneCard = false;

// 			if (m_lUserCardType[i] == CT_POINT || m_lUserCardType[i] == CT_SPECIAL_BOMEBOME)
// 			{
// 				m_CardControl[i].m_blShowLineResult = true;
// 			}
// 			else
// 			{
			m_CardControl[i].m_blShowLineResult = false;

//			}
			m_CardControl[i].SetCardData(m_cbTableCardArray[i], CARD_COUNT_OF_ONE, false);
			m_CardControl[i].m_blShowResult = true;
		}
		KillTimer(IDI_OPENCARD);
		UpdateGameView(NULL);

		m_blMoveFinish = true;
		//更新界面
		UpdateGameView(NULL);

		return;
	}
	else if (IDI_SHOWDISPATCH_CARD_TIP == nIDEvent)
	{
		SetDispatchCardTip(enDispatchCardTip_NULL);
	}
	else if (nIDEvent == IDI_MOVECARD_END)
	{
		KillTimer(IDI_MOVECARD_END);
	}

	__super::OnTimer(nIDEvent);
}

//鼠标消息
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	//左挂角
	if (m_bShowLeft)
	{
		CRect r, r2;
		GetWindowRect(r);
		r2.left = r.Width() / 2 - 404;
		r2.top = r.Height() / 2 - 205;
		r2.right = r2.left + 22;
		r2.bottom = r2.top + 16;

		if (r2.PtInRect(Point))
		{
			m_bShowLeft = false;
			RectifyGameView(r.Width(), r.Height());
		}
	}
	else
	{
		CRect r, r2;
		GetWindowRect(r);
		r2.left = r.Width() / 2 - 404;
		r2.top = r.Height() / 2 - 345;
		r2.right = r2.left + 22;
		r2.bottom = r2.top + 16;

		if (r2.PtInRect(Point))
		{
			m_bShowLeft = true;
			RectifyGameView(r.Width(), r.Height());
		}
	}

	//右挂角
	if (m_bShowRight)
	{
		CRect r, r2;
		GetWindowRect(r);
		r2.left = r.Width() / 2 + 380;
		r2.top = r.Height() / 2 - 205;
		r2.right = r2.left + 22;
		r2.bottom = r2.top + 16;

		if (r2.PtInRect(Point))
		{
			m_bShowRight = false;
			RectifyGameView(r.Width(), r.Height());
		}
	}
	else
	{
		CRect r, r2;
		GetWindowRect(r);
		r2.left = r.Width() / 2 + 380;
		r2.top = r.Height() / 2 - 315;
		r2.right = r2.left + 22;
		r2.bottom = r2.top + 16;

		if (r2.PtInRect(Point))
		{
			m_bShowRight = true;
			RectifyGameView(r.Width(), r.Height());
		}
	}


	for (int i = 0; i < CARD_LOCATION_COUNT; i++)
	{
		if (!m_CardControl[i].m_blAuto)
		{
			m_CardControl[i].OnLButtonDown(nFlags, Point);
		}
	}

	if (m_lCurrentJetton != 0L)
	{
		int iTimer = 1;
		//下注区域
		BYTE cbJettonArea = GetJettonArea(Point);

		//最大下注
		LONGLONG lMaxJettonScore = GetUserMaxJetton();

		if ((m_lAllJettonScore[cbJettonArea] + m_lCurrentJetton) > m_lAreaLimitScore)
		{
			return ;
		}

		//合法判断
		if (lMaxJettonScore < m_lCurrentJetton) return ;

		//发送消息
		if (cbJettonArea != 0xFF) AfxGetMainWnd()->SendMessage(IDM_PLACE_JETTON, cbJettonArea, (LPARAM)m_lCurrentJetton);
	}
	UpdateGameView(NULL);

	__super::OnLButtonDown(nFlags, Point);
}
void CGameClientView::OnLButtonUp(UINT nFlags, CPoint Point)
{

	for (int i = 0; i < CARD_LOCATION_COUNT; i++)
	{
		if (!m_CardControl[i].m_blAuto)
		{
			m_CardControl[i].OnLButtonUp(nFlags, Point);
			UpdateGameView(NULL);
		}

	}
	__super::OnLButtonUp(nFlags, Point);
}
//接受其他控件传来的消息
LRESULT CGameClientView::OnViLBtUp(WPARAM wParam, LPARAM lParam)
{
	CPoint *pPoint = (CPoint*)lParam;
	ScreenToClient(pPoint);
	OnLButtonUp(1, *pPoint);
	return 1;

}
//鼠标移动消息
void CGameClientView::OnMouseMove(UINT nFlags, CPoint point)
{
	for (int i = 0; i < CARD_LOCATION_COUNT; i++)
	{
		if (!m_CardControl[i].m_blAuto)
		{
			m_CardControl[i].OnMouseMove(nFlags, point);
			UpdateGameView(NULL);

		}

	}
	__super::OnMouseMove(nFlags, point);
}
//鼠标消息
void CGameClientView::OnRButtonDown(UINT nFlags, CPoint Point)
{
	//设置变量
	m_lCurrentJetton = 0L;

	if (m_pGameClientDlg->GetGameStatus() != GS_GAME_END && m_cbAreaFlash != 0xFF)
	{
		m_cbAreaFlash = 0xFF;
		UpdateGameView(NULL);
	}

	__super::OnLButtonDown(nFlags, Point);
}

//光标消息
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	if (m_lCurrentJetton != 0L)
	{
		//获取区域
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);
		BYTE cbJettonArea = GetJettonArea(MousePoint);

		//设置变量
		if (m_cbAreaFlash != cbJettonArea)
		{
			m_cbAreaFlash = cbJettonArea;
			UpdateGameView(NULL);
		}

		//区域判断
		if (cbJettonArea == 0xFF)
		{
			SetCursor(LoadCursor(NULL, IDC_NO));
			return TRUE;
		}

		//最大下注
		LONGLONG lMaxJettonScore = GetUserMaxJetton();

		//合法判断
		int iTimer = 1;

		if ((m_lAllJettonScore[cbJettonArea] + m_lCurrentJetton) > m_lAreaLimitScore)
		{
			SetCursor(LoadCursor(NULL, IDC_NO));
			return TRUE;

		}
		if (lMaxJettonScore < m_lCurrentJetton*iTimer)
		{
			SetCursor(LoadCursor(NULL, IDC_NO));
			return TRUE;
		}

		//设置光标
		switch (m_lCurrentJetton)
		{
			case 100:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_SCORE_100)));
				return TRUE;
			}
			case 1000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_SCORE_1000)));
				return TRUE;
			}
			case 10000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_SCORE_10000)));
				return TRUE;
			}
			case 50000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_SCORE_50000)));
				return TRUE;
			}
			case 100000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_SCORE_100000)));
				return TRUE;
			}
			case 500000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_SCORE_500000)));
				return TRUE;
			}
			case 1000000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_SCORE_1000000)));
				return TRUE;
			}
			case 5000000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_SCORE_5000000)));
				return TRUE;
			}
		}
	}

	return __super::OnSetCursor(pWnd, nHitTest, uMessage);
}

//轮换庄家
void CGameClientView::ShowChangeBanker(bool bChangeBanker)
{
	//轮换庄家
	if (bChangeBanker)
	{
		SetTimer(IDI_SHOW_CHANGE_BANKER, 3000, NULL);
		m_bShowChangeBanker = true;
	}
	else
	{
		KillTimer(IDI_SHOW_CHANGE_BANKER);
		m_bShowChangeBanker = false ;
	}

	//更新界面
	UpdateGameView(NULL);
}

//上庄按钮
void CGameClientView::OnApplyBanker()
{
	AfxGetMainWnd()->SendMessage(IDM_APPLY_BANKER, 1, 0);

}

//下庄按钮
void CGameClientView::OnCancelBanker()
{
	AfxGetMainWnd()->SendMessage(IDM_APPLY_BANKER, 0, 0);
}

//艺术字体
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
	//变量定义
	int nStringLength = lstrlen(pszString);
	int nXExcursion[8] = {1, 1, 1, 0, -1, -1, -1, 0};
	int nYExcursion[8] = { -1, 0, 1, 1, 1, 0, -1, -1};

	//绘画边框
	pDC->SetTextColor(crFrame);
	for (int i = 0; i < CountArray(nXExcursion); i++)
	{
		pDC->TextOut(nXPos + nXExcursion[i], nYPos + nYExcursion[i], pszString, nStringLength);
	}

	//绘画字体
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos, nYPos, pszString, nStringLength);

	return;
}
//庄家信息
void CGameClientView::SetBankerInfo(DWORD dwBankerUserID, LONGLONG lBankerScore)
{
	//庄家椅子号
	WORD wBankerUser = INVALID_CHAIR;

	//查找椅子号
	if (0 != dwBankerUserID)
	{
		for (WORD wChairID = 0; wChairID < MAX_CHAIR; ++wChairID)
		{
			tagUserData const *pUserData = GetUserInfo(wChairID);
			if (NULL != pUserData && dwBankerUserID == pUserData->dwUserID)
			{
				wBankerUser = wChairID;
				break;
			}
		}
	}

	//切换判断
	if (m_wBankerUser != wBankerUser)
	{
		m_wBankerUser = wBankerUser;
		m_wBankerTime = 0L;
		m_lBankerWinScore = 0L;
		m_lTmpBankerWinScore = 0L;
	}
	m_lBankerScore = lBankerScore;
}

//绘画标识
void CGameClientView::DrawWinFlags(CDC * pDC)
{
	//非空判断
	//m_nRecordLast = 14;
	if (m_nRecordLast == m_nRecordFirst) return;

	//资源变量
	CImageHandle ImageHandleWinFlags(&m_ImageWinFlags);
	int nIndex = m_nScoreHead;
	COLORREF static clrOld ;

	clrOld = pDC->SetTextColor(RGB(255, 234, 0));
	int nDrawCount = 0;
	while (nIndex != m_nRecordLast && (m_nRecordLast != m_nRecordFirst) && nDrawCount < MAX_FALG_COUNT)
	{
		//胜利标识
		tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[nIndex];
		bool  static bWinMen[4];
		bWinMen[0] = ClientGameRecord.bWinTianMen;
		bWinMen[1] = ClientGameRecord.bWinDiMen ;
		bWinMen[2] = ClientGameRecord.bWinXuanMen;

		//操作结果
		enOperateResult static OperateResult[4];
		OperateResult[0] = ClientGameRecord.enOperateTianMen;
		OperateResult[1] = ClientGameRecord.enOperateDiMen;
		OperateResult[2] = ClientGameRecord.enOperateXuanMen;

		for (int i = 0; i < AREA_COUNT; ++i)
		{
			//位置变量
			int static nYPos = 0, nXPos = 0;
			nYPos = m_nWinFlagsExcursionY + i * 27;
			nXPos = m_nWinFlagsExcursionX + ((nIndex - m_nScoreHead + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY) * 27;

			//胜利标识
			int static nFlagsIndex = 0;
			if (false == bWinMen[i])
			{
				nFlagsIndex = 2;
			}
			else
			{
				nFlagsIndex = 0;
			}
			if (OperateResult[i] != enOperateResult_NULL)
			{
				nFlagsIndex += 1;

				if (nFlagsIndex >= 4)
				{
					nFlagsIndex = 3;
				}
			}
			//绘画标识
			m_ImageWinFlags.AlphaDrawImage(pDC, nXPos, nYPos, m_ImageWinFlags.GetWidth() / 4 ,
			                               m_ImageWinFlags.GetHeight(), m_ImageWinFlags.GetWidth() / 4 * nFlagsIndex, 0, RGB(255, 0, 255));
		}

		//移动下标
		nIndex = (nIndex + 1) % MAX_SCORE_HISTORY;
		nDrawCount++;
	}

	nIndex = 0;
	int   bWinCount[4] = {0};
	int iAllrecordCount = 0;
	while (nIndex != m_nRecordLast && (m_nRecordLast != m_nRecordFirst))
	{
		//胜利标识
		tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[nIndex];
		bool static bWinMen[4];
		bWinMen[0] = ClientGameRecord.bWinTianMen;
		bWinMen[1] = ClientGameRecord.bWinDiMen ;
		bWinMen[2] = ClientGameRecord.bWinXuanMen;

		for (int n = 0; n < 3; n++)
		{
			if (bWinMen[n] == true)
			{
				bWinCount[n]++;
			}
		}
		iAllrecordCount++;

		nIndex = (nIndex + 1);
	}


	//if (m_pGameClientDlg->GetGameStatus() == GS_FREE)
// 	{
// 		CFont InfoFont;
// 		InfoFont.CreateFont(-16, 0, 0, 0, 400, 0, 0, 0, 134, 3, 2, ANTIALIASED_QUALITY, 2, TEXT("宋体"));
// 		CFont * pOldFont = pDC->SelectObject(&InfoFont);
// 		pDC->SetTextColor(RGB(200, 200, 200));
//
// 		for (int n = 0; n < AREA_COUNT; n++)
// 		{
// 			int	nYPos = m_nWinFlagsExcursionY + 85;
// 			int nXPos = m_nWinFlagsExcursionX + 20 + 120 * n;
//
// 			float fCountRe = (float)bWinCount[n] / (float)iAllrecordCount;
// 			fCountRe *= 100;
// 			CString str;
// 			if (n == 0) str = "天:";
// 			else if (n == 1) str = "地:";
// 			else  str = "顺:";
//
// 			CString strS;
// 			strS.Format("%0.2f", fCountRe);
// 			strS += "%";
// 			str += strS;
//
// 			pDC->TextOut(nXPos, nYPos, str);
// 		}
//
// 		pDC->SelectObject(pOldFont);
// 		InfoFont.DeleteObject();
// 	}

	pDC->SetTextColor(clrOld);
}
//手工搓牌
afx_msg void CGameClientView::OnOpenCard()
{
	::AfxGetMainWnd()->SendMessage(IDM_OPEN_CARD, 0, 0);

}
//自动搓牌
afx_msg void CGameClientView::OnAutoOpenCard()
{
	::AfxGetMainWnd()->SendMessage(IDM_AUTO_OPEN_CARD, 0, 0);

}
//移动按钮
void CGameClientView::OnScoreMoveL()
{
	if (m_nRecordFirst == m_nScoreHead) return;

	m_nScoreHead = (m_nScoreHead - 1 + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
	if (m_nScoreHead == m_nRecordFirst) m_btScoreMoveL.EnableWindow(FALSE);

	m_btScoreMoveR.EnableWindow(TRUE);

	//更新界面
	UpdateGameView(NULL);
}

//移动按钮
void CGameClientView::OnScoreMoveR()
{
	int nHistoryCount = (m_nRecordLast - m_nScoreHead + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
	if (nHistoryCount == MAX_FALG_COUNT) return;

	m_nScoreHead = (m_nScoreHead + 1) % MAX_SCORE_HISTORY;
	if (nHistoryCount - 1 == MAX_FALG_COUNT) m_btScoreMoveR.EnableWindow(FALSE);

	m_btScoreMoveL.EnableWindow(TRUE);

	//更新界面
	UpdateGameView(NULL);
}

//显示结果
void CGameClientView::ShowGameResult(CDC *pDC, int nWidth, int nHeight)
{
	//显示判断
	//if (m_pGameClientDlg->GetGameStatus()!=GS_GAME_END) return;
	if (false == m_bShowGameResult) return;

	int	nXPos = nWidth / 2 - 129;
	int	nYPos = nHeight / 2 - 168;
	CImageHandle ImageHandleGameEnd(&m_ImageGameEnd);

	CRect rcAlpha(nXPos + 2, nYPos + 70, nXPos + 2 + m_ImageGameEnd.GetWidth(), nYPos + 70 + m_ImageGameEnd.GetHeight());
	DrawAlphaRect(pDC, &rcAlpha, RGB(74, 70, 73), 0.8f);
	m_ImageGameEnd.AlphaDrawImage(pDC, nXPos + 2, nYPos + 70, RGB(255, 0, 255));

	pDC->SetTextColor(RGB(255, 234, 0));
	CRect rcMeWinScore, rcMeReturnScore;
	rcMeWinScore.left = nXPos + 2 + 40;
	rcMeWinScore.top = nYPos + 70 + 32;
	rcMeWinScore.right = rcMeWinScore.left + 111;
	rcMeWinScore.bottom = rcMeWinScore.top + 34;

	rcMeReturnScore.left = nXPos + 2 + 150;
	rcMeReturnScore.top = nYPos + 70 + 32;
	rcMeReturnScore.right = rcMeReturnScore.left + 111;
	rcMeReturnScore.bottom = rcMeReturnScore.top + 34;

	CString strMeGameScore, strMeReturnScore;
	DrawNumberStringWithSpace(pDC, m_lMeCurGameScore, rcMeWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	DrawNumberStringWithSpace(pDC, m_lMeCurGameReturnScore, rcMeReturnScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	CRect rcBankerWinScore;
	rcBankerWinScore.left = nXPos + 2 + 40;
	rcBankerWinScore.top = nYPos + 70 + 69;
	rcBankerWinScore.right = rcBankerWinScore.left + 111;
	rcBankerWinScore.bottom = rcBankerWinScore.top + 34;

	CString strBankerCurGameScore;
	strBankerCurGameScore.Format(TEXT("%ld"), m_lBankerCurGameScore);
	DrawNumberStringWithSpace(pDC, m_lBankerCurGameScore, rcBankerWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
}

//透明绘画
bool CGameClientView::DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha)
{
	ASSERT(pDC != 0 && lpRect != 0 && clr != CLR_NONE);
	if (pDC == 0 || lpRect == 0 || clr == CLR_NONE)
	{
		return false;
	}
	//全透明
	if (abs(fAlpha) <= 0.000001)
	{
		return true;
	}

	for (LONG l = lpRect->top; l < lpRect->bottom; l++)
	{
		for (LONG k = lpRect->left; k < lpRect->right; k++)
		{
			COLORREF clrBk = pDC->GetPixel(k, l);
			COLORREF clrBlend = RGB(GetRValue(clrBk) * (1 - fAlpha) + GetRValue(clr) * fAlpha,
			                        GetGValue(clrBk) * (1 - fAlpha) + GetGValue(clr) * fAlpha,
			                        GetBValue(clrBk) * (1 - fAlpha) + GetBValue(clr) * fAlpha);
			pDC->SetPixel(k, l, clrBlend);
		}
	}

	return true;
}

//最大下注
LONGLONG CGameClientView::GetUserMaxJetton()
{
	int iTimer = 1;
	//已下注额
	LONGLONG lNowJetton = 0;
	ASSERT(AREA_COUNT <= CountArray(m_lUserJettonScore));
	for (int nAreaIndex = 1; nAreaIndex <= AREA_COUNT; ++nAreaIndex) lNowJetton += m_lUserJettonScore[nAreaIndex];

	//庄家金币
	LONGLONG lBankerScore = 2147483647;
	//	LONGLONG lBankerScore=350000;

	if (m_wBankerUser != INVALID_CHAIR) lBankerScore = m_lBankerScore;
	LONGLONG allJetton = 0;
	for (int nAreaIndex = 1; nAreaIndex <= AREA_COUNT; ++nAreaIndex)
	{
		lBankerScore -= m_lAllJettonScore[nAreaIndex] * iTimer;
		allJetton += m_lAllJettonScore[nAreaIndex] * iTimer;
	}

	//区域限制
	LONGLONG lMeMaxScore = m_lMeMaxScore;

	lMeMaxScore = lMeMaxScore - lNowJetton * iTimer;

	lMeMaxScore = min(lMeMaxScore, m_lAreaLimitScore * iTimer);


	//庄家限制
	lMeMaxScore = min(lMeMaxScore / iTimer, (lBankerScore) / iTimer);

//	lMeMaxScore /= iTimer;

	//非零限制
	//ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return lMeMaxScore;
}

//成绩设置
void CGameClientView::SetGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore)
{
	m_lMeCurGameScore = lMeCurGameScore;
	m_lMeCurGameReturnScore = lMeCurGameReturnScore;
	m_lBankerCurGameScore = lBankerCurGameScore;
}

//绘画数字
void CGameClientView::DrawMeJettonNumber(CDC *pDC)
{
	//绘画数字
	for (int nAreaIndex = 1; nAreaIndex <= AREA_COUNT; ++nAreaIndex)
	{
		if (m_lUserJettonScore[nAreaIndex] > 0)
			DrawNumberString(pDC, m_lUserJettonScore[nAreaIndex], m_PointJettonNumber[nAreaIndex-1].x, m_PointJettonNumber[nAreaIndex-1].y + 25, true);
	}
}

//开始发牌
void CGameClientView::DispatchCard()
{
	//设置界面
	for (int i = 0; i < CountArray(m_CardControl); ++i)
		m_CardControl[i].m_CardItemArray.SetSize(0);

	//设置标识
	m_bNeedSetGameRecord = true;
}

//结束发牌
void CGameClientView::FinishDispatchCard()
{
	//完成判断
	if (m_bNeedSetGameRecord == false) return;

	//设置标识
	m_bNeedSetGameRecord = false;

	//删除定时器
	KillTimer(IDI_DISPATCH_CARD);

	//推断赢家
	bool bWinTianMen, bWinDiMen, bWinXuanMen;

	BYTE TianMultiple, diMultiple, TianXuanltiple;

	DeduceWinner(bWinTianMen, bWinDiMen, bWinXuanMen, TianMultiple, diMultiple, TianXuanltiple);

	//保存记录
	SetGameHistory(bWinTianMen, bWinDiMen, bWinXuanMen);

	//累计积分
	m_lMeStatisticScore += m_lMeCurGameScore;
	m_lBankerWinScore = m_lTmpBankerWinScore;

	//设置赢家
	SetWinnerSide(bWinTianMen, bWinDiMen, bWinXuanMen, true);

	//播放声音
	if (m_lMeCurGameScore > 0) m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(), TEXT("END_WIN"));
	else if (m_lMeCurGameScore < 0) m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(), TEXT("END_LOST"));
	else m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(), TEXT("END_DRAW"));


	BYTE bCode[8] = {0};
	bCode[0] = m_cbTableCardArray[0][0];
	bCode[1] = m_cbTableCardArray[0][1];
	bCode[2] = m_cbTableCardArray[1][0];
	bCode[3] = m_cbTableCardArray[1][1];
	bCode[4] = m_cbTableCardArray[2][0];
	bCode[5] = m_cbTableCardArray[2][1];
	bCode[6] = m_cbTableCardArray[3][0];
	bCode[7] = m_cbTableCardArray[3][1];

	SetCodeHistory(bCode);
}

//胜利边框
void CGameClientView::FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC)
{
	//合法判断

	if (m_cbAreaFlash == 0xFF && false == m_bFlashResult) return;

	//下注判断
	if (false == m_bFlashResult)
	{
		if (m_pGameClientDlg->GetGameStatus() == GS_PLACE_JETTON)
		{
			switch (m_cbAreaFlash)
			{
				case ID_TIAN_MEN:
				{
					//绘画图片
					CImageHandle ImageHandle(&m_ImageFrameTianMen);
					m_ImageFrameTianMen.AlphaDrawImage(pDC, m_rcTianMen.x, m_rcTianMen.y, RGB(255, 0, 255));
					break;
				}
				case ID_DI_MEN:
				{
					//绘画图片
					CImageHandle ImageHandle(&m_ImageFrameDiMen);
					m_ImageFrameDiMen.AlphaDrawImage(pDC, m_rcDimen.x, m_rcDimen.y, RGB(255, 0, 255));
					break;
				}
				case ID_XUAN_MEN:
				{
					//绘画图片
					CImageHandle ImageHandle(&m_ImageFrameXuanMen);
					m_ImageFrameXuanMen.AlphaDrawImage(pDC, m_rcXuanMen.x, m_rcXuanMen.y, RGB(255, 0, 255));
					break;
				}
// 				case ID_HUANG_MEN:
// 				{
// 					//绘画图片
// 					CImageHandle ImageHandle(&m_ImageFrameHuangMen);
// 					m_ImageFrameHuangMen.AlphaDrawImage(pDC, m_rcHuangMen.left, m_rcHuangMen.top, RGB(255, 0, 255));
// 					break;
// 				}
			}
		}
	}
	else
	{
		//门判断
		if (true == m_bWinXuanMen)
		{
			CImageHandle ImageHandleXuanMen(&m_ImageFrameXuanMen);
			m_ImageFrameXuanMen.AlphaDrawImage(pDC, m_rcXuanMen.x, m_rcXuanMen.y, RGB(255, 0, 255));
		}
		if (true == m_bWinTianMen)
		{
			CImageHandle ImageHandleTianMen(&m_ImageFrameTianMen);
			m_ImageFrameTianMen.AlphaDrawImage(pDC, m_rcTianMen.x, m_rcTianMen.y, RGB(255, 0, 255));
		}
		if (true == m_bWinDiMen)
		{
			CImageHandle ImageHandleDiMen(&m_ImageFrameDiMen);
			m_ImageFrameDiMen.AlphaDrawImage(pDC, m_rcDimen.x, m_rcDimen.y, RGB(255, 0, 255));
		}

		//庄全胜判断
		if (false == m_bWinXuanMen && false == m_bWinTianMen && m_bWinDiMen == false)
		{
			//加载资源
			CImageHandle ImageHandleTianMen(&m_ImageFrameTianMen);
			CImageHandle ImageHandleDimen(&m_ImageFrameDiMen);
			CImageHandle ImageHandleXuanMen(&m_ImageFrameXuanMen);


			//绘画图片
			m_ImageFrameTianMen.AlphaDrawImage(pDC, m_rcTianMen.x, m_rcTianMen.y, RGB(255, 0, 255));
			m_ImageFrameDiMen.AlphaDrawImage(pDC, m_rcDimen.x, m_rcDimen.y, RGB(255, 0, 255));
			m_ImageFrameXuanMen.AlphaDrawImage(pDC, m_rcXuanMen.x, m_rcXuanMen.y, RGB(255, 0, 255));

			m_ImageTS.DrawImage(pDC, nWidth / 2 - m_ImageTS.GetWidth() / 2, nHeight / 2 - m_ImageTS.GetHeight() / 2, m_ImageTS.GetWidth(), m_ImageTS.GetHeight(), 0, 0, m_ImageTS.GetWidth(), m_ImageTS.GetHeight());
		}
		else if (true == m_bWinXuanMen && true == m_bWinTianMen && m_bWinDiMen == true)
		{
			//门判断
			if (true == m_bWinXuanMen)
			{
				CImageHandle ImageHandleXuanMen(&m_ImageFrameXuanMen);
				m_ImageFrameXuanMen.AlphaDrawImage(pDC, m_rcXuanMen.x, m_rcXuanMen.y, RGB(255, 0, 255));
			}
			if (true == m_bWinTianMen)
			{
				CImageHandle ImageHandleTianMen(&m_ImageFrameTianMen);
				m_ImageFrameTianMen.AlphaDrawImage(pDC, m_rcTianMen.x, m_rcTianMen.y, RGB(255, 0, 255));
			}
			if (true == m_bWinDiMen)
			{
				CImageHandle ImageHandleDiMen(&m_ImageFrameDiMen);
				m_ImageFrameDiMen.AlphaDrawImage(pDC, m_rcDimen.x, m_rcDimen.y, RGB(255, 0, 255));
			}

			m_ImageTP.DrawImage(pDC, nWidth / 2 - m_ImageTS.GetWidth() / 2, nHeight / 2 - m_ImageTS.GetHeight() / 2, m_ImageTS.GetWidth(), m_ImageTS.GetHeight(), 0, 0, m_ImageTS.GetWidth(), m_ImageTS.GetHeight());
		}
	}
}

//推断赢家
void CGameClientView::DeduceWinner(bool &bWinTian, bool &bWinDi, bool &bWinXuan, BYTE &TianMultiple, BYTE &diMultiple, BYTE &TianXuanltiple)
{
	//大小比较
	bWinTian = (m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX], CARD_COUNT_OF_ONE, m_cbTableCardArray[SHUN_MEN_INDEX], CARD_COUNT_OF_ONE, TianMultiple) == 1) ? true : false;
	bWinDi = (m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX], CARD_COUNT_OF_ONE, m_cbTableCardArray[DUI_MEN_INDEX], CARD_COUNT_OF_ONE, diMultiple) == 1) ? true : false;
	bWinXuan = (m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX], CARD_COUNT_OF_ONE, m_cbTableCardArray[DAO_MEN_INDEX], CARD_COUNT_OF_ONE, TianXuanltiple) == 1) ? true : false;
}

//控件命令
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//获取ID
	WORD wControlID = LOWORD(wParam);

	//控件判断
	switch (wControlID)
	{
		case IDC_JETTON_BUTTON_100:
		{
			//设置变量
			m_lCurrentJetton = 0L;
			break;
		}
		case IDC_JETTON_BUTTON_1000:
		{
			//设置变量
			m_lCurrentJetton = 10000L;
			break;
		}
		case IDC_JETTON_BUTTON_10000:
		{
			//设置变量
			m_lCurrentJetton = 100000L;
			break;
		}
		case IDC_JETTON_BUTTON_50000:
		{
			//设置变量
			m_lCurrentJetton = 1000000L;
			break;
		}
		case IDC_JETTON_BUTTON_100000:
		{
			//设置变量
			m_lCurrentJetton = 5000000L;
			break;
		}
		case IDC_JETTON_BUTTON_500000:
		{
			//设置变量
			m_lCurrentJetton = 10000000L;
			break;
		}
		case IDC_JETTON_BUTTON_1000000:
		{
			//设置变量
			m_lCurrentJetton = 50000000L;
			break;
		}
		case IDC_JETTON_BUTTON_5000000:
		{
			//设置变量
			m_lCurrentJetton = 0L;
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
			AfxGetMainWnd()->SendMessage(IDM_CONTINUE_CARD, 0, 0);
			break;
		}
	}

	return CGameFrameView::OnCommand(wParam, lParam);
}

//我的位置
void CGameClientView::SetMeChairID(DWORD dwMeUserID)
{
	//查找椅子号
	for (WORD wChairID = 0; wChairID < MAX_CHAIR; ++wChairID)
	{
		tagUserData const *pUserData = GetUserInfo(wChairID);
		if (NULL != pUserData && dwMeUserID == pUserData->dwUserID)
		{
			m_wMeChairID = wChairID;
			break;
		}
	}
}

//设置提示
void CGameClientView::SetDispatchCardTip(enDispatchCardTip DispatchCardTip)
{
	//设置变量
	m_enDispatchCardTip = DispatchCardTip;

	//设置定时器
	if (enDispatchCardTip_NULL != DispatchCardTip) SetTimer(IDI_SHOWDISPATCH_CARD_TIP, 2*1000, NULL);
	else KillTimer(IDI_SHOWDISPATCH_CARD_TIP);

	//更新界面
	UpdateGameView(NULL);
}

//绘画庄家
void CGameClientView::DrawBankerInfo(CDC *pDC, int nWidth, int nHeight)
{
	//庄家信息
	pDC->SetTextColor(RGB(255, 234, 0));

	//获取玩家
	tagUserData const *pUserData = m_wBankerUser == INVALID_CHAIR ? NULL : GetUserInfo(m_wBankerUser);

	//位置信息
	CRect static StrRect;
	StrRect.left = nWidth / 2 - 307;
	StrRect.top = nHeight / 2 - 309;
	StrRect.right = StrRect.left + 66;
	StrRect.bottom = StrRect.top + 15;

	//庄家名字
	pDC->DrawText(pUserData == NULL ? (m_bEnableSysBanker ? TEXT("系统坐庄") : TEXT("无人坐庄")) : pUserData->szName, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP | DT_SINGLELINE);

	//庄家总分
	StrRect.left = nWidth / 2 - 184;
	StrRect.top = nHeight / 2 - 309;
	StrRect.right = StrRect.left + 106;
	StrRect.bottom = StrRect.top + 15;
	DrawNumberStringWithSpace(pDC, pUserData == NULL ? 0 : pUserData->lScore, StrRect);

	//庄家局数
	StrRect.left = nWidth / 2 - 36;
	StrRect.top = nHeight / 2 - 309;
	StrRect.right = StrRect.left + 57;
	StrRect.bottom = StrRect.top + 15;
	DrawNumberStringWithSpace(pDC, m_wBankerTime, StrRect);

	//庄家成绩
	StrRect.left = nWidth / 2 + 60;
	StrRect.top = nHeight / 2 - 309;
	StrRect.right = StrRect.left + 75;
	StrRect.bottom = StrRect.top + 15;
	DrawNumberStringWithSpace(pDC, m_lBankerWinScore, StrRect);
}

void CGameClientView::SetFirstShowCard(BYTE *bcCard)
{
	if (bcCard)
	{
		m_bcfirstShowCard[0] = bcCard[0];
		m_bcfirstShowCard[1] = bcCard[1];

		m_Sice[0].SetSiceValue(m_bcfirstShowCard[0]);
		m_Sice[1].SetSiceValue(m_bcfirstShowCard[1]);

		SetTimer(IDI_SICE, 200, NULL);
	}
	else
		ZeroMemory(m_bcfirstShowCard, 0, sizeof(m_bcfirstShowCard));
}

//绘画玩家
void CGameClientView::DrawMeInfo(CDC *pDC, int nWidth, int nHeight)
{
	//合法判断
// 	if (INVALID_CHAIR == m_wMeChairID) return;
//
// 	const tagUserData *pMeUserData = GetUserInfo(m_wMeChairID);
//
// 	if (pMeUserData != NULL)
// 	{
// 		//字体颜色
// 		pDC->SetTextColor(RGB(255, 255, 255));
//
// 		//总共下注
// 		LONGLONG lMeJetton = 0L;
// 		for (int nAreaIndex = 1; nAreaIndex <= AREA_COUNT; ++nAreaIndex) lMeJetton += m_lUserJettonScore[nAreaIndex];
//
// 		//设置位置
// 		CRect static rcDraw;
// 		rcDraw.left = m_MeInfoRect.left;
// 		rcDraw.top = m_MeInfoRect.top;
// 		rcDraw.right = rcDraw.left + 140;
// 		rcDraw.bottom = rcDraw.top + 15;
//
// 		//绘画帐号
// 		pDC->DrawText(pMeUserData->szName, lstrlen(pMeUserData->szName), rcDraw, DT_VCENTER | DT_SINGLELINE | DT_LEFT | DT_END_ELLIPSIS);
//
// 		//绘画积分
// 		rcDraw.top = m_MeInfoRect.top + 20;
// 		rcDraw.bottom = rcDraw.top + 15;
// 		DrawNumberStringWithSpace(pDC, pMeUserData->lScore - lMeJetton, rcDraw, DT_VCENTER | DT_SINGLELINE | DT_LEFT | DT_END_ELLIPSIS);
//
// 		//绘画成绩
// 		rcDraw.top = m_MeInfoRect.top + 2 * 20;
// 		rcDraw.bottom = rcDraw.top + 15;
// 		DrawNumberStringWithSpace(pDC, m_lMeStatisticScore, rcDraw, DT_VCENTER | DT_SINGLELINE | DT_LEFT | DT_END_ELLIPSIS);
//
// 	}
}
//////////////////////////////////////////////////////////////////////////
//银行存款
void CGameClientView::OnBankStorage()
{
	//获取接口
	CGameClientDlg *pGameClientDlg = CONTAINING_RECORD(this, CGameClientDlg, m_GameClientView);
	IClientKernel *pIClientKernel = (IClientKernel *)pGameClientDlg->GetClientKernel(IID_IClientKernel, VER_IClientKernel);

	if (NULL != pIClientKernel)
	{
// 		if (NULL == m_DlgBank.m_hWnd) m_DlgBank.Create(IDD_BANK_STORAGE, this);
// 		m_DlgBank.SetSendInfo(pIClientKernel, pIClientKernel->GetMeUserInfo());
// 		m_DlgBank.m_blCanStore = m_blCanStore;
// 		//m_DlgBank.SetBankerActionType(true);
// 		m_DlgBank.ShowWindow(SW_SHOW);
// 		m_DlgBank.PostMessage(WM_CREADED, 0, 0);


		CPoint Point;
		GetCursorPos(&Point);
		CRect Rect;
		CRect btRect;
		m_btBank.GetWindowRect(&btRect);
//		m_DlgBank.GetWindowRect(&Rect);
		//m_DlgBank.SetPostPoint(CPoint(btRect.right-76,btRect.top+178+30 ));
		//m_DlgBank.SetWindowPos(NULL,)
//		m_DlgBank.CenterWindow();
	}
	AfxGetMainWnd()->SendMessage(IDM_BANK, 0, 0);
}

//银行取款
void CGameClientView::OnBankDraw()
{
	//获取接口
	CGameClientDlg *pGameClientDlg = CONTAINING_RECORD(this, CGameClientDlg, m_GameClientView);
	IClientKernel *pIClientKernel = (IClientKernel *)pGameClientDlg->GetClientKernel(IID_IClientKernel, VER_IClientKernel);

	if (NULL != pIClientKernel)
	{
// 		if (NULL == m_DlgBank.m_hWnd) m_DlgBank.Create(IDD_BANK_STORAGE, this);
// 		m_DlgBank.SetSendInfo(pIClientKernel, pIClientKernel->GetMeUserInfo());
// 		m_DlgBank.SetBankerActionType(false);
// 		m_DlgBank.m_blCanStore = m_blCanStore;
// 		m_DlgBank.ShowWindow(SW_SHOW);
// 		CPoint Point;
// 		GetCursorPos(&Point);
// 		CRect Rect;
// 		CRect btRect;
// 		m_btBank.GetWindowRect(&btRect);
// 		m_DlgBank.GetWindowRect(&Rect);
// 		//m_DlgBank.SetPostPoint(CPoint(btRect.right,btRect.top));
// 		m_DlgBank.CenterWindow();
	}

	AfxGetMainWnd()->SendMessage(IDM_BANK, 0, 0);
}

void CGameClientView::OnUp()
{
	m_ApplyUser.m_AppyUserList.SendMessage(WM_VSCROLL, MAKELONG(SB_LINEUP, 0), NULL);
	m_ApplyUser.m_AppyUserList.Invalidate(TRUE);
	double nPos = m_ApplyUser.m_AppyUserList.GetScrollPos(SB_VERT);
	double nMax = m_ApplyUser.m_AppyUserList.GetScrollLimit(SB_VERT);

}

void CGameClientView::OnDown()
{
	double nPos = m_ApplyUser.m_AppyUserList.GetScrollPos(SB_VERT);
	if (m_ApplyUser.m_AppyUserList.GetItemCount() > 4)
	{
		if (nPos > m_ApplyUser.m_AppyUserList.GetItemCount() - 5)
		{
			return ;

		}

	}
	m_ApplyUser.m_AppyUserList.SendMessage(WM_VSCROLL, MAKELONG(SB_LINEDOWN, 0), NULL);
	m_ApplyUser.m_AppyUserList.Invalidate(TRUE);
	double nMax = m_ApplyUser.m_AppyUserList.GetScrollLimit(SB_VERT);
};

void CGameClientView::OnControl()
{
	if (m_pGameClientDlg)
	{
		m_pGameClientDlg->SendData(SUB_C_GET_WIN_RATE);
		ShowWinCtrl();
	}
	else
		MessageBox("对不起，m_pGameClientDlg为NULL");

	return;
}

//显示界面
afx_msg void CGameClientView::ShowWinCtrl()
{
	if (m_dlgWinCtrl.DoModal() == IDOK)
	{
		m_pGameClientDlg->SendData(SUB_C_SET_WIN_RATE, &m_dlgWinCtrl.m_Info, sizeof(m_dlgWinCtrl.m_Info));
	}
}

void CGameClientView::ClearBank()
{
//	m_DlgBank.ClearBank();

}

//设置历史麻将
void CGameClientView::SetCodeHistory(BYTE bCode[8])
{
	//m_CodeArray[5][8]
	int i = 0;
	if (m_CardHistoryArray[4][0] != 0)
	{
		for (; i < 4; i ++)
		{
			for (int j = 0; j < 8; j ++)
			{
				m_CardHistoryArray[i][j] = m_CardHistoryArray[i + 1][j];
			}
		}
	}
	else
	{
		for (; i < 5; i ++)
			if (m_CardHistoryArray[i][0] == 0) break;
	}

	if (i <= 4)
	{
		for (int j = 0; j < 8; j ++)
		{
			m_CardHistoryArray[i][j] = bCode[j];
		}
	}


	UpdateGameView(NULL);
}