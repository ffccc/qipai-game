#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//常量定义
#define SEND_COUNT					500									//发送次数

//索引定义
#define INDEX_PLAYER				0									//闲家索引
#define INDEX_BANKER				1									//庄家索引

//下注时间
#define IDI_FREE					1									//空闲时间
#define TIME_FREE					10									//空闲时间

//下注时间
#define IDI_PLACE_JETTON			2									//下注时间

#define TIME_PLACE_JETTON			18									//下注时间

//结束时间
#define IDI_GAME_END				3									//结束时间

#define TIME_GAME_END				30									//结束时间

//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount = GAME_PLAYER;				//游戏人数
const enStartMode	CTableFrameSink::m_GameStartMode = enStartMode_TimeControl;	//开始模式
CMD_S_Win_Rate_Info CTableFrameSink::m_CtrlInfo = {0};	//控帛赢家

//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{
	//总下注数
	ZeroMemory(m_lAllJettonScore, sizeof(m_lAllJettonScore));
	ZeroMemory(m_UserPlaceJetton, sizeof(m_UserPlaceJetton));
	//个人下注
	ZeroMemory(m_lUserJettonScore, sizeof(m_lUserJettonScore));

	//控制变量
	m_cbWinSideControl = 0;
	m_nSendCardCount = 0;

	//玩家成绩
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));

	//扑克信息
	ZeroMemory(m_cbCardCount, sizeof(m_cbCardCount));
	ZeroMemory(m_cbTableCardArray, sizeof(m_cbTableCardArray));

	//状态变量
	m_dwJettonTime = 0L;

	//庄家信息
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker = INVALID_CHAIR;
	m_wBankerTime = 0;
	m_lBankerWinScore = 0L;
	m_lBankerCurGameScore = 0L;
	m_bEnableSysBanker = true;
	m_cbLeftCardCount = 0;
	m_bContiueCard = false;
	m_UserLeftScore = 0;
	m_PlayCount = 0;
	m_WinCount = 0;
	memset(&m_CtrlInfo, 0, sizeof(CMD_S_Win_Rate_Info));

	//记录变量
	ZeroMemory(m_GameRecordArrary, sizeof(m_GameRecordArrary));
	m_nRecordFirst = 0;
	m_nRecordLast = 0;
	m_dwRecordCount = 0;

	//设置文件名
	TCHAR szPath[MAX_PATH] = TEXT("");
	GetCurrentDirectory(sizeof(szPath), szPath);
	_snprintf(m_szConfigFileName, sizeof(m_szConfigFileName), TEXT("%s\\Baccarat28GConfig.ini"), szPath);

	LONGLONG lMaxScore = 2147483647;
	TCHAR OutBuf[255];


	TCHAR Session[255];
	memset(Session, 0, 255);
	_snprintf(Session, sizeof(Session), TEXT("BankerCondition"));
	GetPrivateProfileString(Session, TEXT("Score"), "10000", OutBuf, 255, m_szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), "%I64d", &m_lApplyBankerCondition);

	BYTE cbEnableSysBanker = GetPrivateProfileInt(Session, TEXT("EnableSysBanker"), 1, m_szConfigFileName);
	m_bEnableSysBanker = cbEnableSysBanker ? true : false;

	memset(Session, 0, 255);
	_snprintf(Session, sizeof(Session), TEXT("ScoreLimit"));
	GetPrivateProfileString(Session, TEXT("AreaLimitScore"), "100000000", OutBuf, 255, m_szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), "%I64d", &m_lAreaLimitScore);

	memset(OutBuf, 0, 255);
	GetPrivateProfileString(Session, TEXT("UserLimitScore"), "100000000", OutBuf, 255, m_szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), "%I64d", &m_lUserLimitScore);

	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
}

//接口查询
void * __cdecl CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink, Guid, dwQueryVer);
	QUERYINTERFACE(ITableUserAction, Guid, dwQueryVer);
#ifdef __SPECIAL___
#ifdef _DEBUG
	QUERYINTERFACE(ITableUserActionEX, Guid, dwQueryVer);
#else
	QUERYINTERFACE(ITableUserActionEX, Guid, dwQueryVer);
#endif
#endif
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink, Guid, dwQueryVer);
	return NULL;
}

//初始化
bool __cdecl CTableFrameSink::InitTableFrameSink(IUnknownEx * pIUnknownEx)
{
	//查询接口
	ASSERT(pIUnknownEx != NULL);
	m_pITableFrame = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, ITableFrame);
	if (m_pITableFrame == NULL) return false;

	//控制接口
	m_pITableFrameControl = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, ITableFrameControl);
	if (m_pITableFrameControl == NULL) return false;

	//获取参数
	m_pGameServiceOption = m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption != NULL);

	return true;
}

//复位桌子
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//总下注数
	ZeroMemory(m_lAllJettonScore, sizeof(m_lAllJettonScore));

	ZeroMemory(m_UserPlaceJetton, sizeof(m_UserPlaceJetton));

	//个人下注
	ZeroMemory(m_lUserJettonScore, sizeof(m_lUserJettonScore));

	//玩家成绩
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	m_UserLeftScore = 0;

	return;
}

//开始模式
enStartMode __cdecl CTableFrameSink::GetGameStartMode()
{
	return m_GameStartMode;
}

//游戏状态
bool __cdecl CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}

//游戏开始
bool __cdecl CTableFrameSink::OnEventGameStart()
{
	static int nCount = 0;
	nCount ++;
	if (nCount >= 1000)
	{
		exit(0);
	}
	//变量定义
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart, sizeof(GameStart));

	//获取庄家
	IServerUserItem *pIBankerServerUserItem = NULL;
	if (INVALID_CHAIR != m_wCurrentBanker)
		pIBankerServerUserItem = m_pITableFrame->GetServerUserItem(m_wCurrentBanker);

	//设置变量
	GameStart.cbTimeLeave = TIME_PLACE_JETTON;
	GameStart.wBankerUser = m_wCurrentBanker;
	if (pIBankerServerUserItem != NULL)
		GameStart.lBankerScore = pIBankerServerUserItem->GetUserScore()->lScore;
	GameStart.bContiueCard = m_bContiueCard;

	//旁观玩家
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_START, &GameStart, sizeof(GameStart));

	//游戏玩家
	for (WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID)
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);
		if (pIServerUserItem == NULL) continue;

		//设置积分
		GameStart.lUserMaxScore = min(pIServerUserItem->GetUserScore()->lScore, m_lUserLimitScore);

		m_pITableFrame->SendTableData(wChairID, SUB_S_GAME_START, &GameStart, sizeof(GameStart));
	}

	return true;
}

//游戏结束
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	static int nCount = 0;
	nCount ++;

	switch (cbReason)
	{
		case GER_NORMAL:		//常规结束
		{
			//派发扑克
			DispatchTableCard();

			//计算分数
			LONGLONG lBankerWinScore = CalculateScore();

			//递增次数
			m_wBankerTime++;

			//结束消息
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd, sizeof(GameEnd));

			//庄家信息
			GameEnd.nBankerTime = m_wBankerTime;
			GameEnd.lBankerTotallScore = m_lBankerWinScore;
			GameEnd.lBankerScore = lBankerWinScore;

			GameEnd.bcFirstCard[0] = m_bcFirstPostCard[0];		//骰子点数
			GameEnd.bcFirstCard[1] = m_bcFirstPostCard[1];		//骰子点数

			//扑克信息
			CopyMemory(GameEnd.cbTableCardArray, m_cbTableCardArray, sizeof(m_cbTableCardArray));
			GameEnd.cbLeftCardCount = m_cbLeftCardCount;

			//发送积分
			GameEnd.cbTimeLeave = TIME_GAME_END;
			for (WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex)
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserIndex);
				if (pIServerUserItem == NULL) continue;

				//设置成绩
				GameEnd.lUserScore = m_lUserWinScore[wUserIndex];

				//返还积分
				GameEnd.lUserReturnScore = m_lUserReturnScore[wUserIndex];

				//设置税收
				if (m_lUserRevenue[wUserIndex] > 0) GameEnd.lRevenue = m_lUserRevenue[wUserIndex];
				else if (m_wCurrentBanker != INVALID_CHAIR) GameEnd.lRevenue = m_lUserRevenue[m_wCurrentBanker];
				else GameEnd.lRevenue = 0;

				//发送消息
				m_pITableFrame->SendTableData(wUserIndex, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));
				m_pITableFrame->SendLookonData(wUserIndex, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));
			}

			return true;
		}
		case GER_USER_LEFT:		//用户离开
		{
			//闲家判断
			if (m_wCurrentBanker != wChairID)
			{
				//变量定义
				LONGLONG lScore = 0;
				LONGLONG lRevenue = 0;
				enScoreKind ScoreKind = enScoreKind_Flee;

				//统计成绩
				for (int nAreaIndex = 1; nAreaIndex <= AREA_COUNT; ++nAreaIndex) lScore -= m_lUserJettonScore[nAreaIndex][wChairID];

				//写入积分
				if (m_pITableFrame->GetGameStatus() != GS_GAME_END)
				{
					if (lScore != 0L)
					{
						m_pITableFrame->WriteUserScore(pIServerUserItem, lScore, lRevenue, ScoreKind);
					}
					/*LONGLONG ScoreBank = -lScore;
					float Revenue = float(80.0/100.0);
					ScoreBank = LONGLONG(ScoreBank*Revenue);
					m_UserLeftScore +=  ScoreBank;*/
					for (int n = 0; n <= AREA_COUNT; n++)
					{
						m_lUserJettonScore[n][wChairID] = 0;
					}
				}
				else
				{
					//胜利类型 结束状态逃跑
					enScoreKind ScoreKind = (m_lUserWinScore[wChairID] > 0L) ? enScoreKind_Win : enScoreKind_Lost;

					if (m_lUserWinScore[wChairID] != 0L)
					{
						m_pITableFrame->WriteUserScore(pIServerUserItem, m_lUserWinScore[wChairID], m_lUserReturnScore[wChairID], ScoreKind);

						//用户分保存 如果重入则加入地主
						m_UserLeftScore +=  m_lUserWinScore[wChairID];
						m_lUserWinScore[wChairID] = 0;
						m_lUserReturnScore[wChairID] = 0;
					}
				}
				return true;
			}

			//状态判断
			if (m_pITableFrame->GetGameStatus() != GS_GAME_END)
			{
				//提示消息
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg, CountArray(szTipMsg), TEXT("由于庄家[ %s ]强退，游戏提前结束！"), pIServerUserItem->GetAccounts());

				//发送消息
				SendGameMessage(INVALID_CHAIR, szTipMsg);

				//设置状态
				m_pITableFrame->SetGameStatus(GS_GAME_END);

				//设置时间
				m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
				m_pITableFrame->SetGameTimer(IDI_GAME_END, TIME_GAME_END*1000, 1, 0L);
				//派发扑克
				DispatchTableCard();

				//计算分数
				CalculateScore();

				//结束消息
				CMD_S_GameEnd GameEnd;
				ZeroMemory(&GameEnd, sizeof(GameEnd));

				//庄家信息
				GameEnd.nBankerTime = m_wBankerTime;
				GameEnd.lBankerTotallScore = m_lBankerWinScore;
				if (m_lBankerWinScore > 0) GameEnd.lBankerScore = 0;

				//扑克信息
				CopyMemory(GameEnd.cbTableCardArray, m_cbTableCardArray, sizeof(m_cbTableCardArray));
				GameEnd.cbLeftCardCount = m_cbLeftCardCount;

				//发送积分
				GameEnd.cbTimeLeave = TIME_GAME_END;
				for (WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex)
				{
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserIndex);
					if (pIServerUserItem == NULL) continue;

					//设置成绩
					GameEnd.lUserScore = m_lUserWinScore[wUserIndex];

					//返还积分
					GameEnd.lUserReturnScore = m_lUserReturnScore[wUserIndex];

					//设置税收
					if (m_lUserRevenue[wUserIndex] > 0) GameEnd.lRevenue = m_lUserRevenue[wUserIndex];
					else if (m_wCurrentBanker != INVALID_CHAIR) GameEnd.lRevenue = m_lUserRevenue[m_wCurrentBanker];
					else GameEnd.lRevenue = 0;

					//发送消息
					m_pITableFrame->SendTableData(wUserIndex, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));
					m_pITableFrame->SendLookonData(wUserIndex, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));
				}
			}

			//扣除分数
			if (m_lUserWinScore[m_wCurrentBanker] < 0)
			{

				m_pITableFrame->WriteUserScore(m_wCurrentBanker, m_lUserWinScore[m_wCurrentBanker], (LONG)m_lUserRevenue[m_wCurrentBanker], enScoreKind_Flee);

			}
			else
			{
				//if(m_lUserWinScore[m_wCurrentBanker]>0)
				//{
				//	LONGLONG ScoreBank = m_lUserWinScore[m_wCurrentBanker];
				//	IServerUserItem *pBankIServerUserItem = m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
				//	//写入积分
				//	float Revenue = float(80.0/100.0);
				//	ScoreBank = LONGLONG(ScoreBank*Revenue);
				//	if (ScoreBank!=0L) m_pITableFrame->WriteUserScore(pBankIServerUserItem, ScoreBank,m_lUserRevenue[m_wCurrentBanker], enScoreKind_Flee);

				//	if(pIServerUserItem->GetUserScore()->lScore<0)
				//	{
				//

				//	}

				//}
			}
			//已经写分庄清0 避免在重入之后再在时间结束后再写一次
			m_lUserWinScore[m_wCurrentBanker] = 0;
			//切换庄家
			ChangeBanker(true);

			return true;
		}
	}

	return false;
}

//发送场景
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
		case GS_FREE:			//空闲状态
		{
			//发送记录
			SendGameRecord(pIServerUserItem);

			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree, sizeof(StatusFree));

			//控制信息
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusFree.lAreaLimitScore = m_lAreaLimitScore;

			//庄家信息
			StatusFree.bEnableSysBanker = m_bEnableSysBanker;
			StatusFree.wBankerUser = m_wCurrentBanker;
			StatusFree.cbBankerTime = m_wBankerTime;
			StatusFree.lBankerWinScore = m_lBankerWinScore;
			if (m_wCurrentBanker != INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
				StatusFree.lBankerScore = pIServerUserItem->GetUserScore()->lScore;
			}

			//玩家信息
			if (pIServerUserItem->GetUserStatus() != US_LOOKON)
			{
				StatusFree.lUserMaxScore = min(pIServerUserItem->GetUserScore()->lScore, m_lUserLimitScore);
			}

			//全局信息
			DWORD dwPassTime = (DWORD)time(NULL) - m_dwJettonTime;
			StatusFree.cbTimeLeave = (BYTE)(TIME_FREE - __min(dwPassTime, TIME_FREE));

			//发送场景
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem, &StatusFree, sizeof(StatusFree));

			//限制提示
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg, CountArray(szTipMsg), TEXT("本房间上庄条件为：%I64d,区域限制为：%I64d,玩家限制为：%I64d"), m_lApplyBankerCondition,
			           m_lAreaLimitScore, m_lUserLimitScore);

			m_pITableFrame->SendGameMessage(pIServerUserItem, szTipMsg, SMT_INFO);

			//发送申请者
			SendApplyUser(pIServerUserItem);

			return bSuccess;
		}
		case GS_PLACE_JETTON:		//游戏状态
		case GS_GAME_END:			//结束状态
		{
			//发送记录
			SendGameRecord(pIServerUserItem);

			//构造数据
			CMD_S_StatusPlay StatusPlay = {0};

			//全局下注
			CopyMemory(StatusPlay.lAllJettonScore, m_lAllJettonScore, sizeof(StatusPlay.lAllJettonScore));

			//玩家下注
			if (pIServerUserItem->GetUserStatus() != US_LOOKON)
			{
				for (int nAreaIndex = 1; nAreaIndex <= AREA_COUNT; ++nAreaIndex)
					StatusPlay.lUserJettonScore[nAreaIndex] = m_lUserJettonScore[nAreaIndex][wChiarID];

				//最大下注
				StatusPlay.lUserMaxScore = min(pIServerUserItem->GetUserScore()->lScore, m_lUserLimitScore);
			}

			//控制信息
			StatusPlay.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusPlay.lAreaLimitScore = m_lAreaLimitScore;

			//庄家信息
			StatusPlay.bEnableSysBanker = m_bEnableSysBanker;
			StatusPlay.wBankerUser = m_wCurrentBanker;
			StatusPlay.cbBankerTime = m_wBankerTime;
			StatusPlay.lBankerWinScore = m_lBankerWinScore;
			if (m_wCurrentBanker != INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
				StatusPlay.lBankerScore = pIServerUserItem->GetUserScore()->lScore;
			}

			//全局信息
			DWORD dwPassTime = (DWORD)time(NULL) - m_dwJettonTime;
			StatusPlay.cbTimeLeave = (BYTE)(TIME_FREE - __min(dwPassTime, TIME_FREE));
			StatusPlay.cbGameStatus = m_pITableFrame->GetGameStatus();

			//结束判断
			if (cbGameStatus == GS_GAME_END)
			{
				//设置成绩
				StatusPlay.lEndUserScore = m_lUserWinScore[wChiarID];

				//返还积分
				StatusPlay.lEndUserReturnScore = m_lUserReturnScore[wChiarID];

				//设置税收
				if (m_lUserRevenue[wChiarID] > 0) StatusPlay.lEndRevenue = m_lUserRevenue[wChiarID];
				else if (m_wCurrentBanker != INVALID_CHAIR) StatusPlay.lEndRevenue = m_lUserRevenue[m_wCurrentBanker];
				else StatusPlay.lEndRevenue = 0;

				//庄家成绩
				StatusPlay.lEndBankerScore = m_lBankerCurGameScore;

				//扑克信息
				CopyMemory(StatusPlay.cbTableCardArray, m_cbTableCardArray, sizeof(m_cbTableCardArray));
			}
			else
			{

			}

			//限制提示
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg, CountArray(szTipMsg), TEXT("本房间上庄条件为：%I64d,区域限制为：%I64d,玩家限制为：%I64d"), m_lApplyBankerCondition,
				m_lAreaLimitScore, m_lUserLimitScore);

			m_pITableFrame->SendGameMessage(pIServerUserItem, szTipMsg, SMT_INFO);

			//发送申请者
			SendApplyUser(pIServerUserItem);

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem, &StatusPlay, sizeof(StatusPlay));
		}
	}

	return false;
}

//定时器事件
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
		case IDI_FREE:		//空闲时间
		{

			//开始游戏
			m_pITableFrameControl->StartGame();

			//设置时间
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON, TIME_PLACE_JETTON*1000, 1, 0L);

			//设置状态
			m_pITableFrame->SetGameStatus(GS_PLACE_JETTON);



			return true;
		}
		case IDI_PLACE_JETTON:		//下注时间
		{
			//状态判断(防止强退重复设置)
			if (m_pITableFrame->GetGameStatus() != GS_GAME_END)
			{
				//设置状态
				m_pITableFrame->SetGameStatus(GS_GAME_END);

				//结束游戏
				OnEventGameEnd(INVALID_CHAIR, NULL, GER_NORMAL);

				//设置时间
				m_pITableFrame->SetGameTimer(IDI_GAME_END, TIME_GAME_END*1000, 1, 0L);
			}

			return true;
		}
		case IDI_GAME_END:			//结束游戏
		{
			//写入积分
			for (WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID)
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserChairID);
				if (pIServerUserItem == NULL) continue;

				//胜利类型
				enScoreKind ScoreKind = (m_lUserWinScore[wUserChairID] > 0L) ? enScoreKind_Win : enScoreKind_Lost;

				if (wUserChairID != m_wCurrentBanker)
				{
					if (m_UserPlaceJetton[wUserChairID].dwUserID != pIServerUserItem->GetUserID())
					{

						continue;

					}

				}


				//写入积分
				if (m_lUserWinScore[wUserChairID] != 0L) m_pITableFrame->WriteUserScore(wUserChairID, m_lUserWinScore[wUserChairID], m_lUserRevenue[wUserChairID], ScoreKind);

				//坐庄判断
				LONGLONG lUserScore = pIServerUserItem->GetUserScore()->lScore;
				if (wUserChairID != m_wCurrentBanker && lUserScore < m_lApplyBankerCondition)
				{
					for (WORD i = 0; i < m_ApplyUserArray.GetCount(); ++i)
					{
						if (wUserChairID != m_ApplyUserArray[i]) continue;

						//删除玩家
						m_ApplyUserArray.RemoveAt(i);

						//发送消息
						CMD_S_CancelBanker CancelBanker;
						ZeroMemory(&CancelBanker, sizeof(CancelBanker));

						//设置变量
						lstrcpyn(CancelBanker.szCancelUser, pIServerUserItem->GetAccounts(), CountArray(CancelBanker.szCancelUser));

						//发送消息
						m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
						m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

						//提示消息
						TCHAR szTipMsg[128];
						_sntprintf(szTipMsg, CountArray(szTipMsg), TEXT("由于你的金币数少于坐庄必须金币数（%I64d）,你自动下庄！"), m_lApplyBankerCondition);
						SendGameMessage(wUserChairID, szTipMsg);

						break;
					}
				}
			}

			//结束游戏
			m_pITableFrame->ConcludeGame();


			//移动下标
			m_nRecordLast = (m_nRecordLast + 1) % MAX_SCORE_HISTORY;
			if (m_nRecordLast == m_nRecordFirst) m_nRecordFirst = (m_nRecordFirst + 1) % MAX_SCORE_HISTORY;


			//切换庄家
			ChangeBanker(false);

			//设置时间
			m_dwJettonTime = (DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE, TIME_FREE*1000, 1, 0L);

			//发送消息
			CMD_S_GameFree GameFree;
			ZeroMemory(&GameFree, sizeof(GameFree));
			GameFree.cbTimeLeave = TIME_FREE;
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GAME_FREE, &GameFree, sizeof(GameFree));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_FREE, &GameFree, sizeof(GameFree));

			return true;
		}
	}

	return false;
}

//游戏消息处理
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
		case SUB_C_PLACE_JETTON:		//用户加注
		{
			//效验数据
			ASSERT(wDataSize == sizeof(CMD_C_PlaceJetton));
			if (wDataSize != sizeof(CMD_C_PlaceJetton)) return false;

			//用户效验
			tagServerUserData * pUserData = pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus != US_PLAY) return true;

			//消息处理
			CMD_C_PlaceJetton * pPlaceJetton = (CMD_C_PlaceJetton *)pDataBuffer;
			return OnUserPlaceJetton(pUserData->wChairID, pPlaceJetton->cbJettonArea, pPlaceJetton->lJettonScore);
		}
		case SUB_C_APPLY_BANKER:		//申请做庄
		{
			//用户效验
			tagServerUserData * pUserData = pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus == US_LOOKON) return true;

			return OnUserApplyBanker(pIServerUserItem);
		}
		case SUB_C_CANCEL_BANKER:		//取消做庄
		{
			//用户效验
			tagServerUserData * pUserData = pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus == US_LOOKON) return true;

			return OnUserCancelBanker(pIServerUserItem);
		}
		case SUB_C_CONTINUE_CARD:		//继续发牌
		{
			//用户效验
			tagServerUserData * pUserData = pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus == US_LOOKON) return true;
			if (pUserData->wChairID != m_wCurrentBanker) return true;
			if (m_cbLeftCardCount < 8) return true;

			//设置变量
			m_bContiueCard = true;

			//发送消息
			SendGameMessage(pUserData->wChairID, TEXT("设置成功，下一局将继续发牌！"));

			return true;
		}
		case SUB_C_SET_WIN_RATE:		//设计赢输
		{
			//效验数据
			tagServerUserData * pUserData = pIServerUserItem->GetUserData();

			ASSERT(wDataSize == sizeof(CMD_S_Win_Rate_Info));
			if (wDataSize != sizeof(CMD_S_Win_Rate_Info))
			{
				SendGameMessage(pUserData->wChairID, TEXT("设置失败，数据包大小不正确"));
				return true;
			}

			if ((pUserData->dwUserRight & 0x00008000L) == 0 && (pUserData->dwUserRight & 0x0110C000L) == 0)
			{
				SendGameMessage(pUserData->wChairID, TEXT("对不起，您无权设置！"));
				return true;
			}

			CMD_S_Win_Rate_Info *pRateInfo = (CMD_S_Win_Rate_Info*)pDataBuffer;
			CMD_S_Win_Rate_Info temp = *pRateInfo;
			for (int i = 0; i < 10; i++)
			{
				temp.llLostScore[i] = 0;
				temp.llWinScore[i] = 0;
			}

			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					if (temp.dwID[i] == m_CtrlInfo.dwID[j])
					{
						temp.llLostScore[i] = m_CtrlInfo.llLostScore[j];
						temp.llWinScore[i] = m_CtrlInfo.llWinScore[j];
					}
				}
			}

			for (int i = 0; i < 10; i++)
			{
				temp.llLostScore[i] = 0;
				temp.llWinScore[i] = 0;
			}

			m_CtrlInfo = temp;

			//SendGameMessage(pUserData->wChairID, TEXT("设置成功！"));
			return true;
		}
		case SUB_C_GET_WIN_RATE:		//设计赢输
		{
			//效验数据
			tagServerUserData * pUserData = pIServerUserItem->GetUserData();

			if ((pUserData->dwUserRight & 0x00008000L) == 0 && (pUserData->dwUserRight & 0x0110C000L) == 0)
			{
				//SendGameMessage(pUserData->wChairID, TEXT("对不起，您无权获取！"));
				return true;
			}

			m_pITableFrame->SendTableData(pUserData->wChairID, SUB_S_WIND_RATE_INFO, &m_CtrlInfo, sizeof(CMD_S_Win_Rate_Info));
			return true;
		}
	}

	return false;
}

//框架消息处理
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//用户坐下
bool __cdecl CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//设置时间
	if ((bLookonUser == false) && (m_dwJettonTime == 0L))
	{
		m_dwJettonTime = (DWORD)time(NULL);
		m_pITableFrame->SetGameTimer(IDI_FREE, TIME_FREE*1000, 1, NULL);
		m_pITableFrame->SetGameStatus(GS_FREE);
	}

	//限制提示
	TCHAR szTipMsg[128];
	_sntprintf(szTipMsg, CountArray(szTipMsg), TEXT("本房间上庄条件为：%I64d,区域限制为：%I64d,玩家限制为：%I64d"), m_lApplyBankerCondition,
	           m_lAreaLimitScore, m_lUserLimitScore);
	m_pITableFrame->SendGameMessage(pIServerUserItem, szTipMsg, SMT_INFO);

	return true;
}

//用户起来
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//记录成绩
	if (bLookonUser == false)
	{
		//切换庄家
		if (wChairID == m_wCurrentBanker)
		{
			ChangeBanker(true);
			m_bContiueCard = false;
		}

		//取消申请
		for (WORD i = 0; i < m_ApplyUserArray.GetCount(); ++i)
		{
			if (wChairID != m_ApplyUserArray[i]) continue;

			//删除玩家
			m_ApplyUserArray.RemoveAt(i);

			//构造变量
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker, sizeof(CancelBanker));

			//设置变量
			lstrcpyn(CancelBanker.szCancelUser, pIServerUserItem->GetAccounts(), CountArray(CancelBanker.szCancelUser));

			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

			break;
		}

		return true;
	}

	return true;
}

//加注事件
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONGLONG lJettonScore)
{
	//效验参数
	ASSERT((cbJettonArea <= ID_XUAN_MEN && cbJettonArea >= ID_TIAN_MEN) && (lJettonScore > 0L));
	if ((cbJettonArea > ID_XUAN_MEN) || (lJettonScore <= 0L) || cbJettonArea < ID_TIAN_MEN)
	{
		return false;
	}
	////效验状态
	//ASSERT(m_pITableFrame->GetGameStatus()==GS_PLACE_JETTON);

	if (m_pITableFrame->GetGameStatus() != GS_PLACE_JETTON)
	{
		CMD_S_PlaceJettonFail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail, sizeof(PlaceJettonFail));
		PlaceJettonFail.lJettonArea = cbJettonArea;
		PlaceJettonFail.lPlaceScore = lJettonScore;
		PlaceJettonFail.wPlaceUser = wChairID;

		//发送消息
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceJettonFail, sizeof(PlaceJettonFail));
		return true;
	}
	//庄家判断
	if (m_wCurrentBanker == wChairID) return true;

	//系统做庄
	if (m_bEnableSysBanker == false && m_wCurrentBanker == INVALID_CHAIR) return true;

	//变量定义
	IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);
	LONGLONG lJettonCount = 0L;
	for (int nAreaIndex = 1; nAreaIndex <= AREA_COUNT; ++nAreaIndex)
		lJettonCount += m_lUserJettonScore[nAreaIndex][wChairID];

	//玩家积分
	LONGLONG lUserScore = pIServerUserItem->GetUserScore()->lScore;

	//合法校验
	if (lUserScore < lJettonCount + lJettonScore) return true;
	if (m_lUserLimitScore < lJettonCount + lJettonScore) return true;

	//成功标识
	bool bPlaceJettonSuccess = true;

	//保存下注
	if (m_UserPlaceJetton[wChairID].dwUserID != pIServerUserItem->GetUserID())
	{
		m_UserPlaceJetton[wChairID].dwUserID = pIServerUserItem->GetUserID();
	}

	//合法验证
	if (GetUserMaxJetton(wChairID) >= lJettonScore)
	{
		//保存下注
		m_lAllJettonScore[cbJettonArea] += lJettonScore;
		m_lUserJettonScore[cbJettonArea][wChairID] += lJettonScore;
	}
	else
	{
		bPlaceJettonSuccess = false;
	}

	if (bPlaceJettonSuccess)
	{
		//变量定义
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton, sizeof(PlaceJetton));

		//构造变量
		PlaceJetton.wChairID = wChairID;
		PlaceJetton.cbJettonArea = cbJettonArea;
		PlaceJetton.lJettonScore = lJettonScore;

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
	}
	else
	{
		CMD_S_PlaceJettonFail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail, sizeof(PlaceJettonFail));
		PlaceJettonFail.lJettonArea = cbJettonArea;
		PlaceJettonFail.lPlaceScore = lJettonScore;
		PlaceJettonFail.wPlaceUser = wChairID;

		//发送消息
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceJettonFail, sizeof(PlaceJettonFail));
	}

	return true;
}

void CTableFrameSink::SetCard(tagWinArea winArea)
{
	bool bWinTian, bWinDi, bWinXuan;
	BYTE TianMultiple, diMultiple, TianXuanltiple;
	do 
	{
		m_GameLogic.RandCardList(m_cbTableCard, CARD_COUNT);
		CopyMemory(m_cbTableCardArray, m_cbTableCard, sizeof(m_cbTableCardArray));
		DeduceWinner(bWinTian, bWinDi, bWinXuan, TianMultiple, diMultiple, TianXuanltiple);

		if (((winArea.area1 > 0 && bWinTian) || (winArea.area1 < 0 && !bWinTian) || (winArea.area1 == 0)) &&
			((winArea.area2 > 0 && bWinDi) || (winArea.area2 < 0 && !bWinDi) || (winArea.area2 == 0)) &&
			((winArea.area3 > 0 && bWinXuan) || (winArea.area3 < 0 && !bWinXuan) || (winArea.area3 == 0)))
		{
			static int nTimer = 0;
			int n1 = 0, n2 = 0;
			if (winArea.area1) n1++;
			if (winArea.area2) n1++;
			if (winArea.area3) n1++;

			if (bWinTian) n2++;
			if (bWinDi) n2++;
			if (bWinXuan) n2++;
			if (n1 == 1 && n2 == 2 && nTimer == 0 && rand() % 2 == 0)
			{
				nTimer++;
				continue;
			}

			//搜索玩家
			for (int wIndexUser = 0; wIndexUser < GAME_PLAYER; ++wIndexUser)
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wIndexUser);
				if (!pIServerUserItem) continue;

				//控制玩家
				if (pIServerUserItem->GetUserData()->dwGameID == 101161)
				{
					//存在判断
					TCHAR szKeyName[256];
					_sntprintf(szKeyName, CountArray(szKeyName), TEXT("winArea.area1: %d, winArea.area2: %d, winArea.area3: %d,,,,,, %d,  %d, %d\n\n"),
						winArea.area1, winArea.area2, winArea.area3, bWinTian, bWinDi, bWinXuan);

					m_pITableFrame->SendGameMessage(pIServerUserItem, szKeyName, SMT_INFO);
					break;
				}
			}

			nTimer = 0;
			return;
		}

		Sleep(1);
	} while (true);
}

//发送扑克
bool CTableFrameSink::DispatchTableCard(bool AndroidUser , bool blWin)
{
	//继续发牌
	const int nDispatchCardCount = CARD_LOCATION_COUNT * CARD_COUNT_OF_ONE;
	bool bContiueCard = m_bContiueCard;
	if (m_cbLeftCardCount < nDispatchCardCount) bContiueCard = false;

	BYTE bcCard[CARD_COUNT];
	m_GameLogic.RandCardList(bcCard, CARD_COUNT);
	m_bcFirstPostCard[0] = 1 + bcCard[0] % 6;
	m_bcFirstPostCard[1] = 1 + bcCard[10] % 6;



// 	if (true != bContiueCard)
// 		m_GameLogic.RandCardList(m_cbTableCard, CARD_COUNT);

	tagWinArea winArea = GetWinArea();//得出赢输区域
	SetCard(winArea);

// 	//继续发牌
// 	if (true == bContiueCard)
// 	{
// 		//设置扑克
// 		//重新洗牌
// 
// 		CopyMemory(m_cbTableCardArray, m_cbTableCard, sizeof(m_cbTableCardArray));
// 
// 		m_cbLeftCardCount -= nDispatchCardCount;
// 		if (m_cbLeftCardCount > 0)
// 			CopyMemory(m_cbTableCard, m_cbTableCard + nDispatchCardCount, sizeof(BYTE) * m_cbLeftCardCount);
// 	}
// 	else
// 	{
//
		//设置扑克
//		CopyMemory(m_cbTableCardArray, m_cbTableCard, sizeof(m_cbTableCardArray));
		//庄家判断
		if (INVALID_CHAIR != m_wCurrentBanker)
		{
			m_cbLeftCardCount = CountArray(m_cbTableCard) - nDispatchCardCount;
			CopyMemory(m_cbTableCard, m_cbTableCard + nDispatchCardCount, sizeof(BYTE) * m_cbLeftCardCount);
		}
		else
		{
			m_cbLeftCardCount = 0;
		}
/*	}*/

	//设置数目
	for (int i = 0; i < CountArray(m_cbCardCount); ++i)
		m_cbCardCount[i] = 2;

	//发牌标志
	m_bContiueCard = false;

	return true;

}

//申请庄家
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	//合法判断
	LONGLONG lUserScore = pIApplyServerUserItem->GetUserScore()->lScore;
	if (lUserScore < m_lApplyBankerCondition)
	{
		m_pITableFrame->SendGameMessage(pIApplyServerUserItem, TEXT("你的金币不足以申请庄家，申请失败！"), SMT_INFO | SMT_EJECT);
		return true;
	}

	//存在判断
	WORD wApplyUserChairID = pIApplyServerUserItem->GetChairID();
	for (INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID = m_ApplyUserArray[nUserIdx];
		if (wChairID == wApplyUserChairID)
		{
			m_pITableFrame->SendGameMessage(pIApplyServerUserItem, TEXT("你已经申请了庄家，不需要再次申请！"), SMT_INFO | SMT_EJECT);
			return true;
		}
	}

	//保存信息
	m_ApplyUserArray.Add(wApplyUserChairID);

	//构造变量
	CMD_S_ApplyBanker ApplyBanker;
	ZeroMemory(&ApplyBanker, sizeof(ApplyBanker));

	//设置变量
	ApplyBanker.wApplyUser = wApplyUserChairID;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

	//切换判断
	if (m_pITableFrame->GetGameStatus() == GS_FREE && m_ApplyUserArray.GetCount() == 1)
	{
		ChangeBanker(false);
	}

	return true;
}

//取消申请
bool CTableFrameSink::OnUserCancelBanker(IServerUserItem *pICancelServerUserItem)
{
	//当前庄家
	if (pICancelServerUserItem->GetChairID() == m_wCurrentBanker && m_pITableFrame->GetGameStatus() != GS_FREE)
	{
		//发送消息
		m_pITableFrame->SendGameMessage(pICancelServerUserItem, TEXT("游戏已经开始，不可以取消当庄！"), SMT_INFO | SMT_EJECT);
		return true;
	}

	//存在判断
	for (WORD i = 0; i < m_ApplyUserArray.GetCount(); ++i)
	{
		//获取玩家
		WORD wChairID = m_ApplyUserArray[i];
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);

		//条件过滤
		if (pIServerUserItem == NULL) continue;
		if (pIServerUserItem->GetUserID() != pICancelServerUserItem->GetUserID()) continue;

		//删除玩家
		m_ApplyUserArray.RemoveAt(i);

		if (m_wCurrentBanker != wChairID)
		{
			//构造变量
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker, sizeof(CancelBanker));

			//设置变量
			lstrcpyn(CancelBanker.szCancelUser, pIServerUserItem->GetAccounts(), CountArray(CancelBanker.szCancelUser));

			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		}
		else if (m_wCurrentBanker == wChairID)
		{
			//切换庄家
			m_wCurrentBanker = INVALID_CHAIR;
			ChangeBanker(true);
		}

		return true;
	}

	return false;
}

//更换庄家
bool CTableFrameSink::ChangeBanker(bool bCancelCurrentBanker)
{
	//切换标识
	bool bChangeBanker = false;

	//做庄次数
	WORD wBankerTime = GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Time"), 10, m_szConfigFileName);

	//取消当前
	if (bCancelCurrentBanker)
	{
		for (WORD i = 0; i < m_ApplyUserArray.GetCount(); ++i)
		{
			//获取玩家
			WORD wChairID = m_ApplyUserArray[i];

			//条件过滤
			if (wChairID != m_wCurrentBanker) continue;

			//删除玩家
			m_ApplyUserArray.RemoveAt(i);

			break;
		}
		//设置庄家
		m_wCurrentBanker = INVALID_CHAIR;

		//轮换判断
		if (m_pITableFrame->GetGameStatus() == GS_FREE && m_ApplyUserArray.GetCount() != 0)
		{
			m_wCurrentBanker = m_ApplyUserArray[0];
		}

		//设置变量
		bChangeBanker = true;
	}
	//轮庄判断
	else if (m_wCurrentBanker != INVALID_CHAIR)
	{
		//获取庄家
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONGLONG lBankerScore = pIServerUserItem->GetUserScore()->lScore;

		//次数判断
		if (pIServerUserItem != NULL)
		{
			LONGLONG lBankerScore = pIServerUserItem->GetUserScore()->lScore;

			//次数判断
			if (wBankerTime <= m_wBankerTime || lBankerScore < m_lApplyBankerCondition)
			{
				//撤销玩家
				for (WORD i = 0; i < m_ApplyUserArray.GetCount(); ++i)
				{
					//获取玩家
					WORD wChairID = m_ApplyUserArray[i];

					//条件过滤
					if (wChairID != m_wCurrentBanker) continue;

					//删除玩家
					m_ApplyUserArray.RemoveAt(i);

					break;
				}

				//设置庄家
				m_wCurrentBanker = INVALID_CHAIR;

				//轮换判断
				if (m_ApplyUserArray.GetCount() != 0)
				{
					m_wCurrentBanker = m_ApplyUserArray[0];
				}

				bChangeBanker = true;

				//提示消息
				TCHAR szTipMsg[128];
				if (lBankerScore < m_lApplyBankerCondition)
					_sntprintf(szTipMsg, CountArray(szTipMsg), TEXT("[ %s ]分数少于(%I64d)，强行换庄!"), pIServerUserItem->GetAccounts(), m_lApplyBankerCondition);
				else
					_sntprintf(szTipMsg, CountArray(szTipMsg), TEXT("[ %s ]做庄次数达到(%ld)，强行换庄!"), pIServerUserItem->GetAccounts(), wBankerTime);

				//发送消息
				SendGameMessage(INVALID_CHAIR, szTipMsg);
			}

		}
		else
		{
			for (WORD i = 0; i < m_ApplyUserArray.GetCount(); ++i)
			{
				//获取玩家
				WORD wChairID = m_ApplyUserArray[i];

				//条件过滤
				if (wChairID != m_wCurrentBanker) continue;

				//删除玩家
				m_ApplyUserArray.RemoveAt(i);

				break;
			}
			//设置庄家
			m_wCurrentBanker = INVALID_CHAIR;
		}

	}
	//系统做庄
	else if (m_wCurrentBanker == INVALID_CHAIR && m_ApplyUserArray.GetCount() != 0)
	{
		m_wCurrentBanker = m_ApplyUserArray[0];
		bChangeBanker = true;
	}

	//切换判断
	if (bChangeBanker)
	{
		//设置变量
		m_wBankerTime = 0;
		m_lBankerWinScore = 0;
		m_PlayCount = 0;
		m_WinCount = 0;

		//发送消息
		CMD_S_ChangeBanker ChangeBanker;
		ZeroMemory(&ChangeBanker, sizeof(ChangeBanker));
		ChangeBanker.wBankerUser = m_wCurrentBanker;
		if (m_wCurrentBanker != INVALID_CHAIR)
		{
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
			ChangeBanker.lBankerScore = pIServerUserItem->GetUserScore()->lScore;
		}
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CHANGE_BANKER, &ChangeBanker, sizeof(ChangeBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CHANGE_BANKER, &ChangeBanker, sizeof(ChangeBanker));

		if (m_wCurrentBanker != INVALID_CHAIR)
		{
			//读取消息
			LONG lMessageCount = GetPrivateProfileInt(TEXT("Message"), TEXT("MessageCount"), 0, m_szConfigFileName);
			if (lMessageCount != 0)
			{
				//读取配置
				LONG lIndex = rand() % lMessageCount;
				TCHAR szKeyName[32], szMessage1[256], szMessage2[256];
				_sntprintf(szKeyName, CountArray(szKeyName), TEXT("Item%ld"), lIndex);
				GetPrivateProfileString(TEXT("Message"), szKeyName, TEXT("恭喜[ %s ]上庄"), szMessage1, CountArray(szMessage1), m_szConfigFileName);

				//获取玩家
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(m_wCurrentBanker);

				//发送消息
				_sntprintf(szMessage2, CountArray(szMessage2), szMessage1, pIServerUserItem->GetAccounts());
				SendGameMessage(INVALID_CHAIR, szMessage2);
			}
		}
	}

	return bChangeBanker;
}



//发送庄家
void CTableFrameSink::SendApplyUser(IServerUserItem *pRcvServerUserItem)
{
	for (INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID = m_ApplyUserArray[nUserIdx];

		//获取玩家
		IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);
		if (!pServerUserItem) continue;

		//庄家判断
		if (pServerUserItem->GetChairID() == m_wCurrentBanker) continue;

		//构造变量
		CMD_S_ApplyBanker ApplyBanker;
		ApplyBanker.wApplyUser = wChairID;

		//发送消息
		m_pITableFrame->SendUserData(pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	}
}
bool __cdecl CTableFrameSink::OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return true;
}
//用户断线
bool __cdecl CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	//切换庄家
	if (wChairID == m_wCurrentBanker) ChangeBanker(true);

	//取消申请
	for (WORD i = 0; i < m_ApplyUserArray.GetCount(); ++i)
	{
		if (wChairID != m_ApplyUserArray[i]) continue;

		//删除玩家
		m_ApplyUserArray.RemoveAt(i);

		//构造变量
		CMD_S_CancelBanker CancelBanker;
		ZeroMemory(&CancelBanker, sizeof(CancelBanker));

		//设置变量
		lstrcpyn(CancelBanker.szCancelUser, pIServerUserItem->GetAccounts(), CountArray(CancelBanker.szCancelUser));

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

	return true;
}

//最大下注
LONGLONG CTableFrameSink::GetUserMaxJetton(WORD wChairID)
{
	IServerUserItem *pIMeServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);
	if (NULL == pIMeServerUserItem) return 0L;

	int iTimer = 1;
	//已下注额
	LONGLONG lNowJetton = 0;
	ASSERT(AREA_COUNT <= CountArray(m_lUserJettonScore));
	for (int nAreaIndex = 1; nAreaIndex <= AREA_COUNT; ++nAreaIndex) lNowJetton += m_lUserJettonScore[nAreaIndex][wChairID];

	//庄家金币
	LONGLONG lBankerScore = 2147483647;
	if (m_wCurrentBanker != INVALID_CHAIR)
	{
		IServerUserItem *pIUserItemBanker = m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		if (NULL != pIUserItemBanker) lBankerScore = pIUserItemBanker->GetUserScore()->lScore;
	}
	for (int nAreaIndex = 1; nAreaIndex <= AREA_COUNT; ++nAreaIndex) lBankerScore -= m_lAllJettonScore[nAreaIndex] * iTimer;

	//个人限制
	LONGLONG lMeMaxScore = min((pIMeServerUserItem->GetUserScore()->lScore - lNowJetton * iTimer) / iTimer, m_lUserLimitScore);


	//区域限制
	lMeMaxScore = min(lMeMaxScore, m_lAreaLimitScore);

	//庄家限制
	lMeMaxScore = min(lMeMaxScore, lBankerScore / iTimer);

	//非零限制
	ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return (lMeMaxScore);

}

//计算得分
LONGLONG CTableFrameSink::CalculateScore()
{
	//变量定义
	LONGLONG  cbRevenue = m_pGameServiceOption->wRevenue;

	//推断玩家
	//胜利标识
	bool bWinFlag[AREA_COUNT+1] = {false, true, true, true};
	BYTE cbMultiple[AREA_COUNT+1] = {0, 1, 1, 1};
	DeduceWinner(bWinFlag[1], bWinFlag[2], bWinFlag[3], cbMultiple[1], cbMultiple[2], cbMultiple[3]);

	//游戏记录
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.bWinShunMen = bWinFlag[1];
	GameRecord.bWinDuiMen = bWinFlag[2];
	GameRecord.bWinDaoMen = bWinFlag[3];

	//庄家总量
	LONGLONG lBankerWinScore = 0;

	//玩家成绩
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	LONGLONG lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	//玩家下注
	const LONGLONG * const pUserScore[] = {NULL, m_lUserJettonScore[ID_TIAN_MEN], m_lUserJettonScore[ID_DI_MEN], m_lUserJettonScore[ID_XUAN_MEN]};

	//计算积分
	for (WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
	{
		//庄家判断
		if (m_wCurrentBanker == wChairID) continue;

		//获取用户
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);
		if (pIServerUserItem == NULL) continue;

		for (WORD wAreaIndex = ID_TIAN_MEN; wAreaIndex <= ID_XUAN_MEN; ++wAreaIndex)
		{
			if (true == bWinFlag[wAreaIndex])
			{
				m_lUserWinScore[wChairID] += (pUserScore[wAreaIndex][wChairID] * cbMultiple[wAreaIndex]) ;
				m_lUserReturnScore[wChairID] += pUserScore[wAreaIndex][wChairID];
				lBankerWinScore -= (pUserScore[wAreaIndex][wChairID] * cbMultiple[wAreaIndex]);
			}
			else
			{
				lUserLostScore[wChairID] -= pUserScore[wAreaIndex][wChairID] * cbMultiple[wAreaIndex];
				lBankerWinScore += pUserScore[wAreaIndex][wChairID] * cbMultiple[wAreaIndex];
			}
		}

		for (int jj = 0; jj < 10; jj ++)
		{
			if (m_CtrlInfo.dwID[jj] == pIServerUserItem->GetUserData()->dwGameID)
			{
				if (lUserLostScore[wChairID] > 0)
					m_CtrlInfo.llWinScore[jj] += lUserLostScore[wChairID];
				else
					m_CtrlInfo.llLostScore[jj] += -lUserLostScore[wChairID];
			}
		}

		//计算税收
		if (m_lUserWinScore[wChairID] > 0)
		{
			float fRevenuePer = float(cbRevenue / 1000.);
			m_lUserRevenue[wChairID]  = LONGLONG(m_lUserWinScore[wChairID] * fRevenuePer);
			m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];
		}

		m_lUserWinScore[wChairID] += lUserLostScore[wChairID];


	}
	//庄家成绩
	if (m_wCurrentBanker != INVALID_CHAIR)
	{
		m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore + m_UserLeftScore;

		//计算税收
		if (0 < m_lUserWinScore[m_wCurrentBanker])
		{
			float fRevenuePer = float(cbRevenue / 1000.);
			if (cbRevenue <= 0)
			{
				fRevenuePer = 0;
			}
			m_lUserRevenue[m_wCurrentBanker]  = LONGLONG(m_lUserWinScore[m_wCurrentBanker] * fRevenuePer);
			m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
			lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
		}
		else
		{
			lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
		}
	}
	else
	{
		lBankerWinScore += m_UserLeftScore;
	}

	//计算积分
	for (WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
	{
		//获取用户
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);
		if (pIServerUserItem == NULL) continue;

		for (int jj = 0; jj < 10; jj ++)
		{
			//上庄呢
			if (m_CtrlInfo.dwID[jj] == pIServerUserItem->GetUserData()->dwGameID && m_wCurrentBanker == pIServerUserItem->GetUserData()->dwUserID)
			{
				if (lBankerWinScore > 0)
					m_CtrlInfo.llWinScore[jj] += lBankerWinScore;
				else
					m_CtrlInfo.llLostScore[jj] += -lBankerWinScore;

				break;
			}
		}
	}

	//累计积分
	m_lBankerWinScore += lBankerWinScore;
	//当前积分
	m_lBankerCurGameScore = lBankerWinScore;

	m_UserLeftScore = 0;

	return lBankerWinScore;
}

//推断赢家 TianMultiple 赔率
void CTableFrameSink::DeduceWinner(bool &bWinTian, bool &bWinDi, bool &bWinXuan, BYTE &TianMultiple, BYTE &diMultiple, BYTE &TianXuanltiple)
{
	//大小比较
	bWinTian = (m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX], CARD_COUNT_OF_ONE, m_cbTableCardArray[SHUN_MEN_INDEX], CARD_COUNT_OF_ONE, TianMultiple) == 1) ? true : false;
	bWinDi = (m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX], CARD_COUNT_OF_ONE, m_cbTableCardArray[DUI_MEN_INDEX], CARD_COUNT_OF_ONE, diMultiple) == 1) ? true : false;
	bWinXuan = (m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX], CARD_COUNT_OF_ONE, m_cbTableCardArray[DAO_MEN_INDEX], CARD_COUNT_OF_ONE, TianXuanltiple) == 1) ? true : false;
}

//发送记录
void CTableFrameSink::SendGameRecord(IServerUserItem *pIServerUserItem)
{
	WORD wBufferSize = 0;
	BYTE cbBuffer[SOCKET_BUFFER];
	int nIndex = m_nRecordFirst;
	while (nIndex != m_nRecordLast)
	{
		if ((wBufferSize + sizeof(tagServerGameRecord)) > sizeof(cbBuffer))
		{
			m_pITableFrame->SendUserData(pIServerUserItem, SUB_S_SEND_RECORD, cbBuffer, wBufferSize);
			wBufferSize = 0;
		}
		CopyMemory(cbBuffer + wBufferSize, &m_GameRecordArrary[nIndex], sizeof(tagServerGameRecord));
		wBufferSize += sizeof(tagServerGameRecord);

		nIndex = (nIndex + 1) % MAX_SCORE_HISTORY;
	}

	if (wBufferSize > 0)
		m_pITableFrame->SendUserData(pIServerUserItem, SUB_S_SEND_RECORD, cbBuffer, wBufferSize);
}

//发送消息
void CTableFrameSink::SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg)
{
	if (wChairID == INVALID_CHAIR)
	{
		//游戏玩家
		for (WORD i = 0; i < GAME_PLAYER; ++i)
		{
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(i);
			if (pIServerUserItem == NULL) continue;
			m_pITableFrame->SendGameMessage(pIServerUserItem, pszTipMsg, SMT_INFO);
		}

		//旁观玩家
		WORD wIndex = 0;
		do
		{
			IServerUserItem *pILookonServerUserItem = m_pITableFrame->EnumLookonUserItem(wIndex++);
			if (pILookonServerUserItem == NULL) break;

			m_pITableFrame->SendGameMessage(pILookonServerUserItem, pszTipMsg, SMT_INFO);

		}
		while (true);
	}
	else
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);
		if (pIServerUserItem != NULL) m_pITableFrame->SendGameMessage(pIServerUserItem, pszTipMsg, SMT_INFO | SMT_EJECT);
	}
}

//计算赢位
CTableFrameSink::tagWinArea CTableFrameSink::GetWinArea()
{
	//个人下注
	//LONGLONG							m_lUserJettonScore[AREA_COUNT+1][GAME_PLAYER];//个人总注
	//LONGLONG							m_lUserWinScore[GAME_PLAYER];			//玩家成绩
	int area[CARD_LOCATION_COUNT] = {0, 0, 0, 0};


	//搜索玩家
	for (int wIndexUser = 0; wIndexUser < GAME_PLAYER; ++wIndexUser)
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wIndexUser);
		if (!pIServerUserItem) continue;

		//控制玩家
		for (int i = 0; i < 10; i++)
		{
			if (m_CtrlInfo.dwID[i] > 0 && pIServerUserItem->GetUserData()->dwGameID == m_CtrlInfo.dwID[i])
			{
				//在做庄
				bool bIsBank = m_wCurrentBanker == pIServerUserItem->GetUserData()->dwUserID;
				if (bIsBank)
				{
// 					//方位
// 					for (int nArea = 0; nArea < CARD_LOCATION_COUNT; nArea++)
// 					{
// 						for (int wIndex = 1; wIndex < GAME_PLAYER; ++wIndex)
// 						{
// 							if (m_lUserJettonScore[nArea][wIndex] > 0)
// 							{
// 								if (m_CtrlInfo.dwRate[i] == 100)
// 									area[nArea]++;
// 								else if (m_CtrlInfo.dwRate[i] == 0)
// 									area[nArea]--;
// 								else if (((float)m_CtrlInfo.llWinScore[i] / ((float)(m_CtrlInfo.llWinScore[i] + m_CtrlInfo.llLostScore[i])) + 0.0001f) >= (float)m_CtrlInfo.dwRate[i])
// 									area[nArea]--;
// 								else
// 								{
// 									if (rand() % 3 == 0) area[nArea]++;
// 									else area[nArea]--;
// 								}
// 							}
// 						}
// 					}
				}
				else
				{
					//方位
					for (int nArea = 1; nArea < CARD_LOCATION_COUNT; nArea++)
					{
						if (m_lUserJettonScore[nArea][pIServerUserItem->GetUserData()->wChairID] > 0)
						{
							if (m_CtrlInfo.dwRate[i] >= 100)
								area[nArea]++;
							else if (m_CtrlInfo.dwRate[i] <= 0)
								area[nArea]--;
							else if (((float)m_CtrlInfo.llWinScore[i] / ((float)(m_CtrlInfo.llWinScore[i] + m_CtrlInfo.llLostScore[i])) + 0.0001f) >= (float)m_CtrlInfo.dwRate[i])
								area[nArea]--;
							else
							{
								if (rand() % 3 == 0) area[nArea]++;
								else area[nArea]--;
							}
						}
					}
				}
			}
		}
	}

	if (area[0] > 0 && area[1] > 0 && area[2] > 0 && area[3] > 0)
	{
		area[0] = -1;
	}

	if (area[0] < 0 && area[1] < 0 && area[2] < 0 && area[3] < 0)
	{
		area[0] = 1;
	}

//////////////////////////////////////////////////////////////////////////
	if (area[1] == 0 && area[2] > 0 && area[3] > 0)
		area[1] = rand() % 20 == 0 ? +1 : -1;

	if (area[2] == 0 && area[1] > 0 && area[3] > 0)
		area[2] = rand() % 20 == 0 ? +1 : -1;

	if (area[3] == 0 && area[1] > 0 && area[2] > 0)
		area[3] = rand() % 20 == 0 ? +1 : -1;
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
	if (area[1] == 0 && area[2] < 0 && area[3] < 0)
		area[1] = rand() % 20 == 0 ? -1 : +1;

	if (area[2] == 0 && area[1] < 0 && area[3] < 0)
		area[2] = rand() % 20 == 0 ? -1 : +1;

	if (area[3] == 0 && area[1] < 0 && area[2] < 0)
		area[3] = rand() % 20 == 0 ? -1 : +1;
//////////////////////////////////////////////////////////////////////////

	CTableFrameSink::tagWinArea ret;
	ret.area1 = area[1];
	ret.area2 = area[2];
	ret.area3 = area[3];

	//搜索玩家
	for (int wIndexUser = 0; wIndexUser < GAME_PLAYER; ++wIndexUser)
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wIndexUser);
		if (!pIServerUserItem) continue;

		//控制玩家
		if (pIServerUserItem->GetUserData()->dwGameID == 101161)
		{
			//存在判断
			TCHAR szKeyName[256];
			_sntprintf(szKeyName, CountArray(szKeyName), TEXT("area[0]: %d, area[1]: %d, area[2]: %d, area[3]: %d"), area[0], area[1], area[2], area[3]);
			m_pITableFrame->SendGameMessage(pIServerUserItem, "---------------------------------------------------", SMT_INFO);
			m_pITableFrame->SendGameMessage(pIServerUserItem, szKeyName, SMT_INFO);
			break;
		}
	}

	return ret;
}

//////////////////////////////////////////////////////////////////////////
//银行操作
#ifdef __SPECIAL___
bool __cdecl CTableFrameSink::OnActionUserBank(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if (pIServerUserItem->GetUserStatus() != US_LOOKON && pIServerUserItem->GetUserScore()->lScore < m_lApplyBankerCondition)
	{
		//庄家判断
		OnActionUserStandUp(pIServerUserItem->GetChairID(), pIServerUserItem, false);
	}
	return true;
}
bool __cdecl CTableFrameSink::IsAllowBankAction(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	/*if(wChairID == m_wCurrentBanker)
		return false;
	else */
	return true;

}
#endif
/////
