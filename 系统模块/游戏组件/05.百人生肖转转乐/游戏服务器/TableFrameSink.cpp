#include "StdAfx.h"
#include "TableFrameSink.h"

TCHAR g_szLog[1000];


//////////////////////////////////////////////////////////////////////////

//常量定义
#define SEND_COUNT					8									//发送次数

//索引定义
#define INDEX_PLAYER				0									//闲家索引
#define INDEX_BANKER				1									//庄家索引


#define IDI_PLACE_JETTON			1									//下注时间
#define IDI_OPEN_CARD				2									//开奖时间
#define IDI_GAME_FREE               3                                   //空闲时间


//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//游戏人数
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_TimeControl;	//开始模式

//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{
	//状态变量
	m_dwJettonTime=0L;

	//玩家成绩
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));

	//庄家变量
	m_lApplyBankerCondition = 100000000;
	ZeroMemory(&m_CurrentBanker, sizeof(m_CurrentBanker));
	m_cbBankerTimer=0;
	m_lBankerWinScore = 0;
	m_bCancelBanker=false;
	m_lMaxBankerTimer=0L;

	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;
	m_pITableFrameControl=NULL;


	//读取配置
	TCHAR szPath[MAX_PATH]=TEXT("");
	TCHAR ConfigFileName[MAX_PATH]=TEXT("");
	GetModuleFileName(AfxGetInstanceHandle(),szPath,MAX_PATH);
	PathRemoveFileSpec(szPath);
	_snprintf(ConfigFileName,sizeof(ConfigFileName),TEXT("%s\\shengxiaoConfig.ini"),szPath);

	m_lApplyBankerCondition = GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Score"), 10000000, ConfigFileName);
	m_lAreaLimitScore = ( INT64 )GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("AreaLimitScore"), 0, ConfigFileName);
	m_lMaxBankerTimer =	( INT64 )GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("BankerTimer"), 0, ConfigFileName);

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

	//控制接口
	m_pITableFrameControl=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrameControl);
	if (m_pITableFrameControl==NULL) return false;

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//配置参数
	TCHAR   buffer[MAX_PATH];
	if (GetCurrentDirectory(sizeof(buffer), buffer))
	{
		CString strIniFileName;

		CString szDir = buffer;
		if(szDir.Right(1) != "\\")	szDir += "\\";
		szDir += "BaccaratConfig.ini";
		strIniFileName= szDir;
	}

	return true;
}

//复位桌子
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//玩家成绩
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
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
	memset( m_nUserBet , 0 , sizeof(m_nUserBet) );
	memset( m_ScoreInfo , 0 , sizeof(m_ScoreInfo));
	m_bneedWriteTable = FALSE;

	IServerUserItem *pServerItem = m_pITableFrame->GetServerUserItem(m_CurrentBanker.wChairID);
	m_nMaxCanBet = m_CurrentBanker.lUserScore;
	if(pServerItem)
	{
		m_nMaxCanBet = pServerItem->GetUserData()->UserScoreInfo.lGameGold;
	}
	//开始下注
	CMD_S_GameStart GameStart;
	GameStart.cbBankerTimes = m_cbBankerTimer;
	if( m_CurrentBanker.dwUserID != 0 )
		GameStart.wBanker = m_CurrentBanker.wChairID;
	else
		GameStart.wBanker = INVALID_CHAIR;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));

	//////////////////////////////////////////////////////////////////////////
	//日志
	
	try
	{
		TCHAR szLog[1000];
		int  lenth = 0;
		IServerUserItem *pServerItem=NULL;

		pServerItem = m_pITableFrame->GetServerUserItem(m_CurrentBanker.wChairID);
		if( pServerItem )
		{
			lenth += sprintf( szLog+lenth ,"\n庄家ID:%d|金币:%I64d",pServerItem->GetUserID() , pServerItem->GetUserScore()->lGameGold );
		}

		for ( int i=0; i<GAME_PLAYER ;i++ )
		{		
			pServerItem = m_pITableFrame->GetServerUserItem(i);
			if( pServerItem == NULL ) continue;
			lenth += sprintf(  szLog + lenth , "ID:%d|金币:%I64d\n" , pServerItem->GetUserID() , pServerItem->GetUserScore()->lGameGold  );
		}

		SYSTEMTIME st;
		CString strDate;
		GetLocalTime(&st);
		strDate.Format("\\Shengxiao%2d-%2d.txt",st.wMonth,st.wDay);

		TCHAR szPath[MAX_PATH]=TEXT("");
		GetModuleFileName(AfxGetInstanceHandle(),szPath,MAX_PATH);
		PathRemoveFileSpec(szPath);
		strDate=szPath+strDate;

		m_strFileName = strDate;
		FILE *pFile = fopen( m_strFileName , "a+" );
		if( pFile )
		{
			fwrite( ( void *)(&szLog) ,sizeof(TCHAR),lenth ,pFile );
			fclose(pFile);
		}

	}
	catch(...)
	{
		CWnd wnd;
		wnd.MessageBox("写分日志异常1");
	}
	//////////////////////////////////////////////////////////////////////////
	return true;
}

//游戏结束
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束
		{
			if( m_bneedWriteTable )
			{
				m_bneedWriteTable = FALSE;
				m_pITableFrame->WriteTableScore(m_ScoreInfo,GAME_PLAYER,0);
				//for ( int i=0;i<GAME_PLAYER;i++ )
				//{
				//	if( m_pITableFrame->GetServerUserItem(i) )
				//		m_pITableFrame->WriteUserScore( i , m_ScoreInfo[i].lScore , 0 , m_ScoreInfo[i].ScoreKind );
				//}

				memset( m_ScoreInfo , 0 , sizeof(m_ScoreInfo));
			}
			//结束游戏
			m_pITableFrame->ConcludeGame();
			return true;
		}
	case GER_USER_LEFT:		//用户离开
		{

			if( m_pITableFrame->GetGameStatus() == GS_BACCARAT_JETTON )
			{
				//逃跑的是
				if( m_CurrentBanker.wChairID == wChairID )
				{	
					m_bBankerLeft = true;

					//累计闲家所有的计分
					INT64 total=0;
					for ( int i=0;i<GAME_PLAYER;i++ )
					{
						for (int j=0;j<MAX_AREA;j++)
						{
							if ( m_nUserBet[i][j] > 0 )
							{
								total+=m_nUserBet[i][j];
							}
						}
					}

					memset( m_nUserBet,0,sizeof(m_nUserBet) );

					IServerUserItem * pServerItem = m_pITableFrame->GetServerUserItem(wChairID);
					if( pServerItem )
					{
						m_pITableFrame->WriteUserScore( wChairID , -total , 0 , enScoreKind_Lost );
						m_bneedWriteTable = FALSE;
						memset( m_ScoreInfo , 0 , sizeof(m_ScoreInfo));

						TCHAR szLog[50];
						int	lenth = sprintf( szLog , "庄家逃跑ID:%d|扣除%I64d" ,pServerItem->GetUserID(), -total );

						FILE *pFile = fopen( m_strFileName , "a+" );
						if( pFile )
						{
							fwrite( ( void *)(&szLog) ,sizeof(TCHAR),lenth ,pFile );
							fclose(pFile);
						}

					}
				}
				else
				{
					INT64 total=0;
					for (int i=0;i<MAX_AREA;i++)
					{
						if ( m_nUserBet[wChairID][i] > 0 )
						{
							total += m_nUserBet[wChairID][i];
							m_nUserBet[wChairID][i] = 0;
						}
					}

					m_ScoreInfo[wChairID].lScore = 0;
					m_ScoreInfo[wChairID].lRevenue = 0;
					m_ScoreInfo[wChairID].ScoreKind = enScoreKind_Lost;

					IServerUserItem * pServerItem = m_pITableFrame->GetServerUserItem(wChairID);
					if( pServerItem )
					{
						m_pITableFrame->WriteUserScore( wChairID , -total , 0 , enScoreKind_Lost );
						TCHAR szLog[50];
						int	lenth = sprintf( szLog , "闲家逃跑ID:%d|扣除%I64d" ,pServerItem->GetUserID(), -total );

						FILE *pFile = fopen( m_strFileName , "a+" );
						if( pFile )
						{
							fwrite( ( void *)(&szLog) ,sizeof(TCHAR),lenth ,pFile );
							fclose(pFile);
						}

					}

					return true;
				}
			}
			
			//可以正常结算游戏
			if ( m_pITableFrame->GetGameStatus() == GS_BACCARAT_OPENCARD )
			{
				if( m_bneedWriteTable )
				{
					m_bneedWriteTable = FALSE;
					m_pITableFrame->WriteTableScore(m_ScoreInfo,GAME_PLAYER,0);
					//for ( int i=0;i<GAME_PLAYER;i++ )
					//{
					//	if( m_pITableFrame->GetServerUserItem(i) )
					//		m_pITableFrame->WriteUserScore( i , m_ScoreInfo[i].lScore , 0 , m_ScoreInfo[i].ScoreKind );
					//}

					memset( m_ScoreInfo , 0 , sizeof(m_ScoreInfo));
				}
			}

			IServerUserItem *pServerItem = m_pITableFrame->GetServerUserItem( wChairID );
			tagServerUserData *pServerData = pServerItem->GetUserData();
			if( pServerData )
			{
				OnUserApplyBanker( pServerData , false );
				m_CurrentBanker.dwUserID = 0;
				ChangeBanker();
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
	case GS_BACCARAT_FREE:			//空闲状态
		{
			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//全局信息
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusFree.cbTimeLeave=(BYTE)(TIME_GAME_FREE-__min(dwPassTime,TIME_GAME_FREE));
			StatusFree.lAreaLimitScore = m_lAreaLimitScore;
			StatusFree.lMeMaxScore = pIServerUserItem->GetUserScore()->lScore;

			//庄家信息
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusFree.cbBankerTime = m_cbBankerTimer;
			StatusFree.lBankerWin = m_lBankerWinScore;

			if (m_CurrentBanker.dwUserID != 0) 
			{
				StatusFree.cbBankerTime = m_cbBankerTimer;
				StatusFree.lCurrentBankerScore = m_lBankerWinScore;
				StatusFree.wCurrentBankerChairID = m_CurrentBanker.wChairID;
				StatusFree.lBankerTreasure = m_CurrentBanker.lUserScore;
			}
			else StatusFree.wCurrentBankerChairID = INVALID_CHAIR;
			
			//发送申请者
			SendApplyUser(pIServerUserItem);

			//发送游戏记录
			SendGameRecord(pIServerUserItem);

			//限制提示
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄条件为：%I64d,区域限制为：%I64d"),m_lApplyBankerCondition,
				m_lAreaLimitScore);

			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);

			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_BACCARAT_JETTON:		//下注状态
	case GS_BACCARAT_OPENCARD:
		{
			//构造数据
			CMD_S_StatusJetton StatusJetton;
			ZeroMemory(&StatusJetton,sizeof(StatusJetton));

			//全局信息
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusJetton.cbTimeLeave=(BYTE)(TIME_PLACE_JETTON-__min(dwPassTime,TIME_PLACE_JETTON));

			StatusJetton.lAreaLimitScore=m_lAreaLimitScore;
			StatusJetton.cbBankerTime = m_cbBankerTimer;
			StatusJetton.lBankerWin = m_lBankerWinScore;

			//庄家信息
			StatusJetton.lApplyBankerCondition = m_lApplyBankerCondition;

			//下注信息
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
			//	StatusJetton.lMeMaxScore=pIServerUserItem->GetUserScore()->lScore;
			}

			if (m_CurrentBanker.dwUserID != 0) 
			{
				StatusJetton.cbBankerTime = m_cbBankerTimer;
				StatusJetton.lCurrentBankerScore = m_lBankerWinScore;
				StatusJetton.wCurrentBankerChairID = m_CurrentBanker.wChairID;
				StatusJetton.lBankerTreasure = m_CurrentBanker.lUserScore;
			}
			else StatusJetton.wCurrentBankerChairID=INVALID_CHAIR;

			//发送申请者
			SendApplyUser(pIServerUserItem);

			//发送游戏记录
			SendGameRecord(pIServerUserItem);

			//限制提示
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄条件为：%I64d,区域限制为：%I64d"),m_lApplyBankerCondition,
				m_lAreaLimitScore);

			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusJetton,sizeof(StatusJetton));
		}
	//case GS_BACCARAT_OPENCARD:
	//	{
	//		
	//		return true;
	//	}
	}

	return false;
}

//定时器事件
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
try
{
	switch (wTimerID)
	{
	case IDI_PLACE_JETTON:		//下注时间
		{
			m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
			m_pITableFrame->SetGameStatus(GS_BACCARAT_JETTON);

			//开始游戏
			m_bBankerLeft = false;
			m_pITableFrameControl->StartGame();
			m_cbResult = 0xff;
			memset( &m_ScoreInfo,0,sizeof(m_ScoreInfo) );

			//设置时间
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_OPEN_CARD,TIME_PLACE_JETTON*1000,1,0L);
			return true;
		}
	case IDI_OPEN_CARD:			//开奖游戏
		{
			//读取配置
			TCHAR szPath[MAX_PATH]=TEXT("");
			TCHAR ConfigFileName[MAX_PATH]=TEXT("");
			GetModuleFileName(AfxGetInstanceHandle(),szPath,MAX_PATH);
			PathRemoveFileSpec(szPath);
			_snprintf(ConfigFileName,sizeof(ConfigFileName),TEXT("%s\\shengxiaoConfig.ini"),szPath);
			
			BYTE cbTemp = GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Cheat"), 0 , ConfigFileName );
			m_pITableFrame->KillGameTimer(IDI_OPEN_CARD);
			m_pITableFrame->SetGameStatus(GS_BACCARAT_OPENCARD);

			CMD_S_StatusOpenCard OpenCard;
			OpenCard.cbAnimalPos  = rand()%36;		//0-35
			m_cbResult = (OpenCard.cbAnimalPos)%12;	//0-11

			if( cbTemp != 0 && cbTemp>=1 && cbTemp<=12 )
			{
				m_cbResult = OpenCard.cbAnimalPos = cbTemp - 1;
			}

			m_pITableFrame->SendTableData( INVALID_CHAIR , SUB_S_RESLUT , &OpenCard , sizeof(OpenCard));
			m_pITableFrame->SendLookonData( INVALID_CHAIR , SUB_S_RESLUT , &OpenCard , sizeof(OpenCard));

			//计算结果
			memset( &m_ScoreInfo,0,sizeof(m_ScoreInfo) );
		
			try
			{
				if( m_pITableFrame->GetServerUserItem(m_CurrentBanker.wChairID) && m_CurrentBanker.dwUserID != 0 && m_bBankerLeft == false )
				{
					//////////////////////////////////////////////////////////////////////////
					IServerUserItem *pServerItem = NULL;
					TCHAR szLog[200*GAME_PLAYER];
					int  lenth = 0;
					TCHAR *pszResult[]={"大","小"};
					lenth += sprintf( szLog + lenth ,"计分本局结果:%d|%s\n", m_cbResult , pszResult[m_cbResult%2] );
					//////////////////////////////////////////////////////////////////////////

					INT64	total=0;
					for( int i=0 ; i<GAME_PLAYER ;i++ )
					{	
						pServerItem = m_pITableFrame->GetServerUserItem(i);
						if( pServerItem==NULL||m_CurrentBanker.wChairID == i )	continue;

						for ( int j=0;j<MAX_AREA;j++ )
						{
							//此门没有下注
							if( m_nUserBet[i][j]<=0 )	continue;

							if ( j==12 || j==13 )
							{
								//大
								if( j%2==m_cbResult%2 )
								{
									m_ScoreInfo[i].lScore += m_nUserBet[i][j];
									m_ScoreInfo[m_CurrentBanker.wChairID].lScore -= m_nUserBet[i][j];

									lenth += sprintf( szLog + lenth , "ID:%d|大小赢:%I64d\n",
										pServerItem->GetUserID(),m_nUserBet[i][j]);
								}
								else
								{
									m_ScoreInfo[i].lScore -= m_nUserBet[i][j];
									m_ScoreInfo[m_CurrentBanker.wChairID].lScore += m_nUserBet[i][j];

									lenth += sprintf( szLog + lenth , "ID:%d|大小输:%I64d\n",
										pServerItem->GetUserID(),-m_nUserBet[i][j]);
								}

								continue;
							}

							if( j==m_cbResult )
							{
								m_ScoreInfo[i].lScore += m_nUserBet[i][j]*11;
								m_ScoreInfo[m_CurrentBanker.wChairID].lScore -= m_nUserBet[i][j]*11;

								lenth += sprintf( szLog + lenth , "ID:%d|压中赢:%I64d\n",
									pServerItem->GetUserID(),m_nUserBet[i][j]*11);
							}
							else
							{
								m_ScoreInfo[i].lScore -= m_nUserBet[i][j];
								m_ScoreInfo[m_CurrentBanker.wChairID].lScore += m_nUserBet[i][j];

								lenth += sprintf( szLog + lenth , "ID:%d|没压中赢:%I64d\n",
									pServerItem->GetUserID(),-m_nUserBet[i][j]);
							}
						}

						if( m_ScoreInfo[i].lScore>0 )
						{
							m_ScoreInfo[i].ScoreKind = enScoreKind_Win;
						}
						else if (m_ScoreInfo[i].lScore<0)
						{
							m_ScoreInfo[i].ScoreKind = enScoreKind_Lost;
						}
						else
						{
							m_ScoreInfo[i].ScoreKind = enScoreKind_Draw;
						}


						lenth += sprintf( szLog + lenth , "ID:%d|本局:%I64d\n",
							pServerItem->GetUserID(),m_ScoreInfo[i].lScore);
					}

					pServerItem = m_pITableFrame->GetServerUserItem(m_CurrentBanker.wChairID);

					lenth += sprintf( szLog + lenth , "庄家ID:%d|本局:%I64d\n",
						pServerItem->GetUserID(),m_ScoreInfo[m_CurrentBanker.wChairID].lScore);

					m_bneedWriteTable = TRUE;

					CMD_S_UserScore UserScore;
					//
					for (int i=0;i<GAME_PLAYER;i++)
					{
						IServerUserItem *pServerItem = m_pITableFrame->GetServerUserItem(i);
						if( pServerItem == NULL ) continue ;
						UserScore.nScore = m_ScoreInfo[i].lScore;
						UserScore.nBankerScore = m_ScoreInfo[m_CurrentBanker.wChairID].lScore;
						m_pITableFrame->SendTableData( i , SUB_S_GAME_SCORE , &UserScore,sizeof(UserScore) );
					}

					//庄家成绩
					m_lBankerWinScore += m_ScoreInfo[m_CurrentBanker.wChairID].lScore;

					FILE *pFile = fopen( m_strFileName , "a+" );
					if( pFile )
					{
						fwrite( ( void *)(&szLog) ,sizeof(TCHAR),lenth ,pFile );
						fclose(pFile);
					}

				}

			}
			catch(...)
			{
				CWnd wnd;
				wnd.MessageBox("写分异常2");
			}
			//设置时间
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_GAME_FREE,TIME_OPEN_CARD*1000,1,0L);

			return true;
		}
	case IDI_GAME_FREE:
		{
			m_pITableFrame->KillGameTimer(IDI_GAME_FREE);
			//结束游戏
			OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);

			if( m_CurrentBanker.dwUserID != 0 )
				m_cbBankerTimer++;
			
			m_pITableFrame->SendTableData( INVALID_CHAIR , SUB_S_FREE  );
			m_pITableFrame->SendLookonData( INVALID_CHAIR , SUB_S_FREE );

			//设置状态
			m_pITableFrame->SetGameStatus(GS_BACCARAT_FREE);

			//下庄判断
			if (m_bCancelBanker && m_CurrentBanker.dwUserID != 0)
			{		
				//获取玩家
				IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(m_CurrentBanker.wChairID);

				//重置变量
				m_cbBankerTimer = 0;
				m_lBankerWinScore=0;
				ZeroMemory(&m_CurrentBanker, sizeof(m_CurrentBanker));
				m_bCancelBanker=false;

				//发送消息
				SendChangeBankerMsg();

				//删除庄家
				if (pServerUserItem) OnUserApplyBanker(pServerUserItem->GetUserData(), false);
			}


			//轮换庄家
			ChangeBanker();

			//切换判断
			if (m_cbBankerTimer == 0)
			{
				//发送消息
				SendChangeBankerMsg();
			}

			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,TIME_GAME_FREE*1000,1,0L);

			return true;
		}
	}
}
catch(...)
{
	CWnd wnd;
	wnd.MessageBox("异常1");
}

	return false;
}

//游戏消息处理
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{

try
{
	switch (wSubCmdID)
	{
	case SUB_C_PLACE_JETTON:		//用户加注
		{

			if( pIServerUserItem->GetUserStatus() != US_PLAY )	return true;

			if( m_pITableFrame->GetGameStatus() != GS_BACCARAT_JETTON )		return true;


			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_PlaceJetton));
			if (wDataSize!=sizeof(CMD_C_PlaceJetton))
			{
				ASSERT(FALSE);
				return false;
			}

			//消息处理
			CMD_C_PlaceJetton * pPlaceJetton=(CMD_C_PlaceJetton *)pDataBuffer;
			return OnUserPlaceJetton(pIServerUserItem->GetChairID(),pPlaceJetton->cbViewJettonArea,pPlaceJetton->cbAnimal,pPlaceJetton->lJettonScore);
		}
	case SUB_C_APPLY_BANKER:		//申请做庄
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_C_ApplyBanker))
			{
				ASSERT(FALSE);
				return false;
			}

			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus==US_LOOKON)
			{
				ASSERT(FALSE);
				return false;
			}

			//消息处理
			CMD_C_ApplyBanker * pApplyBanker=(CMD_C_ApplyBanker *)pDataBuffer;
			return OnUserApplyBanker(pUserData, pApplyBanker->bApplyBanker);	
		}
	}
}
catch(...)
{
	CWnd wnd;
	wnd.MessageBox("异常2");
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
		m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,1000,1,NULL);
	}

	return true;
}

//用户起来
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//记录成绩
	if (bLookonUser==false)
	{
		//状态判断
		if (m_pITableFrame->GetGameStatus() != GS_BACCARAT_FREE)
		{
			//变量定义
			LONG lScore=0;
			LONG lRevenue=0;
			enScoreKind ScoreKind;

			//写入积分
			if (lScore!=0L)
			{
				//写入积分
				ScoreKind=enScoreKind_Flee;
				m_pITableFrame->WriteUserScore(pIServerUserItem, lScore,lRevenue, ScoreKind);
			}
		}

		//存在判断
		for (INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArrary.GetCount(); ++nUserIdx)
		{
			tagApplyUserInfo ApplyUserInfo = m_ApplyUserArrary[nUserIdx];

			if (ApplyUserInfo.dwUserID == pIServerUserItem->GetUserID())
			{
				//删除玩家
				m_ApplyUserArrary.RemoveAt(nUserIdx);

				//构造变量
				CMD_S_ApplyBanker ApplyBanker;
				CopyMemory(ApplyBanker.szAccount, pIServerUserItem->GetAccounts(), sizeof(ApplyBanker.szAccount));
				ApplyBanker.lScore = pIServerUserItem->GetUserScore()->lScore;
				ApplyBanker.bApplyBanker = false;

				//发送消息
				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
				m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

			}
		}
	}

	if(pIServerUserItem && pIServerUserItem->GetUserData()->dwUserID == m_CurrentBanker.dwUserID )
	{
		m_CurrentBanker.dwUserID = 0;
		ChangeBanker();
	}

	return true;
}

//加注事件
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbViewJettonArea, BYTE cbAnimal , INT64 lJettonScore)
{

	if( m_pITableFrame->GetGameStatus() != GS_BACCARAT_JETTON )	return true;


	if ( cbViewJettonArea>=MAX_AREA || wChairID == INVALID_CHAIR || wChairID >= GAME_PLAYER )
	{
		return false;
	}

	if( m_CurrentBanker.dwUserID == 0 )		return true;
	
	IServerUserItem *pUserItem = m_pITableFrame->GetServerUserItem(wChairID);
	if ( !pUserItem )	return false;

	if( pUserItem->GetUserStatus() != US_PLAY )	return true;

	if(	m_nMaxCanBet <= 0 )
	{
		m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
		m_pITableFrame->SetGameStatus(GS_BACCARAT_OPENCARD);
		m_pITableFrame->SetGameTimer(IDI_OPEN_CARD,3*1000,1,0L);
		return true;
	}



	//庄家逃跑不接受下注
	if( m_bBankerLeft || m_pITableFrame->GetServerUserItem(m_CurrentBanker.wChairID) == NULL ) 
	{
		return true;
	}

	if( cbViewJettonArea==12||cbViewJettonArea==13 )
	{
		if ( m_nMaxCanBet - lJettonScore < 0 )
		{
			return true;
		}
		//two
		m_nMaxCanBet = m_nMaxCanBet - lJettonScore;
	}
	else
	{	
		if ( m_nMaxCanBet - 11*lJettonScore < 0 )
		{
			return true;
		}
		//
		m_nMaxCanBet = m_nMaxCanBet - 11*lJettonScore;
	}

	LONGLONG	total=0;
	for ( int i=0;i<MAX_AREA;i++ )
	{
		total += m_nUserBet[wChairID][i];
	}

	//超出自己最大财富
	if ( pUserItem->GetUserScore()->lGameGold < total + lJettonScore )
	{
		return true;
	}

	m_nUserBet[wChairID][cbViewJettonArea] += lJettonScore;

	CMD_S_PlaceJetton	PlaceJetton;
	PlaceJetton.wChairID = wChairID;
	PlaceJetton.lJettonScore = lJettonScore;
	PlaceJetton.cbJettonArea = cbAnimal;
	
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

	return true;
}



//申请庄家
bool CTableFrameSink::OnUserApplyBanker(tagServerUserData *pUserData, bool bApplyBanker)
{
//	if( m_pITableFrame->GetGameStatus() == GS_BACCARAT_JETTON )	return true;

	//合法判断
	if (bApplyBanker && pUserData->UserScoreInfo.lGameGold < m_lApplyBankerCondition) 
		return true;

	//保存玩家
	if (bApplyBanker)
	{
		//存在判断
		for (INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArrary.GetCount(); ++nUserIdx)
		{
			tagApplyUserInfo ApplyUserInfo = m_ApplyUserArrary[nUserIdx];
			if (ApplyUserInfo.dwUserID == pUserData->dwUserID) return true;
		}

		//构造信息
		tagApplyUserInfo ApplyUserInfo;
		ApplyUserInfo.dwUserID = pUserData->dwUserID;
		ApplyUserInfo.lUserScore = pUserData->UserScoreInfo.lGameGold;
		ApplyUserInfo.wChairID = pUserData->wChairID;

		//插入玩家
		INT_PTR nUserCount = m_ApplyUserArrary.GetCount();
		m_ApplyUserArrary.InsertAt(nUserCount, ApplyUserInfo);

		//构造变量
		CMD_S_ApplyBanker ApplyBanker;
		CopyMemory(ApplyBanker.szAccount, pUserData->szAccounts, sizeof(ApplyBanker.szAccount));
		ApplyBanker.lScore = pUserData->UserScoreInfo.lGameGold+pUserData->lStorageScore;
		ApplyBanker.bApplyBanker = true;

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

		if( m_CurrentBanker.dwUserID == 0 &&  m_pITableFrame->GetGameStatus() == GS_BACCARAT_FREE  )
		{
			ChangeBanker();
		}

		return true;
	}
	else	//取消申请
	{
		//存在判断
		for (INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArrary.GetCount(); ++nUserIdx)
		{
			tagApplyUserInfo ApplyUserInfo = m_ApplyUserArrary[nUserIdx];
			if (ApplyUserInfo.dwUserID == pUserData->dwUserID)
			{
				//当前判断
				if (m_CurrentBanker.dwUserID == ApplyUserInfo.dwUserID)
				{
					m_bCancelBanker = true;
					if( m_pITableFrame->GetGameStatus() != GS_BACCARAT_FREE )
						continue;
				}
 
				//删除玩家
				m_ApplyUserArrary.RemoveAt(nUserIdx);

				//构造变量
				CMD_S_ApplyBanker ApplyBanker;
				CopyMemory(ApplyBanker.szAccount, pUserData->szAccounts, sizeof(ApplyBanker.szAccount));
				ApplyBanker.lScore = pUserData->UserScoreInfo.lGameGold;
				ApplyBanker.bApplyBanker = false;

				//发送消息
				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
				m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

				return true;
			}
		}

		if( m_CurrentBanker.dwUserID == 0 &&  m_pITableFrame->GetGameStatus() == GS_BACCARAT_FREE  )
		{
			ChangeBanker();
		}


		return true;
	}

}

//发送庄家
void CTableFrameSink::SendApplyUser(IServerUserItem *pRcvServerUserItem)
{
	for (INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArrary.GetCount(); ++nUserIdx)
	{
		tagApplyUserInfo ApplyUserInfo = m_ApplyUserArrary[nUserIdx];

		//获取玩家
		IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(ApplyUserInfo.wChairID);
		if (! pServerUserItem) continue;
		
		if( m_CurrentBanker.dwUserID == pServerUserItem->GetUserData()->dwUserID )	continue;

		//构造变量
		CMD_S_ApplyBanker ApplyBanker;
		CopyMemory(ApplyBanker.szAccount, pServerUserItem->GetAccounts(), sizeof(ApplyBanker.szAccount));
		ApplyBanker.lScore = pServerUserItem->GetUserScore()->lScore;
		ApplyBanker.bApplyBanker = true;

		//发送消息
		m_pITableFrame->SendUserData(pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	}
}

//游戏记录
void CTableFrameSink::SendGameRecord(IServerUserItem *pIServerUserItem)
{

}

//更换庄家
void CTableFrameSink::ChangeBanker()
{
	//读取配置
	TCHAR szPath[MAX_PATH]=TEXT("");
	TCHAR ConfigFileName[MAX_PATH]=TEXT("");
	GetModuleFileName(AfxGetInstanceHandle(),szPath,MAX_PATH);
	PathRemoveFileSpec(szPath);
	_snprintf(ConfigFileName,sizeof(ConfigFileName),TEXT("%s\\shengxiaoConfig.ini"),szPath);

	//读取分数
	m_lApplyBankerCondition = GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Score"), 10000000, ConfigFileName);
	m_lAreaLimitScore = GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("AreaLimitScore"), 1000000000, ConfigFileName);
	m_lMaxBankerTimer = GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("BankerTimer"), 20, ConfigFileName);

	//轮庄判断
	if (m_CurrentBanker.dwUserID != 0)
	{
try
{
		//轮换判断
		bool bChangeBanker = false;

		IServerUserItem *pBankerUserItem = m_pITableFrame->GetServerUserItem(m_CurrentBanker.wChairID);
		if( pBankerUserItem == NULL )
		{	
			for (INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArrary.GetCount(); ++nUserIdx)
			{
				if ( m_ApplyUserArrary[nUserIdx].dwUserID == m_CurrentBanker.dwUserID )
				{
					m_ApplyUserArrary.RemoveAt(nUserIdx);
					break;
				}
			}

			m_CurrentBanker.dwUserID = 0;
			ChangeBanker();
			return ;
		}
		INT64 lBankerScore = pBankerUserItem->GetUserScore()->lGameGold;

		if( m_lApplyBankerCondition > lBankerScore   )
			bChangeBanker = true;

		//次数判断
		if (bChangeBanker || m_cbBankerTimer > m_lMaxBankerTimer)
		{
			m_cbBankerTimer = 0;
			m_lBankerWinScore=0;
			m_bCancelBanker = false;

			//客户端删除
			IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(m_CurrentBanker.wChairID);
			OnUserApplyBanker(pServerUserItem->GetUserData(), false);

			//重新赋值
			ZeroMemory(&m_CurrentBanker, sizeof(m_CurrentBanker));
			while(! m_ApplyUserArrary.IsEmpty())
			{
				m_CurrentBanker = m_ApplyUserArrary[0];
				//合法判断
				if (m_CurrentBanker.lUserScore < m_lApplyBankerCondition)
				{
					ZeroMemory(&m_CurrentBanker, sizeof(m_CurrentBanker));

					//获取玩家
					IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(m_ApplyUserArrary[0].wChairID);
					if ( pServerUserItem ) OnUserApplyBanker(pServerUserItem->GetUserData(), false);
				}
				else
					break;
			}
		}
}
catch(...)
{
	CWnd wnd;
	wnd.MessageBox("异常3");
}
	}
	else if (0 < m_ApplyUserArrary.GetCount())
	{
		while(!m_ApplyUserArrary.IsEmpty())
		{
			m_CurrentBanker = m_ApplyUserArrary[0];
			//合法判断
			if (m_CurrentBanker.lUserScore < m_lApplyBankerCondition)
			{
				ZeroMemory(&m_CurrentBanker, sizeof(m_CurrentBanker));
				//获取玩家
				IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(m_ApplyUserArrary[0].wChairID);
				if (pServerUserItem) 
					OnUserApplyBanker(pServerUserItem->GetUserData(), false);
			}
			else
				break;
		}
		m_cbBankerTimer = 0;
		m_lBankerWinScore=0;
		m_bCancelBanker = false;
	}

	//开始下注
	CMD_S_GameStart GameStart;
	GameStart.cbBankerTimes = m_cbBankerTimer;
	if( m_CurrentBanker.dwUserID != 0 )
		GameStart.wBanker = m_CurrentBanker.wChairID;
	else
		GameStart.wBanker = INVALID_CHAIR;

	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CURRENT_BANKER,&GameStart,sizeof(GameStart));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CURRENT_BANKER,&GameStart,sizeof(GameStart));
	
} 
//轮换庄家
void CTableFrameSink::SendChangeBankerMsg()
{
	CMD_S_ChangeBanker ChangeBanker;

	for (WORD i = 0; i < GAME_PLAYER; ++i)
	{
		IServerUserItem *pSeverUserItem = m_pITableFrame->GetServerUserItem(i);
		if (pSeverUserItem==NULL) continue;

		ZeroMemory(&ChangeBanker, sizeof(ChangeBanker));
		if (m_CurrentBanker.dwUserID != 0)
		{
			ChangeBanker.wChairID = m_CurrentBanker.wChairID;
			ChangeBanker.lBankerTreasure = m_CurrentBanker.lUserScore;
		}
		else
		{
			ChangeBanker.wChairID = INVALID_CHAIR;
		}
		ChangeBanker.lBankerScore = m_CurrentBanker.lUserScore;
		ChangeBanker.cbBankerTime = m_cbBankerTimer;

		//我的下注
		m_pITableFrame->SendTableData(i, SUB_S_CHANGE_BANKER, &ChangeBanker, sizeof(ChangeBanker));
		m_pITableFrame->SendLookonData(i, SUB_S_CHANGE_BANKER, &ChangeBanker, sizeof(ChangeBanker));
	}
}

//用户断线
bool __cdecl CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	return true;
}
//////////////////////////////////////////////////////////////////////////

