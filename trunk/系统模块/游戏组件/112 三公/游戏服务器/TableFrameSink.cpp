#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////
//宏定义

//时间定义
#define TIME_BANKER_START			30										//庄家开始
#define TIME_PLAYER_START			30										//玩家开始

//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//游戏人数
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_AllReady;	//开始模式

//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{	
	//状态变量
	m_cbSendCard=FALSE;
	m_cbWaitStart=FALSE;
	m_cbWaitChange=FALSE;
	m_cbWaitContinue=FALSE;
	m_wMaxChairID=INVALID_CHAIR;

	//游戏变量
	m_lExitScore = 0;
	m_bOffLineGameEnd = false;
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_lGameScore,sizeof(m_lGameScore));
	ZeroMemory(m_lUserChipIn,sizeof(m_lUserChipIn));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbUserCardData,sizeof(m_cbUserCardData));

	//状态变量
	ZeroMemory(m_cbUserFinish,sizeof(m_cbUserFinish));
	ZeroMemory(m_cbUserBanker,sizeof(m_cbUserBanker));
	ZeroMemory(m_cbUserContinue,sizeof(m_cbUserContinue));

	//辅助变量
	m_lMaxScore=0L;
	m_lLessScore=0L;
	m_dwBankerStartTime=0L;
	m_wCurrentUser=INVALID_CHAIR;

	//AI变量
	m_dStockScore=0;

	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

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

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	return true;
}

//复位桌子
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//辅助变量
	m_lMaxScore=0L;
	m_lLessScore=0L;

	//发牌状态
	m_lExitScore = 0;
	m_cbSendCard=FALSE;
	ZeroMemory(m_cbUserFinish,sizeof(m_cbUserFinish));

	//游戏变量
	ZeroMemory(m_lGameScore,sizeof(m_lGameScore));
	ZeroMemory(m_lUserChipIn,sizeof(m_lUserChipIn));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbUserCardData,sizeof(m_cbUserCardData));

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
	//设置状态
	m_pITableFrame->SetGameStatus(GS_TK_PLAYING);

	//用户设置
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_pITableFrame->GetServerUserItem(i)!=NULL)
		{
			m_cbPlayStatus[i]=TRUE;
			m_cbUserFinish[i]=FALSE;
		}
		else
		{
			m_cbUserFinish[i]=TRUE;
			m_cbPlayStatus[i]=FALSE;
		}
	}

	//分发扑克
	m_GameLogic.RandCardList(m_cbUserCardData[0],sizeof(m_cbUserCardData)/sizeof(m_cbUserCardData[0][0]));
	//控制最大玩家
	if (m_wMaxChairID != INVALID_CHAIR)
	{
		//扑克分析
		for (WORD wUserIndex=0;wUserIndex<m_wPlayerCount;wUserIndex++)
		{
			//获取用户
			IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wUserIndex);
			if (pIServerUserItem==NULL) continue;
			if (wUserIndex == m_wMaxChairID) continue;

			//对比扑克
			if (m_GameLogic.CompareCard(m_cbUserCardData[wUserIndex],m_cbUserCardData[m_wMaxChairID],MAX_COUNT)==true)
			{
				//交换数据
				BYTE cbTempData[MAX_COUNT];
				CopyMemory(cbTempData,m_cbUserCardData[wUserIndex],MAX_COUNT);
				CopyMemory(m_cbUserCardData[wUserIndex],m_cbUserCardData[m_wMaxChairID],MAX_COUNT);
				CopyMemory(m_cbUserCardData[m_wMaxChairID],cbTempData,MAX_COUNT);
			}
		}
	}

	//变量定义
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));

	//发送数据
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//构造数据
		GameStart.lLessScore=m_lLessScore;
		GameStart.lMaxScore=GetUserMaxChipIn(i);
		CopyMemory(GameStart.cbPlayStatus,m_cbPlayStatus,sizeof(GameStart.cbPlayStatus));

		//发送消息
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}
	m_wMaxChairID = INVALID_CHAIR;

	return true;
}

//游戏结束
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束
		{
			//变量定义
			BYTE cbCardTimes[GAME_PLAYER];
			ZeroMemory(cbCardTimes,sizeof(cbCardTimes));

			//状态设置
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (m_cbPlayStatus[i]==TRUE)
				{
					m_GameLogic.SortCardList(m_cbUserCardData[i],MAX_COUNT);
					cbCardTimes[i]=m_GameLogic.GetCardTimes(m_cbUserCardData[i],MAX_COUNT);
				}
			}

			//定义变量
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//对比扑克
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//用户过滤
				if (i==m_wBankerUser) continue;
				if (m_cbPlayStatus[i]==FALSE) continue;

				//对比扑克
				if (m_GameLogic.CompareCard(m_cbUserCardData[i],m_cbUserCardData[m_wBankerUser],MAX_COUNT)==true) 
				{
					m_lGameScore[i]+=cbCardTimes[i]*m_lUserChipIn[i];
					m_lGameScore[m_wBankerUser]-=cbCardTimes[i]*m_lUserChipIn[i];
				}
				else
				{
					m_lGameScore[i]-=cbCardTimes[m_wBankerUser]*m_lUserChipIn[i];
					m_lGameScore[m_wBankerUser]+=cbCardTimes[m_wBankerUser]*m_lUserChipIn[i];
				}
			}

			//强退分数
			m_lGameScore[m_wBankerUser]+=m_lExitScore;
			m_lExitScore = 0;

			//扣税变量
			LONG lRevenueSum=0L;
			BYTE cbRevenue=m_pGameServiceOption->wRevenue;

			//设置积分
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (m_lGameScore[i]>=100L)
				{
					lRevenueSum+=m_lGameScore[i]*cbRevenue/100L;
					GameEnd.lGameTax[i]=m_lGameScore[i]*cbRevenue/100L;
					GameEnd.lGameScore[i]=m_lGameScore[i]-m_lGameScore[i]*cbRevenue/100L;
				}
				else GameEnd.lGameScore[i]=m_lGameScore[i];
			}

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//修改积分
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (m_cbPlayStatus[i]==TRUE)
				{
					//构造积分
					enScoreKind ScoreKind=(GameEnd.lGameScore[i]>=0L)?enScoreKind_Win:enScoreKind_Lost;

					//写入积分
					m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],GameEnd.lGameTax[i],ScoreKind);
				}
			}

			//庄家点数
			BYTE cbCardType=m_GameLogic.GetCardType(m_cbUserCardData[m_wBankerUser],MAX_COUNT);
			BYTE cbCardValue=m_GameLogic.GetCardValue(m_cbUserCardData[m_wBankerUser],MAX_COUNT);

			//变量定义
			IServerUserItem * pUserInfo=m_pITableFrame->GetServerUserItem(m_wBankerUser);
			tagServerUserData * pServerUserData=pUserInfo->GetUserData();

			//换庄判断
			//if ((pUserInfo->IsAndroidUser()==false)&&((pServerUserData->dwUserRight&UR_LONG_BANKER)==0))
			{
				//抢庄判断
				if ((cbCardType!=CT_VALUES)||(cbCardValue!=9))
				{
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if ((i!=m_wBankerUser)&&(m_cbPlayStatus[i]==TRUE))
						{
							//获取属性
							BYTE cbCardType=m_GameLogic.GetCardType(m_cbUserCardData[i],MAX_COUNT);
							BYTE cbCardValue=m_GameLogic.GetCardValue(m_cbUserCardData[i],MAX_COUNT);

							//抢庄判断
							if ((IsSuitBankerUser(i)==true)&&(cbCardType==CT_VALUES)&&(cbCardValue==9))
							{
								m_cbWaitChange=TRUE;
								m_cbUserBanker[i]=TRUE;
							}
						}
					}
				}
			}

			//库存统计
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//获取用户
				IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
				if ((pIServerUserItem==NULL)||(pIServerUserItem->IsAndroidUser()==false)) continue;

				//库存累计
				m_dStockScore+=GameEnd.lGameScore[i];
			}

			//库存回收
			if(m_dStockScore>=100)m_dStockScore=m_dStockScore/100*95;

			//结束游戏
			m_pITableFrame->ConcludeGame();

			//庄家判断
			if ((m_wBankerUser!=INVALID_CHAIR)&&(IsSuitBankerUser(m_wBankerUser)==false))
			{
				//设置庄家
				m_wBankerUser=INVALID_CHAIR;

				//设置庄家
				SearceBankerUser();
			}

			//下注设置
			if (m_wBankerUser!=INVALID_CHAIR)
			{
				//获取庄家
				IServerUserItem * pIBankerUserItem=m_pITableFrame->GetServerUserItem(m_wBankerUser);

				//设置数据
				if (pIBankerUserItem!=NULL)
				{
					m_lLessScore=m_pGameServiceOption->lCellScore;
					m_lMaxScore=(LONG)(pIBankerUserItem->GetUserScore()->lScore)/37L;
				}

				//等待继续
				m_cbWaitContinue=TRUE;
				ZeroMemory(m_cbUserContinue,sizeof(m_cbUserContinue));

				//抢庄处理
				if (m_cbWaitChange==TRUE)
				{
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if (m_cbUserBanker[i]==TRUE)
						{
							m_wCurrentUser=i;
							m_pITableFrame->SendTableData(i,SUB_S_CHANGE_INFO);
							break;
						}
					}
				}
			}

			return true;
		}
	case GER_USER_LEFT:		//用户离开
		{
			//庄家离开
			if (wChairID==m_wBankerUser)
			{
				//定义变量
				LONG lWasteCount=0L;

				//下注状态
				if (m_cbSendCard==FALSE)
				{
					//设置积分
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						m_lGameScore[i]+=m_lUserChipIn[i];
						m_lGameScore[m_wBankerUser]-=m_lUserChipIn[i];
					}
				}

				//发牌状态
				if (m_cbSendCard==TRUE)
				{
					//变量定义
					BYTE cbCardTimes[GAME_PLAYER];
					ZeroMemory(cbCardTimes,sizeof(cbCardTimes));

					//牌型分析
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if ((m_cbPlayStatus[i]==TRUE)||(i==wChairID))
						{
							m_GameLogic.SortCardList(m_cbUserCardData[i],MAX_COUNT);
							cbCardTimes[i]=m_GameLogic.GetCardTimes(m_cbUserCardData[i],MAX_COUNT);
						}
					}

					//设置积分
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						//用户过滤
						if (i==m_wBankerUser) continue;
						if (m_cbPlayStatus[i]==FALSE) continue;

						//对比扑克
						if (m_GameLogic.CompareCard(m_cbUserCardData[i],m_cbUserCardData[m_wBankerUser],MAX_COUNT)==true)
						{
							m_lGameScore[i]+=cbCardTimes[i]*m_lUserChipIn[i];
							m_lGameScore[m_wBankerUser]-=cbCardTimes[i]*m_lUserChipIn[i];
						}
						else
						{
							lWasteCount+=cbCardTimes[m_wBankerUser]*m_lUserChipIn[i];
							m_lGameScore[i]-=cbCardTimes[m_wBankerUser]*m_lUserChipIn[i];
						}
					}
				}

				//强退分数
				m_lGameScore[m_wBankerUser]+=m_lExitScore;
				m_lExitScore =0;

				//变量定义
				CMD_S_GameEnd GameEnd;
				ZeroMemory(&GameEnd,sizeof(GameEnd));

				//扣税变量
				LONG lRevenueSum=0L;
				BYTE cbRevenue=m_pGameServiceOption->wRevenue;

				//设置积分
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if (m_lGameScore[i]>=100L)
					{
						lRevenueSum+=m_lGameScore[i]*cbRevenue/100L;
						GameEnd.lGameTax[i]=m_lGameScore[i]*cbRevenue/100L;
						GameEnd.lGameScore[i]=m_lGameScore[i]-m_lGameScore[i]*cbRevenue/100L;
					}
					else GameEnd.lGameScore[i]=m_lGameScore[i];
				}

				//发送信息
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

				//修改积分
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if (m_cbPlayStatus[i]==TRUE)
					{
						//构造积分
						LONG lRevenue=GameEnd.lGameTax[i];
						enScoreKind ScoreKind=(i==wChairID)?enScoreKind_Flee:enScoreKind_Win;

						//损耗计算
						if (i==wChairID) lRevenue+=lWasteCount;

						//写入积分
						m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],lRevenue,ScoreKind);
					}
				}

				//结束游戏
				m_pITableFrame->ConcludeGame();

				//等待继续
				m_cbWaitContinue=TRUE;
				ZeroMemory(m_cbUserContinue,sizeof(m_cbUserContinue));
			}

			//闲家离开
			if (wChairID!=m_wBankerUser)
			{
				//定义变量
				//LONG lWasteCount=0L;

				//设置状态
				m_cbPlayStatus[wChairID]=FALSE;

				//下注状态
				if (m_cbSendCard==FALSE)
				{
					//lWasteCount+=m_lUserChipIn[wChairID];
					m_lGameScore[wChairID]-=m_lUserChipIn[wChairID];
				}

				//发牌状态
				if (m_cbSendCard==TRUE)
				{
					//获取倍数
					m_GameLogic.SortCardList(m_cbUserCardData[m_wBankerUser],MAX_COUNT);
					BYTE cbCardTimes=m_GameLogic.GetCardTimes(m_cbUserCardData[m_wBankerUser],MAX_COUNT);

					//设置积分
					//lWasteCount+=m_lUserChipIn[wChairID]*cbCardTimes;
					m_lGameScore[wChairID]-=m_lUserChipIn[wChairID]*cbCardTimes;
				}

				//庄家所有
				m_lExitScore+=(LONG)(m_lGameScore[wChairID]*(-1));

				//写入积分
				m_pITableFrame->WriteUserScore(wChairID,m_lGameScore[wChairID],0,enScoreKind_Flee);

				//结束判断
				if(m_cbSendCard==TRUE)
				{
					//结束判断
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if ((m_cbPlayStatus[i]==TRUE)&&(m_cbUserFinish[i]==FALSE)) break;
					}

					//结束处理
					if (i==m_wPlayerCount) 
					{
						//设置变量
						m_cbSendCard=FALSE;

						//结束游戏
						OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);

						return true;
					}
				}

				//结束判断
				if ((GetTableUserCount()-1)==1)
				{
					//变量定义
					CMD_S_GameEnd GameEnd;
					ZeroMemory(&GameEnd,sizeof(GameEnd));

					//设置变量
					GameEnd.lGameScore[wChairID]=m_lGameScore[wChairID];

					//庄家得分
					if(m_lExitScore>0)
					{
						//计算得分
						BYTE cbRevenue=m_pGameServiceOption->wRevenue;
						if (m_lExitScore>=100L)
						{
							GameEnd.lGameTax[m_wBankerUser] = m_lExitScore*cbRevenue/100L;
							GameEnd.lGameScore[m_wBankerUser] = m_lExitScore-GameEnd.lGameTax[m_wBankerUser];
						}

						//写入积分
						m_pITableFrame->WriteUserScore(m_wBankerUser,GameEnd.lGameScore[m_wBankerUser],GameEnd.lGameTax[m_wBankerUser],enScoreKind_Win);
						m_lExitScore = 0;
					}

					//发送信息
					m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

					//结束游戏
					m_pITableFrame->ConcludeGame();

					//下注设置
					if (m_wBankerUser!=INVALID_CHAIR)
					{
						//获取庄家
						IServerUserItem * pIBankerUserItem=m_pITableFrame->GetServerUserItem(m_wBankerUser);

						//设置数据
						if (pIBankerUserItem!=NULL)
						{
							m_lLessScore=m_pGameServiceOption->lCellScore;
							m_lMaxScore=(LONG)(pIBankerUserItem->GetUserScore()->lScore)/37L;
						}
					}

					//等待继续
					m_cbWaitContinue=TRUE;
					ZeroMemory(m_cbUserContinue,sizeof(m_cbUserContinue));

					return true;
				}

				//发牌判断
				if (m_cbSendCard==FALSE)
				{
					//发牌判断
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						//庄家过滤
						if (i==m_wBankerUser) continue;

						//玩家过滤
						if ((m_cbPlayStatus[i]==TRUE)&&(m_lUserChipIn[i]==0L)) break;
					}

					//发送扑克
					if (i==m_wPlayerCount)
					{
						//扑克分析
						if(ChangeCardData()==false)AnalyseCard();

						//设置变量
						m_cbSendCard=TRUE;

						//构造变量
						CMD_S_SendCard SendCard;
						ZeroMemory(&SendCard,sizeof(SendCard));

						//设置扑克
						for (WORD j=0;j<m_wPlayerCount;j++)
						{
							if (m_cbPlayStatus[j]==TRUE)
							{
								CopyMemory(SendCard.cbUserCardData[j],m_cbUserCardData[j],sizeof(SendCard.cbUserCardData[i]));
							}
						}

						//发送消息
						m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
						m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
					}
				}
			}

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
	case GS_TK_FREE:	//空闲状态
		{
			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//游戏信息
			StatusFree.wBankerUser=m_wBankerUser;
			StatusFree.lLessScore=m_lLessScore;
			StatusFree.lMaxScore=GetUserMaxChipIn(wChiarID);

			//变量定义
			IServerUserItem * pUserInfo=m_pITableFrame->GetServerUserItem(m_wBankerUser);
			tagServerUserData * pServerUserData=pUserInfo->GetUserData();

			//换庄判断
			if ((pUserInfo->IsAndroidUser()==true))
			{
				StatusFree.bLongBanker=TRUE;
			}

			//开始时间
			if ((m_cbWaitStart==TRUE)&&(m_dwBankerStartTime!=0L))
			{
				DWORD dwPassTime=(DWORD)time(NULL)-m_dwBankerStartTime;
				StatusFree.wLeaveTime=(WORD)(__max(TIME_BANKER_START,dwPassTime)-dwPassTime);
			}

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_TK_PLAYING:	//游戏状态
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//庄家AI
			IServerUserItem *pUserInfo = m_pITableFrame->GetServerUserItem(m_wBankerUser);
			if(pUserInfo->IsAndroidUser())StatusPlay.bLongBanker=TRUE;

			//下注信息
			StatusPlay.lLessScore=m_lLessScore;
			StatusPlay.lMaxScore=GetUserMaxChipIn(wChiarID);
			CopyMemory(StatusPlay.lUserChipIn,m_lUserChipIn,sizeof(StatusPlay.lUserChipIn));

			//游戏信息
			StatusPlay.wBankerUser=m_wBankerUser;
			CopyMemory(StatusPlay.cbPlayStatus,m_cbPlayStatus,sizeof(StatusPlay.cbPlayStatus));

			//扑克信息
			if (m_cbSendCard==TRUE)
			{
				StatusPlay.cbSendCard=TRUE;
				StatusPlay.cbNeedFinish=(m_cbUserFinish[wChiarID]==FALSE)?TRUE:FALSE;
				CopyMemory(StatusPlay.cbUserCardData,m_cbUserCardData,sizeof(StatusPlay.cbUserCardData));
			}

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	//效验结果
	ASSERT(FALSE);

	return false;
}

//定时器事件
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	return false;
}

//游戏消息
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_CONTROL_MAX:
		{
			//如果不具有管理员权限 则返回错误
			if((pIServerUserItem->GetUserData()->dwUserRight&UR_GAME_CONTROL)==0)
			{
				return false;
			}
			m_wMaxChairID = pIServerUserItem->GetChairID();
			m_pITableFrame->SendGameMessage(pIServerUserItem,TEXT("控制成功"),SMT_INFO|SMT_EJECT);

			return true;
		}
	case SUB_C_CHANGE_INFO:		//抢庄消息
		{
			//游戏状态
			ASSERT(m_pITableFrame->GetGameStatus()==GS_TK_FREE);
			if (m_pITableFrame->GetGameStatus()!=GS_TK_FREE) return true;

			//用户状态
			ASSERT(pIServerUserItem->GetUserStatus()==US_SIT);
			if (pIServerUserItem->GetUserStatus()!=US_SIT) return true;

			//用户判断
			ASSERT(m_wCurrentUser==pIServerUserItem->GetChairID());
			if (m_wCurrentUser!=pIServerUserItem->GetChairID()) return true;

			//抢庄状态
			ASSERT((m_cbWaitChange==TRUE)&&(m_cbUserBanker[pIServerUserItem->GetChairID()]==TRUE));
			if ((m_cbWaitChange==FALSE)||(m_cbUserBanker[pIServerUserItem->GetChairID()]==FALSE)) return true;

			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_ChangeInfo));
			if (wDataSize!=sizeof(CMD_C_ChangeInfo)) return false;

			//消息处理
			WORD wChairID=pIServerUserItem->GetChairID();
			CMD_C_ChangeInfo * pChangeInfo=(CMD_C_ChangeInfo *)pDataBuffer;

			//设置变量
			m_cbUserBanker[wChairID]=FALSE;

			//设置庄家
			if (pChangeInfo->cbChange==FALSE)
			{
				//寻找用户
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					//获取位置
					WORD wNextChairID=(m_wCurrentUser+i+1)%GAME_PLAYER;

					//抢庄判断
					if (m_cbUserBanker[wNextChairID]==TRUE)
					{
						m_wCurrentUser=wNextChairID;
						m_pITableFrame->SendTableData(wNextChairID,SUB_S_CHANGE_INFO);
						break;
					}
				}

				//完成判断
				if (i!=m_wPlayerCount) return true;
			}
			else
			{
				//设置庄家
				SetBankerUser(wChairID);
			}

			//设置变量
			m_cbWaitChange=FALSE;
			m_wCurrentUser=INVALID_CHAIR;
			ZeroMemory(m_cbUserBanker,sizeof(m_cbUserBanker));

			//状态切换
			if ((m_cbWaitStart==FALSE)&&(m_cbWaitContinue==FALSE))
			{
				//设置变量
				m_cbWaitStart=TRUE;
				m_dwBankerStartTime=(DWORD)time(NULL);

				//构造变量
				CMD_S_TimerInfo TimerInfo;
				TimerInfo.wLeaveTime=TIME_BANKER_START;

				//发送消息
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TIMER_INFO,&TimerInfo,sizeof(TimerInfo));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TIMER_INFO,&TimerInfo,sizeof(TimerInfo));
			}

			return true;
		}
	case SUB_C_USER_CHIPIN:		//用户下注
		{
			//游戏状态
			ASSERT(m_pITableFrame->GetGameStatus()==GS_TK_PLAYING);
			if (m_pITableFrame->GetGameStatus()!=GS_TK_PLAYING) return true;

			//用户状态
			ASSERT(pIServerUserItem->GetUserStatus()==US_PLAY);
			if (pIServerUserItem->GetUserStatus()!=US_PLAY) return true;

			//下注状态
			ASSERT(m_lUserChipIn[pIServerUserItem->GetChairID()]==0L);
			if (m_lUserChipIn[pIServerUserItem->GetChairID()]!=0L) return true;

			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_UserChipin));
			if (wDataSize!=sizeof(CMD_C_UserChipin)) return false;

			//消息处理
			WORD wChairID=pIServerUserItem->GetChairID();
			CMD_C_UserChipin * pUserChipin=(CMD_C_UserChipin *)pDataBuffer;

			//变量定义
			LONG lUserChipin=pUserChipin->lScore;
			LONG lUserMaxScore=GetUserMaxChipIn(wChairID);

			//下注数目
			ASSERT((lUserChipin>=m_lLessScore)&&(lUserChipin<=lUserMaxScore));
			if ((lUserChipin<m_lLessScore)||(lUserChipin>lUserMaxScore)) return false;

			//设置变量
			m_lUserChipIn[wChairID]=lUserChipin;

			//变量定义
			CMD_S_UserChipin UserChipin;
			ZeroMemory(&UserChipin,sizeof(UserChipin));

			//构造变量
			UserChipin.wChairID=wChairID;
			UserChipin.lUserChipin=lUserChipin;

			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_CHIPIN,&UserChipin,sizeof(UserChipin));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_CHIPIN,&UserChipin,sizeof(UserChipin));

			//下注搜索
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//庄家过滤
				if (i==m_wBankerUser) continue;

				//玩家过滤
				if ((m_cbPlayStatus[i]==TRUE)&&(m_lUserChipIn[i]==0L)) break;
			}

			//发送扑克
			if (i==m_wPlayerCount)
			{
				//扑克分析
				if(ChangeCardData()==false)AnalyseCard();

				//设置变量
				m_cbSendCard=TRUE;
				ZeroMemory(m_cbUserFinish,sizeof(m_cbUserFinish));

				//构造变量
				CMD_S_SendCard SendCard;
				ZeroMemory(&SendCard,sizeof(SendCard));

				//设置扑克
				for (WORD j=0;j<m_wPlayerCount;j++)
				{
					if (m_cbPlayStatus[j]==TRUE)
					{
						CopyMemory(SendCard.cbUserCardData[j],m_cbUserCardData[j],sizeof(SendCard.cbUserCardData[i]));
					}
				}

				//发送消息
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
			}

			return true;
		}
	case SUB_C_SEND_FINISH:		//发牌完成
		{
			//游戏状态
			ASSERT(m_pITableFrame->GetGameStatus()==GS_TK_PLAYING);
			if (m_pITableFrame->GetGameStatus()!=GS_TK_PLAYING) return true;

			//用户状态
			ASSERT(pIServerUserItem->GetUserStatus()==US_PLAY);
			if (pIServerUserItem->GetUserStatus()!=US_PLAY) return true;

			//发牌状态
			ASSERT((m_cbSendCard==TRUE)&&(m_cbUserFinish[pIServerUserItem->GetChairID()]==FALSE));
			if ((m_cbSendCard==FALSE)||(m_cbUserFinish[pIServerUserItem->GetChairID()]==TRUE)) return true;

			//设置变量
			m_cbUserFinish[pIServerUserItem->GetChairID()]=TRUE;

			//结束判断
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if ((m_cbPlayStatus[i]==TRUE)&&(m_cbUserFinish[i]==FALSE)) break;
			}

			//结束处理
			if (i==m_wPlayerCount) 
			{
				//设置变量
				m_cbSendCard=FALSE;

				//结束游戏
				OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);
			}

			return true;
		}
	case SUB_C_CONTINUE_GAME:	//继续游戏
		{
			//游戏状态
			ASSERT(m_pITableFrame->GetGameStatus()==GS_TK_FREE);
			if (m_pITableFrame->GetGameStatus()!=GS_TK_FREE) return true;

			//用户状态
			ASSERT(pIServerUserItem->GetUserStatus()==US_SIT);
			if (pIServerUserItem->GetUserStatus()!=US_SIT) return true;

			//等待状态
			ASSERT((m_cbWaitContinue==TRUE)&&(m_cbUserContinue[pIServerUserItem->GetChairID()]==FALSE));
			if ((m_cbWaitContinue==FALSE)||(m_cbUserContinue[pIServerUserItem->GetChairID()]==TRUE)) return true;

			//设置变量
			m_cbUserContinue[pIServerUserItem->GetChairID()]=TRUE;

			//完成判断
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if ((m_cbUserContinue[i]==FALSE)&&(m_pITableFrame->GetServerUserItem(i)!=NULL))
				{
					return true;
				}
			}

			//设置变量
			m_cbWaitContinue=FALSE;

			//开始判断
			if ((m_cbWaitChange==FALSE)&&(GetTableUserCount()>=2))
			{
				//效验状态
				ASSERT(m_cbWaitStart==FALSE);

				//设置变量
				m_cbWaitStart=TRUE;
				m_dwBankerStartTime=(DWORD)time(NULL);

				//变量定义
				CMD_S_TimerInfo TimerInfo;
				CMD_S_ScoreInfo ScoreInfo;
				ZeroMemory(&TimerInfo,sizeof(TimerInfo));
				ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));

				//构造变量
				TimerInfo.wLeaveTime=TIME_BANKER_START;

				//下注信息
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					//构造变量
					ScoreInfo.lLessScore=m_lLessScore;
					ScoreInfo.lMaxScore=GetUserMaxChipIn(i);

					//发送消息
					m_pITableFrame->SendTableData(i,SUB_S_SCORE_INFO,&ScoreInfo,sizeof(ScoreInfo));
					m_pITableFrame->SendLookonData(i,SUB_S_SCORE_INFO,&ScoreInfo,sizeof(ScoreInfo));
				}

				//时间信息
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TIMER_INFO,&TimerInfo,sizeof(TimerInfo));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TIMER_INFO,&TimerInfo,sizeof(TimerInfo));
			}

			return true;
		}
	}

	return false;
}

//框架消息
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//开始消息
	if (wSubCmdID==SUB_GF_USER_READY)
	{
		//效验数据
		ASSERT(wDataSize==0);
		if (wDataSize!=0) throw TEXT("数据错误");

		//状态效验
		ASSERT(m_cbWaitStart==TRUE);
		if (m_cbWaitStart==FALSE) return false;

		//用户效验
		ASSERT(m_wBankerUser==pIServerUserItem->GetChairID());
		if (m_wBankerUser!=pIServerUserItem->GetChairID()) return false;

		//状态效验
		ASSERT(pIServerUserItem->GetUserStatus()==US_SIT);
		if (pIServerUserItem->GetUserStatus()!=US_SIT) return false;

		//设置变量
		m_cbWaitStart=FALSE;
		m_dwBankerStartTime=0L;

		//开始游戏
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if (i!=m_wBankerUser)
			{
				IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
				if (pIServerUserItem!=NULL) pIServerUserItem->GetUserData()->cbUserStatus=US_READY;
			}
		}

		return false;
	}

	return false;
}

////请求坐下
//bool __cdecl CTableFrameSink::OnEventUserReqSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon)
//{
//	//玩家处理
//	if (bReqLookon==false)
//	{
//		//庄家判断
//		if (m_wBankerUser==INVALID_CHAIR)
//		{
//			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
//			if (pUserData->UserScoreInfo.lScore<(m_pGameServiceOption->lRestrictScore)) return false;
//		}
//	}
//
//	return true;
//}

//用户断线
bool __cdecl CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if(m_pITableFrame->GetGameStatus()==GS_TK_PLAYING && (m_lUserChipIn[wChairID]>0L || m_wBankerUser==wChairID) && !m_bOffLineGameEnd)m_bOffLineGameEnd = true;

	return true;
}

//用户重入
bool __cdecl CTableFrameSink::OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	if(m_bOffLineGameEnd)
	{
		for(WORD i=0;i<GAME_PLAYER;i++)
		{
			IServerUserItem * pIUserItem=m_pITableFrame->GetServerUserItem(i);
			if(pIUserItem==NULL)continue;
			if(pIUserItem->GetUserData()->cbUserStatus==US_OFFLINE)break;
		}

		if(i==GAME_PLAYER)m_bOffLineGameEnd = false;
	}
	return true;
}

//用户坐下
bool __cdecl CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//玩家处理
	if (bLookonUser==false)
	{
		//设置庄家
		if (m_wBankerUser==INVALID_CHAIR)
		{
			//效验状态
			ASSERT(m_cbWaitStart==FALSE);
			ASSERT(m_cbWaitChange==FALSE);
			ASSERT(m_cbWaitContinue==FALSE);//?

			//设置庄家
			SetBankerUser(wChairID);

			return true;
		}

		//状态设置
		if (m_cbWaitContinue==TRUE) m_cbUserContinue[wChairID]=TRUE;

		//状态切换
		if ((m_cbWaitStart==FALSE)&&(m_cbWaitContinue==FALSE)&&(m_cbWaitChange==FALSE))
		{
			//设置变量
			m_cbWaitStart=TRUE;
			m_dwBankerStartTime=(DWORD)time(NULL);

			//构造变量
			CMD_S_TimerInfo TimerInfo;
			TimerInfo.wLeaveTime=TIME_BANKER_START;

			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TIMER_INFO,&TimerInfo,sizeof(TimerInfo));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TIMER_INFO,&TimerInfo,sizeof(TimerInfo));
		}
	}

	return true;
}

//用户起来
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//玩家处理
	if (bLookonUser==false)
	{
		//庄家离开
		if (wChairID==m_wBankerUser)
		{
			//设置变量
			m_lMaxScore=0L;
			m_lLessScore=0L;
			m_wBankerUser=INVALID_CHAIR;

			//设置庄家
			if (SearceBankerUser()==false) 
			{
				m_cbWaitContinue=FALSE;
				return true;
			}

			//等待变量
			if (m_cbWaitStart==TRUE)
			{
				m_cbWaitStart=FALSE;
				m_dwBankerStartTime=0L;
			}

			//换庄状态
			if ((m_cbWaitChange==TRUE)&&(m_wBankerUser!=INVALID_CHAIR))
			{
				//设置变量
				m_cbUserBanker[m_wBankerUser]=FALSE;

				//完成判断
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if (m_cbUserBanker[i]==TRUE) break;
				}

				//完成处理
				if (i==m_wPlayerCount)
				{
					//设置变量
					m_cbWaitChange=FALSE;
					m_wCurrentUser=INVALID_CHAIR;
					ZeroMemory(m_cbUserBanker,sizeof(m_cbUserBanker));
				}
			}
		}

		//换庄状态
		if (m_cbWaitChange==TRUE)
		{
			//设置变量
			m_cbUserBanker[wChairID]=FALSE;

			//当前用户
			if (wChairID==m_wCurrentUser)
			{
				//寻找用户
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					//获取位置
					WORD wNextChairID=(m_wCurrentUser+i+1)%GAME_PLAYER;

					//抢庄判断
					if (m_cbUserBanker[wNextChairID]==TRUE)
					{
						m_wCurrentUser=wNextChairID;
						m_pITableFrame->SendTableData(m_wCurrentUser,SUB_S_CHANGE_INFO);
						break;
					}
				}

				//完成判断
				if (i!=m_wPlayerCount) return true;
			}

			//设置变量
			m_cbWaitChange=FALSE;
			m_wCurrentUser=INVALID_CHAIR;
			ZeroMemory(m_cbUserBanker,sizeof(m_cbUserBanker));
		}

		//等待继续
		if (m_cbWaitContinue==TRUE)
		{
			//用户搜索
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if ((m_cbUserContinue[i]==FALSE)&&(m_pITableFrame->GetServerUserItem(i))!=NULL)
				{
					return true;
				}
			}

			//完成判断
			m_cbWaitContinue=FALSE;
			ZeroMemory(m_cbUserContinue,sizeof(m_cbUserContinue));
		}

		//停止时间
		WORD wAddCount = ((m_pITableFrame->GetServerUserItem(wChairID)!=NULL)?0:1);
		if ((m_cbWaitStart==TRUE)&&((GetTableUserCount()+wAddCount)<=1))
		{
			//设置变量
			m_cbWaitStart=FALSE;
			m_dwBankerStartTime=0L;

			//构造变量
			CMD_S_TimerInfo TimerInfo;
			ZeroMemory(&TimerInfo,sizeof(TimerInfo));

			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TIMER_INFO,&TimerInfo,sizeof(TimerInfo));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TIMER_INFO,&TimerInfo,sizeof(TimerInfo));

			return true;
		}

		//获取属性
		WORD wUserCount=GetTableUserCount()+wAddCount;
		BYTE cbGameStatus=m_pITableFrame->GetGameStatus();

		//开始时间
		if ((m_cbWaitStart==FALSE)&&(m_cbWaitContinue==FALSE)&&(m_cbWaitChange==FALSE)&&(cbGameStatus==GS_FREE)&&(wUserCount>=2))
		{
			//游戏结束
			if(m_bOffLineGameEnd)
			{
				m_bOffLineGameEnd = false;
				return true;
			}

			//设置变量
			m_cbWaitStart=TRUE;
			m_dwBankerStartTime=(DWORD)time(NULL);

			//构造变量
			CMD_S_TimerInfo TimerInfo;
			ZeroMemory(&TimerInfo,sizeof(TimerInfo));

			//构造变量
			TimerInfo.wLeaveTime=TIME_BANKER_START;

			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TIMER_INFO,&TimerInfo,sizeof(TimerInfo));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TIMER_INFO,&TimerInfo,sizeof(TimerInfo));

			return true;
		}
	}

	return true;
}

//设置庄家
void CTableFrameSink::SetBankerUser(WORD wBankerUser)
{
	//相同判断
	ASSERT(m_wBankerUser!=wBankerUser);
	if (m_wBankerUser==wBankerUser) return;

	//游戏变量
	m_lMaxScore=0L;
	m_lLessScore=0L;
	m_wBankerUser=wBankerUser;

	//设置判断
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//用户数目
		IServerUserItem * pIBankerUserItem=m_pITableFrame->GetServerUserItem(m_wBankerUser);
		tagServerUserData * pServerUserData=pIBankerUserItem->GetUserData();

		//设置数据
		m_lLessScore=m_pGameServiceOption->lCellScore;
		m_lMaxScore=(LONG)(pIBankerUserItem->GetUserScore()->lScore)/37L;

		//构造变量
		CMD_S_BankerUser BankerUser;
		ZeroMemory(&BankerUser,sizeof(BankerUser));

		//长庄判断
		if ((pIBankerUserItem->IsAndroidUser()==true))
		{
			BankerUser.bLongBanker=TRUE;
		}

		//发送消息
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//构造变量
			BankerUser.lLessScore=m_lLessScore;
			BankerUser.wBankerUser=m_wBankerUser;
			BankerUser.lMaxScore=GetUserMaxChipIn(i);

			//发送消息
			m_pITableFrame->SendTableData(i,SUB_S_BANKER_USER,&BankerUser,sizeof(BankerUser));
			m_pITableFrame->SendLookonData(i,SUB_S_BANKER_USER,&BankerUser,sizeof(BankerUser));
		}
	}

	return;
}

//庄家检查
bool CTableFrameSink::IsSuitBankerUser(WORD wChairID)
{
	//获取用户
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	if (pIServerUserItem==NULL) return false;

	//积分检测
	tagServerUserData * pServerUserData=pIServerUserItem->GetUserData();
	if (pServerUserData->UserScoreInfo.lScore<(m_pGameServiceOption->lRestrictScore)) return false;

	return true;
}

//寻找庄家
bool CTableFrameSink::SearceBankerUser()
{
	//寻找庄家
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (IsSuitBankerUser(i)==true)
		{
			SetBankerUser(i);
			return true;
		}
	}

	//弹出用户
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem==NULL) continue;

		//发送消息
		if (pIServerUserItem->IsAndroidUser()==false)
		{
			m_pITableFrame->SendGameMessage(pIServerUserItem,TEXT("由于没有玩家有足够的信用额担任庄家，游戏解散！"),SMT_CLOSE_GAME|SMT_EJECT);
		}
		else
		{
			ITableFrameManager * pITableFrameManager=QUERY_OBJECT_PTR_INTERFACE(m_pITableFrame,ITableFrameManager);
			pITableFrameManager->PerformStandUpAction(pIServerUserItem);
		}
	}

	return false;
}

//扑克分析
void CTableFrameSink::AnalyseCard()
{
	//扑克分析
	for (WORD wUserIndex=0;wUserIndex<m_wPlayerCount;wUserIndex++)
	{
		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wUserIndex);
		if ((pIServerUserItem==NULL)||(pIServerUserItem->IsAndroidUser()==false)) continue;

		//变量定义
		LONG lAndroidScore=0;

		//倍数变量
		BYTE cbCardTimes[GAME_PLAYER];
		ZeroMemory(cbCardTimes,sizeof(cbCardTimes));

		//扑克变量
		BYTE cbUserCardData[GAME_PLAYER][3];
		CopyMemory(cbUserCardData,m_cbUserCardData,sizeof(m_cbUserCardData));

		//排列扑克
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			m_GameLogic.SortCardList(cbUserCardData[i],MAX_COUNT);
		}

		//查找倍数
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if (m_cbPlayStatus[i]==TRUE)
			{
				cbCardTimes[i]=m_GameLogic.GetCardTimes(cbUserCardData[i],MAX_COUNT);
			}
		}

		//对比扑克
		if (wUserIndex==m_wBankerUser)
		{
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//用户过滤
				if ((i==m_wBankerUser)||(m_cbPlayStatus[i]==FALSE)) continue;

				//对比扑克
				if (m_GameLogic.CompareCard(cbUserCardData[i],cbUserCardData[m_wBankerUser],MAX_COUNT)==true)
				{
					lAndroidScore-=cbCardTimes[i]*m_lUserChipIn[i];
				}
				else
				{
					lAndroidScore+=cbCardTimes[m_wBankerUser]*m_lUserChipIn[i];
				}
			}
		}
		else
		{
			//对比扑克
			if (m_GameLogic.CompareCard(cbUserCardData[m_wBankerUser],cbUserCardData[wUserIndex],MAX_COUNT)==true)
			{
				lAndroidScore-=cbCardTimes[m_wBankerUser]*m_lUserChipIn[wUserIndex];
			}
			else
			{
				lAndroidScore+=cbCardTimes[wUserIndex]*m_lUserChipIn[wUserIndex];
			}
		}

		//库存判断
		if ((m_dStockScore+lAndroidScore)<0.0)
		{
			//变量定义
			WORD wWinUser=INVALID_CHAIR;

			//最大用户
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//用户过滤
				if (m_cbPlayStatus[i]==FALSE) continue;

				//设置用户
				if (wWinUser==INVALID_CHAIR)
				{
					wWinUser=i;
					continue;
				}

				//对比扑克
				if (m_GameLogic.CompareCard(cbUserCardData[i],m_cbUserCardData[wWinUser],MAX_COUNT)==true)
				{
					wWinUser=i;
				}
			}

			//交换数据
			BYTE cbTempData[MAX_COUNT];
			CopyMemory(cbTempData,m_cbUserCardData[wUserIndex],MAX_COUNT);
			CopyMemory(m_cbUserCardData[wUserIndex],m_cbUserCardData[wWinUser],MAX_COUNT);
			CopyMemory(m_cbUserCardData[wWinUser],cbTempData,MAX_COUNT);
		}

		break;
	}

	return;
}

//用户数目
WORD CTableFrameSink::GetTableUserCount()
{
	//变量定义
	WORD wUserCount=0;

	//用户统计
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_pITableFrame->GetServerUserItem(i)!=NULL) 
		{
			wUserCount++;
		}
	}

	return wUserCount;
}

//最大数目
LONG CTableFrameSink::GetUserMaxChipIn(WORD wChairID)
{
	//获取用户
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);

	//用户判断
	if (pIServerUserItem==NULL) return 0L;

	return __min(m_lMaxScore,(LONG)(pIServerUserItem->GetUserScore()->lScore)/9);
}

//交换数据
bool CTableFrameSink::ChangeCardData()
{
	////用户人数
	//BYTE bUserCount =0;
	//for(WORD i=0;i<GAME_PLAYER;i++) if(m_cbPlayStatus[i]==TRUE) bUserCount++;

	////查找用户
	//for(WORD i=0;i<GAME_PLAYER;i++)
	//{
	//	//存在限制
	//	INT nType = m_pITableFrame->GetUserType(i);
	//	if(nType==enUserType_MustLostUser || nType==enUserType_MustWinUser)break;
	//}
	//if(i==GAME_PLAYER)return false;

	////用户信息
	//INT nUserWinTag[GAME_PLAYER];
	//ZeroMemory(nUserWinTag,sizeof(nUserWinTag));

	////确定输赢
	//WORD wWinUser = INVALID_CHAIR,wLoseUser = INVALID_CHAIR;
	//WORD wWinCount = 0,wLoseCount = 0;
	//for(WORD i=0;i<GAME_PLAYER;i++)
	//{
	//	if(m_cbPlayStatus[i]!=TRUE)continue;
	//	nUserWinTag[i] = m_pITableFrame->GetUserType(i);
	//	if(nUserWinTag[i] == enUserType_MustWinUser)
	//	{
	//		if(wWinUser == INVALID_CHAIR)wWinUser = i;
	//		wWinCount++;
	//	}
	//	else if(nUserWinTag[i] == enUserType_MustLostUser)
	//	{
	//		if(wLoseUser == INVALID_CHAIR)wLoseUser = i;
	//		wLoseCount++;
	//	}
	//}
	//if(wWinCount==bUserCount || wLoseCount==bUserCount)return true;

	////交换数据
	//bool bReturnValue = false;
	//if(wWinUser < GAME_PLAYER)
	//{
	//	for(WORD i=0;i<GAME_PLAYER;i++)
	//	{
	//		if(m_cbPlayStatus[i]!=TRUE || i==wWinUser)continue;
	//		if(m_GameLogic.CompareCard(m_cbUserCardData[i],m_cbUserCardData[wWinUser],MAX_COUNT)==TRUE)
	//		{
	//			BYTE bTempData[MAX_COUNT];
	//			CopyMemory(bTempData,m_cbUserCardData[i],sizeof(bTempData));
	//			CopyMemory(m_cbUserCardData[i],m_cbUserCardData[wWinUser],sizeof(bTempData));
	//			CopyMemory(m_cbUserCardData[wWinUser],bTempData,sizeof(bTempData));
	//		}
	//	}
	//	bReturnValue = true;
	//}

	////交换数据
	//if(wLoseUser < GAME_PLAYER)
	//{
	//	for(WORD i=0;i<GAME_PLAYER;i++)
	//	{
	//		if(m_cbPlayStatus[i]!=TRUE || i==wLoseUser)continue;
	//		if(m_GameLogic.CompareCard(m_cbUserCardData[i],m_cbUserCardData[wLoseUser],MAX_COUNT)==FALSE)
	//		{
	//			BYTE bTempData[MAX_COUNT];
	//			CopyMemory(bTempData,m_cbUserCardData[i],sizeof(bTempData));
	//			CopyMemory(m_cbUserCardData[i],m_cbUserCardData[wLoseUser],sizeof(bTempData));
	//			CopyMemory(m_cbUserCardData[wLoseUser],bTempData,sizeof(bTempData));
	//		}
	//	}
	//	bReturnValue = true;
	//}

	return false;
}

//////////////////////////////////////////////////////////////////////////
