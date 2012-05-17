#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_FREE						99									//空闲时间
#define IDI_PLACE_JETTON				2									//下注时间
#define IDI_ANDROID_USER_PLACE_JETTON	101									//机器人下注
#define IDI_DISPATCH_CARD				301									//发牌时间
//
//

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	m_lMeMaxScore = 0;						//最大下注
	m_lAreaLimitScore = 0;					//区域限制
	m_lApplyBankerCondition = 0;			//申请条件

	//下注信息
	memset(m_lAllJettonScore, 0, sizeof(m_lAllJettonScore));//全体总注
	memset(m_lMeUserJettonScore, 0, sizeof(m_lMeUserJettonScore));//个人总注
	memset(m_cbTableCardArray, 0, sizeof(m_cbTableCardArray));//桌面扑克
	m_cbLeftCardCount = 0;						//扑克数目

	//成绩报告
	m_lEndBankerScore = 0;					//庄家成绩
	m_lEndUserScore = 0;					//玩家成绩
	m_lEndUserReturnScore = 0;				//返回积分
	m_lEndRevenue = 0;						//游戏税收

	//当庄信息
	m_nMeBankerCount = 0;					//当庄次数
	m_nMePlayerCount = 0;					//闲家次数

	//庄家信息
	m_lBankerScore = 0;						//庄家积分
	m_wCurrentBanker = INVALID_CHAIR;					//当前庄家
	m_cbBankerTime = 0;						//庄家局数
	m_lBankerWinScore = 0;					//庄家成绩

	//申请列表
	//m_AppyBankerAdroidUserArray.RemoveAll();	//申请玩家
	m_AppyBankerAdroidUserCount = 0;
	m_bAppyBanker = false;

	//控制变量
	m_bEnableAndroidUserBanker = true;			//允许做庄
	m_bEnableSysBanker = true;					//允许系统做庄

	m_lMeStatisticScore = 0L;					//累计积分
}

//析构函数
CAndroidUserItemSink::~CAndroidUserItemSink()
{

}

//接口查询
void * __cdecl CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink, Guid, dwQueryVer);
	return NULL;
}

//初始接口
bool __cdecl CAndroidUserItemSink::InitUserItemSink(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pIAndroidUserItem = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IAndroidUserItem);
	if (m_pIAndroidUserItem == NULL) return false;

	return true;
}

//重置接口
bool __cdecl CAndroidUserItemSink::RepositUserItemSink()
{

	return true;
}

//时间消息
bool __cdecl CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	try
	{
		switch (nTimerID)
		{
				//空闲：可申请做庄，转帐等
			case IDI_FREE:
			{
				m_pIAndroidUserItem->KillGameTimer(IDI_FREE);
				break;
			}
			//发牌
			case IDI_DISPATCH_CARD:
			{
				return true;
				break;
			}
			//下注
			case IDI_PLACE_JETTON:
			{
				return true;
				break;
			}
			//机器人自动下注
			case IDI_ANDROID_USER_PLACE_JETTON:
			{
				m_pIAndroidUserItem->KillGameTimer(IDI_ANDROID_USER_PLACE_JETTON);
				if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID()) return true;

				if (m_pIAndroidUserItem->GetGameStatus() == GS_PLACE_JETTON)
				{
					LONGLONG llMaxScore = m_lMeMaxScore;//min(, m_pIAndroidUserItem->GetMeUserItem()->GetUserScore()->lGameGold / 2);
					const LONGLONG llScore[] = {10000L, 100000L, 1000000L, 5000000L, 10000000L, 50000000L};
					int index = -1;
					for (int i = 5; i >= 0; i--)
					{
						if (llMaxScore > llScore[i])
						{
							index = i;
							break;
						}
					}

					if (index >= 0 && rand() % 3 == 0)
					{
						int nAreaIndex = 1 + rand() % AREA_COUNT;
						int nScoreIndex = rand() % 2;
						if (nAreaIndex >= 0 && nAreaIndex <= 6 && nScoreIndex >= 0 && nScoreIndex <= 2)
							AndroidUserPlaceJetton(nAreaIndex, llScore[nScoreIndex]);
					}

					int nTimer = 1 + rand() % 2;
					while (nTimer <= 0)
					{
						nTimer = 1 + rand() % 2;
					}

					m_pIAndroidUserItem->SetGameTimer(IDI_ANDROID_USER_PLACE_JETTON, nTimer);
				}
				break;
			}
		}

		return true;
	}
	catch (...)
	{

	}
}

//游戏消息
bool __cdecl CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
		case SUB_S_GAME_FREE:		//游戏空闲
		{
			return OnSubGameFree(pBuffer, wDataSize);
		}
		case SUB_S_GAME_START:		//游戏开始
		{
			return OnSubGameStart(pBuffer, wDataSize);
		}
		case SUB_S_PLACE_JETTON:	//用户加注
		{
			return OnSubPlaceJetton(pBuffer, wDataSize);
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
			return OnSubGameEnd(pBuffer, wDataSize);
		}
		case SUB_S_SEND_RECORD:		//游戏记录
		{
			return true;
		}
		case SUB_S_PLACE_JETTON_FAIL:	//下注失败
		{
			return OnSubPlaceJettonFail(pBuffer, wDataSize);
		}
// 	case SUB_S_CANCEL_CAN:
// 		{
// 			return true;
// 		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

//游戏消息
bool __cdecl CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	return true;
}

//场景消息
bool __cdecl CAndroidUserItemSink::OnEventGameScene(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	try
	{

		switch (cbGameStatus)
		{
			case GS_FREE:			//空闲状态
			{
				//效验数据
				ASSERT(wDataSize == sizeof(CMD_S_StatusFree));
				if (wDataSize != sizeof(CMD_S_StatusFree)) return false;

				//消息处理
				CMD_S_StatusFree * pStatusFree = (CMD_S_StatusFree *)pData;

				//设置时间
				//m_pIAndroidUserItem->SetGameTimer(IDI_FREE, pStatusFree->cbTimeLeave);

				//玩家信息
				m_lMeMaxScore = pStatusFree->lUserMaxScore;
				//庄家信息
				m_wCurrentBanker = pStatusFree->wBankerUser;
				m_lBankerScore = pStatusFree->lBankerScore;
				m_cbBankerTime = pStatusFree->cbBankerTime;
				m_lBankerWinScore = pStatusFree->lBankerWinScore;
				m_bEnableSysBanker = pStatusFree->bEnableSysBanker;


				//控制信息
				m_lApplyBankerCondition = pStatusFree->lApplyBankerCondition;
				m_lAreaLimitScore = pStatusFree->lAreaLimitScore;

				//设置状态
				m_pIAndroidUserItem->SetGameStatus(GS_FREE);

				return true;
			}
			case GS_PLAYING:		//游戏状态
			case GS_GAME_END:		//结束状态
			{
				//效验数据
				ASSERT(wDataSize == sizeof(CMD_S_StatusPlay));
				if (wDataSize != sizeof(CMD_S_StatusPlay)) return false;

				//消息处理
				CMD_S_StatusPlay * pStatusPlay = (CMD_S_StatusPlay *)pData;

				//下注信息
				for (int nAreaIndex = 1; nAreaIndex <= AREA_COUNT; ++nAreaIndex)
				{
					m_lAllJettonScore[nAreaIndex] = pStatusPlay->lAllJettonScore[nAreaIndex];
					m_lMeUserJettonScore[nAreaIndex] = pStatusPlay->lUserJettonScore[nAreaIndex];
				}

				//玩家积分
				m_lMeMaxScore = pStatusPlay->lUserMaxScore;


				//控制信息
				m_lApplyBankerCondition = pStatusPlay->lApplyBankerCondition;
				m_lAreaLimitScore = pStatusPlay->lAreaLimitScore;

				if (pStatusPlay->cbGameStatus == GS_GAME_END)
				{
					//扑克信息
					memcpy(m_cbTableCardArray, pStatusPlay->cbTableCardArray, sizeof(m_cbTableCardArray));

					//设置成绩
					m_lEndBankerScore = pStatusPlay->lEndBankerScore;				//庄家成绩
					m_lEndUserScore = pStatusPlay->lEndUserScore;					//玩家成绩
					m_lEndUserReturnScore = pStatusPlay->lEndUserReturnScore;		//返回积分
					m_lEndRevenue = pStatusPlay->lEndRevenue;						//游戏税收
				}

				//庄家信息
				m_wCurrentBanker = pStatusPlay->wBankerUser;
				m_lBankerScore = pStatusPlay->lBankerScore;
				m_cbBankerTime = pStatusPlay->cbBankerTime;
				m_lBankerWinScore = pStatusPlay->lBankerWinScore;
				m_bEnableSysBanker = pStatusPlay->bEnableSysBanker;

				//设置状态
				m_pIAndroidUserItem->SetGameStatus(pStatusPlay->cbGameStatus);
				//设置时间
				//m_pIAndroidUserItem->SetGameTimer(pStatusPlay->cbGameStatus == GS_GAME_END ? IDI_DISPATCH_CARD : IDI_PLACE_JETTON, pStatusPlay->cbTimeLeave);

				return true;
			}
		}

		return true;
	}
	catch (...)
	{

	}
}

//用户进入
void __cdecl CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户离开
void __cdecl CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户积分
void __cdecl CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户状态
void __cdecl CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户段位
void __cdecl CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//游戏空闲
bool CAndroidUserItemSink::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	try
	{
		//消息处理
		CMD_S_GameFree * pGameFree = (CMD_S_GameFree *)pBuffer;

		//设置时间
		//m_pIAndroidUserItem->SetGameTimer(IDI_FREE, pGameFree->cbTimeLeave);

		//设置状态
		m_pIAndroidUserItem->SetGameStatus(GS_FREE);
		//申请做庄


		return true;
	}
	catch (...)
	{

	}
}

//个人下注
void CAndroidUserItemSink::AndroidUserPlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	try
	{
		//变量定义
		CMD_C_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton, sizeof(PlaceJetton));

		//构造变量
		PlaceJetton.cbJettonArea = cbViewIndex;
		PlaceJetton.lJettonScore = lJettonCount;

		//发送消息
		m_pIAndroidUserItem->SendSocketData(SUB_C_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));

	}
	catch (...)
	{

	}
}

//申请做庄
void CAndroidUserItemSink::RandomApplyBanker()
{
	try
	{
		if (m_bEnableAndroidUserBanker && m_pIAndroidUserItem && m_pIAndroidUserItem->GetMeUserItem() && m_pIAndroidUserItem->GetMeUserItem()->GetUserScore())
		{
			if (m_wCurrentBanker != m_pIAndroidUserItem->GetChairID())
			{
				if (m_pIAndroidUserItem->GetGameStatus() == GS_FREE &&
				        //m_AppyBankerAdroidUserArray.GetCount() <= 0 &&
				        m_AppyBankerAdroidUserCount < 1 &&
				        m_pIAndroidUserItem->GetMeUserItem()->GetUserScore()->lGameGold > m_lApplyBankerCondition &&
				        !m_bAppyBanker && (rand() % 10 == 0 || m_wCurrentBanker == INVALID_CHAIR))
				{
					//申请上庄
					m_pIAndroidUserItem->SendSocketData(SUB_C_APPLY_BANKER, NULL, 0);
					m_bAppyBanker = true;
				}
			}
			else if (m_cbBankerTime >= 3 && rand() % 3 == 0)
			{
				//下庄
				m_pIAndroidUserItem->SendSocketData(SUB_C_CANCEL_BANKER, NULL, 0);
				m_bAppyBanker = false;
			}
		}
	}
	catch (...)
	{

	}
}

//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	try
	{
		//效验数据
		ASSERT(wDataSize == sizeof(CMD_S_GameStart));
		if (wDataSize != sizeof(CMD_S_GameStart)) return false;

		//消息处理
		CMD_S_GameStart * pGameStart = (CMD_S_GameStart *)pBuffer;

		//庄家信息
		m_lBankerScore = pGameStart->lBankerScore;					//庄家积分
		m_wCurrentBanker = pGameStart->wBankerUser;					//当前庄家

		//玩家信息
		m_lMeMaxScore = pGameStart->lUserMaxScore;

		//设置时间
		//m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON, pGameStart->cbTimeLeave);
		int nTimer = 1 + rand() % 2;
		while (nTimer <= 0)
		{
			nTimer = 1 + rand() % 2;
		}
		m_pIAndroidUserItem->SetGameTimer(IDI_ANDROID_USER_PLACE_JETTON, nTimer);
		RandomApplyBanker();

		//设置状态
		m_pIAndroidUserItem->SetGameStatus(GS_PLACE_JETTON);

		return true;
	}
	catch (...)
	{

	}
}

//用户加注
bool CAndroidUserItemSink::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{
	try
	{
		//效验数据
		ASSERT(wDataSize == sizeof(CMD_S_PlaceJetton));
		if (wDataSize != sizeof(CMD_S_PlaceJetton)) return false;

		//消息处理
		CMD_S_PlaceJetton * pPlaceJetton = (CMD_S_PlaceJetton *)pBuffer;

		if (m_pIAndroidUserItem->GetChairID() != pPlaceJetton->wChairID/* || IsLookonMode()*/)
		{
			m_lAllJettonScore[pPlaceJetton->cbJettonArea] += pPlaceJetton->lJettonScore;
		}
	}
	catch (...)
	{

	}
	return true;
}

//切换庄家
bool CAndroidUserItemSink::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize == sizeof(CMD_S_ChangeBanker));
	if (wDataSize != sizeof(CMD_S_ChangeBanker)) return false;

	//消息处理
	CMD_S_ChangeBanker * pChangeBanker = (CMD_S_ChangeBanker *)pBuffer;

	//庄家信息
	m_lBankerScore = pChangeBanker->lBankerScore;						//庄家积分
	m_wCurrentBanker = pChangeBanker->wBankerUser;					//当前庄家

	//删除玩家
//	if (m_wCurrentBanker != INVALID_CHAIR)
//	{
// 		//删除玩家
// 		for (int i = 0; i < m_AppyBankerAdroidUserArray.GetCount(); i++)
// 		{
// 			if (m_AppyBankerAdroidUserArray[i] == pChangeBanker->wBankerUser)
// 				m_AppyBankerAdroidUserArray.RemoveAt(i);
// 		}
	m_AppyBankerAdroidUserCount--;
//	}
	if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID())
		m_bAppyBanker = false;

	return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	try
	{
		//效验数据
		ASSERT(wDataSize == sizeof(CMD_S_GameEnd));
		if (wDataSize != sizeof(CMD_S_GameEnd)) return false;

		//消息处理
		CMD_S_GameEnd * pGameEnd = (CMD_S_GameEnd *)pBuffer;


		//设置时间
		//m_pIAndroidUserItem->SetGameTimer(IDI_DISPATCH_CARD, pGameEnd->cbTimeLeave);

		//扑克信息
		memcpy(m_cbTableCardArray, pGameEnd->cbTableCardArray, sizeof(m_cbTableCardArray));

		//设置状态
		m_pIAndroidUserItem->SetGameStatus(GS_GAME_END);

		m_lEndBankerScore = pGameEnd->lBankerTotallScore;
		m_cbBankerTime = pGameEnd->nBankerTime;
		m_cbLeftCardCount = pGameEnd->cbLeftCardCount;

		//成绩信息
		m_lEndBankerScore = pGameEnd->lBankerScore;				//庄家成绩
		m_lEndUserScore = pGameEnd->lUserScore;					//玩家成绩
		m_lEndUserReturnScore = pGameEnd->lUserReturnScore;		//返回积分
		m_lEndRevenue = pGameEnd->lRevenue;						//游戏税收
		//庄家信息
		if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID())
			m_lMeStatisticScore += m_lEndBankerScore;
		else
			m_lMeStatisticScore += m_lEndUserScore;

		return true;
	}
	catch (...)
	{

	}
}

//下注失败
bool CAndroidUserItemSink::OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize == sizeof(CMD_S_PlaceJettonFail));
	if (wDataSize != sizeof(CMD_S_PlaceJettonFail)) return false;
//
// 	//消息处理
// 	CMD_S_PlaceJettonFail * pPlaceJettonFail = (CMD_S_PlaceJettonFail *)pBuffer;
//
// 	//效验参数
// 	ASSERT(pPlaceJettonFail->lJettonArea <= ID_JIAO_R && pPlaceJettonFail->lJettonArea >= ID_SHUN_MEN);
// 	if (!(pPlaceJettonFail->lJettonArea <= ID_JIAO_R && pPlaceJettonFail->lJettonArea >= ID_SHUN_MEN)) return false;
//
// 	//自己判断
// 	if (m_pIAndroidUserItem->GetChairID() == pPlaceJettonFail->wPlaceUser)
// 	{
// 		//合法校验
// 		ASSERT(m_lMeUserJettonScore[cbViewIndex] >= pPlaceJettonFail->lPlaceScore);
// 		if (pPlaceJettonFail->lPlaceScore > m_lMeUserJettonScore[pPlaceJettonFail->lJettonArea]) return false;
//
// 		//设置下注
// 		m_lMeUserJettonScore[pPlaceJettonFail->lJettonArea] -= pPlaceJettonFail->lPlaceScore;
// 	}

	return true;
}

//申请做庄
bool CAndroidUserItemSink::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	try
	{
		//效验数据
		ASSERT(wDataSize == sizeof(CMD_S_ApplyBanker));
		if (wDataSize != sizeof(CMD_S_ApplyBanker)) return false;

		//消息处理
		CMD_S_ApplyBanker * pApplyBanker = (CMD_S_ApplyBanker *)pBuffer;

		//插入玩家
// 	if (m_wCurrentBanker != pApplyBanker->wApplyUser && !IsApplyedBanker(pApplyBanker->wApplyUser))
// 	{
// 		m_AppyBankerAdroidUserArray.Add(pApplyBanker->wApplyUser);
// 	}
		if (pApplyBanker->wApplyUser == m_pIAndroidUserItem->GetChairID())
			m_bAppyBanker = true;

		m_AppyBankerAdroidUserCount++;

		return true;
	}
	catch (...)
	{

	}
}

//取消申请
bool CAndroidUserItemSink::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	try
	{
		//效验数据
		ASSERT(wDataSize == sizeof(CMD_S_CancelBanker));
		if (wDataSize != sizeof(CMD_S_CancelBanker)) return false;

		//消息处理
		CMD_S_CancelBanker * pCancelBanker = (CMD_S_CancelBanker *)pBuffer;


// 	//删除玩家
// 	WORD wCancelUserID = INVALID_CHAIR;
// 	for (int i = 1; i < GAME_PLAYER; i++)
// 	{
// 		IServerUserItem *pServerUserItem = m_pIAndroidUserItem->GetTableUserItem(i);
// 		if (pServerUserItem)
// 		{
// 			tagServerUserData *pUserData = pServerUserItem->GetUserData();
// 			if (pUserData)
// 			{
// 				if (lstrcmp(pUserData->szAccounts, pCancelBanker->szCancelUser) == 0)
// 				{
// 					wCancelUserID = i;
// 					break;
// 				}
// 			}
// 		}
// 	}

// 	if (wCancelUserID != INVALID_CHAIR)
// 	{
// 		for (int i = 0; i < m_AppyBankerAdroidUserArray.GetCount(); i++)
// 		{
// 			if (m_AppyBankerAdroidUserArray[i] == wCancelUserID)
// 				m_AppyBankerAdroidUserArray.RemoveAt(i);
// 		}
// 	}

// 	if (pCancelBanker->szCancelUser == m_pIAndroidUserItem->GetChairID())
// 		m_bAppyBanker = false;

		m_AppyBankerAdroidUserCount--;
	}
	catch (...)
	{

	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
