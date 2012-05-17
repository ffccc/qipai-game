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
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bUserHuXiCount,sizeof(m_bUserHuXiCount));

	//标志变量
	m_bOutCard=false;
	m_bDispatch=false;
	m_bEstimate=false;

	//提偎变量
	m_bTiCard=false;
	m_bSendNotify=false;

	//辅助变量
	m_cbOutCardData=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wOutCardUser=INVALID_CHAIR;

	//发牌信息
	m_bLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//用户状态
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_bUserAction,sizeof(m_bUserAction));
	ZeroMemory(m_bChiCardKind,sizeof(m_bChiCardKind));
	ZeroMemory(m_bPerformAction,sizeof(m_bPerformAction));

	//限制状态
	ZeroMemory(m_bMingPao,sizeof(m_bMingPao));
	ZeroMemory(m_bAbandonCard,sizeof(m_bAbandonCard));

	//组合扑克
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_bWeaveItemCount,sizeof(m_bWeaveItemCount));

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
	//QUERYINTERFACE(ITableFrameEvent,Guid,dwQueryVer);
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

	//设置参数
	//m_pITableFrame->SetLessTimes(50L);

	return true;
}

//复位桌子
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//游戏变量
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bUserHuXiCount,sizeof(m_bUserHuXiCount));

	//标志变量
	m_bOutCard=false;
	m_bDispatch=false;
	m_bEstimate=false;

	//提偎变量
	m_bTiCard=false;
	m_bSendNotify=false;

	//辅助变量
	m_cbOutCardData=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wOutCardUser=INVALID_CHAIR;

	//发牌信息
	m_bLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//用户状态
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_bUserAction,sizeof(m_bUserAction));
	ZeroMemory(m_bChiCardKind,sizeof(m_bChiCardKind));
	ZeroMemory(m_bPerformAction,sizeof(m_bPerformAction));

	//限制状态
	ZeroMemory(m_bMingPao,sizeof(m_bMingPao));
	ZeroMemory(m_bAbandonCard,sizeof(m_bAbandonCard));

	//组合扑克
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_bWeaveItemCount,sizeof(m_bWeaveItemCount));

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
	m_pITableFrame->SetGameStatus(GS_PLAYING);

	//混乱扑克
	m_bLeftCardCount=CountArray(m_cbRepertoryCard);
	m_GameLogic.RandCardData(m_cbRepertoryCard,CountArray(m_cbRepertoryCard));

	//分发扑克
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_bLeftCardCount-=(MAX_COUNT-1);
		m_GameLogic.SwitchToCardIndex(&m_cbRepertoryCard[m_bLeftCardCount],MAX_COUNT-1,m_cbCardIndex[i]);
	}

	//发送扑克
	BYTE cbBankerCard=m_cbRepertoryCard[--m_bLeftCardCount];
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(cbBankerCard)]++;

	//数据统计
	bool bTiCardAction=false;
	BYTE cbTiCardCount[GAME_PLAYER],cbWeiCardCount[GAME_PLAYER];
	BYTE cbTiCardIndex[GAME_PLAYER][5],cbWeiCardIndex[GAME_PLAYER][7];
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//类型统计
		cbTiCardCount[i]=m_GameLogic.GetAcitonTiCard(m_cbCardIndex[i],cbTiCardIndex[i]);
		cbWeiCardCount[i]=m_GameLogic.GetActionWeiCard(m_cbCardIndex[i],cbWeiCardIndex[i]);

		//设置标志
		if (cbTiCardCount[i]>0) bTiCardAction=true;
	}

	//设置变量
	m_bOutCard=true;
	m_cbOutCardData=0;
	m_wCurrentUser=m_wBankerUser;
	m_wOutCardUser=INVALID_CHAIR;

	//构造数据
	CMD_S_GameStart GameStart;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wCurrentUser=(bTiCardAction==true)?INVALID_CHAIR:m_wCurrentUser;

	//发送数据
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//设置数据
		m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameStart.cbCardData,CountArray(GameStart.cbCardData));

		//发送数据
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	//提牌处理
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//变量定义
		WORD wChairID=(m_wBankerUser+i)%GAME_PLAYER;

		//提牌处理
		if (cbTiCardCount[wChairID]>0)
		{
			//设置组合
			for (BYTE j=0;j<cbTiCardCount[wChairID];j++)
			{
				//变量定义
				BYTE cbRemoveCount=m_cbCardIndex[wChairID][cbTiCardIndex[wChairID][j]];
				BYTE cbTiCardData=m_GameLogic.SwitchToCardData(cbTiCardIndex[wChairID][j]);

				//设置扑克
				m_cbCardIndex[wChairID][cbTiCardIndex[wChairID][j]]=0;

				//设置组合
				BYTE cbIndex=m_bWeaveItemCount[wChairID]++;
				m_WeaveItemArray[wChairID][cbIndex].cbCardCount=4;
				m_WeaveItemArray[wChairID][cbIndex].cbWeaveKind=ACK_TI;
				m_WeaveItemArray[wChairID][cbIndex].cbCenterCard=cbTiCardData;
				m_WeaveItemArray[wChairID][cbIndex].cbCardList[0]=cbTiCardData;
				m_WeaveItemArray[wChairID][cbIndex].cbCardList[1]=cbTiCardData;
				m_WeaveItemArray[wChairID][cbIndex].cbCardList[2]=cbTiCardData;
				m_WeaveItemArray[wChairID][cbIndex].cbCardList[3]=cbTiCardData;

				//构造数据
				CMD_S_UserTiCard UserTiCard;
				UserTiCard.wActionUser=wChairID;
				UserTiCard.cbActionCard=cbTiCardData;
				UserTiCard.cbRemoveCount=cbRemoveCount;

				//发送数据
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_TI_CARD,&UserTiCard,sizeof(UserTiCard));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_TI_CARD,&UserTiCard,sizeof(UserTiCard));
			}

			//更新胡息
			UpdateUserHuXiCount(wChairID);
		}
	}

	//出牌提示
	if (bTiCardAction==true) SendOutCardNotify(m_wCurrentUser,true);

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

			//设置用户
			GameEnd.cbHuCard=0;
			GameEnd.cbReason=cbReason;
			GameEnd.wWinUser=INVALID_CHAIR;
			GameEnd.wProvideUser=INVALID_CHAIR;

			//剩余扑克
			BYTE cbPostion=0;
			BYTE cbCardData[MAX_COUNT];
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//转换扑克
				BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],cbCardData,CountArray(cbCardData));

				//设置扑克
				GameEnd.bCardCount[i]=cbCardCount;
				CopyMemory(&GameEnd.bCardData[cbPostion],cbCardData,cbCardCount*sizeof(BYTE));

				//设置索引
				cbPostion+=cbCardCount;
			}

			//结束信息
			CMD_S_GameEnd GameEndTemp;
			GameEndTemp=GameEnd;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
//				GameEndTemp.lGameScore[i]=m_pITableFrame->AccountGameGold(GameEndTemp.lGameScore[i]);
			}

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEndTemp,sizeof(GameEndTemp));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEndTemp,sizeof(GameEndTemp));

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
			BYTE cbPostion=0;
			BYTE cbCardData[MAX_COUNT];
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//转换扑克
				BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],cbCardData,CountArray(cbCardData));

				//设置扑克
				GameEnd.bCardCount[i]=cbCardCount;
				CopyMemory(&GameEnd.bCardData[cbPostion],cbCardData,cbCardCount*sizeof(BYTE));

				//设置索引
				cbPostion+=cbCardCount;
			}

			//设置用户
			GameEnd.cbReason=cbReason;
			GameEnd.wWinUser=wChairID;
			GameEnd.wProvideUser=INVALID_CHAIR;
			GameEnd.cbHuCard=(m_wOutCardUser!=INVALID_CHAIR)?m_cbOutCardData:0;;

			//胜利结束
			if (wChairID!=INVALID_CHAIR)
			{
				//统计胡息
				tagHuCardInfo HuCardInfo;
				m_GameLogic.GetHuCardInfo(m_cbCardIndex[wChairID],GameEnd.cbHuCard,m_bUserHuXiCount[wChairID],HuCardInfo);

				//计算积分
				LONG lScore=(((HuCardInfo.cbHuXiCount+m_bUserHuXiCount[wChairID])-15)/3+1)*m_pGameServiceOption->lCellScore;

				//统计积分
				GameEnd.lGameScore[wChairID]=2*lScore;
				GameEnd.lGameScore[(wChairID+1)%m_wPlayerCount]=-lScore;
				GameEnd.lGameScore[(wChairID+2)%m_wPlayerCount]=-lScore;

				//更换庄家
				m_wBankerUser=wChairID;
			}
			else 
			{
				//更换庄家
				m_wBankerUser=(m_wBankerUser+m_wPlayerCount-1)%m_wPlayerCount;
			}

			//结束信息
			CMD_S_GameEnd GameEndTemp;
			GameEndTemp=GameEnd;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
			//	GameEndTemp.lGameScore[i]=m_pITableFrame->AccountGameGold(GameEndTemp.lGameScore[i]);
			}

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEndTemp,sizeof(GameEndTemp));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEndTemp,sizeof(GameEndTemp));

			//修改积分
			tagScoreInfo ScoreInfo[m_wPlayerCount];
			memset(&ScoreInfo,0,sizeof(ScoreInfo));
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				ScoreInfo[i].lScore=GameEnd.lGameScore[i];
				ScoreInfo[i].ScoreKind=(GameEnd.lGameScore[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
			}
			//m_pITableFrame->WriteTableScore(ScoreInfo,m_wPlayerCount,GameEnd.lGameTax);

			//结束游戏
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_USER_LEFT:		//用户强退
		{
			//效验参数
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount);

			//构造数据
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			//设置用户
			GameEnd.cbHuCard=0;
			GameEnd.cbReason=cbReason;
			GameEnd.wWinUser=INVALID_CHAIR;
			GameEnd.wProvideUser=INVALID_CHAIR;

			//统计积分
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (i==wChairID) GameEnd.lGameScore[i]=-20*m_pGameServiceOption->lCellScore;
				else GameEnd.lGameScore[i]=10*m_pGameServiceOption->lCellScore;
			}

			//剩余扑克
			BYTE cbPostion=0;
			BYTE cbCardData[MAX_COUNT];
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//转换扑克
				BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],cbCardData,CountArray(cbCardData));

				//设置扑克
				GameEnd.bCardCount[i]=cbCardCount;
				CopyMemory(&GameEnd.bCardData[cbPostion],cbCardData,cbCardCount*sizeof(BYTE));

				//设置索引
				cbPostion+=cbCardCount;
			}

			//结束信息
			CMD_S_GameEnd GameEndTemp;
			GameEndTemp=GameEnd;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//GameEndTemp.lGameScore[i]=m_pITableFrame->AccountGameGold(GameEndTemp.lGameScore[i]);
			}

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEndTemp,sizeof(GameEndTemp));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEndTemp,sizeof(GameEndTemp));

			//修改积分
			tagScoreInfo ScoreInfo[m_wPlayerCount];
			memset(&ScoreInfo,0,sizeof(ScoreInfo));
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				ScoreInfo[i].lScore=GameEnd.lGameScore[i];
				ScoreInfo[i].ScoreKind=(GameEnd.lGameScore[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
			}
			//m_pITableFrame->WriteTableScore(ScoreInfo,m_wPlayerCount,GameEnd.lGameTax);

			//切换用户
			if (wChairID==m_wBankerUser) m_wBankerUser=(m_wBankerUser+m_wPlayerCount-1)%m_wPlayerCount;

			//通知消息
			TCHAR szMessage[128]=TEXT("");
			_snprintf(szMessage,CountArray(szMessage),TEXT("由于 [ %s ] 离开游戏，游戏结束"),pIServerUserItem->GetAccounts());
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				IServerUserItem * pISendUserItem=m_pITableFrame->GetServerUserItem(i);
				if (pISendUserItem!=NULL) m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_INFO);
			}
			WORD wIndex=0;
			do
			{
				IServerUserItem * pISendUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
				if (pISendUserItem==NULL) break;
				m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_INFO);
			} while (true);

			//结束游戏
			m_pITableFrame->ConcludeGame();

			return true;
		}
	}

	return false;
}

//发送场景
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret)
{
	switch (bGameStatus)
	{
	case GS_FREE:		//空闲状态
		{
			//变量定义
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//构造数据
			StatusFree.wBankerUser=m_wBankerUser;
			StatusFree.lCellScore=m_pGameServiceOption->lCellScore;

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_PLAYING:	//游戏状态
		{
			//变量定义
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//游戏信息
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;
			
			//出牌信息
			StatusPlay.wOutCardUser=m_wOutCardUser;
			StatusPlay.cbOutCardData=m_cbOutCardData;
			StatusPlay.bOutCard=(m_bOutCard==true)?TRUE:FALSE;

			//扑克信息
			StatusPlay.bLeftCardCount=m_bLeftCardCount;
			CopyMemory(StatusPlay.cbCardIndex,m_cbCardIndex[wChiarID],sizeof(m_cbCardIndex[wChiarID]));
			for (WORD i=0;i<m_wPlayerCount;i++) StatusPlay.bUserCardCount[i]=m_GameLogic.GetCardCount(m_cbCardIndex[i]);

			//组合信息
			CopyMemory(StatusPlay.WeaveItemArray,m_WeaveItemArray,sizeof(m_WeaveItemArray));
			CopyMemory(StatusPlay.bWeaveItemCount,m_bWeaveItemCount,sizeof(m_bWeaveItemCount));

			//动作信息
			StatusPlay.bUserAction=m_bUserAction[wChiarID];
			StatusPlay.bResponse=(m_bResponse[wChiarID]==true)?TRUE:FALSE;

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

//游戏消息处理
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_OUT_CARD:		//出牌消息
		{
			//效验消息
			ASSERT(wDataSize==sizeof(CMD_C_OutCard));
			if (wDataSize!=sizeof(CMD_C_OutCard)) return false;

			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//消息处理
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			return OnUserOutCard(pUserData->wChairID,pOutCard->cbCardData);
		}
	case SUB_C_OPERATE_CARD:	//操作消息
		{
			//效验消息
			ASSERT(wDataSize==sizeof(CMD_C_OperateCard));
			if (wDataSize!=sizeof(CMD_C_OperateCard)) return false;

			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//消息处理
			CMD_C_OperateCard * pOperateCard=(CMD_C_OperateCard *)pDataBuffer;
			return OnUserOperateCard(pUserData->wChairID,pOperateCard->cbOperateCode,pOperateCard->cbChiKind);
		}
	case SUB_C_CONTINUE_CARD:	//继续命令
		{
			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//消息处理
			return OnUserContinueCard(pUserData->wChairID);
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
bool __cdecl CTableFrameSink::OnEventUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//庄家设置
	if ((bLookonUser==false)&&(m_wBankerUser==INVALID_CHAIR))
	{
		m_wBankerUser=pIServerUserItem->GetChairID();
	}

	return true;
}

//用户起来
bool __cdecl CTableFrameSink::OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//庄家设置
	if ((bLookonUser==false)&&(wChairID==m_wBankerUser))
	{
		m_wBankerUser=INVALID_CHAIR;

		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if ((i!=wChairID)&&(m_pITableFrame->GetServerUserItem(i)!=NULL))
			{
				m_wBankerUser=i;
				break;
			}
		}
	}

	return true;
}

//用户出牌
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData)
{
	//效验参数
	ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);
	ASSERT((wChairID==m_wCurrentUser)&&(m_bOutCard==true));

	//效验状态
	if (m_GameLogic.IsValidCard(cbCardData)==false) return false;
	if ((wChairID!=m_wCurrentUser)||(m_bOutCard==false)) return false;

	//删除扑克
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//发送消息
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbOutCardData=cbCardData;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));

	//设置变量
	m_bOutCard=false;
	m_bDispatch=false;
	m_bEstimate=false;
	m_wOutCardUser=wChairID;
	m_cbOutCardData=cbCardData;
	m_wCurrentUser=INVALID_CHAIR;
	m_wResumeUser=(wChairID+m_wPlayerCount-1)%m_wPlayerCount;

	//臭牌记录
	m_bAbandonCard[wChairID][m_GameLogic.SwitchToCardIndex(m_cbOutCardData)]=true;

	//动作判断
	BYTE cbCurrentCard=(m_wOutCardUser!=INVALID_CHAIR)?m_cbOutCardData:0;
	bool bUserAction=EstimateUserRespond(m_wOutCardUser,cbCurrentCard,m_bDispatch);

	//派发扑克
	if (bUserAction==false) DispatchCardData((wChairID+m_wPlayerCount-1)%m_wPlayerCount);

	return true;
}

//用户操作
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbChiKind)
{
	//效验用户
	ASSERT(m_wCurrentUser==INVALID_CHAIR);
	if (m_wCurrentUser!=INVALID_CHAIR) return true;

	//效验状态
	ASSERT((m_bResponse[wChairID]==false)&&((cbOperateCode==ACK_NULL)||(m_bUserAction[wChairID]&cbOperateCode)!=0));
	if ((m_bResponse[wChairID]==true)||((cbOperateCode!=ACK_NULL)&&(m_bUserAction[wChairID]&cbOperateCode)==0)) return true;

	//变量定义
	BYTE cbTargetAction=cbOperateCode;
	WORD wTargetUser=(cbOperateCode==ACK_NULL)?INVALID_CHAIR:wChairID;

	//设置变量
	m_bResponse[wChairID]=true;
	m_bChiCardKind[wChairID]=cbChiKind;
	m_bPerformAction[wChairID]=cbOperateCode;

	//优先处理
	if ((cbOperateCode!=ACK_NULL)&&(m_bUserAction[wChairID]&ACK_CHI_EX)) m_bPerformAction[wChairID]|=ACK_CHI_EX;

	//执行判断
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//获取动作
		BYTE cbUserAction=(m_bResponse[i]==false)?m_bUserAction[i]:m_bPerformAction[i];

		//动作判断
		if (cbUserAction>cbTargetAction)
		{
			wTargetUser=i;
			cbTargetAction=cbUserAction;
		}
	}
	if ((wTargetUser!=INVALID_CHAIR)&&(m_bResponse[wTargetUser]==false)) return true;

	//吃胡排序
	if (cbTargetAction==ACK_CHIHU)
	{
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if ((m_bResponse[i]==false)&&(m_bUserAction[i]&ACK_CHIHU)) return true;
		}
	}

	//臭牌记录
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if ((m_bUserAction[i]>cbTargetAction)&&(i!=wTargetUser))
		{
			m_bAbandonCard[i][m_GameLogic.SwitchToCardIndex(m_cbOutCardData)]=true;
		}
	}

	//设置状态
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_bUserAction,sizeof(m_bUserAction));
	ZeroMemory(m_bPerformAction,sizeof(m_bPerformAction));

	//吃牌操作
	if (cbTargetAction&ACK_CHI)
	{
		//提取吃牌
		tagChiCardInfo ChiCardInfo[6];
		BYTE cbChiCardCount=m_GameLogic.GetActionChiCard(m_cbCardIndex[wTargetUser],m_cbOutCardData,ChiCardInfo);

		//吃牌搜索
		for (BYTE j=0;j<cbChiCardCount;j++)
		{
			if (ChiCardInfo[j].cbChiKind==m_bChiCardKind[wTargetUser]) break;
		}
		if (j==cbChiCardCount) return false;

		//吃牌处理
		BYTE cbDebarCard=m_cbOutCardData;
		for (BYTE k=0;k<ChiCardInfo[j].cbResultCount;k++)
		{
			//设置组合
			BYTE cbIndex=m_bWeaveItemCount[wTargetUser]++;
			m_WeaveItemArray[wTargetUser][cbIndex].cbCardCount=3;
			m_WeaveItemArray[wTargetUser][cbIndex].cbWeaveKind=ACK_CHI;
			m_WeaveItemArray[wTargetUser][cbIndex].cbCenterCard=m_cbOutCardData;
			m_WeaveItemArray[wTargetUser][cbIndex].cbCardList[0]=ChiCardInfo[j].cbCardData[k][0];
			m_WeaveItemArray[wTargetUser][cbIndex].cbCardList[1]=ChiCardInfo[j].cbCardData[k][1];
			m_WeaveItemArray[wTargetUser][cbIndex].cbCardList[2]=ChiCardInfo[j].cbCardData[k][2];

			//删除扑克
			for (BYTE l=0;l<3;l++)
			{
				BYTE cbCurrentCard=ChiCardInfo[j].cbCardData[k][l];
				if (cbCurrentCard==cbDebarCard) cbDebarCard=0;
				else m_cbCardIndex[wTargetUser][m_GameLogic.SwitchToCardIndex(cbCurrentCard)]--;
			}
		}

		//更新胡息
		UpdateUserHuXiCount(wTargetUser);

		//设置变量
		m_wOutCardUser=INVALID_CHAIR;

		//构造数据
		CMD_S_UserChiCard UserChiCard;
		UserChiCard.wActionUser=wTargetUser;
		UserChiCard.cbActionCard=m_cbOutCardData;
		UserChiCard.cbResultCount=ChiCardInfo[j].cbResultCount;
		CopyMemory(UserChiCard.cbCardData,ChiCardInfo[j].cbCardData,sizeof(UserChiCard.cbCardData));

		//发送数据
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_CHI_CARD,&UserChiCard,sizeof(UserChiCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_CHI_CARD,&UserChiCard,sizeof(UserChiCard));

		//出牌提示
		SendOutCardNotify(wTargetUser,true);

		return true;
	}

	//碰牌操作
	if (cbTargetAction&ACK_PENG)
	{
		//设置扑克
		m_cbCardIndex[wTargetUser][m_GameLogic.SwitchToCardIndex(m_cbOutCardData)]=0;

		//设置组合
		BYTE cbIndex=m_bWeaveItemCount[wTargetUser]++;
		m_WeaveItemArray[wTargetUser][cbIndex].cbCardCount=3;
		m_WeaveItemArray[wTargetUser][cbIndex].cbWeaveKind=ACK_PENG;
		m_WeaveItemArray[wTargetUser][cbIndex].cbCenterCard=m_cbOutCardData;
		m_WeaveItemArray[wTargetUser][cbIndex].cbCardList[0]=m_cbOutCardData;
		m_WeaveItemArray[wTargetUser][cbIndex].cbCardList[1]=m_cbOutCardData;
		m_WeaveItemArray[wTargetUser][cbIndex].cbCardList[2]=m_cbOutCardData;

		//更新胡息
		UpdateUserHuXiCount(wTargetUser);

		//设置变量
		m_wOutCardUser=INVALID_CHAIR;

		//构造数据
		CMD_S_UserPengCard UserPengCard;
		UserPengCard.wActionUser=wTargetUser;
		UserPengCard.cbActionCard=m_cbOutCardData;

		//发送数据
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_PENG_CARD,&UserPengCard,sizeof(UserPengCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_PENG_CARD,&UserPengCard,sizeof(UserPengCard));

		//出牌提示
		SendOutCardNotify(wTargetUser,true);

		return true;
	}

	//吃胡操作
	if (cbTargetAction&ACK_CHIHU)
	{
		//结束游戏
		OnEventGameEnd(wTargetUser,NULL,GER_NORMAL);

		return true;
	}

	//动作判断
	bool bUserAction=false;
	if (m_bEstimate==false)
	{
		BYTE cbCurrentCard=(m_wOutCardUser!=INVALID_CHAIR)?m_cbOutCardData:0;
		bUserAction=EstimateUserRespond(m_wOutCardUser,cbCurrentCard,m_bDispatch);
	}

	//派发扑克
	if (bUserAction==false) DispatchCardData(m_wResumeUser);

	return true;
}

//继续命令
bool CTableFrameSink::OnUserContinueCard(WORD wChairID)
{
	//效验用户
	ASSERT((wChairID==m_wCurrentUser)&&(m_bOutCard==false));
	if ((wChairID!=m_wCurrentUser)||(m_bOutCard==true)) return false;

	//派发扑克
	DispatchCardData((wChairID+m_wPlayerCount-1)%m_wPlayerCount);

	return true;
}

//派发扑克
bool CTableFrameSink::DispatchCardData(WORD wCurrentUser)
{
	//状态效验
	ASSERT(wCurrentUser!=INVALID_CHAIR);
	if (wCurrentUser==INVALID_CHAIR) return false;

	//结束判断
	if (m_bLeftCardCount==0)
	{
		OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);
		return true;
	}

	//发送扑克
	BYTE bSendCardData=m_cbRepertoryCard[--m_bLeftCardCount];

	//发送数据
	CMD_S_SendCard SendCard;
	SendCard.cbCardData=bSendCardData;
	SendCard.wAttachUser=wCurrentUser;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	//设置变量
	m_bDispatch=true;
	m_bEstimate=false;
	m_wCurrentUser=wCurrentUser;
	m_wOutCardUser=wCurrentUser;
	m_cbOutCardData=bSendCardData;
	m_wResumeUser=(wCurrentUser+m_wPlayerCount-1)%m_wPlayerCount;

	//动作判断
	bool bUserAction=EstimateUserRespond(wCurrentUser,bSendCardData,m_bDispatch);

	//出牌提示
	if (bUserAction==false) SendOutCardNotify(m_wCurrentUser,false);

	return true;
}

//出牌提示
bool CTableFrameSink::SendOutCardNotify(WORD wCurrentUser, bool bOutCard)
{
	//效验参数
	ASSERT(wCurrentUser!=INVALID_CHAIR);
	if (wCurrentUser==INVALID_CHAIR) return false;

	//过牌判断
	if (bOutCard==true)
	{
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			if ((m_cbCardIndex[wCurrentUser][i]>0)&&(m_cbCardIndex[wCurrentUser][i]<3)) break;
		}
		if (i==MAX_INDEX) bOutCard=false;
	}

	//设置变量
	m_bOutCard=bOutCard;
	m_wCurrentUser=wCurrentUser;

	//构造数据
	CMD_S_OutCardNotify OutCardNotify;
	OutCardNotify.wCurrentUser=m_wCurrentUser;
	OutCardNotify.bOutCard=(m_bOutCard==true)?TRUE:FALSE;

	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD_NOTIFY,&OutCardNotify,sizeof(OutCardNotify));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD_NOTIFY,&OutCardNotify,sizeof(OutCardNotify));

	return true;
}

//响应判断
bool CTableFrameSink::EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, bool bDispatch)
{
	//执行变量
	bool bPerformAction=false;

	//提牌判断
	if ((m_bEstimate==false)&&(bDispatch==true)&&(m_GameLogic.IsTiPaoCard(m_cbCardIndex[wCenterUser],cbCenterCard)==true))
	{
		//变量定义
		BYTE cbRemoveIndex=m_GameLogic.SwitchToCardIndex(cbCenterCard);
		BYTE cbRemoveCount=m_cbCardIndex[wCenterUser][cbRemoveIndex];

		//删除扑克
		m_cbCardIndex[wCenterUser][cbRemoveIndex]=0;

		//设置组合
		BYTE cbIndex=m_bWeaveItemCount[wCenterUser]++;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardCount=4;
		m_WeaveItemArray[wCenterUser][cbIndex].cbWeaveKind=ACK_TI;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCenterCard=m_cbOutCardData;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[0]=m_cbOutCardData;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[1]=m_cbOutCardData;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[2]=m_cbOutCardData;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[3]=m_cbOutCardData;

		//更新胡息
		UpdateUserHuXiCount(wCenterUser);

		//发送数据
		CMD_S_UserTiCard UserTiCard;
		UserTiCard.wActionUser=wCenterUser;
		UserTiCard.cbActionCard=cbCenterCard;
		UserTiCard.cbRemoveCount=cbRemoveCount;

		//发送数据
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_TI_CARD,&UserTiCard,sizeof(UserTiCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_TI_CARD,&UserTiCard,sizeof(UserTiCard));

		//设置变量
		cbCenterCard=0;
		bPerformAction=true;
		m_wResumeUser=wCenterUser;
		m_wOutCardUser=INVALID_CHAIR;

		//出牌提示
		m_bTiCard=true;
		m_bSendNotify=true;
	}

	//偎牌变提
	if ((bPerformAction==false)&&(m_bEstimate==false)&&(bDispatch==true))
	{
		for (BYTE cbIndex=0;cbIndex<m_bWeaveItemCount[wCenterUser];cbIndex++)
		{
			//变量定义
			BYTE cbWeaveKind=m_WeaveItemArray[wCenterUser][cbIndex].cbWeaveKind;
			BYTE cbWeaveCard=m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[0];

			//转换判断
			if ((cbCenterCard!=cbWeaveCard)||(cbWeaveKind!=ACK_WEI)) continue;

			//设置组合
			m_WeaveItemArray[wCenterUser][cbIndex].cbCardCount=4;
			m_WeaveItemArray[wCenterUser][cbIndex].cbWeaveKind=ACK_TI;
			m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[3]=cbCenterCard;

			//更新胡息
			UpdateUserHuXiCount(wCenterUser);

			//构造数据
			CMD_S_UserTiCard UserTiCard;
			UserTiCard.cbRemoveCount=0;
			UserTiCard.wActionUser=wCenterUser;
			UserTiCard.cbActionCard=cbCenterCard;

			//发送数据
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_TI_CARD,&UserTiCard,sizeof(UserTiCard));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_TI_CARD,&UserTiCard,sizeof(UserTiCard));

			//设置变量
			cbCenterCard=0;
			bPerformAction=true;
			m_wResumeUser=wCenterUser;
			m_wOutCardUser=INVALID_CHAIR;

			//出牌提示
			m_bTiCard=true;
			m_bSendNotify=true;

			break;
		}
	}

	//偎牌判断
	if ((bPerformAction==false)&&(bDispatch==true)&&(m_GameLogic.IsWeiPengCard(m_cbCardIndex[wCenterUser],cbCenterCard)==true))
	{
		//设置扑克
		m_cbCardIndex[wCenterUser][m_GameLogic.SwitchToCardIndex(cbCenterCard)]=0;

		//设置组合
		BYTE cbIndex=m_bWeaveItemCount[wCenterUser]++;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardCount=3;
		m_WeaveItemArray[wCenterUser][cbIndex].cbWeaveKind=ACK_WEI;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCenterCard=cbCenterCard;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[0]=cbCenterCard;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[1]=cbCenterCard;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[2]=cbCenterCard;

		//更新胡息
		UpdateUserHuXiCount(wCenterUser);

		//构造数据
		CMD_S_UserWeiCard UserWeiCard;
		UserWeiCard.wActionUser=wCenterUser;
		UserWeiCard.cbActionCard=cbCenterCard;

		//发送数据
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_WEI_CARD,&UserWeiCard,sizeof(UserWeiCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_WEI_CARD,&UserWeiCard,sizeof(UserWeiCard));

		//设置变量
		cbCenterCard=0;
		bPerformAction=true;
		m_wResumeUser=wCenterUser;
		m_wOutCardUser=INVALID_CHAIR;

		//出牌提示
		m_bTiCard=false;
		m_bSendNotify=true;
	}

	//胡牌判断
	if ((m_bEstimate==false)&&(m_bDispatch==true))	
	{
		//动作变量
		ZeroMemory(m_bResponse,sizeof(m_bResponse));
		ZeroMemory(m_bUserAction,sizeof(m_bUserAction));
		ZeroMemory(m_bChiCardKind,sizeof(m_bChiCardKind));
		ZeroMemory(m_bPerformAction,sizeof(m_bPerformAction));

		//胡牌判断
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//用户过滤
			if ((bDispatch==false)&&(i==wCenterUser)) continue;			

			//胡牌判断
			tagHuCardInfo HuCardInfo;
			if (m_GameLogic.GetHuCardInfo(m_cbCardIndex[i],cbCenterCard,m_bUserHuXiCount[i],HuCardInfo)==true) m_bUserAction[i]|=ACK_CHIHU;
		}

		//响应处理
		if ((m_bUserAction[0]!=ACK_NULL)||(m_bUserAction[1]!=ACK_NULL)||(m_bUserAction[2]!=ACK_NULL))
		{
			//设置用户
			m_bEstimate=true;
			m_wCurrentUser=INVALID_CHAIR;

			//发送消息
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				CMD_S_OperateNotify OperateNotify;
				OperateNotify.wResumeUser=m_wResumeUser;
				OperateNotify.cbActionCard=cbCenterCard;
				OperateNotify.cbOperateCode=m_bUserAction[i];
				m_pITableFrame->SendTableData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
			}

			return true;
		}
	}

	//设置标志
	m_bEstimate=true;

	//出牌提示
	if (m_bSendNotify==true)
	{
		//发送消息
		if (m_bTiCard==false) SendOutCardNotify(m_wResumeUser,true);
		else SendOutCardNotify(m_wResumeUser,IsAllowOutCard(m_wResumeUser));

		//设置变量
		m_bTiCard=false;
		m_bSendNotify=false;

		return true;
	}

	//跑牌判断
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//用户过滤
		if (i==wCenterUser) continue;

		//跑牌判断
		if (m_GameLogic.IsTiPaoCard(m_cbCardIndex[i],cbCenterCard)==true)
		{
			//变量定义
			WORD wActionUser=i;
			BYTE cbRemoveIndex=m_GameLogic.SwitchToCardIndex(cbCenterCard);
			BYTE cbRemoveCount=m_cbCardIndex[wActionUser][cbRemoveIndex];

			//删除扑克
			m_cbCardIndex[wActionUser][cbRemoveIndex]=0;

			//设置组合
			BYTE cbIndex=m_bWeaveItemCount[wActionUser]++;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardCount=4;
			m_WeaveItemArray[wActionUser][cbIndex].cbWeaveKind=ACK_PAO;
			m_WeaveItemArray[wActionUser][cbIndex].cbCenterCard=cbCenterCard;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardList[0]=cbCenterCard;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardList[1]=cbCenterCard;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardList[2]=cbCenterCard;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardList[3]=cbCenterCard;

			//更新胡息
			UpdateUserHuXiCount(wActionUser);

			//构造数据
			CMD_S_UserPaoCard UserPaoCard;
			UserPaoCard.wActionUser=wActionUser;
			UserPaoCard.cbActionCard=cbCenterCard;
			UserPaoCard.cbRemoveCount=cbRemoveCount;

			//发送数据
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_PAO_CARD,&UserPaoCard,sizeof(UserPaoCard));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_PAO_CARD,&UserPaoCard,sizeof(UserPaoCard));

			//设置变量
			m_wOutCardUser=INVALID_CHAIR;

			//出牌提示
			SendOutCardNotify(wActionUser,IsAllowOutCard(wActionUser));

			return true;
		}
	}

	//跑牌转换
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		for (BYTE cbIndex=0;cbIndex<m_bWeaveItemCount[i];cbIndex++)
		{
			//变量定义
			BYTE cbWeaveKind=m_WeaveItemArray[i][cbIndex].cbWeaveKind;
			BYTE cbWeaveCard=m_WeaveItemArray[i][cbIndex].cbCardList[0];

			//转换判断
			bool bChangeWeave=false;
			if (cbCenterCard==cbWeaveCard)
			{
				if (cbWeaveKind==ACK_WEI) bChangeWeave=true;
				else if ((cbWeaveKind==ACK_PENG)&&(wCenterUser==i)) bChangeWeave=true;
			}
			if (bChangeWeave==false) continue;

			//变量定义
			WORD wActionUser=i;
			BYTE cbResultWeave=((wCenterUser==i)&&(cbWeaveKind==ACK_WEI))?ACK_TI:ACK_PAO;

			//设置组合
			m_WeaveItemArray[wActionUser][cbIndex].cbCardCount=4;
			m_WeaveItemArray[wActionUser][cbIndex].cbWeaveKind=cbResultWeave;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardList[3]=cbCenterCard;

			//明跑判断
			if ((m_bDispatch==false)&&(cbResultWeave==ACK_PAO)) m_bMingPao[wCenterUser]=true;

			//更新胡息
			UpdateUserHuXiCount(wActionUser);

			//发送数据
			if (cbResultWeave==ACK_TI)
			{
				//提跑数据
				CMD_S_UserTiCard UserTiCard;
				UserTiCard.cbRemoveCount=0;
				UserTiCard.wActionUser=wActionUser;
				UserTiCard.cbActionCard=cbCenterCard;

				//发送数据
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_TI_CARD,&UserTiCard,sizeof(UserTiCard));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_TI_CARD,&UserTiCard,sizeof(UserTiCard));
			}
			else
			{
				//跑牌数据
				CMD_S_UserPaoCard UserPaoCard;
				UserPaoCard.cbRemoveCount=0;
				UserPaoCard.wActionUser=wActionUser;
				UserPaoCard.cbActionCard=cbCenterCard;

				//发送数据
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_PAO_CARD,&UserPaoCard,sizeof(UserPaoCard));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_PAO_CARD,&UserPaoCard,sizeof(UserPaoCard));
			}

			//设置变量
			m_wOutCardUser=INVALID_CHAIR;

			//出牌提示
			SendOutCardNotify(wActionUser,IsAllowOutCard(wActionUser));

			return true;
		}
	}

	//动作变量
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_bUserAction,sizeof(m_bUserAction));
	ZeroMemory(m_bChiCardKind,sizeof(m_bChiCardKind));
	ZeroMemory(m_bPerformAction,sizeof(m_bPerformAction));

	//吃碰判断
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//用户过滤
		if ((bDispatch==false)&&(i==wCenterUser)) continue;

		//吃碰判断
		BYTE cbOperateIndex=m_GameLogic.SwitchToCardIndex(cbCenterCard);
		if ((m_bMingPao[i]==false)&&(m_bAbandonCard[i][cbOperateIndex]==false))
		{
			//碰牌判断
			if (m_GameLogic.IsWeiPengCard(m_cbCardIndex[i],cbCenterCard)==true)	m_bUserAction[i]|=ACK_PENG;

			//吃牌判断
			WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
			if (((wEatUser==i)||(wCenterUser==i))&&(m_GameLogic.IsChiCard(m_cbCardIndex[i],cbCenterCard)==true))
			{
				m_bUserAction[i]|=ACK_CHI;
				if (i==wCenterUser) m_bUserAction[i]|=ACK_CHI_EX;
			}
		}
	}

	//响应处理
	if ((m_bUserAction[0]!=ACK_NULL)||(m_bUserAction[1]!=ACK_NULL)||(m_bUserAction[2]!=ACK_NULL))
	{
		//设置用户
		m_wCurrentUser=INVALID_CHAIR;

		//发送消息
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			CMD_S_OperateNotify OperateNotify;
			OperateNotify.wResumeUser=m_wResumeUser;
			OperateNotify.cbActionCard=cbCenterCard;
			OperateNotify.cbOperateCode=m_bUserAction[i];
			m_pITableFrame->SendTableData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
		}

		return true;
	}

	return false;
}

//出牌判断
bool CTableFrameSink::IsAllowOutCard(WORD wChairID)
{
	//跑提计算
	BYTE cbTiPaoCount=0;
	for (WORD i=0;i<m_bWeaveItemCount[wChairID];i++)
	{
		if (m_WeaveItemArray[wChairID][i].cbCardCount==4) cbTiPaoCount++;
	}

	return (cbTiPaoCount<=1)?true:false;
}

//更新胡息
bool CTableFrameSink::UpdateUserHuXiCount(WORD wChairID)
{
	//胡息计算
	BYTE bUserHuXiCount=0;
	for (BYTE i=0;i<m_bWeaveItemCount[wChairID];i++)
	{
		bUserHuXiCount+=m_GameLogic.GetWeaveHuXi(m_WeaveItemArray[wChairID][i]);
	}
	
	//设置胡息
	m_bUserHuXiCount[wChairID]=bUserHuXiCount;

	return true;
}

//////////////////////////////////////////////////////////////////////////
