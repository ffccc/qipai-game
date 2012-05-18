#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//辅助时间
#define TIME_LESS					1									//最少时间

//游戏时间
#define TIME_OUT_CARD				8									//出牌时间
#define TIME_START_GAME				8									//开始时间

#define TIME_PASS_CARD				3									//PASS时间

//游戏时间
#define IDI_OUT_CARD				(IDI_ANDROID_ITEM_SINK+0)			//出牌时间
#define IDI_START_GAME				(IDI_ANDROID_ITEM_SINK+1)			//开始时间
#define IDI_PASS_CARD				(IDI_ANDROID_ITEM_SINK+2)			//PASS时间

//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	m_wOutCardUser = INVALID_CHAIR;
	m_wMeChairId =  INVALID_CHAIR;

	//游戏变量
	m_bOutCard3=TRUE;
	m_wBankerUser=INVALID_CHAIR;
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bCardData,0,sizeof(m_bCardData));
	ZeroMemory(&m_AnalysedCard,sizeof(m_AnalysedCard));

	//出牌变量
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//出牌记录
	ZeroMemory( m_bOutCardRecord,sizeof(m_bOutCardRecord) );
	m_bOutCardCount = 0;

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
	m_wOutCardUser = INVALID_CHAIR;
	m_wMeChairId = INVALID_CHAIR;

	//游戏变量
	m_bOutCard3=TRUE;
	m_wBankerUser=INVALID_CHAIR;
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bCardData,0,sizeof(m_bCardData));
	ZeroMemory(&m_AnalysedCard,sizeof(m_AnalysedCard));

	//出牌变量
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//出牌记录
	ZeroMemory( m_bOutCardRecord,sizeof(m_bOutCardRecord) );
	m_bOutCardCount = 0;

	return true;
}

//时间消息
bool __cdecl CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:	//开始游戏
		{
			//开始判断
			m_pIAndroidUserItem->SendUserReady(NULL,0);

			return true;
		}
	case IDI_OUT_CARD:		//用户出牌
		{
			tagOutCardResult OutCardResult;

			ASSERT( m_bOutCardCount <= FULL_COUNT );
			//判断一轮结束
			for( int i = 0; i < GAME_PLAYER; i++ )
			{
				if( m_bCardCount[i]==0 ) return true;
			}

			//首次出牌
			if((m_bOutCard3==TRUE)&&(m_wBankerUser==m_pIAndroidUserItem->GetChairID())&&(m_bCardCount[m_wBankerUser]==13) )
			{
				if( m_GameLogic.AutomatismPlay( m_AnalysedCard,m_pIAndroidUserItem->GetChairID(),m_wOutCardUser,m_bTurnCardData,m_bTurnCardCount,
					m_bCardData,m_bCardCount,m_bOutCardRecord,m_bOutCardCount,OutCardResult,true ) )
				{
					//删除扑克
					BYTE bSourceCount=m_bCardCount[m_wMeChairId];
					m_bCardCount[m_wMeChairId] -=OutCardResult.cbCardCount;
					m_GameLogic.RemoveCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,m_bCardData[m_wMeChairId],bSourceCount);

					//发送数据
					CMD_C_OutCard OutCard;
					OutCard.bCardCount=OutCardResult.cbCardCount;
					CopyMemory( OutCard.bCardData,OutCardResult.cbResultCard,sizeof(BYTE)*OutCardResult.cbCardCount );
					m_pIAndroidUserItem->SendSocketData( SUB_C_OUT_CART,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE) );

					return true;
				}
				ASSERT(FALSE);
				return false;
			}

			//非首次出牌
			if( m_GameLogic.AutomatismPlay( m_AnalysedCard,m_pIAndroidUserItem->GetChairID(),m_wOutCardUser,m_bTurnCardData,m_bTurnCardCount,
				m_bCardData,m_bCardCount,m_bOutCardRecord,m_bOutCardCount,OutCardResult) )
			{
				//删除扑克
				BYTE bSourceCount=m_bCardCount[m_wMeChairId];
				m_bCardCount[m_wMeChairId] -=OutCardResult.cbCardCount;
				m_GameLogic.RemoveCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,m_bCardData[m_wMeChairId],bSourceCount);

				//发送数据
				CMD_C_OutCard OutCard;
				OutCard.bCardCount=OutCardResult.cbCardCount;
				CopyMemory( OutCard.bCardData,OutCardResult.cbResultCard,sizeof(BYTE)*OutCardResult.cbCardCount );
				m_pIAndroidUserItem->SendSocketData( SUB_C_OUT_CART,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE) );

				return true;
			}

			m_pIAndroidUserItem->SendSocketData( SUB_C_PASS_CARD );
			return true;
		}
	case IDI_PASS_CARD:
		{
			m_pIAndroidUserItem->SendSocketData( SUB_C_PASS_CARD );
			return true;
		}
	}

	return false;
}

//游戏消息
bool __cdecl CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:		//游戏开始
		{
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_OUT_CARD:		//用户出牌
		{
			return OnSubOutCard(pData,wDataSize);
		}
	case SUB_S_PASS_CARD:		//放弃出牌
		{
			return OnSubPassCard(pData,wDataSize);
		}
	case SUB_S_GAME_END:		//游戏结束
		{
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_STRUSTEE:
		{
			return true;
		}
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
	case GS_RF_FREE:	//空闲状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//变量定义
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
			IServerUserItem * pIServerUserItem=m_pIAndroidUserItem->GetMeUserItem();

			//玩家设置
			if (pIServerUserItem->GetUserStatus()!=US_READY)
			{
				UINT nElapse=rand()%TIME_START_GAME+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
			}

			return true;
		}
	case GS_RF_PLAYING:		//游戏状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//设置变量
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_bTurnCardCount=pStatusPlay->bTurnCardCount;
			m_bTurnOutType=m_GameLogic.GetCardType(pStatusPlay->bTurnCardData,m_bTurnCardCount);

			CopyMemory(m_bCardData,pStatusPlay->bCardData,sizeof(m_bCardData));

			CopyMemory(m_bTurnCardData,pStatusPlay->bTurnCardData,pStatusPlay->bTurnCardCount);
			CopyMemory(m_bCardCount,pStatusPlay->bCardCount,sizeof(BYTE)*GAME_PLAYER);

			m_wOutCardUser = pStatusPlay->wLastOutUser;


			//设置定时器
			if (pStatusPlay->wCurrentUser==m_pIAndroidUserItem->GetChairID())
			{
				UINT nElapse=rand()%TIME_OUT_CARD+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_OUT_CARD,nElapse);
			}

			return true;
		}
	}

	ASSERT(false);
	return false;
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

//庄家信息
bool CAndroidUserItemSink::OnSubGameStart(void * pData, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pData;

	//设置变量
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	m_bOutCard3=pGameStart->cbOutCard3;
	m_wBankerUser=pGameStart->wBankerUser;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	m_bOutCardCount = 0;
	ZeroMemory( m_bOutCardRecord,sizeof(m_bOutCardRecord) );

	//设置扑克
	CopyMemory(m_bCardData,pGameStart->cbCardData,sizeof(m_bCardData));

	//扑克数目
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_bCardCount[i]=13;
	}

	m_wMeChairId = m_pIAndroidUserItem->GetChairID();
	//分析分布
	m_GameLogic.AnalyseCardData( m_bCardData[m_wMeChairId],m_bCardCount[m_wMeChairId],m_AnalysedCard );

	//玩家设置
	if (pGameStart->wCurrentUser==m_pIAndroidUserItem->GetChairID())
	{
		UINT nElapse=rand()%TIME_OUT_CARD+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_OUT_CARD,nElapse);
	}

	return true;
}

//用户出牌
bool CAndroidUserItemSink::OnSubOutCard(void * pData, WORD wDataSize)
{
	//变量定义
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pData;
	WORD wHeadSize=sizeof(CMD_S_OutCard)-sizeof(pOutCard->bCardData);

	//效验数据
	ASSERT(wDataSize>=wHeadSize);
	if (wDataSize<wHeadSize) return false;
	ASSERT(wDataSize==(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0])));
	if (wDataSize!=(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0]))) return false;

	//设置变量
	if( pOutCard->wOutCardUser != m_wMeChairId )
	{
		m_GameLogic.RemoveCard(pOutCard->bCardData,pOutCard->bCardCount,
			m_bCardData[pOutCard->wOutCardUser],m_bCardCount[pOutCard->wOutCardUser]);
		m_bCardCount[pOutCard->wOutCardUser]-=pOutCard->bCardCount;
	}


	//记录出牌
	m_bTurnCardCount=pOutCard->bCardCount;
	m_bTurnOutType=m_GameLogic.GetCardType(pOutCard->bCardData,pOutCard->bCardCount);
	CopyMemory(m_bTurnCardData,pOutCard->bCardData,sizeof(BYTE)*pOutCard->bCardCount);

	m_wOutCardUser = pOutCard->wOutCardUser;

	//出牌数记录
	for( int i = 0; i < m_bTurnCardCount; i++ )
	{
		m_bOutCardRecord[m_bOutCardCount++] = m_bTurnCardData[i];
	}


	//最大判断
	if (pOutCard->wCurrentUser==pOutCard->wOutCardUser)
	{
		//设置变量
		m_bTurnCardCount=0;
		m_bTurnOutType=CT_ERROR;
		memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	}

	//玩家设置
	if (m_pIAndroidUserItem->GetChairID()==pOutCard->wCurrentUser)
	{
		UINT nElapse;
		//牌数小于出牌数自动PASS
		if( m_bTurnCardCount > m_bCardCount[pOutCard->wCurrentUser] )
		{
			nElapse=rand()%TIME_PASS_CARD+TIME_LESS;
			m_pIAndroidUserItem->SetGameTimer(IDI_PASS_CARD,nElapse);
			return true;
		}
		nElapse=rand()%TIME_OUT_CARD+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_OUT_CARD,nElapse);
	}

	return true;
}

//用户放弃
bool CAndroidUserItemSink::OnSubPassCard(void * pData, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_PassCard)) return false;
	CMD_S_PassCard * pPassCard=(CMD_S_PassCard *)pData;

	//一轮判断
	if (pPassCard->bNewTurn==TRUE)
	{
		m_bTurnCardCount=0;
		m_bTurnOutType=CT_ERROR;
		memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	}

	//玩家设置
	if (m_pIAndroidUserItem->GetChairID()==pPassCard->wCurrentUser)
	{
		UINT nElapse;
		//牌数小于出牌数自动PASS
		if( m_bTurnCardCount > m_bCardCount[pPassCard->wCurrentUser] )
		{
			nElapse=rand()%TIME_PASS_CARD+TIME_LESS;
			m_pIAndroidUserItem->SetGameTimer(IDI_PASS_CARD,nElapse);
			return true;
		}
		nElapse=rand()%TIME_OUT_CARD+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_OUT_CARD,nElapse);
	}

	return true;
}


//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(void * pData, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pData;

	//设置状态
	m_pIAndroidUserItem->SetGameStatus(GS_RF_FREE);

	//删除时间
	m_pIAndroidUserItem->KillGameTimer(IDI_OUT_CARD);

	//开始设置
	UINT nElapse=rand()%TIME_START_GAME+TIME_LESS;
	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

	return true;
}

//////////////////////////////////////////////////////////////////////////
