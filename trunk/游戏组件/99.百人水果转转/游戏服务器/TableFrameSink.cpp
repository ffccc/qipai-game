#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//常量定义
#define SEND_COUNT					300									//发送次数

//索引定义
#define INDEX_PLAYER				0									//闲家索引
#define INDEX_BANKER				1									//庄家索引

//下注时间
#define IDI_FREE					1									//空闲计时器
#ifdef _DEBUG
#define TIME_FREE					10									//空闲时间
#else
#define TIME_FREE					10									//空闲时间
#endif

//下注时间
#define IDI_PLACE_JETTON			2									//下注计时器

#ifdef _DEBUG
#define TIME_PLACE_JETTON			40									//下注时间
#else
#define TIME_PLACE_JETTON			20									//下注时间
#endif

//结束时间
#define IDI_GAME_END				3									//结束计时器

#ifdef _DEBUG
#define TIME_GAME_END				20									//结束时间
#else
#define TIME_GAME_END				20									//结束时间
#endif

//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//游戏人数
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_TimeControl;	//开始模式

//初始化各区域赔率
const int CTableFrameSink::m_iOddsArray[JETTON_AREA_COUNT]={40,4,30,3,20,2,10,1};

//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink() : m_iMaxAreaScore(100000000)
{
	//总下注数
	ZeroMemory(m_iTotalJettonScore, sizeof(m_iTotalJettonScore));

	//各区域可下分
	ZeroMemory(m_iAreaScoreLimit, sizeof(m_iAreaScoreLimit));

	//控制变量
	m_iGoalAnimal = -1;						//每局游戏会开出的动物
	m_iGameTimes = 0;


	//个人下注
	ZeroMemory(m_iUserAreaScore, sizeof(m_iUserAreaScore));


	//玩家成绩	
	ZeroMemory(m_iUserWinScore,sizeof(m_iUserWinScore));
	ZeroMemory(m_iUserReturnScore,sizeof(m_iUserReturnScore));
	ZeroMemory(m_iUserRevenue,sizeof(m_iUserRevenue));

	m_CtrlAccountArray.RemoveAll();

	//状态变量
	m_dwJettonTime=0L;

	//庄家信息
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker=INVALID_CHAIR;
	m_wBankerTime=0;
	m_iBankerWinScore=0L;		
	m_iBankerCurGameScore=0L;
	m_bEnableSysBanker=true;
	m_nHasWinTimes = 0;

	//记录变量
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;
	m_nRecordLast=0;
	m_dwRecordCount=0;

	//设置文件名
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_snprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\SGDZ_Config.ini"),szPath);

	//控制变量
	__int64 lMaxScore=9223372036854775807;
	CString str;

	//配置信息
	TCHAR szBuffer[MAX_PATH];
	GetPrivateProfileString(TEXT("BankerCondition"),TEXT("Score"),TEXT("500000000"),szBuffer,CountArray(szBuffer),m_szConfigFileName);
	m_iApplyBankerCondition=_atoi64(szBuffer);

	GetPrivateProfileString(TEXT("ScoreLimit"),TEXT("SysLimitScore"),TEXT("9223372036854775807"),szBuffer,CountArray(szBuffer),m_szConfigFileName);
	m_iSysLimitScore=_atoi64(szBuffer);

	GetPrivateProfileString(TEXT("ScoreLimit"),TEXT("UserLimitScore"),TEXT("9223372036854775807"),szBuffer,CountArray(szBuffer),m_szConfigFileName);
	m_iUserLimitScore=_atoi64(szBuffer);

	BYTE cbEnableSysBanker=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("EnableSysBanker"), 1, m_szConfigFileName);
	m_bEnableSysBanker=cbEnableSysBanker?true:false;

	BYTE cbCtrlWin = GetPrivateProfileInt(TEXT("ControlWin"), TEXT("Win"), 0, m_szConfigFileName);
	m_bControlWin = cbCtrlWin>0 ? true : false;

	//庄家一次做庄多少局
	m_nMaxBankerTimes = GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Time"), 10, m_szConfigFileName);

	//读取控制账号信息
	if( m_bControlWin)
	{
		int nAccountNum = GetPrivateProfileInt(TEXT("ControlWin"), TEXT("CtrlAccountNum"), 0, m_szConfigFileName);
		if( nAccountNum > 0 )
		{
			for( int i=0; i<nAccountNum; i++)
			{
				TCHAR szTemp[64]={0};
				ControlAccount CtrlAccount;
				ZeroMemory(&CtrlAccount, sizeof(CtrlAccount));
				sprintf(szTemp, "Account_%d", i);
				CtrlAccount.dwUserID = GetPrivateProfileInt(TEXT("ControlWin"), szTemp, -1, m_szConfigFileName);
				if( CtrlAccount.dwUserID >=0 )
				{
					ZeroMemory(szTemp, sizeof(szTemp));
					sprintf(szTemp, "WinOdds_%d", i);
					CtrlAccount.nOdds = GetPrivateProfileInt(TEXT("ControlWin"), szTemp, 0, m_szConfigFileName);
					m_CtrlAccountArray.Add(CtrlAccount);
				}
			}
		}
	}
	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
	m_ApplyUserArray.RemoveAll();
	m_CtrlAccountArray.RemoveAll();
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

	return true;
}

//复位桌子
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//总下注数
	ZeroMemory(m_iTotalJettonScore, sizeof(m_iTotalJettonScore));

	//各区域可下分
	ZeroMemory(m_iAreaScoreLimit, sizeof(m_iAreaScoreLimit));

	//个人下注
	ZeroMemory(m_iUserAreaScore, sizeof(m_iUserAreaScore));

	//玩家成绩	
	ZeroMemory(m_iUserWinScore,sizeof(m_iUserWinScore));
	ZeroMemory(m_iUserReturnScore,sizeof(m_iUserReturnScore));
	ZeroMemory(m_iUserRevenue,sizeof(m_iUserRevenue));

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
	//变量定义
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));
	m_iGameTimes++;

	//获取庄家
	IServerUserItem *pIBankerServerUserItem=NULL;
	if (INVALID_CHAIR!=m_wCurrentBanker) 
		pIBankerServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);

	//设置变量
	GameStart.cbTimeLeave=TIME_PLACE_JETTON;
	GameStart.wBankerUser=m_wCurrentBanker;
	if (pIBankerServerUserItem!=NULL) 
		GameStart.iBankerScore=pIBankerServerUserItem->GetUserScore()->lScore;

	//每局游戏开始时刻，计算各区域初始可下分值
	CalculateAreaInitLimite();

	for( int i=0; i<JETTON_AREA_COUNT; i++ )
		GameStart.iAreaLimitScore[i] = m_iAreaScoreLimit[i];

    //发送给旁观玩家
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	

	//发送给游戏玩家
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (pIServerUserItem==NULL) 
			continue;

		//设置积分
		GameStart.iUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_iUserLimitScore);
		m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	
	}

	return true;
}

//每局游戏开始时刻，计算各区域初始可下分值
bool __cdecl CTableFrameSink::CalculateAreaInitLimite()
{
	//初始化变量
	__int64 iBankerScore = 0;
	IServerUserItem * pIBankerServerUserItem = NULL;

	//有人做庄，获得庄家信息
	if( INVALID_CHAIR != m_wCurrentBanker )
	{
		pIBankerServerUserItem = m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		if( pIBankerServerUserItem == NULL )
			return FALSE;

		//获得庄家当前的积分(金币)
		iBankerScore = pIBankerServerUserItem->GetUserScore()->lScore;

		//根据庄家金币数量，计算各区域的初始化可下分
		for( int i=0; i<JETTON_AREA_COUNT; i++ )
			m_iAreaScoreLimit[i] = __min(iBankerScore/(m_iOddsArray[i]+1), m_iMaxAreaScore);
	}
	else //如果无人做庄
	{
		if( m_bEnableSysBanker ) //如果允许系统做庄
		{
			for( int i=0; i<JETTON_AREA_COUNT; i++ )
			{
				if( i%2 == 0 )
				{
					m_iAreaScoreLimit[i] = __min( m_iSysLimitScore/(m_iOddsArray[i]/m_iOddsArray[i+1]), m_iMaxAreaScore);
				}
				else
				{
					m_iAreaScoreLimit[i] = m_iSysLimitScore;	//如果是低倍率区域，则该区域的限注为配置文件指定的限额
				}
			}
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}
//游戏结束
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束	
		{
			//派发扑克
			DispatchTableCard();

			//计算分数
			__int64 iBankerWinScore=CalculateScore();

			//递增次数
			m_wBankerTime++;

			//结束消息
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//庄家信息
			GameEnd.nBankerTime = m_wBankerTime;
			GameEnd.iBankerTotallScore=m_iBankerWinScore;
			GameEnd.iBankerScore=iBankerWinScore;
			GameEnd.cbGoalAnimal = m_iGoalAnimal;

			//发送积分
			GameEnd.cbTimeLeave=TIME_GAME_END;	
			for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserIndex);
				if ( pIServerUserItem == NULL ) 
					continue;

				//设置成绩
				GameEnd.iUserScore=m_iUserWinScore[wUserIndex];

				//返还积分
				GameEnd.iUserReturnScore=m_iUserReturnScore[wUserIndex];

				//设置税收
				if (m_iUserRevenue[wUserIndex]>0) 
					GameEnd.iRevenue=m_iUserRevenue[wUserIndex];
				else if (m_wCurrentBanker!=INVALID_CHAIR) 
					GameEnd.iRevenue=m_iUserRevenue[m_wCurrentBanker];
				else 
					GameEnd.iRevenue=0;

				//发送消息					
				m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			return true;
		}
	case GER_USER_LEFT:		//用户离开
		{
			//闲家离开，即刻结算
			if (m_wCurrentBanker!=wChairID)
			{
				//变量定义
				__int64 iScore=0;
				__int64 iRevenue=0;
				enScoreKind ScoreKind=enScoreKind_Flee;

				//统计成绩
				for( int i=0; i<JETTON_AREA_COUNT; i++)
					iScore += m_iUserAreaScore[wChairID][i];
				iScore = -iScore;

				//写入积分
				if (iScore!=0L) 
					m_pITableFrame->WriteUserScore(pIServerUserItem, iScore,iRevenue, ScoreKind);

				return true;
			}

			//庄家离开，状态判断
			if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
			{
				//提示消息
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("由于庄家[ %s ]强退，游戏提前结束！"),pIServerUserItem->GetAccounts());

				//发送消息
				SendGameMessage(INVALID_CHAIR,szTipMsg);	

				//设置状态
				m_pITableFrame->SetGameStatus(GS_GAME_END);

				//设置时间
				m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,TIME_GAME_END*1000,1,0L);

				//派发扑克
				DispatchTableCard();

				//计算分数
				CalculateScore();

				//结束消息
				CMD_S_GameEnd GameEnd;
				ZeroMemory(&GameEnd,sizeof(GameEnd));

				//庄家信息
				GameEnd.nBankerTime = m_wBankerTime;
				GameEnd.iBankerTotallScore=m_iBankerWinScore;
				if (m_iBankerWinScore>0) 
					GameEnd.iBankerScore=0;

				//发送积分
				GameEnd.cbTimeLeave=TIME_GAME_END;	
				for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
				{
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserIndex);
					if ( pIServerUserItem == NULL ) 
						continue;

					//设置成绩
					GameEnd.iUserScore=m_iUserWinScore[wUserIndex];

					//返还积分
					GameEnd.iUserReturnScore=m_iUserReturnScore[wUserIndex];

					//设置税收
					if (m_iUserRevenue[wUserIndex]>0) 
						GameEnd.iRevenue=m_iUserRevenue[wUserIndex];
					else if (m_wCurrentBanker!=INVALID_CHAIR) 
						GameEnd.iRevenue=m_iUserRevenue[m_wCurrentBanker];
					else 
						GameEnd.iRevenue=0;

					//发送消息					
					m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				}
			}

			//扣除分数
			if (m_iUserWinScore[m_wCurrentBanker]<0)
			{
				m_pITableFrame->WriteUserScore(m_wCurrentBanker,m_iUserWinScore[m_wCurrentBanker], m_iUserRevenue[m_wCurrentBanker], enScoreKind_Flee);
			}

			//切换庄家
			ChangeBanker(true);

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
			SendGameRecord(pIServerUserItem);

			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));			

			//控制信息
			StatusFree.iApplyBankerCondition = m_iApplyBankerCondition;
			for( int i=0; i<JETTON_AREA_COUNT; i++)
				StatusFree.iAreaLimitScore[i] = m_iAreaScoreLimit[i];

			//庄家信息
			StatusFree.bEnableSysBanker=m_bEnableSysBanker;
			StatusFree.wBankerUser=m_wCurrentBanker;	
			StatusFree.cbBankerTime=m_wBankerTime;
			StatusFree.iBankerWinScore=m_iBankerWinScore;
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
				StatusFree.iBankerScore=pIServerUserItem->GetUserScore()->lScore;
			}

			//玩家信息
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusFree.iUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_iUserLimitScore); 
			}

			//全局信息
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusFree.cbTimeLeave=(BYTE)(TIME_FREE-__min(dwPassTime,TIME_FREE));

			//发送场景
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

			//限制提示
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄条件为：%I64d, 玩家限制为：%I64d"),m_iApplyBankerCondition,m_iUserLimitScore);
			
			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);
						
			//发送申请者
			SendApplyUser(pIServerUserItem);

			return bSuccess;
		}
	case GS_PLACE_JETTON:		//游戏状态
	case GS_GAME_END:			//结束状态
		{
			//发送记录
			SendGameRecord(pIServerUserItem);		

			//构造数据
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//全局下注
			for (int i=0; i<JETTON_AREA_COUNT; i++ )
			{
				StatusPlay.iTotalAreaScore[i] = m_iTotalJettonScore[i];
				StatusPlay.iAreaScoreLimit[i] = m_iAreaScoreLimit[i];
			}

			//玩家下注
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				for(int i=0; i<JETTON_AREA_COUNT; i++)
					StatusPlay.iUserAreaScore[i] = m_iUserAreaScore[wChiarID][i];

				//最大下注
				StatusPlay.iUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_iUserLimitScore);
			}

			//控制信息
			StatusPlay.iApplyBankerCondition=m_iApplyBankerCondition;		

			//庄家信息
			StatusPlay.bEnableSysBanker=m_bEnableSysBanker;
			StatusPlay.wBankerUser=m_wCurrentBanker;			
			StatusPlay.cbBankerTime=m_wBankerTime;
			StatusPlay.iBankerWinScore=m_iBankerWinScore;	
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
				StatusPlay.iBankerScore=pIServerUserItem->GetUserScore()->lScore;
			}	

			//全局信息
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusPlay.cbTimeLeave=(BYTE)(TIME_FREE-__min(dwPassTime,TIME_FREE));
			StatusPlay.cbGameStatus=m_pITableFrame->GetGameStatus();			

			//结束判断
			if (cbGameStatus==GS_GAME_END)
			{
				//设置成绩
				StatusPlay.iEndUserScore=m_iUserWinScore[wChiarID];

				//返还积分
				StatusPlay.iEndUserReturnScore=m_iUserReturnScore[wChiarID];

				//设置税收
				if (m_iUserRevenue[wChiarID]>0) 
					StatusPlay.iEndRevenue=m_iUserRevenue[wChiarID];
				else if (m_wCurrentBanker!=INVALID_CHAIR) 
					StatusPlay.iEndRevenue=m_iUserRevenue[m_wCurrentBanker];
				else 
					StatusPlay.iEndRevenue=0;

				//庄家成绩
				StatusPlay.iEndBankerScore=m_iBankerCurGameScore;

			}

			//发送申请者
			SendApplyUser( pIServerUserItem );

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	return false;
}

//定时器事件
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_FREE:		//空闲时间结束
		{
			//开始游戏
			m_pITableFrameControl->StartGame();

			//启动下注计时器
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,TIME_PLACE_JETTON*1000,1,0L);

			//设置游状态为下注阶段
			m_pITableFrame->SetGameStatus(GS_PLACE_JETTON);

			return true;
		}
	case IDI_PLACE_JETTON:		//下注时间结束
		{
			//状态判断(防止强退重复设置)
			if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
			{
				//设置状态
				m_pITableFrame->SetGameStatus(GS_GAME_END);			

				//结束游戏
				OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);

				//设置时间
				m_pITableFrame->SetGameTimer(IDI_GAME_END,TIME_GAME_END*1000,1,0L);			
			}

			return true;
		}
	case IDI_GAME_END:			//结束游戏
		{
			//写入积分
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserChairID);
				if ( pIServerUserItem == NULL ) 
					continue;

				//胜利类型
				enScoreKind ScoreKind=(m_iUserWinScore[wUserChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;

				//写入积分
				if (m_iUserWinScore[wUserChairID]!=0L) 
					m_pITableFrame->WriteUserScore(wUserChairID,m_iUserWinScore[wUserChairID], m_iUserRevenue[wUserChairID], ScoreKind);

				//坐庄条件判断。一轮游戏结束，玩家有输有赢。所以需要在申请上庄队列里把输了钱不在满足做庄的玩家清除掉
				__int64 iUserScore=pIServerUserItem->GetUserScore()->lScore;
				if (wUserChairID!=m_wCurrentBanker && iUserScore<m_iApplyBankerCondition)
				{
					for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
					{
						if (wUserChairID!=m_ApplyUserArray[i]) continue;

						//从庄家队列删除不符合上庄的玩家
						m_ApplyUserArray.RemoveAt(i);

						//发送消息
						CMD_S_CancelBanker CancelBanker;
						ZeroMemory(&CancelBanker,sizeof(CancelBanker));

						//设置变量
						lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));

						//发送消息
						m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
						m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

						//提示消息
						TCHAR szTipMsg[128];
						_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("由于你的金币数少于坐庄必须金币数（%I64d）,你自动下庄！"),m_iApplyBankerCondition);
						SendGameMessage(wUserChairID,szTipMsg);

						break;
					}
				}
			}

			//结束游戏
			m_pITableFrame->ConcludeGame();

			//切换庄家
			ChangeBanker(false);

			//设置时间
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,TIME_FREE*1000,1,0L);

			//发送消息
			CMD_S_GameFree GameFree;
			ZeroMemory(&GameFree,sizeof(GameFree));
			GameFree.cbTimeLeave=TIME_FREE;
			GameFree.cbGameRecord = m_iGoalAnimal;
			GameFree.iGameTimes = m_iGameTimes;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));

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
			if (wDataSize!=sizeof(CMD_C_PlaceJetton)) 
				return false;

			if( pIServerUserItem == NULL )
				return true;

			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if( pUserData == NULL )
				return true;

			if (pUserData->cbUserStatus!=US_PLAY) 
				return true;

			//消息处理
			CMD_C_PlaceJetton * pPlaceJetton=(CMD_C_PlaceJetton *)pDataBuffer;
			if( pPlaceJetton == NULL )
				return FALSE;

			return OnUserPlaceJetton(pUserData->wChairID,pPlaceJetton->cbJettonArea,pPlaceJetton->iJettonScore);
		}
	case SUB_C_APPLY_BANKER:		//申请做庄
		{
			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if( pUserData == NULL )
				return FALSE;

			if (pUserData->cbUserStatus==US_LOOKON)
				return true;

			return OnUserApplyBanker(pIServerUserItem);	
		}
	case SUB_C_CANCEL_BANKER:		//取消做庄
		{
			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if( pUserData == NULL )
				return FALSE;
			if (pUserData->cbUserStatus==US_LOOKON)
				return true;

			return OnUserCancelBanker(pIServerUserItem);	
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
		m_pITableFrame->SetGameStatus(GS_FREE);
	}

	//限制提示
	TCHAR szTipMsg[128];
	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("此房间上庄条件为：%I64d, 玩家限制为：%I64d"),m_iApplyBankerCondition,m_iUserLimitScore);
	m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);

	return true;
}

//用户起来
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//记录成绩
	if (bLookonUser==false)
	{
		//切换庄家
		if (wChairID==m_wCurrentBanker) ChangeBanker(true);

		//取消申请
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			if (wChairID!=m_ApplyUserArray[i]) 
				continue;

			//删除玩家
			m_ApplyUserArray.RemoveAt(i);

			//构造变量
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//设置变量
			lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));

			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

			break;
		}

		return true;
	}

	return true;
}

//加注事件
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, __int64 iJettonScore)
{
	//效验参数
	ASSERT((cbJettonArea<=ID_SMALL_SNAKE)&&(iJettonScore>0L));
	if ((cbJettonArea>ID_SMALL_SNAKE)||(iJettonScore<=0L)) 
		return false;

	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus()==GS_PLACE_JETTON);
	if (m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON) 
		return true;

	//庄家判断，如果是庄家，则不应该下注
	if (m_wCurrentBanker==wChairID) 
		return true;

	//如果当前不允许系统做庄，且没有庄家，则返回
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) 
		return true;

	//取得玩家资料
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	if( pIServerUserItem == NULL )
		return FALSE;

	//计算该玩家在各个区域所下注数额的总和
	__int64 iJettonCount = 0;
	for( int i=0; i<JETTON_AREA_COUNT; i++ )
		iJettonCount += m_iUserAreaScore[wChairID][i];


	//计算该玩家当前总共具有多少积分（金币）
	__int64 iUserScore = pIServerUserItem->GetUserScore()->lScore;

	//合法校验，如果已下注额和即将下注数额相加大于玩家具备的积分，则返回
	if (iUserScore < iJettonCount + iJettonScore) 
		return true;

	//如果已下注额和即将下注数额相加大于玩家的限注，则返回
	if (m_iUserLimitScore < iJettonCount + iJettonScore) 
		return true;

	//成功标识
	bool bPlaceJettonSuccess=false;
	if( cbJettonArea > ID_SMALL_SNAKE || cbJettonArea < ID_BIG_TIGER )
		return true;

	//如果某区域已下总注加上即将下注数额小于等于该区域当前的下注上限，则进行操作，否则返回
	if( m_iTotalJettonScore[cbJettonArea] + iJettonScore <= m_iAreaScoreLimit[cbJettonArea ])
	{
		m_iTotalJettonScore[cbJettonArea] += iJettonScore;
		m_iUserAreaScore[wChairID][cbJettonArea] += iJettonScore;
		m_iAreaScoreLimit[cbJettonArea] -= iJettonScore;
		bPlaceJettonSuccess = true;
	}
	else
	{
		return true;
	}

	//加注成功
	if (bPlaceJettonSuccess)
	{
		//变量定义
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

		//构造变量
		PlaceJetton.wChairID=wChairID;
		PlaceJetton.cbJettonArea=cbJettonArea;
		PlaceJetton.iJettonScore=iJettonScore;

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
	}
	else	//加注失败
	{
		CMD_S_PlaceJettonFail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
		PlaceJettonFail.lJettonArea=cbJettonArea;
		PlaceJettonFail.iPlaceScore=iJettonScore;

		//发送消息
		m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
	}

	return true;
}

//发送扑克
bool CTableFrameSink::DispatchTableCard( )
{
	//系统要求控制胜率
	if( m_bControlWin )
	{
		if( m_wCurrentBanker != INVALID_CHAIR )	//当前有人做庄
		{
			//取得玩家资料
			IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
			if( pIServerUserItem == NULL )
				return FALSE;
			bool bIsInQueue = false;
			int nWinOdds = 0;

			for( int i=0; i< m_CtrlAccountArray.GetCount(); i++ )
			{
				ControlAccount tempAccount = m_CtrlAccountArray.GetAt(i);
				if( tempAccount.dwUserID == pIServerUserItem->GetUserData()->dwUserID )
				{
					nWinOdds = tempAccount.nOdds;
					bIsInQueue = true;

					break;
				}
			}

			//如果当前庄家在控制胜率队列里
			if( bIsInQueue )
			{
				if( nWinOdds > 0 && nWinOdds <= 100 )
				{
					CtrlDispatchCard(nWinOdds);
				}
				else	//乱填胜率，按常规处理
				{
					NormalDispatchCard();
				}
			}
			else	//按常规处理
			{
				NormalDispatchCard();
			}
		}
		else //当前无人做庄
		{
			//允许系统做庄
			if( m_bEnableSysBanker )
				CtrlDispatchCard();
		}
	}
	else
	{
		NormalDispatchCard();
	}
	return true;
}

//常规发牌
void CTableFrameSink::NormalDispatchCard()
{
	static int nBig=0;	//开大的机会,10次开一次
	static bool bHasBig=false;	//控制周期内是否开过大
	m_iGoalAnimal = rand()%PATH_STEP_NUMBER;
	if( m_iGoalAnimal%2==0 )	//如果本次开大
	{
		if( bHasBig )	//如果控制周期内已经开过大,否则不做处理
		{
			m_iGoalAnimal = (m_iGoalAnimal+1)%PATH_STEP_NUMBER;
		}
		else
		{
			bHasBig = true;
		}
	}
	nBig++;
	if( nBig >= 10 )
	{
		bHasBig = false;
		nBig = 0;
	}
}

//控制发牌
void CTableFrameSink::CtrlDispatchCard(int nCtrlWinOdds)
{
	//某庄家在其做庄周期内确保赢的次数
	int nCanWinTimes = m_nMaxBankerTimes * nCtrlWinOdds/100;

	//还剩多少局就得下庄
	int nLeaveTimes = m_nMaxBankerTimes - m_wBankerTime;

	//还能赢多少局
	int nLeaveWinTimes = nCanWinTimes - m_nHasWinTimes;

	CString strMsg;

	if( nLeaveTimes > nLeaveWinTimes )	//如果剩余局数大于剩余赢局，表示还可以随机一下
	{
		m_iGoalAnimal = rand()%PATH_STEP_NUMBER;
		__int64 nRet = 0;
		bool bRet = GetOneAreaWinOdds(m_iGoalAnimal%8, nRet);
		if( bRet && nRet > 0 )	//如果随机出来的结果也是赢，则应该算上
		{
			m_nHasWinTimes ++;
		}
	}
	else	//如果剩余赢局小于等于剩余局数，则把把赢
	{
		int n = CalculateWinOdds();
		m_iGoalAnimal = n + (rand()%3) * JETTON_AREA_COUNT;
		m_nHasWinTimes ++;
	}
}

//申请庄家
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	if( pIApplyServerUserItem == NULL )
		return true;

	//合法判断
	__int64 lUserScore=pIApplyServerUserItem->GetUserScore()->lScore;
	if (lUserScore<m_iApplyBankerCondition)
	{
		m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("你的金币不足以申请庄家，申请失败！"),SMT_INFO|SMT_EJECT);
		return true;
	}

	//存在判断
	WORD wApplyUserChairID=pIApplyServerUserItem->GetChairID();
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];
		if (wChairID==wApplyUserChairID)
		{
			m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("你已经申请了庄家，不需要再次申请！"),SMT_INFO|SMT_EJECT);
			return true;
		}
	}

	//保存信息 
	m_ApplyUserArray.Add(wApplyUserChairID);

	//构造变量
	CMD_S_ApplyBanker ApplyBanker;
	ZeroMemory(&ApplyBanker,sizeof(ApplyBanker));

	//设置变量
	ApplyBanker.wApplyUser=wApplyUserChairID;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

	//切换判断
	if (m_pITableFrame->GetGameStatus()==GS_FREE && m_ApplyUserArray.GetCount()==1)
	{
		ChangeBanker(false);
	}

	return true;
}

//取消申请
bool CTableFrameSink::OnUserCancelBanker(IServerUserItem *pICancelServerUserItem)
{
	//当前庄家
	if (pICancelServerUserItem->GetChairID()==m_wCurrentBanker && m_pITableFrame->GetGameStatus()!=GS_FREE)
	{
		//发送消息
		m_pITableFrame->SendGameMessage(pICancelServerUserItem,TEXT("游戏已经开始，不可以取消当庄！"),SMT_INFO|SMT_EJECT);
		return true;
	}

	//存在判断
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		//获取玩家
		WORD wChairID=m_ApplyUserArray[i];
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);

		//条件过滤
		if (pIServerUserItem==NULL) 
			continue;
		if (pIServerUserItem->GetUserID()!=pICancelServerUserItem->GetUserID()) 
			continue;

		//删除玩家
		m_ApplyUserArray.RemoveAt(i);

		if (m_wCurrentBanker!=wChairID)
		{
			//构造变量
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//设置变量
			lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));

			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		}
		else if (m_wCurrentBanker==wChairID)
		{
			//切换庄家 
			m_wCurrentBanker=INVALID_CHAIR;
			ChangeBanker(true);
		}

		return true;
	}

	return false;
}

//更换庄家
bool CTableFrameSink::ChangeBanker(bool bCancelCurrentBanker)
{
	//切换标识
	bool bChangeBanker=false;

	//取消当前庄家做庄资格
	if (bCancelCurrentBanker)
	{
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			//获取玩家
			WORD wChairID=m_ApplyUserArray[i];

			//条件过滤
			if (wChairID!=m_wCurrentBanker) 
				continue;

			//删除玩家
			m_ApplyUserArray.RemoveAt(i);

			break;
		}

		//设置庄家
		m_wCurrentBanker=INVALID_CHAIR;

		//轮换判断
		if (m_pITableFrame->GetGameStatus()==GS_FREE && m_ApplyUserArray.GetCount()!=0)
		{
			m_wCurrentBanker=m_ApplyUserArray[0];
		}

		//设置变量
		bChangeBanker=true;
	}
	//轮庄判断。当前庄家暂时还有机会做庄
	else if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//获取庄家
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		if( pIServerUserItem == NULL )
			return FALSE;

		__int64 iBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//次数判断。如果超过了做庄的次数或者所剩积分不再满足做庄条件，则强制替换掉
		if (m_nMaxBankerTimes<=m_wBankerTime || iBankerScore<m_iApplyBankerCondition)
		{
			//撤销玩家
			for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
			{
				//获取玩家
				WORD wChairID=m_ApplyUserArray[i];

				//条件过滤
				if (wChairID!=m_wCurrentBanker) continue;

				//删除玩家
				m_ApplyUserArray.RemoveAt(i);

				break;
			}

			//设置庄家
			m_wCurrentBanker=INVALID_CHAIR;

			//轮换判断
			if (m_ApplyUserArray.GetCount()!=0)
			{
				m_wCurrentBanker=m_ApplyUserArray[0];
			}

			bChangeBanker=true;

			//提示消息
			TCHAR szTipMsg[128];
			if (iBankerScore<m_iApplyBankerCondition)
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]分数少于(%I64d)，强行换庄!"),pIServerUserItem->GetAccounts(),m_iApplyBankerCondition);
			else
            	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]做庄次数达到(%d)，强行换庄!"),pIServerUserItem->GetAccounts(),m_nMaxBankerTimes);
			
			//发送消息
			SendGameMessage(INVALID_CHAIR,szTipMsg);	
		}
	}
	//如果当前是系统做庄，则可以换一个真人玩家来做庄
	else if (m_wCurrentBanker==INVALID_CHAIR && m_ApplyUserArray.GetCount()!=0)
	{
		m_wCurrentBanker=m_ApplyUserArray[0];
		bChangeBanker=true;
	}

	//切换判断
	if (bChangeBanker)
	{
		//设置变量
		m_wBankerTime = 0;
		m_iBankerWinScore=0;

		//发送消息
		CMD_S_ChangeBanker ChangeBanker;
		ZeroMemory(&ChangeBanker,sizeof(ChangeBanker));
		ChangeBanker.wBankerUser=m_wCurrentBanker;
		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
			if( pIServerUserItem == NULL )
				return FALSE;
			ChangeBanker.iBankerScore=pIServerUserItem->GetUserScore()->lScore;
		}
		else
		{
			//如果if的条件不满足，应该如何处理？
		}
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&ChangeBanker,sizeof(ChangeBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&ChangeBanker,sizeof(ChangeBanker));

		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			//读取消息
			LONG lMessageCount=GetPrivateProfileInt(TEXT("Message"),TEXT("MessageCount"),0,m_szConfigFileName);
			if (lMessageCount!=0)
			{
				//读取配置
				LONG lIndex=rand()%lMessageCount;
				TCHAR szKeyName[32],szMessage1[256],szMessage2[256];				
				_sntprintf(szKeyName,CountArray(szKeyName),TEXT("Item%ld"),lIndex);
				GetPrivateProfileString(TEXT("Message"),szKeyName,TEXT("恭喜[ %s ]上庄"),szMessage1,CountArray(szMessage1),m_szConfigFileName);

				//获取玩家
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);

				//发送消息
				_sntprintf(szMessage2,CountArray(szMessage2),szMessage1,pIServerUserItem->GetAccounts());
				SendGameMessage(INVALID_CHAIR,szMessage2);
			}
		}
	}

	return bChangeBanker;
}



//发送申请上庄的玩家列表
void CTableFrameSink::SendApplyUser( IServerUserItem *pRcvServerUserItem )
{
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];

		//获取玩家
		IServerUserItem *pServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (pServerUserItem == NULL) 
			continue;

		//庄家判断
		if (pServerUserItem->GetChairID()==m_wCurrentBanker) 
			continue;

		//构造变量
		CMD_S_ApplyBanker ApplyBanker;
		ZeroMemory(&ApplyBanker, sizeof(ApplyBanker));
		ApplyBanker.wApplyUser=wChairID;

		//发送消息
		m_pITableFrame->SendUserData(pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	}
}

//用户断线
bool __cdecl CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	//切换庄家
	if (wChairID==m_wCurrentBanker) 
		ChangeBanker(true);

	//取消申请
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		if (wChairID!=m_ApplyUserArray[i]) 
			continue;

		//删除玩家
		m_ApplyUserArray.RemoveAt(i);

		//构造变量
		CMD_S_CancelBanker CancelBanker;
		ZeroMemory(&CancelBanker,sizeof(CancelBanker));

		//设置变量
		lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

	return true;
}

//计算得分
__int64 CTableFrameSink::CalculateScore()
{
	//变量定义
	__int64 cbRevenue=m_pGameServiceOption->wRevenue;

	//游戏记录
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.cbRecord = m_iGoalAnimal;
	GameRecord.cbGameTimes = m_iGameTimes;

	//移动下标
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst ) 
		m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;

	//庄家本局成绩
	__int64 iBankerWinScore = 0;

	//玩家成绩
	__int64 iUserLostScore[GAME_PLAYER];

	//玩家成绩
	ZeroMemory(m_iUserWinScore, sizeof(m_iUserWinScore));
	ZeroMemory(m_iUserRevenue, sizeof(m_iUserRevenue));
	ZeroMemory(iUserLostScore, sizeof(iUserLostScore));
	
	//中奖区域
	BYTE cbHotArea = m_iGoalAnimal % (JETTON_AREA_COUNT);

	//计算积分
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//庄家判断
		if (m_wCurrentBanker==i) 
			continue;

		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem==NULL) 
			continue;

		for(int iArea=0; iArea<JETTON_AREA_COUNT; iArea++ )
		{
			//某个区域压中
			if( cbHotArea == iArea )	
			{
				m_iUserWinScore[i] += m_iUserAreaScore[i][iArea] * m_iOddsArray[iArea];
				m_iUserReturnScore[i] += m_iUserAreaScore[i][iArea];
				iBankerWinScore -= m_iUserAreaScore[i][iArea] * m_iOddsArray[iArea];
			}
			else
			{
				iUserLostScore[i] -= m_iUserAreaScore[i][iArea];
				iBankerWinScore += m_iUserAreaScore[i][iArea];
			}
		}

		//计算税收
		if (0 < m_iUserWinScore[i])
		{
			float fRevenuePer=float(cbRevenue/1000.+0.000001);
			m_iUserRevenue[i]  = __int64(m_iUserWinScore[i]*fRevenuePer);
			m_iUserWinScore[i] -= m_iUserRevenue[i];
		}

		//总的分数
		m_iUserWinScore[i] += iUserLostScore[i];
	}

	//庄家成绩
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		m_iUserWinScore[m_wCurrentBanker] = iBankerWinScore;

		//计算税收
		if (0 < m_iUserWinScore[m_wCurrentBanker])
		{
			float fRevenuePer=float(cbRevenue/1000.+0.000001);
			m_iUserRevenue[m_wCurrentBanker]  = __int64(m_iUserWinScore[m_wCurrentBanker]*fRevenuePer);
			m_iUserWinScore[m_wCurrentBanker] -= m_iUserRevenue[m_wCurrentBanker];
			iBankerWinScore = m_iUserWinScore[m_wCurrentBanker];
		}		
	}

	//累计积分
	m_iBankerWinScore += iBankerWinScore;

	//当前积分
	m_iBankerCurGameScore=iBankerWinScore;

	return iBankerWinScore;
}

//计算某个区域的输赢情况，返回输赢的金币数
bool CTableFrameSink::GetOneAreaWinOdds(int nAreaIndex, __int64 & iResult)
{
	iResult = 0;
	if( nAreaIndex > ID_SMALL_SNAKE || nAreaIndex < ID_BIG_TIGER )
		return false;

	__int64 nAllUserScore = 0;
	__int64 nTempScore = 0;

	for( int i=0; i<JETTON_AREA_COUNT; i++)
		nAllUserScore += m_iTotalJettonScore[i];

	for( int j=0; j<GAME_PLAYER; j++)
	{
		if( m_wCurrentBanker == j )
			continue;

		IServerUserItem * pUserItem = m_pITableFrame->GetServerUserItem(j);
		if( pUserItem == NULL )
			continue;

		nTempScore += m_iUserAreaScore[j][nAreaIndex];
	}

	iResult = nAllUserScore - nTempScore * m_iOddsArray[nAreaIndex];
	return true;
}

//胜率计算，系统做庄或作弊时用。返回值为最有利的胜利区域，范围在0-7
int	CTableFrameSink::CalculateWinOdds()
{
	//保存每个区域作为庄家来说，能赢取的金币
	__int64 nWinScore[JETTON_AREA_COUNT]={0};
	__int64 nTempScore = 0;
	__int64 nAllScore = 0;
	__int64 nSortResult = 0;
	int nRet = ID_SMALL_SNAKE;	//返回值

	//计算本轮所下的总注
	for(int k=0; k<JETTON_AREA_COUNT; k++ )
		nAllScore += m_iTotalJettonScore[k];


	for( int i=0; i<JETTON_AREA_COUNT; i++ )
	{
		//i区域所下总注乘以倍率，看如果开中该区域，庄家输赢情况
		GetOneAreaWinOdds(i, nWinScore[i]);
	}

	//系统做庄，赢最大的注
	if( m_bEnableSysBanker && m_wCurrentBanker == INVALID_CHAIR )
	{
		for( int t=0; t<JETTON_AREA_COUNT; t++ )
		{

			if( nWinScore[t] > nSortResult )
			{
				nSortResult = nWinScore[t];
				nRet = t;
			}
		}
	}
	else	//如果常规庄家，则只要赢就可以了
	{
		for( int i=0; i<JETTON_AREA_COUNT; i++ )
		{
			if( nWinScore[i] > 0 )
			{
				nRet = i;
				break;;
			}
		}
	}
	
	return nRet;
}

//发送记录
void CTableFrameSink::SendGameRecord(IServerUserItem *pIServerUserItem)
{
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
	if (wBufferSize>0) 
		m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
}

//发送消息
void CTableFrameSink::SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg)
{
	if (wChairID==INVALID_CHAIR)
	{
		//游戏玩家
		for (WORD i=0; i<GAME_PLAYER; ++i)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
			if (pIServerUserItem==NULL) continue;
			m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_INFO);
		}

		//旁观玩家
		WORD wIndex=0;
		do {
			IServerUserItem *pILookonServerUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
			if (pILookonServerUserItem==NULL) break;

			m_pITableFrame->SendGameMessage(pILookonServerUserItem,pszTipMsg,SMT_INFO);

		}while(true);
	}
	else
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (pIServerUserItem!=NULL) m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_INFO|SMT_EJECT);
	}
}
//////////////////////////////////////////////////////////////////////////

