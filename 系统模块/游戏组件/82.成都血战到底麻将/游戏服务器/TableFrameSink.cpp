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
	m_lSiceCount=0;
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	ZeroMemory( m_GangScore,sizeof(m_GangScore) );
	ZeroMemory( m_cbGenCount,sizeof(m_cbGenCount) );
	ZeroMemory( m_bPlayStatus,sizeof(m_bPlayStatus) );
	ZeroMemory( m_lGameScore,sizeof(m_lGameScore) );
	m_cbWinCount = 0;
	memset( m_wWinOrder,INVALID_CHAIR,sizeof(m_wWinOrder) );
	ZeroMemory( m_lGameTax,sizeof(m_lGameTax) );
	ZeroMemory( m_wLostFanShu,sizeof(m_wLostFanShu) );

	//出牌信息
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//堆立信息
	m_wHeapHand = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//发牌信息
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//运行变量
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//状态变量
	m_bSendStatus=false;
	m_bGangStatus = false;
	m_bGangOutStatus = false;
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//用户状态
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//组合扑克
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//结束信息
	m_cbChiHuCard=0;
	ZeroMemory( m_dwChiHuKind,sizeof(m_dwChiHuKind) );
	memset( m_wProvider,INVALID_CHAIR,sizeof(m_wProvider) );

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
	if (m_pITableFrame==NULL)
		return false;

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	return true;
}

//复位桌子
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//游戏变量
	m_lSiceCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	ZeroMemory( m_GangScore,sizeof(m_GangScore) );
	ZeroMemory( m_cbGenCount,sizeof(m_cbGenCount) );
	ZeroMemory( m_lGameScore,sizeof(m_lGameScore) );
	ZeroMemory( m_bPlayStatus,sizeof(m_bPlayStatus) );
	m_cbWinCount = 0;
	memset( m_wWinOrder,INVALID_CHAIR,sizeof(m_wWinOrder) );
	ZeroMemory( m_lGameTax,sizeof(m_lGameTax) );
	ZeroMemory( m_wLostFanShu,sizeof(m_wLostFanShu) );

	//出牌信息
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//堆立信息
	m_wHeapHand = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//发牌信息
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//运行变量
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;
	m_wBankerUser = INVALID_CHAIR;

	//状态变量
	m_bSendStatus=false;
	m_bGangStatus = false;
	m_bGangOutStatus = false;
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//用户状态
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//组合扑克
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//结束信息
	m_cbChiHuCard=0;
	ZeroMemory(m_dwChiHuKind,sizeof(m_dwChiHuKind));
	
	for( WORD i = 0; i < GAME_PLAYER; i++ )
		m_ChiHuRight[i].SetEmpty();

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
	m_pITableFrame->SetGameStatus(GS_MJ_PLAY);

	//混乱扑克
	m_lSiceCount = MAKELONG(MAKEWORD(rand()%6+1,rand()%6+1),MAKEWORD(rand()%6+1,rand()%6+1));
	m_cbLeftCardCount=CountArray(m_cbRepertoryCard);
	m_GameLogic.RandCardData(m_cbRepertoryCard,CountArray(m_cbRepertoryCard));
	m_wBankerUser = ((BYTE)(m_lSiceCount>>24)+(BYTE)(m_lSiceCount>>16)-1)%GAME_PLAYER;

	//test
	//BYTE byTest[] = {
	//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						
	//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						
	//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						
	//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,	
	//	0x11,0x12,0x27,0x13,0x15,0x16,0x17,0x18,0x19,						
	//	0x11,0x12,0x13,0x14,0x12,0x12,0x12,
	//	
	//	0x14,0x13,0x16,0x17,

	//	0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x17,0x22,
	//	0x23,0x24,0x25,0x25,
	//	
	//	0x17,0x17,0x17,0x21,0x22,0x23,0x24,0x25,0x26,
	//	0x27,0x28,0x29,0x12,

	//	0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x21,
	//	0x22,0x23,0x24,0x25,

	//	0x01,0x21,0x12,0x12,0x13,0x13,0x14,0x14,0x15,
	//	0x15,0x16,0x16,0x16,
	//};
	//CopyMemory( m_cbRepertoryCard,byTest,sizeof(byTest) );
	//m_wBankerUser = 0;
	//end test

	//分发扑克
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if( m_pITableFrame->GetServerUserItem(i) != NULL )
		{
			m_bPlayStatus[i] = true;
			m_cbLeftCardCount-=(MAX_COUNT-1);
			m_GameLogic.SwitchToCardIndex(&m_cbRepertoryCard[m_cbLeftCardCount],MAX_COUNT-1,m_cbCardIndex[i]);
		}
		else
		{
			m_bPlayStatus[i] = false;
		}
	}

	//发送扑克
	m_cbSendCardCount++;
	m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	//设置变量
	m_cbProvideCard=0;
	m_wProvideUser=INVALID_CHAIR;
	m_wCurrentUser=m_wBankerUser;

	//堆立信息
	WORD wSice = WORD(m_lSiceCount&0xffff);
	BYTE cbSiceTakeCount= HIBYTE(wSice)+LOBYTE(wSice);
	WORD wTakeChairID=(m_wBankerUser+cbSiceTakeCount-1)%GAME_PLAYER;
	BYTE cbTakeCount=MAX_REPERTORY-m_cbLeftCardCount;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//计算数目
		BYTE cbValidCount=HEAP_FULL_COUNT-m_cbHeapCardInfo[wTakeChairID][1]-((i==0)?(cbSiceTakeCount)*2:0);
		BYTE cbRemoveCount=__min(cbValidCount,cbTakeCount);

		//提取扑克
		cbTakeCount-=cbRemoveCount;
		m_cbHeapCardInfo[wTakeChairID][(i==0)?1:0]+=cbRemoveCount;

		//完成判断
		if (cbTakeCount==0)
		{
			m_wHeapHand=wTakeChairID;
			m_wHeapTail=(m_wBankerUser+cbSiceTakeCount-1)%GAME_PLAYER;
			//牌头加上4张,凑够108张牌
			m_cbHeapCardInfo[m_wHeapHand][0] -= 4;
			break;
		}
		//切换索引
		wTakeChairID=(wTakeChairID+1)%GAME_PLAYER;
	}

	//动作分析
	bool bAroseAction=false;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//庄家判断
		if (i==m_wBankerUser)
		{
			//杠牌判断
			tagGangCardResult GangCardResult;
			m_cbUserAction[i]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[i],NULL,0,GangCardResult);

			//胡牌判断
			CChiHuRight chr;
			m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]--;
			m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],NULL,0,m_cbSendCardData,chr);
			m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;
		}
	}

	//构造数据
	CMD_S_GameStart GameStart;
	GameStart.lSiceCount=m_lSiceCount;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wCurrentUser=m_wCurrentUser;
	GameStart.wHeapHand = m_wHeapHand;
	GameStart.wHeapTail = m_wHeapTail;
	GameStart.cbLeftCardCount = m_cbLeftCardCount;
	CopyMemory(GameStart.cbHeapCardInfo,m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//发送数据
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if( !m_bPlayStatus[i] ) continue;

		//设置变量
		GameStart.cbUserAction=m_cbUserAction[i];
		m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameStart.cbCardData);

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
	case GER_NORMAL:		//常规结束
		{
			//变量定义
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			GameEnd.wLeftUser = INVALID_CHAIR;

			//结束信息
			WORD wWinner = INVALID_CHAIR;
			BYTE cbLeftUserCount = 0;			//判断是否流局
			bool bUserStatus[GAME_PLAYER];		//
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
				m_ChiHuRight[i].GetRightData( &GameEnd.dwChiHuRight[i],MAX_RIGHT_COUNT );
				//流局玩家数
				if( m_ChiHuRight[i].IsEmpty() ) cbLeftUserCount++;
				//当前玩家状态
				if( NULL != m_pITableFrame->GetServerUserItem(i) ) bUserStatus[i] = true;
				else bUserStatus[i] = false;
			}
			
			LONG lHuaZhuScore[GAME_PLAYER];
			LONG lChaJiaoScore[GAME_PLAYER];
			LONG lGangScore[GAME_PLAYER];
			ZeroMemory( lGangScore,sizeof(lGangScore) );
			ZeroMemory( lHuaZhuScore,sizeof(lHuaZhuScore) );
			ZeroMemory( lChaJiaoScore,sizeof(lChaJiaoScore) );
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				if( cbLeftUserCount>1 && m_bPlayStatus[i] && m_ChiHuRight[i].IsEmpty() )
				{
					//查花猪
					bool bHuaZhu = m_GameLogic.IsHuaZhu(m_cbCardIndex[i],m_WeaveItemArray[i],m_cbWeaveItemCount[i]);
					if( bHuaZhu )
					{
						//花猪赔给非花猪玩家16倍点数。
						for( WORD j = 0; j < GAME_PLAYER; j++ )
						{
							if( j == i || !m_bPlayStatus[j] ) continue;
							if( !m_GameLogic.IsHuaZhu(m_cbCardIndex[j],m_WeaveItemArray[j],m_cbWeaveItemCount[j]) )
							{
								lHuaZhuScore[i] -= m_pGameServiceOption->lCellScore*16L;
								lHuaZhuScore[j] += m_pGameServiceOption->lCellScore*16L;
							}
						}
					}
					//流局的时候玩家还没有叫牌，则必须退回全部刮风下雨所得。
					if( !m_GameLogic.IsTingCard( m_cbCardIndex[i],m_WeaveItemArray[i],m_cbWeaveItemCount[i] ) )
					{
						//退回所有刮风下雨所得,即只计算扣分部分（只有在呼叫转移的情况出现）。
						for( WORD j = 0; j < m_GangScore[i].cbGangCount; j++ )
						{
							if( m_GangScore[i].lScore[j][i] < 0 )
							{
								for( WORD k = 0; k < GAME_PLAYER; k++ )
									lGangScore[k] += m_GangScore[i].lScore[j][k];
							}
						}
						//查大叫
						if( !bHuaZhu )
						{
							//没听牌的玩家（花猪不用）赔给听牌的玩家最大的可能番数
							for( WORD j = 0; j < GAME_PLAYER; j++ )
							{
								if( j == i || !m_bPlayStatus[j] ) continue;
								if( m_GameLogic.IsTingCard(m_cbCardIndex[j],m_WeaveItemArray[j],m_cbWeaveItemCount[j]) )
								{
									//查找最大番数
									WORD wFanShu = 0;
									CChiHuRight chr;
									for( BYTE k = 0; k < MAX_INDEX-7; k++ )
									{
										BYTE cbCurrentCard = m_GameLogic.SwitchToCardData(k);
										if( WIK_CHI_HU == m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[j],m_WeaveItemArray[j],
											m_cbWeaveItemCount[j],cbCurrentCard,chr) )
										{
											FiltrateRight(j,chr);
											WORD wFanShuTemp = m_GameLogic.GetChiHuActionRank(chr);
											//根,青龙七对和龙七对不算根
											BYTE cbGenCount = 0;
											if( (chr&CHR_QING_LONG_QI_DUI).IsEmpty() && (chr&CHR_LONG_QI_DUI).IsEmpty() )
											{
												BYTE cbCardIndexTemp[MAX_INDEX];
												CopyMemory( cbCardIndexTemp,m_cbCardIndex[j],sizeof(cbCardIndexTemp) );
												for( BYTE n = 0; n < m_cbWeaveItemCount[j]; n++ )
												{
													if( m_WeaveItemArray[j][n].cbWeaveKind == WIK_GANG ) continue;
													BYTE cbCardBuffer[4];
													BYTE cbCardCount = m_GameLogic.GetWeaveCard( m_WeaveItemArray[j][n].cbWeaveKind,
														m_WeaveItemArray[j][n].cbCenterCard,cbCardBuffer );
													for( BYTE m = 0; m < cbCardCount; m++ )
													{
														BYTE cbIndex = m_GameLogic.SwitchToCardIndex( cbCardBuffer[m] );
														cbCardIndexTemp[cbIndex]++;
													}
												}
												for( BYTE n = 0; n < MAX_INDEX; n++ )
												{
													if( cbCardIndexTemp[n] == 4 ) cbGenCount++;
												}
											}
											for( BYTE i = 0; i < cbGenCount; i++ )
												wFanShuTemp *= 2;
											//取最大番数
											if( wFanShuTemp > wFanShu ) wFanShu = wFanShuTemp;
										}
									}
									//查叫赔钱
									lChaJiaoScore[i] -= m_pGameServiceOption->lCellScore*wFanShu;
									lChaJiaoScore[j] += m_pGameServiceOption->lCellScore*wFanShu;
								}
							}
						}
					}
					//已叫牌，得杠分
					else
					{
						for( BYTE j = 0; j < m_GangScore[i].cbGangCount; j++ )
						{
							for( WORD k = 0; k < GAME_PLAYER; k++ )
								lGangScore[k] += m_GangScore[i].lScore[j][k];
						}
					}
				}
				//已胡牌玩家，算杠分
				else
				{
					for( BYTE j = 0; j < m_GangScore[i].cbGangCount; j++ )
					{
						for( WORD k = 0; k < GAME_PLAYER; k++ )
							lGangScore[k] += m_GangScore[i].lScore[j][k];
					}
				}
			}

			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				m_lGameScore[i] += lGangScore[i];
				m_lGameScore[i] += lHuaZhuScore[i];
				m_lGameScore[i] += lChaJiaoScore[i];

				//防止负分
				IServerUserItem *pUserItem = m_pITableFrame->GetServerUserItem(i);
				if( GAME_GENRE_SCORE!=m_pGameServiceOption->wServerType &&
					m_lGameScore[i] < 0L && -m_lGameScore[i] > pUserItem->GetUserScore()->lScore )
					m_lGameScore[i] = -pUserItem->GetUserScore()->lScore;
			}
		
			//统计积分
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if( NULL == m_pITableFrame->GetServerUserItem(i) ) continue;

				//设置积分
				if( GAME_GENRE_SCORE != m_pGameServiceOption->wServerType )
				{
					if (m_lGameScore[i]>0L)
					{
						m_lGameTax[i] = (LONG)((LONGLONG)m_lGameScore[i]*m_pGameServiceOption->wRevenue/1000L);
						m_lGameScore[i] -= m_lGameTax[i];
					}
				}
				
				enScoreKind ScoreKind;
				if( m_lGameScore[i] > 0L ) ScoreKind = enScoreKind_Win;
				else if( m_lGameScore[i] < 0L ) ScoreKind = enScoreKind_Lost;
				else ScoreKind = enScoreKind_Draw;

				//写入积分
				m_pITableFrame->WriteUserScore(i,m_lGameScore[i],m_lGameTax[i],ScoreKind);
			}

			CopyMemory( GameEnd.lGameScore,m_lGameScore,sizeof(m_lGameScore) );
			CopyMemory( GameEnd.lGangScore,lGangScore,sizeof(lGangScore) );
			CopyMemory( GameEnd.wWinOrder,m_wWinOrder,sizeof(m_wWinOrder) );
			CopyMemory( GameEnd.wProvideUser,m_wProvider,sizeof(m_wProvider) );
			CopyMemory( GameEnd.lGameTax,m_lGameTax,sizeof(m_lGameTax) );
			CopyMemory( GameEnd.cbGenCount,m_cbGenCount,sizeof(m_cbGenCount) );
			CopyMemory( GameEnd.wLostFanShu,m_wLostFanShu,sizeof(m_wLostFanShu) );

			//发送结束信息
			m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd) );
			m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd) );

			//结束游戏
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_DISMISS:		//游戏解散
		{
			//变量定义
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			GameEnd.wLeftUser = INVALID_CHAIR;

			//设置变量
			memset( GameEnd.wProvideUser,INVALID_CHAIR,sizeof(GameEnd.wProvideUser) );

			//拷贝扑克
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//结束游戏
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_USER_LEFT:		//用户强退
		{
			//变量定义
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			GameEnd.wLeftUser = wChairID;

			//结束信息
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
				m_ChiHuRight[i].GetRightData( &GameEnd.dwChiHuRight[i],MAX_RIGHT_COUNT );
			}

			LONG lGangScore[GAME_PLAYER];
			ZeroMemory( lGangScore,sizeof(lGangScore) );
			//扣分,逃跑者垫付所有刮风下雨的钱,并扣16倍点数*未胡牌玩家.
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				for( BYTE j = 0; j < m_GangScore[i].cbGangCount; j++ )
				{
					for( WORD k = 0; k < GAME_PLAYER; k++ )
					{
						if( m_GangScore[i].lScore[j][k] < 0 ) 
							lGangScore[wChairID] += m_GangScore[i].lScore[j][k];
						else if( k != wChairID )
							lGangScore[k] += m_GangScore[i].lScore[j][k];
					}
				}
			}
			m_bPlayStatus[wChairID] = false;
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				if( m_bPlayStatus[i] ) 
				{
					m_lGameScore[i] += 16L*m_pGameServiceOption->lCellScore;
					m_lGameScore[wChairID] -= 16L*m_pGameServiceOption->lCellScore;
				}
				m_lGameScore[i] += lGangScore[i];
			}

			//统计积分
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if( NULL == m_pITableFrame->GetServerUserItem(i) ) continue;

				//设置积分
				LONG lGameTax = 0L;
				if( GAME_GENRE_SCORE != m_pGameServiceOption->wServerType )
				{
					if (m_lGameScore[i]>0L)
					{
						lGameTax = (LONG)((LONGLONG)m_lGameScore[i]*m_pGameServiceOption->wRevenue/1000L);
						m_lGameScore[i] -= lGameTax;
					}
					else
					{
						//防止负分
						IServerUserItem *pUserItem = m_pITableFrame->GetServerUserItem(i);
						if( GAME_GENRE_SCORE!=m_pGameServiceOption->wServerType && -m_lGameScore[i] > pUserItem->GetUserScore()->lScore )
							m_lGameScore[i] = -pUserItem->GetUserScore()->lScore;
					}
				}

				enScoreKind ScoreKind;
				if( i == wChairID ) ScoreKind = enScoreKind_Flee;
				else if( m_lGameScore[i] > 0L ) ScoreKind = enScoreKind_Win;
				else if( m_lGameScore[i] < 0L ) ScoreKind = enScoreKind_Lost;
				else ScoreKind = enScoreKind_Draw;

				//写入积分
				m_pITableFrame->WriteUserScore(i,m_lGameScore[i],lGameTax,ScoreKind);
			}

			CopyMemory( GameEnd.lGameScore,m_lGameScore,sizeof(m_lGameScore) );
			CopyMemory( GameEnd.lGangScore,lGangScore,sizeof(lGangScore) );
			CopyMemory( GameEnd.wWinOrder,m_wWinOrder,sizeof(m_wWinOrder) );
			CopyMemory( GameEnd.wProvideUser,m_wProvider,sizeof(m_wProvider) );
			CopyMemory( GameEnd.wLostFanShu,m_wLostFanShu,sizeof(m_wLostFanShu) );

			//发送结束信息
			m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd) );
			m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd) );

			//结束游戏
			m_pITableFrame->ConcludeGame();

			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);
	return false;
}

//发送场景
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_MJ_FREE:	//空闲状态
		{
			//变量定义
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//构造数据
			StatusFree.wBankerUser=m_wBankerUser;
			StatusFree.lCellScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusFree.bTrustee,m_bTrustee,sizeof(m_bTrustee));

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_MJ_PLAY:	//游戏状态
		{
			//变量定义
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//游戏变量
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusPlay.bTrustee,m_bTrustee,sizeof(m_bTrustee));
			CopyMemory(StatusPlay.wWinOrder,m_wWinOrder,sizeof(m_wWinOrder));

			//状态变量
			StatusPlay.cbActionCard=m_cbProvideCard;
			StatusPlay.cbLeftCardCount=m_cbLeftCardCount;
			StatusPlay.cbActionMask=(m_bResponse[wChiarID]==false)?m_cbUserAction[wChiarID]:WIK_NULL;

			//历史记录
			StatusPlay.wOutCardUser=m_wOutCardUser;
			StatusPlay.cbOutCardData=m_cbOutCardData;
			CopyMemory(StatusPlay.cbDiscardCard,m_cbDiscardCard,sizeof(StatusPlay.cbDiscardCard));
			CopyMemory(StatusPlay.cbDiscardCount,m_cbDiscardCount,sizeof(StatusPlay.cbDiscardCount));

			//组合扑克
			CopyMemory(StatusPlay.WeaveItemArray,m_WeaveItemArray,sizeof(m_WeaveItemArray));
			CopyMemory(StatusPlay.cbWeaveCount,m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

			//堆立信息
			StatusPlay.wHeapHand = m_wHeapHand;
			StatusPlay.wHeapTail = m_wHeapTail;
			CopyMemory(StatusPlay.cbHeapCardInfo,m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

			//扑克数据
			StatusPlay.cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[wChiarID],StatusPlay.cbCardData);
			StatusPlay.cbSendCardData=((m_cbSendCardData!=0)&&(m_wProvideUser==wChiarID))?m_cbSendCardData:0x00;

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
			return OnUserOperateCard(pUserData->wChairID,pOperateCard->cbOperateCode,pOperateCard->cbOperateCard);
		}
	case SUB_C_TRUSTEE:
		{
			CMD_C_Trustee *pTrustee =(CMD_C_Trustee *)pDataBuffer;
			if(wDataSize != sizeof(CMD_C_Trustee)) return false;

			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();

			m_bTrustee[pUserData->wChairID]=pTrustee->bTrustee;
			CMD_S_Trustee Trustee;
			Trustee.bTrustee=pTrustee->bTrustee;
			Trustee.wChairID = pUserData->wChairID;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));

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
	return true;
}

//用户起来
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//庄家设置
	if (bLookonUser==false)
	{
		m_bTrustee[wChairID]=false;
		CMD_S_Trustee Trustee;
		Trustee.bTrustee=false;
		Trustee.wChairID = wChairID;
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
	}

	return true;
}

//用户出牌
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData)
{
	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//错误断言
	ASSERT(wChairID==m_wCurrentUser);
	ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);

	//效验参数
	if (wChairID!=m_wCurrentUser) return false;
	if (m_GameLogic.IsValidCard(cbCardData)==false) return false;

	//删除扑克
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//设置变量
	m_bSendStatus=true;
	if( m_bGangStatus )
	{
		m_bGangStatus = false;
		m_bGangOutStatus = true;
	}
	m_cbUserAction[wChairID]=WIK_NULL;
	m_cbPerformAction[wChairID]=WIK_NULL;

	//出牌记录
	m_cbOutCardCount++;
	m_wOutCardUser=wChairID;
	m_cbOutCardData=cbCardData;

	//构造数据
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbOutCardData=cbCardData;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));

	m_wProvideUser=wChairID;
	m_cbProvideCard=cbCardData;

	//用户切换
	m_wCurrentUser=(wChairID+m_wPlayerCount-1)%m_wPlayerCount;
	while( !m_bPlayStatus[m_wCurrentUser] ) 
		m_wCurrentUser=(m_wCurrentUser+m_wPlayerCount-1)%m_wPlayerCount;

	//响应判断
	bool bAroseAction=EstimateUserRespond(wChairID,cbCardData,EstimatKind_OutCard);

	//派发扑克
	if (bAroseAction==false) DispatchCardData(m_wCurrentUser);

	return true;
}

//用户操作
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard)
{
	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY)
		return true;

	//效验用户
	ASSERT((wChairID==m_wCurrentUser)||(m_wCurrentUser==INVALID_CHAIR));
	if ((wChairID!=m_wCurrentUser)&&(m_wCurrentUser!=INVALID_CHAIR)) 
		return false;

	//被动动作
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//效验状态
		ASSERT(m_bResponse[wChairID]==false);
		ASSERT((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)!=0));

		//效验状态
		if (m_bResponse[wChairID]==true) 
			return false;
		if ((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)==0))
			return false;

		//变量定义
		WORD wTargetUser=wChairID;
		BYTE cbTargetAction=cbOperateCode;

		//设置变量
		m_bResponse[wChairID]=true;
		m_cbPerformAction[wChairID]=cbOperateCode;
		m_cbOperateCard[wChairID]=(cbOperateCard==0)?m_cbProvideCard:cbOperateCard;

		//在自己未摸下一张牌的一圈内,不能弃上一家而胡下一家
		if( cbTargetAction == WIK_NULL && (WIK_CHI_HU&m_cbUserAction[wTargetUser]) &&
			m_wProvideUser != wTargetUser )
			m_bEnjoinChiHu[wTargetUser] = true;

		//执行判断
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//获取动作
			BYTE cbUserAction=(m_bResponse[i]==false)?m_cbUserAction[i]:m_cbPerformAction[i];

			//优先级别
			BYTE cbUserActionRank=m_GameLogic.GetUserActionRank(cbUserAction);
			BYTE cbTargetActionRank=m_GameLogic.GetUserActionRank(cbTargetAction);

			//动作判断
			if (cbUserActionRank>cbTargetActionRank)
			{
				wTargetUser=i;
				cbTargetAction=cbUserAction;
			}
		}
		if (m_bResponse[wTargetUser]==false) 
			return true;

		//吃胡等待
		if (cbTargetAction==WIK_CHI_HU)
		{
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if ((m_bResponse[i]==false)&&(m_cbUserAction[i]&WIK_CHI_HU))
					return true;
			}
		}

		//放弃操作
		if (cbTargetAction==WIK_NULL)
		{
			//用户状态
			ZeroMemory(m_bResponse,sizeof(m_bResponse));
			ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
			ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
			ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

			//发送扑克
			DispatchCardData(m_wResumeUser);

			return true;
		}

		//变量定义
		BYTE cbTargetCard=m_cbOperateCard[wTargetUser];

		//出牌变量
		m_cbOutCardData=0;
		m_bSendStatus=true;
		m_wOutCardUser=INVALID_CHAIR;
		m_bEnjoinChiHu[wTargetUser]=false;

		//胡牌操作
		if (cbTargetAction==WIK_CHI_HU)
		{
			//结束信息
			m_cbChiHuCard=cbTargetCard;

			for (WORD i=(m_wProvideUser+m_wPlayerCount-1)%GAME_PLAYER;i!=m_wProvideUser;i = (i+m_wPlayerCount-1)%GAME_PLAYER)
			{
				//过虑判断
				if ((m_cbPerformAction[i]&WIK_CHI_HU)==0)
					continue;

				//胡牌判断
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[i];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[i];
				m_dwChiHuKind[i] = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbChiHuCard,m_ChiHuRight[i]);

				//插入扑克
				if (m_dwChiHuKind[i]!=WIK_NULL) 
				{
					m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(m_cbChiHuCard)]++;
					ProcessChiHuUser( i,false );
				}
			}

			//判断结束
			BYTE cbPlayCount = 0;
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				if( m_bPlayStatus[i] ) cbPlayCount++;
			}

			if( cbPlayCount < 2 ) 
				OnEventGameEnd( INVALID_CHAIR,NULL,GER_NORMAL );
			else
			{
				//用户状态
				ZeroMemory(m_bResponse,sizeof(m_bResponse));
				ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
				ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
				ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

				//发送扑克
				m_wResumeUser = (m_wResumeUser+GAME_PLAYER-1)%GAME_PLAYER;
				while( !m_bPlayStatus[m_wResumeUser] ) m_wResumeUser = (m_wResumeUser+GAME_PLAYER-1)%GAME_PLAYER;
				DispatchCardData(m_wResumeUser);
			}

			return true;
		}

		//用户状态
		ZeroMemory(m_bResponse,sizeof(m_bResponse));
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
		ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

		//组合扑克
		ASSERT(m_cbWeaveItemCount[wTargetUser]<4);
		WORD wIndex=m_cbWeaveItemCount[wTargetUser]++;
		m_WeaveItemArray[wTargetUser][wIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wTargetUser][wIndex].cbCenterCard=cbTargetCard;
		m_WeaveItemArray[wTargetUser][wIndex].cbWeaveKind=cbTargetAction;
		m_WeaveItemArray[wTargetUser][wIndex].wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;

		//删除扑克
		switch (cbTargetAction)
		{
		case WIK_LEFT:		//上牌操作
			{
				//删除扑克
				BYTE cbRemoveCard[3];
				m_GameLogic.GetWeaveCard(WIK_LEFT,cbTargetCard,cbRemoveCard);
				VERIFY( m_GameLogic.RemoveCard(cbRemoveCard,3,&cbTargetCard,1) );
				VERIFY( m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,2) );

				break;
			}
		case WIK_RIGHT:		//上牌操作
			{
				//删除扑克
				BYTE cbRemoveCard[3];
				m_GameLogic.GetWeaveCard(WIK_RIGHT,cbTargetCard,cbRemoveCard);
				VERIFY( m_GameLogic.RemoveCard(cbRemoveCard,3,&cbTargetCard,1) );
				VERIFY( m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,2) );

				break;
			}
		case WIK_CENTER:	//上牌操作
			{
				//删除扑克
				BYTE cbRemoveCard[3];
				m_GameLogic.GetWeaveCard(WIK_CENTER,cbTargetCard,cbRemoveCard);
				VERIFY( m_GameLogic.RemoveCard(cbRemoveCard,3,&cbTargetCard,1) );
				VERIFY( m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,2) );

				break;
			}
		case WIK_PENG:		//碰牌操作
			{
				//删除扑克
				BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard};
				VERIFY( m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,2) );

				break;
			}
		case WIK_GANG:		//杠牌操作
			{
				//删除扑克,被动动作只存在放杠
				BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard,cbTargetCard};
				VERIFY( m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard)) );

				break;
			}
		default:
			ASSERT( FALSE );
			return false;
		}

		//构造结果
		CMD_S_OperateResult OperateResult;
		OperateResult.wOperateUser=wTargetUser;
		OperateResult.cbOperateCard=cbTargetCard;
		OperateResult.cbOperateCode=cbTargetAction;
		OperateResult.wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

		//设置用户
		m_wCurrentUser=wTargetUser;

		//杠牌处理
		if (cbTargetAction==WIK_GANG)
		{
			//发送信息
			CMD_S_GangScore gs;
			ZeroMemory( &gs,sizeof(gs) );
			gs.wChairId = wTargetUser;
			gs.cbXiaYu = FALSE;
			gs.lGangScore[wTargetUser] = m_pGameServiceOption->lCellScore*2L;
			gs.lGangScore[m_wProvideUser] = -m_pGameServiceOption->lCellScore*2L;
			m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_GANG_SCORE,&gs,sizeof(gs) );
			m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_GANG_SCORE,&gs,sizeof(gs) );

			//杠得分
			BYTE cbIndex = m_GangScore[wTargetUser].cbGangCount++;
			m_GangScore[wTargetUser].lScore[cbIndex][wTargetUser] = m_pGameServiceOption->lCellScore*2;
			m_GangScore[wTargetUser].lScore[cbIndex][m_wProvideUser] = -m_pGameServiceOption->lCellScore*2;
			m_bGangStatus = true;
			DispatchCardData(wTargetUser,true);
		}

		return true;
	}

	//主动动作
	if (m_wCurrentUser==wChairID)
	{
		//效验操作
		ASSERT((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)!=0));
		if ((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)==0))
			return false;

		//扑克效验
		ASSERT((cbOperateCode==WIK_NULL)||(cbOperateCode==WIK_CHI_HU)||(m_GameLogic.IsValidCard(cbOperateCard)==true));
		if ((cbOperateCode!=WIK_NULL)&&(cbOperateCode!=WIK_CHI_HU)&&(m_GameLogic.IsValidCard(cbOperateCard)==false)) 
			return false;

		//设置变量
		m_bSendStatus=true;
		m_bEnjoinChiHu[m_wCurrentUser]=false;
		m_cbUserAction[m_wCurrentUser]=WIK_NULL;
		m_cbPerformAction[m_wCurrentUser]=WIK_NULL;

		bool bPublic=false;

		//执行动作
		switch (cbOperateCode)
		{
		case WIK_GANG:			//杠牌操作
			{
				//变量定义
				BYTE cbWeaveIndex=0xFF;
				BYTE cbCardIndex=m_GameLogic.SwitchToCardIndex(cbOperateCard);

				//杠牌处理
				if (m_cbCardIndex[wChairID][cbCardIndex]==1)
				{
					//寻找组合
					for (BYTE i=0;i<m_cbWeaveItemCount[wChairID];i++)
					{
						BYTE cbWeaveKind=m_WeaveItemArray[wChairID][i].cbWeaveKind;
						BYTE cbCenterCard=m_WeaveItemArray[wChairID][i].cbCenterCard;
						if ((cbCenterCard==cbOperateCard)&&(cbWeaveKind==WIK_PENG))
						{
							bPublic=true;
							cbWeaveIndex=i;
							break;
						}
					}

					//效验动作
					ASSERT(cbWeaveIndex!=0xFF);
					if (cbWeaveIndex==0xFF) return false;

					//组合扑克
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=TRUE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;
				}
				else
				{
					//扑克效验
					ASSERT(m_cbCardIndex[wChairID][cbCardIndex]==4);
					if (m_cbCardIndex[wChairID][cbCardIndex]!=4) 
						return false;

					//设置变量
					bPublic=false;
					cbWeaveIndex=m_cbWeaveItemCount[wChairID]++;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=FALSE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;
				}

				//删除扑克
				m_cbCardIndex[wChairID][cbCardIndex]=0;

				m_bGangStatus = true;

				//杠牌得分
				LONG lScore = bPublic?m_pGameServiceOption->lCellScore:m_pGameServiceOption->lCellScore*2L;
				BYTE cbGangIndex = m_GangScore[wChairID].cbGangCount++;
				for( WORD i = 0; i < GAME_PLAYER; i++ )
				{
					if( !m_bPlayStatus[i] || i == wChairID ) continue;

					m_GangScore[wChairID].lScore[cbGangIndex][i] = -lScore;
					m_GangScore[wChairID].lScore[cbGangIndex][wChairID] += lScore;
				}

				//构造结果
				CMD_S_OperateResult OperateResult;
				OperateResult.wOperateUser=wChairID;
				OperateResult.wProvideUser=wChairID;
				OperateResult.cbOperateCode=cbOperateCode;
				OperateResult.cbOperateCard=cbOperateCard;

				//发送消息
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));


				//发送信息
				CMD_S_GangScore gs;
				gs.wChairId = wChairID;
				ZeroMemory( &gs,sizeof(gs) );
				gs.cbXiaYu = bPublic?FALSE:TRUE;
				for( WORD i = 0; i < GAME_PLAYER; i++ )
				{
					if( !m_bPlayStatus[i] || i == wChairID ) continue;

					gs.lGangScore[i] = -lScore;
					gs.lGangScore[wChairID] += lScore;
				}
				m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_GANG_SCORE,&gs,sizeof(gs) );
				m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_GANG_SCORE,&gs,sizeof(gs) );

				//效验动作
				bool bAroseAction=false;
				if (bPublic==true) bAroseAction=EstimateUserRespond(wChairID,cbOperateCard,EstimatKind_GangCard);

				//发送扑克
				if (bAroseAction==false)
				{
					DispatchCardData(wChairID,true);
				}
				return true;
			}
		case WIK_CHI_HU:		//吃胡操作
			{
				//吃牌权位
				if (m_cbOutCardCount==0)
				{
					m_wProvideUser = m_wCurrentUser;
					m_cbProvideCard = m_cbSendCardData;
				}

				//普通胡牌
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
				m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],&m_cbProvideCard,1);
				m_dwChiHuKind[wChairID] = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,m_cbProvideCard,
					m_ChiHuRight[wChairID]);
				m_cbCardIndex[wChairID][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]++;

				//结束信息
				m_cbChiHuCard=m_cbProvideCard;

				ProcessChiHuUser( wChairID,false );

				//判断结束
				BYTE cbPlayCount = 0;
				for( WORD i = 0; i < GAME_PLAYER; i++ )
				{
					if( m_bPlayStatus[i] ) cbPlayCount++;
				}

				if( cbPlayCount < 2 ) 
					OnEventGameEnd( INVALID_CHAIR,NULL,GER_NORMAL );
				else
				{
					//发牌
					m_wCurrentUser = (m_wCurrentUser+GAME_PLAYER-1)%GAME_PLAYER;
					while( !m_bPlayStatus[m_wCurrentUser] )
						m_wCurrentUser = (m_wCurrentUser+GAME_PLAYER-1)%GAME_PLAYER;
					DispatchCardData( m_wCurrentUser,false );
				}

				return true;
			}
		}

		return true;
	}

	return false;
}

//发送操作
bool CTableFrameSink::SendOperateNotify()
{
	//发送提示
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbUserAction[i]!=WIK_NULL)
		{
			//构造数据
			CMD_S_OperateNotify OperateNotify;
			OperateNotify.wResumeUser=m_wResumeUser;
			OperateNotify.cbActionCard=m_cbProvideCard;
			OperateNotify.cbActionMask=m_cbUserAction[i];

			//发送数据
			m_pITableFrame->SendTableData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
			m_pITableFrame->SendLookonData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
		}
	}

	return true;
}

//派发扑克
bool CTableFrameSink::DispatchCardData(WORD wCurrentUser,bool bTail)
{
	//状态效验
	ASSERT(wCurrentUser!=INVALID_CHAIR);
	if (wCurrentUser==INVALID_CHAIR)
		return false;

	//丢弃扑克
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		m_cbDiscardCount[m_wOutCardUser]++;
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]-1]=m_cbOutCardData;
	}

	//荒庄结束
	if (m_cbLeftCardCount==0)
	{
		m_cbChiHuCard=0;
		m_wProvideUser=INVALID_CHAIR;
		OnEventGameEnd(m_wProvideUser,NULL,GER_NORMAL);

		return true;
	}

	//设置变量
	m_cbOutCardData=0;
	m_wCurrentUser=wCurrentUser;
	m_wOutCardUser=INVALID_CHAIR;
	m_bEnjoinChiHu[wCurrentUser]=false;

	//杠后炮
	if( m_bGangOutStatus )
	{
		m_bGangOutStatus = false;
	}

	//发牌处理
	if (m_bSendStatus==true)
	{
		//发送扑克
		m_cbSendCardCount++;
		m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];

		//胡牌判断
		CChiHuRight chr;
		m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wCurrentUser],m_WeaveItemArray[wCurrentUser],
			m_cbWeaveItemCount[wCurrentUser],m_cbSendCardData,chr);

		//加牌
		m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

		//设置变量
		m_wProvideUser=wCurrentUser;
		m_cbProvideCard=m_cbSendCardData;

		//杠牌判断
		if ((m_bEnjoinChiPeng[wCurrentUser]==false)&&(m_cbLeftCardCount>0))
		{
			tagGangCardResult GangCardResult;
			m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[wCurrentUser],
				m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],GangCardResult);
		}
	}

	//堆立信息
	ASSERT( m_wHeapHand != INVALID_CHAIR && m_wHeapTail != INVALID_CHAIR );
	if( !bTail )
	{
		//切换索引
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapHand][0]+m_cbHeapCardInfo[m_wHeapHand][1];
		if (cbHeapCount==HEAP_FULL_COUNT) 
			m_wHeapHand=(m_wHeapHand+1)%CountArray(m_cbHeapCardInfo);
		m_cbHeapCardInfo[m_wHeapHand][0]++;
	}
	else
	{
		//切换索引
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapTail][0]+m_cbHeapCardInfo[m_wHeapTail][1];
		if (cbHeapCount==HEAP_FULL_COUNT) 
			m_wHeapTail=(m_wHeapTail+3)%CountArray(m_cbHeapCardInfo);
		m_cbHeapCardInfo[m_wHeapTail][1]++;
	}

	//构造数据
	CMD_S_SendCard SendCard;
	SendCard.wCurrentUser=wCurrentUser;
	SendCard.bTail = bTail;
	SendCard.cbActionMask=m_cbUserAction[wCurrentUser];
	SendCard.cbCardData=(m_bSendStatus==true)?m_cbSendCardData:0x00;

	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	return true;
}

//响应判断
bool CTableFrameSink::EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind)
{
	//变量定义
	bool bAroseAction=false;

	//用户状态
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//动作判断
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//用户过滤
		if (wCenterUser==i||!m_bPlayStatus[i]) continue;

		//出牌类型
		if (EstimatKind==EstimatKind_OutCard)
		{
			//吃碰判断
			if (m_bEnjoinChiPeng[i]==false)
			{
				////碰牌判断
				m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard);

				////吃牌判断
				//WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
				//if (wEatUser==i)
				//	m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard);

				//杠牌判断
				if (m_cbLeftCardCount>0) 
				{
					m_cbUserAction[i]|=m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard);
				}
			}
		}

		//胡牌判断
		if (m_bEnjoinChiHu[i]==false )
		{
			//吃胡判断
			CChiHuRight chr;
			BYTE cbWeaveCount=m_cbWeaveItemCount[i];
			m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,chr);
		}

		//结果判断
		if (m_cbUserAction[i]!=WIK_NULL) 
			bAroseAction=true;
	}

	//结果处理
	if (bAroseAction==true) 
	{
		//设置变量
		m_wProvideUser=wCenterUser;
		m_cbProvideCard=cbCenterCard;
		m_wResumeUser=m_wCurrentUser;
		m_wCurrentUser=INVALID_CHAIR;

		//发送提示
		SendOperateNotify();

		return true;
	}

	return false;
}

//
void CTableFrameSink::ProcessChiHuUser( WORD wChairId, bool bGiveUp )
{
	if( !bGiveUp )
	{
		//引用权位
		CChiHuRight &chr = m_ChiHuRight[wChairId];

		FiltrateRight( wChairId,chr );

		WORD wFanShu = m_GameLogic.GetChiHuActionRank( chr );
		//根,青龙七对和龙七对不算根
		if( (chr&CHR_QING_LONG_QI_DUI).IsEmpty() && (chr&CHR_LONG_QI_DUI).IsEmpty() )
		{
			BYTE cbCardIndexTemp[MAX_INDEX];
			CopyMemory( cbCardIndexTemp,m_cbCardIndex[wChairId],sizeof(cbCardIndexTemp) );
			for( BYTE i = 0; i < m_cbWeaveItemCount[wChairId]; i++ )
			{
				if( m_WeaveItemArray[wChairId][i].cbWeaveKind == WIK_GANG ) continue;
				BYTE cbCardBuffer[4];
				BYTE cbCardCount = m_GameLogic.GetWeaveCard( m_WeaveItemArray[wChairId][i].cbWeaveKind,
					m_WeaveItemArray[wChairId][i].cbCenterCard,cbCardBuffer );
				for( BYTE j = 0; j < cbCardCount; j++ )
				{
					BYTE cbIndex = m_GameLogic.SwitchToCardIndex( cbCardBuffer[j] );
					cbCardIndexTemp[cbIndex]++;
				}
			}
			for( BYTE i = 0; i < MAX_INDEX; i++ )
			{
				if( cbCardIndexTemp[i] == 4 ) m_cbGenCount[wChairId]++;
			}
		}
		for( BYTE i = 0; i < m_cbGenCount[wChairId]; i++ )
			wFanShu *= 2;
		LONG lChiHuScore = wFanShu*m_pGameServiceOption->lCellScore;
		
		//杠上炮,呼叫转移
		if( !(chr&CHR_GANG_SHANG_PAO).IsEmpty() )
		{
			BYTE cbGangIndex = m_GangScore[m_wProvideUser].cbGangCount-1;
			//一炮多响的情况下,胡牌者平分杠得分
			BYTE cbChiHuCount = 0;
			for( WORD i = 0; i < GAME_PLAYER; i++ )
				if( m_cbPerformAction[i]==WIK_CHI_HU ) cbChiHuCount++;
			if( cbChiHuCount == 1 )
			{
				LONG lScore = m_GangScore[m_wProvideUser].lScore[cbGangIndex][wChairId];
				m_GangScore[m_wProvideUser].lScore[cbGangIndex][wChairId] = m_GangScore[m_wProvideUser].lScore[cbGangIndex][m_wProvideUser];
				m_GangScore[m_wProvideUser].lScore[cbGangIndex][m_wProvideUser] = lScore;
			}
			else
			{
				LONG lGangScore = m_GangScore[m_wProvideUser].lScore[cbGangIndex][m_wProvideUser]/cbChiHuCount;
				lGangScore = __max(lGangScore,m_pGameServiceOption->lCellScore);
				for( WORD i = 0; i < GAME_PLAYER; i++ )
				{
					if( m_cbPerformAction[i]==WIK_CHI_HU )
						m_GangScore[m_wProvideUser].lScore[cbGangIndex][i] = lGangScore;
				}
				m_GangScore[m_wProvideUser].lScore[cbGangIndex][m_wProvideUser] = 0;
				for( WORD i = 0; i < GAME_PLAYER; i++ )
				{
					if( i != m_wProvideUser)
						m_GangScore[m_wProvideUser].lScore[cbGangIndex][m_wProvideUser] += m_GangScore[m_wProvideUser].lScore[cbGangIndex][i];
				}
			}
		}
		//抢杠杠分不算
		else if( !(chr&CHR_QIANG_GANG).IsEmpty() )
		{
			m_GangScore[m_wProvideUser].cbGangCount--;
		}

		if( m_wProvideUser != wChairId )
			m_wLostFanShu[m_wProvideUser][wChairId] = lChiHuScore;
		else
		{
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				if( !m_bPlayStatus[i] || i == wChairId ) continue;

				m_wLostFanShu[i][wChairId] = lChiHuScore;
			}
		}

		if( m_wProvideUser == wChairId )
		{
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				if( !m_bPlayStatus[i] || i == wChairId ) continue;

				//胡牌分
				m_lGameScore[i] -= lChiHuScore;
				m_lGameScore[wChairId] += lChiHuScore;
			}
		}
		//点炮
		else
		{
			m_lGameScore[m_wProvideUser] -= lChiHuScore;
			m_lGameScore[wChairId] += lChiHuScore;
		}

		//设置变量
		m_wProvider[wChairId] = m_wProvideUser;
		m_wWinOrder[m_cbWinCount++] = wChairId;
		m_bGangStatus = false;
		m_bGangOutStatus = false;

		//发送消息
		CMD_S_ChiHu ChiHu;
		ChiHu.wChiHuUser = wChairId;
		ChiHu.wProviderUser = m_wProvideUser;
		ChiHu.lGameScore = m_lGameScore[wChairId];
		ChiHu.cbCardCount = m_GameLogic.GetCardCount( m_cbCardIndex[wChairId] );
		ChiHu.cbChiHuCard = m_cbProvideCard;
		ChiHu.cbWinOrder = m_cbWinCount;
		m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_CHI_HU,&ChiHu,sizeof(ChiHu) );
		m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_CHI_HU,&ChiHu,sizeof(ChiHu) );

		//设置变量
		m_bPlayStatus[wChairId] = false;
	}

	return;
}

//
void CTableFrameSink::FiltrateRight( WORD wChairId,CChiHuRight &chr )
{
	//权位增加
	//抢杠
	if( m_wCurrentUser == INVALID_CHAIR && m_bGangStatus )
		chr |= CHR_QIANG_GANG;
	//天胡
	if( m_cbSendCardCount == 1 && m_cbOutCardCount == 0 )
		chr |= CHR_TIAN_HU;
	//地胡
	else if( m_cbSendCardCount == 1 && m_cbOutCardCount == 1 )
		chr |= CHR_DI_HU;
	//清对
	if( !(chr&CHR_QING_YI_SE).IsEmpty() && !(chr&CHR_DA_DUI_ZI).IsEmpty() )
		chr |= CHR_QING_DUI;
	//龙七对
	if( !(chr&CHR_QI_XIAO_DUI).IsEmpty() )
	{
		for( BYTE i = 0; i < MAX_INDEX; i++ )
		{
			if( m_cbCardIndex[wChairId][i] == 4 )
			{
				chr |= CHR_LONG_QI_DUI;
				break;
			}
		}
	}
	//清七对
	if( !(chr&CHR_QING_YI_SE).IsEmpty() && !(chr&CHR_QI_XIAO_DUI).IsEmpty() )
		chr |= CHR_QING_QI_DUI;
	//清幺九
	if( !(chr&CHR_QING_YI_SE).IsEmpty() && !(chr&CHR_DAI_YAO).IsEmpty() )
		chr |= CHR_QING_YAO_JIU;
	//青龙七对
	if( !(chr&CHR_QING_YI_SE).IsEmpty() && !(chr&CHR_LONG_QI_DUI).IsEmpty() )
		chr |= CHR_QING_LONG_QI_DUI;

	//取最大权位
	if( !(chr&CHR_QING_LONG_QI_DUI).IsEmpty() )
		chr = CHR_QING_LONG_QI_DUI;
	else if( !(chr&CHR_TIAN_HU).IsEmpty() )
		chr = CHR_TIAN_HU;
	else if( !(chr&CHR_DI_HU).IsEmpty() )
		chr = CHR_DI_HU;
	else if( !(chr&CHR_QING_YAO_JIU).IsEmpty() )
		chr = CHR_QING_YAO_JIU;
	else if( !(chr&CHR_QING_QI_DUI).IsEmpty() )
		chr = CHR_QING_QI_DUI;
	else if( !(chr&CHR_LONG_QI_DUI).IsEmpty() )
		chr = CHR_LONG_QI_DUI;
	else if( !(chr&CHR_JIANG_DUI).IsEmpty() )
		chr = CHR_JIANG_DUI;
	else if( !(chr&CHR_QING_DUI).IsEmpty() )
		chr = CHR_QING_DUI;
	else if( !(chr&CHR_QI_XIAO_DUI).IsEmpty() )
		chr = CHR_QI_XIAO_DUI;
	else if( !(chr&CHR_DAI_YAO).IsEmpty() )
		chr = CHR_DAI_YAO;
	else if( !(chr&CHR_QING_YI_SE).IsEmpty() )
		chr = CHR_QING_YI_SE;
	else if( !(chr&CHR_DA_DUI_ZI).IsEmpty() )
		chr = CHR_DA_DUI_ZI;

	//附加权位
	//杠上花
	if( m_wCurrentUser==wChairId && m_bGangStatus )
		chr |= CHR_GANG_KAI;
	//杠上炮
	if( m_bGangOutStatus && !m_bGangStatus )
		chr |= CHR_GANG_SHANG_PAO;
}

//////////////////////////////////////////////////////////////////////////
