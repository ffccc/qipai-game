#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//游戏人数
//const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//开始模式
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_AllReady;	//开始模式
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{
	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	//游戏变量
	for(WORD i=0 ; i<GAME_PLAYER ; ++i) m_nChip[i] = -1 ;
	ZeroMemory(m_bUserCardData , sizeof(m_bUserCardData)) ;
	ZeroMemory(m_bSegmentCard , sizeof(m_bSegmentCard)) ;
	ZeroMemory(m_bFinishSegment , sizeof(m_bFinishSegment)) ;
	m_wBanker = INVALID_CHAIR ;
	m_wFirstCallBankerUser = INVALID_CHAIR ;
	m_bShowCardCount=0 ; 
	m_bCompleteCompareCount=0 ;
	m_bShowCardTime=3 ;
	m_bSetChipCount=0 ;
	m_lMaxChip = -1 ;
	for(BYTE i=0; i<GAME_PLAYER; ++i) m_bBaoPai[i] = false ;


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
	for(WORD i=0 ; i<GAME_PLAYER ; ++i) m_nChip[i] = -1 ;
	ZeroMemory(m_bUserCardData , sizeof(m_bUserCardData)) ;
	ZeroMemory(m_bSegmentCard , sizeof(m_bSegmentCard)) ;
	ZeroMemory(m_bFinishSegment , sizeof(m_bFinishSegment)) ;
	m_bShowCardCount=0 ; 
	m_bCompleteCompareCount=0 ;
	m_bShowCardTime=3 ;
	m_bSetChipCount=0 ;
	for(BYTE i=0; i<GAME_PLAYER; ++i) m_bBaoPai[i] = false ;

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
	m_pITableFrame->SetGameStatus(GS_WK_CALL_BANKER);

	//游戏变量
	for(WORD i=0 ; i<GAME_PLAYER ; ++i) m_nChip[i] = -1 ;
	ZeroMemory(m_bUserCardData , sizeof(m_bUserCardData)) ;
	ZeroMemory(m_bSegmentCard , sizeof(m_bSegmentCard)) ;
	ZeroMemory(m_bFinishSegment , sizeof(m_bFinishSegment)) ;
	m_bShowCardCount=0 ; 
	m_bCompleteCompareCount=0 ;
	m_bShowCardCount=0 ;
	m_bSetChipCount=0;
	for(BYTE i=0; i<GAME_PLAYER; ++i) m_bBaoPai[i] = false ;

	CMD_S_StartCallBanker  StartCallBanker ;
	memset( &StartCallBanker , 0 , sizeof(StartCallBanker)) ;

	//首次叫庄
	if ( m_wFirstCallBankerUser == INVALID_CHAIR )
	{
		//在座玩家
		WORD wSitDownUser[GAME_PLAYER] ;
		BYTE cbSitDownUsercount = 0 ;
		for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
		{
			IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem( wChairID ) ;
			if ( pServerUserItem ) wSitDownUser[cbSitDownUsercount++] = wChairID ;
		}

		//叫庄玩家
		BYTE cbCallerIndex = rand() % cbSitDownUsercount;

		m_wFirstCallBankerUser = wSitDownUser[cbCallerIndex] ;
	}
	else
	{
		//轮换庄家
		while ( true )
		{
			m_wFirstCallBankerUser = ( m_wFirstCallBankerUser + 1 ) % GAME_PLAYER ;
			if ( m_pITableFrame->GetServerUserItem( m_wFirstCallBankerUser ) != NULL ) break ;
		}
	}

	StartCallBanker.wCurrentCaller = m_wFirstCallBankerUser ;
	m_wCurrentCaller = m_wFirstCallBankerUser ;

	//发送消息
	m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_START_CALL_BANKER, &StartCallBanker, sizeof( StartCallBanker ) ) ;
	m_pITableFrame->SendLookonData( INVALID_CHAIR, SUB_S_START_CALL_BANKER, &StartCallBanker, sizeof( StartCallBanker ) ) ;

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

			GameEnd.bEndMode = GER_DISMISS ;

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//结束游戏
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_NORMAL:		//常规结束
		{

			//构造数据
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			GameEnd.bEndMode = GER_NORMAL ;

			bool bBankerDragon = false , //庄家乌龙
				 bUserDragon   = false ; //闲家乌龙

			if((true==m_GameLogic.CompareCard(m_bSegmentCard[m_wBanker][0] , m_bSegmentCard[m_wBanker][1] , 3 , 5 , false)) &&
			   (true==m_GameLogic.CompareCard(m_bSegmentCard[m_wBanker][1] , m_bSegmentCard[m_wBanker][2] , 5 , 5 , false)))
			   bBankerDragon = false ;
			else
				bBankerDragon = true ;

			//玩家个数
			BYTE cbUsercount = 0 ;
			for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
			{
				IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem( wChairID ) ;
				if ( pServerUserItem ) cbUsercount++ ;
			}

			for(WORD i=0 ; i<GAME_PLAYER ; ++i)
			{
				BYTE bWindCount=0 ;
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(i);
				if(pIServerUserItem)
				{
					if(i!=m_wBanker)
					{

						if(true==m_GameLogic.CompareCard(m_bSegmentCard[m_wBanker][0] , m_bSegmentCard[i][0] , 3 , 3 , true)) ++bWindCount ;
						if(true==m_GameLogic.CompareCard(m_bSegmentCard[m_wBanker][1] , m_bSegmentCard[i][1] , 5 , 5 , true)) ++bWindCount ;
						if(true==m_GameLogic.CompareCard(m_bSegmentCard[m_wBanker][2] , m_bSegmentCard[i][2] , 5 , 5 , true)) ++bWindCount ;

						if((true==m_GameLogic.CompareCard(m_bSegmentCard[i][0] , m_bSegmentCard[i][1] , 3 , 5 , false)) &&
							(true==m_GameLogic.CompareCard(m_bSegmentCard[i][1] , m_bSegmentCard[i][2] , 5 , 5 , false)))
							bUserDragon = false ;
						else
							bUserDragon = true ;

						//摆乌龙
						//if(true==bUserDragon && true==bBankerDragon)
						//{
						//	GameEnd.lGameScore[i] = 0 ;
						//	GameEnd.lGameScore[m_wBanker] = 0 ;
						//}
						if(true==bBankerDragon && !bUserDragon)
						{
							//三倍下注
							GameEnd.lGameScore[i] = 3*m_nChip[i] ;
							GameEnd.lGameScore[m_wBanker] -= 3*m_nChip[i] ;
						}

						if(true==bUserDragon && !bBankerDragon)
						{
							//赔给庄家
							GameEnd.lGameScore[i] = -3*m_nChip[i] ;
							GameEnd.lGameScore[m_wBanker] += 3*m_nChip[i] ;
						}

						//非乌龙
						if ( !bUserDragon && !bBankerDragon )
						{
							//闲家赢三倍
							if( bWindCount > 2 )
							{
								GameEnd.lGameScore[i] = 3 * m_nChip[i] ;
								GameEnd.lGameScore[m_wBanker] -= ( 3 * m_nChip[i] );
							}
							else if ( bWindCount == 2 )
							{
								GameEnd.lGameScore[i] = m_nChip[i] ;
								GameEnd.lGameScore[m_wBanker] -= ( m_nChip[i] );
							}
							//庄家赢三倍
							else if ( bWindCount == 0 )
							{
								GameEnd.lGameScore[i] = - ( 3 * m_nChip[i] );
								GameEnd.lGameScore[m_wBanker] += ( 3 * m_nChip[i] );
							}
							else if ( bWindCount == 1 )
							{
								GameEnd.lGameScore[i] = -m_nChip[i] ;
								GameEnd.lGameScore[m_wBanker] += m_nChip[i] ;
							}
						}

					}
				}
			}
			//计算税收
			if (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
			{
				LONG lTmpTax ;
				for (WORD i=0;i<GAME_PLAYER;i++)
				{
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(i);
					if(pIServerUserItem)
					{
						lTmpTax=0;
						if ( GameEnd.lGameScore[i] >= 100 )
						{
							float fTaxPer = m_pGameServiceOption->wRevenue/1000.; 
							lTmpTax   = LONG(GameEnd.lGameScore[i]*fTaxPer);
						}
						GameEnd.lGameTax	  +=lTmpTax;
						GameEnd.lGameScore[i] = GameEnd.lGameScore[i] - lTmpTax;

						//修改分数
						m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],lTmpTax,GameEnd.lGameScore[i]>0L?enScoreKind_Win:enScoreKind_Lost);
					}
				}
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

			//定义变量
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			LONG wUserToltalChip=(pIServerUserItem->GetUserData()->UserScoreInfo).lScore ;

			GameEnd.bEndMode = GER_USER_LEFT ;

			//统计积分
			if(wChairID!=m_wBanker)
			{
				if(-1!=m_nChip[wChairID])
				{
					GameEnd.lGameScore[wChairID] = - 3 * m_nChip[wChairID] ;
					GameEnd.lGameScore[m_wBanker] = 3 * m_nChip[wChairID] ;
				}
				else
				{
                    GameEnd.lGameScore[wChairID] = -min(3 * m_pGameServiceOption->lCellScore, wUserToltalChip ) ;
				}
			}
			else
			{
				BYTE bPlayerCount=0 ; 
				for(WORD wID=0 ; wID<GAME_PLAYER ; ++wID)
				{
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wID);
					if(pIServerUserItem) ++bPlayerCount ;

					if(pIServerUserItem && wID!=m_wBanker && -1!=m_nChip[wID])
					{
						GameEnd.lGameScore[m_wBanker] -= 3 * m_nChip[wID] ;
						GameEnd.lGameScore[wID] = 3 * m_nChip[wID] ;
					}
				}

				if( 0== GameEnd.lGameScore[m_wBanker] )
				{
					GameEnd.lGameScore[m_wBanker] = -min(3*m_pGameServiceOption->lCellScore,wUserToltalChip);
				}
			}

			//计算税收
			if (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
			{
				LONG lTmpTax;
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(i);
					if (pIServerUserItem==NULL) continue;
					lTmpTax=0;
					if (GameEnd.lGameScore[i]>=100L)
					{
						float fTaxPer = m_pGameServiceOption->wRevenue/1000.; 
						lTmpTax   = LONG(GameEnd.lGameScore[i]*fTaxPer);
					}
					GameEnd.lGameTax+=lTmpTax;
					GameEnd.lGameScore[i]=GameEnd.lGameScore[i]-lTmpTax;
					
					//修改分数
					m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],lTmpTax,GameEnd.lGameScore[i]>0L?enScoreKind_Win:enScoreKind_Lost);
				}
			}

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//结束游戏
			m_pITableFrame->ConcludeGame();

            return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//发送场景
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret)
{
	switch (bGameStatus)
	{
	case GS_WK_FREE:		//空闲状态
		{

			//发送场景
			return m_pITableFrame->SendGameScene( pIServerUserItem, NULL, 0 );
		}
	case GS_WK_SETCHIP:		//下注状态 
		{
			CMD_S_StatusSetChip StatusSetChip ;
			memset(&StatusSetChip , 0 , sizeof(StatusSetChip)) ;

			StatusSetChip.wMaxChip = m_lMaxChip ;
			StatusSetChip.wUserToltalChip=(pIServerUserItem->GetUserData()->UserScoreInfo).lScore ;


			StatusSetChip.wBanker	= m_wBanker ;
			CopyMemory(StatusSetChip.bHandCardData , m_bUserCardData[wChiarID] , HAND_CARD_COUNT) ;
			CopyMemory(StatusSetChip.nChip , m_nChip , sizeof(m_nChip)) ;

			//设置变量

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusSetChip,sizeof(StatusSetChip));
		}
	case GS_WK_PLAYING:		//游戏状态
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			StatusPlay.wMaxChip = m_lMaxChip ;
			StatusPlay.wUserToltalChip=(pIServerUserItem->GetUserData()->UserScoreInfo).lScore ;

			StatusPlay.wBanker	= m_wBanker ;
			CopyMemory(StatusPlay.bHandCardData , m_bUserCardData[wChiarID] , HAND_CARD_COUNT) ;
			CopyMemory(StatusPlay.bSegmentCard , m_bSegmentCard , sizeof(m_bSegmentCard)) ;
			CopyMemory(StatusPlay.bFinishSegment , m_bFinishSegment , sizeof(m_bFinishSegment)) ;
			CopyMemory(StatusPlay.nChip , m_nChip , sizeof(m_nChip)) ;

			//设置变量

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	case GS_WK_CALL_BANKER :	//叫庄状态
		{
			CMD_S_StatusCallBanker StatusCallBanker ;

			ZeroMemory( &StatusCallBanker, sizeof( StatusCallBanker ) ) ;

			StatusCallBanker.wCurrentCaller = m_wCurrentCaller ;

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusCallBanker,sizeof(StatusCallBanker));
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
	case SEB_C_SHOW_CARD_TIME:		//摊牌时间
		{
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();	
			return OnShowCardTime(pUserData->wChairID , pDataBuffer, wDataSize) ;
			return true ;
		}
	case SUB_C_SETCHIP:				//设置下注
		{
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();	
			return OnSetChip(pUserData->wChairID , pDataBuffer, wDataSize) ;
		}
	case SUB_C_SEGCARD:				//设置分段
		{
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();	
			return OnSetSegment(pUserData->wChairID , pDataBuffer, wDataSize) ;
		}
	case SUB_C_SHOWCARD:			//玩家摊牌
		{
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();	
			return OnShowCard(pUserData->wChairID , pDataBuffer, wDataSize) ;
		}
	case SUB_C_COMPLETE_COMPARE:	//完成比较
		{
			++m_bCompleteCompareCount;
			int nPlayerCount=0 ; 
			for(WORD i=0 ; i<GAME_PLAYER ; ++i)
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(i);
				if(pIServerUserItem) ++nPlayerCount ;
			}
			if(nPlayerCount==m_bCompleteCompareCount)
			{
				tagServerUserData * pUserData=pIServerUserItem->GetUserData();	
				return OnEventGameEnd(pUserData->wChairID,NULL,GER_NORMAL);
			}
			return true ;
		}
	case SUB_C_BAO_PAI:				//玩家包牌
		{
			m_bBaoPai[pIServerUserItem->GetChairID()] = true ;

			return true ;
		}
	case SUB_C_CALL_BANKER :		//玩家叫庄
		{
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();	

			//设置变量
			m_wFirstCallBankerUser = pUserData->wChairID ;
			m_wBanker = pUserData->wChairID ;

			CMD_S_CallBanker CallBanker ;
			ZeroMemory( &CallBanker, sizeof( CallBanker ) ) ;
			CallBanker.bCallBanker = true ;
			CallBanker.wCurrentCaller = INVALID_CHAIR ;
			CallBanker.wLastCaller = pUserData->wChairID ;

			//发送消息
			m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_CALL_BANKER, &CallBanker, sizeof( CallBanker ) ) ;
			m_pITableFrame->SendLookonData( INVALID_CHAIR, SUB_S_CALL_BANKER, &CallBanker, sizeof( CallBanker ) ) ;

			m_pITableFrame->SetGameStatus( GS_WK_SETCHIP) ;

			return true ;
		}
	case SUB_C_NO_CALL_BANKER :		//玩家不叫
		{
			tagServerUserData * pUserData = pIServerUserItem->GetUserData();	

			CMD_S_CallBanker CallBanker ;
			ZeroMemory( &CallBanker, sizeof( CallBanker ) ) ;
			CallBanker.bCallBanker = false ;
			CallBanker.wLastCaller = pUserData->wChairID ;

			//设置变量
			while ( true )
			{
				//轮换玩家
				m_wCurrentCaller = ( m_wCurrentCaller + 1 ) % GAME_PLAYER ;
				if ( m_pITableFrame->GetServerUserItem( m_wCurrentCaller ) != NULL || m_wFirstCallBankerUser == m_wCurrentCaller ) break ;
			}

			//结束判断
			if ( m_wCurrentCaller == m_wFirstCallBankerUser )
			{
				//都不做庄
				m_pITableFrame->ConcludeGame() ;

				//发送消息
				m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_END_ALL_NO_CALL_BANKER ) ;
				m_pITableFrame->SendLookonData( INVALID_CHAIR, SUB_S_END_ALL_NO_CALL_BANKER ) ;

				return true ;
			}

			CallBanker.wCurrentCaller = m_wCurrentCaller ;

			//发送消息
			m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_CALL_BANKER, &CallBanker, sizeof( CallBanker ) ) ;
			m_pITableFrame->SendLookonData( INVALID_CHAIR, SUB_S_CALL_BANKER, &CallBanker, sizeof( CallBanker ) ) ;

			return true ;
		}
	}
	
	return false;
}

//框架消息处理
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//设置下注
bool CTableFrameSink::OnSetChip(const WORD wChairID , const void * pDataBuffer, WORD wDataSize)
{
	//数据验证 
	ASSERT(INVALID_CHAIR!=wChairID) ;
	if(INVALID_CHAIR==wChairID) return false ;

	ASSERT(sizeof(CMD_C_SetChip)==wDataSize) ;
	if(sizeof(CMD_C_SetChip)!=wDataSize) return false ;

	CMD_C_SetChip *pSetChip = (CMD_C_SetChip*) pDataBuffer ;

	m_nChip[wChairID] = pSetChip->nChip ;

#ifdef _DEBUG
/*
	CString str ;
	str.Format("error:%ld" , m_nChip[wChairID]) ;
	if(m_wBanker==wChairID)
        AfxMessageBox(str) ;*/

#endif

	//发送数据
	CMD_S_SetChip SetChip ;
	memset(&SetChip , 0 , sizeof(SetChip)) ;
	SetChip.nChip = pSetChip->nChip ; 
	SetChip.wCurrentUser = wChairID ;


	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SETCHIP,&SetChip,sizeof(SetChip));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SETCHIP,&SetChip,sizeof(SetChip)); 


	//计算人数
	WORD wPlayerCount=0 ; 
	for(WORD wID=0 ; wID<GAME_PLAYER ; ++wID)
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wID);
		if(pIServerUserItem) ++wPlayerCount ;
	}

	++m_bSetChipCount ;

	if(m_bSetChipCount==(wPlayerCount-1))
	{
		//洗牌发牌
		BYTE bCardData[52] ;
		m_GameLogic.RandCardList(bCardData , 52) ;
		////////////////////////////////////////////////////////////

//		BYTE bCardData[52]={	
//			0x13,0x03,0x33,0x08,0x19,0x0c,0x1c,0x2c,0x1a,0x3a,0x05,0x32,0x22,
//0x21,0x11,0x2d,0x17,0x16,0x15,0x04,0x12,0x1d,0x1b,0x0b,0x19,0x09,
//			0x01,0x02,0x05,0x06,0x07,0x08,0x0A,0x0C,
//	0x12,0x13,0x14,0x18,	
//	0x21,0x23,0x24,0x28,0x29,0x2A,0x2C,	
//	0x32,0x33,0x34,0x36,0x37,0x39,0x3C,	
//		} ;
		//////////////////////////////////////////////////////////
		CMD_S_SendCard SendCard ;
		memset(&SendCard , 0 , sizeof(SendCard)) ;
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(m_wBanker);
		//		SendCard.wMaxChip = (pIServerUserItem->GetUserData()->UserScoreInfo).lScore/wPlayerCount ;

		//设置状态
		m_pITableFrame->SetGameStatus(GS_WK_PLAYING);

		//设置庄家
		SendCard.wBanker = m_wBanker ;

		//保存扑克
		for(WORD i=0 ; i<GAME_PLAYER ; ++i)
		{
			CopyMemory(m_bUserCardData[i] , bCardData+i*HAND_CARD_COUNT , HAND_CARD_COUNT) ;
			m_GameLogic.SortCardList(m_bUserCardData[i] , HAND_CARD_COUNT) ;
			CopyMemory(SendCard.bCardData , m_bUserCardData[i] , HAND_CARD_COUNT) ;
			SendCard.wCurrentUser = i ;

			IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(i);
			if(pIServerUserItem)
			{
				SendCard.wUserToltalChip = (pIServerUserItem->GetUserData()->UserScoreInfo).lScore ;
				m_pITableFrame->SendTableData(i,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
				m_pITableFrame->SendLookonData(i,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
			}
		}
	}

	return true ;
}

//设置分段
bool CTableFrameSink::OnSetSegment(const WORD wChairID , const void * pDataBuffer, WORD wDataSize) 
{
	//数据验证 
	ASSERT(INVALID_CHAIR!=wChairID) ;
	if(INVALID_CHAIR==wChairID) return false ;

	ASSERT(sizeof(CMD_C_SegCard)==wDataSize) ;
	if(sizeof(CMD_C_SegCard)!=wDataSize) return false ;

	CMD_C_SegCard *pSetSeg = (CMD_C_SegCard*) pDataBuffer ;

	//发送数据
	CMD_S_SetSegment SetSegment ;
	memset(&SetSegment , 0 , sizeof(SetSegment)) ;

	SetSegment.SegmentType = pSetSeg->SegmentType ; 
	SetSegment.wCurrentUser=wChairID ;

	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SETSEGMENT,&SetSegment,sizeof(SetSegment));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SETSEGMENT,&SetSegment,sizeof(SetSegment)); 

	return true ;
}

//玩家摊牌
bool CTableFrameSink::OnShowCard(const WORD wChairID , const void * pDataBuffer, WORD wDataSize)
{
	//数据验证 
	ASSERT(INVALID_CHAIR!=wChairID) ;
	if(INVALID_CHAIR==wChairID) return false ;

	ASSERT(sizeof(CMD_C_ShowCard)==wDataSize) ;
	if(sizeof(CMD_C_ShowCard)!=wDataSize) return false ;

	CMD_C_ShowCard *pSetSeg = (CMD_C_ShowCard*) pDataBuffer ;

	//设置数据
	m_bFinishSegment[wChairID] = true ;
	CopyMemory(m_bSegmentCard[wChairID][0] , pSetSeg->bFrontCard , 3) ;
	CopyMemory(m_bSegmentCard[wChairID][1] , pSetSeg->bMidCard , 5) ;
	CopyMemory(m_bSegmentCard[wChairID][2] , pSetSeg->bBackCard , 5) ;

	//发送数据
	CMD_S_ShowCard  ShowCard ;
	memset(&ShowCard , 0 , sizeof(ShowCard)) ;

	//计算人数
	WORD wPlayerCount=0 ; 
	for(WORD wID=0 ; wID<GAME_PLAYER ; ++wID)
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wID);
		if(pIServerUserItem) ++wPlayerCount ;
	}

	m_bShowCardCount++ ;

	for(WORD i=0 ; i<GAME_PLAYER ; ++i)
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(i);
		if(pIServerUserItem)
		{
			if(true==m_bFinishSegment[i])
			{
				for(WORD j=0 ; j<GAME_PLAYER ; ++j)
				{
					if(true==m_bFinishSegment[j])
					{
						if(i!=m_wBanker && m_bShowCardCount!=wPlayerCount && i!=j)
							ShowCard.bCanSeeShowCard = false ;
						else
						{
                            ShowCard.bCanSeeShowCard = true ;
						}
						ShowCard.wCurrentUser = j ;
						CopyMemory(ShowCard.bFrontCard , m_bSegmentCard[j][0] , 3) ;
						CopyMemory(ShowCard.bMidCard ,   m_bSegmentCard[j][1] , 5) ;
						CopyMemory(ShowCard.bBackCard ,  m_bSegmentCard[j][2] , 5) ;

						m_pITableFrame->SendTableData(i,SUB_S_SHOW_CARD,&ShowCard,sizeof(ShowCard));
						m_pITableFrame->SendLookonData(i,SUB_S_SHOW_CARD,&ShowCard,sizeof(ShowCard)); 
					}
				}
			}
			else
			{
				for(WORD j=0 ; j<GAME_PLAYER ; ++j)
				{
					if(true==m_bFinishSegment[j])
					{
						ShowCard.bCanSeeShowCard = false ;
						ShowCard.wCurrentUser = j ;

						m_pITableFrame->SendTableData(i,SUB_S_SHOW_CARD,&ShowCard,sizeof(ShowCard));
						m_pITableFrame->SendLookonData(i,SUB_S_SHOW_CARD,&ShowCard,sizeof(ShowCard)); 
					}
				}
			}
		}
	}

	int nPlayerCount=0 ; 
	for(WORD i=0 ; i<GAME_PLAYER ; ++i)
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(i);
		if(pIServerUserItem) ++nPlayerCount ;
	}

	if(nPlayerCount==m_bShowCardCount)
	{
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_COMPARE_CARD , NULL , 0);
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_COMPARE_CARD , NULL , 0); 

	}

	return true ;
}

//用户坐下
bool __cdecl CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	return true;
}

//用户起来
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if(false==bLookonUser)
	{
		m_wFirstCallBankerUser = INVALID_CHAIR ;
	}

	return true;
}


//摊牌时间
bool CTableFrameSink::OnShowCardTime(const WORD wChairID , const void * pDataBuffer, WORD wDataSize)
{
	//数据验证 
	ASSERT(INVALID_CHAIR!=wChairID) ;
	if(INVALID_CHAIR==wChairID) return false ;

	ASSERT(sizeof(CMD_C_ShowCardTime)==wDataSize) ;
	if(sizeof(CMD_C_ShowCardTime)!=wDataSize) return false ;

	if(wChairID!=m_wBanker) return true ;

	CMD_C_ShowCardTime *pShowCardTime = (CMD_C_ShowCardTime*) pDataBuffer ;
	
	CMD_S_ShowCardTime ShowCardTime ;
	memset(&ShowCardTime , 0 , sizeof(CMD_S_ShowCardTime)) ;
	ShowCardTime.bShowCardTime = pShowCardTime->bShowCardTime ;
	ShowCardTime.wBanker = m_wBanker ;
	ShowCardTime.wMaxChip = pShowCardTime->wMaxChip ;

	m_bShowCardTime = pShowCardTime->bShowCardTime ;
	m_lMaxChip = pShowCardTime->wMaxChip ;
    
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SHOW_CARD_TIME,&ShowCardTime,sizeof(ShowCardTime));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SHOW_CARD_TIME,&ShowCardTime,sizeof(ShowCardTime)); 

	return true ;
}
//////////////////////////////////////////////////////////////////////////

