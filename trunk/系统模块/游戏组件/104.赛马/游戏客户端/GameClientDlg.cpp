#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include "resource.h"
#include "ch.h"
//////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_FREE					99									//空闲时间
#define IDI_PLACE_JETTON			100									//下注时间
#define IDI_HORSE_RUN				301									//赛马时间

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	//ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(IDM_PLACE_JETTON,OnPlaceJetton)
	ON_MESSAGE(IDM_APPLY_BANKER, OnApplyBanker)
	ON_MESSAGE(IDM_BANK, OnBank)
	ON_WM_SETTINGCHANGE()
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//限制信息
	m_lMeMaxScore=0L;			
	m_lAreaLimitScore=0L;		
	m_lApplyBankerCondition=0L;	

	//个人下注
	m_lMeScore2=0L;
	m_lMeScore3=0L;
	m_lMeScore1=0L;
	m_lMeScore6=0L;
	m_lMeScore5=0L;
	m_lMeScore4=0L;
	m_lMeScore7=0L;
	m_lMeScore8=0L;

	//庄家信息
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;

	//状态变量
	m_bMeApplyBanker=false;

	ZeroMemory(&rtMax,sizeof(rtMax));
	
	return;
}

//析构函数
CGameClientDlg::~CGameClientDlg()
{
}


//初始函数

BOOL CGameClientDlg::OnInitDialog()
{
	__super::OnInitDialog();
	
	//SetWindowPos(NULL,200,50,1024,768,SWP_NOZORDER);
	ModifyStyle(WS_THICKFRAME, CS_VREDRAW| CS_HREDRAW|DS_MODALFRAME|DS_CENTER, 0);
	CenterWindow();
	RestoreWindow();

	CRect rtWindow;
	GetWindowRect(&rtWindow);
	ScreenToClient(&rtWindow);
	rtMax.left = rtWindow.right-48;
	rtMax.right = rtMax.left+22;
	rtMax.top = rtWindow.top+2;
	rtMax.bottom = rtMax.top+17;
	//TCHAR str[100];
	//sprintf(str,"%d,%d,%d,%d,\n%d,%d,%d,%d",rtMax.left,rtMax.right,rtMax.top,rtMax.bottom,rtWindow.left,rtWindow.right,rtWindow.top,rtWindow.bottom);
	//MessageBox(str);

	return TRUE;
}

//初始函数
bool CGameClientDlg::InitGameFrame()
{
	//设置标题
	SetWindowText(TEXT("赛马游戏  --  Ver：6.6.1.0"));

	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//加载声音
	VERIFY(m_DTSDBackground.Create(TEXT("BACK_GROUND")));
	VERIFY(m_DTSDCheer[0].Create(TEXT("CHEER1")));
	VERIFY(m_DTSDCheer[1].Create(TEXT("CHEER2")));
	VERIFY(m_DTSDCheer[2].Create(TEXT("CHEER3")));

 
	return true;
}

//重置框架
void CGameClientDlg::ResetGameFrame()
{
	//限制信息
	m_lMeMaxScore=0L;			

	//个人下注
	m_lMeScore2=0L;
	m_lMeScore3=0L;
	m_lMeScore1=0L;
	m_lMeScore6=0L;
	m_lMeScore5=0L;
	m_lMeScore4=0L;
	m_lMeScore7=0L;
	m_lMeScore8=0L;

	//庄家信息
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;

	//状态变量
	m_bMeApplyBanker=false;

	return;
}

//游戏设置
void CGameClientDlg::OnGameOptionSet()
{
	return;
}

//时间消息
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	static int num = 0;
	if ((nTimerID==IDI_PLACE_JETTON)&&(nElapse==0))
	{
		num = 0;
		//设置光标
		m_GameClientView.SetCurrentJetton(0L);

		//禁止按钮
		m_GameClientView.m_btJetton100.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton1000.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton10000.EnableWindow(FALSE);	
		m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton5000000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton10000000.EnableWindow(FALSE);		


		//庄家按钮
		m_GameClientView.m_btApplyBanker.EnableWindow( FALSE );
		m_GameClientView.m_btCancelBanker.EnableWindow( FALSE );

		
	}
	if ((nTimerID==IDI_PLACE_JETTON)&&IsEnableSound()) 
	{
		if (nElapse<=2&&nElapse>1) PlayGameSound(HORSE_READY);
	}


	if (nTimerID == IDI_HORSE_RUN && nElapse > 11 && (nElapse%3) == 0)
	{
		m_GameClientView.SetEveryHorseSpeed(8- (nElapse-11)/3);
	}

	if (nTimerID == IDI_HORSE_RUN)
	{	
		if (nElapse == 34)
		{
			PlayGameSound(RACE_RUN);
		}	
		if (IsEnableSound() && m_GameClientView.GetIsFinishRace())
		{
			
			num ++;
			if (num == 3)
			{
				PlayGameSound(RACE_OVER);
			}						
		}
	}

	return true;
}

//旁观状态
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
}

//网络消息
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_FREE:		//游戏空闲
		{
			return OnSubGameFree(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:		//游戏开始
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON:	//用户加注
		{
			return OnSubPlaceJetton(pBuffer,wDataSize);
		}
	case SUB_S_APPLY_BANKER:	//申请做庄
		{
			return OnSubUserApplyBanker(pBuffer, wDataSize);
		}
	case SUB_S_CANCEL_BANKER:	//取消做庄
		{
			return OnSubUserCancelBanker(pBuffer, wDataSize);
		}
	case SUB_S_CHANGE_BANKER:	//切换庄家
		{
			return OnSubChangeBanker(pBuffer, wDataSize);
		}
	case SUB_S_GAME_END:		//游戏结束
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_SEND_RECORD:		//游戏记录
		{
			return OnSubGameRecord(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON_FAIL:	//下注失败
		{
			return OnSubPlaceJettonFail(pBuffer,wDataSize);
		}
	case SUB_S_UPDATE_USER_SCORE:		//更新成绩
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_UpdateUserScore));
			if (wDataSize!=sizeof(CMD_S_UpdateUserScore)) return false;

			//消息处理
			CMD_S_UpdateUserScore * UpdateUserScore=(CMD_S_UpdateUserScore *)pBuffer;
			m_lMeMaxScore = UpdateUserScore->lUserScore;
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			return true;
		}
	case SUB_S_UPDATE_BANKER_SCORE:	//更新成绩
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_UpdateBankerScore));
			if (wDataSize!=sizeof(CMD_S_UpdateUserScore)) return false;

			//消息处理
			CMD_S_UpdateBankerScore * UpdateBankerScore=(CMD_S_UpdateBankerScore *)pBuffer;
			m_lBankerScore = UpdateBankerScore->lBankerScore;
			m_GameClientView.SetBankerInfo(m_wCurrentBanker,m_lBankerScore);
			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

//游戏场景
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_FREE:			//空闲状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//消息处理
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//设置时间
			SetGameTimer(GetMeChairID(),IDI_FREE,pStatusFree->cbTimeLeave);		

            //玩家信息
			m_lMeMaxScore=pStatusFree->lUserMaxScore;			
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			WORD wMeChairID=GetMeChairID();
			m_GameClientView.SetMeChairID(SwitchViewChairID(wMeChairID));
		
            //庄家信息
			SetBankerInfo(pStatusFree->wBankerUser,pStatusFree->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusFree->cbBankerTime,pStatusFree->lBankerWinScore);
			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

            //控制信息
			m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusFree->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			//设置状态
			SetGameStatus(GS_FREE);

			//更新控制
			UpdateButtonContron();
			m_GameClientView.UpdateGameView(NULL);

			return true;
		}
	case GS_PLAYING:		//游戏状态
	case GS_GAME_END:		//结束状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//消息处理
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//全局下注
			m_GameClientView.PlaceUserJetton(ID_HORSE1,pStatusPlay->lAllScore1);
			m_GameClientView.PlaceUserJetton(ID_HORSE2,pStatusPlay->lAllScore2);
			m_GameClientView.PlaceUserJetton(ID_HORSE3,pStatusPlay->lAllScore3);
			m_GameClientView.PlaceUserJetton(ID_HORSE4,pStatusPlay->lAllScore4);
			m_GameClientView.PlaceUserJetton(ID_HORSE5,pStatusPlay->lAllScore5);
			m_GameClientView.PlaceUserJetton(ID_HORSE6,pStatusPlay->lAllScore6);
			m_GameClientView.PlaceUserJetton(ID_HORSE7,pStatusPlay->lAllScore7);
			m_GameClientView.PlaceUserJetton(ID_HORSE8,pStatusPlay->lAllScore8);

			//玩家下注
			SetMePlaceJetton(ID_HORSE1,pStatusPlay->lUserScore1);
			SetMePlaceJetton(ID_HORSE2,pStatusPlay->lUserScore2);
			SetMePlaceJetton(ID_HORSE3,pStatusPlay->lUserScore3);
			SetMePlaceJetton(ID_HORSE4,pStatusPlay->lUserScore4);
			SetMePlaceJetton(ID_HORSE5,pStatusPlay->lUserScore5);
			SetMePlaceJetton(ID_HORSE6,pStatusPlay->lUserScore6);
			SetMePlaceJetton(ID_HORSE7,pStatusPlay->lUserScore7);
			SetMePlaceJetton(ID_HORSE8,pStatusPlay->lUserScore8);

			//玩家积分
			m_lMeMaxScore=pStatusPlay->lUserMaxScore;			
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			WORD wMeChairID=GetMeChairID();
			m_GameClientView.SetMeChairID(SwitchViewChairID(wMeChairID));

			//控制信息
			m_lApplyBankerCondition=pStatusPlay->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusPlay->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			if (pStatusPlay->cbGameStatus==GS_GAME_END)
			{
				//扑克信息
				m_GameClientView.SetHorseSpeed(pStatusPlay->cbHorseSpeed);
				m_GameClientView.FinishRun();

				//设置成绩
				m_GameClientView.SetCurGameScore(pStatusPlay->lEndUserScore,pStatusPlay->lEndUserReturnScore,pStatusPlay->lEndBankerScore,pStatusPlay->lEndRevenue);
			}
			else
			{
				//m_GameClientView.SetHorseSpeed(NULL);

				//播放声音
				if (IsEnableSound()) m_DTSDBackground.Play(0,true);
			}

			//庄家信息
			SetBankerInfo(pStatusPlay->wBankerUser,pStatusPlay->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusPlay->cbBankerTime,pStatusPlay->lBankerWinScore);
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			//设置状态
			SetGameStatus(pStatusPlay->cbGameStatus);

			//设置时间
			SetGameTimer(GetMeChairID(),pStatusPlay->cbGameStatus==GS_GAME_END?IDI_HORSE_RUN:IDI_PLACE_JETTON,pStatusPlay->cbTimeLeave);

			//更新按钮
			UpdateButtonContron();
			m_GameClientView.UpdateGameView(NULL);

			return true;
		}
	}

	return false;
}

//游戏开始
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//庄家信息
	SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

	//玩家信息
	m_lMeMaxScore=pGameStart->lUserMaxScore;
	m_GameClientView.SetMeMaxScore(m_lMeMaxScore);

	m_lAreaLimitScore = pGameStart->lAreaLimitScore;
	m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

	//设置时间
	SetGameTimer(GetMeChairID(),IDI_PLACE_JETTON,pGameStart->cbTimeLeave);
	//设置空闲状态
	m_GameClientView.SetFreeTime();

	//设置状态
	SetGameStatus(GS_PLACE_JETTON);

	//更新控制
	UpdateButtonContron();

	//更新界面
	m_GameClientView.UpdateGameView(NULL);

	//播放声音
	if (IsEnableSound()) 
	{
		PlayGameSound(GAME_START);
		m_DTSDBackground.Play(0,true);
	}

	return true;
}

//游戏空闲
bool CGameClientDlg::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;

	//消息处理
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;

	//设置时间
	SetGameTimer(GetMeChairID(),IDI_FREE,pGameFree->cbTimeLeave);

	//设置空闲状态
	m_GameClientView.SetFreeTime();

	//设置状态
	SetGameStatus(GS_FREE);

	//清理桌面
	m_GameClientView.SetWinnerSide(0xFF);
	m_GameClientView.CleanUserJetton();
	for (int nAreaIndex=ID_HORSE1; nAreaIndex<=ID_HORSE8; ++nAreaIndex) SetMePlaceJetton(nAreaIndex,0);

	//更新控件
	UpdateButtonContron();

	return true;
}

//用户加注
bool CGameClientDlg::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) return false;

	//消息处理
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;

	//加注界面
	m_GameClientView.PlaceUserJetton(pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);

	//播放声音
	if (IsEnableSound()) 
	{
		if (pPlaceJetton->wChairID!=GetMeChairID() || IsLookonMode())
		{
			if (pPlaceJetton->lJettonScore==5000000 || pPlaceJetton->lJettonScore==10000000) PlayGameSound(ADD_GOLD_EX);
			else PlayGameSound(ADD_GOLD);
			m_DTSDCheer[rand()%3].Play();
		}
	}
    
	return true;
}

//游戏结束
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//取消动画
	for (WORD wAreaIndex=ID_HORSE1; wAreaIndex<=ID_HORSE8; ++wAreaIndex) m_GameClientView.SetBombEffect(false,wAreaIndex);

	//清空下注
	//m_GameClientView.CleanUserJetton();

	if (IsEnableSound())
	{
		PlayGameSound(HORSE_GO);
		
	}
	//设置时间
	SetGameTimer(GetMeChairID(),IDI_HORSE_RUN, pGameEnd->cbTimeLeave);

	////扑克信息
	m_GameClientView.SetHorseSpeed(pGameEnd->cbHorseSpeed);

	//庄家信息
	m_GameClientView.SetBankerScore(pGameEnd->nBankerTime, pGameEnd->lBankerTotallScore);

	//成绩信息
	m_GameClientView.SetCurGameScore(pGameEnd->lUserScore,pGameEnd->lUserReturnScore,pGameEnd->lBankerScore,pGameEnd->lRevenue);

	//设置状态
	SetGameStatus(GS_GAME_END);
	//m_GameBank.ShowWindow(SW_HIDE);
	//更新成绩
	for (WORD wUserIndex = 0; wUserIndex < MAX_CHAIR; ++wUserIndex)
	{
		tagUserData const *pUserData = GetUserData(wUserIndex);

		if ( pUserData == NULL ) continue;
		tagApplyUser ApplyUser ;

		//更新信息
		ApplyUser.lUserScore = pUserData->lScore;
		ApplyUser.strUserName = pUserData->szName;
		m_GameClientView.m_ApplyUser.UpdateUser(ApplyUser);
	}

	//更新控件
	UpdateButtonContron();

	//停止声音
	m_DTSDBackground.Stop();
	m_DTSDCheer[0].Stop();
	m_DTSDCheer[1].Stop();
	m_DTSDCheer[2].Stop();
	//m_DTSDRaceHorse.Stop();

	return true;
}

//更新控制
void CGameClientDlg::UpdateButtonContron()
{
	//置能判断
	bool bEnablePlaceJetton=true;
	if (m_bEnableSysBanker==false&&m_wCurrentBanker==INVALID_CHAIR) bEnablePlaceJetton=false;
	if (GetGameStatus()!=GS_PLACE_JETTON) bEnablePlaceJetton=false;
	if (m_wCurrentBanker==GetMeChairID()) bEnablePlaceJetton=false;
	if (IsLookonMode()) bEnablePlaceJetton=false;

	//下注按钮
	if (bEnablePlaceJetton==true)
	{
		//计算积分
		LONGLONG lCurrentJetton=m_GameClientView.GetCurrentJetton();
		LONGLONG lLeaveScore=m_lMeMaxScore-m_lMeScore1-m_lMeScore2-m_lMeScore3-m_lMeScore4-m_lMeScore5-
			m_lMeScore6-m_lMeScore7-m_lMeScore8;

		//设置光标
		if (lCurrentJetton>lLeaveScore)
		{
			if (lLeaveScore>=10000000L) m_GameClientView.SetCurrentJetton(10000000L);
			else if (lLeaveScore>=5000000L) m_GameClientView.SetCurrentJetton(5000000L);
			else if (lLeaveScore>=1000000L) m_GameClientView.SetCurrentJetton(1000000L);
			else if (lLeaveScore>=100000L) m_GameClientView.SetCurrentJetton(100000L);
			else if (lLeaveScore>=10000L) m_GameClientView.SetCurrentJetton(10000L);
			else if (lLeaveScore>=1000L) m_GameClientView.SetCurrentJetton(1000L);
			else if (lLeaveScore>=100L) m_GameClientView.SetCurrentJetton(100L);
			else m_GameClientView.SetCurrentJetton(0L);
		}

		//控制按钮
		m_GameClientView.m_btJetton100.EnableWindow((lLeaveScore>=100)?TRUE:FALSE);
		m_GameClientView.m_btJetton1000.EnableWindow((lLeaveScore>=1000)?TRUE:FALSE);
		m_GameClientView.m_btJetton10000.EnableWindow((lLeaveScore>=10000)?TRUE:FALSE);
		m_GameClientView.m_btJetton100000.EnableWindow((lLeaveScore>=100000)?TRUE:FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow((lLeaveScore>=1000000)?TRUE:FALSE);		
		m_GameClientView.m_btJetton5000000.EnableWindow((lLeaveScore>=5000000)?TRUE:FALSE);
		m_GameClientView.m_btJetton10000000.EnableWindow((lLeaveScore>=10000000)?TRUE:FALSE);		
	}
	else
	{
		//设置光标
		m_GameClientView.SetCurrentJetton(0L);

		//禁止按钮
		m_GameClientView.m_btJetton100.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton1000.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton10000.EnableWindow(FALSE);	
		m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton5000000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton10000000.EnableWindow(FALSE);
	}

	//庄家按钮
	if (!IsLookonMode())
	{
		//获取信息
		const tagUserData *pMeUserData=GetUserData(GetMeChairID());

		//申请按钮
		bool bEnableApply=true;
		if (m_wCurrentBanker==GetMeChairID()) bEnableApply=false;
		if (m_bMeApplyBanker) bEnableApply=false;
		if (pMeUserData->lScore<m_lApplyBankerCondition) bEnableApply=false;
		m_GameClientView.m_btApplyBanker.EnableWindow(bEnableApply?TRUE:FALSE);

		//取消按钮
		bool bEnableCancel=true;
		if (m_wCurrentBanker==GetMeChairID() && GetGameStatus()!=GS_FREE) bEnableCancel=false;
		if (m_bMeApplyBanker==false) bEnableCancel=false;
		m_GameClientView.m_btCancelBanker.EnableWindow(bEnableCancel?TRUE:FALSE);
		m_GameClientView.m_btCancelBanker.SetButtonImage(m_wCurrentBanker==GetMeChairID()?IDB_BT_CANCEL_BANKER:IDB_BT_CANCEL_APPLY,false);

		//显示判断
		if (m_bMeApplyBanker)
		{
			m_GameClientView.m_btCancelBanker.ShowWindow(SW_SHOW);
			m_GameClientView.m_btApplyBanker.ShowWindow(SW_HIDE);
		}
		else
		{
			m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
			m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
		}
	}
	else
	{
		m_GameClientView.m_btCancelBanker.EnableWindow(FALSE);
		m_GameClientView.m_btApplyBanker.EnableWindow(FALSE);
		m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
		m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
	}

	return;
}

//加注消息
LRESULT CGameClientDlg::OnPlaceJetton(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	BYTE cbJettonArea=(BYTE)wParam;
	LONGLONG lJettonScore=(LONGLONG)lParam;

	//庄家判断
	if ( GetMeChairID() == m_wCurrentBanker ) return true;

	//状态判断
	if (GetGameStatus()!=GS_PLACE_JETTON)
	{
		UpdateButtonContron();
		return true;
	}

	//设置变量
	switch (cbJettonArea)
	{
		case ID_HORSE1:
			{
				m_lMeScore1 += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_HORSE1,m_lMeScore1);
				break;
			}
		case ID_HORSE2:
			{
				m_lMeScore2 += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_HORSE2,m_lMeScore2);
				break;
			}
		case ID_HORSE3:
			{
				m_lMeScore3 += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_HORSE3,m_lMeScore3);
				break;
			}
		
		case ID_HORSE4:
			{
				m_lMeScore4 += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_HORSE4,m_lMeScore4);
				break;
			}
		case ID_HORSE5:
			{
				m_lMeScore5 += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_HORSE5,m_lMeScore5);
				break;
			}
		case ID_HORSE6:
			{
				m_lMeScore6 += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_HORSE6,m_lMeScore6);
				break;
			}
		case ID_HORSE7:
			{
				m_lMeScore7 += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_HORSE7,m_lMeScore7);
				break;
			}
		case ID_HORSE8:
			{
				m_lMeScore8 += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_HORSE8,m_lMeScore8);
				break;
			}
		}

	//变量定义
	CMD_C_PlaceJetton PlaceJetton;
	ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

	//构造变量
	PlaceJetton.cbJettonArea=cbJettonArea;
	PlaceJetton.lJettonScore=lJettonScore;

	//发送消息
	SendData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

	//更新按钮
	UpdateButtonContron();

	//播放声音
	if (IsEnableSound()) 
	{
		if (lJettonScore==5000000 || lJettonScore==10000000) PlayGameSound(ADD_GOLD_EX);
		else PlayGameSound(ADD_GOLD);
		m_DTSDCheer[rand()%3].Play();
	}


	return 0;
}

//申请消息
LRESULT CGameClientDlg::OnApplyBanker(WPARAM wParam, LPARAM lParam)
{
	//合法判断
	tagUserData const *pMeUserData = GetUserData( GetMeChairID() );
	if (pMeUserData->lScore < m_lApplyBankerCondition) return true;

	//旁观判断
	if (IsLookonMode()) return true;

	//转换变量
	bool bApplyBanker = wParam ? true:false;

	//当前判断
	if (m_wCurrentBanker == GetMeChairID() && bApplyBanker) return true;

	if (bApplyBanker)
	{
		//发送消息
		SendData(SUB_C_APPLY_BANKER, NULL, 0);

		m_bMeApplyBanker=true;
	}
	else
	{
		//发送消息
		SendData(SUB_C_CANCEL_BANKER, NULL, 0);

		m_bMeApplyBanker=false;
	}

	//设置按钮
	UpdateButtonContron();

	return true;
}

//申请做庄
bool CGameClientDlg::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;

	//消息处理
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;

	//获取玩家
	tagUserData const *pUserData=GetUserData(pApplyBanker->wApplyUser);

	//插入玩家
	if (m_wCurrentBanker!=pApplyBanker->wApplyUser)
	{
		tagApplyUser ApplyUser;
		ApplyUser.strUserName=pUserData->szName;
		ApplyUser.lUserScore=pUserData->lScore;
		m_GameClientView.m_ApplyUser.InserUser(ApplyUser);
	}

	//自己判断
	if (IsLookonMode()==false && GetMeChairID()==pApplyBanker->wApplyUser) m_bMeApplyBanker=true;

	//更新控件
	UpdateButtonContron();

	return true;
}

//取消做庄
bool CGameClientDlg::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;

	//消息处理
	CMD_S_ApplyBanker * pCancelyBanker=(CMD_S_ApplyBanker *)pBuffer;

	//获取玩家
	tagUserData const *pUserData=GetUserData(pCancelyBanker->wApplyUser);

	//删除玩家
	tagApplyUser ApplyUser;
	ApplyUser.strUserName=pUserData->szName;
	ApplyUser.lUserScore=pUserData->lScore;
	m_GameClientView.m_ApplyUser.DeleteUser(ApplyUser);

	//自己判断
	if (IsLookonMode()==false && GetMeChairID()==pCancelyBanker->wApplyUser) m_bMeApplyBanker=false;

	//更新控件
	UpdateButtonContron();

	return true;
}

//切换庄家
bool CGameClientDlg::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	//消息处理
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

	//显示图片
	m_GameClientView.ShowChangeBanker(m_wCurrentBanker!=pChangeBanker->wBankerUser);

	//自己判断
	if (m_wCurrentBanker==GetMeChairID() && IsLookonMode() == false && pChangeBanker->wBankerUser!=GetMeChairID()) 
	{
		m_bMeApplyBanker=false;
	}
	else if (IsLookonMode() == false && pChangeBanker->wBankerUser==GetMeChairID())
	{
		m_bMeApplyBanker=true;
	}

	//庄家信
	SetBankerInfo(pChangeBanker->wBankerUser,pChangeBanker->lBankerScore);
	m_GameClientView.SetBankerScore(0,0);

	//删除玩家
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		tagUserData const *pBankerUserData=GetUserData(m_wCurrentBanker);
		if (pBankerUserData != NULL)
		{
			tagApplyUser ApplyUser;
			ApplyUser.strUserName = pBankerUserData->szName;
			m_GameClientView.m_ApplyUser.DeleteUser(ApplyUser);
		}
	}

	//更新界面
	UpdateButtonContron();
	m_GameClientView.UpdateGameView(NULL);

	return true;
}

//游戏记录
bool CGameClientDlg::OnSubGameRecord(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize%sizeof(tagServerGameRecord)==0);
	if (wDataSize%sizeof(tagServerGameRecord)!=0) return false;

	//设置记录
	WORD wRecordCount=wDataSize/sizeof(tagServerGameRecord);
	for (WORD wIndex=0;wIndex<wRecordCount;wIndex++) 
	{
		tagServerGameRecord * pServerGameRecord=(((tagServerGameRecord *)pBuffer)+wIndex);

		m_GameClientView.SetGameHistory(enOperateResult_NULL, pServerGameRecord->cbWinner);
	}

	return true;
}

//下注失败
bool CGameClientDlg::OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJettonFail));
	if (wDataSize!=sizeof(CMD_S_PlaceJettonFail)) return false;

	//消息处理
	CMD_S_PlaceJettonFail * pPlaceJettonFail=(CMD_S_PlaceJettonFail *)pBuffer;

	//效验参数
	BYTE cbViewIndex=pPlaceJettonFail->lJettonArea;
	ASSERT(cbViewIndex<=ID_HORSE8);
	if (cbViewIndex>ID_HORSE8) return false;

	LONGLONG lJettonCount=pPlaceJettonFail->lPlaceScore;
	if (cbViewIndex==ID_HORSE1)
	{
		m_lMeScore1=lJettonCount-lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeScore1);
	}
	else if (cbViewIndex==ID_HORSE2) 
	{
		m_lMeScore2=lJettonCount-lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeScore2);
	}
	else if (cbViewIndex==ID_HORSE3) 
	{
		m_lMeScore3=lJettonCount-lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeScore3);
	}
	else if (cbViewIndex==ID_HORSE4) 
	{
		m_lMeScore5=lJettonCount-lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeScore4);
	}
	else if (cbViewIndex==ID_HORSE5) 
	{
		m_lMeScore4=lJettonCount-lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeScore5);
	}
	else if (cbViewIndex==ID_HORSE6) 
	{
		m_lMeScore6=lJettonCount-lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeScore6);
	}
	else if (cbViewIndex==ID_HORSE7) 
	{
		m_lMeScore7=lJettonCount-lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeScore7);
	}
	else if (cbViewIndex==ID_HORSE8) 
	{
		m_lMeScore8=lJettonCount-lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeScore8);
	}
	else {ASSERT(FALSE);return false;}

	return true;
}

//设置庄家
void CGameClientDlg::SetBankerInfo(WORD wBanker,LONGLONG lScore)
{
	m_wCurrentBanker=wBanker;
	m_lBankerScore=lScore;
	WORD wBankerViewChairID=m_wCurrentBanker==INVALID_CHAIR ? INVALID_CHAIR:SwitchViewChairID(m_wCurrentBanker);
	m_GameClientView.SetBankerInfo(wBankerViewChairID,m_lBankerScore);
}

//个人下注
void CGameClientDlg::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	//效验参数
	ASSERT(cbViewIndex<=ID_HORSE8);
	if (cbViewIndex>ID_HORSE8) return;

	if (cbViewIndex==ID_HORSE1) m_lMeScore1=lJettonCount;
	else if (cbViewIndex==ID_HORSE2) m_lMeScore2=lJettonCount;
	else if (cbViewIndex==ID_HORSE3) m_lMeScore3=lJettonCount;
	else if (cbViewIndex==ID_HORSE4) m_lMeScore5=lJettonCount;
	else if (cbViewIndex==ID_HORSE5) m_lMeScore4=lJettonCount;
	else if (cbViewIndex==ID_HORSE6) m_lMeScore6=lJettonCount;
	else if (cbViewIndex==ID_HORSE7) m_lMeScore7=lJettonCount;
	else if (cbViewIndex==ID_HORSE8) m_lMeScore8=lJettonCount;
	else {ASSERT(FALSE);return;}

	//设置界面
	m_GameClientView.SetMePlaceJetton(cbViewIndex,lJettonCount);
}


LRESULT CGameClientDlg::OnBank(WPARAM wParam, LPARAM lParam)
{
	IClientKernel *pIClientKernel = (IClientKernel *)GetClientKernel(IID_IClientKernel,VER_IClientKernel);
	ASSERT(pIClientKernel!=NULL);
	if (pIClientKernel==NULL) return 0;
	pIClientKernel->ShowBankDialog( 0,0 );
	//发送消息
	SendData(SUB_C_OPEN_BANK, NULL, 0);
	return 0;
}

void CGameClientDlg::OnGetMinMaxInfo(MINMAXINFO * lpMMI)
{
	//lpMMI->ptMaxSize.x = 1024;
	//lpMMI->ptMaxSize.y = 740;
	//lpMMI->ptMaxTrackSize.x = 1024;
	//lpMMI->ptMaxTrackSize.y = 740;
	//lpMMI->ptMinTrackSize.x = 1024;
	//lpMMI->ptMinTrackSize.y = 740;

	//lpMMI->ptMaxPosition.x = 300;
	//lpMMI->ptMaxPosition.y = 300;
	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CGameClientDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//CRect rtMax;
	//GetMaxPos(rtMax);
	if (PtInRect(&rtMax,point))
	{
		return;
	}

	__super::OnLButtonUp(nFlags, point);
}
void CGameClientDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//CRect rtMax;
	//GetMaxPos(rtMax);
	if (PtInRect(&rtMax,point))
	{
		return;
	}


	__super::OnLButtonDown(nFlags, point);
}

void CGameClientDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//CRect rtMax;
	//GetMaxPos(rtMax);
	if (PtInRect(&rtMax,point))
	{
		return;
	}


	__super::OnMouseMove(nFlags, point);
}
//////////////////////////////////////////////////////////////////////////
