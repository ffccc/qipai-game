#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//常量定义
#define SEND_COUNT					8									//发送次数

//索引定义
#define INDEX_PLAYER				0									//闲家索引
#define INDEX_BANKER				1									//庄家索引

//下注时间
#define IDI_FREE					1									//空闲时间
#define TIME_FREE					6									//空闲时间

//游戏时间
#define IDI_GAME					2									//游戏时间
#define TIME_GAME					20									//游戏时间

//结束时间
#define IDI_GAME_END				12									//结束时间
#define TIME_GAME_END				12									//结束时间

//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//游戏人数
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_TimeControl;	//开始模式

//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink():m_pGameServiceOption(NULL)
{
	//下注信息
	m_lOddScore=0L;
	m_lEvenScore=0L;
	m_nSendCardCount = 0;
	
	//状态变量
	m_dwJettonTime=0L;

	//下注信息
	ZeroMemory(m_lUserOddScore,sizeof(m_lUserOddScore));
	ZeroMemory(m_lUserEvenScore,sizeof(m_lUserEvenScore));

	//玩家成绩
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));

	//扑克信息
	ZeroMemory(m_cbDice,sizeof(m_cbDice));

	//庄家变量
	m_lApplyBankerCondition = 100000000;
	ZeroMemory( &m_CurrentBanker, sizeof( m_CurrentBanker ) );
	m_cbBankerTimer=0;
	m_lBankerWinScore = 0;
	m_UserLeftScore=0;
	
	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;
	m_pITableFrameControl=NULL;
	m_pGameServiceOption = NULL;

	//游戏记录
	ZeroMemory(m_GameRecordArrary, sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;	
	m_nRecordLast=0;	

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
#ifdef __SPECIAL___
	QUERYINTERFACE(ITableUserActionEX,Guid,dwQueryVer);	
#endif
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
	bool blGetConfig = true;
	if(m_pGameServiceOption !=0)
	{
		blGetConfig = false;

	}
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	if(blGetConfig==false)
	{
		return true;
	}

	//设置文件名
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_snprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\OddEvenBattleConfig.ini"),szPath);

	LONGLONG lMaxScore=2147483647;
	TCHAR OutBuf[255];


	TCHAR Session[255];
	memset(Session,0,255);
	_snprintf(Session,sizeof(Session),TEXT("BankerCondition%d"),m_pGameServiceOption->wServerID);
	GetPrivateProfileString(Session,TEXT("Score"),"1000",OutBuf,255,m_szConfigFileName);
	_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_lApplyBankerCondition);

	BYTE cbIsHightRoom=GetPrivateProfileInt(Session, TEXT("IsHightRoom"), 1, m_szConfigFileName);
	m_IsHightRoom=cbIsHightRoom?true:false;

	if(m_IsHightRoom)
	{
		memset(OutBuf,0,255);
		GetPrivateProfileString(Session, TEXT("LowerBigBankerCondition"),"1500000000",OutBuf,255,m_szConfigFileName);
		_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_LowerBigBankerCondition);

	}else
	{
		memset(OutBuf,0,255);
		GetPrivateProfileString(Session, TEXT("LowerBigBankerCondition"),"500000000",OutBuf,255,m_szConfigFileName);
		_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_LowerBigBankerCondition);
	}

	memset(Session,0,255);
	_snprintf(Session,sizeof(Session),TEXT("ScoreLimit%d"),m_pGameServiceOption->wServerID);
	GetPrivateProfileString(Session,TEXT("AreaLimitScore"),"2147483647",OutBuf,255,m_szConfigFileName);
	_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_lAreaLimitScore);

	memset(OutBuf,0,255);
	GetPrivateProfileString(Session,TEXT("UserLimitScore"),"2147483647",OutBuf,255,m_szConfigFileName);
	_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_lUserLimitScore);

	return true;

}

//复位桌子
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//下注信息
	m_lOddScore=0L;
	m_lEvenScore=0L;
	m_UserLeftScore=0;

	//下注信息
	ZeroMemory(m_lUserOddScore,sizeof(m_lUserOddScore));
	ZeroMemory(m_lUserEvenScore,sizeof(m_lUserEvenScore));

	//玩家成绩
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));

	//扑克信息
	ZeroMemory(m_cbDice,sizeof(m_cbDice));

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
	m_pITableFrame->SetGameStatus(GS_WK_GAME);

	
	//变量定义
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));

	GameStart.cbGameTime = TIME_GAME;
	GameStart.wCurBankerID = m_CurrentBanker.wChairID;
	GameStart.wServerID = m_pGameServiceOption->wServerID;


	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));

	//轮换庄家
	ChangeBanker();

	//切换判断
	if ( m_cbBankerTimer == 0 )
	{
		//发送消息
		SendChangeBankerMsg();
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
			//结束消息
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			GameEnd.nBankerTime = m_cbBankerTimer;

			GameEnd.lBankerTotalScore = m_lBankerWinScore;

			//推断玩家
			DeduceWinner(GameEnd.cbWinner);			

			//写入积分
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserChairID);
				if ( pIServerUserItem == NULL ) continue;

				//胜利类型
				enScoreKind ScoreKind=(m_lUserWinScore[wUserChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;

				//写入积分
				if (m_lUserWinScore[wUserChairID]!=0L) m_pITableFrame->WriteUserScore(wUserChairID,m_lUserWinScore[wUserChairID], (LONG)m_lUserRevenue[wUserChairID], ScoreKind);

				//庄家判断
				if ( m_CurrentBanker.dwUserID == pIServerUserItem->GetUserID() ) m_CurrentBanker.lUserScore = pIServerUserItem->GetUserScore()->lScore;
			}

			//发送积分
			GameEnd.cbTimeLeave=TIME_FREE;	
			if ( m_CurrentBanker.dwUserID != 0 ) GameEnd.lBankerTreasure = m_CurrentBanker.lUserScore;
			for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserIndex);
				if ( pIServerUserItem == NULL ) continue;

				//发送消息					
				GameEnd.lMeMaxScore=pIServerUserItem->GetUserScore()->lScore;
				m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			//庄家判断
			if ( m_CurrentBanker.dwUserID != 0 )
			{
				//获取用户
				IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(m_CurrentBanker.wChairID);
				if (pIServerUserItem) 
				{
					//积分判断
					if ( pIServerUserItem->GetUserScore()->lScore < m_lApplyBankerCondition )
					{
						//玩家下庄
						OnUserApplyBanker( pIServerUserItem->GetUserData(), false, false );
						//提示消息
						TCHAR szTipMsg[128];
						_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("由于你的金币数少于坐庄必须金币数（%I64d）,你自动下庄！"),m_lApplyBankerCondition);
						m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);
					}
				}
			}

				//更新积分
			for ( INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArrary.GetCount(); ++nUserIdx )
			{
				tagApplyUserInfo &ApplyUserInfo = m_ApplyUserArrary[ nUserIdx ];
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(ApplyUserInfo.wChairID);
				if ( pIServerUserItem == NULL ) continue;
				ApplyUserInfo.lUserScore = pIServerUserItem->GetUserScore()->lScore;
			}

			//结束游戏
			m_pITableFrame->ConcludeGame();

			//移动下标
			m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
			if ( m_nRecordLast == m_nRecordFirst ) m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;


			return true;
		}
	case GER_USER_LEFT:		//用户离开
		{
			//胜利类型
			enScoreKind ScoreKind=(m_lUserWinScore[wChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;

			//写入积分
			if (m_lUserWinScore[wChairID]!=0L) m_pITableFrame->WriteUserScore(wChairID,m_lUserWinScore[wChairID], (LONG)m_lUserRevenue[wChairID], ScoreKind);
			m_lUserWinScore[wChairID] = 0;

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
	case GS_FREE:			//空闲状态
		{
			//发送记录
			WORD wBufferSize=0;
			BYTE cbBuffer[SOCKET_BUFFER];
			int nIndex = m_nRecordFirst;
			while ( nIndex != m_nRecordLast )
			{
				if ((wBufferSize+sizeof(tagServerGameRecord))>sizeof(cbBuffer))
				{
					m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
					wBufferSize=0;
				}
				CopyMemory(cbBuffer+wBufferSize,&m_GameRecordArrary[nIndex],sizeof(tagServerGameRecord));
				wBufferSize+=sizeof(tagServerGameRecord);

				nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
			}
			if (wBufferSize>0) m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);

			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//下注信息
			StatusFree.lOddcore=m_lOddScore;
			StatusFree.lEvenScore=m_lEvenScore;

			StatusFree.lAreaLimitScore = m_lAreaLimitScore;
			StatusFree.lUserLimitScore = m_lUserLimitScore;

			//庄家信息
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;

			//下注信息
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusFree.lMeOddScore=m_lUserOddScore[wChiarID];
				StatusFree.lMeEvenScore=m_lUserEvenScore[wChiarID];
				StatusFree.lMeMaxScore=pIServerUserItem->GetUserScore()->lScore;
			}
	
			if ( m_CurrentBanker.dwUserID != 0 ) 
			{
				StatusFree.cbBankerTime = m_cbBankerTimer;
				StatusFree.lCurrentBankerScore = m_lBankerWinScore;
				StatusFree.wCurrentBankerChairID = m_CurrentBanker.wChairID;
				StatusFree.lBankerTreasure = m_CurrentBanker.lUserScore;
			}
			else StatusFree.wCurrentBankerChairID = INVALID_CHAIR;
			

			//全局信息
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusFree.cbTimeLeave=(BYTE)(TIME_FREE-__min(dwPassTime,TIME_FREE));

			//发送场景
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄条件为：%I64d,区域限制为：%I64d,玩家限制为：%I64d"),m_lApplyBankerCondition,
				m_lAreaLimitScore,m_lUserLimitScore);

			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);
			//发送申请者
			SendApplyUser(  pIServerUserItem );

			return bSuccess;
		}
	case GS_PLAYING:		//游戏状态
	case GS_WK_JETTON:	
	case GS_WK_END:	
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//下注信息
			StatusPlay.lOddcore=m_lOddScore;
			StatusPlay.lEvenScore=m_lEvenScore;

			StatusPlay.lAreaLimitScore = m_lAreaLimitScore;
			StatusPlay.lUserLimitScore = m_lUserLimitScore;

			//庄家信息
			StatusPlay.lApplyBankerCondition = m_lApplyBankerCondition;

			//下注信息
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusPlay.lMeOddScore=m_lUserOddScore[wChiarID];
				StatusPlay.lMeEvenScore=m_lUserEvenScore[wChiarID];
				StatusPlay.lMeMaxScore=pIServerUserItem->GetUserScore()->lScore;
			}

			if ( m_CurrentBanker.dwUserID != 0 ) 
			{
				StatusPlay.cbBankerTime = m_cbBankerTimer;
				StatusPlay.lCurrentBankerScore = m_lBankerWinScore;
				StatusPlay.wCurrentBankerChairID = m_CurrentBanker.wChairID;
				StatusPlay.lBankerTreasure = m_CurrentBanker.lUserScore;
			}
			else StatusPlay.wCurrentBankerChairID = INVALID_CHAIR;

			//色子信息
			CopyMemory(StatusPlay.cbDice,m_cbDice,sizeof(m_cbDice));

			//发送申请者
			SendApplyUser( pIServerUserItem );

			//发送场景
			m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

			return true;
		}
	}

	return false;
}

//定时器事件
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_FREE:		//空闲时间
		{

			RandDice();

			MakeAndroidUserBanker();

			//开始游戏
			m_pITableFrameControl->StartGame();

			//设置时间
			m_pITableFrame->SetGameTimer(IDI_GAME,TIME_GAME*1000,1,0L);

			//设置状态
			m_pITableFrame->SetGameStatus(GS_WK_GAME);

			return true;
		}
	case IDI_GAME:
		{
			//设置时间
			m_pITableFrame->SetGameTimer(IDI_GAME_END,TIME_GAME_END*1000,1,0L);

			//设置状态
			m_pITableFrame->SetGameStatus(GS_WK_END);

			CMD_S_EndPlaceJetton EndPlaceJetton;
			ZeroMemory(&EndPlaceJetton, sizeof(EndPlaceJetton));
			EndPlaceJetton.cbGameTime = TIME_GAME_END;
			CopyMemory(EndPlaceJetton.cbDice, m_cbDice, sizeof(EndPlaceJetton.cbDice));

			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_END_PLACE_JETTON,&EndPlaceJetton,sizeof(EndPlaceJetton));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_END_PLACE_JETTON,&EndPlaceJetton,sizeof(EndPlaceJetton));

			//计算分数
			CalculateScore();

			return true;
		}
	case IDI_GAME_END:			//结束游戏
		{
			//结束游戏
			OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);

			//设置时间
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,TIME_FREE*1000,1,0L);

			//设置状态
			m_pITableFrame->SetGameStatus(GS_WK_FREE);

			//庄家信息
			if ( m_CurrentBanker.dwUserID != 0 )
			{
				CMD_S_ChangeUserScore ChangeUserScore;
				ZeroMemory( &ChangeUserScore, sizeof( ChangeUserScore ) );
				ChangeUserScore.wCurrentBankerChairID = m_CurrentBanker.wChairID;
				ChangeUserScore.lCurrentBankerScore = m_lBankerWinScore;
				ChangeUserScore.cbBankerTime = m_cbBankerTimer;
				ChangeUserScore.lScore = m_CurrentBanker.lUserScore;
				ChangeUserScore.wChairID = m_CurrentBanker.wChairID;
				m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_CHANGE_USER_SCORE,&ChangeUserScore,sizeof(ChangeUserScore));
				m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_CHANGE_USER_SCORE,&ChangeUserScore,sizeof(ChangeUserScore));
			}

			return true;
		}
	}

	return false;
}

//游戏消息处理
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_PLACE_JETTON:		//用户加注
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_PlaceJetton));
			if (wDataSize!=sizeof(CMD_C_PlaceJetton)) return false;

			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//消息处理
			CMD_C_PlaceJetton * pPlaceJetton=(CMD_C_PlaceJetton *)pDataBuffer;
			return OnUserPlaceJetton(pUserData->wChairID,pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);
		}
	case SUB_C_APPLY_BANKER:		//申请做庄
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_ApplyBanker));
			if (wDataSize!=sizeof(CMD_C_ApplyBanker)) return false;

			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			//if (pUserData->cbUserStatus!=US_SIT) return true;

			//消息处理
			CMD_C_ApplyBanker * pApplyBanker=(CMD_C_ApplyBanker *)pDataBuffer;

			return OnUserApplyBanker( pUserData, pApplyBanker->bApplyBanker );	
		}

case SUB_C_MANAGE_CONTROL:
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_ManageControl));
			if (wDataSize!=sizeof(CMD_C_ManageControl)) return false;

			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if((pUserData->dwUserRight&UR_GAME_CONTROL)==0) return false;

			//类型转换
			CMD_C_ManageControl *pManageControl=(CMD_C_ManageControl*)pDataBuffer;
			//
			//区域控制

			TCHAR Session[255];
			memset(Session,0,255);
			if(m_pGameServiceOption)
			_snprintf(Session,sizeof(Session),TEXT("ManagerControl%d"),m_pGameServiceOption->wServerID);
			else
			{
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg,128,"m_pGameServiceOption ==NULL SUB_C_MANAGE_CONTROL");
				
			}

			if (pManageControl->bControlBanker)
			{
				WritePrivateProfileString(Session, TEXT("BankerWin"), pManageControl->nControlBanker?TEXT("1"):TEXT("0"), m_szConfigFileName);
			}
			else
			{
				WritePrivateProfileString(Session, TEXT("BankerWin"), TEXT("9"), m_szConfigFileName);
			}

			if (pManageControl->bControlArea)
			{
				TCHAR szWindSide[16];
				_sntprintf(szWindSide,CountArray(szWindSide),TEXT("%d"),pManageControl->nControlArea+1);
				WritePrivateProfileString(Session, TEXT("WinSide"), szWindSide,m_szConfigFileName);
			}
			else
			{
				WritePrivateProfileString(Session, TEXT("WinSide"), TEXT("9"),m_szConfigFileName);
			}

			//发送消息
			m_pITableFrame->SendGameMessage(pIServerUserItem,TEXT("设置成功"),SMT_INFO|SMT_EJECT);

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
	//设置时间
	if ((bLookonUser==false)&&(m_dwJettonTime==0L))
	{
		m_dwJettonTime=(DWORD)time(NULL);
		m_pITableFrame->SetGameTimer(IDI_FREE,TIME_FREE*1000,1,NULL);
		m_pITableFrame->SetGameStatus(GS_WK_FREE);
	}
	//限制提示
	TCHAR szTipMsg[128];
	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄条件为：%I64d,区域限制为：%I64d,玩家限制为：%I64d"),m_lApplyBankerCondition,
		m_lAreaLimitScore,m_lUserLimitScore);
	m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);

	return true;
}

//用户起来
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//记录成绩
	if (bLookonUser==false)
	{
		//状态判断
		if ( m_pITableFrame->GetGameStatus() != GS_PLAYING )
		{
			//变量定义
			RETYPELONG lScore=0;
			RETYPELONG lRevenue=0;
			enScoreKind ScoreKind;

			//置零变量
			m_lUserOddScore[wChairID] = 0;
			m_lUserEvenScore[wChairID] = 0;

			//写入积分
			if (lScore!=0L)
			{
				//写入积分
				ScoreKind=enScoreKind_Flee;
				m_pITableFrame->WriteUserScore(pIServerUserItem, lScore,(LONG)lRevenue, ScoreKind);
			}
		}

		//存在判断
		for ( INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArrary.GetCount(); ++nUserIdx )
		{
			tagApplyUserInfo ApplyUserInfo = m_ApplyUserArrary[ nUserIdx ];

			if ( ApplyUserInfo.dwUserID == pIServerUserItem->GetUserID() )
			{

				//构造变量
				CMD_S_ApplyBanker ApplyBanker;
				CopyMemory( ApplyBanker.szAccount, pIServerUserItem->GetAccounts(), sizeof( ApplyBanker.szAccount ) );
				ApplyBanker.lScore = pIServerUserItem->GetUserScore()->lScore;
				ApplyBanker.bApplyBanker = false;

				//发送消息
				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof( ApplyBanker ) );
				m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof( ApplyBanker ) );

				//当前判断
				if ( ApplyUserInfo.dwUserID == m_CurrentBanker.dwUserID )
				{
					//重置变量
					ZeroMemory( &m_CurrentBanker, sizeof( m_CurrentBanker ) );
					m_cbBankerTimer=0;
					m_lBankerWinScore=0;

					//发送消息
					SendChangeBankerMsg();
				}
				
				//删除玩家
				m_ApplyUserArrary.RemoveAt( nUserIdx );
			}
		}
	}

	return true;
}

//加注事件
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, RETYPELONG lJettonScore)
{
	//效验参数
	ASSERT((cbJettonArea<=ID_EVEN)&&(lJettonScore>0L));
	if ((cbJettonArea>ID_EVEN)||(lJettonScore<=0L)) return false;



	if (m_pITableFrame->GetGameStatus()!=GS_WK_GAME)
	{
		CMD_S_PlaceJettonFail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
		PlaceJettonFail.lJettonArea=cbJettonArea;
		PlaceJettonFail.lPlaceScore=lJettonScore;
		PlaceJettonFail.wPlaceUser=wChairID;

		//发送消息
		m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
		return true;
	}


	//庄家判断
	if ( m_CurrentBanker.dwUserID != 0 && m_CurrentBanker.wChairID == wChairID ) return true;
	if ( m_CurrentBanker.dwUserID == 0 ) return true;


	//变量定义
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	RETYPELONG lJettonCount=m_lUserOddScore[wChairID]+m_lUserEvenScore[wChairID];

	RETYPELONG lUserScore = pIServerUserItem->GetUserScore()->lScore;

	if ( lUserScore < lJettonCount + lJettonScore ) return true;

	//合法验证
	if ( ID_ODD == cbJettonArea )
	{
		if ( GetMaxOddScore(wChairID) < lJettonScore ) return true;

		//保存下注
		m_lOddScore+= lJettonScore;
		m_lUserOddScore[wChairID] += lJettonScore;
	}
	else if ( ID_EVEN == cbJettonArea )
	{
		if ( GetMaxEvenScore(wChairID) <lJettonScore ) return true;

		//保存下注
		m_lEvenScore+= lJettonScore;
		m_lUserEvenScore[wChairID] += lJettonScore;
	}
	else
	{
		ASSERT(FALSE);
		return true;
	}

	//变量定义
	CMD_S_PlaceJetton PlaceJetton;
	ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

	//构造变量
	PlaceJetton.wChairID=wChairID;
	PlaceJetton.cbJettonArea=cbJettonArea;
	PlaceJetton.lJettonScore=lJettonScore;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

	return true;
}

//发送扑克
bool CTableFrameSink::RandDice()
{
	//混乱色子
	m_cbDice[0] = rand() % 6 + 1;
	m_cbDice[1] = rand() % 6 + 1;


	TCHAR Session[255];
	memset(Session,0,255);
	_snprintf(Session,sizeof(Session),TEXT("ManagerControl%d"),m_pGameServiceOption->wServerID);


	int m_cbWinSideControl=GetPrivateProfileInt(Session, TEXT("WinSide"), 9, m_szConfigFileName );

	int nBankerWin=GetPrivateProfileInt(Session, TEXT("BankerWin"), 9, m_szConfigFileName );

	if (0==nBankerWin || 1==nBankerWin)
	{

		if (m_nSendCardCount < SEND_COUNT)
		{
			//庄家得分
			LONGLONG lBankerWinScore=CalculateScoreBanker();

			if (0==nBankerWin && lBankerWinScore>0)
			{
				m_nSendCardCount++;
				RandDice();
			}
			else if (1==nBankerWin && lBankerWinScore<0)
			{
				m_nSendCardCount++;
				RandDice();

			}else
			{
				m_nSendCardCount=0;

			}
		}else
		{
			m_nSendCardCount = 0;

		}
	}else
	{
		if(m_cbWinSideControl==1)
		{

			BYTE Tmp1[]={2,4,6};
			BYTE Tmp2[]={1,3,5};
			if(m_cbDice[0]%2==1)
			{
				int index = (rand()%3);
				m_cbDice[1]= Tmp1[index];
			}else
			{
				int index = (rand()%3);
				m_cbDice[1]= Tmp2[index];
			}
	
		}else
		{
			if(m_cbWinSideControl==2)
			{
				BYTE Tmp1[]={2,4,6};
				BYTE Tmp2[]={1,3,5};

				if(m_cbDice[0]%2==1)
				{
					int index = (rand()%3);
					m_cbDice[1]= Tmp2[index];

				}else
				{
					int index = (rand()%3);
					m_cbDice[1]= Tmp1[index];

				}

			}
		}
	}

	return true;
}

//盈利判断
LONGLONG CTableFrameSink::AccountPayoffScore()
{
	return 0;
}

//申请庄家
bool CTableFrameSink::OnUserApplyBanker( tagServerUserData *pUserData, bool bApplyBanker, bool bUserAction )
{
	//状态判断
//	if ( bUserAction == true && m_pITableFrame->GetGameStatus() != GS_WK_FREE ) return true;

	//合法判断
	if ( bApplyBanker && pUserData->UserScoreInfo.lScore < m_lApplyBankerCondition ) return true;

	//保存玩家
	if ( bApplyBanker )
	{
		//存在判断
		for ( INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArrary.GetCount(); ++nUserIdx )
		{
			tagApplyUserInfo ApplyUserInfo = m_ApplyUserArrary[ nUserIdx ];
			if ( ApplyUserInfo.dwUserID == pUserData->dwUserID ) return true;
		}

		//构造信息
		tagApplyUserInfo ApplyUserInfo;
		ApplyUserInfo.dwUserID = pUserData->dwUserID;
		ApplyUserInfo.lUserScore = pUserData->UserScoreInfo.lScore;
		ApplyUserInfo.wChairID = pUserData->wChairID;

		//插入玩家
		INT_PTR nUserCount = m_ApplyUserArrary.GetCount();
		m_ApplyUserArrary.InsertAt( nUserCount, ApplyUserInfo );

		//构造变量
		CMD_S_ApplyBanker ApplyBanker;
		CopyMemory( ApplyBanker.szAccount, pUserData->szAccounts, sizeof( ApplyBanker.szAccount ) );
		ApplyBanker.lScore = pUserData->UserScoreInfo.lScore+pUserData->lStorageScore;
		ApplyBanker.bApplyBanker = true;

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof( ApplyBanker ) );
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof( ApplyBanker ) );

		return true;
	}
	else	//取消申请
	{
		//存在判断
		for ( INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArrary.GetCount(); ++nUserIdx )
		{
			tagApplyUserInfo ApplyUserInfo = m_ApplyUserArrary[ nUserIdx ];

			if ( ApplyUserInfo.dwUserID == pUserData->dwUserID )
			{
				//删除玩家
				m_ApplyUserArrary.RemoveAt( nUserIdx );

				//当前判断
				if ( m_CurrentBanker.dwUserID == ApplyUserInfo.dwUserID )
				{
					m_CurrentBanker.dwUserID = 0;
					ChangeBanker();
					SendChangeBankerMsg();
				}

				//构造变量
				CMD_S_ApplyBanker ApplyBanker;
				CopyMemory( ApplyBanker.szAccount, pUserData->szAccounts, sizeof( ApplyBanker.szAccount ) );
				ApplyBanker.lScore = pUserData->UserScoreInfo.lScore;
				ApplyBanker.bApplyBanker = false;

				//发送消息
				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof( ApplyBanker ) );
				m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof( ApplyBanker ) );


				return true;
			}
		}

		return true;
	}

	return true;

}
void CTableFrameSink::RandList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{

	//混乱准备
	BYTE *cbCardData = new BYTE[cbBufferCount];
	CopyMemory(cbCardData,cbCardBuffer,cbBufferCount);

	//混乱扑克
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(cbBufferCount-cbRandCount);
		cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[cbBufferCount-cbRandCount];
	} while (cbRandCount<cbBufferCount);

	delete []cbCardData;
	cbCardData = NULL;

	return;
}
//处理机器人上庄
void CTableFrameSink::MakeAndroidUserBanker()
{

	BYTE  bcAndroidUser[GAME_PLAYER];
	int   bcOutAppUserArrayCount = 0;
	int   inInAppUserArrayCount = 0;
	int i = 0;

	TCHAR Session[255];
	memset(Session,0,255);
	_snprintf(Session,sizeof(Session),TEXT("BankerCondition%d"),m_pGameServiceOption->wServerID);



	for (WORD wCharidid=0; wCharidid<GAME_PLAYER; ++wCharidid)
	{

		IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(wCharidid);

		if(!pServerUserItem)
		{
			memset(&m_AndroidUser[wCharidid],0,sizeof(TAndroidUser));
			continue;
		}

		if(pServerUserItem&&pServerUserItem->IsAndroidUser())
		{
			m_AndroidUser[wCharidid].blAndroidUser = true;

		}else
		{
			m_AndroidUser[wCharidid].blAndroidUser = false;
			memset(&m_AndroidUser[wCharidid],0,sizeof(TAndroidUser));
			continue;

		}
		if (m_CurrentBanker.wChairID !=wCharidid )
		{
			if(m_AndroidUser[wCharidid].blAndroidUser==true)
			{
				m_AndroidUser[wCharidid].PlayCount++;
				m_AndroidUser[wCharidid].BankCount=0;

			}
			bool blInArray = false;
			for (INT nUserIdx=0; nUserIdx<m_ApplyUserArrary.GetCount(); ++nUserIdx)
			{
				if(wCharidid == m_ApplyUserArrary[nUserIdx].wChairID)
				{
					inInAppUserArrayCount++;
					blInArray = true;
					break;

				}
			}

			int	m_nAndroidUserPlayerCount=GetPrivateProfileInt(Session, TEXT("AndroidUserPlayerCount"), 2, m_szConfigFileName);

			if(blInArray==false&&m_AndroidUser[wCharidid].PlayCount>m_nAndroidUserPlayerCount&&m_pITableFrame->GetServerUserItem(wCharidid)->GetUserScore()->lScore>m_lApplyBankerCondition
				)
			{
				bcAndroidUser[i++] = (BYTE)wCharidid;
				bcOutAppUserArrayCount++;
			}

		}else
		{

			if(m_AndroidUser[wCharidid].blAndroidUser==true)
			{
				m_AndroidUser[wCharidid].PlayCount=0;
				m_AndroidUser[wCharidid].BankCount++;

			}
			int	m_nAndroidUserBankerCount=GetPrivateProfileInt(Session, TEXT("AndroidUserBankerCount"), 1, m_szConfigFileName);
			if(m_AndroidUser[wCharidid].BankCount>m_nAndroidUserBankerCount)
			{
				IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(wCharidid);
				if(pServerUserItem)
					OnUserApplyBanker(pServerUserItem->GetUserData(),false,true);
				m_AndroidUser[wCharidid].BankCount = 0;

			}

		}

	}
	WORD wMinApplyBankerCount=GetPrivateProfileInt(Session, TEXT("MinApplyBankerCount"), 2, m_szConfigFileName);

	if(inInAppUserArrayCount<wMinApplyBankerCount&&bcOutAppUserArrayCount>0)
	{
		RandList(bcAndroidUser,bcOutAppUserArrayCount);

		if(bcOutAppUserArrayCount<wMinApplyBankerCount-inInAppUserArrayCount)
		{ 
			for (int i = 0;i<bcOutAppUserArrayCount;i++)
			{
				IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(bcAndroidUser[i]);

				if(pServerUserItem)
					OnUserApplyBanker(pServerUserItem->GetUserData(),true,true);
			}


		}else
		{
			for (int i = 0;i<wMinApplyBankerCount-inInAppUserArrayCount;i++)
			{
				IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(bcAndroidUser[i]);

				if(pServerUserItem)
					OnUserApplyBanker(pServerUserItem->GetUserData(),true,true);
			}

		}

	}

}
//发送庄家
void CTableFrameSink::SendApplyUser( IServerUserItem *pRcvServerUserItem )
{
	for ( INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArrary.GetCount(); ++nUserIdx )
	{
		tagApplyUserInfo ApplyUserInfo = m_ApplyUserArrary[ nUserIdx ];

		//获取玩家
		IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem( ApplyUserInfo.wChairID );
		if ( ! pServerUserItem ) continue;

		//构造变量
		CMD_S_ApplyBanker ApplyBanker;
		CopyMemory( ApplyBanker.szAccount, pServerUserItem->GetAccounts(), sizeof( ApplyBanker.szAccount ) );
		ApplyBanker.lScore = pServerUserItem->GetUserScore()->lScore;
		ApplyBanker.bApplyBanker = true;

		//发送消息
		m_pITableFrame->SendUserData( pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof( ApplyBanker ) );
	}
}
//获得能够坐庄次数
WORD CTableFrameSink::GetBankerTime()
{


	//读取配置
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	TCHAR szFileName[MAX_PATH];
	_snprintf(szFileName,sizeof(szFileName),TEXT("%s\\OddEvenBattleConfig.ini"),szPath);
	TCHAR Session[255];
	memset(Session,0,255);
	_snprintf(Session,sizeof(Session),TEXT("BankerCondition%d"),m_pGameServiceOption->wServerID);

	WORD wBankerTime=GetPrivateProfileInt(Session, TEXT("Time"), 20, szPath);

	if(m_CurrentBanker.wChairID==INVALID_CHAIR)
	{
		return wBankerTime;

	}



	if(m_cbBankerTimer>=wBankerTime)
	{
		//获取庄家
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_CurrentBanker.wChairID);

		if(pIServerUserItem==NULL)
		{
			return wBankerTime;
		}

		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;
		LONGLONG ScoreCondition = 0;

	
		ScoreCondition= m_LowerBigBankerCondition;
		

		if(lBankerScore>=ScoreCondition)
		{

			WORD wBankerTimeAdd=GetPrivateProfileInt(Session, TEXT("TimeAdd"), 10, szFileName);
			return wBankerTimeAdd+wBankerTime;

		}else
		{
			for (WORD i=0; i<m_ApplyUserArrary.GetCount(); ++i)
			{
				//获取玩家
				tagApplyUserInfo wChairID=m_ApplyUserArrary[i];

				//条件过滤
				if(wChairID.wChairID==m_CurrentBanker.wChairID)continue;

				IServerUserItem *pbIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID.wChairID);

				if(pbIServerUserItem)
				{
					LONGLONG lScore=pbIServerUserItem->GetUserScore()->lScore;

					if(lBankerScore<lScore)
					{
						return 0;
					}
				}

				break;
			}

		}	 
	}

	return wBankerTime;

}
//更换庄家
void CTableFrameSink::ChangeBanker()
{

	try
	{
			
			//递增次数
			m_cbBankerTimer++;
			//读取配置
			TCHAR szPath[MAX_PATH]=TEXT("");
			GetCurrentDirectory(sizeof(szPath),szPath);

			//读取配置
			TCHAR szFileName[MAX_PATH];
			_snprintf(szFileName,sizeof(szFileName),TEXT("%s\\OddEvenBattleConfig.ini"),szPath);

			TCHAR Session[255];
			memset(Session,0,255);
			_snprintf(Session,sizeof(Session),TEXT("BankerCondition"));


			WORD wBankerTime = GetBankerTime();
			
			LONGLONG lBigScore = 0; 

			for(int i = 0;i<m_ApplyUserArrary.GetCount();i++)
			{
				tagApplyUserInfo wChairID=m_ApplyUserArrary[i];

				if(wChairID.wChairID==m_CurrentBanker.wChairID)
				{
					continue;
				}
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID.wChairID);
				if(pIServerUserItem)
				{
					if(pIServerUserItem->GetUserScore()->lScore>lBigScore)
					{
						lBigScore = pIServerUserItem->GetUserScore()->lScore;

					}	
				}else
				{
					continue ;
				}

			}
			//轮庄判断
			if ( m_CurrentBanker.dwUserID != 0 )
			{
				//轮换判断
				IServerUserItem *pBankerUserItem = m_pITableFrame->GetServerUserItem(m_CurrentBanker.wChairID);
				if(pBankerUserItem)
				{

					//次数判断
					if ( (wBankerTime <= m_cbBankerTimer) || (m_CurrentBanker.lUserScore < m_lApplyBankerCondition))
					{
						m_cbBankerTimer = 0;
						m_lBankerWinScore=0;

						//客户端删除
						IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem( m_CurrentBanker.wChairID );
						OnUserApplyBanker( pServerUserItem->GetUserData(), false, false ) ;

						//重新赋值
						ZeroMemory( &m_CurrentBanker, sizeof( m_CurrentBanker ) );

						while( ! m_ApplyUserArrary.IsEmpty() )
						{
							m_CurrentBanker = m_ApplyUserArrary[ 0 ];
							//合法判断
							if ( m_CurrentBanker.lUserScore < m_lApplyBankerCondition )
							{
								ZeroMemory( &m_CurrentBanker, sizeof( m_CurrentBanker ) );

								//获取玩家
								IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem( m_ApplyUserArrary[ 0 ].wChairID );
								if (  pServerUserItem  ) OnUserApplyBanker( pServerUserItem->GetUserData(), false, false );
							}
							else
								break;
						}
					}

				}else if ( 0 < m_ApplyUserArrary.GetCount() )
				{

					while( ! m_ApplyUserArrary.IsEmpty() )
					{
						m_CurrentBanker = m_ApplyUserArrary[ 0 ];
						//合法判断
						if ( m_CurrentBanker.lUserScore < m_lApplyBankerCondition )
						{
							ZeroMemory( &m_CurrentBanker, sizeof( m_CurrentBanker ) );
							//获取玩家
							IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem( m_ApplyUserArrary[ 0 ].wChairID );
							if (  pServerUserItem  ) OnUserApplyBanker( pServerUserItem->GetUserData(), false, false );
						}
						else
							break;
					}
					m_cbBankerTimer = 0;
					m_lBankerWinScore=0;
				}
				
			}
			else if ( 0 < m_ApplyUserArrary.GetCount() )
			{
				while( ! m_ApplyUserArrary.IsEmpty() )
				{
					m_CurrentBanker = m_ApplyUserArrary[ 0 ];
					//合法判断
					if ( m_CurrentBanker.lUserScore < m_lApplyBankerCondition )
					{
						ZeroMemory( &m_CurrentBanker, sizeof( m_CurrentBanker ) );
						//获取玩家
						IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem( m_ApplyUserArrary[ 0 ].wChairID );
						if (  pServerUserItem  ) OnUserApplyBanker( pServerUserItem->GetUserData(), false, false );
					}
					else
						break;
				}
				m_cbBankerTimer = 0;
				m_lBankerWinScore=0;
			}
	}
	catch (...)
	{
	}
}

//轮换庄家
void CTableFrameSink::SendChangeBankerMsg()
{
	CMD_S_ChangeBanker ChangeBanker;

	for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
	{
		IServerUserItem *pSeverUserItem = m_pITableFrame->GetServerUserItem( wChairID );
		if ( ! pSeverUserItem ) continue;

		ZeroMemory( &ChangeBanker, sizeof( ChangeBanker ) );
		if ( m_CurrentBanker.dwUserID != 0 )
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
		m_pITableFrame->SendTableData( wChairID, SUB_S_CHANGE_BANKER, &ChangeBanker, sizeof( ChangeBanker ) );
		m_pITableFrame->SendLookonData( wChairID, SUB_S_CHANGE_BANKER, &ChangeBanker, sizeof( ChangeBanker ) );
	}

	if(m_CurrentBanker.dwUserID == 0 || m_CurrentBanker.wChairID>=GAME_PLAYER)return;
	TCHAR Session[255]={0};
	_snprintf(Session,sizeof(Session),TEXT("Message%d"),m_pGameServiceOption->wServerID);

	//读取消息
	LONGLONG lMessageCount=GetPrivateProfileInt(Session,TEXT("MessageCount"),0,m_szConfigFileName);
	if (lMessageCount!=0)
	{
		//读取配置
		LONGLONG lIndex=rand()%lMessageCount;
		TCHAR szKeyName[32],szMessage1[256],szMessage2[256];				
		_sntprintf(szKeyName,CountArray(szKeyName),TEXT("Item%ld"),lIndex);
		GetPrivateProfileString(Session,szKeyName,TEXT("恭喜[ %s ]上庄"),szMessage1,CountArray(szMessage1),m_szConfigFileName);

		//获取玩家
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_CurrentBanker.wChairID);
		if(pIServerUserItem==NULL)return;

		//发送消息
		_sntprintf(szMessage2,CountArray(szMessage2),szMessage1,pIServerUserItem->GetAccounts());

		//游戏玩家
		for (WORD i=0; i<GAME_PLAYER; ++i)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
			if (pIServerUserItem==NULL) continue;
			m_pITableFrame->SendGameMessage(pIServerUserItem,szMessage2,SMT_INFO);
		}

		//旁观玩家
		WORD wIndex=0;
		do {
			IServerUserItem *pILookonServerUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
			if (pILookonServerUserItem==NULL) break;

			m_pITableFrame->SendGameMessage(pILookonServerUserItem,szMessage2,SMT_INFO);

		}while(true);
	}
}

//用户断线
bool __cdecl CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	return true;
}

//最大下注
RETYPELONG CTableFrameSink::GetMaxOddScore(WORD wChairID)
{
	//其他区域
	RETYPELONG lOtherAreaScore = m_lEvenScore;

	IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);

	//最大下注
	RETYPELONG lMaxOddScore = m_CurrentBanker.lUserScore+lOtherAreaScore;
	lMaxOddScore -= (m_lOddScore);

	//已下注额
	RETYPELONG lNowJetton = 0;
	lNowJetton += m_lUserOddScore[wChairID];
	lNowJetton += m_lUserEvenScore[wChairID];

	//我的下注
	
	RETYPELONG lMeLessScore  = min(pServerUserItem->GetUserScore()->lScore,m_lUserLimitScore);
	 lMeLessScore =  lMeLessScore  - lNowJetton;
	ASSERT(lMeLessScore >= 0 );
	lMeLessScore = max(lMeLessScore, 0);

	//最大下注
	RETYPELONG lMaxJetton = min(lMaxOddScore, lMeLessScore);
	RETYPELONG lAreaLimit = m_lAreaLimitScore - m_lOddScore;
	lMaxJetton = min(lMaxJetton, lAreaLimit);

	return lMaxJetton;
}
//最大下注
RETYPELONG CTableFrameSink::GetMaxEvenScore(WORD wChairID)
{
	//其他区域
	RETYPELONG lOtherAreaScore = m_lOddScore ;

	IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);

	//最大下注
	RETYPELONG lMaxEvenScore = lOtherAreaScore + m_CurrentBanker.lUserScore;
	lMaxEvenScore -= (m_lEvenScore);

	//已下注额
	RETYPELONG lNowJetton = 0;
	lNowJetton += m_lUserOddScore[wChairID];
	lNowJetton += m_lUserEvenScore[wChairID];

	//我的下注
	RETYPELONG lMeLessScore = min(m_lUserLimitScore,pServerUserItem->GetUserScore()->lScore);
	lMeLessScore  = lMeLessScore - lNowJetton;
	ASSERT(lMeLessScore >= 0 );
	lMeLessScore = max(lMeLessScore, 0);

	//最大下注
	RETYPELONG lMaxJetton = min(lMaxEvenScore, lMeLessScore);
	RETYPELONG lAreaLimit = m_lAreaLimitScore - m_lEvenScore;
	lMaxJetton = min(lMaxJetton, lAreaLimit);

	

	return lMaxJetton;
}

//计算得分
void CTableFrameSink::CalculateScore()
{
	//结束消息
	CMD_S_GameScore GameScore;
	ZeroMemory(&GameScore,sizeof(GameScore));

	//变量定义
	RETYPELONG cbRevenue=m_pGameServiceOption->wRevenue;

	//推断玩家
	DeduceWinner(GameScore.cbWinner);

	//游戏记录
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.cbDicePoint = m_cbDice[0] + m_cbDice[1];
	GameRecord.wWinner = GameScore.cbWinner;


	//庄家总量
	RETYPELONG lBankerWinScore = 0;

	//玩家成绩
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	RETYPELONG lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	//玩家下注
	RETYPELONG *pUserScore[] = {NULL, m_lUserOddScore, m_lUserEvenScore};

	//计算积分
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//庄家判断
		if ( m_CurrentBanker.dwUserID != 0 && m_CurrentBanker.wChairID == i ) continue;

		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem==NULL) continue;

		for ( WORD wAreaIndex = ID_ODD; wAreaIndex <= ID_EVEN; ++wAreaIndex )
		{
			if ( wAreaIndex == GameScore.cbWinner ) 
			{
				m_lUserWinScore[i] += ( pUserScore[wAreaIndex][i] ) ;
				m_lUserReturnScore[i] += pUserScore[wAreaIndex][i] ;
				lBankerWinScore -= ( pUserScore[wAreaIndex][i] ) ;
			}
			else
			{
				lUserLostScore[i] -= pUserScore[wAreaIndex][i] ;
				lBankerWinScore += pUserScore[wAreaIndex][i] ;
			}
		}

		//计算税收
		if ( 100 <= m_lUserWinScore[i] )
		{
			float fRevenuePer=float(cbRevenue/1000.);
			m_lUserRevenue[i]  = RETYPELONG(m_lUserWinScore[i]*fRevenuePer);
			m_lUserWinScore[i] -= m_lUserRevenue[i];
		}

		//总的分数
		m_lUserWinScore[i] += lUserLostScore[i];
	}

	//庄家成绩
	if ( m_CurrentBanker.dwUserID != 0 )
	{
		WORD wBankerChairID = m_CurrentBanker.wChairID;
		m_lUserWinScore[wBankerChairID] = lBankerWinScore;

		//计算税收
		if ( 100 <= m_lUserWinScore[wBankerChairID] )
		{
			float fRevenuePer=float(cbRevenue/1000.);
			m_lUserRevenue[wBankerChairID]  = RETYPELONG(m_lUserWinScore[wBankerChairID]*fRevenuePer);
			m_lUserWinScore[wBankerChairID] -= m_lUserRevenue[wBankerChairID];
			lBankerWinScore = m_lUserWinScore[wBankerChairID];
		}
		IServerUserItem *pBankerUserItem = m_pITableFrame->GetServerUserItem(wBankerChairID);

		//累计积分
		m_lBankerWinScore += lBankerWinScore;
	}

	//买注总量
	GameScore.lDrawOddScore=m_lOddScore;
	GameScore.lDrawEvenScore=m_lEvenScore;

	GameScore.lBankerScore = lBankerWinScore;

	//发送积分
	for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserIndex);
		if ( pIServerUserItem == NULL ) continue;

		//我的下注
		GameScore.lMeOddScore=m_lUserOddScore[wUserIndex];
		GameScore.lMeEvenScore=m_lUserEvenScore[wUserIndex];

		//发送消息
		GameScore.lMeGameScore=m_lUserWinScore[wUserIndex];
		GameScore.lMeReturnScore = m_lUserReturnScore[wUserIndex];
		m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_SCORE,&GameScore,sizeof(GameScore));
		m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_SCORE,&GameScore,sizeof(GameScore));
	}
	return ;
}
//计算得分
LONGLONG CTableFrameSink::CalculateScoreBanker()
{


	RETYPELONG							m_lUserWinScore[GAME_PLAYER];		//玩家成绩
	RETYPELONG							m_lUserReturnScore[GAME_PLAYER];	//返回下注
	RETYPELONG							m_lUserRevenue[GAME_PLAYER];		//

	//结束消息
	CMD_S_GameScore GameScore;
	ZeroMemory(&GameScore,sizeof(GameScore));

	//变量定义
	RETYPELONG cbRevenue=m_pGameServiceOption->wRevenue;

	//推断玩家
	DeduceWinner(GameScore.cbWinner);

	//游戏记录
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.cbDicePoint = m_cbDice[0] + m_cbDice[1];
	GameRecord.wWinner = GameScore.cbWinner;

	//庄家总量
	RETYPELONG lBankerWinScore = 0;

	//玩家成绩
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	RETYPELONG lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	//玩家下注
	RETYPELONG *pUserScore[] = {NULL, m_lUserOddScore, m_lUserEvenScore};

	//计算积分
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//庄家判断
		if ( m_CurrentBanker.dwUserID != 0 && m_CurrentBanker.wChairID == i ) continue;

		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem==NULL) continue;

		for ( WORD wAreaIndex = ID_ODD; wAreaIndex <= ID_EVEN; ++wAreaIndex )
		{
			if ( wAreaIndex == GameScore.cbWinner ) 
			{
				m_lUserWinScore[i] += ( pUserScore[wAreaIndex][i] ) ;
				m_lUserReturnScore[i] += pUserScore[wAreaIndex][i] ;
				lBankerWinScore -= ( pUserScore[wAreaIndex][i] ) ;
			}
			else
			{
				lUserLostScore[i] -= pUserScore[wAreaIndex][i] ;
				lBankerWinScore += pUserScore[wAreaIndex][i] ;
			}
		}

		//计算税收
		if ( 100 <= m_lUserWinScore[i] )
		{
			float fRevenuePer=float(cbRevenue/1000.);
			m_lUserRevenue[i]  = RETYPELONG(m_lUserWinScore[i]*fRevenuePer);
			m_lUserWinScore[i] -= m_lUserRevenue[i];
		}

		//总的分数
		m_lUserWinScore[i] += lUserLostScore[i];
	}

	//庄家成绩
	if ( m_CurrentBanker.dwUserID != 0 )
	{
		WORD wBankerChairID = m_CurrentBanker.wChairID;
		m_lUserWinScore[wBankerChairID] = lBankerWinScore;

		//计算税收
		if ( 100 <= m_lUserWinScore[wBankerChairID] )
		{
			float fRevenuePer=float(cbRevenue/1000.);
			m_lUserRevenue[wBankerChairID]  = RETYPELONG(m_lUserWinScore[wBankerChairID]*fRevenuePer);
			m_lUserWinScore[wBankerChairID] -= m_lUserRevenue[wBankerChairID];
			lBankerWinScore = m_lUserWinScore[wBankerChairID];
		}
		IServerUserItem *pBankerUserItem = m_pITableFrame->GetServerUserItem(wBankerChairID);


	}

	return  lBankerWinScore;
}
//推断赢家
void CTableFrameSink::DeduceWinner(BYTE &cbWinner)
{
	//计算单双
	bool bEven = ((m_cbDice[0] + m_cbDice[1]) % 2 == 0 ) ? true : false;

	cbWinner = bEven ? ID_EVEN : ID_ODD; 
}
//////////////////////////////////////////////////////////////////////////
//银行操作
#ifdef __SPECIAL___
bool __cdecl CTableFrameSink::OnActionUserBank(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if (pIServerUserItem->GetUserStatus()!=US_LOOKON && pIServerUserItem->GetUserScore()->lScore < m_lApplyBankerCondition)
	{
		//庄家判断
		OnActionUserStandUp(pIServerUserItem->GetChairID(), pIServerUserItem, false);
	}
	return true;
}
bool __cdecl CTableFrameSink::IsAllowBankAction(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	/*if(wChairID == m_CurrentBanker.dwUserID)
		return false;
	else */
		return true;

}
#endif
