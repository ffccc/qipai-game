#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include ".\gameclientdlg.h"

//////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_FREE					99									//空闲时间
#define IDI_PLACE_JETTON			100									//下注时间
#define IDI_DISPATCH_CARD			301									//发牌时间
#define IDI_PLACE_JETTON_BUFFER		302									//发牌时间

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_MESSAGE(IDM_PLACE_JETTON,OnPlaceJetton)
	ON_MESSAGE(IDM_APPLY_BANKER, OnApplyBanker)
	ON_MESSAGE(IDM_ADMIN_COMMDN,OnAdminCommand)
	ON_WM_TIMER()
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
	m_lMeTieScore=0L;
	m_lMeBankerScore=0L;
	m_lMePlayerScore=0L;
	m_lMeTieSamePointScore=0L;
	m_lMePlayerKingScore=0L;
	m_lMeBankerKingScore=0L;
	m_lMePlayerTwoPair=0L;
	m_lMeBankerTwoPair=0L;

	//庄家信息
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;

	//状态变量
	m_bMeApplyBanker=false;

	return;
}

//析构函数
CGameClientDlg::~CGameClientDlg()
{
}

//初始函数
bool CGameClientDlg::InitGameFrame()
{
	//设置标题
	SetWindowText(TEXT("天乐庄闲平游戏  --  Ver：6.6.1.0"));

	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//加载声音
	VERIFY(m_DTSDBackground.Create(TEXT("BACK_GROUND")));
	VERIFY(m_DTSDCheer[0].Create(TEXT("CHEER1")));
	VERIFY(m_DTSDCheer[1].Create(TEXT("CHEER2")));
	VERIFY(m_DTSDCheer[2].Create(TEXT("CHEER3")));

	m_PlaceJettonArray.RemoveAll();

	return true;
}

//重置框架
void CGameClientDlg::ResetGameFrame()
{
	//限制信息
	m_lMeMaxScore=0L;			

	//个人下注
	m_lMeTieScore=0L;
	m_lMeBankerScore=0L;
	m_lMePlayerScore=0L;
	m_lMeTieSamePointScore=0L;
	m_lMePlayerKingScore=0L;
	m_lMeBankerKingScore=0L;
	m_lMePlayerTwoPair=0L;
	m_lMeBankerTwoPair=0L;

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
	if ((nTimerID==IDI_PLACE_JETTON)&&(nElapse==0))
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

		//庄家按钮
		m_GameClientView.m_btApplyBanker.EnableWindow( FALSE );
		m_GameClientView.m_btCancelBanker.EnableWindow( FALSE );

		return false;
	}

	if (IsEnableSound()) 
	{
		if (nTimerID==IDI_PLACE_JETTON&&nElapse<=5) PlayGameSound(AfxGetInstanceHandle(),TEXT("TIME_WARIMG"));
	}

	if((nTimerID == IDI_DISPATCH_CARD)&&(nElapse==0)) return false;

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
			while(m_PlaceJettonArray.GetCount()>0)
			{
				CMD_S_PlaceJetton &PlaceJetton=m_PlaceJettonArray[0];
				PlaceJetton.cbAndroidUser=FALSE;
				OnSubPlaceJetton(&PlaceJetton,sizeof(PlaceJetton));
				m_PlaceJettonArray.RemoveAt(0);
			}
			KillTimer(IDI_PLACE_JETTON_BUFFER);
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
	case SUB_S_AMDIN_COMMAND:
		{
			return OnSubReqResult(pBuffer,wDataSize);
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


			if((GetUserData(GetMeChairID())->dwUserRight&UR_GAME_CONTROL)!=0)
			{
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
			}
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
			m_GameClientView.PlaceUserJetton(ID_PING_JIA,pStatusPlay->lAllTieScore);
			m_GameClientView.PlaceUserJetton(ID_TONG_DIAN_PING,pStatusPlay->lAllTieSamePointScore);
			m_GameClientView.PlaceUserJetton(ID_XIAN_JIA,pStatusPlay->lAllPlayerScore);
			m_GameClientView.PlaceUserJetton(ID_XIAN_TIAN_WANG,pStatusPlay->lAllPlayerKingScore);
			m_GameClientView.PlaceUserJetton(ID_ZHUANG_JIA,pStatusPlay->lAllBankerScore);
			m_GameClientView.PlaceUserJetton(ID_ZHUANG_TIAN_WANG,pStatusPlay->lAllBankerKingScore);
			m_GameClientView.PlaceUserJetton(ID_PLAYER_TWO_PAIR,pStatusPlay->lAllPlayerTwoPair);
			m_GameClientView.PlaceUserJetton(ID_BANKER_TWO_PAIR,pStatusPlay->lAllBankerTwoPair);

			//玩家下注
			SetMePlaceJetton(ID_PING_JIA,pStatusPlay->lUserTieScore);
			SetMePlaceJetton(ID_TONG_DIAN_PING,pStatusPlay->lUserTieSamePointScore);
			SetMePlaceJetton(ID_XIAN_JIA,pStatusPlay->lUserPlayerScore);
			SetMePlaceJetton(ID_XIAN_TIAN_WANG,pStatusPlay->lUserPlayerKingScore);
			SetMePlaceJetton(ID_ZHUANG_JIA,pStatusPlay->lUserBankerScore);
			SetMePlaceJetton(ID_ZHUANG_TIAN_WANG,pStatusPlay->lUserBankerKingScore);
			SetMePlaceJetton(ID_PLAYER_TWO_PAIR,pStatusPlay->lUserPlayerTwoPair);
			SetMePlaceJetton(ID_BANKER_TWO_PAIR,pStatusPlay->lUserBankerTwoPair);

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
				m_GameClientView.SetCardInfo(pStatusPlay->cbCardCount,pStatusPlay->cbTableCardArray);
				m_GameClientView.FinishDispatchCard(true);

				//设置成绩
				m_GameClientView.SetCurGameScore(pStatusPlay->lEndUserScore,pStatusPlay->lEndUserReturnScore,pStatusPlay->lEndBankerScore,pStatusPlay->lEndRevenue);
			}
			else
			{
				m_GameClientView.SetCardInfo(NULL,NULL);

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
			SetGameTimer(GetMeChairID(),pStatusPlay->cbGameStatus==GS_GAME_END?IDI_DISPATCH_CARD:IDI_PLACE_JETTON,pStatusPlay->cbTimeLeave);

			if((GetUserData(GetMeChairID())->dwUserRight&UR_GAME_CONTROL)!=0)
			{
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
			}
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

	//设置时间
	WORD wMeChair = GetMeChairID();
	SetGameTimer(wMeChair,IDI_PLACE_JETTON,pGameStart->cbTimeLeave);

	//设置状态
	SetGameStatus(GS_PLACE_JETTON);

	//更新控制
	UpdateButtonContron();

	//更新界面
	m_GameClientView.UpdateGameView(NULL);

	//播放声音
	if (IsEnableSound()) 
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
		m_DTSDBackground.Play(0,true);
	}

	m_PlaceJettonArray.RemoveAll();
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

	//设置状态
	SetGameStatus(GS_FREE);

	//清理桌面
	m_GameClientView.SetWinnerSide(0xFF);
	m_GameClientView.CleanUserJetton();
	for (int nAreaIndex=ID_XIAN_JIA; nAreaIndex<=ID_BANKER_TWO_PAIR; ++nAreaIndex) SetMePlaceJetton(nAreaIndex,0);

	//更新控件
	UpdateButtonContron();

	//完成发牌
	m_GameClientView.FinishDispatchCard();

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

	//缓冲判断
	if (pPlaceJetton->cbAndroidUser)
	{
		m_PlaceJettonArray.Add(*pPlaceJetton);
		if (m_PlaceJettonArray.GetCount()==1) SetTimer(IDI_PLACE_JETTON_BUFFER,70,NULL);
		return true;
	}

	if (GetMeChairID()!=pPlaceJetton->wChairID || IsLookonMode())
	{
		//加注界面
		m_GameClientView.PlaceUserJetton(pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);

		//播放声音
		if (IsEnableSound()) 
		{
			if (pPlaceJetton->wChairID!=GetMeChairID() || IsLookonMode())
			{
				if (pPlaceJetton->lJettonScore==5000000) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
				else PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
				m_DTSDCheer[rand()%3].Play();
			}
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
	for (WORD wAreaIndex=ID_XIAN_JIA; wAreaIndex<=ID_BANKER_TWO_PAIR; ++wAreaIndex) m_GameClientView.SetBombEffect(false,wAreaIndex);

	//设置时间
	SetGameTimer(GetMeChairID(),IDI_DISPATCH_CARD, pGameEnd->cbTimeLeave);

	//扑克信息
	m_GameClientView.SetCardInfo(pGameEnd->cbCardCount,pGameEnd->cbTableCardArray);
	m_GameClientView.ClearAreaFlash();

	//庄家信息
	m_GameClientView.SetBankerScore(pGameEnd->nBankerTime, pGameEnd->lBankerTotallScore);

	//成绩信息
	m_GameClientView.SetCurGameScore(pGameEnd->lUserScore,pGameEnd->lUserReturnScore,pGameEnd->lBankerScore,pGameEnd->lRevenue);

	//设置状态
	SetGameStatus(GS_GAME_END);

	//更新控件
	UpdateButtonContron();

	//停止声音
	for (int i=0; i<CountArray(m_DTSDCheer); ++i) m_DTSDCheer[i].Stop();

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
		LONGLONG lMeLeaveScore=m_lMeMaxScore-m_lMeTieScore-m_lMeBankerScore-m_lMePlayerScore-m_lMeBankerKingScore-m_lMePlayerKingScore-
			m_lMeTieSamePointScore-m_lMePlayerTwoPair-m_lMeBankerTwoPair;
		LONGLONG lBankerLeaveScore=m_lBankerScore-m_GameClientView.GetTotalChip();
		LONGLONG lLeaveScore = (m_lBankerScore==0?lMeLeaveScore:__min(lMeLeaveScore, lBankerLeaveScore));

		//设置光标
		if (lCurrentJetton>lLeaveScore)
		{
			if (lLeaveScore>=5000000L) m_GameClientView.SetCurrentJetton(5000000L);
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
		m_GameClientView.m_btCancelBanker.SetButtonImage(m_wCurrentBanker==GetMeChairID()?IDB_BT_CANCEL_BANKER:IDB_BT_CANCEL_APPLY,AfxGetInstanceHandle(),false);

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

#ifdef __SPECIAL___
	//银行按钮
	m_GameClientView.m_btBankerDraw.EnableWindow(TRUE);
	m_GameClientView.m_btBankerStorage.EnableWindow(FALSE);
	m_GameClientView.m_btBankerStorage.ShowWindow(SW_SHOW);
	m_GameClientView.m_btBankerStorage.EnableWindow((GetGameStatus()==GS_FREE || IsLookonMode())?TRUE:FALSE);
	if(m_wCurrentBanker==GetMeChairID()&&IsLookonMode()==false)
	{
		m_GameClientView.m_btBankerStorage.EnableWindow(FALSE);
	}
	if (m_wCurrentBanker==GetMeChairID()) m_GameClientView.m_blCanStore = false;
	else  m_GameClientView.m_blCanStore = true;
	if(m_wCurrentBanker==GetMeChairID()) m_GameClientView.m_DlgBank.m_blCanGetOnlyFree = false;
	else m_GameClientView.m_DlgBank.m_blCanGetOnlyFree = false;
	m_GameClientView.m_DlgBank.m_blCanStore = m_GameClientView.m_blCanStore;

	if(m_GameClientView.m_DlgBank.m_hWnd !=NULL)
	{
		m_GameClientView.m_DlgBank.ShowItem();
	}

#endif

	return;
}

//加注消息
LRESULT CGameClientDlg::OnPlaceJetton(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	BYTE cbJettonArea=(BYTE)wParam;
	LONGLONG lJettonScore= m_GameClientView.GetCurrentJetton();

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
		case ID_XIAN_JIA:
			{
				m_lMePlayerScore += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_XIAN_JIA,m_lMePlayerScore);
				break;
			}
		case ID_PING_JIA:
			{
				m_lMeTieScore += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_PING_JIA,m_lMeTieScore);
				break;
			}
		case ID_ZHUANG_JIA:
			{
				m_lMeBankerScore += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_ZHUANG_JIA,m_lMeBankerScore);
				break;
			}
		case ID_TONG_DIAN_PING:
			{
				m_lMeTieSamePointScore += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_TONG_DIAN_PING,m_lMeTieSamePointScore);
				break;
			}
		case ID_XIAN_TIAN_WANG:
			{
				m_lMePlayerKingScore += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_XIAN_TIAN_WANG,m_lMePlayerKingScore);
				break;
			}
		case ID_ZHUANG_TIAN_WANG:
			{
				m_lMeBankerKingScore += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_ZHUANG_TIAN_WANG,m_lMeBankerKingScore);
				break;
			}
		case ID_PLAYER_TWO_PAIR:
			{
				m_lMePlayerTwoPair += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_PLAYER_TWO_PAIR,m_lMePlayerTwoPair);
				break;
			}
		case ID_BANKER_TWO_PAIR:
			{
				m_lMeBankerTwoPair += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_BANKER_TWO_PAIR,m_lMeBankerTwoPair);
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

	//预先显示
	m_GameClientView.PlaceUserJetton(cbJettonArea,lJettonScore);

	//播放声音
	if (IsEnableSound()) 
	{
		if (lJettonScore==5000000) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
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
	bool bApplyBanker = (wParam!=0) ? true:false;

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
		m_GameClientView.m_ApplyUser.m_AppyUserList.Invalidate(TRUE);
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
	CMD_S_CancelBanker * pCancelBanker=(CMD_S_CancelBanker *)pBuffer;

	//删除玩家
	tagApplyUser ApplyUser;
	ApplyUser.strUserName=pCancelBanker->szCancelUser;
	ApplyUser.lUserScore=0;
	m_GameClientView.m_ApplyUser.DeleteUser(ApplyUser);
	m_GameClientView.m_ApplyUser.m_AppyUserList.Invalidate(TRUE);

	//自己判断
	const tagUserData *pMeUserData=GetUserData(GetMeChairID());
	if (IsLookonMode()==false && lstrcmp(pMeUserData->szName,pCancelBanker->szCancelUser)==0) m_bMeApplyBanker=false;

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
			m_GameClientView.m_ApplyUser.m_AppyUserList.Invalidate(TRUE);
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

	//结果变量
	tagGameRecord GameRecord;
	ZeroMemory(&GameRecord,sizeof(GameRecord));

	//设置记录
	WORD wRecordCount=wDataSize/sizeof(tagServerGameRecord);
	for (WORD wIndex=0;wIndex<wRecordCount;wIndex++) 
	{
		tagServerGameRecord * pServerGameRecord=(((tagServerGameRecord *)pBuffer)+wIndex);

		m_GameClientView.SetGameHistory(enOperateResult_NULL, pServerGameRecord->cbPlayerCount, pServerGameRecord->cbBankerCount,
			pServerGameRecord->cbKingWinner,pServerGameRecord->bPlayerTwoPair,pServerGameRecord->bBankerTwoPair);
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
	ASSERT(cbViewIndex<=ID_BANKER_TWO_PAIR);
	if (cbViewIndex>ID_BANKER_TWO_PAIR) return false;

	//自己判断
	if (GetMeChairID()==pPlaceJettonFail->wPlaceUser && false==IsLookonMode())
	{
		//加注界面
		m_GameClientView.PlaceUserJetton(pPlaceJettonFail->lJettonArea,-pPlaceJettonFail->lPlaceScore);

		LONGLONG lJettonCount=pPlaceJettonFail->lPlaceScore;
		if (cbViewIndex==ID_XIAN_JIA)
		{
			//合法校验
			ASSERT(m_lMePlayerScore>=lJettonCount);
			if (lJettonCount>m_lMePlayerScore) return false;

			m_lMePlayerScore-=lJettonCount;
			m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMePlayerScore);
		}
		else if (cbViewIndex==ID_PING_JIA) 
		{
			//合法校验
			ASSERT(m_lMeTieScore>=lJettonCount);
			if (lJettonCount>m_lMeTieScore) return false;

			m_lMeTieScore-=lJettonCount;
			m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeTieScore);
		}
		else if (cbViewIndex==ID_ZHUANG_JIA) 
		{
			//合法校验
			ASSERT(m_lMeBankerScore>=lJettonCount);
			if (lJettonCount>m_lMeBankerScore) return false;

			m_lMeBankerScore-=lJettonCount;
			m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeBankerScore);
		}
		else if (cbViewIndex==ID_XIAN_TIAN_WANG) 
		{
			//合法校验
			ASSERT(m_lMePlayerKingScore>=lJettonCount);
			if (lJettonCount>m_lMePlayerKingScore) return false;

			m_lMePlayerKingScore-=lJettonCount;
			m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMePlayerKingScore);
		}
		else if (cbViewIndex==ID_ZHUANG_TIAN_WANG) 
		{
			//合法校验
			ASSERT(m_lMeBankerKingScore>=lJettonCount);
			if (lJettonCount>m_lMeBankerKingScore) return false;

			m_lMeBankerKingScore-=lJettonCount;
			m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeBankerKingScore);
		}
		else if (cbViewIndex==ID_TONG_DIAN_PING) 
		{
			//合法校验
			ASSERT(m_lMeTieSamePointScore>=lJettonCount);
			if (lJettonCount>m_lMeTieSamePointScore) return false;

			m_lMeTieSamePointScore-=lJettonCount;
			m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeTieSamePointScore);
		}
		else if (cbViewIndex==ID_BANKER_TWO_PAIR) 
		{
			//合法校验
			ASSERT(m_lMeBankerTwoPair>=lJettonCount);
			if (lJettonCount>m_lMeBankerTwoPair) return false;

			m_lMeBankerTwoPair-=lJettonCount;
			m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeBankerTwoPair);
		}
		else if (cbViewIndex==ID_PLAYER_TWO_PAIR) 
		{
			//合法校验
			ASSERT(m_lMePlayerTwoPair>=lJettonCount);
			if (lJettonCount>m_lMePlayerTwoPair) return false;

			m_lMePlayerTwoPair-=lJettonCount;
			m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMePlayerTwoPair);
		}
		else {ASSERT(FALSE);return false;}
	}

	return true;
}

//设置庄家
void CGameClientDlg::SetBankerInfo(WORD wBanker,LONGLONG lScore)
{
	m_wCurrentBanker=wBanker;
	m_lBankerScore=lScore;
	WORD wBankerViewChairID=(m_wCurrentBanker==INVALID_CHAIR) ? INVALID_CHAIR:SwitchViewChairID(m_wCurrentBanker);
	m_GameClientView.SetBankerInfo(wBankerViewChairID,m_lBankerScore);
}

//个人下注
void CGameClientDlg::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	//效验参数
	ASSERT(cbViewIndex<=ID_BANKER_TWO_PAIR);
	if (cbViewIndex>ID_BANKER_TWO_PAIR) return;

	if (cbViewIndex==ID_XIAN_JIA) m_lMePlayerScore=lJettonCount;
	else if (cbViewIndex==ID_PING_JIA) m_lMeTieScore=lJettonCount;
	else if (cbViewIndex==ID_ZHUANG_JIA) m_lMeBankerScore=lJettonCount;
	else if (cbViewIndex==ID_XIAN_TIAN_WANG) m_lMePlayerKingScore=lJettonCount;
	else if (cbViewIndex==ID_ZHUANG_TIAN_WANG) m_lMeBankerKingScore=lJettonCount;
	else if (cbViewIndex==ID_TONG_DIAN_PING) m_lMeTieSamePointScore=lJettonCount;
	else if (cbViewIndex==ID_BANKER_TWO_PAIR) m_lMeBankerTwoPair=lJettonCount;
	else if (cbViewIndex==ID_PLAYER_TWO_PAIR) m_lMePlayerTwoPair=lJettonCount;
	else {ASSERT(FALSE);return;}

	//设置界面
	m_GameClientView.SetMePlaceJetton(cbViewIndex,lJettonCount);
}
//////////////////////////////////////////////////////////////////////////
LRESULT CGameClientDlg::OnAdminCommand(WPARAM wParam,LPARAM lParam)
{
	SendData(SUB_C_AMDIN_COMMAND,(CMD_C_AdminReq*)wParam,sizeof(CMD_C_AdminReq));
	return true;
}

bool CGameClientDlg::OnSubReqResult(const void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize==sizeof(CMD_S_CommandResult));
	if(wDataSize!=sizeof(CMD_S_CommandResult)) return false;

	m_GameClientView.m_AdminDlg.ReqResult(pBuffer);
	return true;
}
void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (IDI_PLACE_JETTON_BUFFER==nIDEvent)
	{
		if (m_PlaceJettonArray.GetCount()>0)
		{
			CMD_S_PlaceJetton &PlaceJetton=m_PlaceJettonArray[0];
			PlaceJetton.cbAndroidUser=FALSE;
			OnSubPlaceJetton(&PlaceJetton,sizeof(PlaceJetton));
			m_PlaceJettonArray.RemoveAt(0);

			if(m_PlaceJettonArray.GetCount()>30)
			{
				CMD_S_PlaceJetton &PlaceJetton=m_PlaceJettonArray[0];
				PlaceJetton.cbAndroidUser=FALSE;
				OnSubPlaceJetton(&PlaceJetton,sizeof(PlaceJetton));
				m_PlaceJettonArray.RemoveAt(0);
			}
		}

		if (m_PlaceJettonArray.GetCount()==0) KillTimer(IDI_PLACE_JETTON_BUFFER);
		return;
	}

	CGameFrameDlg::OnTimer(nIDEvent);
}
