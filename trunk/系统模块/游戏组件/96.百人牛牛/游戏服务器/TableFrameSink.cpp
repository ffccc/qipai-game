#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//常量定义
#define SEND_COUNT					100									//发送次数

//索引定义
#define INDEX_PLAYER				0									//闲家索引
#define INDEX_BANKER				1									//庄家索引

//时间定义
#define IDI_FREE					1									//空闲时间
#define IDI_PLACE_JETTON			2									//下注时间
#define IDI_GAME_END				3									//结束时间


//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//游戏人数
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_TimeControl;	//开始模式

//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{
	//总下注数
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//个人下注
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//控制变量
	m_cbWinSideControl=0;
	m_nSendCardCount=0;

	//玩家成绩	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//扑克信息
	ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

	//状态变量
	m_dwJettonTime=0L;
	m_bExchangeBanker=true;
	m_wAddTime=0;

	//庄家信息
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker=INVALID_CHAIR;
	m_wBankerTime=0;
	m_lBankerWinScore=0L;		
	m_lBankerCurGameScore=0L;
	m_bEnableSysBanker=true;
	m_cbLeftCardCount=0;
	m_bContiueCard=false;

	//记录变量
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;
	m_nRecordLast=0;
	m_dwRecordCount=0;

	m_cbExcuteTimes = 0;								
	m_cbControlStyle = 0;	
	m_cbWinAreaCount = 0;
	ZeroMemory(m_bWinArea, sizeof(m_bWinArea));	
	ZeroMemory(m_nCompareCard, sizeof(m_nCompareCard));

	//机器人控制
	m_nChipRobotCount = 0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
}

//接口查询
void * __cdecl CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);	
#ifdef __BANKER___
	QUERYINTERFACE(ITableUserActionEX,Guid,dwQueryVer);	
#endif
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//初始化
bool __cdecl CTableFrameSink::InitTableFrameSink(IUnknownEx * pIUnknownEx)
{
	//查询接口
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//控制接口
	m_pITableFrameControl=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrameControl);
	if (m_pITableFrameControl==NULL) return false;

	//设置文件名
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_snprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\OxBattleConfig.ini"),szPath);

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//房间名称
	memcpy(m_szRoomName, m_pGameServiceOption->szGameRoomName, sizeof(m_pGameServiceOption->szGameRoomName));

	ReadConfigInformation(true);	

	return true;
}

//复位桌子
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//总下注数
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//个人下注
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//玩家成绩	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//机器人控制
	m_nChipRobotCount = 0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

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
	//变量定义
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));

	//获取庄家
	IServerUserItem *pIBankerServerUserItem=NULL;
	if (INVALID_CHAIR!=m_wCurrentBanker) 
	{
		pIBankerServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		m_lBankerScore=pIBankerServerUserItem->GetUserScore()->lScore;
	}

	//设置变量
	GameStart.cbTimeLeave=m_nPlaceJettonTime;
	GameStart.wBankerUser=m_wCurrentBanker;
	if (pIBankerServerUserItem!=NULL) GameStart.lBankerScore=pIBankerServerUserItem->GetUserScore()->lScore;
	GameStart.bContiueCard=m_bContiueCard;

	//下注机器人数量
	int nChipRobotCount = 0;
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
			nChipRobotCount++;
	}

	GameStart.nChipRobotCount = min(nChipRobotCount, m_nMaxChipRobot);

	MyDebug(_T("机器人数量 %d 下注机器人上限 %d"), nChipRobotCount, m_nMaxChipRobot);

	//机器人控制
	m_nChipRobotCount = 0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

	//旁观玩家
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	

	//游戏玩家
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;
		int iTimer = 10;

		//设置积分
		GameStart.lUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_lUserLimitScore*iTimer);

		m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	
	}

	return true;
}

//游戏结束
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束	
		{
			//游戏作弊
			if(m_cbControlStyle>0&&m_cbExcuteTimes>0)
			{
				//获取控制目标牌型
				GetSuitResult();
				m_cbExcuteTimes--;
			}
			else
			{
				GameCheat();
			}

			//计算分数
			LONGLONG lBankerWinScore=CalculateScore();

			//递增次数
			m_wBankerTime++;

			//结束消息
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//庄家信息
			GameEnd.nBankerTime = m_wBankerTime;
			GameEnd.lBankerTotallScore=m_lBankerWinScore;
			GameEnd.lBankerScore=lBankerWinScore;
			GameEnd.bcFirstCard = m_bcFirstPostCard;

			//扑克信息
			CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
			GameEnd.cbLeftCardCount=m_cbLeftCardCount;

			//发送积分
			GameEnd.cbTimeLeave=m_nGameEndTime;	
			for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserIndex);
				if ( pIServerUserItem == NULL ) continue;

				//设置成绩
				GameEnd.lUserScore=m_lUserWinScore[wUserIndex];

				//返还积分
				GameEnd.lUserReturnScore=m_lUserReturnScore[wUserIndex];

				//设置税收
				if (m_lUserRevenue[wUserIndex]>0) GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
				else if (m_wCurrentBanker!=INVALID_CHAIR) GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
				else GameEnd.lRevenue=0;

				//发送消息					
				m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			return true;
		}
	case GER_USER_LEFT:		//用户离开
		{
			//闲家判断
			if (m_wCurrentBanker!=wChairID)
			{
				//变量定义
				LONGLONG lRevenue=0;

				//是否开牌
				if (m_pITableFrame->GetGameStatus() == GS_PLACE_JETTON)
				{
					//返回下注
					for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
					{
						if (m_lUserJettonScore[nAreaIndex][wChairID] != 0)
						{
							CMD_S_PlaceJettonFail PlaceJettonFail;
							ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
							PlaceJettonFail.lJettonArea=nAreaIndex;
							PlaceJettonFail.lPlaceScore=m_lUserJettonScore[nAreaIndex][wChairID];
							PlaceJettonFail.wPlaceUser=wChairID;

							//游戏玩家
							for (WORD i=0; i<GAME_PLAYER; ++i)
							{
								IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
								if (pIServerUserItem==NULL) 
									continue;

								m_pITableFrame->SendTableData(i,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
							}

							m_lAllJettonScore[nAreaIndex] -= m_lUserJettonScore[nAreaIndex][wChairID];
							m_lUserJettonScore[nAreaIndex][wChairID] = 0;
						}
					}
				}
				else
				{
					//胜利类型
					enScoreKind ScoreKind=(m_lUserWinScore[wChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;

					//清除下注
					for (int nAreaIndex = 1; nAreaIndex <= AREA_COUNT; nAreaIndex++)
					{
						m_lUserJettonScore[nAreaIndex][wChairID] = 0;
					}

					//写入积分
					if (m_lUserWinScore[wChairID]!=0L) 
					{
						//库存金币
						if (!pIServerUserItem->IsAndroidUser())
							m_lStorageStart -= m_lUserWinScore[wChairID];

						m_pITableFrame->WriteUserScore(pIServerUserItem,m_lUserWinScore[wChairID], static_cast<long>(m_lUserRevenue[wChairID]), ScoreKind);
						m_lUserWinScore[wChairID] = 0;
					}
				}

				return true;
			}

			//状态判断
			if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
			{
				//提示消息
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("由于庄家[ %s ]强退，游戏提前结束！"),pIServerUserItem->GetAccounts());

				//发送消息
				SendGameMessage(INVALID_CHAIR,szTipMsg);	

				//设置状态
				m_pITableFrame->SetGameStatus(GS_GAME_END);

				//设置时间
				m_dwJettonTime=(DWORD)time(NULL);
				m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_nGameEndTime*1000,1,0L);

				if(m_cbControlStyle>0&&m_cbExcuteTimes>0)
				{
					//获取控制目标牌型
					GetSuitResult();
					m_cbExcuteTimes--;
				}

				//计算分数
				CalculateScore();

				//结束消息
				CMD_S_GameEnd GameEnd;
				ZeroMemory(&GameEnd,sizeof(GameEnd));

				//庄家信息
				GameEnd.nBankerTime = m_wBankerTime;
				GameEnd.lBankerTotallScore=m_lBankerWinScore;
				GameEnd.lBankerScore=m_lUserWinScore[m_wCurrentBanker];

				//扑克信息
				CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
				GameEnd.cbLeftCardCount=m_cbLeftCardCount;

				//发送积分
				GameEnd.cbTimeLeave=m_nGameEndTime;	
				for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
				{
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserIndex);
					if ( pIServerUserItem == NULL ) continue;

					//设置成绩
					GameEnd.lUserScore=m_lUserWinScore[wUserIndex];

					//返还积分
					GameEnd.lUserReturnScore=m_lUserReturnScore[wUserIndex];

					//设置税收
					if (m_lUserRevenue[wUserIndex]>0) GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
					else if (m_wCurrentBanker!=INVALID_CHAIR) GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
					else GameEnd.lRevenue=0;

					//发送消息					
					m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				}
			}

			//扣除分数
			m_pITableFrame->WriteUserScore(m_wCurrentBanker,m_lUserWinScore[m_wCurrentBanker], m_lUserRevenue[m_wCurrentBanker], (m_lUserWinScore[m_wCurrentBanker]>0?enScoreKind_Win:enScoreKind_Lost));

			//库存金币
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
			if (pIServerUserItem != NULL && !pIServerUserItem->IsAndroidUser())
				m_lStorageStart -= m_lUserWinScore[m_wCurrentBanker];

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
			ZeroMemory(&StatusFree,sizeof(StatusFree));			

			//控制信息
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusFree.lAreaLimitScore = m_lAreaLimitScore;

			//庄家信息
			StatusFree.bEnableSysBanker=m_bEnableSysBanker;
			StatusFree.wBankerUser=m_wCurrentBanker;	
			StatusFree.cbBankerTime=m_wBankerTime;
			StatusFree.lBankerWinScore=m_lBankerWinScore;
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
				StatusFree.lBankerScore=pIServerUserItem->GetUserScore()->lScore;
			}

			//玩家信息
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusFree.lUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_lUserLimitScore*10); 
			}

			//房间名称
			CopyMemory(StatusFree.szGameRoomName, m_pGameServiceOption->szGameRoomName, sizeof(StatusFree.szGameRoomName));

			//全局信息
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusFree.cbTimeLeave=(BYTE)(m_nFreeTime-__min(dwPassTime,(DWORD)m_nFreeTime));

			//发送场景
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

			//限制提示
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄条件为：%I64d,区域限制为：%I64d,玩家限制为：%I64d"),m_lApplyBankerCondition,
				m_lAreaLimitScore,m_lUserLimitScore);

			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);

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
			CMD_S_StatusPlay StatusPlay={0};

			//全局下注
			CopyMemory(StatusPlay.lAllJettonScore,m_lAllJettonScore,sizeof(StatusPlay.lAllJettonScore));

			//玩家下注
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
					StatusPlay.lUserJettonScore[nAreaIndex] = m_lUserJettonScore[nAreaIndex][wChiarID];

				//最大下注
				StatusPlay.lUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_lUserLimitScore*10);
			}

			//控制信息
			StatusPlay.lApplyBankerCondition=m_lApplyBankerCondition;		
			StatusPlay.lAreaLimitScore=m_lAreaLimitScore;		

			//庄家信息
			StatusPlay.bEnableSysBanker=m_bEnableSysBanker;
			StatusPlay.wBankerUser=m_wCurrentBanker;			
			StatusPlay.cbBankerTime=m_wBankerTime;
			StatusPlay.lBankerWinScore=m_lBankerWinScore;	
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				StatusPlay.lBankerScore=m_lBankerScore;
			}	

			//全局信息
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			int	nTotalTime = (cbGameStatus==GS_PLACE_JETTON?m_nPlaceJettonTime:m_nGameEndTime);
			StatusPlay.cbTimeLeave=(BYTE)(nTotalTime-__min(dwPassTime,(DWORD)nTotalTime));
			StatusPlay.cbGameStatus=m_pITableFrame->GetGameStatus();			

			//房间名称
			CopyMemory(StatusPlay.szGameRoomName, m_pGameServiceOption->szGameRoomName, sizeof(StatusPlay.szGameRoomName));

			//结束判断
			if (cbGameStatus==GS_GAME_END)
			{
				//设置成绩
				StatusPlay.lEndUserScore=m_lUserWinScore[wChiarID];

				//返还积分
				StatusPlay.lEndUserReturnScore=m_lUserReturnScore[wChiarID];

				//设置税收
				if (m_lUserRevenue[wChiarID]>0) StatusPlay.lEndRevenue=m_lUserRevenue[wChiarID];
				else if (m_wCurrentBanker!=INVALID_CHAIR) StatusPlay.lEndRevenue=m_lUserRevenue[m_wCurrentBanker];
				else StatusPlay.lEndRevenue=0;

				//庄家成绩
				StatusPlay.lEndBankerScore=m_lBankerCurGameScore;

				//扑克信息
				CopyMemory(StatusPlay.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
			}

			//发送场景
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

			//限制提示
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄条件为：%I64d,区域限制为：%I64d,玩家限制为：%I64d"),m_lApplyBankerCondition,
				m_lAreaLimitScore,m_lUserLimitScore);
			
			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);
			
			//发送申请者
			SendApplyUser( pIServerUserItem );

			return bSuccess;
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
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,m_nPlaceJettonTime*1000,1,0L);

			//派发扑克
			DispatchTableCard();

			//设置状态
			m_pITableFrame->SetGameStatus(GS_PLACE_JETTON);

			return true;
		}
	case IDI_PLACE_JETTON:		//下注时间
		{
			//状态判断(防止强退重复设置)
			if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
			{
				//设置状态
				m_pITableFrame->SetGameStatus(GS_GAME_END);			

				//结束游戏
				OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);

				//设置时间
				m_dwJettonTime=(DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_nGameEndTime*1000,1,0L);		
			}

			return true;
		}
	case IDI_GAME_END:			//结束游戏
		{
			//写入积分
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserChairID);
				if ( pIServerUserItem == NULL ) continue;

				//胜利类型
				enScoreKind ScoreKind=(m_lUserWinScore[wUserChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;

				//写入积分
				if (m_lUserWinScore[wUserChairID]!=0L) m_pITableFrame->WriteUserScore(wUserChairID,m_lUserWinScore[wUserChairID], m_lUserRevenue[wUserChairID], ScoreKind);

				//MyDebug(_T("库存金币 A %I64d [%d %I64d]"), m_lStorageStart, wUserChairID, m_lUserWinScore[wUserChairID]);

				//库存金币
				if (!pIServerUserItem->IsAndroidUser())
					m_lStorageStart -= m_lUserWinScore[wUserChairID];
			}

			//提示消息	JJWillDo 删除库存提示
			/*TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("%I64d - %I64d*%d/1000 = %I64d"), m_lStorageStart, m_lStorageStart, m_nStorageDeduct, m_lStorageStart - m_lStorageStart*m_nStorageDeduct/1000);
			SendGameMessage(INVALID_CHAIR,szTipMsg);*/

			//库存衰减
			if (m_lStorageStart > 0)
				m_lStorageStart = m_lStorageStart - m_lStorageStart*m_nStorageDeduct/1000;
					AddMoney();

			//结束游戏
			m_pITableFrame->ConcludeGame();

			//读取配置
			if (m_bRefreshCfg)
				ReadConfigInformation(false);

			//切换庄家
			ChangeBanker(false);

			//设置时间
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,m_nFreeTime*1000,1,0L);

			//发送消息
			CMD_S_GameFree GameFree;
			ZeroMemory(&GameFree,sizeof(GameFree));
			GameFree.cbTimeLeave=m_nFreeTime;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));

			return true;
		}
	}

	return false;
}


bool CTableFrameSink::OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const void*pDataBuffer)
{

	//如果不具有管理员权限 则返回错误
	if((pIServerUserItem->GetUserData()->dwUserRight&UR_GAME_CONTROL)==0)
	{
		return false;
	}

	const CMD_C_AdminReq* AdminReq=static_cast<const CMD_C_AdminReq*>(pDataBuffer);
	switch(AdminReq->cbReqType)
	{
	case RQ_RESET_CONTROL:
		{	
			m_cbControlStyle=0;
			m_cbWinAreaCount=0;
			m_cbExcuteTimes=0;
			ZeroMemory(m_bWinArea,sizeof(m_bWinArea));

			CMD_S_CommandResult cResult;
			cResult.cbResult=CR_ACCEPT;
			cResult.cbAckType=ACK_RESET_CONTROL;
			m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
			break;
		}
	case RQ_SET_WIN_AREA:
		{

			const tagAdminReq*pAdminReq=reinterpret_cast<const tagAdminReq*>(AdminReq->cbExtendData);
			switch(pAdminReq->m_cbControlStyle)
			{
			case CS_BET_AREA:	//区域设置
				{
					m_cbControlStyle=pAdminReq->m_cbControlStyle;
					m_cbExcuteTimes=pAdminReq->m_cbExcuteTimes;
					m_cbWinAreaCount=0;
					BYTE cbIndex=0;
					for(cbIndex=0;cbIndex<AREA_COUNT;cbIndex++)
					{
						m_bWinArea[cbIndex]=pAdminReq->m_bWinArea[cbIndex];
						if(m_bWinArea[cbIndex])
							m_cbWinAreaCount++;
					}
					CMD_S_CommandResult cResult;
					cResult.cbResult=CR_ACCEPT;
					cResult.cbAckType=ACK_SET_WIN_AREA;
					m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
					break;
				}
			case CS_BANKER_LOSE:	//庄家输牌
				{
					m_cbControlStyle=pAdminReq->m_cbControlStyle;
					m_cbExcuteTimes=pAdminReq->m_cbExcuteTimes;
					CMD_S_CommandResult cResult;
					cResult.cbResult=CR_ACCEPT;
					cResult.cbAckType=ACK_SET_WIN_AREA;
					m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));

					break;
				}
			case CS_BANKER_WIN:		//庄家赢牌
				{
					m_cbControlStyle=pAdminReq->m_cbControlStyle;
					m_cbExcuteTimes=pAdminReq->m_cbExcuteTimes;
					CMD_S_CommandResult cResult;
					cResult.cbResult=CR_ACCEPT;
					cResult.cbAckType=ACK_SET_WIN_AREA;
					m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
					break;
				}
			default:	//拒绝请求
				{
					CMD_S_CommandResult cResult;
					cResult.cbResult=CR_REFUSAL;
					cResult.cbAckType=ACK_SET_WIN_AREA;
					m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
					break;
				}
			}

			break;
		}
	case RQ_PRINT_SYN:
		{
			CMD_S_CommandResult cResult;
			cResult.cbResult=CR_ACCEPT;
			cResult.cbAckType=ACK_PRINT_SYN;
			tagAdminReq*pAdminReq=reinterpret_cast<tagAdminReq*>(cResult.cbExtendData);
			pAdminReq->m_cbControlStyle=m_cbControlStyle;
			pAdminReq->m_cbExcuteTimes=m_cbExcuteTimes;
			memcpy(pAdminReq->m_bWinArea,m_bWinArea,sizeof(m_bWinArea));
			m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
			break;
		}
	default:
		{
			break;
		}


	}
	return true;
}
void CTableFrameSink::AddMoney()
{
	IServerUserItem * pIServerUserItemMax = NULL;
	IServerUserItem * pIServerUserItemMin = NULL;
	for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserChairID);
		if ( pIServerUserItem == NULL) continue;

		if (pIServerUserItem->IsAndroidUser())
		{
			if (pIServerUserItemMax == NULL)
			{
				pIServerUserItemMax = pIServerUserItem;
			}
			if (pIServerUserItemMin == NULL)
			{
				pIServerUserItemMin = pIServerUserItem;
			}

			if (pIServerUserItemMax->GetUserScore()->lScore<pIServerUserItem->GetUserScore()->lScore)
			{
				pIServerUserItemMax = pIServerUserItem;
			} 

			if (pIServerUserItemMin->GetUserScore()->lScore>pIServerUserItem->GetUserScore()->lScore)
			{
				pIServerUserItemMin = pIServerUserItem;
			} 			
		}
	}

	if ( pIServerUserItemMin == NULL || pIServerUserItemMax == NULL) return;
	if (pIServerUserItemMin->GetUserScore()->lScore<10000000&&pIServerUserItemMax->GetUserScore()->lScore>100000000)
	{
		m_pITableFrame->WriteUserScore(pIServerUserItemMin,10000000,0,enScoreKind_Win);
		m_pITableFrame->WriteUserScore(pIServerUserItemMax,-10000000,0,enScoreKind_Lost);
	} 

}

//游戏消息处理
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_PLACE_JETTON:		//用户加注
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_PlaceJetton));
			if (wDataSize!=sizeof(CMD_C_PlaceJetton)) return false;

			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//消息处理
			CMD_C_PlaceJetton * pPlaceJetton=(CMD_C_PlaceJetton *)pDataBuffer;
			return OnUserPlaceJetton(pUserData->wChairID,pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);
		}
	case SUB_C_APPLY_BANKER:		//申请做庄
		{
			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus==US_LOOKON) return true;

			return OnUserApplyBanker(pIServerUserItem);	
		}
	case SUB_C_CANCEL_BANKER:		//取消做庄
		{
			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus==US_LOOKON) return true;

			return OnUserCancelBanker(pIServerUserItem);	
		}
	case SUB_C_CONTINUE_CARD:		//继续发牌
		{
			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus==US_LOOKON) return true;
			if (pUserData->wChairID!=m_wCurrentBanker) return true;
			if (m_cbLeftCardCount < 8) return true;

			//设置变量
			m_bContiueCard=true;

			//发送消息
			SendGameMessage(pUserData->wChairID,TEXT("设置成功，下一局将继续发牌！"));

			return true;
		}
	case SUB_C_AMDIN_COMMAND:
		{
			ASSERT(wDataSize==sizeof(CMD_C_AdminReq));
			if(wDataSize!=sizeof(CMD_C_AdminReq)) return false;
			return OnSubAmdinCommand(pIServerUserItem,pDataBuffer);
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
	if ((bLookonUser==false)&&(m_dwJettonTime==0L))
	{
		m_dwJettonTime=(DWORD)time(NULL);
		m_pITableFrame->SetGameTimer(IDI_FREE,m_nFreeTime*1000,1,NULL);
		m_pITableFrame->SetGameStatus(GS_FREE);
	}

	//限制提示
	TCHAR szTipMsg[128];
	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄条件为：%I64d,区域限制为：%I64d,玩家限制为：%I64d"),m_lApplyBankerCondition,
		m_lAreaLimitScore,m_lUserLimitScore);
	m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);

	return true;
}

//用户起来
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//记录成绩
	if (bLookonUser==false)
	{
		//切换庄家
		if (wChairID==m_wCurrentBanker)
		{
			ChangeBanker(true);
			m_bContiueCard=false;
		}

		//取消申请
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			if (wChairID!=m_ApplyUserArray[i]) continue;

			//删除玩家
			m_ApplyUserArray.RemoveAt(i);

			//构造变量
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//设置变量
			lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));

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
	ASSERT((cbJettonArea<=ID_HUANG_MEN && cbJettonArea>=ID_TIAN_MEN)&&(lJettonScore>0L));
	if ((cbJettonArea>ID_HUANG_MEN)||(lJettonScore<=0L) || cbJettonArea<ID_TIAN_MEN)
	{
		return false;
	}
	////效验状态
	//ASSERT(m_pITableFrame->GetGameStatus()==GS_PLACE_JETTON);

	if (m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON)
	{
		CMD_S_PlaceJettonFail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
		PlaceJettonFail.lJettonArea=cbJettonArea;
		PlaceJettonFail.lPlaceScore=lJettonScore;
		PlaceJettonFail.wPlaceUser=wChairID;

		//发送消息
		m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
		return true;
	}
	//庄家判断
	if (m_wCurrentBanker==wChairID)
	{
		return true;
	}
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) 
	{
		return true;
	}
	//变量定义
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	LONGLONG lJettonCount=0L;
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lJettonCount += m_lUserJettonScore[nAreaIndex][wChairID];

	//玩家积分
	LONGLONG lUserScore = pIServerUserItem->GetUserScore()->lScore;

	//合法校验
	if (lUserScore < lJettonCount + lJettonScore) return true;
	if (m_lUserLimitScore < lJettonCount + lJettonScore) return true;

	//成功标识
	bool bPlaceJettonSuccess=true;

	//合法验证
	if (GetUserMaxJetton(wChairID) >= lJettonScore)
	{
		//机器人验证
		if(pIServerUserItem->IsAndroidUser())
		{
			//区域限制
			if (m_lRobotAreaScore[cbJettonArea] + lJettonScore > m_lRobotAreaLimit)
				return true;

			//数目限制
			bool bHaveChip = false;
			for (int i = 0; i < AREA_COUNT; i++)
			{
				if (m_lUserJettonScore[i+1][wChairID] != 0)
					bHaveChip = true;
			}

			if (!bHaveChip)
			{
				MyDebug(_T("下注机器人数目 [%d / %d]"), m_nChipRobotCount, m_nMaxChipRobot);
				if (m_nChipRobotCount+1 > m_nMaxChipRobot)
				{
					bPlaceJettonSuccess = false;
				}
				else
					m_nChipRobotCount++;
			}

			//统计分数
			if (bPlaceJettonSuccess)
				m_lRobotAreaScore[cbJettonArea] += lJettonScore;
		}

		if (bPlaceJettonSuccess)
		{
			//保存下注
			m_lAllJettonScore[cbJettonArea] += lJettonScore;
			m_lUserJettonScore[cbJettonArea][wChairID] += lJettonScore;			
		}
	}
	else
	{
		bPlaceJettonSuccess=false;
	}

	if (bPlaceJettonSuccess)
	{
		//变量定义
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

		//构造变量
		PlaceJetton.wChairID=wChairID;
		PlaceJetton.cbJettonArea=cbJettonArea;
		PlaceJetton.lJettonScore=lJettonScore;
		PlaceJetton.bIsAndroid=pIServerUserItem->IsAndroidUser();

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
	}
	else
	{
		CMD_S_PlaceJettonFail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
		PlaceJettonFail.lJettonArea=cbJettonArea;
		PlaceJettonFail.lPlaceScore=lJettonScore;
		PlaceJettonFail.wPlaceUser=wChairID;

		//发送消息
		m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
	}

	return true;
}

//发送扑克
bool CTableFrameSink::DispatchTableCard()
{
	//继续发牌
	int const static nDispatchCardCount=25;
	bool bContiueCard=m_bContiueCard;
	if (m_cbLeftCardCount<nDispatchCardCount) bContiueCard=false;

	//继续发牌
	if (true==bContiueCard)
	{
		//设置扑克
		CopyMemory(&m_cbTableCardArray[0][0],m_cbTableCard,sizeof(m_cbTableCardArray));
		m_cbLeftCardCount -= nDispatchCardCount;
		if (m_cbLeftCardCount>0) CopyMemory(m_cbTableCard, m_cbTableCard + nDispatchCardCount, sizeof(BYTE)*m_cbLeftCardCount);
	}
	else
	{
		//重新洗牌
		BYTE bcCard[CARD_COUNT];
		m_GameLogic.RandCardList(bcCard,54);
		m_bcFirstPostCard = bcCard[0];
		m_GameLogic.RandCardList(m_cbTableCard,CountArray(m_cbTableCard));	

		//设置扑克
		CopyMemory(&m_cbTableCardArray[0][0], m_cbTableCard, sizeof(m_cbTableCardArray));

		if(0)
		{

			BYTE temp1[5]={0x3D,0x08,0x38,0x26,0x05};
			BYTE temp2[5]={0x41,0x08,0x08,0x28,0x17};
			BYTE temp3[]={ 0x2D,0x0C,0x18,0x33,0x31};
			BYTE temp4[]={ 0x27,0x02,0x42,0x0D,0x07};
			BYTE temp5[]={ 0x16,0x12,0x1C,0x3B,0x3A};
			CopyMemory(&m_cbTableCardArray[0][0], temp1, sizeof(m_cbTableCardArray[0]));
			CopyMemory(&m_cbTableCardArray[1][0], temp2, sizeof(m_cbTableCardArray[1]));
			CopyMemory(&m_cbTableCardArray[2][0], temp3, sizeof(m_cbTableCardArray[2]));
			CopyMemory(&m_cbTableCardArray[3][0], temp4, sizeof(m_cbTableCardArray[3]));
			CopyMemory(&m_cbTableCardArray[4][0], temp5, sizeof(m_cbTableCardArray[3]));

		}

		//庄家判断
		if (INVALID_CHAIR != m_wCurrentBanker)
		{
			m_cbLeftCardCount=CountArray(m_cbTableCard)-nDispatchCardCount;
			CopyMemory(m_cbTableCard, m_cbTableCard + nDispatchCardCount, sizeof(BYTE)*m_cbLeftCardCount);
		}
		else
		{
			m_cbLeftCardCount=0;
		}
	}

	//设置数目
	for (int i=0; i<CountArray(m_cbCardCount); ++i) m_cbCardCount[i]=5;

	//发牌标志
	m_bContiueCard=false;

	return true;
}

//申请庄家
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	//合法判断
	LONGLONG lUserScore=pIApplyServerUserItem->GetUserScore()->lScore;
	if (lUserScore<m_lApplyBankerCondition)
	{
		m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("你的金币不足以申请庄家，申请失败！"),SMT_INFO|SMT_EJECT);
		return true;
	}

	//存在判断
	WORD wApplyUserChairID=pIApplyServerUserItem->GetChairID();
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];
		if (wChairID==wApplyUserChairID)
		{
			m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("你已经申请了庄家，不需要再次申请！"),SMT_INFO|SMT_EJECT);
			return true;
		}
	}

	//保存信息 
	m_ApplyUserArray.Add(wApplyUserChairID);

	//构造变量
	CMD_S_ApplyBanker ApplyBanker;
	ZeroMemory(&ApplyBanker,sizeof(ApplyBanker));

	//设置变量
	ApplyBanker.wApplyUser=wApplyUserChairID;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

	//切换判断
	if (m_pITableFrame->GetGameStatus()==GS_FREE && m_ApplyUserArray.GetCount()==1)
	{
		ChangeBanker(false);
	}

	return true;
}

//取消申请
bool CTableFrameSink::OnUserCancelBanker(IServerUserItem *pICancelServerUserItem)
{
	//当前庄家
	if (pICancelServerUserItem->GetChairID()==m_wCurrentBanker && m_pITableFrame->GetGameStatus()!=GS_FREE)
	{
		//发送消息
		m_pITableFrame->SendGameMessage(pICancelServerUserItem,TEXT("游戏已经开始，不可以取消当庄！"),SMT_INFO|SMT_EJECT);
		return true;
	}

	//存在判断
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		//获取玩家
		WORD wChairID=m_ApplyUserArray[i];
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);

		//条件过滤
		if (pIServerUserItem==NULL) continue;
		if (pIServerUserItem->GetUserID()!=pICancelServerUserItem->GetUserID()) continue;

		//删除玩家
		m_ApplyUserArray.RemoveAt(i);

		if (m_wCurrentBanker!=wChairID)
		{
			//构造变量
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//设置变量
			lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));

			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		}
		else if (m_wCurrentBanker==wChairID)
		{
			//切换庄家 
			m_wCurrentBanker=INVALID_CHAIR;
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
	bool bChangeBanker=false;

	//做庄次数
	WORD wBankerTime=m_nBankerTimeLimit;

	//取消当前
	if (bCancelCurrentBanker)
	{
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			//获取玩家
			WORD wChairID=m_ApplyUserArray[i];

			//条件过滤
			if (wChairID!=m_wCurrentBanker) continue;

			//删除玩家
			m_ApplyUserArray.RemoveAt(i);

			break;
		}

		//设置庄家
		m_wCurrentBanker=INVALID_CHAIR;

		//轮换判断
		TakeTurns();

		//设置变量
		bChangeBanker=true;
		m_bExchangeBanker = true;
		m_wAddTime=0;
	}
	//轮庄判断
	else if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//获取庄家
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);

		//次数判断
		if(pIServerUserItem!= NULL)
		{
			LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

			//次数判断
			if (wBankerTime+m_wAddTime<=m_wBankerTime || lBankerScore<m_lApplyBankerCondition)
			{
				//庄家增加判断 同一个庄家情况下只判断一次
				if(wBankerTime <= m_wBankerTime && m_bExchangeBanker && lBankerScore >= m_lApplyBankerCondition)
				{
					//加庄局数设置：当庄家坐满设定的局数之后(m_wBankerTime)，
					//所带金币值还超过下面申请庄家列表里面所有玩家金币时，
					//可以再加坐庄m_lBankerAdd局，加庄局数可设置。

					//金币超过m_lExtraBankerScore之后，
					//就算是下面玩家的金币值大于他的金币值，他也可以再加庄m_lBankerScoreAdd局。
					bool bScoreMAX = true;
					m_bExchangeBanker = false;

					for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
					{
						//获取玩家
						WORD wChairID = m_ApplyUserArray[i];
						IServerUserItem *pIUserItem = m_pITableFrame->GetServerUserItem(wChairID);
						LONGLONG lScore = pIServerUserItem->GetUserScore()->lScore;

						if ( wChairID != m_wCurrentBanker && lBankerScore <= lScore )
						{
							bScoreMAX = false;
							break;
						}
					}

					if ( bScoreMAX || (lBankerScore > m_lExtraBankerScore && m_lExtraBankerScore != 0l) )
					{
						BYTE bType = 0;
						if ( bScoreMAX && m_nBankerTimeAdd != 0 && m_nBankerTimeAdd != 0 )
						{
							bType = 1;
							m_wAddTime = m_nBankerTimeAdd;
						}
						if ( lBankerScore > m_lExtraBankerScore && m_lExtraBankerScore != 0l && m_nExtraBankerTime != 0 )
						{
							bType += 2;
							if (bType == 3)
							{
								bType = (m_nExtraBankerTime>m_nBankerTimeAdd?2:1);
								m_wAddTime = (m_nExtraBankerTime>m_nBankerTimeAdd?m_nExtraBankerTime:m_nBankerTimeAdd);
							}
							else
								m_wAddTime = m_nExtraBankerTime;
						}

						//提示消息
						TCHAR szTipMsg[128] = {};
						if (bType == 1 )
							_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]的分数超过其他申请上庄玩家，获得%d次额外坐庄次数!"),pIServerUserItem->GetAccounts(),m_wAddTime);
						else if (bType == 2)
							_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]的分数超过[%I64d]，获得%d次额外坐庄次数!"),pIServerUserItem->GetAccounts(),m_lExtraBankerScore,m_wAddTime);
						else
							bType = 0;

						if (bType != 0)
						{
							//发送消息
							SendGameMessage(INVALID_CHAIR,szTipMsg);
							return true;
						}
					}
				}

				//撤销玩家
				for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
				{
					//获取玩家
					WORD wChairID=m_ApplyUserArray[i];

					//条件过滤
					if (wChairID!=m_wCurrentBanker) continue;

					//删除玩家
					m_ApplyUserArray.RemoveAt(i);

					break;
				}

				//设置庄家
				m_wCurrentBanker=INVALID_CHAIR;

				//轮换判断
				TakeTurns();

				//提示消息
				TCHAR szTipMsg[128];
				if (lBankerScore<m_lApplyBankerCondition)
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]分数少于(%I64d)，强行换庄!"),pIServerUserItem->GetAccounts(),m_lApplyBankerCondition);
				else
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]做庄次数达到(%d)，强行换庄!"),pIServerUserItem->GetAccounts(),wBankerTime+m_wAddTime);

				bChangeBanker=true;
				m_bExchangeBanker = true;
				m_wAddTime = 0;

				//发送消息
				SendGameMessage(INVALID_CHAIR,szTipMsg);	
			}
		}
		else
		{
			for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
			{
				//获取玩家
				WORD wChairID=m_ApplyUserArray[i];

				//条件过滤
				if (wChairID!=m_wCurrentBanker) continue;

				//删除玩家
				m_ApplyUserArray.RemoveAt(i);

				break;
			}
			//设置庄家
			m_wCurrentBanker=INVALID_CHAIR;
		}

	}
	//系统做庄
	else if (m_wCurrentBanker==INVALID_CHAIR && m_ApplyUserArray.GetCount()!=0)
	{
		//轮换判断
		TakeTurns();

		bChangeBanker=true;
		m_bExchangeBanker = true;
		m_wAddTime = 0;
	}

	//切换判断
	if (bChangeBanker)
	{
		//设置变量
		m_wBankerTime = 0;
		m_lBankerWinScore=0;

		//发送消息
		CMD_S_ChangeBanker ChangeBanker;
		ZeroMemory(&ChangeBanker,sizeof(ChangeBanker));
		ChangeBanker.wBankerUser=m_wCurrentBanker;
		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
			ChangeBanker.lBankerScore=pIServerUserItem->GetUserScore()->lScore;
		}
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&ChangeBanker,sizeof(ChangeBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&ChangeBanker,sizeof(ChangeBanker));

		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			//获取参数
			m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
			ASSERT(m_pGameServiceOption!=NULL);

			//读取消息
			int lMessageCount=GetPrivateProfileInt(m_pGameServiceOption->szGameRoomName,TEXT("MessageCount"),0,m_szConfigFileName);
			if (lMessageCount!=0)
			{
				//读取配置
				LONGLONG lIndex=rand()%lMessageCount;
				TCHAR szKeyName[32],szMessage1[256],szMessage2[256];				
				_sntprintf(szKeyName,CountArray(szKeyName),TEXT("Item%I64d"),lIndex);
				GetPrivateProfileString(m_pGameServiceOption->szGameRoomName,szKeyName,TEXT("恭喜[ %s ]上庄"),szMessage1,CountArray(szMessage1),m_szConfigFileName);

				//获取玩家
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);

				//发送消息
				_sntprintf(szMessage2,CountArray(szMessage2),szMessage1,pIServerUserItem->GetAccounts());
				SendGameMessage(INVALID_CHAIR,szMessage2);
			}
		}
	}

	return bChangeBanker;
}

//轮换判断
void CTableFrameSink::TakeTurns()
{
	//变量定义
	int nInvalidApply = 0;

	for (int i = 0; i < m_ApplyUserArray.GetCount(); i++)
	{
		if (m_pITableFrame->GetGameStatus() == GS_FREE)
		{
			//获取分数
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_ApplyUserArray[i]);
			if (pIServerUserItem != NULL)
			{
				if (pIServerUserItem->GetUserScore()->lScore >= m_lApplyBankerCondition)
				{
					m_wCurrentBanker=m_ApplyUserArray[i];
					break;
				}
				else
				{
					nInvalidApply = i + 1;

					//发送消息
					CMD_S_CancelBanker CancelBanker = {};

					//设置变量
					lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));

					//发送消息
					m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
					m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

					//提示消息
					TCHAR szTipMsg[128] = {};
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("由于你的金币数（%I64d）少于坐庄必须金币数（%I64d）,你无法上庄！"),
						pIServerUserItem->GetUserScore()->lScore, m_lApplyBankerCondition);
					SendGameMessage(m_ApplyUserArray[i],szTipMsg);
				}
			}
		}
	}

	//删除玩家
	if (nInvalidApply != 0)
		m_ApplyUserArray.RemoveAt(0, nInvalidApply);
}

//发送庄家
void CTableFrameSink::SendApplyUser( IServerUserItem *pRcvServerUserItem )
{
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];

		//获取玩家
		IServerUserItem *pServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (!pServerUserItem) continue;

		//庄家判断
		if (pServerUserItem->GetChairID()==m_wCurrentBanker) continue;

		//构造变量
		CMD_S_ApplyBanker ApplyBanker;
		ApplyBanker.wApplyUser=wChairID;

		//发送消息
		m_pITableFrame->SendUserData(pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	}
}

//用户断线
bool __cdecl CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	//切换庄家
	if (wChairID==m_wCurrentBanker) ChangeBanker(true);

	//取消申请
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		if (wChairID!=m_ApplyUserArray[i]) continue;

		//删除玩家
		m_ApplyUserArray.RemoveAt(i);

		//构造变量
		CMD_S_CancelBanker CancelBanker;
		ZeroMemory(&CancelBanker,sizeof(CancelBanker));

		//设置变量
		lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));

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
	IServerUserItem *pIMeServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	if (NULL==pIMeServerUserItem) return 0L;

	int iTimer = 10;
	//已下注额
	LONGLONG lNowJetton = 0;
	ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lNowJetton += m_lUserJettonScore[nAreaIndex][wChairID];

	//庄家金币
	LONGLONG lBankerScore=2147483647;
	if (m_wCurrentBanker!=INVALID_CHAIR)
		lBankerScore=m_lBankerScore;
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lBankerScore-=m_lAllJettonScore[nAreaIndex]*iTimer;

	//个人限制
	LONGLONG lMeMaxScore = min((pIMeServerUserItem->GetUserScore()->lScore-lNowJetton*iTimer)/iTimer, m_lUserLimitScore);

	const tagUserScore *Info = pIMeServerUserItem->GetUserScore();

	const tagServerUserData *Data = pIMeServerUserItem->GetUserData();

	//区域限制
	lMeMaxScore=min(lMeMaxScore,m_lAreaLimitScore);

	//庄家限制
	lMeMaxScore=min(lMeMaxScore,lBankerScore/iTimer);

	//非零限制
	ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return (lMeMaxScore);

}
//计算得分
LONGLONG CTableFrameSink::CalculateScore()
{
	//	return 1;
	//变量定义
	LONGLONG static cbRevenue=m_pGameServiceOption->wRevenue;

	//推断玩家
	bool static bWinTianMen, bWinDiMen, bWinXuanMen,bWinHuang;
	BYTE TianMultiple,diMultiple,TianXuanltiple,HuangMultiple;
	TianMultiple  = 1;
	diMultiple = 1 ;
	TianXuanltiple = 1;
	HuangMultiple = 1;
	DeduceWinner(bWinTianMen, bWinDiMen, bWinXuanMen,bWinHuang,TianMultiple,diMultiple,TianXuanltiple,HuangMultiple );

	//游戏记录
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.bWinShunMen=bWinTianMen;
	GameRecord.bWinDuiMen=bWinDiMen;
	GameRecord.bWinDaoMen=bWinXuanMen;
	GameRecord.bWinHuang=bWinHuang;

	BYTE  cbMultiple[] = {0, 1, 1, 1, 1, 1, 1};

	cbMultiple[1] = TianMultiple;
	cbMultiple[2] = diMultiple;
	cbMultiple[3] = TianXuanltiple;
	cbMultiple[4] = HuangMultiple;

	//移动下标
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst ) m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;

	//庄家总量
	LONGLONG lBankerWinScore = 0;

	//玩家成绩
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	LONGLONG lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	//玩家下注
	LONGLONG *const pUserScore[] = {NULL,m_lUserJettonScore[ID_TIAN_MEN], m_lUserJettonScore[ID_DI_MEN], m_lUserJettonScore[ID_XUAN_MEN],
		m_lUserJettonScore[ID_HUANG_MEN]};	

	//区域倍率
	//BYTE static const cbMultiple[] = {0, 1, 1, 1, 1, 1, 1};

	//胜利标识
	bool static bWinFlag[AREA_COUNT+1];
	bWinFlag[0]=false;
	bWinFlag[ID_TIAN_MEN]=bWinTianMen;
	bWinFlag[ID_DI_MEN]=bWinDiMen;
	bWinFlag[ID_XUAN_MEN]=bWinXuanMen;
	bWinFlag[ID_HUANG_MEN]=bWinHuang;

	//FILE *pf = fopen("C://ServLong.txt","ab+");
	TCHAR szBuffer[256] = TEXT("");

	//计算积分
	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//庄家判断
		if (m_wCurrentBanker==wChairID) continue;

		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;

		for (WORD wAreaIndex = ID_TIAN_MEN; wAreaIndex <= ID_HUANG_MEN; ++wAreaIndex)
		{

			if (true==bWinFlag[wAreaIndex]) 
			{
				m_lUserWinScore[wChairID] += ( pUserScore[wAreaIndex][wChairID] * cbMultiple[wAreaIndex] ) ;
				m_lUserReturnScore[wChairID] += pUserScore[wAreaIndex][wChairID] ;
				lBankerWinScore -= ( pUserScore[wAreaIndex][wChairID] * cbMultiple[wAreaIndex] ) ;
			}
			else
			{
				lUserLostScore[wChairID] -= pUserScore[wAreaIndex][wChairID]*cbMultiple[wAreaIndex];
				lBankerWinScore += pUserScore[wAreaIndex][wChairID]*cbMultiple[wAreaIndex];
			}
		}

		//计算税收
		if (0 < m_lUserWinScore[wChairID])
		{
			double fRevenuePer=double(cbRevenue/1000.0);
			m_lUserRevenue[wChairID]  = LONGLONG(m_lUserWinScore[wChairID]*fRevenuePer);
			m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];
		}

		//总的分数
		memset(szBuffer,0,256);
		m_lUserWinScore[wChairID] += lUserLostScore[wChairID];
		/*	_snprintf(szBuffer,CountArray(szBuffer),TEXT(" 本局分数 %ld 返回分数 %d 用户名字%s 用户分数%d\r\n"),m_lUserWinScore[wChairID],m_lUserRevenue[wChairID],
		pIServerUserItem->GetUserData()->szAccounts,pIServerUserItem->GetUserData()->UserScoreInfo.lScore);
		fwrite(szBuffer,1,strlen(szBuffer)+1,pf);*/
	}

	//庄家成绩
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;

		//计算税收
		if (0 < m_lUserWinScore[m_wCurrentBanker])
		{
			double fRevenuePer=double(cbRevenue/1000.0);
			m_lUserRevenue[m_wCurrentBanker]  = LONGLONG(m_lUserWinScore[m_wCurrentBanker]*fRevenuePer);
			m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
			lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
		}	
	}

	//累计积分
	m_lBankerWinScore += lBankerWinScore;

	//当前积分
	m_lBankerCurGameScore=lBankerWinScore;

	return lBankerWinScore;
}


void CTableFrameSink::DeduceWinner(bool &bWinTian, bool &bWinDi, bool &bWinXuan,bool &bWinHuan,BYTE &TianMultiple,BYTE &diMultiple,BYTE &TianXuanltiple,BYTE &HuangMultiple)
{
	//大小比较
	bWinTian=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],5,m_cbTableCardArray[SHUN_MEN_INDEX],5,TianMultiple)==1?true:false;
	bWinDi=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],5,m_cbTableCardArray[DUI_MEN_INDEX],5,diMultiple)==1?true:false;
	bWinXuan=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],5,m_cbTableCardArray[DAO_MEN_INDEX],5,TianXuanltiple)==1?true:false;
	bWinHuan=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],5,m_cbTableCardArray[HUAN_MEN_INDEX],5,HuangMultiple)==1?true:false;
}

//发送记录
void CTableFrameSink::SendGameRecord(IServerUserItem *pIServerUserItem)
{
	WORD wBufferSize=0;
	BYTE cbBuffer[SOCKET_BUFFER];
	int nIndex = m_nRecordFirst;
	while ( nIndex != m_nRecordLast )
	{
		if ((wBufferSize+sizeof(tagServerGameRecord))>sizeof(cbBuffer))
		{
			m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
			wBufferSize=0;
		}
		CopyMemory(cbBuffer+wBufferSize,&m_GameRecordArrary[nIndex],sizeof(tagServerGameRecord));
		wBufferSize+=sizeof(tagServerGameRecord);

		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
	}
	if (wBufferSize>0) m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
}

//发送消息
void CTableFrameSink::SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg)
{
	if (wChairID==INVALID_CHAIR)
	{
		//游戏玩家
		for (WORD i=0; i<GAME_PLAYER; ++i)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
			if (pIServerUserItem==NULL) continue;
			m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_INFO);
		}

		//旁观玩家
		WORD wIndex=0;
		do {
			IServerUserItem *pILookonServerUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
			if (pILookonServerUserItem==NULL) break;

			m_pITableFrame->SendGameMessage(pILookonServerUserItem,pszTipMsg,SMT_INFO);

		}while(true);
	}
	else
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (pIServerUserItem!=NULL) m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_INFO|SMT_EJECT);
	}
}

//读取配置
void CTableFrameSink::ReadConfigInformation(bool bReadFresh)
{
	//变量定义
	TCHAR OutBuf[255] = {};

	//每盘刷新
	if (bReadFresh)
	{
		//每盘刷新
		BYTE cbRefreshCfg = GetPrivateProfileInt(m_szRoomName, TEXT("Refresh"), 0, m_szConfigFileName);
		m_bRefreshCfg = cbRefreshCfg?true:false;
	}

	//上庄条件
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_szRoomName, TEXT("score"), _T("100"), OutBuf, 255, m_szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), _T("%I64d"), &m_lApplyBankerCondition);

	//做庄次数
	m_nBankerTimeLimit = GetPrivateProfileInt(m_szRoomName, TEXT("Time"), 10, m_szConfigFileName);
	m_nBankerTimeAdd = GetPrivateProfileInt(m_szRoomName, TEXT("TimeAdd"), 10, m_szConfigFileName);

	//额外条件
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_szRoomName, TEXT("BankerScore"), _T("9000000000"), OutBuf, 255, m_szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), _T("%I64d"), &m_lExtraBankerScore);

	m_nExtraBankerTime = GetPrivateProfileInt(m_szRoomName, TEXT("BankerAdd"), 10, m_szConfigFileName);

	//下注限制
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_szRoomName, TEXT("AreaLimitScore"), _T("2147483647"), OutBuf, 255, m_szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), _T("%I64d"), &m_lAreaLimitScore);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_szRoomName, TEXT("UserLimitScore"), _T("2147483647"), OutBuf, 255, m_szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), _T("%I64d"), &m_lUserLimitScore);
	if(m_lUserLimitScore*10<0)		m_lUserLimitScore = 2147483647/10;

	//系统坐庄
	BYTE cbEnableSysBanker=GetPrivateProfileInt(m_szRoomName, TEXT("EnableSysBanker"), 1, m_szConfigFileName);
	m_bEnableSysBanker=cbEnableSysBanker?true:false;

	//库存设置
	m_lStorageStart = GetPrivateProfileInt(m_szRoomName, TEXT("StorageStart"), 0, m_szConfigFileName);
	m_nStorageDeduct = GetPrivateProfileInt(m_szRoomName, TEXT("StorageDeduct"), 0, m_szConfigFileName);

	//下注机器人数目
	m_nMaxChipRobot = GetPrivateProfileInt(m_szRoomName, TEXT("RobotBetCount"), 10, m_szConfigFileName);
	if (m_nMaxChipRobot < 0)	m_nMaxChipRobot = 10;

	//机器人下注限制
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_szRoomName, TEXT("RobotAreaLimit"), _T("50000000"), OutBuf, 255, m_szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), _T("%I64d"), &m_lRobotAreaLimit);

	//时间配置
	m_nFreeTime = GetPrivateProfileInt(m_szRoomName, TEXT("FreeTime"), 10, m_szConfigFileName);
	m_nPlaceJettonTime = GetPrivateProfileInt(m_szRoomName, TEXT("BetTime"), 15, m_szConfigFileName);
	m_nGameEndTime = GetPrivateProfileInt(m_szRoomName, TEXT("EndTime"), 28, m_szConfigFileName);
	if (m_nFreeTime <= 0		|| m_nFreeTime > 99)			m_nFreeTime = 10;
	if (m_nPlaceJettonTime <= 0 || m_nPlaceJettonTime > 99)		m_nPlaceJettonTime = 15;
	if (m_nGameEndTime <= 0		|| m_nGameEndTime > 99)			m_nGameEndTime = 28;

	MyDebug(_T("配置 A 坐庄 [%I64d %d %d] 额外 [%I64d %d] 系统庄 %d 刷新 %d"), m_lApplyBankerCondition, m_nBankerTimeLimit, m_nBankerTimeAdd, 
		m_lExtraBankerScore, m_nExtraBankerTime, m_bEnableSysBanker, m_bRefreshCfg);

	MyDebug(_T("配置 B 限制 [%I64d %I64d] 库存 [%I64d %d] 时间 [%d %d %d] 机人下注 %d"), m_lUserLimitScore, m_lAreaLimitScore,
		m_lStorageStart, m_nStorageDeduct, m_nFreeTime, m_nPlaceJettonTime, m_nGameEndTime, m_nMaxChipRobot);
}

//输赢处理	(暂时采用特殊处理，以后给其解耦，适用到任何百人财富游戏)
bool CTableFrameSink::GameCheat()
{
	//变量定义		
	bool bSpecUser[GAME_PLAYER] = {};			//特定玩家 (当bSytemBanker为true时表示普通玩家 false时表示权限玩家)
	bool bHaveSpecUser = false;					//是否有特定玩家
	int nWinType = 0;							//比较类型 0x0000 4位表示各家与庄家比牌结果 
	BYTE bMultiple[4];							//比较倍数
	LONGLONG lAllTypeLose = 0;					//输分统计 
	BYTE bCardData[5][5] = {};					//临时扑克数据
	BYTE bNeedPos = 0;							//需要方位 (当bSytemBanker为true时表示最大 false时表示最小)
	bool bSystemBanker = false;					//系统坐庄 (包括特殊玩家坐庄)
	int nArryChip[] = {0x0000, 0x1000, 0x0100, 0x0010, 0x0001};		//下注方位

	//系统坐庄	(包括权限玩家坐庄)
	if (m_wCurrentBanker == INVALID_CHAIR)
		bSystemBanker = true;
	else
	{
		IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		if (pServerUserItem != NULL)	bSystemBanker = pServerUserItem->IsAndroidUser();
	}

	//统计玩家
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(i);
		if (pServerUserItem == NULL)	continue;

		bSpecUser[i] = (pServerUserItem->IsAndroidUser() ^ bSystemBanker);
		if (!bHaveSpecUser)		
			bHaveSpecUser = bSpecUser[i];
	}

	//MyDebug(_T("作弊 A 系统坐庄 %s 指定玩家 %s"), (bSystemBanker?_T("是"):_T("否")), (bHaveSpecUser?_T("有"):_T("没有")));
	if (!bHaveSpecUser)	return false;

	//输赢类型
	memcpy(bCardData, m_cbTableCardArray, sizeof(bCardData));
	for (int i = 0; i < 4; i++)
		nWinType += (m_GameLogic.CompareCard(bCardData[i+1],5,bCardData[0],5,bMultiple[i])>0 ? 0 : (1<<(12-i*4)));
	ASSERT(nWinType<=0x1111);

	//MyDebug(_T("作弊 B 系统坐庄 %s 输赢类型 %x"), (bSystemBanker?_T("是"):_T("否")), nWinType);
	if (nWinType == (bSystemBanker?0x0000:0x1111))	return false;

	//输分统计
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		if (!bSpecUser[i])	continue;

		for (int k = 0; k < 4; k++)
		{
			if (m_lUserJettonScore[k+1][i] != 0)
			{
				if ((nWinType&nArryChip[k+1]) != 0)
					lAllTypeLose += m_lUserJettonScore[k+1][i] * (bSystemBanker?1:-1) * bMultiple[k];
				else
					lAllTypeLose -= m_lUserJettonScore[k+1][i] * (bSystemBanker?1:-1) * bMultiple[k];
			}
		}
	}

	//MyDebug(_T("作弊 C 输赢方式 %x 库存 %I64d 本次 %I64d"), nWinType, m_lStorageStart, lAllTypeLose);
	if (lAllTypeLose == 0)	return false;
	if (m_lStorageStart - lAllTypeLose > 0)	return false; 

	//需求位置
	for (int i = 1; i < 5; i++)
	{
		BYTE bTmpMul = 0;
		if (m_GameLogic.CompareCard(bCardData[bNeedPos], 5, bCardData[i], 5, bTmpMul) == (bSystemBanker?1:-1))
			bNeedPos = i;
	}
	//MyDebug(_T("作弊 D 指定玩家 %d [%x %x %x %x %x] 庄家 %d [%x %x %x %x %x]"), bNeedPos, bCardData[bNeedPos][0], bCardData[bNeedPos][1], 
	//	bCardData[bNeedPos][2], bCardData[bNeedPos][3], bCardData[bNeedPos][4], 0, m_cbTableCardArray[0][0], m_cbTableCardArray[0][1],
	//	bCardData[bNeedPos][2], bCardData[bNeedPos][3], bCardData[bNeedPos][4]);

	//交换手牌
	BYTE bTempCardData[5] = {};
	memcpy(bTempCardData,					m_cbTableCardArray[0],				sizeof(bTempCardData));
	memcpy(m_cbTableCardArray[0],			m_cbTableCardArray[bNeedPos],		sizeof(bTempCardData));
	memcpy(m_cbTableCardArray[bNeedPos],	bTempCardData,						sizeof(bTempCardData));

	return true;
}

//银行操作
#ifdef __BANKER___
bool __cdecl CTableFrameSink::OnActionUserBank(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return true;
}
#endif

//是否是有效组合
bool CTableFrameSink::GetSuitCardCombine(BYTE cbStack[])
{
	switch(m_cbControlStyle)
	{
	case CS_BET_AREA:
		{
			AreaWinCard(cbStack);
			break;
		}
	case CS_BANKER_WIN:
		{
			BankerWinCard(true,cbStack);
			break;
		}
	case CS_BANKER_LOSE:
		{
			BankerWinCard(false,cbStack);
			break;
		}

	}

	return true;
}

bool CTableFrameSink::AreaWinCard(BYTE cbStack[])
{
	bool bIsUser[MAX_CARDGROUP]={0};	//是否已经使用
	BYTE cbStackCount=0;	//栈中元素数量
	BYTE cbIndex=0;
	BYTE cbMultiple=0;
	//比较牌之间关系
	for(cbIndex=0;cbIndex<MAX_CARDGROUP;cbIndex++)
	{
		for(BYTE j=cbIndex;j<MAX_CARDGROUP;j++)
		{

			m_nCompareCard[j][cbIndex]=m_GameLogic.CompareCard(m_cbTableCardArray[cbIndex],MAX_CARD,m_cbTableCardArray[j],MAX_CARD,cbMultiple);
			m_nCompareCard[j][cbIndex]*=cbMultiple;
			m_nCompareCard[cbIndex][j]=-m_nCompareCard[j][cbIndex];
		}
	}

	//查找派牌组合
	for(cbIndex=0;cbIndex<=MAX_CARDGROUP;)
	{
		if(cbIndex<MAX_CARDGROUP)
		{
			if(bIsUser[cbIndex]) cbIndex++;
			else
			{
				cbStack[cbStackCount]=cbIndex;
				bIsUser[cbIndex]=true;
				cbStackCount++;
				if(cbStackCount==MAX_CARDGROUP)	//如果已经找到一种组合
				{

					if(m_bWinArea[0]==(m_nCompareCard[cbStack[1]][cbStack[0]]>0?true:false)		\
						&&m_bWinArea[1]==(m_nCompareCard[cbStack[2]][cbStack[0]]>0?true:false)	\
						&&m_bWinArea[2]==(m_nCompareCard[cbStack[3]][cbStack[0]]>0?true:false)	\
						&&m_bWinArea[3]==(m_nCompareCard[cbStack[4]][cbStack[0]]>0?true:false))
						break;
					cbIndex=MAX_CARDGROUP;
				}
				else
				{
					cbIndex=0;
					continue;
				}
			}
		}
		else
		{
			if(cbStackCount>0)
			{
				cbIndex=cbStack[--cbStackCount];
				bIsUser[cbIndex]=false;
				cbIndex++;
				continue;
			}
			else break;
		}
	}
	return true;
}

LONGLONG CTableFrameSink::GetBankerWinScore(int nWinMultiple[])
{
	//庄家总量
	LONGLONG lBankerWinScore = 0;

	for(BYTE cbIndex=0;cbIndex<AREA_COUNT;cbIndex++)
	{
		lBankerWinScore+=m_lAllJettonScore[cbIndex+1]*nWinMultiple[cbIndex];
	}
	return lBankerWinScore;
}

void CTableFrameSink::BankerWinCard(bool bIsWin,BYTE cbStack[])
{
	bool bIsUser[MAX_CARDGROUP]={0};	//是否已经使用
	BYTE cbStackCount=0;	//栈中元素数量
	BYTE cbIndex=0;
	BYTE cbMultiple;
	//比较牌之间关系
	for(cbIndex=0;cbIndex<MAX_CARDGROUP;cbIndex++)
	{
		for(BYTE j=cbIndex;j<MAX_CARDGROUP;j++)
		{
			m_nCompareCard[j][cbIndex]=m_GameLogic.CompareCard(m_cbTableCardArray[cbIndex],MAX_CARD,m_cbTableCardArray[j],MAX_CARD,cbMultiple);
			m_nCompareCard[j][cbIndex]*=cbMultiple;
			m_nCompareCard[cbIndex][j]=-m_nCompareCard[j][cbIndex];
		}
	}

	//查找派牌组合
	for(cbIndex=0;cbIndex<=MAX_CARDGROUP;)
	{
		if(cbIndex<MAX_CARDGROUP)
		{
			if(bIsUser[cbIndex]) cbIndex++;
			else
			{
				cbStack[cbStackCount]=cbIndex;
				bIsUser[cbIndex]=true;
				cbStackCount++;
				if(cbStackCount==MAX_CARDGROUP)	//如果已经找到一种组合
				{
					int nWinMultiple[4];
					for(int x=0;x<4;x++)
					{
						nWinMultiple[x]=m_nCompareCard[cbStack[x+1]][cbStack[0]];
					}

					if(bIsWin&&GetBankerWinScore(nWinMultiple)<0)
						break;
					if(!bIsWin&&GetBankerWinScore(nWinMultiple)>=0)
						break;
					cbIndex=MAX_CARDGROUP;
				}
				else
				{
					cbIndex=0;
					continue;
				}
			}
		}
		else
		{
			if(cbStackCount>0)
			{
				cbIndex=cbStack[--cbStackCount];
				bIsUser[cbIndex]=false;
				cbIndex++;
				continue;
			}
			else break;
		}
	}
}

void CTableFrameSink::GetSuitResult()
{
	//排序扑克
	BYTE cbSuitStack[MAX_CARDGROUP];
	//构建牌型
	//BuildCardGroup();
	//自动组合
	GetSuitCardCombine(cbSuitStack);

	//重新设置纸牌排列
	BYTE UserCard[MAX_CARDGROUP][MAX_CARD];
	memcpy(UserCard,m_cbTableCardArray,sizeof(UserCard));
	BYTE cbIndex=0;
	for(cbIndex=0;cbIndex<MAX_CARDGROUP;cbIndex++)
		memcpy(m_cbTableCardArray[cbIndex],UserCard[cbSuitStack[cbIndex]],sizeof(BYTE)*MAX_CARD);
}
//////////////////////////////////////////////////////////////////////////
