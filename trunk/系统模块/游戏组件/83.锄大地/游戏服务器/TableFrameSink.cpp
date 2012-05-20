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
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_bUserStrustee,sizeof(m_bUserStrustee));

	//历史积分
	memset(m_lAllTurnScore,0,sizeof(m_lAllTurnScore));
	memset(m_lLastTurnScore,0,sizeof(m_lLastTurnScore));

	//运行信息
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	memset(m_bCount2,0,sizeof(m_bCount2));
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
CTableFrameSink::~CTableFrameSink()
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

	//运行信息
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	memset(m_bCount2,0,sizeof(m_bCount2));
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
	BYTE bRandCard[52];
	m_GameLogic.RandCardList(bRandCard,CountArray(bRandCard));
	//分发扑克
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_bCardCount[i]=13;
		CopyMemory(&m_bHandCardData[i],&bRandCard[i*m_bCardCount[i]],sizeof(BYTE)*m_bCardCount[i]);
	}

	//设置用户 
	BYTE bOut3=FALSE;
	if (m_wBankerUser==INVALID_CHAIR)
	{
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			for (BYTE j=0;j<m_bCardCount[i];j++)
			{
				if (m_bHandCardData[i][j]==0x03) 
				{
					bOut3=TRUE;
					m_wBankerUser=i;
					break;
				}
			}
			if (m_wBankerUser!=INVALID_CHAIR) break;
		}
	}

	//设置用户
	m_wCurrentUser=m_wBankerUser;

	//排列扑克
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_GameLogic.SortCardList(m_bHandCardData[i],m_bCardCount[i]);
	}

	//发送扑克
	CMD_S_GameStart GameStart;
	GameStart.cbOutCard3=bOut3;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wCurrentUser=m_wCurrentUser;
	IServerUserItem *pUser=NULL;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		pUser = m_pITableFrame->GetServerUserItem(i);
		if( pUser->IsAndroidUser() )
			CopyMemory(GameStart.cbCardData,m_bHandCardData,sizeof(GameStart.cbCardData));
		else
		{
			CopyMemory(GameStart.cbCardData,m_bHandCardData[i],sizeof(BYTE)*m_bCardCount[i]);
		}
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

			//剩余扑克
			BYTE bCardPos=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}

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
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//剩余扑克
			BYTE bCardPos=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}

			//统计牌分
			LONG lCardScore[GAME_PLAYER];
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{
				LONG lScoreTimes=1;
				if (m_bCardCount[i]==13) lScoreTimes=4;
				else if (m_bCardCount[i]>=10) lScoreTimes=3;
				else if (m_bCardCount[i]>=8) lScoreTimes=2;
				lCardScore[i]=lScoreTimes*m_bCardCount[i];
			}

			//统计积分
			LONG lGameTax[GAME_PLAYER];
			ZeroMemory(lGameTax,sizeof(lGameTax));
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//积分信息
				for (WORD j=0;j<m_wPlayerCount;j++)
				{
					if (j!=i)
					{
						LONG lCellScore=m_pGameServiceOption->lCellScore;
						GameEnd.lGameScore[i]+=(lCardScore[j]-lCardScore[i])*lCellScore;
					}
				}
				
				//扣税
				if( GAME_GENRE_GOLD == m_pGameServiceOption->wServerType )
				{
					if( GameEnd.lGameScore[i] > 5L )
					{
						lGameTax[i] = GameEnd.lGameScore[i]*m_pGameServiceOption->wRevenue/1000L;
						GameEnd.lGameScore[i] -= lGameTax[i];
					}
				}
			}

			//历史积分
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				m_lAllTurnScore[i]+=GameEnd.lGameScore[i];
				m_lLastTurnScore[i]=GameEnd.lGameScore[i];
			}

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//修改分数
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				enScoreKind ScoreKind;
				if( GameEnd.lGameScore[i] == 0 ) ScoreKind = enScoreKind_Draw;
				else if( GameEnd.lGameScore[i] > 0 ) ScoreKind = enScoreKind_Win;
				else ScoreKind = enScoreKind_Lost;
				m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],lGameTax[i],ScoreKind);
			}

			//切换庄家
			m_wBankerUser=wChairID;

			//结束游戏
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_USER_LEFT:		//用户强退
		{
			//定义变量
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//剩余扑克
			BYTE bCardPos=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}

			//统计积分
			GameEnd.lGameScore[wChairID] = -200L*m_pGameServiceOption->lCellScore;
			

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//修改积分
			m_pITableFrame->WriteUserScore(wChairID,GameEnd.lGameScore[wChairID],0,enScoreKind_Flee);

			//切换庄家
			m_wBankerUser=INVALID_CHAIR;

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

			//托管标志
			CopyMemory(StatusFree.bStrustee,m_bUserStrustee,sizeof(m_bUserStrustee));

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_RF_PLAYING:		//游戏状态
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//设置变量
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wLastOutUser=m_wTurnWiner;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.bTurnCardCount=m_bTurnCardCount;
			StatusPlay.lBaseScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusPlay.bCardCount,m_bCardCount,sizeof(m_bCardCount));
			CopyMemory(StatusPlay.bTurnCardData,m_bTurnCardData,sizeof(BYTE)*m_bTurnCardCount);
			//托管标志
			CopyMemory(StatusPlay.bStrustee,m_bUserStrustee,sizeof(m_bUserStrustee));
			
			if( pIServerUserItem->IsAndroidUser() )
			{
				CopyMemory(StatusPlay.bCardData,m_bHandCardData,sizeof(m_bHandCardData));
			}
			else
			{
				CopyMemory(StatusPlay.bCardData,m_bHandCardData[wChiarID],sizeof(BYTE)*m_bCardCount[wChiarID]);
			}

			//历史积分
			CopyMemory(StatusPlay.lAllTurnScore,m_lAllTurnScore,sizeof(m_lAllTurnScore));
			CopyMemory(StatusPlay.lLastTurnScore,m_lLastTurnScore,sizeof(m_lLastTurnScore));

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
	case SUB_C_OUT_CART:		//用户出牌
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
	case SUB_C_STRUSTEE:
		{
			//变量定义
			CMD_C_Strustee *pStrustee = (CMD_C_Strustee *)pDataBuffer;

			//效验数据
			ASSERT( wDataSize == sizeof(CMD_C_Strustee) );
			if( wDataSize != sizeof(CMD_C_Strustee))  return false;

			//用户变量
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();

			//消息处理
			return OnUserStrustee(pUserData->wChairID,pStrustee->bStrustee);
		}
	}

	return false;
}

//框架消息处理
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//用户起来
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//庄家设置
	if (bLookonUser==false) 
	{
		m_wBankerUser=INVALID_CHAIR;
		if( m_bUserStrustee[wChairID] == true )
		{
			m_bUserStrustee[wChairID] = false;
			//托管标志
			CMD_S_Strustee Strustee;
			Strustee.bStrustee = m_bUserStrustee[wChairID];
			Strustee.wChairId = wChairID;
			IServerUserItem *pUserItem = NULL;
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				pUserItem = m_pITableFrame->GetServerUserItem(i);
				if( pUserItem == NULL || pUserItem == pIServerUserItem ) continue;
				m_pITableFrame->SendTableData(i,SUB_S_STRUSTEE,&Strustee,sizeof(Strustee));
			}
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_STRUSTEE,&Strustee,sizeof(Strustee));
		}
	}
	return true;
}

//用户出牌
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount)
{
	//效验状态
	if (m_pITableFrame->GetGameStatus()!=GS_RF_PLAYING) return true;
	if (wChairID!=m_wCurrentUser) return false;

	//类型判断
	BYTE bCardType=m_GameLogic.GetCardType(bCardData,bCardCount);
	if (bCardType==CT_ERROR) return false;

	//首出判断
/*	if ((wChairID==m_wBankerUser)&&(m_bCardCount[wChairID]==13))
	{
		for (BYTE i=0;i<bCardCount;i++)
		{
			if (bCardData[i]==0x03) break;
		}
		if (i==bCardCount) return false;
	}*/

	//强制出大
	WORD wNextUser=(wChairID+1)%GAME_PLAYER;
//	if ((m_bCardCount[wNextUser]==1)&&(bCardCount==1)&&(bCardData[0]!=m_bHandCardData[wChairID][0])) return false;

	//更随出牌
	if (m_bTurnCardCount==0) m_bTurnCardCount=bCardCount;
	else if (m_GameLogic.CompareCard(m_bTurnCardData,bCardData,m_bTurnCardCount,bCardCount)==false) return false;

	//删除扑克
	if (m_GameLogic.RemoveCard(bCardData,bCardCount,m_bHandCardData[wChairID],m_bCardCount[wChairID])==false) return false;
	m_bCardCount[wChairID]-=bCardCount;

	//大牌记录
	for (BYTE i=0;i<bCardCount;i++)
	{
		BYTE cbCardValue=m_GameLogic.GetCardValue(bCardData[i]);
		if (cbCardValue==0x02) m_bCount2[m_GameLogic.GetCardColor(bCardData[i])>>4]=true;
	}

	//大牌判断
	bool bMostest=false;
	BYTE cbMostValue=m_GameLogic.GetCardValue(bCardData[0]);
	BYTE cbMostColor=m_GameLogic.GetCardColor(bCardData[0]);

	//一牌判断
	if (cbMostValue==0x02)
	{
		for (BYTE i=(cbMostColor>>4);i<CountArray(m_bCount2);i++)
		{
			if (m_bCount2[i]==false) break;
		}
		if (i==CountArray(m_bCount2)) bMostest=true;
	}

	//出牌记录
	m_bTurnCardCount=bCardCount;
	CopyMemory(m_bTurnCardData,bCardData,sizeof(BYTE)*bCardCount);

	//切换用户
	m_wTurnWiner=wChairID;
	if (m_bCardCount[wChairID]!=0)
	{
		if (bMostest==false) m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
	}
	else m_wCurrentUser=INVALID_CHAIR;

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

	//出牌最大
	if (bMostest==true) m_bTurnCardCount=0;

	//结束判断
	if (m_wCurrentUser==INVALID_CHAIR) OnEventGameEnd(wChairID,NULL,GER_NORMAL);

	return true;
}

//用户放弃
bool CTableFrameSink::OnUserPassCard(WORD wChairID)
{
	//效验状态
	if (m_pITableFrame->GetGameStatus()!=GS_RF_PLAYING) return true;
	if ((wChairID!=m_wCurrentUser)||(m_bTurnCardCount==0)) return false;

	//设置变量
	m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
	if (m_wCurrentUser==m_wTurnWiner) m_bTurnCardCount=0;

	//发送数据
	CMD_S_PassCard PassCard;
	PassCard.wPassUser=wChairID;
	PassCard.wCurrentUser=m_wCurrentUser;
	PassCard.bNewTurn=(m_bTurnCardCount==0)?TRUE:FALSE;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));

	return true;
}

//用户托管
bool CTableFrameSink::OnUserStrustee(WORD wChairID,bool bStrustee)
{
	//效验
	ASSERT( m_bUserStrustee[wChairID] != bStrustee );
	if( m_bUserStrustee[wChairID] == bStrustee ) return true;

	//设置变量
	m_bUserStrustee[wChairID] = bStrustee;

	//发送数据
	CMD_S_Strustee Strustee;
	Strustee.wChairId = wChairID;
	Strustee.bStrustee = bStrustee;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_STRUSTEE,&Strustee,sizeof(Strustee));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_STRUSTEE,&Strustee,sizeof(Strustee));

	return true;
}

//////////////////////////////////////////////////////////////////////////
