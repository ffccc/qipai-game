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
	//逻辑变量
	m_cbPackCount=2;
	m_cbMainValue=0x01;
	m_cbMainColor=COLOR_ERROR;
	m_bNotCall=false;
	m_bIsGanGuoBa=false;
	m_bIsRuanGanGuoBa=false;
	m_bIsYingGanGuoBa=false;

	//连局信息
	m_cbValueOrder[0]=0x01;
	m_cbValueOrder[1]=0x01;

	//游戏变量
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//叫牌信息
	m_cbCallCard=0;
	m_cbCallCount=0;
	m_wCallCardUser=INVALID_CHAIR;

	//状态变量
	m_cbScoreCardCount=0;
	ZeroMemory(m_bCallCard,sizeof(m_bCallCard));
	ZeroMemory(m_bKouFlag,sizeof(m_bKouFlag));
	ZeroMemory(m_cbScoreCardData,sizeof(m_cbScoreCardData));

	//出牌变量
	m_wTurnWinner=INVALID_CHAIR;
	m_wFirstOutUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));

	//底牌扑克
	m_cbConcealCount=0;
	ZeroMemory(m_cbConcealCard,sizeof(m_cbConcealCard));

	//用户扑克
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

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
	//逻辑变量
	m_cbPackCount=2;
	m_cbMainColor=COLOR_ERROR;
	m_bNotCall=false;
	m_bIsGanGuoBa=false;
	m_bIsRuanGanGuoBa=false;
	m_bIsYingGanGuoBa=false;

	//游戏变量
	m_wCurrentUser=INVALID_CHAIR;

	//叫牌信息
	m_cbCallCard=0;
	m_cbCallCount=0;
	m_wCallCardUser=INVALID_CHAIR;

	//状态变量
	m_cbScoreCardCount=0;
	ZeroMemory(m_bCallCard,sizeof(m_bCallCard));
	ZeroMemory(m_bKouFlag,sizeof(m_bKouFlag));
	ZeroMemory(m_cbScoreCardData,sizeof(m_cbScoreCardData));

	//出牌变量
	m_wTurnWinner=INVALID_CHAIR;
	m_wFirstOutUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));

	//底牌扑克
	m_cbConcealCount=0;
	ZeroMemory(m_cbConcealCard,sizeof(m_cbConcealCard));

	//用户扑克
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

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
	m_pITableFrame->SetGameStatus(GS_UG_CALL);

	//设置变量
	m_cbMainColor=COLOR_ERROR;
	m_bNotCall=false;
	m_bIsGanGuoBa=false;
	m_bIsRuanGanGuoBa=false;
	m_bIsYingGanGuoBa=false;

	//设置属性
	m_GameLogic.SetMainColor(m_cbMainColor);
	m_GameLogic.SetMainValue(m_cbMainValue);
	m_GameLogic.SetPackCount(m_cbPackCount);

	//构造数据
	CMD_S_GameStart GameStart;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.cbPackCount=m_cbPackCount;
	GameStart.cbMainValue=m_cbMainValue;
	GameStart.cbValueOrder[0]=m_cbValueOrder[0];
	GameStart.cbValueOrder[1]=m_cbValueOrder[1];

	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));

	//发送扑克
	DispatchUserCard();

	return true;
}

//游戏结束
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_DISMISS:		//游戏解散
		{
			//定义变量
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//底牌信息
			GameEnd.cbConcealCount=m_cbConcealCount;
			CopyMemory(GameEnd.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);

			//游戏得分
			GameEnd.wConcealScore=m_GameLogic.GetCardScore(m_cbConcealCard,m_cbConcealCount);
			GameEnd.wGameScore=m_GameLogic.GetCardScore(m_cbScoreCardData,m_cbScoreCardCount);

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

			//底牌信息
			GameEnd.cbConcealCount=m_cbConcealCount;
			CopyMemory(GameEnd.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);

			//游戏得分
			GameEnd.wConcealScore=m_GameLogic.GetCardScore(m_cbConcealCard,m_cbConcealCount);
			GameEnd.wGameScore=m_GameLogic.GetCardScore(m_cbScoreCardData,m_cbScoreCardCount);


			//扣底倍数
			if ((wChairID!=m_wBankerUser)&&(((wChairID+2)%m_wPlayerCount)!=m_wBankerUser)&&(m_GameLogic.GetCardLogicColor(m_cbOutCardData[wChairID],m_cbOutCardCount[wChairID])==64))
			{
				//类型判断
				BYTE cbCardType=m_GameLogic.GetCardType(m_cbOutCardData[wChairID],m_cbOutCardCount[wChairID]);
				if (cbCardType==CT_ERROR) return false;

				if (cbCardType!=CT_THROW_CARD) 
				{
					switch (m_cbOutCardCount[wChairID])
					{
					case 1:
						GameEnd.wConcealTime=1;
						break;
					case 2:
						GameEnd.wConcealTime=2;
						break;
					case 4:
						GameEnd.wConcealTime=4;
						break;
					case 6:
						GameEnd.wConcealTime=6;
						break;
					case 8:
						GameEnd.wConcealTime=8;
						break;
					default:
						GameEnd.wConcealTime=1;
						break;
					}
				}
				else
				{
					GameEnd.wConcealTime=1;
				}
			}

			//是否扣王
			bool bIsKouWang=false;
			//是否抠底(大小王抠底)
			bool bIsKingKou=false;

			//----------------------------------------------------------------------------
			//判断是否扣王
			BYTE bSmallKing=0;  //小王和大王数
			BYTE bBigKing=0;

			for(byte i=0;i<m_cbConcealCount;i++)
			{
				if(m_GameLogic.GetCardValue(m_cbConcealCard[i])==14) bSmallKing++;
				if(m_GameLogic.GetCardValue(m_cbConcealCard[i])==15) bBigKing++;
			}
			if(bSmallKing!=0||bBigKing!=0) bIsKouWang=true;
			//----------------------------------------------------------------------------
			//判断是否抠底(大小王抠底)
			BYTE bSmallKou=0;
			BYTE bBigKou=0;

			if(GameEnd.wConcealTime==1)  //单抠
			{
				if(m_GameLogic.GetCardValue(m_cbOutCardData[wChairID][0])==14) bSmallKou=1;
				if(m_GameLogic.GetCardValue(m_cbOutCardData[wChairID][0])==15) bBigKou=1;
			}
			if(GameEnd.wConcealTime==2)  //双抠
			{
				if(m_GameLogic.GetCardValue(m_cbOutCardData[wChairID][0])==14) bSmallKou=2;
				if(m_GameLogic.GetCardValue(m_cbOutCardData[wChairID][0])==15) bBigKou=2;
			}
			if(bSmallKou!=0||bBigKou!=0) bIsKingKou=true;
			//----------------------------------------------------------------------------
			LONG lBankerScore=0;
			LONG lLostScore=0;
			BYTE bNum=0;

			if(bIsKouWang) //扣王计算
			{
				//庄家胜利
				if((wChairID==m_wBankerUser)&&(wChairID==(m_wBankerUser+2)%m_wPlayerCount))
				{
					switch(bSmallKing)
					{
					case 0:
						switch(bBigKing)
						{
						case 1:
							{
								lBankerScore=4;
								lLostScore=-2;
								bNum=2;
								break;
							}
						case 2:
							{
								lBankerScore=8;
								lLostScore=-4;
								bNum=5;
								break;
							}
						}
						break;
					case 1:
						switch(bBigKing)
						{
						case 0:
							{
								lBankerScore=2;
								lLostScore=-1;
								bNum=1;
								break;
							}
						case 1:
							{
								lBankerScore=6;
								lLostScore=-3;
								bNum=4;
								break;
							}
						case 2:
							{
								lBankerScore=14;
								lLostScore=-7;
								bNum=7;
								break;
							}
						}
						break;
					case 2:
						switch(bBigKing)
						{
						case 0:
							{
								lBankerScore=6;
								lLostScore=-3;
								bNum=3;
								break;
							}
						case 1:
							{
								lBankerScore=12;
								lLostScore=-6;
								bNum=6;
								break;
							}
						case 2:
							{
								lBankerScore=16;
								lLostScore=-8;
								bNum=9;
								break;
							}
						}
						break;
					}
					//打家升级判断
					if(((m_cbValueOrder[m_wBankerUser%2]+bNum)%13+1)>=11&&m_cbValueOrder[m_wBankerUser%2]<11)	m_cbValueOrder[m_wBankerUser%2]=11;
					else if(((m_cbValueOrder[m_wBankerUser%2]+bNum)%13+1)>=12&&m_cbValueOrder[m_wBankerUser%2]==11)	m_cbValueOrder[m_wBankerUser%2]=12;
					else if(((m_cbValueOrder[m_wBankerUser%2]+bNum)%13+1)>=13&&m_cbValueOrder[m_wBankerUser%2]==12)	m_cbValueOrder[m_wBankerUser%2]=13;
					else if(((m_cbValueOrder[m_wBankerUser%2]+bNum)%13+1)>13&&m_cbValueOrder[m_wBankerUser%2]==13)	m_cbValueOrder[m_wBankerUser%2]=1;
					else m_cbValueOrder[m_wBankerUser%2]=(m_cbValueOrder[m_wBankerUser%2]+bNum)%13+1;

					//玩家得分
					GameEnd.lScore[m_wBankerUser]=lBankerScore*m_pGameServiceOption->lCellScore;
					GameEnd.lScore[(m_wBankerUser+2)%m_wPlayerCount]=lBankerScore*m_pGameServiceOption->lCellScore;
					GameEnd.lScore[(m_wBankerUser+1)%m_wPlayerCount]=lLostScore*m_pGameServiceOption->lCellScore;
					GameEnd.lScore[(m_wBankerUser+3)%m_wPlayerCount]=lLostScore*m_pGameServiceOption->lCellScore;
				}
				////防家胜利
				else
				{
					switch(bSmallKing)
					{
					case 0:
						switch(bBigKing)
						{
						case 1:
							{
								lBankerScore=4;
								lLostScore=-2;
								bNum=2;
								break;
							}
						case 2:
							{
								lBankerScore=8;
								lLostScore=-4;
								bNum=5;
								break;
							}
						}
						break;
					case 1:
						switch(bBigKing)
						{
						case 0:
							{
								lBankerScore=2;
								lLostScore=-1;
								bNum=1;
								break;
							}
						case 1:
							{
								lBankerScore=6;
								lLostScore=-3;
								bNum=4;
								break;
							}
						case 2:
							{
								lBankerScore=14;
								lLostScore=-7;
								bNum=7;
								break;
							}
						}
						break;
					case 2:
						switch(bBigKing)
						{
						case 0:
							{
								lBankerScore=6;
								lLostScore=-3;
								bNum=3;
								break;
							}
						case 1:
							{
								lBankerScore=12;
								lLostScore=-6;
								bNum=6;
								break;
							}
						case 2:
							{
								lBankerScore=16;
								lLostScore=-8;
								bNum=9;
								break;
							}
						}
						break;
					}
					//降级惩罚分
					BYTE bWinScore=0;
					BYTE bLostScore=0;

					//打家降级判断 J->A    Q->6   K->J
					if(m_cbValueOrder[m_wBankerUser%2]==11)	
					{
						m_cbValueOrder[m_wBankerUser%2]=1;
						bLostScore=-5;
						bWinScore=10;
					}
					if(m_cbValueOrder[m_wBankerUser%2]==12)	
					{
						m_cbValueOrder[m_wBankerUser%2]=6;
						bLostScore=-6;
						bWinScore=12;
					}
					if(m_cbValueOrder[m_wBankerUser%2]==13)
					{
						m_cbValueOrder[m_wBankerUser%2]=11;
						bLostScore=-7;
						bWinScore=14;
					}

					//防家升级判断
					if(((m_cbValueOrder[(m_wBankerUser+1)%2]+bNum)%13+1)>=11&&m_cbValueOrder[(m_wBankerUser+1)%2]<11)	m_cbValueOrder[(m_wBankerUser+1)%2]=11;
					else if(((m_cbValueOrder[(m_wBankerUser+1)%2]+bNum)%13+1)>=12&&m_cbValueOrder[(m_wBankerUser+1)%2]==11)	m_cbValueOrder[(m_wBankerUser+1)%2]=12;
					else if(((m_cbValueOrder[(m_wBankerUser+1)%2]+bNum)%13+1)>=13&&m_cbValueOrder[(m_wBankerUser+1)%2]==12)	m_cbValueOrder[(m_wBankerUser+1)%2]=13;
					else if(((m_cbValueOrder[(m_wBankerUser+1)%2]+bNum)%13+1)>13&&m_cbValueOrder[(m_wBankerUser+1)%2]==13)	m_cbValueOrder[(m_wBankerUser+1)%2]=1;
					else m_cbValueOrder[(m_wBankerUser+1)%2]=(m_cbValueOrder[(m_wBankerUser+1)%2]+bNum)%13+1;
				
					//玩家得分
					GameEnd.lScore[m_wBankerUser]=(lLostScore+bLostScore)*m_pGameServiceOption->lCellScore;
					GameEnd.lScore[(m_wBankerUser+2)%m_wPlayerCount]=(lLostScore+bLostScore)*m_pGameServiceOption->lCellScore;
					GameEnd.lScore[(m_wBankerUser+1)%m_wPlayerCount]=(lBankerScore+bWinScore)*m_pGameServiceOption->lCellScore;
					GameEnd.lScore[(m_wBankerUser+3)%m_wPlayerCount]=(lBankerScore+bWinScore)*m_pGameServiceOption->lCellScore;
				}
			}
			//----------------------------------------------------------------------------
			else if (bIsKingKou) //抠底计算
			{
				//防家胜利
				if((wChairID!=m_wBankerUser)&&(wChairID!=(m_wBankerUser+2)%m_wPlayerCount))
				{
					if(bSmallKou==1) 
					{
						lBankerScore=2;
						lLostScore=-1;
						bNum=1;
					}
					if(bSmallKou==2) 
					{
						lBankerScore=6;
						lLostScore=-3;
						bNum=3;
					}
					if(bBigKou==1) 
					{
						lBankerScore=4;
						lLostScore=-2;
						bNum=2;
					}
					if(bBigKou==2) 
					{
						lBankerScore=8;
						lLostScore=-4;
						bNum=5;
					}
					//降级惩罚分
					BYTE bWinScore=0;
					BYTE bLostScore=0;

					//打家降级判断 J->A    Q->6   K->J
					if(m_cbValueOrder[m_wBankerUser%2]==11)	
					{
						m_cbValueOrder[m_wBankerUser%2]=1;
						bLostScore=-5;
						bWinScore=10;
					}
					if(m_cbValueOrder[m_wBankerUser%2]==12)	
					{
						m_cbValueOrder[m_wBankerUser%2]=6;
						bLostScore=-6;
						bWinScore=12;
					}
					if(m_cbValueOrder[m_wBankerUser%2]==13)
					{
						m_cbValueOrder[m_wBankerUser%2]=11;
						bLostScore=-7;
						bWinScore=14;
					}

					//防家升级判断
					if(((m_cbValueOrder[(m_wBankerUser+1)%2]+bNum)%13+1)>=11&&m_cbValueOrder[(m_wBankerUser+1)%2]<11)	m_cbValueOrder[(m_wBankerUser+1)%2]=11;
					else if(((m_cbValueOrder[(m_wBankerUser+1)%2]+bNum)%13+1)>=12&&m_cbValueOrder[(m_wBankerUser+1)%2]==11)	m_cbValueOrder[(m_wBankerUser+1)%2]=12;
					else if(((m_cbValueOrder[(m_wBankerUser+1)%2]+bNum)%13+1)>=13&&m_cbValueOrder[(m_wBankerUser+1)%2]==12)	m_cbValueOrder[(m_wBankerUser+1)%2]=13;
					else if(((m_cbValueOrder[(m_wBankerUser+1)%2]+bNum)%13+1)>13&&m_cbValueOrder[(m_wBankerUser+1)%2]==13)	m_cbValueOrder[(m_wBankerUser+1)%2]=1;
					else m_cbValueOrder[(m_wBankerUser+1)%2]=(m_cbValueOrder[(m_wBankerUser+1)%2]+bNum)%13+1;

					//玩家得分
					GameEnd.lScore[m_wBankerUser]=(lLostScore+bLostScore)*m_pGameServiceOption->lCellScore;
					GameEnd.lScore[(m_wBankerUser+2)%m_wPlayerCount]=(lLostScore+bLostScore)*m_pGameServiceOption->lCellScore;
					GameEnd.lScore[(m_wBankerUser+1)%m_wPlayerCount]=(lBankerScore+bWinScore)*m_pGameServiceOption->lCellScore;
					GameEnd.lScore[(m_wBankerUser+3)%m_wPlayerCount]=(lBankerScore+bWinScore)*m_pGameServiceOption->lCellScore;
				}
				else
				{
					if(bSmallKou==1) 
					{
						lBankerScore=2;
						lLostScore=-1;
						bNum=1;
					}
					if(bSmallKou==2) 
					{
						lBankerScore=6;
						lLostScore=-3;
						bNum=3;
					}
					if(bBigKou==1) 
					{
						lBankerScore=4;
						lLostScore=-2;
						bNum=2;
					}
					if(bBigKou==2) 
					{
						lBankerScore=8;
						lLostScore=-4;
						bNum=5;
					}
					//打家升级判断
					if(((m_cbValueOrder[m_wBankerUser%2]+bNum)%13+1)>=11&&m_cbValueOrder[m_wBankerUser%2]<11)	m_cbValueOrder[m_wBankerUser%2]=11;
					else if(((m_cbValueOrder[m_wBankerUser%2]+bNum)%13+1)>=12&&m_cbValueOrder[m_wBankerUser%2]==11)	m_cbValueOrder[m_wBankerUser%2]=12;
					else if(((m_cbValueOrder[m_wBankerUser%2]+bNum)%13+1)>=13&&m_cbValueOrder[m_wBankerUser%2]==12)	m_cbValueOrder[m_wBankerUser%2]=13;
					else if(((m_cbValueOrder[m_wBankerUser%2]+bNum)%13+1)>13&&m_cbValueOrder[m_wBankerUser%2]==13)	m_cbValueOrder[m_wBankerUser%2]=1;
					else m_cbValueOrder[m_wBankerUser%2]=(m_cbValueOrder[m_wBankerUser%2]+bNum)%13+1;

					//玩家得分
					GameEnd.lScore[m_wBankerUser]=lBankerScore*m_pGameServiceOption->lCellScore;
					GameEnd.lScore[(m_wBankerUser+2)%m_wPlayerCount]=lBankerScore*m_pGameServiceOption->lCellScore;
					GameEnd.lScore[(m_wBankerUser+1)%m_wPlayerCount]=lLostScore*m_pGameServiceOption->lCellScore;
					GameEnd.lScore[(m_wBankerUser+3)%m_wPlayerCount]=lLostScore*m_pGameServiceOption->lCellScore;
				}
			}
			//----------------------------------------------------------------------------
			else //普通计算
			{
				//统计变量
				WORD wWholeScore=GameEnd.wGameScore+GameEnd.wConcealTime*GameEnd.wConcealScore;

				//积分统计
				if (wWholeScore==0) 
				{
					lBankerScore=3L;
					m_cbValueOrder[m_wBankerUser%2]=(m_cbValueOrder[m_wBankerUser%2]+2)%13+1;
				}
				/*
				else if (wWholeScore<20*m_cbPackCount) 
				{
					lBankerScore=2L;
					m_cbValueOrder[m_wBankerUser%2]=(m_cbValueOrder[m_wBankerUser%2]+1)%13+1;
				}
				*/
				else if (wWholeScore<40*m_cbPackCount)
				{
					lBankerScore=1L;
					m_cbValueOrder[m_wBankerUser%2]=m_cbValueOrder[m_wBankerUser%2]%13+1;
				}
				else
				{
					lBankerScore=-(wWholeScore-40*m_cbPackCount)/(10*m_cbPackCount)-1;
					m_cbValueOrder[(m_wBankerUser+1)%2]=(BYTE)((m_cbValueOrder[(m_wBankerUser+1)%2]-lBankerScore-2)%13+1);
				}
				
				//玩家得分
				GameEnd.lScore[m_wBankerUser]=lBankerScore*m_pGameServiceOption->lCellScore;
				GameEnd.lScore[(m_wBankerUser+2)%m_wPlayerCount]=lBankerScore*m_pGameServiceOption->lCellScore;
				GameEnd.lScore[(m_wBankerUser+1)%m_wPlayerCount]=-(lBankerScore*m_pGameServiceOption->lCellScore);
				GameEnd.lScore[(m_wBankerUser+3)%m_wPlayerCount]=-(lBankerScore*m_pGameServiceOption->lCellScore);

			}

			//----------------------------------------------------------------------------------------------------
			//计算税收
			LONG lGameTax=0L;
			if (m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)
			{
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if (GameEnd.lScore[i]>=100L)
					{
						lGameTax+=GameEnd.lScore[i]/10L;
						GameEnd.lScore[i]=GameEnd.lScore[i]*45L/50L;
					}
				}
			}

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//修改积分
			tagScoreInfo ScoreInfo[m_wPlayerCount];
			ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				ScoreInfo[i].lScore=GameEnd.lScore[i];
				ScoreInfo[i].ScoreKind=(GameEnd.lScore[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
			}
//			m_pITableFrame->WriteTableScore(ScoreInfo,m_wPlayerCount,lGameTax);

			//庄家切换
			if (GameEnd.lScore[m_wBankerUser]>0L) m_wBankerUser=(m_wBankerUser+2)%m_wPlayerCount;
			else m_wBankerUser=(m_wBankerUser+1)%m_wPlayerCount;

			//主牌设置
			m_cbMainValue=m_cbValueOrder[m_wBankerUser%2];

			//结束游戏
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_USER_LEFT:		//用户强退
		{
			//定义变量
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//底牌信息
			GameEnd.cbConcealCount=m_cbConcealCount;
			CopyMemory(GameEnd.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);

			//游戏得分
			GameEnd.wConcealScore=m_GameLogic.GetCardScore(m_cbConcealCard,m_cbConcealCount);
			GameEnd.wGameScore=m_GameLogic.GetCardScore(m_cbScoreCardData,m_cbScoreCardCount);

			//玩家得分
			GameEnd.lScore[wChairID]=-8*m_pGameServiceOption->lCellScore;
			
			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//修改积分
			tagScoreInfo ScoreInfo;
			ScoreInfo.ScoreKind=enScoreKind_Flee;
			ScoreInfo.lScore=GameEnd.lScore[wChairID];
			m_pITableFrame->WriteUserScore(wChairID,ScoreInfo.lScore ,ScoreInfo.lRevenue ,ScoreInfo.ScoreKind );
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

	ASSERT(FALSE);

	return false;
}

//发送场景
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_UG_FREE:		//空闲状态
		{
			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//设置变量
			StatusFree.lBaseScore=m_pGameServiceOption->lCellScore;

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_UG_CALL:		//叫牌状态
		{
			//构造数据
			CMD_S_StatusCall StatusCall;
			ZeroMemory(&StatusCall,sizeof(StatusCall));

			//游戏变量
			StatusCall.wBankerUser=m_wBankerUser;
			StatusCall.cbPackCount=m_cbPackCount;
			StatusCall.cbMainValue=m_cbMainValue;
			StatusCall.cbValueOrder[0]=m_cbValueOrder[0];
			StatusCall.cbValueOrder[1]=m_cbValueOrder[1];

			//发牌变量
			StatusCall.cbCardCount=m_cbHandCardCount[wChiarID];
			CopyMemory(StatusCall.cbCardData,m_cbHandCardData[wChiarID],sizeof(BYTE)*m_cbHandCardCount[wChiarID]);

			//叫牌信息
			StatusCall.cbCallCard=m_cbCallCard;
			StatusCall.cbCallCount=m_cbCallCount;
			StatusCall.wCallCardUser=m_wCallCardUser;
			StatusCall.cbComplete=(m_bCallCard[wChiarID]==true)?TRUE:FALSE;

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusCall,sizeof(StatusCall));
		}
	case GS_UG_BACK:		//底牌状态
		{
			//构造数据
			CMD_S_StatusBack StatusBack;
			ZeroMemory(&StatusBack,sizeof(StatusBack));

			//游戏变量
			StatusBack.cbPackCount=m_cbPackCount;
			StatusBack.cbMainValue=m_cbMainValue;
			StatusBack.cbMainColor=m_cbMainColor;
			StatusBack.cbValueOrder[0]=m_cbValueOrder[0];
			StatusBack.cbValueOrder[1]=m_cbValueOrder[1];

			//叫牌变量
			StatusBack.cbCallCard=m_cbCallCard;
			StatusBack.cbCallCount=m_cbCallCount;
			StatusBack.wCallCardUser=m_wCallCardUser;

			//用户变量
			StatusBack.wBankerUser=m_wBankerUser;
			StatusBack.wCurrentUser=m_wCurrentUser;

			//扑克变量
			StatusBack.cbCardCount=m_cbHandCardCount[wChiarID];
			CopyMemory(StatusBack.cbCardData,m_cbHandCardData[wChiarID],sizeof(BYTE)*m_cbHandCardCount[wChiarID]);

			//底牌信息
			if (wChiarID==m_wBankerUser)
			{
				StatusBack.cbConcealCount=m_cbConcealCount;
				CopyMemory(StatusBack.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
			}

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusBack,sizeof(StatusBack));
		}
	case GS_UG_PLAY:		//游戏状态
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//游戏变量
			StatusPlay.cbPackCount=m_cbPackCount;
			StatusPlay.cbMainValue=m_cbMainValue;
			StatusPlay.cbMainColor=m_cbMainColor;
			StatusPlay.cbValueOrder[0]=m_cbValueOrder[0];
			StatusPlay.cbValueOrder[1]=m_cbValueOrder[1];

			//叫牌变量
			StatusPlay.cbCallCard=m_cbCallCard;
			StatusPlay.cbCallCount=m_cbCallCount;
			StatusPlay.wCallCardUser=m_wCallCardUser;

			//用户变量
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.wFirstOutUser=m_wFirstOutUser;

			//扑克变量
			StatusPlay.cbCardCount=m_cbHandCardCount[wChiarID];
			CopyMemory(StatusPlay.cbCardData,m_cbHandCardData[wChiarID],sizeof(BYTE)*m_cbHandCardCount[wChiarID]);

			//底牌信息
			if (wChiarID==m_wBankerUser)
			{
				StatusPlay.cbConcealCount=m_cbConcealCount;
				CopyMemory(StatusPlay.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
			}

			//出牌变量
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				StatusPlay.cbOutCardCount[i]=m_cbOutCardCount[i];
				CopyMemory(StatusPlay.cbOutCardData[i],m_cbOutCardData[i],sizeof(BYTE)*m_cbOutCardCount[i]);
			}

			//得分变量
			StatusPlay.cbScoreCardCount=m_cbScoreCardCount;
			CopyMemory(StatusPlay.cbScoreCardData,m_cbScoreCardData,sizeof(StatusPlay.cbScoreCardData));

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	case GS_UG_WAIT:	//等待状态
		{
			//构造数据
			CMD_S_StatusWait StatusWait;
			ZeroMemory(&StatusWait,sizeof(StatusWait));

			//用户变量
			StatusWait.wBankerUser=m_wBankerUser;

			//游戏变量
			StatusWait.cbPackCount=m_cbPackCount;
			StatusWait.cbMainValue=m_cbMainValue;
			StatusWait.cbValueOrder[0]=m_cbValueOrder[0];
			StatusWait.cbValueOrder[1]=m_cbValueOrder[1];

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusWait,sizeof(StatusWait));
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
	case SUB_C_CALL_CARD:		//用户叫牌
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_CallCard));
			if (wDataSize!=sizeof(CMD_C_CallCard)) return false;

			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//消息处理
			CMD_C_CallCard * pCallCard=(CMD_C_CallCard *)pDataBuffer;
			return OnUserCallCard(pUserData->wChairID,pCallCard->cbCallCard,pCallCard->cbCallCount);
		}
	case SUB_C_CALL_FINISH:		//叫牌完成
		{
			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//消息处理
			return OnUserCallFinish(pUserData->wChairID);
		}
	case SUB_C_CONCEAL_CARD:	//底牌扑克
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_ConcealCard));
			if (wDataSize!=sizeof(CMD_C_ConcealCard)) return false;

			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//消息处理
			CMD_C_ConcealCard * pConcealCard=(CMD_C_ConcealCard *)pDataBuffer;
			return OnUserConcealCard(pUserData->wChairID,pConcealCard->cbConcealCard,pConcealCard->cbConcealCount);
		}
	case SUB_C_KOU_WANG:	//用户扣王
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_KouWang));
			if (wDataSize!=sizeof(CMD_C_KouWang)) return false;

			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//消息处理
			CMD_C_KouWang * pKouWang=(CMD_C_KouWang *)pDataBuffer;
			return OnUserKouWang(pUserData->wChairID,pKouWang->cbConcealCard,pKouWang->cbConcealCount,pKouWang->cbUserCardData,pKouWang->cbUserCardCount);
		}
	case SUB_C_OUT_CARD:		//用户出牌
		{
			//变量定义
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			WORD wHeadSize=sizeof(CMD_C_OutCard)-sizeof(pOutCard->cbCardData);

			//效验数据
			ASSERT(wDataSize>=wHeadSize);
			if (wDataSize<wHeadSize) return false;
			ASSERT(wDataSize==(wHeadSize+pOutCard->cbCardCount*sizeof(pOutCard->cbCardData[0])));
			if (wDataSize!=(wHeadSize+pOutCard->cbCardCount*sizeof(pOutCard->cbCardData[0]))) return false;

			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//消息处理
			return OnUserOutCard(pUserData->wChairID,pOutCard->cbCardData,pOutCard->cbCardCount);
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
bool __cdecl CTableFrameSink::OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//设置变量
	if (bLookonUser==false)
	{
		m_cbMainValue=0x01;
		m_cbValueOrder[0]=0x01;
		m_cbValueOrder[1]=0x01;
		m_wBankerUser=INVALID_CHAIR;
	}

	return true;
}

//用户叫牌
bool CTableFrameSink::OnUserCallCard(WORD wChairID, BYTE cbCallCard, BYTE cbCallCount)
{
	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus()==GS_UG_CALL);
	if (m_pITableFrame->GetGameStatus()!=GS_UG_CALL) return true;

	//用户效验
	ASSERT(m_bCallCard[wChairID]==false);
	if (m_bCallCard[wChairID]==true) return false;

	//数目统计
	BYTE cbHandCardCount=0;
	for (BYTE i=0;i<m_cbHandCardCount[wChairID];i++)
	{
		if (m_cbHandCardData[wChairID][i]==cbCallCard) cbHandCardCount++;
	}

	//效验数据
	ASSERT((cbHandCardCount>=cbCallCount)&&(cbCallCount>0));
	if ((cbHandCardCount<cbCallCount)||(cbCallCount==0)) return false;

	//叫牌判断
	if (m_wCallCardUser==wChairID)
	{
		//效验数据
		ASSERT((cbCallCard==m_cbCallCard)&&(cbCallCount>m_cbCallCount));
		if ((cbCallCard!=m_cbCallCard)||(cbCallCount<=m_cbCallCount)) return true;
	}
	else
	{
		//效验数据
		BYTE cbPackCount=m_GameLogic.GetPackCount();
		BYTE cbCardColor=m_GameLogic.GetCardColor(cbCallCard);
		BYTE cbCallColor=m_GameLogic.GetCardColor(m_cbCallCard);

		//效验数据
		ASSERT((cbCallCount>m_cbCallCount)||((cbCallCount==cbPackCount)&&(cbCardColor>cbCallColor)));
		if ((cbCallCount<=m_cbCallCount)&&((cbCallCount!=cbPackCount)||(cbCardColor<=cbCallColor))) return true;
	}

	//设置变量
	m_cbCallCard=cbCallCard;
	m_wCallCardUser=wChairID;
	m_cbCallCount=cbCallCount;

	//构造数据
	CMD_S_CallCard CallCard;
	CallCard.cbCallCard=m_cbCallCard;
	CallCard.cbCallCount=m_cbCallCount;
	CallCard.wCallCardUser=m_wCallCardUser;

	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CALL_CARD,&CallCard,sizeof(CallCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CALL_CARD,&CallCard,sizeof(CallCard));

	return true;
}

//叫牌完成
bool CTableFrameSink::OnUserCallFinish(WORD wChairID)
{
	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus()==GS_UG_CALL);
	if (m_pITableFrame->GetGameStatus()!=GS_UG_CALL) return true;

	//用户效验
	ASSERT(m_bCallCard[wChairID]==false);
	if (m_bCallCard[wChairID]==true) return false;

	//变量设置
	m_bCallCard[wChairID]=true;

	//完成判断
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_bCallCard[i]==false) break;
	}
	if (i!=m_wPlayerCount) return true;

	//庄家设置
	if (m_wBankerUser==INVALID_CHAIR) m_wBankerUser=m_wCallCardUser;

	//重新发牌
	if (m_wBankerUser==INVALID_CHAIR)
	{
		//设置状态
		ZeroMemory(m_bCallCard,sizeof(m_bCallCard));
		
		m_bNotCall=true;
		m_wBankerUser=0;
		m_wCallCardUser=0;
		//发送扑克
		//DispatchUserCard();
		//return true;
	}

	//花色调整
	if(m_bNotCall==false)
	{
		if (m_wCallCardUser==INVALID_CHAIR) m_cbMainColor=COLOR_NT;
		else m_cbMainColor=m_GameLogic.GetCardColor(m_cbCallCard);
	}
	else
	{
		BYTE bCardValue=m_GameLogic.GetCardValue(m_cbConcealCard[2]);
		m_cbMainColor=m_GameLogic.GetCardColor(m_cbConcealCard[2]);
		if(bCardValue==14) m_cbMainColor=COLOR_HEI_TAO;
		if(bCardValue==15) m_cbMainColor=COLOR_HONG_TAO;
		if(bCardValue==m_cbMainValue)
		{
			m_cbMainColor=m_GameLogic.GetCardColor(bCardValue);
			//干锅巴成立
			m_bIsGanGuoBa=true;

			//设置变量
			WORD wHaveKing=VALUE_ERROR;
			m_bIsYingGanGuoBa=false;
			
			//判断软硬锅巴
			for(BYTE i=0;i<m_cbConcealCount;i++)
			{
				wHaveKing=m_cbConcealCard[i];
				if(wHaveKing==78||wHaveKing==79) 
				{
					m_bIsYingGanGuoBa=true;
				}
			}
			m_bIsRuanGanGuoBa=(m_bIsYingGanGuoBa==true)?false:true;
		}
	}
	
	//设置变量
	m_wCurrentUser=m_wBankerUser;
	m_wFirstOutUser=m_wBankerUser;
	m_GameLogic.SetMainColor(m_cbMainColor);
	m_pITableFrame->SetGameStatus(GS_UG_BACK);

	//设置底牌
	BYTE cbHandCardCount=m_cbHandCardCount[m_wCurrentUser];
	CopyMemory(&m_cbHandCardData[m_wCurrentUser][cbHandCardCount],m_cbConcealCard,m_cbConcealCount*sizeof(BYTE));
	m_cbHandCardCount[m_wCurrentUser]+=m_cbConcealCount;

	//构造数据
	CMD_S_SendConceal SendConceal;
	SendConceal.cbMainColor=m_cbMainColor;
	SendConceal.wBankerUser=m_wBankerUser;
	SendConceal.wCurrentUser=m_wCurrentUser;

	//发送数据
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//构造底牌
		if (i==m_wCurrentUser)
		{
			SendConceal.cbConcealCount=m_cbConcealCount;
			CopyMemory(SendConceal.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
		}
		else 
		{
			SendConceal.cbConcealCount=0;
			ZeroMemory(SendConceal.cbConcealCard,sizeof(SendConceal.cbConcealCard));
		}

		//发送消息
		m_pITableFrame->SendTableData(i,SUB_S_SEND_CONCEAL,&SendConceal,sizeof(SendConceal));
		m_pITableFrame->SendLookonData(i,SUB_S_SEND_CONCEAL,&SendConceal,sizeof(SendConceal));
	}

	return true;
}

//底牌扑克
bool CTableFrameSink::OnUserConcealCard(WORD wChairID, BYTE cbConcealCard[], BYTE cbConcealCount)
{
	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus()==GS_UG_BACK);
	if (m_pITableFrame->GetGameStatus()!=GS_UG_BACK) return true;

	//效验数据
	ASSERT((wChairID==m_wCurrentUser)&&(m_cbConcealCount==cbConcealCount));
	if ((wChairID!=m_wCurrentUser)||(m_cbConcealCount!=cbConcealCount)) return false;

	//删除底牌
	if (m_GameLogic.RemoveCard(cbConcealCard,cbConcealCount,m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//设置扑克
	m_cbHandCardCount[wChairID]-=cbConcealCount;
	CopyMemory(m_cbConcealCard,cbConcealCard,cbConcealCount*sizeof(BYTE));

	//扑克排列
	for (WORD i=0;i<m_wPlayerCount;i++) 
	{
		ASSERT(m_cbHandCardCount[i]==m_GameLogic.GetDispatchCount());
		m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i]);
	}

	//干锅巴状态不必扣王
	if(m_bIsGanGuoBa==false)
	{
		//设置状态
		m_pITableFrame->SetGameStatus(GS_UG_KOU);

		m_bKouFlag[m_wCurrentUser]=true;
		WORD wNextUser=INVALID_CHAIR;

		//判断用户
		for(WORD i=0;i<m_wPlayerCount;i++)
		{
			if(i==INVALID_CHAIR) break;
			WORD wHaveKing=VALUE_ERROR;
			if(m_wCurrentUser!=i)
			{
				m_bKouFlag[i]=true;
				for(WORD j=0;j<m_cbHandCardCount[i];j++)
				{
					wHaveKing=m_cbHandCardData[i][j];
					if(wHaveKing==78||wHaveKing==79) 
					{
						wNextUser=i;
						m_bKouFlag[i]=false;
						break;
					}
				}
			}
		}
		
		//构造数据
		CMD_S_KouWang KouWang;
		KouWang.wNextUser=wNextUser;
		
		//发送消息
		for(i=0;i<m_wPlayerCount;i++)
		{
			//构造底牌
			KouWang.cbConcealCount=m_cbConcealCount;
			CopyMemory(KouWang.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);

			m_pITableFrame->SendTableData(i,SUB_S_KOU_WANG,&KouWang,sizeof(KouWang));
			m_pITableFrame->SendLookonData(i,SUB_S_KOU_WANG,&KouWang,sizeof(KouWang));
		}
	}
	else
	{
		//设置状态
		m_pITableFrame->SetGameStatus(GS_UG_PLAY);

		//构造数据
		CMD_S_GamePlay GamePlay;
		GamePlay.wCurrentUser=m_wCurrentUser;

		//发送数据
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//构造底牌
			GamePlay.cbConcealCount=m_cbConcealCount;
			CopyMemory(GamePlay.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
			
			//发送消息
			m_pITableFrame->SendTableData(i,SUB_S_GAME_PLAY,&GamePlay,sizeof(GamePlay));
			m_pITableFrame->SendLookonData(i,SUB_S_GAME_PLAY,&GamePlay,sizeof(GamePlay));
		}
	}
	return true;
}

//用户扣王
bool CTableFrameSink::OnUserKouWang(WORD wChairID, BYTE cbConcealCard[], BYTE cbConcealCount,BYTE cbUserCard[], BYTE cbUserCardCount)
{
	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus()==GS_UG_KOU);
	if (m_pITableFrame->GetGameStatus()!=GS_UG_KOU) return true;

	//效验用户
	ASSERT((cbUserCardCount<=MAX_COUNT)&&(cbConcealCount<=8));
	if ((cbUserCardCount>MAX_COUNT)||(cbConcealCount>8)) return false;

	//变量设置
	m_bKouFlag[wChairID]=true;
	CopyMemory(m_cbConcealCard,cbConcealCard,sizeof(BYTE)*cbConcealCount);
	CopyMemory(m_cbHandCardData[wChairID],cbUserCard,sizeof(BYTE)*cbUserCardCount);

	//是否都经过扣王
	bool bAllKou=true;   

	for(BYTE i=0;i<m_wPlayerCount;i++)
	{
		if(m_bKouFlag[i]==false) 
		{
			bAllKou=false;	
			break;
		}
	}

	//还有人没扣王
	if(bAllKou==false)
	{
		//构造数据
		CMD_S_KouWang KouWang;
		for(BYTE i=0;i<m_wPlayerCount;i++)
		{
			if(m_bKouFlag[i]==false)
			{
				KouWang.wNextUser=i;
				break;
			}
		}

		//发送消息
		for(i=0;i<m_wPlayerCount;i++)
		{
			//构造底牌
			KouWang.cbConcealCount=cbConcealCount;
			CopyMemory(KouWang.cbConcealCard,cbConcealCard,sizeof(BYTE)*cbConcealCount);

			m_pITableFrame->SendTableData(i,SUB_S_KOU_WANG,&KouWang,sizeof(KouWang));
			m_pITableFrame->SendLookonData(i,SUB_S_KOU_WANG,&KouWang,sizeof(KouWang));
		}
	}
	else
	{
		//设置状态
		m_pITableFrame->SetGameStatus(GS_UG_PLAY);

		//构造数据
		CMD_S_GamePlay GamePlay;
		GamePlay.wCurrentUser=m_wCurrentUser;

		//发送数据
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//构造底牌
			GamePlay.cbConcealCount=m_cbConcealCount;
			CopyMemory(GamePlay.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
			
			//发送消息
			m_pITableFrame->SendTableData(i,SUB_S_GAME_PLAY,&GamePlay,sizeof(GamePlay));
			m_pITableFrame->SendLookonData(i,SUB_S_GAME_PLAY,&GamePlay,sizeof(GamePlay));
		}
	}

	return true;
}


//用户出牌
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount)
{
	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus()==GS_UG_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_UG_PLAY) return true;

	//效验用户
	ASSERT((wChairID==m_wCurrentUser)&&(cbCardCount<=MAX_COUNT));
	if ((wChairID!=m_wCurrentUser)||(cbCardCount>MAX_COUNT)) return false;

	//首出牌者
	if (m_wFirstOutUser==wChairID)
	{
		//类型判断
		BYTE cbCardType=m_GameLogic.GetCardType(cbCardData,cbCardCount);
		if (cbCardType==CT_ERROR) return false;
	
		//甩牌处理
		if (cbCardType==CT_THROW_CARD)
		{
			//甩牌判断
			tagOutCardResult OutCardResult;
			ZeroMemory(&OutCardResult,sizeof(OutCardResult));
			bool bThrowSuccess=m_GameLogic.EfficacyThrowCard(cbCardData,cbCardCount,wChairID,m_cbHandCardData,m_cbHandCardCount[wChairID],OutCardResult);

			//结果处理
			if (bThrowSuccess==false)
			{
				//删除扑克
				if (m_GameLogic.RemoveCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false) return false;
				m_cbHandCardCount[wChairID]-=OutCardResult.cbCardCount;

				//出牌记录
				m_cbOutCardCount[wChairID]=OutCardResult.cbCardCount;
				CopyMemory(m_cbOutCardData[wChairID],OutCardResult.cbResultCard,OutCardResult.cbCardCount);

				//切换用户
				m_wCurrentUser=(wChairID+1)%m_wPlayerCount;

				//构造数据
				CMD_S_ThrowResult ThrowResult;
				ThrowResult.wOutCardUser=wChairID;
				ThrowResult.wCurrentUser=m_wCurrentUser;
				ThrowResult.cbThrowCardCount=cbCardCount;
				ThrowResult.cbResultCardCount=OutCardResult.cbCardCount;
				CopyMemory(&ThrowResult.cbCardDataArray[0],cbCardData,sizeof(BYTE)*cbCardCount);
				CopyMemory(&ThrowResult.cbCardDataArray[cbCardCount],OutCardResult.cbResultCard,sizeof(BYTE)*OutCardResult.cbCardCount);

				//发送计算
				BYTE cbCardDataCount=ThrowResult.cbThrowCardCount+ThrowResult.cbResultCardCount;
				WORD wSendSize=sizeof(ThrowResult)-sizeof(ThrowResult.cbCardDataArray)+cbCardDataCount*sizeof(BYTE);

				//发送数据
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_THROW_RESULT,&ThrowResult,wSendSize);
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_THROW_RESULT,&ThrowResult,wSendSize);

				return true;
			}
			
		}
		
	}
	else
	{
		//数目判断
		ASSERT(cbCardCount==m_cbOutCardCount[m_wFirstOutUser]);
		if (cbCardCount!=m_cbOutCardCount[m_wFirstOutUser]) return false;

		//出牌效验
		if (m_GameLogic.EfficacyOutCard(cbCardData,cbCardCount,m_cbOutCardData[m_wFirstOutUser],
			m_cbOutCardCount[m_wFirstOutUser],m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false)
		{
			ASSERT(FALSE);
			return false;
		}
	}

	//删除扑克
	if (m_GameLogic.RemoveCard(cbCardData,cbCardCount,m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false) return false;
	m_cbHandCardCount[wChairID]-=cbCardCount;

	//出牌记录
	m_cbOutCardCount[wChairID]=cbCardCount;
	CopyMemory(m_cbOutCardData[wChairID],cbCardData,cbCardCount);

	//切换用户
	m_wCurrentUser=(wChairID+1)%m_wPlayerCount;
	if (m_wCurrentUser==m_wFirstOutUser) m_wCurrentUser=INVALID_CHAIR;

	//构造数据
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbCardCount=cbCardCount;
	OutCard.wCurrentUser=m_wCurrentUser;
	CopyMemory(OutCard.cbCardData,cbCardData,cbCardCount*sizeof(BYTE));

	//发送数据
	WORD wSendSize=sizeof(OutCard)-sizeof(OutCard.cbCardData)+OutCard.cbCardCount*sizeof(BYTE);
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);

	//一轮结算
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//判断胜者
		m_wTurnWinner=m_GameLogic.CompareCardArray(m_cbOutCardData,m_cbOutCardCount[0],m_wFirstOutUser);

		//计算得分
		if ((m_wTurnWinner!=m_wBankerUser)&&((m_wTurnWinner+2)%m_wPlayerCount!=m_wBankerUser))
		{
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				BYTE * pcbScoreCard=&m_cbScoreCardData[m_cbScoreCardCount];
				BYTE cbMaxCount=CountArray(m_cbScoreCardData)-m_cbScoreCardCount;
				m_cbScoreCardCount+=m_GameLogic.GetScoreCard(m_cbOutCardData[i],m_cbOutCardCount[i],pcbScoreCard,cbMaxCount);
			}
		}

		//设置变量
		if (m_cbHandCardCount[m_wTurnWinner]!=0)
		{
			m_wFirstOutUser=m_wTurnWinner;
			ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
			ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));
		}

		//用户切换
		m_wCurrentUser=(m_cbHandCardCount[m_wTurnWinner]==0)?INVALID_CHAIR:m_wTurnWinner;

		//构造数据
		CMD_TurnBalance TurnBalance;
		TurnBalance.wTurnWinner=m_wTurnWinner;
		TurnBalance.wCurrentUser=m_wCurrentUser;

		//发送数据
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TURN_BALANCE,&TurnBalance,sizeof(TurnBalance));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TURN_BALANCE,&TurnBalance,sizeof(TurnBalance));
	}

	//结束判断
	if (m_wCurrentUser==INVALID_CHAIR) OnEventGameEnd(m_wTurnWinner,NULL,GER_NORMAL);

	return true;
}

//派发扑克
bool CTableFrameSink::DispatchUserCard()
{
	//混乱扑克
	BYTE cbRandCard[CELL_PACK*2];
	m_GameLogic.RandCardList(cbRandCard,CELL_PACK*m_cbPackCount);

	//用户扑克
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_cbHandCardCount[i]=m_GameLogic.GetDispatchCount();
		CopyMemory(&m_cbHandCardData[i],&cbRandCard[i*m_cbHandCardCount[i]],sizeof(BYTE)*m_cbHandCardCount[i]);
	}

	//底牌扑克
	m_cbConcealCount=CELL_PACK*m_cbPackCount-m_wPlayerCount*m_GameLogic.GetDispatchCount();
	CopyMemory(m_cbConcealCard,&cbRandCard[CELL_PACK*m_cbPackCount-m_cbConcealCount],sizeof(BYTE)*m_cbConcealCount);

	//发送开始
	CMD_S_SendCard SendCard;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//效验状态
		ASSERT(m_cbHandCardCount[i]<=CountArray(SendCard.cbCardData));
		if (m_cbHandCardCount[i]>CountArray(SendCard.cbCardData)) return false;

		//拷贝扑克
		SendCard.cbCardCount=m_cbHandCardCount[i];
		CopyMemory(SendCard.cbCardData,m_cbHandCardData[i],sizeof(BYTE)*m_cbHandCardCount[i]);

		//发送统计
		WORD wHeadSize=sizeof(SendCard)-sizeof(SendCard.cbCardData);
		WORD wSendSize=wHeadSize+sizeof(SendCard.cbCardData[0])*SendCard.cbCardCount;

		//发送数据
		m_pITableFrame->SendTableData(i,SUB_S_SEND_CARD,&SendCard,wSendSize);
		m_pITableFrame->SendLookonData(i,SUB_S_SEND_CARD,&SendCard,wSendSize);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
