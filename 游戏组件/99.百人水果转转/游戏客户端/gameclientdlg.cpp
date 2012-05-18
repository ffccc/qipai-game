#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//计时器标识
#define IDI_FREE					99									//空闲时间
#define IDI_PLACE_JETTON			100									//下注时间
#define IDI_DISPATCH_CARD			301									//发牌时间

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_MESSAGE(IDM_APPLY_BANKER, OnApplyBanker)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//限制信息
	m_iMeMaxScore=0L;			
	m_iApplyBankerCondition=0L;	

	//个人下注
	ZeroMemory(m_iMeAreaScoreArray, sizeof(m_iMeAreaScoreArray));

	//庄家信息
	m_iBankerScore=0L;
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
	SetWindowText(TEXT("水果转转游戏"));

	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//加载声音
	VERIFY(m_DTSDBackground.Create(TEXT("BACK_GROUND")));
	VERIFY(m_DTSDCheer[0].Create(TEXT("CHEER1")));
	VERIFY(m_DTSDCheer[1].Create(TEXT("CHEER2")));
	VERIFY(m_DTSDCheer[2].Create(TEXT("CHEER3")));
	VERIFY(m_DTStopJetton.Create("STOP_JETTON"));
	return true;
}

//重置框架
void CGameClientDlg::ResetGameFrame()
{
	//限制信息
	m_iMeMaxScore=0L;			

	//个人下注
	ZeroMemory(m_iMeAreaScoreArray, sizeof(m_iMeAreaScoreArray));

	//庄家信息
	m_iBankerScore=0L;
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
	}

	if (IsEnableSound()) 
	{
		if (nTimerID==IDI_PLACE_JETTON&&nElapse<=5) 
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("TIME_WARIMG"));
			if( nElapse <= 0 )
			{
				PlayGameSound(AfxGetInstanceHandle(), TEXT("STOP_JETTON"));
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
			if (wDataSize!=sizeof(CMD_S_StatusFree)) 
				return false;

			//消息处理
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;
			if( pStatusFree == NULL )
				return FALSE;

			//设置时间
			SetGameTimer(GetMeChairID(),IDI_FREE,pStatusFree->cbTimeLeave);

            //玩家信息
			m_iMeMaxScore=pStatusFree->iUserMaxScore;

			m_GameClientView.SetMeMaxScore(m_iMeMaxScore);
			WORD wMeChairID=GetMeChairID();
		
			WORD wSwitchViewChairID=SwitchViewChairID(wMeChairID);
			m_GameClientView.SetMeChairID(wSwitchViewChairID);
			
            //庄家信息
			SetBankerInfo(pStatusFree->wBankerUser,pStatusFree->iBankerScore);
			m_GameClientView.SetBankerScore(pStatusFree->cbBankerTime,pStatusFree->iBankerWinScore);
			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

            //控制信息
			m_iApplyBankerCondition=pStatusFree->iApplyBankerCondition;
			m_GameClientView.SetAreaLimitScore(pStatusFree->iAreaLimitScore, JETTON_AREA_COUNT);

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
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) 
				return false;

			//消息处理
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;
			if( pStatusPlay == NULL )
				return FALSE;

			//全局下注
			for( int i=0; i<JETTON_AREA_COUNT; i++)
				m_GameClientView.PlaceUserJetton(i, pStatusPlay->iTotalAreaScore[i]);

			//玩家下注
			for( int i=0; i<JETTON_AREA_COUNT; i++ )
				SetMePlaceJetton(i, pStatusPlay->iUserAreaScore[i]);

			//玩家积分
			m_iMeMaxScore=pStatusPlay->iUserMaxScore;			
			m_GameClientView.SetMeMaxScore(m_iMeMaxScore);
			WORD wMeChairID=GetMeChairID();
			m_GameClientView.SetMeChairID(SwitchViewChairID(wMeChairID));

			//控制信息
			m_iApplyBankerCondition=pStatusPlay->iApplyBankerCondition;
			m_GameClientView.SetAreaLimitScore(pStatusPlay->iAreaScoreLimit, JETTON_AREA_COUNT);

			if (pStatusPlay->cbGameStatus==GS_GAME_END)
			{
				//设置成绩
				m_GameClientView.SetCurGameScore(pStatusPlay->iEndUserScore,pStatusPlay->iEndUserReturnScore,pStatusPlay->iEndBankerScore,pStatusPlay->iEndRevenue);
			}
			else
			{
				//播放声音
				if (IsEnableSound()) m_DTSDBackground.Play(0,true);
			}

			//庄家信息
			SetBankerInfo(pStatusPlay->wBankerUser,pStatusPlay->iBankerScore);
			m_GameClientView.SetBankerScore(pStatusPlay->cbBankerTime,pStatusPlay->iBankerWinScore);
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			//设置状态
			SetGameStatus(pStatusPlay->cbGameStatus);

			//设置时间
			SetGameTimer(GetMeChairID(),pStatusPlay->cbGameStatus==GS_GAME_END?IDI_DISPATCH_CARD:IDI_PLACE_JETTON,pStatusPlay->cbTimeLeave);

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
	if (wDataSize!=sizeof(CMD_S_GameStart)) 
		return false;

	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	if( pGameStart == NULL )
		return FALSE;

	//庄家信息
	SetBankerInfo(pGameStart->wBankerUser,pGameStart->iBankerScore);

	//玩家信息
	m_iMeMaxScore=pGameStart->iUserMaxScore;
	m_GameClientView.SetMeMaxScore(m_iMeMaxScore);

	//设置各下注区域初始可下分
	m_GameClientView.SetAreaLimitScore(pGameStart->iAreaLimitScore, JETTON_AREA_COUNT);

	//设置时间
	SetGameTimer(GetMeChairID(),IDI_PLACE_JETTON,pGameStart->cbTimeLeave);

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

	return true;
}

//游戏空闲
bool CGameClientDlg::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) 
		return false;

	//消息处理
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;
	if( pGameFree == NULL )
		return false;

	//设置时间
	SetGameTimer(GetMeChairID(),IDI_FREE,pGameFree->cbTimeLeave);

	tagGameRecord GameRecord;
	ZeroMemory(&GameRecord, sizeof(GameRecord));
	GameRecord.cbGameRecord = pGameFree->cbGameRecord;
	GameRecord.cbGameTimes = pGameFree->iGameTimes;

	m_GameClientView.m_GameRecord.FillGameRecord(&GameRecord, 1);

	//设置状态
	SetGameStatus(GS_FREE);

	//清理桌面
	m_GameClientView.SetWinnerSide(0xFF);
	m_GameClientView.CleanUserJetton();
	for (int nAreaIndex=ID_BIG_TIGER; nAreaIndex<=ID_SMALL_SNAKE; ++nAreaIndex) 
		SetMePlaceJetton(nAreaIndex,0);

	//更新控件
	UpdateButtonContron();

	return true;
}

//用户加注
bool CGameClientDlg::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) 
		return false;

	//消息处理
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;
	if( pPlaceJetton == NULL )
		return false;
	if( pPlaceJetton->cbJettonArea>ID_SMALL_SNAKE || pPlaceJetton->cbJettonArea<ID_BIG_TIGER )
		return false;

	//加注界面
	m_GameClientView.PlaceUserJetton(pPlaceJetton->cbJettonArea,pPlaceJetton->iJettonScore);

	//播放声音
	if (IsEnableSound()) 
	{
		if (pPlaceJetton->wChairID!=GetMeChairID() || IsLookonMode())
		{
			if (pPlaceJetton->iJettonScore==5000000) 
				PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
			else 
				PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
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
	if (wDataSize!=sizeof(CMD_S_GameEnd)) 
		return false;

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	if( pGameEnd == NULL )
		return FALSE;

	//取消动画
	for (WORD wAreaIndex=ID_BIG_TIGER; wAreaIndex<=ID_SMALL_SNAKE; ++wAreaIndex) 
		m_GameClientView.SetBombEffect(false,wAreaIndex);


	m_GameClientView.UpdateCartoonTimeElapse(pGameEnd->cbGoalAnimal);	//构造卡通计时器间隔数组

	//设置发牌计时器
	SetGameTimer(GetMeChairID(),IDI_DISPATCH_CARD, pGameEnd->cbTimeLeave);

	//庄家信息
	m_GameClientView.SetBankerScore(pGameEnd->nBankerTime, pGameEnd->iBankerTotallScore);

	//成绩信息
	m_GameClientView.SetCurGameScore(pGameEnd->iUserScore,pGameEnd->iUserReturnScore,pGameEnd->iBankerScore,pGameEnd->iRevenue);

	//设置状态
	SetGameStatus(GS_GAME_END);

	//更新成绩
	for (WORD wUserIndex = 0; wUserIndex < MAX_CHAIR; wUserIndex++)
	{
		tagUserData const *pUserData = GetUserData(wUserIndex);

		if ( pUserData == NULL ) 
			continue;

		tagApplyUser ApplyUser ;
		ZeroMemory(&ApplyUser, sizeof(ApplyUser));

		//更新信息
		ApplyUser.iUserScore = pUserData->lScore;
		lstrcpyn(ApplyUser.szUserName, pUserData->szName, lstrlen(pUserData->szName));
		ApplyUser.dwUserID = pUserData->dwUserID;
		m_GameClientView.m_ApplyUser.UpdateUser(ApplyUser);
	}

	//更新控件
	UpdateButtonContron();

	//停止声音
	for (int i=0; i<CountArray(m_DTSDCheer); ++i) 
		m_DTSDCheer[i].Stop();

	return true;
}

//更新控制
void CGameClientDlg::UpdateButtonContron()
{
	//置能判断
	bool bEnablePlaceJetton=true;
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) 
		bEnablePlaceJetton=false;

	if (GetGameStatus()!=GS_PLACE_JETTON) 
		bEnablePlaceJetton=false;

	if (m_wCurrentBanker==GetMeChairID()) 
		bEnablePlaceJetton=false;

	if (IsLookonMode())
		bEnablePlaceJetton=false;

	//下注按钮
	if (bEnablePlaceJetton==true)
	{
		//计算积分
		__int64 iCurrentJetton=m_GameClientView.GetCurrentJetton();
		__int64 iLeaveScore= 0;
		__int64	iTemp = 0;
		for( int i=0; i<JETTON_AREA_COUNT; i++ )
			iTemp += m_iMeAreaScoreArray[i];
		iLeaveScore = m_iMeMaxScore - iTemp;

		//设置光标
		if (iCurrentJetton>iLeaveScore)
		{
			if (iLeaveScore>=5000000L) 
				m_GameClientView.SetCurrentJetton(5000000L);
			else if (iLeaveScore>=1000000L) 
				m_GameClientView.SetCurrentJetton(1000000L);
			else if (iLeaveScore>=100000L) 
				m_GameClientView.SetCurrentJetton(100000L);
			else if (iLeaveScore>=10000L) 
				m_GameClientView.SetCurrentJetton(10000L);
			else if (iLeaveScore>=1000L) 
				m_GameClientView.SetCurrentJetton(1000L);
			else if (iLeaveScore>=100L) 
				m_GameClientView.SetCurrentJetton(100L);
			else 
				m_GameClientView.SetCurrentJetton(0L);
		}

		//控制按钮
		m_GameClientView.m_btJetton100.EnableWindow((iLeaveScore>=100)?TRUE:FALSE);
		m_GameClientView.m_btJetton1000.EnableWindow((iLeaveScore>=1000)?TRUE:FALSE);
		m_GameClientView.m_btJetton10000.EnableWindow((iLeaveScore>=10000)?TRUE:FALSE);
		m_GameClientView.m_btJetton100000.EnableWindow((iLeaveScore>=100000)?TRUE:FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow((iLeaveScore>=1000000)?TRUE:FALSE);		
		m_GameClientView.m_btJetton5000000.EnableWindow((iLeaveScore>=5000000)?TRUE:FALSE);
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
		if (m_wCurrentBanker==GetMeChairID()) 
			bEnableApply=false;

		if (m_bMeApplyBanker) 
			bEnableApply=false;

		if (pMeUserData->lScore<m_iApplyBankerCondition) 
			bEnableApply=false;

		m_GameClientView.m_btApplyBanker.EnableWindow(bEnableApply?TRUE:FALSE);

		//取消按钮
		bool bEnableCancel=true;
		if (m_wCurrentBanker==GetMeChairID() && GetGameStatus()!=GS_FREE) 
			bEnableCancel=false;

		if (m_bMeApplyBanker==false) 
			bEnableCancel=false;

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

	//控制申请上庄玩家列表
	int nApplayCount = m_GameClientView.m_ApplyUser.GetApplayBankerUserCount();
	if( nApplayCount <= MAX_SCORE_BANKER )
	{
		m_GameClientView.m_btBankerMoveUp.EnableWindow(FALSE);
		m_GameClientView.m_btBankerMoveDown.EnableWindow(FALSE);
	}
	else
	{
		m_GameClientView.m_btBankerMoveUp.EnableWindow(TRUE);
		m_GameClientView.m_btBankerMoveDown.EnableWindow(TRUE);
	}

	//控制游戏记录按钮控件
	int nRecord = m_GameClientView.m_GameRecord.GetRecordCount();
	if( nRecord <= MAX_SHOW_HISTORY )
	{
		m_GameClientView.m_btScoreMoveL.EnableWindow(FALSE);
		m_GameClientView.m_btScoreMoveR.EnableWindow(FALSE);
	}
	else
	{
		int nReadPos = m_GameClientView.m_GameRecord.GetCurReadPos();
		int nWritePos = m_GameClientView.m_GameRecord.GetCurWritePos();
		m_GameClientView.m_btScoreMoveL.EnableWindow(TRUE);
		if( nReadPos != nWritePos-1 )
			m_GameClientView.m_btScoreMoveR.EnableWindow(TRUE);
	}
	return;
}

//加注消息
void CGameClientDlg::OnPlaceJetton(BYTE iJettonArea, __int64 iJettonNum)
{
	//变量定义
	BYTE cbJettonArea=iJettonArea;			//下注区域
	__int64 iJettonScore=iJettonNum;		//筹码数量

	if( cbJettonArea > ID_SMALL_SNAKE || cbJettonArea < ID_BIG_TIGER )
		return;

	//庄家不能下注
	if ( GetMeChairID() == m_wCurrentBanker )
		return;

	//非下注状态，直接返回
	if (GetGameStatus()!=GS_PLACE_JETTON)
	{
		UpdateButtonContron();
		return;
	}

	//根据下注区域，设置本人下注筹码
	m_iMeAreaScoreArray[cbJettonArea] += iJettonScore;
	m_GameClientView.SetMePlaceJetton(cbJettonArea, m_iMeAreaScoreArray[cbJettonArea]);

	//变量定义
	CMD_C_PlaceJetton PlaceJetton;
	ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

	//构造变量
	PlaceJetton.cbJettonArea=cbJettonArea;
	PlaceJetton.iJettonScore=iJettonScore;

	//发送网络消息
	SendData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

	//更新按钮
	UpdateButtonContron();

	//播放声音
	if (IsEnableSound()) 
	{
		if (iJettonScore==5000000) 
			PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
		else 
			PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
		m_DTSDCheer[rand()%3].Play();
	}

	return;
}

void CGameClientDlg::OpenBank()
{
	//OnEventOpenBank();
	return;
}
//申请消息
LRESULT CGameClientDlg::OnApplyBanker(WPARAM wParam, LPARAM lParam)
{
	//合法判断
	tagUserData const *pMeUserData = GetUserData( GetMeChairID() );
	if( pMeUserData == NULL )
		return true;

	if (pMeUserData->lScore < m_iApplyBankerCondition) 
		return true;

	//旁观判断
	if (IsLookonMode()) 
		return true;

	//转换变量
	bool bApplyBanker = wParam ? true:false;

	//如果当前就是庄家，则不允许再次申请
	if (m_wCurrentBanker == GetMeChairID() && bApplyBanker) 
		return true;

	if (bApplyBanker)
	{
		//发送申请上庄消息
		SendData(SUB_C_APPLY_BANKER, NULL, 0);
		m_bMeApplyBanker=true;
	}
	else
	{
		//发送取消上庄消息
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
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) 
		return false;

	//消息处理
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;
	if( pApplyBanker == NULL )
		return FALSE;

	//获取玩家
	tagUserData const *pUserData=GetUserData(pApplyBanker->wApplyUser);
	if( pUserData == NULL )
		return FALSE;

	//插入玩家
	if (m_wCurrentBanker != pApplyBanker->wApplyUser)
	{
		tagApplyUser ApplyUser;
		::ZeroMemory(&ApplyUser, sizeof(ApplyUser));

		lstrcpyn(ApplyUser.szUserName, pUserData->szName, sizeof(ApplyUser.szUserName));

		ApplyUser.iUserScore=pUserData->lScore;
		ApplyUser.dwUserID = pUserData->dwUserID;
		m_GameClientView.m_ApplyUser.InserUser(ApplyUser);
	}

	//自己判断
	if (IsLookonMode()==false && GetMeChairID()==pApplyBanker->wApplyUser) 
		m_bMeApplyBanker=true;

	//更新控件
	UpdateButtonContron();

	return true;
}

//取消做庄
bool CGameClientDlg::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) 
		return false;

	//消息处理
	CMD_S_CancelBanker * pCancelBanker=(CMD_S_CancelBanker *)pBuffer;
	if( pCancelBanker == NULL )
		return FALSE;

	//删除玩家
	tagApplyUser ApplyUser;
	::ZeroMemory(&ApplyUser, sizeof(ApplyUser));
	lstrcpyn(ApplyUser.szUserName, pCancelBanker->szCancelUser, lstrlen(pCancelBanker->szCancelUser));
	ApplyUser.iUserScore=0;
	ApplyUser.dwUserID = 0xFF;
	for (WORD wUserIndex = 0; wUserIndex < MAX_CHAIR; ++wUserIndex)
	{
		tagUserData const *pUserData = GetUserData(wUserIndex);

		if ( pUserData == NULL ) 
			continue;
		if(!lstrcmpi(pUserData->szName, pCancelBanker->szCancelUser))
		{
			ApplyUser.dwUserID = pUserData->dwUserID;
			break;
		}
	}
	if( ApplyUser.dwUserID != 0xFF)
		m_GameClientView.m_ApplyUser.DeleteUser(ApplyUser);

	//自己判断
	const tagUserData *pMeUserData=GetUserData(GetMeChairID());
	if( pMeUserData == NULL )
		return FALSE;

	//本人取消做庄
	if (IsLookonMode()==false && !lstrcmp(pMeUserData->szName,pCancelBanker->szCancelUser)) 
		m_bMeApplyBanker=false;

	//更新控件
	UpdateButtonContron();

	return true;
}

//切换庄家
bool CGameClientDlg::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) 
		return false;

	//消息处理
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;
	if( pChangeBanker == NULL )
		return FALSE;

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
	SetBankerInfo(pChangeBanker->wBankerUser,pChangeBanker->iBankerScore);
	m_GameClientView.SetBankerScore(0,0);

	//删除玩家
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		tagUserData const *pBankerUserData=GetUserData(m_wCurrentBanker);
		if (pBankerUserData != NULL)
		{
			tagApplyUser ApplyUser;
			::ZeroMemory(&ApplyUser, sizeof(ApplyUser));
			lstrcpyn(ApplyUser.szUserName, pBankerUserData->szName, lstrlen(pBankerUserData->szName));
			ApplyUser.dwUserID = pBankerUserData->dwUserID;
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
	if (wDataSize%sizeof(tagServerGameRecord)!=0) 
		return false;

	if( pBuffer == NULL )
		return FALSE;

	//结果变量
	tagGameRecord GameRecord;
	ZeroMemory(&GameRecord,sizeof(GameRecord));

	//设置记录
	WORD wRecordCount=wDataSize/sizeof(tagServerGameRecord);
	for (WORD wIndex=0;wIndex<wRecordCount;wIndex++) 
	{
		tagServerGameRecord * pServerGameRecord=(((tagServerGameRecord *)pBuffer)+wIndex);
		if( pServerGameRecord == NULL )
			continue;

		GameRecord.cbGameTimes = pServerGameRecord->cbGameTimes;
		GameRecord.cbGameRecord = pServerGameRecord->cbRecord;
		m_GameClientView.SetGameHistory(&GameRecord, 1);
	}

	return true;
}

//下注失败
bool CGameClientDlg::OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJettonFail));
	if (wDataSize!=sizeof(CMD_S_PlaceJettonFail)) 
		return false;

	//消息处理
	CMD_S_PlaceJettonFail * pPlaceJettonFail=(CMD_S_PlaceJettonFail *)pBuffer;
	if( pPlaceJettonFail == NULL )
		return FALSE;

	//效验参数
	BYTE cbViewIndex=pPlaceJettonFail->lJettonArea;	//取得下注区域
	ASSERT(cbViewIndex<=ID_SMALL_SNAKE);
	if (cbViewIndex>ID_SMALL_SNAKE) 
		return false;

	//下注数量
	__int64 iJettonCount=pPlaceJettonFail->iPlaceScore;
	for( int i=0; i<JETTON_AREA_COUNT; i++ )
	{
		if( cbViewIndex == i )
		{
			m_iMeAreaScoreArray[i] -= iJettonCount;
			m_GameClientView.SetMePlaceJetton(cbViewIndex, m_iMeAreaScoreArray[i]);
		}
	}
	return true;
}

//设置庄家
void CGameClientDlg::SetBankerInfo(WORD wBanker,__int64 iScore)
{
	m_wCurrentBanker=wBanker;
	m_iBankerScore=iScore;
	WORD wBankerViewChairID=m_wCurrentBanker==INVALID_CHAIR ? INVALID_CHAIR:SwitchViewChairID(m_wCurrentBanker);
	m_GameClientView.SetBankerInfo(wBankerViewChairID,m_iBankerScore);
}

//个人下注
void CGameClientDlg::SetMePlaceJetton(BYTE cbViewIndex, __int64 iJettonCount)
{
	//效验参数
	ASSERT(cbViewIndex<= ID_SMALL_SNAKE);
	if (cbViewIndex>ID_SMALL_SNAKE) 
		return;

	for( int i=0; i<JETTON_AREA_COUNT; i++ )
	{
		if( cbViewIndex == i )
			m_iMeAreaScoreArray[i] = iJettonCount;
	}

	//设置界面
	m_GameClientView.SetMePlaceJetton(cbViewIndex,iJettonCount);
}
//////////////////////////////////////////////////////////////////////////
