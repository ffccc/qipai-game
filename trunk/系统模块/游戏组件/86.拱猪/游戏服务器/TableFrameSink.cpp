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
	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	//游戏变量
    m_wCurrentUser  = INVALID_CHAIR ;						 
	for(WORD i=0 ; i<GAME_PLAYER ; ++i)	m_wShowCardUser[i] = INVALID_CHAIR; 
	ZeroMemory(m_bShowCard , sizeof(m_bShowCard)) ;
	m_bShowCardUserCount = 0;
	m_bShowCardTime = 0;
	ZeroMemory(m_bScoreCard , sizeof(m_bScoreCard)) ;
	ZeroMemory(m_bScoreCardCount , sizeof(m_bScoreCardCount)) ;
	m_bTotalShowCardCount = 0 ;
	memset(m_bShowCardCount , 0 , sizeof(m_bShowCardCount)) ;

	//运行信息
	m_wTurnWiner  = INVALID_CHAIR;					 
	m_bTurnCardCount = 0 ;					 
	memset(m_bTurnCardData, 0 , GAME_PLAYER);	
	m_wFirstOutUser = INVALID_CHAIR;
	m_wGetGpadeUser = INVALID_CHAIR;

	//扑克信息
	memset(m_bCardCount , 0 , GAME_PLAYER);	
	ZeroMemory(m_bHandCardData , sizeof(m_bHandCardData)) ;
    
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
	m_wCurrentUser  = INVALID_CHAIR ;						 
	for(WORD i=0 ; i<GAME_PLAYER ; ++i)	m_wShowCardUser[i] = INVALID_CHAIR;  
	ZeroMemory(m_bShowCard , sizeof(m_bShowCard)) ;
	m_bShowCardUserCount = 0;
	m_bShowCardTime = 0;
	ZeroMemory(m_bScoreCard , sizeof(m_bScoreCard)) ;
	ZeroMemory(m_bScoreCardCount , sizeof(m_bScoreCardCount)) ;
	m_bTotalShowCardCount = 0 ;
	memset(m_bShowCardCount , 0 , sizeof(m_bShowCardCount)) ;

	//运行信息
	m_wTurnWiner  = INVALID_CHAIR;					 
	m_bTurnCardCount = 0 ;					 
	memset(m_bTurnCardData, 0 , GAME_PLAYER);	
	m_wFirstOutUser = INVALID_CHAIR;

	//扑克信息
	memset(m_bCardCount , 0 , GAME_PLAYER);	
	ZeroMemory(m_bHandCardData , sizeof(m_bHandCardData)) ;


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
	m_pITableFrame->SetGameStatus(GS_WK_SHOWCARD);

	//游戏变量
	m_wCurrentUser  = INVALID_CHAIR ;						 
	for(WORD i=0 ; i<GAME_PLAYER ; ++i)	m_wShowCardUser[i] = INVALID_CHAIR;  
	ZeroMemory(m_bShowCard , sizeof(m_bShowCard)) ;
	m_bShowCardUserCount = 0;
	memset(m_bShowCardCount , 0 , sizeof(m_bShowCardCount)) ;

	//运行信息
	m_wTurnWiner  = INVALID_CHAIR;					 
	m_bTurnCardCount = 0 ;					 
	memset(m_bTurnCardData, 0 , GAME_PLAYER);	
	m_wFirstOutUser = INVALID_CHAIR;

	//混乱扑克
	BYTE bRandCard[52];
	m_GameLogic.RandCardList(bRandCard,sizeof(bRandCard)/sizeof(bRandCard[0]));

	//分发扑克
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_bCardCount[i]=HAND_CARD_COUNT;
		CopyMemory(&m_bHandCardData[i],&bRandCard[i*m_bCardCount[i]],sizeof(BYTE)*m_bCardCount[i]);
		m_GameLogic.SortCardList(m_bHandCardData[i],m_bCardCount[i]);
	}
	//设置用户	

	//发送扑克
	CMD_S_SendCard SendCard;
	memset(&SendCard , 0 , sizeof(SendCard)) ;

	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		SendCard.wCurrentUser=i;
		CopyMemory(SendCard.bCardData,m_bHandCardData[i],sizeof(SendCard.bCardData));
        m_pITableFrame->SendTableData(i,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
		m_pITableFrame->SendLookonData(i,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	}

	//首先出牌者
	if(INVALID_CHAIR==m_wGetGpadeUser)
	{
		for (WORD i=0;i<m_wPlayerCount;i++)
			for(BYTE j=0 ; j<HAND_CARD_COUNT ; ++j)
			{
				BYTE bCardColor = m_GameLogic.GetCardColor(m_bHandCardData[i][j]) ;
				bCardColor = bCardColor>>4 ;
				BYTE bCardValue = m_GameLogic.GetCardValue(m_bHandCardData[i][j]) ;
				if(1==bCardColor && 2==bCardValue) 
				{
					m_wCurrentUser = i ;
					break ;
				}
			}
	}
	else  
		m_wCurrentUser = m_wGetGpadeUser ;

	return true;
}

//游戏结束
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_DISMISS:		//游戏解散
		{
			m_wGetGpadeUser = INVALID_CHAIR;

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

			//计算积分
			bool    bShowSpade   = false ,    //亮黑桃Q
				    bShowHearts  = false ,    //亮红桃A
					bShowClubs   = false ,    //亮梅花10
					bShowDiamonds= false ;    //亮方块J
			BYTE    bCardColor ;
			BYTE    bCardValue ;

			//判断亮牌
			for(BYTE i=0 ; i<GAME_PLAYER ; ++i)
			{
				for(BYTE j=0 ; j<4 ; ++j)
				{
					if(0!=m_bShowCard[i][j])
					{
						bCardColor = m_GameLogic.GetCardColor(m_bShowCard[i][j]) ;
						bCardColor = bCardColor>>4 ;
						bCardValue = m_GameLogic.GetCardValue(m_bShowCard[i][j]) ;
						//判断扑克
                        
						//黑桃Q
						if(3==bCardColor && 12==bCardValue)  bShowSpade         = true ;
						//红桃A
						else if(2==bCardColor && 1==bCardValue)   bShowHearts   = true ;
						//方块J
						else if(0==bCardColor && 11==bCardValue)  bShowDiamonds = true ;
                        //梅花10
						else if(1==bCardColor && 10==bCardValue)  bShowClubs    = true ; 
					}
				}
			}

			//扑克分数
			int  bScore[16]={0,0,0,-10,-10,-10,-10,-10,-10,-20,-30,-40,-50,100,-100,50} ;
			BYTE bIndex ;

			//红桃翻倍
			if(true==bShowHearts) for(BYTE i=0 ; i<13 ; ++i) bScore[i] *=2 ;
			//猪牌翻倍
			if(true==bShowSpade)    bScore[14] *= 2 ;
			//羊牌翻倍
			if(true==bShowDiamonds) bScore[13] *= 2 ;

			bool  bGetClubs    = false  ,     //得变压器
				  bGetHearts   = false  ,     //得红桃A
				  bGetSpade    = false ,      //得黑桃Q
				  bGetDiamonds = false ;      //得方块J

			LONG  lCellScore=m_pGameServiceOption->lCellScore;

			//计算积分
			for(BYTE i=0 ; i<GAME_PLAYER ; ++i)
			{
				bGetClubs    = false ;
				bGetHearts   = false ;
				bGetSpade    = false ;
				bGetDiamonds = false ;      

				//没有分牌
				if(0==m_bScoreCardCount[i]) continue ;

				for(BYTE j=0 ; j<m_bScoreCardCount[i] ; ++j)
				{
					//数据检验
					ASSERT(0!=m_bScoreCard[i][j]) ;
					if(0==m_bScoreCard[i][j]) return false ;
					bCardColor = m_GameLogic.GetCardColor(m_bScoreCard[i][j]) ;
					bCardColor = bCardColor>>4 ;
					bCardValue = m_GameLogic.GetCardValue(m_bScoreCard[i][j]) ;
					//数据检验
					ASSERT(2==bCardColor || (0==bCardColor && 11==bCardValue) || (1==bCardColor && 10==bCardValue) || (3==bCardColor && 12==bCardValue));   

					bIndex = bCardValue-2 ;
					//黑桃Q
					if(3==bCardColor && 12==bCardValue) 
					{
						bGetSpade   = true ;
						bIndex      = 14 ; 
					}
					//红桃A
					if(2==bCardColor && 1==bCardValue) 
					{
						bGetHearts   = true ;
						bIndex       = 12 ; 
					}
					//方块J
					if(0==bCardColor && 11==bCardValue)
					{
						bGetDiamonds = true ;
						bIndex       = 13 ; 
					}
					//梅花10
					if(1==bCardColor && 10==bCardValue)
					{
						bGetClubs    = true ; 
						bIndex       = 15 ; 
					}
					//非梅花10
					if(1!=bCardColor) GameEnd.lGameScore[i]  += bScore[bIndex] ;
				}	

				//调整得分

				//处理变压
				if(1==m_bScoreCardCount[i]      && true==bShowClubs  && true==bGetClubs)  GameEnd.lGameScore[i] = 100 ;
				else if(1==m_bScoreCardCount[i] && false==bShowClubs && true==bGetClubs)  GameEnd.lGameScore[i] = 50  ;
				else if(m_bScoreCardCount[i]>1  && false==bShowClubs && true==bGetClubs)  GameEnd.lGameScore[i] *= 2  ;
				else if(m_bScoreCardCount[i]>1  && true==bShowClubs  && true==bGetClubs)  GameEnd.lGameScore[i] *= 4  ;

				//满红得分
				if((13==m_bScoreCardCount[i]) && (false==bGetClubs) && (false==bGetDiamonds) && (false==bGetSpade))
				{
					if(true==bShowHearts) GameEnd.lGameScore[i] = 400 ;
					else                  GameEnd.lGameScore[i] = 200 ;
				}
				//满贯得分
				if(16==m_bScoreCardCount[i])
				{
					//玩家亮牌
					if(false!=bShowSpade || false!=bShowHearts || false!=bShowDiamonds || false!=bShowClubs) 
					{
						GameEnd.lGameScore[i] = 0 ;
						for(BYTE k=0 ; k<15 ; ++k) GameEnd.lGameScore[i] += abs(bScore[k]) ;
						//变压器
						if(true==bShowClubs) GameEnd.lGameScore[i] *= 4 ;
						else                 GameEnd.lGameScore[i] *= 2 ;
					}
					else//没有亮牌 
						GameEnd.lGameScore[i] = 800 ;
					
				}
			}

			//平衡积分
			LONG wTempScore[GAME_PLAYER] ;
			memset(wTempScore , 0 , sizeof(wTempScore)) ;
			wTempScore[0] = GameEnd.lGameScore[0]-(GameEnd.lGameScore[1]+GameEnd.lGameScore[2]+GameEnd.lGameScore[3])/3 ;
			wTempScore[1] = GameEnd.lGameScore[1]-(GameEnd.lGameScore[0]+GameEnd.lGameScore[2]+GameEnd.lGameScore[3])/3 ;
			wTempScore[2] = GameEnd.lGameScore[2]-(GameEnd.lGameScore[0]+GameEnd.lGameScore[1]+GameEnd.lGameScore[3])/3 ;
			wTempScore[3] = GameEnd.lGameScore[3]-(GameEnd.lGameScore[0]+GameEnd.lGameScore[1]+GameEnd.lGameScore[2])/3 ;
			
			 //平衡处理
			if(0!=(wTempScore[0]+wTempScore[1]+wTempScore[2]+wTempScore[3]))
			{
				if((wTempScore[0]+wTempScore[1]+wTempScore[2]+wTempScore[3])<0)
				{
					for(BYTE i=0 ; i<GAME_PLAYER ;)
					{
						if(wTempScore[i]<0) 
						{
							wTempScore[i] += 1 ;
							if(0==(wTempScore[0]+wTempScore[1]+wTempScore[2]+wTempScore[3])) break; 
						}
						++i ;
						if((0!=(wTempScore[0]+wTempScore[1]+wTempScore[2]+wTempScore[3])) && i==GAME_PLAYER) i=0 ;
					}
				}
				else
				{
					for(BYTE i=0 ; i<GAME_PLAYER ;)
					{
						if(wTempScore[i]>0) 
						{
							wTempScore[i] -= 1 ;
							if(0==(wTempScore[0]+wTempScore[1]+wTempScore[2]+wTempScore[3])) break; 
						}
						++i ;
						if((0!=(wTempScore[0]+wTempScore[1]+wTempScore[2]+wTempScore[3])) && i==GAME_PLAYER) i=0 ;
					}
				}
			}

			for(BYTE i=0 ; i<GAME_PLAYER ; ++i)
			{
				GameEnd.lGameScore[i] = wTempScore[i] ;
				GameEnd.lGameScore[i] *= lCellScore ;	
			}


			//计算税收
			if (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
			{
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if (GameEnd.lGameScore[i]>=100L)
					{
						GameEnd.lGameTax+=GameEnd.lGameScore[i]/10L;
						GameEnd.lGameScore[i]=GameEnd.lGameScore[i]*45L/50L;
					}
				}
			}		

			TRACE("下面是玩家积分:\n") ;
			for(BYTE i=0 ; i<GAME_PLAYER ; ++i)
			{
				CString str ;
				str.Format("%ld\n" , GameEnd.lGameScore[i]) ;
				TRACE(str);
			}
			TRACE("积分结束\n");
			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//修改积分
			tagScoreInfo ScoreInfo[m_wPlayerCount];
			memset(&ScoreInfo,0,sizeof(ScoreInfo));
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				ScoreInfo[i].lScore=GameEnd.lGameScore[i];
				ScoreInfo[i].ScoreKind=(GameEnd.lGameScore[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
			}
//			m_pITableFrame->WriteTableScore(ScoreInfo,m_wPlayerCount,GameEnd.lGameTax);

			//结束游戏
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_USER_LEFT:		//用户强退
		{
			m_wGetGpadeUser = INVALID_CHAIR;

			//定义变量
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			//统计积分
			GameEnd.lGameScore[wChairID]=-100L*m_pGameServiceOption->lCellScore;

			//计算税收
			if (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
			{
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if (GameEnd.lGameScore[i]>=100L)
					{
						GameEnd.lGameTax+=GameEnd.lGameScore[i]/10L;
						GameEnd.lGameScore[i]=GameEnd.lGameScore[i]*45L/50L;
					}
				}
			}

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//修改积分
			tagScoreInfo ScoreInfo;
			ScoreInfo.ScoreKind=enScoreKind_Draw;
			ScoreInfo.lScore=GameEnd.lGameScore[wChairID];
			m_pITableFrame->WriteUserScore(wChairID,ScoreInfo.lScore ,ScoreInfo.lRevenue ,ScoreInfo.ScoreKind );

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
			//构造数据
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_WK_SHOWCARD:	//玩家亮牌
		{
			CMD_S_StatusShowCard StatusShowCard ;
			memset(&StatusShowCard , 0 , sizeof(StatusShowCard)) ;
			StatusShowCard.wCurrentUser = wChiarID ;
			CopyMemory(StatusShowCard.ShowCard , m_bShowCard , sizeof(m_bShowCard)) ;
			CopyMemory(StatusShowCard.ShowCardUser , m_wShowCardUser , sizeof(m_wShowCardUser)) ;
			CopyMemory(StatusShowCard.bCardData , m_bHandCardData[wChiarID] , sizeof(m_bHandCardData[wChiarID])) ;
			CopyMemory(StatusShowCard.ShowCardCount , m_bShowCardCount , sizeof(m_bShowCardCount)) ;

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusShowCard,sizeof(StatusShowCard));
		}
	case GS_WK_PLAYING:		//游戏状态
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//设置变量
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.wLastOutUser=m_wTurnWiner;
			StatusPlay.wFirstOutUser = m_wFirstOutUser ;

			CopyMemory(StatusPlay.ScoreCard , m_bScoreCard , sizeof(m_bScoreCard)) ;
			CopyMemory(StatusPlay.ShowCard , m_bShowCard , sizeof(m_bShowCard)) ;
			CopyMemory(StatusPlay.ShowCardUser , m_wShowCardUser , sizeof(m_wShowCardUser)) ;
			CopyMemory(StatusPlay.bCardCount,m_bCardCount,sizeof(m_bCardCount));
			CopyMemory(StatusPlay.bTurnCardData,m_bTurnCardData,sizeof(BYTE)*sizeof(StatusPlay.bTurnCardData));
			CopyMemory(StatusPlay.bCardData,m_bHandCardData[wChiarID],sizeof(BYTE)*m_bCardCount[wChiarID]);
			CopyMemory(StatusPlay.bShowCardCount , m_bShowCardCount , sizeof(m_bShowCardCount)) ;
			CopyMemory(StatusPlay.bScoreCardCount , m_bScoreCardCount , sizeof(m_bScoreCardCount)) ;

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	return false;
}
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_SHOW_CART:       //玩家亮牌
		{
			CMD_C_ShowCard *pShowCard = (CMD_C_ShowCard*)pDataBuffer ;
			
			//数据校验
			ASSERT(wDataSize==sizeof(CMD_C_ShowCard)) ;
			if(wDataSize!=sizeof(CMD_C_ShowCard)) return false ;
            
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();		

			++m_bShowCardTime;

			//没有亮牌
			if(0==pShowCard->bShowCardCount) 
			{
				//游戏开始 
				if(GAME_PLAYER==m_bShowCardTime)
				{
					//亮第四张
					if(3==m_bTotalShowCardCount)
					{
						BYTE bCardColor ;
						BYTE bCardValue ;
						bool bHaveShow=false ;

						for(BYTE i=0 ; i<GAME_PLAYER ; ++i)
							for(BYTE j=0 ; j<HAND_CARD_COUNT ; ++j) 
							{
								bCardColor = m_GameLogic.GetCardColor(m_bHandCardData[i][j]) ;
								bCardColor = bCardColor>>4 ;
								bCardValue = m_GameLogic.GetCardValue(m_bHandCardData[i][j]) ;
								//判断扑克
								if((3==bCardColor && 12==bCardValue)  || //黑桃Q
									(2==bCardColor && 1==bCardValue)  || //红桃A
									(0==bCardColor && 11==bCardValue) || //方块J
									(1==bCardColor && 10==bCardValue))   //梅花10
								{
									bHaveShow = false ;
									for(BYTE p=0 ; p<GAME_PLAYER ; ++p)
										for(BYTE c=0 ; c<4 ; ++c)
										{
											if(m_bShowCard[p][c]==m_bHandCardData[i][j]) bHaveShow=true ;
										}
										if(false==bHaveShow)
										{
											m_wShowCardUser[i] = i ; 
											for(BYTE k=0 ; k<4 ; ++k)  if(0==m_bShowCard[i][k]) break ;
											m_bShowCard[i][k] = m_bHandCardData[i][j] ;

											//亮牌数据
											CMD_S_ShowCard ShowCard ;
											memset(&ShowCard , 0 , sizeof(ShowCard)) ;
											ShowCard.wShowCardUser = i ;
											ShowCard.bShowCardCount = k+1 ;
											CopyMemory(ShowCard.bShowCard , m_bShowCard[i] , ShowCard.bShowCardCount) ;

											m_bShowCardCount[i] = ShowCard.bShowCardCount ;

											m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SHOW_CARD,&ShowCard,sizeof(ShowCard));
											m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SHOW_CARD,&ShowCard,sizeof(ShowCard));
											break ;
										}
								}
							}
					}

					CMD_S_GameStart GameStart ;
					memset(&GameStart , 0 , sizeof(GameStart)) ;
					GameStart.wCurrentUser = m_wCurrentUser ;
					GameStart.bFirstGame   = (INVALID_CHAIR==m_wGetGpadeUser) ;
					m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
					m_pITableFrame->SetGameStatus(GS_WK_PLAYING);
				}
				return true ;
			}
			
			//记录亮牌
			m_wShowCardUser[m_bShowCardUserCount++] = pUserData->wChairID ;
			CopyMemory(m_bShowCard[pUserData->wChairID] , pShowCard->bShowCard , pShowCard->bShowCardCount*sizeof(BYTE)) ;

			return OnUserShowCard(pUserData->wChairID , pShowCard->bShowCard , pShowCard->bShowCardCount) ;
		}
	case SUB_C_OUT_CART:		//用户出牌
		{
			//变量定义
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			WORD wHeadSize=sizeof(CMD_C_OutCard)-sizeof(pOutCard->bCardData);

			//效验数据
			ASSERT(wDataSize>=wHeadSize);
			if (wDataSize<wHeadSize) 
			{
				return false;
			}

			ASSERT(wDataSize==(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData)));
			if (wDataSize!=(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData))) 
			{
				return false;
			}


			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//消息处理
		
			return OnUserOutCard(pUserData->wChairID,pOutCard->bCardData,1);
			//////////////////////////////////////////////////////////////////////////

		}
	}

	return true;
}

//玩家亮牌
bool CTableFrameSink::OnUserShowCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount)
{
	//亮牌数据
	CMD_S_ShowCard ShowCard ;
	memset(&ShowCard , 0 , sizeof(ShowCard)) ;
	ShowCard.wShowCardUser = wChairID ;
	ShowCard.bShowCardCount = bCardCount ;
	CopyMemory(ShowCard.bShowCard , bCardData , bCardCount) ;

	//保留亮牌
	m_wShowCardUser[wChairID] = wChairID ;      
	CopyMemory(m_bShowCard[wChairID] , bCardData , bCardCount) ;
	m_bTotalShowCardCount += bCardCount ;

	m_bShowCardCount[wChairID] = bCardCount ;

	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SHOW_CARD,&ShowCard,sizeof(ShowCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SHOW_CARD,&ShowCard,sizeof(ShowCard));
    

	//游戏开始 
	if(GAME_PLAYER==m_bShowCardTime)
	{
		//亮第四张
	    if(3==m_bTotalShowCardCount)
		{
			BYTE bCardColor ;
			BYTE bCardValue ;
			bool bHaveShow=false ;

			for(BYTE i=0 ; i<GAME_PLAYER ; ++i)
				for(BYTE j=0 ; j<HAND_CARD_COUNT ; ++j) 
				{
                    bCardColor = m_GameLogic.GetCardColor(m_bHandCardData[i][j]) ;
					bCardColor = bCardColor>>4 ;
					bCardValue = m_GameLogic.GetCardValue(m_bHandCardData[i][j]) ;
					//判断扑克
					if((3==bCardColor && 12==bCardValue)  || //黑桃Q
						(2==bCardColor && 1==bCardValue)  || //红桃A
						(0==bCardColor && 11==bCardValue) || //方块J
						(1==bCardColor && 10==bCardValue))   //梅花10
					{
						bHaveShow = false ;
						for(BYTE p=0 ; p<GAME_PLAYER ; ++p)
							for(BYTE c=0 ; c<4 ; ++c)
							{
								if(m_bShowCard[p][c]==m_bHandCardData[i][j]) bHaveShow=true ;
							}
						if(false==bHaveShow)
						{
							m_wShowCardUser[i] = i ; 
							for(BYTE k=0 ; k<4 ; ++k)  if(0==m_bShowCard[i][k]) break ;
                            m_bShowCard[i][k] = m_bHandCardData[i][j] ;

							//亮牌数据
							CMD_S_ShowCard ShowCard ;
							memset(&ShowCard , 0 , sizeof(ShowCard)) ;
							ShowCard.wShowCardUser = i ;
							ShowCard.bShowCardCount = k+1 ;
							CopyMemory(ShowCard.bShowCard , m_bShowCard[i] , ShowCard.bShowCardCount) ;

							m_bShowCardCount[i] = ShowCard.bShowCardCount ;

							m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SHOW_CARD,&ShowCard,sizeof(ShowCard));
							m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SHOW_CARD,&ShowCard,sizeof(ShowCard));

							break ;
						}
					}
				}				
		}
		CMD_S_GameStart GameStart ;
		memset(&GameStart , 0 , sizeof(GameStart)) ;
		GameStart.wCurrentUser = m_wCurrentUser ;
		GameStart.bFirstGame   = (INVALID_CHAIR==m_wGetGpadeUser) ;
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SetGameStatus(GS_WK_PLAYING);
	}
    return true ;
}


//用户出牌
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE bCardData, BYTE bCardCount)
{
	//效验状态
	if (m_pITableFrame->GetGameStatus()!=GS_WK_PLAYING) return true;
	if (wChairID!=m_wCurrentUser) return false;

	//删除扑克
	if (m_GameLogic.RemoveCard(&bCardData,bCardCount,m_bHandCardData[wChairID],m_bCardCount[wChairID])==false) return false;
	m_bCardCount[wChairID]-=bCardCount;	

	//更改出牌
	m_bTurnCardCount++ ;
	if(1==m_bTurnCardCount) m_wFirstOutUser = wChairID ;
	m_bTurnCardData[wChairID] = bCardData ;
	if(GAME_PLAYER==m_bTurnCardCount) 
	{
		m_bTurnCardCount = 0 ;

		//最大出牌
		WORD wMostUser = m_wFirstOutUser ;
		BYTE bFirstCardColor = m_GameLogic.GetCardColor(m_bTurnCardData[m_wFirstOutUser]) ;
        bFirstCardColor = bFirstCardColor >> 4 ;
		BYTE bMostCardValue  = m_GameLogic.GetCardValue(m_bTurnCardData[m_wFirstOutUser]) ;
		bool bFoudGpade = false ;

		if(1==bMostCardValue) bMostCardValue += 13 ;
		for(WORD i=0 ; i<GAME_PLAYER ; ++i)
		{
			BYTE bCurrentCarColor  = m_GameLogic.GetCardColor(m_bTurnCardData[i]) ;
            bCurrentCarColor = bCurrentCarColor >> 4 ;
			BYTE bCurrentCardValue = m_GameLogic.GetCardValue(m_bTurnCardData[i]) ;
			if(1==bCurrentCardValue) bCurrentCardValue += 13 ;
			if(bCurrentCarColor == bFirstCardColor)
				if(bMostCardValue < bCurrentCardValue)
				{
					bMostCardValue = bCurrentCardValue ;
					wMostUser = i ;
				}
			//得到黑桃Q
			if(3==bCurrentCarColor && 12==bCurrentCardValue) bFoudGpade=true ;
		}
		//得到黑桃Q
		if(true==bFoudGpade) m_wGetGpadeUser = wMostUser ;

		//获取分数
		BYTE bScoreCard[GAME_PLAYER] ;
		BYTE bScoreCount=0 ;
        if(true==m_GameLogic.GetScoreCard(m_bTurnCardData , bScoreCard , GAME_PLAYER , bScoreCount))
		{
			CopyMemory(&m_bScoreCard[wMostUser][m_bScoreCardCount[wMostUser]] , bScoreCard , bScoreCount) ;
            m_bScoreCardCount[wMostUser] += bScoreCount ;
		}

        m_wTurnWiner = wMostUser ;
		if (m_bCardCount[wChairID]!=0)
		{
			m_wCurrentUser = wMostUser ;
		}
		else m_wCurrentUser=INVALID_CHAIR;	
		m_wFirstOutUser = wMostUser;	
		memset(m_bTurnCardData , 0 , sizeof(m_bTurnCardData)) ;
	}	
	else
	{
		m_wTurnWiner=wChairID;
		m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
		if(0==m_bCardCount[m_wCurrentUser])	m_wCurrentUser=INVALID_CHAIR;
	}
	//切换用户
	
	//构造数据
	CMD_S_OutCard OutCard;
	OutCard.bCardCount=bCardCount;
	OutCard.wOutCardUser=wChairID;
	OutCard.wCurrentUser=m_wCurrentUser;
	WORD wSendSize;
	CopyMemory(&OutCard.bCardData,&bCardData,bCardCount*sizeof(BYTE));
	wSendSize=sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE);

	//发送数据

	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);

	//判断结束游戏
	
	if (m_wCurrentUser==INVALID_CHAIR) 
	{
		OnEventGameEnd(wChairID,NULL,GER_NORMAL);
	}

	return true;
}
//定时器事件
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	return false;
}

//游戏消息处理


//框架消息处理
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//////////////////////////////////////////////////////////////////////////
