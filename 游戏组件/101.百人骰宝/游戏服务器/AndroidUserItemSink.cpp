#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////
//辅助时间
#define TIME_LESS					2									//最少时间
#define TIME_JETTON				    1									//下注时间
#define TIME_RAND				    5									//随机时间
#define TIME_APPLY_BANKER           20                                  // 20秒后检查

//游戏时间
#define IDI_USER_JETTON			(IDI_ANDROID_ITEM_SINK+0)			    // 下注定时器ID
#define IDI_PLACE_JETTON        (IDI_ANDROID_ITEM_SINK+1)               // 下注阶段
#define IDI_APPLY_BANKER        (IDI_ANDROID_ITEM_SINK+2)               // 申请上庄
#define IDI_CANCEL_BANKER       (IDI_ANDROID_ITEM_SINK+3)               // 申请下庄
#define IDI_SELECT_SICBOTYPE    (IDI_ANDROID_ITEM_SINK+4)               // 选择摇骰子方式

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	m_lMeMaxScore =0;						         // 最大下注
	m_lAreaLimitScore=0;					         // 区域限制
	m_lApplyBankerCondition=0;			             // 申请条件
	ZeroMemory(m_lMeScore,sizeof(m_lMeScore));       // 每个方位总注
	ZeroMemory(m_lAllScore,sizeof(m_lAllScore));	 // 每个方位总注
	m_lBankerScore = 0;						         // 庄家积分
	m_wCurrentBanker = INVALID_CHAIR;	             // 当前庄家
	m_pIAndroidUserItem = NULL;
	m_bEnableNote = false;                           // 是否可以下注
	m_bEnableSysBanker = false;                      // 是否可以系统坐庄
	m_iMaxAndroidBanker = MAX_ANDROID_BANKER;
	m_lAllMaxScore=0L;                     // 当局可以下注的总注
	m_arApplyBanker.RemoveAll();
	return;
}

//析构函数
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//接口查询
void * __cdecl CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//初始接口
bool __cdecl CAndroidUserItemSink::InitUserItemSink(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}

//重置接口
bool __cdecl CAndroidUserItemSink::RepositUserItemSink()
{
	m_lMeMaxScore =0;						         // 最大下注
	m_lAreaLimitScore=0;					         // 区域限制
	m_lApplyBankerCondition=0;			             // 申请条件
	ZeroMemory(m_lMeScore,sizeof(m_lMeScore));       // 每个方位总注
	ZeroMemory(m_lAllScore,sizeof(m_lAllScore));	 // 每个方位总注
	m_lBankerScore = 0;						         // 庄家积分
	m_wCurrentBanker = INVALID_CHAIR;	             // 当前庄家
	m_bEnableNote = false;
	m_lAllMaxScore=0L;                     // 当局可以下注的总注
	return true;
}

//时间消息
bool __cdecl CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_USER_JETTON:	// 机器人下注
		{
			return OnPlaceJetton();
		}
	case IDI_PLACE_JETTON:	// 总的下注时间到了
		{
			if (NULL != m_pIAndroidUserItem)
			{
				m_pIAndroidUserItem->KillGameTimer(IDI_USER_JETTON);
			}
			m_bEnableNote = false;
			return true;
		}
	case IDI_APPLY_BANKER:
		{
			if (m_arApplyBanker.GetCount() >= m_iMaxAndroidBanker)
			{
				return true;
			}
			//合法判断
			IServerUserItem *pMeUserItem = m_pIAndroidUserItem->GetMeUserItem();
			if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID())
			{
				return true;
			}

			// 申请上庄
			if (pMeUserItem->GetUserScore()->lScore < m_lApplyBankerCondition)
			{
				return true;
			}

			// 自己要不再队列中
			for (int i=0; i<m_arApplyBanker.GetCount(); ++i)
			{
				if (m_arApplyBanker[i] == m_pIAndroidUserItem->GetUserID())
				{
					return true;
				}
			}

			//发送消息
			WORD wChairID = INVALID_CHAIR;
			m_pIAndroidUserItem->SendSocketData(SUB_C_APPLY_BANKER, (void *)&wChairID, sizeof(wChairID));
			return true;
		}
	case IDI_CANCEL_BANKER:
		{
			if (m_arApplyBanker.GetCount() < m_iMaxAndroidBanker)
			{
				return true;
			}

			//合法判断
			IServerUserItem *pMeUserItem = m_pIAndroidUserItem->GetMeUserItem();
			if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID())
			{
				return true;
			}

			// 自己在申请的庄家队列中
			for (int i=0; i<m_arApplyBanker.GetCount(); ++i)
			{
				if (m_arApplyBanker[i] == m_pIAndroidUserItem->GetUserID())
				{
					//发送消息
					WORD wChairID = INVALID_CHAIR;
					m_pIAndroidUserItem->SendSocketData(SUB_C_CANCEL_BANKER, (void *)&wChairID, sizeof(wChairID));
					return true;
				}
			}
			return true;
		}
	case IDI_SELECT_SICBOTYPE:
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_SELECT_SICBOTYPE);
			if (m_wCurrentBanker != m_pIAndroidUserItem->GetChairID())
			{
				return true;
			}
			// 发送摇骰子方式
			CMD_C_ChangeSicboType cmdChangeSicboType;
			ZeroMemory(&cmdChangeSicboType, sizeof(cmdChangeSicboType));
			cmdChangeSicboType.enSicboType = (E_SICBO_TYPE)(rand()%enSicboType_InTheAir);
			m_pIAndroidUserItem->SendSocketData(SUB_C_CHANGE_SICBO_TYPE, &cmdChangeSicboType, sizeof(cmdChangeSicboType));
			return true;
		}
	default: break;
	}
	return false;
}

//游戏消息
bool __cdecl CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_FREE:		//游戏空闲
		{
			m_lMeMaxScore =0;						         // 最大下注
			m_lAllMaxScore=0L;                     // 当局可以下注的总注
			ZeroMemory(m_lMeScore,sizeof(m_lMeScore));       // 每个方位总注
			ZeroMemory(m_lAllScore,sizeof(m_lAllScore));	 // 每个方位总注
			m_bEnableNote = false;
			return  true;
		}
	case SUB_S_GAME_START:		// 游戏开始
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_GameStart));
			if (wDataSize!=sizeof(CMD_S_GameStart))
			{
				return false;
			}

			//消息处理
			CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

			ZeroMemory(m_lMeScore, sizeof(m_lMeScore));
			ZeroMemory(m_lAllScore, sizeof(m_lAllScore));

			m_lMeMaxScore=pGameStart->lUserMaxScore;
			m_wCurrentBanker = pGameStart->wBankerUser;
			m_lBankerScore = pGameStart->lBankerScore;
			m_lAllMaxScore=pGameStart->lAllMaxScore;               // 当局可以下注的总注
			if (NULL != m_pIAndroidUserItem)
			{
				m_bEnableNote = true;
				UINT nElapse=(rand()%TIME_RAND) +TIME_LESS;
				if (pGameStart->cbTimeLeave>(TIME_LESS+nElapse))
				{
					if (m_wCurrentBanker != m_pIAndroidUserItem->GetChairID())
					{
						m_pIAndroidUserItem->SetGameTimer(IDI_USER_JETTON,nElapse);
						m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON,pGameStart->cbTimeLeave-TIME_LESS);
					}
					else
					{
						//m_pIAndroidUserItem->SetGameTimer(IDI_SELECT_SICBOTYPE,nElapse);
					}
				}
			}

			//设置文件名
			TCHAR szPath[MAX_PATH]=TEXT("");
			TCHAR szINIPath[MAX_PATH] = TEXT("");
			GetCurrentDirectory(sizeof(szPath),szPath);
			_snprintf(szINIPath,sizeof(szINIPath),TEXT("%s\\SicboShowHandConfig.ini"),szPath);
			iRandLimit[0]=GetPrivateProfileInt("RandLimit","Area0",200,szINIPath);
			iRandLimit[1]=GetPrivateProfileInt("RandLimit","Area1",200,szINIPath);
			iRandLimit[2]=GetPrivateProfileInt("RandLimit","Area2",-1,szINIPath);
			iRandLimit[3]=GetPrivateProfileInt("RandLimit","Area3",20,szINIPath);
			iRandLimit[4]=GetPrivateProfileInt("RandLimit","Area4",60,szINIPath);
			iRandLimit[5]=GetPrivateProfileInt("RandLimit","Area5",85,szINIPath);
			iRandLimit[6]=GetPrivateProfileInt("RandLimit","Area6",97,szINIPath);
			return true;
		}
	case SUB_S_PLACE_JETTON:	//用户加注
		{
			return OnSubPlaceJetton(pBuffer,wDataSize);
		}
	case SUB_S_APPLY_BANKER:	//申请做庄
		{
			return OnSubUserApplyBanker(pBuffer,wDataSize);
		}
	case SUB_S_CANCEL_BANKER:	//取消做庄
		{
			return OnSubUserCancelBanker(pBuffer,wDataSize);
		}
	case SUB_S_CHANGE_BANKER:	// 切换庄家
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
			if (wDataSize!=sizeof(CMD_S_ChangeBanker))
			{
				return false;
			}

			//消息处理
			CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

			//庄家信息
			m_wCurrentBanker = pChangeBanker->wBankerUser;
			m_lBankerScore = pChangeBanker->lBankerScore;
			m_iMaxAndroidBanker = 1 + rand()%MAX_ANDROID_BANKER;

			// 删除玩家
			for (int i=0; i<m_arApplyBanker.GetCount(); ++i)
			{
				if (m_arApplyBanker[i] == pChangeBanker->dwUserID)
				{
					m_arApplyBanker.RemoveAt(i);
					if (m_arApplyBanker.GetCount() < m_iMaxAndroidBanker)
					{
						m_pIAndroidUserItem->SetGameTimer(IDI_APPLY_BANKER, (TIME_LESS+rand()%TIME_APPLY_BANKER));	
					}
					return true;
				}
			}
			return true;
		}
	case SUB_S_GAME_END:		//游戏结束
		{
			m_bEnableNote = false;
			// 清除定时器
			if (NULL != m_pIAndroidUserItem)
			{				
				m_pIAndroidUserItem->KillGameTimer(IDI_USER_JETTON);
				m_pIAndroidUserItem->KillGameTimer(IDI_SELECT_SICBOTYPE);
				if (INVALID_CHAIR == m_wCurrentBanker)
				{
					m_arApplyBanker.RemoveAll();
					m_pIAndroidUserItem->SetGameTimer(IDI_APPLY_BANKER, (TIME_LESS+rand()%TIME_APPLY_BANKER));
				}
			}
			return true;
		}
	case SUB_S_SEND_RECORD:		//游戏记录
	case SUB_C_CHANGE_SICBO_TYPE:
		{
			// 机器人不需要处理
			return true;
		}
	case SUB_S_PLACE_JETTON_FAIL:	//下注失败
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_PlaceJettonFail));
			if (wDataSize!=sizeof(CMD_S_PlaceJettonFail))
			{
				return false;
			}

			//消息处理
			CMD_S_PlaceJettonFail * pPlaceJettonFail=(CMD_S_PlaceJettonFail *)pBuffer;

			//效验参数
			BYTE cbViewIndex=pPlaceJettonFail->lJettonArea;
			ASSERT(cbViewIndex<COUNT_AZIMUTH);
			if (cbViewIndex>=COUNT_AZIMUTH)
			{
				return false;
			}

			__int64 lJettonCount=pPlaceJettonFail->lPlaceScore;
			m_lMeScore[cbViewIndex] -= lJettonCount;
			if (m_lMeScore[cbViewIndex] < 0)
			{
				m_lMeScore[cbViewIndex] = 0;
			}
			return true;
		}
	default:break;
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
	switch (cbGameStatus)
	{
	case GS_FREE:			//空闲状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//消息处理
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			//玩家信息
			m_lMeMaxScore=pStatusFree->lUserMaxScore;
			m_lAllMaxScore = 0;
			m_wCurrentBanker = pStatusFree->wBankerUser;
			m_lBankerScore = pStatusFree->lBankerScore;

			//控制信息
			m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusFree->lAreaLimitScore;
			m_bEnableSysBanker = pStatusFree->bEnableSysBanker;
			if (INVALID_CHAIR == m_wCurrentBanker)
			{
				m_pIAndroidUserItem->SetGameTimer(IDI_APPLY_BANKER, TIME_LESS);	
			}
			else
			{
				m_pIAndroidUserItem->SetGameTimer(IDI_APPLY_BANKER, TIME_APPLY_BANKER);	
			}
			return true;
		}
	case GS_PLAYING:		//游戏状态
	case GS_GAME_END:		//结束状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//消息处理
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			// 绘制下注数字			
			for (BYTE i=0; i<COUNT_AZIMUTH; ++i)
			{
				//全局下注
				m_lAllScore[i] = pStatusPlay->lAllScore[i];
				m_lMeScore[i] = pStatusPlay->lUserScore[i];
			}

			//玩家积分
			m_lMeMaxScore=pStatusPlay->lUserMaxScore;

			//控制信息
			m_lApplyBankerCondition=pStatusPlay->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusPlay->lAreaLimitScore;
			m_lAllMaxScore=pStatusPlay->lAllMaxScore;               // 当局可以下注的总注
	
			//庄家信息
			m_wCurrentBanker = pStatusPlay->wBankerUser;
			m_lBankerScore = pStatusPlay->lBankerScore;
			m_bEnableSysBanker = pStatusPlay->bEnableSysBanker;
			return true;
		}
	}

	return true;
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

// 所有玩家都开始了
void __cdecl CAndroidUserItemSink::OnEventAllUserStart(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return ;
}

// 下注处理
bool CAndroidUserItemSink::OnPlaceJetton(void)
{
	if (NULL == m_pIAndroidUserItem)
	{
		m_bEnableNote = false;
		return true;
	}
	//庄家判断
	if ( m_pIAndroidUserItem->GetChairID() == m_wCurrentBanker ) 
	{
		m_bEnableNote = false;
		return true;
	}
	if ((INVALID_CHAIR == m_wCurrentBanker)
		&&(!m_bEnableSysBanker)) 
	{
		m_bEnableNote = false;
		return true;
	}

	if (m_bEnableNote)
	{
		__int64 lUserNote[7]={100L,1000L,10000L,100000L,500000L,1000000L,5000000L};
		 
		int iRandArea[COUNT_AZIMUTH] = {-1,114,228,340,                   // 大，小，单，双 58/1000
			452,453,455,460,470,482,494,506,518,530,535,540,543,545,     //点数
			546,576,606,636,666,696,                                     // 点 30/1000
			726,741,756,771,786,801,816,831,846,861,876,881,896,911,926, // 俩个15/1000
			941,949,957,965,973,981,                                     // 对子8/1000 *12
			989,990,991,992,993,994,995};                                // 三同1/1000

		// 先选定区域，然后再定下多少注
		int iRand = rand()%1000;  // 下注区域
		BYTE byArea = enCardType_Big;
		for (BYTE i=(COUNT_AZIMUTH-1); i>=0; --i)
		{
			if (iRand > iRandArea[i])
			{
				byArea = i;
				break;
			}
		}

		// 获取该区域的最大下注数
		__int64 lMaxPlayerScore = GetMaxAndroidScore(byArea);
		if (lMaxPlayerScore >= lUserNote[0])
		{
			__int64 lJettonScore = lUserNote[0];
			iRand = rand()%100;
			for (int i=6; i>=0; --i)
			{
				if (iRand > iRandLimit[i])
				{
					if (lMaxPlayerScore >=lUserNote[i])
					{
						lJettonScore = lUserNote[i];
					}
					break;
				}
			}

			//设置变量
			m_lMeScore[byArea] += lJettonScore;

			//变量定义
			CMD_C_PlaceJetton PlaceJetton;
			ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

			// 构造变量
			PlaceJetton.cbJettonArea=byArea;
			PlaceJetton.lJettonScore=lJettonScore;

			//发送消息
			m_pIAndroidUserItem->SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

			// 启动下次下注
			UINT nElapse=rand()%TIME_RAND+TIME_JETTON;
			m_pIAndroidUserItem->SetGameTimer(IDI_USER_JETTON,nElapse);
		}
		else
		{
			m_bEnableNote = false;
		}				
	}
	return true;
}

//用户加注
bool CAndroidUserItemSink::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{
	if (NULL == m_pIAndroidUserItem)
	{
		return true;
	}
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) return false;

	//消息处理
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;
	if ((pPlaceJetton->cbJettonArea>=COUNT_AZIMUTH) || (pPlaceJetton->lJettonScore<0))
	{
		return true;
	}

	// 更新玩家下注界面
	m_lAllScore[pPlaceJetton->cbJettonArea] += pPlaceJetton->lJettonScore;
	return true;
}

//申请做庄
bool CAndroidUserItemSink::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;

	//消息处理
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;

	// 查找一遍是否重复
	for (int i=0; i<m_arApplyBanker.GetCount(); ++i)
	{
		if (pApplyBanker->dwUserID == m_arApplyBanker[i])
		{
			return true;
		}
	}
	m_arApplyBanker.Add(pApplyBanker->dwUserID);

	// 队列中玩家过多时申请下庄
	if ((m_arApplyBanker.GetCount() > m_iMaxAndroidBanker)
		&& (m_wCurrentBanker != m_pIAndroidUserItem->GetChairID()))
	{
		//合法判断
		IServerUserItem *pMeUserItem = m_pIAndroidUserItem->GetMeUserItem();
		if (NULL == pMeUserItem)
		{
			return true;
		}
		m_pIAndroidUserItem->SetGameTimer(IDI_CANCEL_BANKER, (TIME_LESS+rand()%TIME_APPLY_BANKER));
	}
	return true;
}

//取消做庄
bool CAndroidUserItemSink::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;

	//消息处理
	CMD_S_CancelBanker * pCancelBanker=(CMD_S_CancelBanker *)pBuffer;

	// 删除玩家
	for (int i=0; i<m_arApplyBanker.GetCount(); ++i)
	{
		if (m_arApplyBanker[i] == pCancelBanker->dwUserID)
		{
			m_arApplyBanker.RemoveAt(i);
			if (m_arApplyBanker.GetCount() < m_iMaxAndroidBanker)
			{
				m_pIAndroidUserItem->SetGameTimer(IDI_APPLY_BANKER, (TIME_LESS+rand()%TIME_APPLY_BANKER));				
			}
			return true;
		}
	}
	return true;
}

// 最大下注
__int64 CAndroidUserItemSink::GetMaxAndroidScore(int iArea)
{
	if ((iArea < 0) || (iArea>=COUNT_AZIMUTH))
	{
		return 0L;
	}
	__int64 i64UserAllScore = 0; // 玩家自己已经下注的分
	__int64 i64AllUserAllScore=0;// 所有玩家下注数 
	__int64 i64AllLossScore=0;   // 庄家总赔数
	__int64 i64LoseAreaScore=0;  // 庄家输的区域下注数
	for (int i=0; i<COUNT_AZIMUTH; ++i)
	{
		i64UserAllScore += m_lMeScore[i];
		i64AllUserAllScore += m_lAllScore[i];
	}
	// 机器人限制
	if (i64AllUserAllScore > 350000000L)
	{
		m_bEnableNote = false;
		return 0L;
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
				__int64 i64LoseAreaScoreTemp=0;  // 庄家输的区域下注数
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

				// 算出这手牌输多少钱
				for (int l=0; l<iCount; ++l)
				{
					if (enCardType_Illegal == enCardType[l])
					{
						continue;
					}
					i64LoseAreaScoreTemp +=m_lAllScore[enCardType[l]];  // 庄家输的区域下注数
					if ((enCardType[l]<enCardType_SicboOne) ||(enCardType[l]>enCardType_SicboSix))
					{
						i64AllLossScoreTemp += (m_lAllScore[enCardType[l]] * m_GameLogic.m_i64Loss_Percent[enCardType[l]]);
					}
					else
					{
						// 需要换算个数
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
	//OUTPUT_DEBUG_STRING(_T("client %d区域下注庄家最多输 %I64d"),iArea, i64AllLossScore);

	// 获取基本上限
	__int64 lAllMaxScore = m_lAllMaxScore;
	__int64 lUserMaxScore = m_lMeMaxScore;

	// 减去已经下注的
	lUserMaxScore -= i64UserAllScore;
	if (lUserMaxScore<=0)
	{
		return 0L;
	}

	OUTPUT_DEBUG_STRING(_T("adroid 最大下注 %I64d, 所有玩家下注 %I64d,  庄家当前输 %I64d  玩家下注区域%d"),
		lAllMaxScore, i64AllUserAllScore, i64AllLossScore, iArea);

	// 庄家可以赔的钱
	lAllMaxScore = lAllMaxScore - i64LoseAreaScore + i64AllUserAllScore;
	lAllMaxScore -= i64AllLossScore;
	if ((iArea<enCardType_SicboOne) ||(iArea>enCardType_SicboSix))
	{
		lAllMaxScore = lAllMaxScore/(m_GameLogic.m_i64Loss_Percent[iArea] -1);
	}
	else
	{
		lAllMaxScore = lAllMaxScore/3; // 以最大倍数算
	}
	if (lAllMaxScore<=0)
	{
		return 0L;
	}
	lUserMaxScore = min(lAllMaxScore, lUserMaxScore);
	return lUserMaxScore;
}

//////////////////////////////////////////////////////////////////////////
