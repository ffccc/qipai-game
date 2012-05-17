#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////
//宏定义
//常量定义
#define SEND_COUNT					300									//发送次数

//索引定义
#define INDEX_PLAYER				0									//闲家索引
#define INDEX_BANKER				1									//庄家索引

//下注时间
#define IDI_FREE					1									//空闲时间
#define TIME_FREE					20									//空闲时间

//下注时间
#define IDI_PLACE_JETTON			2									//下注时间
#define TIME_PLACE_JETTON			20									//下注时间

//结束时间
#define IDI_GAME_END				3									//结束时间

#define TIME_GAME_END				22									//结束时间
//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//游戏人数
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_TimeControl;	//开始模式

//////////////////////////////////////////////////////////////////////////

//区域索引转换
BYTE g_cbTurnAreaIndex[][4]={{},
							{7,21},
							{},
							{},
							{3,10,18,25},
							{4,11,17,24},
							{8,16,27},
							{6,15,26},
							{1,12,20},
							{2,13,22},
							{5,23},
							{9,19}};

//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{
	InitializeCriticalSection(&m_csPlaceJetton);
	//庄家信息
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker=INVALID_CHAIR;
	m_wBankerTime=0;
	m_lBankerWinScore=0L;		
	m_lBankerCurGameScore=0L;
	m_bEnableSysBanker=true;
	m_bChangeBanker=false;
	//玩家成绩	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));
	//状态变量
	m_dwJettonTime=0L;
	//全体下注
	m_lALLBigSharkScore=0L;
	m_lALLFeiQinScore=0L;
	m_lALLZouShouScore=0L;
	m_lALLYanZiScore=0L;				
	m_lALLTuZiScore=0L;				
	m_lALLGeZiScore=0L;					
	m_lALLXiongMaoScore=0L;					
	m_lALLKongQueScore=0L;					
	m_lALLHouZiScore=0L;					
	m_lALLLaoYingScore=0L;					
	m_lALLShiZiScore=0L;
	//控制变量
	m_cbWinSideControl=0;
	m_cbMoveEndIndex = 0;
	m_cbResultIndex = 0;
	m_cbEndIndex = 0;
	//个人下注
	ZeroMemory(m_lUserBigSharkScore,sizeof(m_lUserBigSharkScore));
	ZeroMemory(m_lUserFeiQinScore,sizeof(m_lUserFeiQinScore));
	ZeroMemory(m_lUserZouShouScore,sizeof(m_lUserZouShouScore));
	ZeroMemory(m_lUserYanZiScore,sizeof(m_lUserYanZiScore));
	ZeroMemory(m_lUserTuZiScore,sizeof(m_lUserTuZiScore));
	ZeroMemory(m_lUserGeZiScore,sizeof(m_lUserGeZiScore));
	ZeroMemory(m_lUserXiongMaoScore,sizeof(m_lUserXiongMaoScore));
	ZeroMemory(m_lUserKongQueScore,sizeof(m_lUserKongQueScore));
	ZeroMemory(m_lUserHouZiScore,sizeof(m_lUserHouZiScore));
	ZeroMemory(m_lUserLaoYingScore,sizeof(m_lUserLaoYingScore));
	ZeroMemory(m_lUserShiZiScore,sizeof(m_lUserShiZiScore));							
	m_cbScoreHistory.RemoveAll();
	m_cbZuoBiIndex = 0;
	m_cbZuoBiCount = 0;
	m_cbLastResultIndex = 0;
	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
	DeleteCriticalSection(&m_csPlaceJetton);
}

//接口查询
void * __cdecl CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableFrameEvent,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//初始化
bool __cdecl CTableFrameSink::InitTableFrameSink(IUnknownEx * pIUnknownEx)
{
	//查询接口
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=GET_OBJECTPTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//控制接口
	m_pITableFrameControl=GET_OBJECTPTR_INTERFACE(pIUnknownEx,ITableFrameControl);
	if (m_pITableFrameControl==NULL) return false;

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//读取配置
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);

	//读取配置
	TCHAR szFileName[MAX_PATH];
	_snprintf(szFileName,sizeof(szFileName),TEXT("%s\\SharkConfig.ini"),szPath);

	//控制变量
	LONG lMaxScore=2147483647;
	m_lApplyBankerCondition=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Score"), 100, szFileName);
	m_lAreaLimitScore=GetPrivateProfileInt(TEXT("ScoreLimit"), TEXT("AreaLimitScore"), lMaxScore, szFileName);
	m_lUserLimitScore=GetPrivateProfileInt(TEXT("ScoreLimit"), TEXT("UserLimitScore"), lMaxScore, szFileName);
	BYTE cbEnableSysBanker=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("EnableSysBanker"), 1, szFileName);
	m_bEnableSysBanker=cbEnableSysBanker?true:false;
	m_bMaxBankerTime = GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Time"), 5, szFileName);

	return true;
}

//复位桌子
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//全体下注
	m_lALLBigSharkScore=0L;
	m_lALLFeiQinScore=0L;
	m_lALLZouShouScore=0L;
	m_lALLYanZiScore=0L;				
	m_lALLTuZiScore=0L;				
	m_lALLGeZiScore=0L;					
	m_lALLXiongMaoScore=0L;					
	m_lALLKongQueScore=0L;					
	m_lALLHouZiScore=0L;					
	m_lALLLaoYingScore=0L;					
	m_lALLShiZiScore=0L;
	//个人下注
	ZeroMemory(m_lUserBigSharkScore,sizeof(m_lUserBigSharkScore));
	ZeroMemory(m_lUserFeiQinScore,sizeof(m_lUserFeiQinScore));
	ZeroMemory(m_lUserZouShouScore,sizeof(m_lUserZouShouScore));
	ZeroMemory(m_lUserYanZiScore,sizeof(m_lUserYanZiScore));
	ZeroMemory(m_lUserTuZiScore,sizeof(m_lUserTuZiScore));
	ZeroMemory(m_lUserGeZiScore,sizeof(m_lUserGeZiScore));
	ZeroMemory(m_lUserXiongMaoScore,sizeof(m_lUserXiongMaoScore));
	ZeroMemory(m_lUserKongQueScore,sizeof(m_lUserKongQueScore));
	ZeroMemory(m_lUserHouZiScore,sizeof(m_lUserHouZiScore));
	ZeroMemory(m_lUserLaoYingScore,sizeof(m_lUserLaoYingScore));
	ZeroMemory(m_lUserShiZiScore,sizeof(m_lUserShiZiScore));

	m_cbMoveEndIndex = 0;
	m_cbResultIndex = 0;
	m_cbEndIndex = 0;
	
	//玩家成绩	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));
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

	//获取庄家
	IServerUserItem *pIBankerServerUserItem=NULL;
	if (INVALID_CHAIR!=m_wCurrentBanker) 
		pIBankerServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);

	//设置变量
	GameStart.cbTimeLeave=TIME_PLACE_JETTON;
	GameStart.wBankerUser=m_wCurrentBanker;
	if (pIBankerServerUserItem!=NULL) 
		GameStart.lBankerScore=pIBankerServerUserItem->GetUserScore()->lScore;

	//旁观玩家
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	

	//游戏玩家
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;

		//设置积分
		GameStart.lUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_lUserLimitScore);

		m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	
	}
	return true;
}

//游戏结束
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束
	case GER_USER_LEFT:		//用户离开
		{
			if (cbReason==GER_USER_LEFT)
			{
				//庄家判断
				if (m_wCurrentBanker==wChairID)
				{
					//提示消息
					TCHAR szTipMsg[128];
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("由于庄家[ %s ]强退，游戏提前结束！"),pIServerUserItem->GetAccounts());

					//发送消息
					SendGameMessage(INVALID_CHAIR,szTipMsg);	

					//设置状态
					m_pITableFrame->SetGameStatus(GS_GAME_END);
					
					//停止时间
					m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);

					//设置时间
					m_pITableFrame->SetGameTimer(IDI_GAME_END,TIME_GAME_END*1000,1,0L);
				}
				else
				{
					//变量定义
					LONG lScore=0;
					LONG lRevenue=0;
					enScoreKind ScoreKind=enScoreKind_Flee;
					
					if (m_pITableFrame->GetGameStatus()==GS_GAME_END)
					{
						//写入积分
						//m_pITableFrame->WriteUserScore(pIServerUserItem,m_lUserWinScore[wChairID], m_lUserRevenue[wChairID], ScoreKind);
					}
					else
					{
						//统计成绩
						lScore=-( m_lUserYanZiScore[wChairID] + m_lUserTuZiScore[wChairID] + m_lUserGeZiScore[wChairID] + 
							m_lUserXiongMaoScore[wChairID] + m_lUserKongQueScore[wChairID] + m_lUserHouZiScore[wChairID]+
							m_lUserLaoYingScore[wChairID]+m_lUserShiZiScore[wChairID]);

						//清空数据
						m_lUserYanZiScore[wChairID] = 0;
						m_lUserTuZiScore[wChairID] = 0;
						m_lUserGeZiScore[wChairID] = 0;
						m_lUserXiongMaoScore[wChairID] = 0;
						m_lUserKongQueScore[wChairID] = 0;
						m_lUserHouZiScore[wChairID] = 0;
						m_lUserLaoYingScore[wChairID] = 0;
						m_lUserShiZiScore[wChairID] = 0;
						//写入积分
						//m_pITableFrame->WriteUserScore(pIServerUserItem, lScore,lRevenue, ScoreKind);
					}
				}
			}

			//庄家判断
			if (cbReason==GER_USER_LEFT && wChairID!=m_wCurrentBanker) return true;

			//产生索引
			RandGameIndex();

			//计算分数
			LONG lBankerWinScore=CalculateScore();

			//逃跑判断
			//if (cbReason==GER_USER_LEFT && wChairID==m_wCurrentBanker && m_lUserWinScore[m_wCurrentBanker]<0)
			//{
			//	//m_pITableFrame->WriteUserScore(m_wCurrentBanker,m_lUserWinScore[m_wCurrentBanker], m_lUserRevenue[m_wCurrentBanker], enScoreKind_Flee);
			//}

			//递增次数
			m_wBankerTime++;

			//结束消息
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			
			//庄家信息
			GameEnd.nBankerTime = m_wBankerTime;
			GameEnd.lBankerTotallScore=m_lBankerWinScore;
			if (cbReason==GER_USER_LEFT && lBankerWinScore>0) GameEnd.lBankerScore=0;
			else GameEnd.lBankerScore=lBankerWinScore;

			//发送积分
			GameEnd.cbTimeLeave=TIME_GAME_END;	
			for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserIndex);
				if ( pIServerUserItem == NULL ) continue;
				tagServerUserData *pUserData = pIServerUserItem->GetUserData();

				//设置成绩
				GameEnd.lUserScore=m_lUserWinScore[wUserIndex];

				//返还积分
				GameEnd.lUserReturnScore=m_lUserReturnScore[wUserIndex];
	
				//设置索引
				GameEnd.cbEndIndex = m_cbEndIndex;
				GameEnd.cbMoveEndIndex = m_cbMoveEndIndex;
				GameEnd.cbResultIndex = m_cbResultIndex;

				//设置税收
				if (m_lUserRevenue[wUserIndex]>0) GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
				else if (m_wCurrentBanker!=INVALID_CHAIR) GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
				else GameEnd.lRevenue=0;
			
				//玩家下注信息
				GameEnd.lUserBigSharkScore = m_lUserBigSharkScore[wUserIndex];
				GameEnd.lUserFeiQinScore = m_lUserFeiQinScore[wUserIndex];
				GameEnd.lUserZouShouScore = m_lUserZouShouScore[wUserIndex];
				GameEnd.lUserYanZiScore = m_lUserYanZiScore[wUserIndex];					
				GameEnd.lUserTuZiScore = m_lUserTuZiScore[wUserIndex];				
				GameEnd.lUserGeZiScore = m_lUserGeZiScore[wUserIndex];					
				GameEnd.lUserXiongMaoScore = m_lUserXiongMaoScore[wUserIndex];				
				GameEnd.lUserKongQueScore = m_lUserKongQueScore[wUserIndex];					
				GameEnd.lUserHouZiScore = m_lUserHouZiScore[wUserIndex];				
				GameEnd.lUserLaoYingScore = m_lUserLaoYingScore[wUserIndex];					
				GameEnd.lUserShiZiScore = m_lUserShiZiScore[wUserIndex];				
	
				//发送消息					
				m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			//切换庄家
			//if (cbReason==GER_USER_LEFT && wChairID==m_wCurrentBanker) ChangeBanker(true);

			return true;
		}
	}

	return false;
}

//发送场景
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStation, bool bSendSecret)
{
	switch (cbGameStation)
	{
	case GS_FREE:		//空闲状态
		{
			//发送记录
			SendGameRecord();

			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));			

			//控制信息
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusFree.lAreaLimitScore = m_lAreaLimitScore;

			//庄家信息
			StatusFree.bEnableSysBanker=m_bEnableSysBanker;
			StatusFree.wBankerUser=m_wCurrentBanker;	
			StatusFree.cbBankerTime=m_wBankerTime;
			StatusFree.lBankerWinScore=m_lBankerWinScore;
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
				StatusFree.lBankerScore=pIServerUserItem->GetUserScore()->lScore;
			}

			//玩家信息
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusFree.lUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_lUserLimitScore); 
			}

			//全局信息
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusFree.cbTimeLeave=(BYTE)(TIME_FREE-__min(dwPassTime,TIME_FREE));

			//发送场景
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

			//限制提示
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("\n上庄条件为：%s \n区域限制为：%s \n玩家限制为：%s \n连庄次数为：%d"),
				DrawScoreString(m_lApplyBankerCondition),DrawScoreString(m_lAreaLimitScore),DrawScoreString(m_lUserLimitScore),m_bMaxBankerTime);

			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);



			//发送申请者
			SendApplyUser(pIServerUserItem);
			return bSuccess;
		}
	case GS_PLACE_JETTON:		//游戏状态
	case GS_GAME_END:			//结束状态
		{
			//发送记录
			SendGameRecord();		

			//构造数据
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//全局下注
			StatusPlay.lALLBigSharkScore=m_lALLBigSharkScore;
			StatusPlay.lALLFeiQinScore=m_lALLFeiQinScore;
			StatusPlay.lALLZouShouScore=m_lALLZouShouScore;
			StatusPlay.lALLYanZiScore=m_lALLYanZiScore;		
			StatusPlay.lALLTuZiScore=m_lALLTuZiScore;		
			StatusPlay.lALLGeZiScore=m_lALLGeZiScore;		
			StatusPlay.lALLXiongMaoScore=m_lALLXiongMaoScore;
			StatusPlay.lALLKongQueScore=m_lALLKongQueScore;	
			StatusPlay.lALLHouZiScore=m_lALLHouZiScore;	
			StatusPlay.lALLLaoYingScore=m_lALLLaoYingScore;	
			StatusPlay.lALLShiZiScore=m_lALLShiZiScore;	

			//玩家下注
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusPlay.lUserBigSharkScore=m_lUserBigSharkScore[wChiarID];
				StatusPlay.lUserFeiQinScore=m_lUserFeiQinScore[wChiarID];
				StatusPlay.lUserZouShouScore=m_lUserZouShouScore[wChiarID];
				StatusPlay.lUserYanZiScore=m_lUserYanZiScore[wChiarID];			
				StatusPlay.lUserTuZiScore=m_lUserTuZiScore[wChiarID];		
				StatusPlay.lUserGeZiScore=m_lUserGeZiScore[wChiarID];		
				StatusPlay.lUserXiongMaoScore=m_lUserXiongMaoScore[wChiarID];	
				StatusPlay.lUserKongQueScore=m_lUserKongQueScore[wChiarID];	
				StatusPlay.lUserHouZiScore=m_lUserHouZiScore[wChiarID];	
				StatusPlay.lUserLaoYingScore=m_lUserLaoYingScore[wChiarID];		
				StatusPlay.lUserShiZiScore=m_lUserShiZiScore[wChiarID];

				//最大下注
				StatusPlay.lUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_lUserLimitScore);
			}
			//用户状态
			StatusPlay.cbUserStatus = pIServerUserItem->GetUserStatus();
			//控制信息
			StatusPlay.lApplyBankerCondition=m_lApplyBankerCondition;		
			StatusPlay.lAreaLimitScore=m_lAreaLimitScore;		

			//庄家信息
			StatusPlay.bEnableSysBanker=m_bEnableSysBanker;
			StatusPlay.wBankerUser=m_wCurrentBanker;			
			StatusPlay.cbBankerTime=m_wBankerTime;
			StatusPlay.lBankerWinScore=m_lBankerWinScore;	
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
				StatusPlay.lBankerScore=pIServerUserItem->GetUserScore()->lScore;
			}	

			//全局信息
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusPlay.cbTimeLeave=(BYTE)(TIME_FREE-__min(dwPassTime,TIME_FREE));
			StatusPlay.cbGameStatus=m_pITableFrame->GetGameStatus();			

			//结束判断
			if (cbGameStation==GS_GAME_END)
			{
				//设置成绩
				StatusPlay.lEndUserScore=m_lUserWinScore[wChiarID];

				//返还积分
				StatusPlay.lEndUserReturnScore=m_lUserReturnScore[wChiarID];

				//设置税收
				if (m_lUserRevenue[wChiarID]>0) StatusPlay.lEndRevenue=m_lUserRevenue[wChiarID];
				else if (m_wCurrentBanker!=INVALID_CHAIR) StatusPlay.lEndRevenue=m_lUserRevenue[m_wCurrentBanker];
				else StatusPlay.lEndRevenue=0;

				//庄家成绩
				StatusPlay.lEndBankerScore=m_lBankerCurGameScore;

				//索引信息
				StatusPlay.cbEndIndex = m_cbEndIndex;
				StatusPlay.cbMoveEndIndex = m_cbMoveEndIndex;
				StatusPlay.cbResultIndex = m_cbResultIndex;
			}
			//限制提示
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("\n上庄条件为：%s \n区域限制为：%s \n玩家限制为：%s \n连庄次数为：%d"),
				DrawScoreString(m_lApplyBankerCondition),DrawScoreString(m_lAreaLimitScore),DrawScoreString(m_lUserLimitScore),m_bMaxBankerTime);

			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);

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
	case IDI_FREE:		//空闲时间
		{
		//	RandGameIndex();
			//开始游戏
			m_pITableFrameControl->StartGame();
			//设置时间
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,TIME_PLACE_JETTON*1000,1,0L);
			if (m_bChangeBanker)
				SysChangeBanker(true);
			//设置状态
			m_pITableFrame->SetGameStatus(GS_PLACE_JETTON);

			return true;
		}
	case IDI_PLACE_JETTON:		//下注时间
		{
			//设置状态
			m_pITableFrame->SetGameStatus(GS_GAME_END);			

			//结束游戏
			OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);

			//设置时间
			m_pITableFrame->SetGameTimer(IDI_GAME_END,TIME_GAME_END*1000,1,0L);			

			return true;
		}
	case IDI_GAME_END:			//结束游戏
		{
			//写入积分
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserChairID);
				if ( pIServerUserItem == NULL ) continue;
				tagServerUserData *pUserDate = pIServerUserItem->GetUserData();
				if (pUserDate->cbUserStatus!=US_PLAY) continue;
				//胜利类型
				enScoreKind ScoreKind=(m_lUserWinScore[wUserChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;

				//坐庄判断
				LONG lUserScore=pIServerUserItem->GetUserScore()->lScore;

				//写入积分
				if (m_lUserWinScore[wUserChairID]!=0L&&(lUserScore+m_lUserWinScore[wUserChairID])<2000000000) 
					//m_pITableFrame->WriteUserScore(wUserChairID,m_lUserWinScore[wUserChairID], m_lUserRevenue[wUserChairID], ScoreKind);

				
				if (wUserChairID!=m_wCurrentBanker && pIServerUserItem->GetUserScore()->lScore<m_lApplyBankerCondition)
				{
					for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
					{
						if (wUserChairID!=m_ApplyUserArray[i]) continue;

						//删除玩家
						m_ApplyUserArray.RemoveAt(i);

						//发送消息
						CMD_S_CancelBanker CancelBanker;
						ZeroMemory(&CancelBanker,sizeof(CancelBanker));

						//设置变量
						CancelBanker.wCancelUser=wUserChairID;

						//发送消息
						m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
						m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

						//提示消息
						TCHAR szTipMsg[128];
						_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("由于你的金币数少于坐庄必须金币数（%ld）,你自动下庄！"),m_lApplyBankerCondition);
						SendGameMessage(wUserChairID,szTipMsg);

						break;
					}
				}
			}

			//结束游戏
			m_pITableFrame->ConcludeGame();

			//切换庄家
			ChangeBanker(false);
			
			SysChangeBanker(true);
			//发送历史
			SendGameRecord();

			//设置时间
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,TIME_FREE*1000,1,0L);

			//发送消息
			CMD_S_GameFree GameFree;
			ZeroMemory(&GameFree,sizeof(GameFree));
			GameFree.cbTimeLeave=TIME_FREE;
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
			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus==US_LOOKON) return true;

			return OnUserApplyBanker(pIServerUserItem);	
		}
	case SUB_C_CANCEL_BANKER:		//取消做庄
		{
			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus==US_LOOKON) return true;

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

//用户断线
bool __cdecl CTableFrameSink::OnEventUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	//切换庄家
	if (wChairID==m_wCurrentBanker) ChangeBanker(true);

	//取消申请
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		if (wChairID!=m_ApplyUserArray[i]) continue;

		//删除玩家
		m_ApplyUserArray.RemoveAt(i);

		//构造变量
		CMD_S_CancelBanker CancelBanker;
		ZeroMemory(&CancelBanker,sizeof(CancelBanker));

		//设置变量
		CancelBanker.wCancelUser=wChairID;

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

	return true;
}
//用户坐下
bool __cdecl CTableFrameSink::OnEventUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
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
	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄条件为：%ld,区域限制为：%ld,玩家限制为：%ld"),m_lApplyBankerCondition,
		m_lAreaLimitScore,m_lUserLimitScore);
	m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);

	return true;
}

//用户起来
bool __cdecl CTableFrameSink::OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//记录成绩
	if (bLookonUser==false)
	{
		//切换庄家
		if (wChairID==m_wCurrentBanker) ChangeBanker(true);

		//取消申请
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			if (wChairID!=m_ApplyUserArray[i]) continue;

			//删除玩家
			m_ApplyUserArray.RemoveAt(i);

			//构造变量
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//设置变量
			CancelBanker.wCancelUser=wChairID;

			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

			break;
		}

		return true;
	}

	return true;
}

//更换庄家
bool CTableFrameSink::ChangeBanker(bool bCancelCurrentBanker)
{
	//切换标识
	bool bChangeBanker=false;

	//做庄次数
	WORD wBankerTime=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Time"), 5, m_szConfigFileName);

	//取消当前
	if (bCancelCurrentBanker)
	{
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
		if (m_pITableFrame->GetGameStatus()==GS_FREE && m_ApplyUserArray.GetCount()!=0)
		{
			m_wCurrentBanker=m_ApplyUserArray[0];
		}

		//设置变量
		bChangeBanker=true;
	}
	//轮庄判断
	else if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		if (m_wCurrentBanker == INVALID_CHAIR)
		{
			m_bChangeBanker = false;//m_bChangeBanker?false:true;
		}
		else
		{
			m_bChangeBanker = true;
		}
		//获取庄家
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//次数判断
		if (wBankerTime<=m_wBankerTime || lBankerScore<m_lApplyBankerCondition)
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
			if (lBankerScore<m_lApplyBankerCondition)
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]分数少于(%ld)，强行换庄!"),pIServerUserItem->GetAccounts(),m_lApplyBankerCondition);
			else
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]做庄次数达到(%d)，强行换庄!"),pIServerUserItem->GetAccounts(),wBankerTime);

			//发送消息
			SendGameMessage(INVALID_CHAIR,szTipMsg);	
		}
	}
	//系统做庄
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
		m_lBankerWinScore=0;

		//发送消息
		CMD_S_ChangeBanker ChangeBanker;
		ZeroMemory(&ChangeBanker,sizeof(ChangeBanker));
		ChangeBanker.wBankerUser=m_wCurrentBanker;
		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
			ChangeBanker.lBankerScore=pIServerUserItem->GetUserScore()->lScore;
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

//加注事件
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONG lJettonScore)
{
	//效验参数
	ASSERT((cbJettonArea<=ID_SHI_ZI)&&(lJettonScore>0L));
	if ((cbJettonArea>ID_SHI_ZI)||(lJettonScore<=0L)) return false;

	//效验状态
	//ASSERT(m_pITableFrame->GetGameStatus()==GS_PLACE_JETTON);
	if (m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON) return true;

	//庄家判断
	if (m_wCurrentBanker==wChairID) return true;
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) return true;

	EnterCriticalSection(&m_csPlaceJetton);

	//变量定义
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	LONG lJettonCount=m_lUserBigSharkScore[wChairID]+m_lUserFeiQinScore[wChairID]+m_lUserZouShouScore[wChairID]+
		m_lUserYanZiScore[wChairID]+m_lUserTuZiScore[wChairID]+m_lUserGeZiScore[wChairID]+
		m_lUserXiongMaoScore[wChairID] + m_lUserKongQueScore[wChairID] + m_lUserHouZiScore[wChairID]+
		m_lUserLaoYingScore[wChairID]+m_lUserShiZiScore[wChairID];

	LONG lUserScore = pIServerUserItem->GetUserScore()->lScore;

	//合法校验
	if (lUserScore < lJettonCount + lJettonScore) return true;
	if (m_lUserLimitScore < lJettonCount + lJettonScore) return true;

	//成功标识
	bool bPlaceJettonSuccess=true;

	//合法验证
	if ( ID_BIG_SHARK == cbJettonArea )
	{
		if (GetMaxScore(wChairID,lJettonScore,cbJettonArea) >= 0 )
		{
			//保存下注
			m_lALLBigSharkScore += lJettonScore;
			m_lUserBigSharkScore[wChairID] += lJettonScore;			
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_FEI_QIN == cbJettonArea )
	{
		if (GetMaxScore(wChairID,lJettonScore,cbJettonArea) >= 0 )
		{
			//保存下注
			m_lALLFeiQinScore += lJettonScore;
			m_lUserFeiQinScore[wChairID] += lJettonScore;			
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_ZOU_SHOU == cbJettonArea )
	{
		if (GetMaxScore(wChairID,lJettonScore,cbJettonArea) >= 0 )
		{
			//保存下注
			m_lALLZouShouScore += lJettonScore;
			m_lUserZouShouScore[wChairID] += lJettonScore;			
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_YAN_ZI == cbJettonArea )
	{
		if (GetMaxScore(wChairID,lJettonScore,cbJettonArea) >= 0 )
		{
			//保存下注
			m_lALLYanZiScore += lJettonScore;
			m_lUserYanZiScore[wChairID] += lJettonScore;			
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_TU_ZI == cbJettonArea )
	{
		if (GetMaxScore(wChairID,lJettonScore,cbJettonArea) >= 0 )
		{
			//保存下注
			m_lALLTuZiScore += lJettonScore;
			m_lUserTuZiScore[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_GE_ZI == cbJettonArea )
	{
		if (GetMaxScore(wChairID,lJettonScore,cbJettonArea) >= 0 )
		{
			//保存下注
			m_lALLGeZiScore += lJettonScore;
			m_lUserGeZiScore[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_XIONG_MAO == cbJettonArea )
	{
		if (GetMaxScore(wChairID,lJettonScore,cbJettonArea) >= 0 )
		{
			//保存下注
			m_lALLXiongMaoScore += lJettonScore;
			m_lUserXiongMaoScore[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_KONG_QUE == cbJettonArea )
	{
		if (GetMaxScore(wChairID,lJettonScore,cbJettonArea) >= 0 )
		{
			//保存下注
			m_lALLKongQueScore += lJettonScore;
			m_lUserKongQueScore[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_HOU_ZI == cbJettonArea )
	{
		if (GetMaxScore(wChairID,lJettonScore,cbJettonArea) >= 0 )
		{
			//保存下注
			m_lALLHouZiScore += lJettonScore;
			m_lUserHouZiScore[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if (ID_LAO_YING==cbJettonArea)
	{
		if (GetMaxScore(wChairID,lJettonScore,cbJettonArea) >= 0 )
		{
			//保存下注
			m_lALLLaoYingScore+=lJettonScore;
			m_lUserLaoYingScore[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if (ID_SHI_ZI==cbJettonArea)
	{
		if (GetMaxScore(wChairID,lJettonScore,cbJettonArea) >= 0 )
		{
			//保存下注
			m_lALLShiZiScore+=lJettonScore;
			m_lUserShiZiScore[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else
	{
		bPlaceJettonSuccess=false;
	}
	
	LeaveCriticalSection(&m_csPlaceJetton);

	if (bPlaceJettonSuccess)
	{
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
	}
	else
	{
		CMD_S_PlaceJettonFail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
		PlaceJettonFail.lJettonArea=cbJettonArea;
		PlaceJettonFail.lPlaceScore=lJettonScore;

		//发送消息
		m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
	}

	return true;
}
//产生索引
void CTableFrameSink::RandGameIndex()
{
	srand((unsigned)time(NULL));
	//BYTE cbGaiLv[5];
	//ZeroMemory(cbGaiLv, sizeof(cbGaiLv));;
	//m_cbResultIndex = 0;
	//BYTE cbQuYu = 0;  //区域
	//BYTE cbSaveCount = 0; //保护
	//LONG lAuto=0;
	////读取配置
	//m_cbWinSideControl = GetPrivateProfileInt( TEXT("ManagerControl"), TEXT("WinSide"), 0, m_szConfigFileName );
	//cbGaiLv[0] = GetPrivateProfileInt( TEXT("GaiLv"), TEXT("YAN_ZI"), 0, m_szConfigFileName );
	//cbGaiLv[1] = GetPrivateProfileInt( TEXT("GaiLv"), TEXT("GE_ZI"), 0, m_szConfigFileName );
	//cbGaiLv[2] = GetPrivateProfileInt( TEXT("GaiLv"), TEXT("KONG_QUE"), 0, m_szConfigFileName );
	//cbGaiLv[3] = GetPrivateProfileInt( TEXT("GaiLv"), TEXT("LAO_YING"), 0, m_szConfigFileName );
	//cbGaiLv[4] = GetPrivateProfileInt( TEXT("GaiLv"), TEXT("SMALL"), 0, m_szConfigFileName );
	//
	//lAuto = GetPrivateProfileInt( TEXT("Auto"), TEXT("IsAuto"), 0, m_szConfigFileName );
	//cbSaveCount = GetPrivateProfileInt( TEXT("Safe"), TEXT("IsSafe"), 0, m_szConfigFileName ); 

	//if (m_cbWinSideControl!=0 && (cbSaveCount==0||(m_cbZuoBiIndex!=0&&m_cbZuoBiIndex!=m_cbWinSideControl)||m_cbZuoBiCount<cbSaveCount))
	//{
	//	if (m_cbZuoBiIndex==m_cbWinSideControl)
	//		m_cbZuoBiCount++;
	//	else
	//	{
	//		m_cbZuoBiCount = 0;
	//		m_cbZuoBiIndex = m_cbWinSideControl;
	//		m_cbZuoBiCount++;
	//	}
	//	m_cbResultIndex = m_cbWinSideControl;
	//}
	//else
	//{
	//	m_cbZuoBiIndex=0;
	//	LONG cbSunScore = 0;
	//	cbSunScore = m_lALLBigSharkScore+m_lALLFeiQinScore+m_lALLZouShouScore+m_lALLYanZiScore
	//					+m_lALLTuZiScore+m_lALLGeZiScore+m_lALLXiongMaoScore
	//					+m_lALLKongQueScore+m_lALLHouZiScore+m_lALLLaoYingScore+m_lALLShiZiScore;
	//	//区域倍率
	//	BYTE cbMultiple[12] = {0, 24, 2, 2, 6, 6, 8, 8, 8, 8, 12, 12};
	//	//玩家下注
	//	LONG pUserAllScore[12] = {NULL, m_lALLBigSharkScore,m_lALLFeiQinScore,m_lALLZouShouScore,m_lALLYanZiScore, m_lALLTuZiScore, m_lALLGeZiScore, m_lALLXiongMaoScore, 
	//						  m_lALLKongQueScore, m_lALLHouZiScore,m_lALLLaoYingScore,m_lALLShiZiScore};
	//	//结果索引
	//	CArrayTemplate<BYTE> cbRelset;
	//	cbRelset.RemoveAll();
	//	//最小记录
	//	LONG lMinScore = 0L;
	//	if (lAuto!=0&&cbSunScore>=lAuto)//自动跑向
	//	{
	//		//寻找没有下注的区域
	//		for (BYTE i=4; i<12; i++)
	//		{
	//			if (pUserAllScore[i]==0)
	//				cbRelset.Add(i);
	//		}
	//		if (cbRelset.GetCount()==0)
	//		{
	//			//寻找最小区域
	//			for (BYTE i=4; i<12; i++)
	//			{
	//				if (pUserAllScore[i]==0)
	//					continue;
	//				if (lMinScore==0)
	//				{
	//					lMinScore = pUserAllScore[i]*cbMultiple[i];
	//					cbRelset.Add(i);
	//				}
	//				else if (lMinScore==pUserAllScore[i]*cbMultiple[i])
	//				{
	//					cbRelset.Add(i);
	//				}
	//				else if (lMinScore>pUserAllScore[i]*cbMultiple[i])
	//				{
	//					cbRelset.RemoveAll();
	//					cbRelset.Add(i);
	//					lMinScore = pUserAllScore[i]*cbMultiple[i];
	//				}
	//			}
	//			BYTE cbRandIndex = 0;
	//			cbRandIndex = rand()%cbRelset.GetCount();
	//			m_cbResultIndex = cbRelset[cbRandIndex];
	//		}
	//		else
	//		{
	//			BYTE cbRandIndex = 0;
	//			cbRandIndex = rand()%cbRelset.GetCount();
	//			m_cbResultIndex = cbRelset[cbRandIndex];
	//		}
	//	}
	//	else
	//	{
	//		BYTE cbRandIndex[100]; //随机序列
	//		ZeroMemory(cbRandIndex, sizeof(cbRandIndex));
	//		BYTE cbRand = 0;
	//		for (BYTE i=0; i<4; i++)
	//		{
	//			BYTE j = 0;
	//			while (j<cbGaiLv[i])
	//			{
	//				//srand((unsigned)time(NULL)*10);
	//				cbRand = rand()%50+m_RandGen.NormalRandom(0,50,0,51);
	//				if (cbRandIndex[cbRand]!=0)
	//					continue;
	//				cbRandIndex[cbRand] = i*2+1;
	//				j++;
	//			}
	//		}
	//		BYTE cbResult = m_RandGen.NormalRandom(0,100,0,100);//rand()%50+
	//		if (cbRandIndex[cbResult]!=0)
	//		{
	//			m_cbResultIndex = cbRandIndex[cbResult];
	//		}
	//		else
	//		{	
	//			BYTE cbTemp = rand()%4*2+2;
 //				m_cbResultIndex = cbTemp;
	//		}

	//	}
	//}
	m_cbLastResultIndex = m_cbResultIndex;
	m_cbMoveEndIndex = 28*3+rand()%28;

	BYTE cbAreaIndex=0;
	for (int i=1;i<12;i++)
	{
		for (int j=0;j<4;j++)
		{
			if (g_cbTurnAreaIndex[i][j]==m_cbMoveEndIndex%28 && g_cbTurnAreaIndex[i][j]!=0)
			{
				cbAreaIndex=i;
			}
		}
	}
	//if (m_cbResultIndex>(m_cbMoveEndIndex%8+1))
	//{
	//	m_cbEndIndex = m_cbResultIndex-(m_cbMoveEndIndex%8+1);
	//}
	//else if (m_cbResultIndex<(m_cbMoveEndIndex%8+1))
	//{
	//	m_cbEndIndex = 7-((m_cbMoveEndIndex%8)-m_cbResultIndex);
	//}
	//else
	{
		m_cbEndIndex = 0;
	}
	//记录信息
	if (m_cbScoreHistory.GetCount()<MAX_SCORE_HISTORY)
	{
		m_cbScoreHistory.Add(cbAreaIndex-3);
	}
	else
	{
		/*m_cbScoreHistory.RemoveAt(0);*/
		for (BYTE i=0; i<MAX_SCORE_HISTORY-1; i++)
		{
			m_cbScoreHistory[i] = m_cbScoreHistory[i+1];
		}
		m_cbScoreHistory[MAX_SCORE_HISTORY-1] = m_cbResultIndex;//MAX_SCORE_HISTORY
	}
}

//计算得分
LONG CTableFrameSink::CalculateScore()
{
	//变量定义
	LONG cbRevenue=m_pGameServiceOption->cbRevenue;
	//庄家总量
	LONG lBankerWinScore = 0;
	LONG lUserLostScore[GAME_PLAYER];
	//玩家成绩
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	BYTE cbAreaIndex=0;
	for (int i=1;i<12;i++)
	{
		for (int j=0;j<4;j++)
		{
			if (g_cbTurnAreaIndex[i][j]==(m_cbMoveEndIndex%28) && g_cbTurnAreaIndex[i][j]!=0)
			{
				cbAreaIndex=i;
			}
		}
	}
	//TCHAR szTemp[64];
	//sprintf(szTemp,"%d %d",cbAreaIndex,m_cbMoveEndIndex);
	//AfxMessageBox(szTemp);
	//区域倍率
	BYTE cbMultiple[] = {0, 24, 2, 2, 6, 6, 8, 8, 8, 8, 12, 12};
	//玩家下注
	LONG *pUserScore[] = {NULL, m_lUserBigSharkScore,m_lUserFeiQinScore,m_lUserZouShouScore,m_lUserYanZiScore, m_lUserTuZiScore, m_lUserGeZiScore, m_lUserXiongMaoScore, 
		m_lUserKongQueScore, m_lUserHouZiScore,m_lUserLaoYingScore,m_lUserShiZiScore};
	//玩家成绩
	for (WORD i=0; i<GAME_PLAYER; i++)
	{
		//庄家判断
		if (m_wCurrentBanker==i) continue;

		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem==NULL) continue;
		for (WORD wAreaIndex=ID_BIG_SHARK; wAreaIndex<=ID_SHI_ZI; wAreaIndex++)
		{
			if (cbAreaIndex==0)
			{
				m_lUserWinScore[i] -= pUserScore[wAreaIndex][i];
			}
			else if (cbAreaIndex==1)
			{
				m_lUserWinScore[i] += ( pUserScore[wAreaIndex][i] * cbMultiple[wAreaIndex] ) ;
				m_lUserReturnScore[i] += pUserScore[wAreaIndex][i] ;
			}
			else if (wAreaIndex==cbAreaIndex-3)
			{
				m_lUserWinScore[i] += ( pUserScore[wAreaIndex][i] * cbMultiple[wAreaIndex] ) ;
				m_lUserReturnScore[i] += pUserScore[wAreaIndex][i] ;
				//lBankerWinScore -= ( pUserScore[wAreaIndex][i] * cbMultiple[wAreaIndex] ) ;
			}
			else
			{
				m_lUserWinScore[i] -= pUserScore[cbAreaIndex][i];
			}

			if (cbAreaIndex%2==0 && cbAreaIndex>3)
			{
				m_lUserWinScore[i] += ( pUserScore[2][i] * 2 ) ;
				m_lUserReturnScore[i] += pUserScore[2][i] ;
			}
			else
			{
				m_lUserWinScore[i] -= pUserScore[2][i];
			}
			if (cbAreaIndex%2==1 && cbAreaIndex>3)
			{
				m_lUserWinScore[i] += ( pUserScore[3][i] * 2 ) ;
				m_lUserReturnScore[i] += pUserScore[3][i] ;
			}
			else
			{
				m_lUserWinScore[i] -= pUserScore[3][i];
			}

 			//lBankerWinScore += pUserScore[wAreaIndex][i];
		}
		m_lUserWinScore[i] -= m_lUserReturnScore[i];
		//计算税收
		if (0 < m_lUserWinScore[i])
		{
			m_lUserRevenue[i]  = m_lUserWinScore[i]*cbRevenue/100;
			m_lUserWinScore[i] -= m_lUserRevenue[i];
		}

	}
	//庄家成绩
	//if (m_wCurrentBanker!=INVALID_CHAIR)
	//{
	//	m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;

	//	//计算税收
	//	if (0 < m_lUserWinScore[m_wCurrentBanker])
	//	{
	//		
	//		m_lUserRevenue[m_wCurrentBanker]  =m_lUserWinScore[m_wCurrentBanker]*cbRevenue/100;
	//		m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
	//		lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
	//	}		
	//}

	//累计积分
	m_lBankerWinScore += lBankerWinScore;

	//当前积分
	m_lBankerCurGameScore=lBankerWinScore;

	return lBankerWinScore;
}

//申请庄家
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	//合法判断
	LONG lUserScore=pIApplyServerUserItem->GetUserScore()->lScore;
	if (lUserScore<m_lApplyBankerCondition)
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
		if (pIServerUserItem==NULL) continue;
		if (pIServerUserItem->GetUserID()!=pICancelServerUserItem->GetUserID()) continue;

		//删除玩家
		m_ApplyUserArray.RemoveAt(i);

		if (m_wCurrentBanker!=wChairID)
		{
			//构造变量
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//设置变量
			CancelBanker.wCancelUser=wChairID;

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

//发送庄家
void CTableFrameSink::SendApplyUser( IServerUserItem *pRcvServerUserItem )
{
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];

		//获取玩家
		IServerUserItem *pServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (!pServerUserItem) continue;

		//庄家判断
		if (pServerUserItem->GetChairID()==m_wCurrentBanker) continue;

		//构造变量
		CMD_S_ApplyBanker ApplyBanker;
		ApplyBanker.wApplyUser=wChairID;

		//发送消息
		m_pITableFrame->SendUserData(pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	}
}

//发送记录
void CTableFrameSink::SendGameRecord()
{
	BYTE cbSocreHistroy[MAX_SCORE_HISTORY];
	ZeroMemory(cbSocreHistroy, sizeof(cbSocreHistroy));
	for (BYTE i=0; i<m_cbScoreHistory.GetCount(); i++)
	{
		cbSocreHistroy[i] = m_cbScoreHistory[i];
	}

	CMD_S_ScoreHistory ScoreHistory;
	ZeroMemory(&ScoreHistory, sizeof(ScoreHistory));

	CopyMemory(ScoreHistory.cbScoreHistroy,cbSocreHistroy,sizeof(ScoreHistory.cbScoreHistroy));

	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_SCORE_HISTORY, &cbSocreHistroy, sizeof(cbSocreHistroy));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_SCORE_HISTORY, &cbSocreHistroy, sizeof(cbSocreHistroy));
}

//最大下注
LONG CTableFrameSink::GetMaxScore(WORD wChairID, LONG lJettonScore,BYTE cbArea)
{
	//玩家限制
	LONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	LONG lAreaScore[]={0,m_lALLBigSharkScore,m_lALLFeiQinScore,m_lALLZouShouScore,
		m_lALLYanZiScore,m_lALLTuZiScore,m_lALLGeZiScore,m_lALLXiongMaoScore,
		m_lALLKongQueScore,m_lALLHouZiScore,m_lALLLaoYingScore,m_lALLShiZiScore};

	//区域限制
	LONG lAreaLimit = m_lAreaLimitScore - lAreaScore[cbArea];

	LONG lMaxJetton=0L;
	lMaxJetton=min(lUserMaxJetton,lAreaLimit);

	return lMaxJetton;
}

//最大下注
LONG CTableFrameSink::GetUserMaxJetton(WORD wChairID)
{
	//已下注额
	LONG lNowJetton = 0;
	lNowJetton += m_lUserBigSharkScore[wChairID];
	lNowJetton += m_lUserFeiQinScore[wChairID];
	lNowJetton += m_lUserZouShouScore[wChairID];
	lNowJetton += m_lUserYanZiScore[wChairID];
	lNowJetton += m_lUserTuZiScore[wChairID];
	lNowJetton += m_lUserGeZiScore[wChairID];
	lNowJetton += m_lUserXiongMaoScore[wChairID];
	lNowJetton += m_lUserKongQueScore[wChairID];
	lNowJetton += m_lUserHouZiScore[wChairID];
	lNowJetton += m_lUserLaoYingScore[wChairID];
	lNowJetton += m_lUserShiZiScore[wChairID];

	//获取玩家
	IServerUserItem *pIMeServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);

	//玩家下注
	LONG lMeMaxScore = min(pIMeServerUserItem->GetUserScore()->lScore, m_lUserLimitScore);
	lMeMaxScore -= lNowJetton;
	ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return lMeMaxScore;
}

void CTableFrameSink::SysChangeBanker(bool bCancelCurrentBanker)
{
	//做庄次数
	WORD wBankerTime=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Time"), 5, m_szConfigFileName);
	
	if ((m_wBankerTime<wBankerTime&&m_wBankerTime!=0) || !m_bEnableSysBanker || m_wCurrentBanker!=INVALID_CHAIR)
		return;
	
	
	//系统上庄
	if (bCancelCurrentBanker)
	{
		//设置变量
		//m_bEnableSysBanker = true;
		m_wBankerTime = 0;
		m_bChangeBanker = false;
		//发送消息
		CMD_S_ChangeSysBanker ChangeSysBanker;
		ZeroMemory(&ChangeSysBanker,sizeof(ChangeSysBanker));
		ChangeSysBanker.bEnableSysBanker = true;
		ChangeSysBanker.cbBankerTime = m_wBankerTime;

		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_SYS_BANKER,&ChangeSysBanker,sizeof(ChangeSysBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_SYS_BANKER,&ChangeSysBanker,sizeof(ChangeSysBanker));

		//提示消息
		TCHAR szTipMsg[128];
		_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[稻草人]上庄"));
		//发送消息
		SendGameMessage(INVALID_CHAIR,szTipMsg);
	}
	else
	{
		//设置变量
		//m_bEnableSysBanker = false;
		m_wBankerTime = 0;
		m_bChangeBanker = true;
		//发送消息
		CMD_S_ChangeSysBanker ChangeSysBanker;
		ZeroMemory(&ChangeSysBanker,sizeof(ChangeSysBanker));
		ChangeSysBanker.bEnableSysBanker = false;
		ChangeSysBanker.cbBankerTime = m_wBankerTime;

		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_SYS_BANKER,&ChangeSysBanker,sizeof(ChangeSysBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_SYS_BANKER,&ChangeSysBanker,sizeof(ChangeSysBanker));

		//提示消息
		TCHAR szTipMsg[128];
		_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ 稻草人]做庄次数达到(%d)，强行换庄!"),wBankerTime);
		//发送消息
		SendGameMessage(INVALID_CHAIR,szTipMsg);
	}
}

//输出信息
CString CTableFrameSink::DrawScoreString(LONG lNumber)
{
	CString strNumber=TEXT("");
	strNumber.Format(TEXT("%ld"),lNumber>0?lNumber:-lNumber);
	int p=strNumber.GetLength()-4;
	while(p>0)
	{
		strNumber.Insert(p,",");
		p-=4;
	}
	if(lNumber<0)
	{
		strNumber=TEXT("-")+strNumber;
	}
	return strNumber;
}
//////////////////////////////////////////////////////////////////////////
