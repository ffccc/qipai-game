#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//游戏人数
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//开始模式

//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{
	//游戏变量
	m_wFirstUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_lGameScore,sizeof(m_lGameScore));
	
	//胜利信息
	m_wWinCount=0;
	memset(m_wWindUserList,0,sizeof(m_wWindUserList));

	//运行信息
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//扑克信息
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));

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
	//游戏变量
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_lGameScore,sizeof(m_lGameScore));

	//胜利信息
	m_wWinCount=0;
	memset(m_wWindUserList,0,sizeof(m_wWindUserList));

	//运行信息
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//扑克信息
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));

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
	m_pITableFrame->SetGameStatus(GS_RF_PLAYING);

	//混乱扑克
	BYTE bRandCard[108];
	m_GameLogic.RandCardList(bRandCard,CountArray(bRandCard));

	//分发扑克
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_bCardCount[i]=27;
		CopyMemory(&m_bHandCardData[i],&bRandCard[i*m_bCardCount[i]],sizeof(BYTE)*m_bCardCount[i]);
		m_GameLogic.SortCardList(m_bHandCardData[i],m_bCardCount[i],ST_ORDER);
	}

	//设置用户
	if (m_wFirstUser==INVALID_CHAIR)
	{
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			for (BYTE j=0;j<m_bCardCount[i];j++)
			{
				if (m_bHandCardData[i][j]==0x03) 
				{
					m_wCurrentUser=i;
					break;
				}
			}
			if (m_wCurrentUser!=INVALID_CHAIR) break;
		}
	}
	else m_wCurrentUser=m_wFirstUser;

	//发送扑克
	CMD_S_GameStart GameStart;
	GameStart.wCurrentUser=m_wCurrentUser;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//构造扑克
		CopyMemory(GameStart.cbCardData[0],m_bHandCardData[i],sizeof(GameStart.cbCardData[0]));
		CopyMemory(GameStart.cbCardData[1],m_bHandCardData[(i+2)%GAME_PLAYER],sizeof(GameStart.cbCardData[1]));

		//发送数据
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	return true;
}

//游戏结束
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_DISMISS:		//游戏解散
		{
			//效验参数
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount);

			//构造数据
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//结束游戏
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_NORMAL:		//常规结束
		{
			//定义变量
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			//剩余扑克
			BYTE bCardPos=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}

			//胜利统计
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (m_bCardCount[i]!=0)
				{
					ASSERT(m_wWinCount<GAME_PLAYER);
					m_wWindUserList[m_wWinCount++]=i;
				}
			}

			//手上得分
			LONG lHandScore=0L;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				for (BYTE j=0;j<m_bCardCount[i];j++)
				{
					if ((m_bHandCardData[i][j]&LOGIC_MASK_VALUE)==5) lHandScore+=5;
					if ((m_bHandCardData[i][j]&LOGIC_MASK_VALUE)==10) lHandScore+=10;
					if ((m_bHandCardData[i][j]&LOGIC_MASK_VALUE)==13) lHandScore+=10;
				}
			}

			//累计得分
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if ((i%2)!=(m_wWindUserList[3]%2))
				{
					m_lGameScore[i%2]+=lHandScore;
					break;
				}
			}

			//翻倍计算   翻倍时用户积分 +-100
			if (((m_wWindUserList[0]%2)==(m_wWindUserList[1]%2))&&(m_lGameScore[m_wWindUserList[0]%2]==200L))
			{
				m_lGameScore[m_wWindUserList[0]%2]=2L*m_lGameScore[m_wWindUserList[0]%2]+200L;				
            }

			//胜利得分
			for (int i=1;i<m_wPlayerCount;i++)
			{
				if ((m_wWindUserList[i]%2)==(m_wWindUserList[0]%2))//判断赢的家     如果是赢伙的玩家进行循环
				{
					WORD wWinner=m_wWindUserList[i];
					m_lGameScore[wWinner%2]+=(40-(i-1)*20);
					m_lGameScore[(wWinner+1)%2]-=(40-(i-1)*20);
					break;
				}
			}
            
			//统计积分
			LONG lCellScore=m_pGameServiceOption->lCellScore;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.lGameScore[i]=lCellScore*((m_lGameScore[i%2]-m_lGameScore[(i+1)%2])/2L);
			}
			for (WORD i=0;i<2;i++)
			{
				GameEnd.lDrawScore[i]=m_lGameScore[i];
			}
            
			//计算税收
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (GameEnd.lGameScore[i]<=0) continue;
				if (GameEnd.lGameScore[i]<=20)
				{
					GameEnd.lGameTax=1L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax;
				}
				else if (GameEnd.lGameScore[i]<=40)
				{
					GameEnd.lGameTax=2L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax;
				}
				else if (GameEnd.lGameScore[i]<=60)
				{
					GameEnd.lGameTax=3L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax;
				}
				else if (GameEnd.lGameScore[i]<=80)
				{
					GameEnd.lGameTax=4L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax;
				}
				else if (GameEnd.lGameScore[i]<=100)
				{
					GameEnd.lGameTax=5L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax;
				}
				else if (GameEnd.lGameScore[i]<=120)
				{
					GameEnd.lGameTax=6L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax;
				}
				else if (GameEnd.lGameScore[i]<=140)
				{
					GameEnd.lGameTax=7L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax;
				}
				else if (GameEnd.lGameScore[i]<=160)
				{
					GameEnd.lGameTax=8L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax;
				}
                else if (GameEnd.lGameScore[i]<=180)
				{
					GameEnd.lGameTax=9L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax;
				}
                else if (GameEnd.lGameScore[i]<=200)
				{
					GameEnd.lGameTax=10L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax;
				}
                else if (GameEnd.lGameScore[i]<=220)
				{
					GameEnd.lGameTax=11L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax;
				}
                else if (GameEnd.lGameScore[i]<=240)
				{
					GameEnd.lGameTax=12L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax;
				}
                else if (GameEnd.lGameScore[i]<=340)
				{
					GameEnd.lGameTax=17L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax;
				}
            }

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//修改积分
			tagScoreInfo ScoreInfo[m_wPlayerCount];
			memset(&ScoreInfo,0,sizeof(ScoreInfo));
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				ScoreInfo[i].lScore=GameEnd.lGameScore[i];
				if (GameEnd.lGameScore[i]==0) ScoreInfo[i].ScoreKind=enScoreKind_Draw;
				else ScoreInfo[i].ScoreKind=(GameEnd.lGameScore[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
			}
///			m_pITableFrame->WriteTableScore(ScoreInfo,m_wPlayerCount,GameEnd.lGameTax);

			//设置变量
			m_wFirstUser=m_wWindUserList[0];

			//结束游戏
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_USER_LEFT:		//用户强退
		{
			//定义变量
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			//剩余扑克
			BYTE bCardPos=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}

			//统计积分
			GameEnd.lGameScore[wChairID]=-600L*m_pGameServiceOption->lCellScore;

			//计算税收
			if (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
			{
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if (GameEnd.lGameScore[i]>=100L)
					{
						GameEnd.lGameTax+=GameEnd.lGameScore[i]/50L;
						GameEnd.lGameScore[i]=GameEnd.lGameScore[i]*49L/50L;
					}
				}
			}

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//修改积分
			tagScoreInfo ScoreInfo;
			ScoreInfo.ScoreKind=enScoreKind_Flee;
			ScoreInfo.lScore=GameEnd.lGameScore[wChairID];
//			m_pITableFrame->WriteUserScore(wChairID,ScoreInfo);

			//结束游戏
			m_pITableFrame->ConcludeGame();

			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//发送场景
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_RF_FREE:		//空闲状态
		{
			//构造数据
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//设置变量
			StatusFree.lBaseScore=m_pGameServiceOption->lCellScore;

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_RF_PLAYING:		//游戏状态
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//设置变量
			StatusPlay.wLastOutUser=m_wTurnWiner;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.bTurnCardCount=m_bTurnCardCount;
			StatusPlay.lBaseScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusPlay.bCardCount,m_bCardCount,sizeof(m_bCardCount));
			CopyMemory(StatusPlay.bTurnCardData,m_bTurnCardData,sizeof(BYTE)*m_bTurnCardCount);

			//构造扑克
			CopyMemory(StatusPlay.lGameScore,m_lGameScore,sizeof(m_lGameScore));
			CopyMemory(StatusPlay.cbCardData[0],m_bHandCardData[wChiarID],sizeof(BYTE)*m_bCardCount[wChiarID]);
			CopyMemory(StatusPlay.cbCardData[1],m_bHandCardData[(wChiarID+2)%GAME_PLAYER],sizeof(BYTE)*m_bCardCount[(wChiarID+2)%GAME_PLAYER]);

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	return false;
}

//定时器事件
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	return false;
}

//游戏消息处理
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_OUT_CARD:		//用户出牌
		{
			//变量定义
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			WORD wHeadSize=sizeof(CMD_C_OutCard)-sizeof(pOutCard->bCardData);

			//效验数据
			ASSERT(wDataSize>=wHeadSize);
			if (wDataSize<wHeadSize) return false;
			ASSERT(wDataSize==(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0])));
			if (wDataSize!=(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0]))) return false;

			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//消息处理
			return OnUserOutCard(pUserData->wChairID,pOutCard->bCardData,pOutCard->bCardCount);
		}
	case SUB_C_PASS_CARD:
		{
			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//消息处理
			return OnUserPassCard(pUserData->wChairID);
		}
	}

	return true;
}

//框架消息处理
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//用户起来
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if (bLookonUser==false) m_wFirstUser=INVALID_CHAIR;
	return true;
}

//用户出牌
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount)
{
	//效验状态
	if (m_pITableFrame->GetGameStatus()!=GS_RF_PLAYING) return true;
	if (wChairID!=m_wCurrentUser) return false;

	//数目效验
	ASSERT(bCardCount<=MAX_COUNT);
	if (bCardCount>MAX_COUNT) return false;

	//类型判断
	BYTE bLastType=CT_ERROR;
	if (m_bTurnCardCount>0) bLastType=m_GameLogic.GetCardType(m_bTurnCardData,m_bTurnCardCount,true);
	BYTE bCardType=m_GameLogic.GetCardType(bCardData,bCardCount,((bLastType==CT_DOUBLE_BOMB)||(bLastType==CT_DOUBLE_LINE)));
	if (bCardType==CT_ERROR) return false;

	//更随出牌
	if (m_bTurnCardCount==0) m_bTurnCardCount=bCardCount;
	else if (m_GameLogic.CompareCard(bCardData,m_bTurnCardData,bCardCount,m_bTurnCardCount)==false) return false;

	//删除扑克
	if (m_GameLogic.RemoveCard(bCardData,bCardCount,m_bHandCardData[wChairID],m_bCardCount[wChairID])==false) return false;
	m_bCardCount[wChairID]-=bCardCount;

	//出牌记录
	m_bTurnCardCount=bCardCount;
	CopyMemory(m_bTurnCardData,bCardData,sizeof(BYTE)*bCardCount);

	//计算得分
	for (BYTE i=0;i<m_bTurnCardCount;i++)
	{
		if ((m_bTurnCardData[i]&LOGIC_MASK_VALUE)==5) m_lGameScore[2]+=5;
		if ((m_bTurnCardData[i]&LOGIC_MASK_VALUE)==10) m_lGameScore[2]+=10;
		if ((m_bTurnCardData[i]&LOGIC_MASK_VALUE)==13) m_lGameScore[2]+=10;
	}

	//胜利判断
	if (m_bCardCount[wChairID]==0)
	{
		//设置用户
		m_wWindUserList[m_wWinCount++]=wChairID;

		//结束判断
		if (m_bCardCount[(wChairID+2)%GAME_PLAYER]==0) m_wCurrentUser=INVALID_CHAIR;
	}

	//切换用户
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		//设置用户
		m_wTurnWiner=wChairID;
		m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;

		//用户调整
		while (m_bCardCount[m_wCurrentUser]==0) m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
	}

	//构造数据
	CMD_S_OutCard OutCard;
	OutCard.bCardCount=bCardCount;
	OutCard.wOutCardUser=wChairID;
	OutCard.wCurrentUser=m_wCurrentUser;
	CopyMemory(OutCard.bCardData,m_bTurnCardData,m_bTurnCardCount*sizeof(BYTE));

	//发送数据
	WORD wSendSize=sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE);
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);

	//结束判断
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//累计积分
		m_lGameScore[wChairID%2]+=m_lGameScore[2];
		m_lGameScore[2]=0L;

		//结束游戏
		OnEventGameEnd(wChairID,NULL,GER_NORMAL);
	}

	return true;
}

//用户放弃
bool CTableFrameSink::OnUserPassCard(WORD wChairID)
{
	//效验状态
	if (m_pITableFrame->GetGameStatus()!=GS_RF_PLAYING) return true;
	if ((wChairID!=m_wCurrentUser)||(m_bTurnCardCount==0)) return false;

	//用户调整
	do
	{
		m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
		if (m_wCurrentUser==m_wTurnWiner) 
		{
			m_bTurnCardCount=0;
			break;
		}
	} while (m_bCardCount[m_wCurrentUser]==0);

	//用户调整
	if (m_bCardCount[m_wCurrentUser]==0)
	{
		m_wTurnWiner=(m_wCurrentUser+2)%m_wPlayerCount;
		m_wCurrentUser=(m_wCurrentUser+2)%m_wPlayerCount;
	}

	//计算得分
	if (m_bTurnCardCount==0)
	{
		m_lGameScore[m_wCurrentUser%2]+=m_lGameScore[2];
		m_lGameScore[2]=0L;
	}

	//发送数据
	CMD_S_PassCard PassCard;
	PassCard.wPassUser=wChairID;
	PassCard.wCurrentUser=m_wCurrentUser;
	PassCard.bNewTurn=(m_bTurnCardCount==0)?TRUE:FALSE;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));

	return true;
}

//////////////////////////////////////////////////////////////////////////
