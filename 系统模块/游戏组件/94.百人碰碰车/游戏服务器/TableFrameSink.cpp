#include "StdAfx.h"
#include "TableFrameSink.h"
//////////////////////////////////////////////////////////////////////////

//常量定义
#define SEND_COUNT					300									//发送次数

//索引定义
#define INDEX_PLAYER				0									//闲家索引
#define INDEX_BANKER				1									//庄家索引

//下注时间
#define IDI_FREE					1									//空闲时间
#define TIME_FREE					10									//空闲时间

//下注时间
#define IDI_PLACE_JETTON			2									//下注时间
#define TIME_PLACE_JETTON			20									//下注时间

//结束时间
#define IDI_GAME_END				30									//结束时间
#define TIME_GAME_END				20									//结束时间
//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//游戏人数
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_TimeControl;	//开始模式

//////////////////////////////////////////////////////////////////////////
int  Findstr(char *buf,char findchar,int len)
{
	int i = 0;
	while (i<len)
	{
		if(buf[i++]==findchar)
			return (i-1);
	}
	return -1;
}
char* strGetbuffer(char *buf,int ipos,char *OutBuf,int outLen)
{
	if(ipos<outLen)
		memcpy(OutBuf,buf,ipos);
	else
	{
		memcpy(OutBuf,buf,outLen);
	}
	return  OutBuf;

}
void MakeStirngToStirng(char *buf,int len,char *outbuf,int &Outlen)
{

	size_t iPos = 0;
	Outlen =0;
	while (1)
	{
		iPos = Findstr(buf,',',(INT)strlen(buf));
		if(iPos==-1)
		{
			char tmp[256]={0};
			char *strtmp = buf;
			outbuf[Outlen++] = atoi(strtmp);
			break;
		}
		char tmp[256]={0};
		char *strtmp = strGetbuffer(buf,(INT)iPos,tmp,256);
		outbuf[Outlen++] = atoi(strtmp);
		buf = buf+iPos+1;
	}


}
//构造函数
CTableFrameSink::CTableFrameSink()
{
	//总下注数
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//个人下注
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//控制变量
	m_cbWinSideControl=0;
	m_nSendCardCount=0;

	m_CheckImage = 0;

	m_cbControlArea = 0xff;
	m_cbControlTimes = 0;

	//玩家成绩	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//扑克信息
	ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

	//状态变量
	m_dwJettonTime=0L;

	//庄家信息
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker=INVALID_CHAIR;
	m_wBankerTime=0;
	m_lBankerWinScore=0L;		
	m_lBankerCurGameScore=0L;
	m_bEnableSysBanker=true;
	m_cbLeftCardCount=0;
	m_bContiueCard=false;

	//记录变量
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;
	m_nRecordLast=0;
	m_dwRecordCount=0;

	//控制变量
	m_StorageStart = 0l;
	m_StorageDeduct = 0l;

	//机器人控制					
	m_lRobotAreaLimit = 0l;
	m_lRobotBetCount = 0l;										

	//庄家设置
	m_lBankerMAX = 0l;
	m_lBankerAdd = 0l;							
	m_lBankerScoreMAX = 0l;
	m_lBankerScoreAdd = 0l;
	m_lPlayerBankerMAX = 0l;
	m_bExchangeBanker = true;

	//时间控制
	m_cbFreeTime = TIME_FREE;
	m_cbBetTime = TIME_PLACE_JETTON;
	m_cbEndTime = TIME_GAME_END;

	//机器人控制
	m_nChipRobotCount = 0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

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
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//设置文件名
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_snprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\BumperCarBattle.ini"),szPath);
	memcpy(m_szGameRoomName, m_pGameServiceOption->szGameRoomName, sizeof(m_szGameRoomName));

	ReadConfigInformation(true);

	return true;
}

//复位桌子
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//总下注数
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//个人下注
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//玩家成绩	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//机器人控制
	m_nChipRobotCount = 0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));


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
	{
		pIBankerServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		m_lBankerScore=pIBankerServerUserItem->GetUserScore()->lScore;
	}

	//设置变量
	GameStart.cbTimeLeave=m_cbBetTime;
	GameStart.wBankerUser=m_wCurrentBanker;
	GameStart.lBankerScore = 0x7fffffffffffffff;
	if (pIBankerServerUserItem!=NULL) 
		GameStart.lBankerScore=pIBankerServerUserItem->GetUserScore()->lScore;

	GameStart.bContiueCard=m_bContiueCard;
	
	//下注机器人数量
	int nChipRobotCount = 0;
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
			nChipRobotCount++;
	}

	GameStart.nChipRobotCount = min(nChipRobotCount, m_nMaxChipRobot);

	//机器人控制
	m_nChipRobotCount = 0;

	//旁观玩家
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	

	//游戏玩家
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;

		if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR)
		{
			GameStart.lBankerScore = 1;
		}

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
		{
			//控制
			if ( m_cbControlArea != 0xff && m_cbControlTimes > 0)
			{
				BYTE cbControlArea[8][4] = { 1, 9, 17, 25, 
					3, 11, 19, 27,
					5, 13, 21, 30,
					7, 15, 23, 32,

					2, 10, 18, 26,
					4, 12, 20, 28,
					6, 14, 22, 31,
					8, 16, 24, 0 };
				m_cbTableCardArray[0][0] = cbControlArea[m_cbControlArea][rand()%4];
				m_cbControlTimes--;
				if(m_cbControlTimes == 0)
				{
					m_cbControlArea = 0xff;
				}
			}
			else
			{
				while(true)
				{
					//派发扑克
					DispatchTableCard();

					//试探性判断
					if (ProbeJudge())
					{
						break;
					}
				}
			}

			//计算分数
			LONGLONG lBankerWinScore=CalculateScore();

			//递增次数
			m_wBankerTime++;

			//结束消息
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//庄家信息
			GameEnd.nBankerTime = m_wBankerTime;
			GameEnd.lBankerTotallScore=m_lBankerWinScore;
			GameEnd.lBankerScore=lBankerWinScore;
			GameEnd.bcFirstCard = m_bcFirstPostCard;

			//扑克信息
			CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
			GameEnd.cbLeftCardCount=m_cbLeftCardCount;

			//发送积分
			GameEnd.cbTimeLeave=m_cbEndTime;	
			for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserIndex);
				if ( pIServerUserItem == NULL ) continue;

				//设置成绩
				GameEnd.lUserScore=m_lUserWinScore[wUserIndex];

				//返还积分
				GameEnd.lUserReturnScore=m_lUserReturnScore[wUserIndex];

				//设置税收
				if (m_lUserRevenue[wUserIndex]>0) GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
				else if (m_wCurrentBanker!=INVALID_CHAIR) GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
				else GameEnd.lRevenue=0;

				//发送消息					
				m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			return true;
		}
	case GER_USER_LEFT:		//用户离开
		{
			//闲家判断
			if (m_wCurrentBanker!=wChairID)
			{
				//变量定义
				LONGLONG lScore=0;
				LONGLONG lRevenue=0;
				enScoreKind ScoreKind=enScoreKind_Flee;

				//统计成绩
				for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lScore -= m_lUserJettonScore[nAreaIndex][wChairID];

				//写入积分
				if (m_pITableFrame->GetGameStatus() != GS_GAME_END)
				{
					for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
					{
						if (m_lUserJettonScore[nAreaIndex][wChairID] != 0)
						{
							CMD_S_PlaceJettonFail PlaceJettonFail;
							ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
							PlaceJettonFail.lJettonArea=nAreaIndex;
							PlaceJettonFail.lPlaceScore=m_lUserJettonScore[nAreaIndex][wChairID];
							PlaceJettonFail.wPlaceUser=wChairID;

							//游戏玩家
							for (WORD i=0; i<GAME_PLAYER; ++i)
							{
								IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
								if (pIServerUserItem==NULL) 
									continue;

								m_pITableFrame->SendTableData(i,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
							}

							m_lAllJettonScore[nAreaIndex] -= m_lUserJettonScore[nAreaIndex][wChairID];
							m_lUserJettonScore[nAreaIndex][wChairID] = 0;
						}
					}
				}
				else
				{

					for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
					{
						if (m_lUserJettonScore[nAreaIndex][wChairID] != 0)
						{
							m_lUserJettonScore[nAreaIndex][wChairID] = 0;
						}
					}

					//胜利类型
					enScoreKind ScoreKind=(m_lUserWinScore[wChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;

					//写入积分
					if (m_lUserWinScore[wChairID]!=0L) 
					{
						m_pITableFrame->WriteUserScore(pIServerUserItem,m_lUserWinScore[wChairID], static_cast<long>(m_lUserRevenue[wChairID]), ScoreKind);
						m_lUserWinScore[wChairID] = 0;
					}
				}
				return true;
			}

			//状态判断
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
				m_dwJettonTime=(DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);

				//控制
				if ( m_cbControlArea != 0xff && m_cbControlTimes > 0)
				{
					BYTE cbControlArea[8][4] = { 1, 9, 17, 25, 
						3, 11, 19, 27,
						5, 13, 21, 30,
						7, 15, 23, 32,

						2, 10, 18, 26,
						4, 12, 20, 28,
						6, 14, 22, 31,
						8, 16, 24, 0 };
					m_cbTableCardArray[0][0] = cbControlArea[m_cbControlArea][rand()%4];
					m_cbControlTimes--;
					if(m_cbControlTimes == 0)
					{
						m_cbControlArea = 0xff;
					}
				}
				else
				{
					while(true)
					{
						//派发扑克
						DispatchTableCard();

						//试探性判断
						if (ProbeJudge())
						{
							break;
						}
					}
				}

				//计算分数
				CalculateScore();

				//结束消息
				CMD_S_GameEnd GameEnd;
				ZeroMemory(&GameEnd,sizeof(GameEnd));

				//庄家信息
				GameEnd.nBankerTime = m_wBankerTime;
				GameEnd.lBankerTotallScore=m_lBankerWinScore;
				if (m_lBankerWinScore>0) GameEnd.lBankerScore=0;

				//扑克信息
				CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
				GameEnd.cbLeftCardCount=m_cbLeftCardCount;

				//发送积分
				GameEnd.cbTimeLeave=m_cbEndTime;	
				for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
				{
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserIndex);
					if ( pIServerUserItem == NULL ) continue;

					//设置成绩
					GameEnd.lUserScore=m_lUserWinScore[wUserIndex];

					//返还积分
					GameEnd.lUserReturnScore=m_lUserReturnScore[wUserIndex];

					//设置税收
					if (m_lUserRevenue[wUserIndex]>0) GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
					else if (m_wCurrentBanker!=INVALID_CHAIR) GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
					else GameEnd.lRevenue=0;

					//发送消息					
					m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				}
			}

			//扣除分数
			if (m_lUserWinScore[m_wCurrentBanker] != 0l)
			{
				m_pITableFrame->WriteUserScore(m_wCurrentBanker,m_lUserWinScore[m_wCurrentBanker], static_cast<long>(m_lUserRevenue[m_wCurrentBanker]), enScoreKind_Flee);
				m_lUserWinScore[m_wCurrentBanker] = 0;
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
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusFree.lAreaLimitScore = m_lAreaLimitScore;
			StatusFree.CheckImage = m_CheckImage;

			//庄家信息
			StatusFree.bEnableSysBanker=m_bEnableSysBanker;
			StatusFree.wBankerUser=m_wCurrentBanker;	
			StatusFree.cbBankerTime=m_wBankerTime;
			StatusFree.lBankerWinScore=m_lBankerWinScore;
			StatusFree.lBankerScore = 0x7fffffffffffffff;
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
				StatusFree.lBankerScore=pIServerUserItem->GetUserScore()->lScore;
			}

			//玩家信息
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusFree.lUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_lUserLimitScore*4); 
			}

			//全局信息
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusFree.cbTimeLeave=(BYTE)(m_cbFreeTime-__min(dwPassTime,m_cbFreeTime));

			//房间名称
			CopyMemory(StatusFree.szGameRoomName, m_pGameServiceOption->szGameRoomName, sizeof(StatusFree.szGameRoomName));

			//发送场景
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

			//限制提示
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄条件为：%I64d,区域限制为：%I64d,玩家限制为：%I64d"),m_lApplyBankerCondition,
				m_lAreaLimitScore,m_lUserLimitScore);

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
			CMD_S_StatusPlay StatusPlay={0};

			//全局下注
			CopyMemory(StatusPlay.lAllJettonScore,m_lAllJettonScore,sizeof(StatusPlay.lAllJettonScore));

			//玩家下注
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
					StatusPlay.lUserJettonScore[nAreaIndex] = m_lUserJettonScore[nAreaIndex][wChiarID];

				//最大下注
				StatusPlay.lUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_lUserLimitScore);
			}

			//控制信息
			StatusPlay.lApplyBankerCondition=m_lApplyBankerCondition;		
			StatusPlay.lAreaLimitScore=m_lAreaLimitScore;		

			//庄家信息
			StatusPlay.bEnableSysBanker=m_bEnableSysBanker;
			StatusPlay.wBankerUser=m_wCurrentBanker;			
			StatusPlay.cbBankerTime=m_wBankerTime;
			StatusPlay.lBankerWinScore=m_lBankerWinScore;	
			StatusPlay.CheckImage = m_CheckImage;
			StatusPlay.lBankerScore = 0x7fffffffffffffff;
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				StatusPlay.lBankerScore=m_lBankerScore;
			}	

			//全局信息
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			int	nTotalTime = (cbGameStatus==GS_PLACE_JETTON?m_cbBetTime:m_cbEndTime);
			StatusPlay.cbTimeLeave=(BYTE)(nTotalTime-__min(dwPassTime,(DWORD)nTotalTime));
			StatusPlay.cbGameStatus=m_pITableFrame->GetGameStatus();						

			//结束判断
			if (cbGameStatus==GS_GAME_END)
			{
				StatusPlay.cbTimeLeave=(BYTE)(m_cbEndTime-__min(dwPassTime,m_cbEndTime));

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

				//扑克信息
				CopyMemory(StatusPlay.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
			}

			//房间名称
			CopyMemory(StatusPlay.szGameRoomName, m_pGameServiceOption->szGameRoomName, sizeof(StatusPlay.szGameRoomName));

			//发送场景
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

			//限制提示
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄条件为：%I64d,区域限制为：%I64d,玩家限制为：%I64d"),m_lApplyBankerCondition,
				m_lAreaLimitScore,m_lUserLimitScore);
			
			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);
			
			//发送申请者
			SendApplyUser( pIServerUserItem );

			return bSuccess;
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
			//开始游戏
			m_pITableFrameControl->StartGame();

			//设置时间
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,m_cbBetTime*1000,1,0L);

			//设置状态
			m_pITableFrame->SetGameStatus(GS_PLACE_JETTON);

			return true;
		}
	case IDI_PLACE_JETTON:		//下注时间
		{
			//状态判断(防止强退重复设置)
			if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
			{
				//设置状态
				m_pITableFrame->SetGameStatus(GS_GAME_END);			

				//结束游戏
				OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);

				//设置时间
				m_dwJettonTime=(DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);			
			}

			return true;
		}
	case IDI_GAME_END:			//结束游戏
		{
			//写入积分
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserChairID);
				if ( pIServerUserItem == NULL ) continue;

				//胜利类型
				enScoreKind ScoreKind=(m_lUserWinScore[wUserChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;

				//写入积分
				if (m_lUserWinScore[wUserChairID]!=0L) m_pITableFrame->WriteUserScore(wUserChairID,m_lUserWinScore[wUserChairID], m_lUserRevenue[wUserChairID], ScoreKind);
			}

			//结束游戏
			m_pITableFrame->ConcludeGame();

			//读取配置
			if (m_bRefreshCfg)
				ReadConfigInformation(false);

			//切换庄家
			ChangeBanker(false);

			//设置时间
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,0L);

			//发送消息
			CMD_S_GameFree GameFree;
			ZeroMemory(&GameFree,sizeof(GameFree));

			GameFree.cbTimeLeave = m_cbFreeTime;
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
	case SUB_C_CONTINUE_CARD:		//继续发牌
		{
			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus==US_LOOKON) return true;
			if (pUserData->wChairID!=m_wCurrentBanker) return true;
			if (m_cbLeftCardCount < 8) return true;

			//设置变量
			m_bContiueCard=true;

			//发送消息
			SendGameMessage(pUserData->wChairID,TEXT("设置成功，下一局将继续发牌！"));

			return true;
		}
	case SUB_C_CHECK_IMAGE:		//用户选择背景图
		{
			ASSERT(wDataSize==sizeof(CMD_C_CheckImage));
			if (wDataSize!=sizeof(CMD_C_CheckImage)) return false;

			CMD_C_CheckImage * pCheckImage=(CMD_C_CheckImage *)pDataBuffer;
			this->m_CheckImage = pCheckImage->Index;
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CHECK_IMAGE, pCheckImage, sizeof(CMD_C_CheckImage));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CHECK_IMAGE, pCheckImage, sizeof(CMD_C_CheckImage));
			return true;

		}
	case SUB_C_ADMIN_COMMDN:
		{
			ASSERT(wDataSize==sizeof(CMD_C_ControlApplication));
			if(wDataSize!=sizeof(CMD_C_ControlApplication)) return false;

			if ( (pIServerUserItem->GetUserData()->dwUserRight&UR_GAME_CONTROL) == 0 )
				return false;

			//消息处理
			CMD_C_ControlApplication * pControlApplication=(CMD_C_ControlApplication *)pDataBuffer;
			return OnAdminControl(pControlApplication, pIServerUserItem);
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
		m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,NULL);
		m_pITableFrame->SetGameStatus(GS_FREE);
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
		//切换庄家
		if (wChairID==m_wCurrentBanker)
		{
			ChangeBanker(true);
			m_bContiueCard=false;
		}

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
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONGLONG lJettonScore)
{
	//效验参数
	ASSERT((cbJettonArea<=AREA_COUNT && cbJettonArea>=1)&&(lJettonScore>0L));
	if ((cbJettonArea>AREA_COUNT)||(lJettonScore<=0L) || cbJettonArea<1)
	{
		return false;
	}

	if (m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON)
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
	if (m_wCurrentBanker==wChairID) return true;
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) return true;

	//变量定义
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	LONGLONG lJettonCount=0L;
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lJettonCount += m_lUserJettonScore[nAreaIndex][wChairID];

	//玩家积分
	LONGLONG lUserScore = pIServerUserItem->GetUserScore()->lScore;

	//合法校验
	if (lUserScore < lJettonCount + lJettonScore) return true;
	if (m_lUserLimitScore < lJettonCount + lJettonScore) return true;

	//成功标识
	bool bPlaceJettonSuccess=true;

	//合法验证
	if (GetUserMaxJetton(wChairID,cbJettonArea) >= lJettonScore)
	{
		//机器人验证
		if(pIServerUserItem->IsAndroidUser())
		{
			//区域限制
			if (m_lRobotAreaScore[cbJettonArea] + lJettonScore > m_lRobotAreaLimit)
				return true;

			//数目限制
			bool bHaveChip = false;
			for (int i = 0; i < AREA_COUNT; i++)
			{
				if (m_lUserJettonScore[i+1][wChairID] != 0)
					bHaveChip = true;
			}

			if (!bHaveChip)
			{
				if (m_nChipRobotCount+1 > m_nMaxChipRobot)
				{
					bPlaceJettonSuccess = false;
				}
				else
					m_nChipRobotCount++;
			}

			//统计分数
			if (bPlaceJettonSuccess)
				m_lRobotAreaScore[cbJettonArea] += lJettonScore;
		}

		if (bPlaceJettonSuccess)
		{
			//保存下注
			m_lAllJettonScore[cbJettonArea] += lJettonScore;
			m_lUserJettonScore[cbJettonArea][wChairID] += lJettonScore;			
		}	
	}
	else
	{
		bPlaceJettonSuccess=false;
	}

	if (bPlaceJettonSuccess)
	{
		//变量定义
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

		//构造变量
		PlaceJetton.wChairID=wChairID;
		PlaceJetton.cbJettonArea=cbJettonArea;
		PlaceJetton.lJettonScore=lJettonScore;

		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (pIServerUserItem != NULL) 
			PlaceJetton.cbAndroid = pIServerUserItem->IsAndroidUser()? TRUE : FALSE;

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
		PlaceJettonFail.wPlaceUser=wChairID;

		//发送消息
		m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
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


//发送扑克
bool CTableFrameSink::DispatchTableCard()
{
	BYTE Tmp[2000];

	m_TableCardLen = 0;

	MakeStirngToStirng((char*)Tmp,(INT)strlen((char*)Tmp),(char*)m_cbTableCard,m_TableCardLen);

	if(m_TableCardLen > CountArray(m_cbTableCard))
	{
		m_TableCardLen = CountArray(m_cbTableCard);
	}

	m_bcFirstPostCard = 0;
	m_GameLogic.RandCardList(m_cbTableCard,100);	
	CopyMemory(&m_cbTableCardArray[0][0], m_cbTableCard, sizeof(m_cbTableCardArray));

	//设置数目
	for (int i=0; i<CountArray(m_cbCardCount); ++i) 
		m_cbCardCount[i]=1;

	//发牌标志
	m_bContiueCard = false;

	return true;
}

//申请庄家
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	//合法判断
	LONGLONG lUserScore=pIApplyServerUserItem->GetUserScore()->lScore;
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
		TakeTurns();

		//设置变量
		bChangeBanker=true;
		m_bExchangeBanker = true;
	}
	//轮庄判断
	else if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//获取庄家
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);

		if(pIServerUserItem!= NULL)
		{
			LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

			//次数判断
			if (m_lPlayerBankerMAX<=m_wBankerTime || lBankerScore<m_lApplyBankerCondition)
			{
				//庄家增加判断 同一个庄家情况下只判断一次
				if(m_lPlayerBankerMAX <= m_wBankerTime && m_bExchangeBanker && lBankerScore>=m_lApplyBankerCondition)
				{
					//加庄局数设置：当庄家坐满设定的局数之后(m_lBankerMAX)，
					//所带金币值还超过下面申请庄家列表里面所有玩家金币时，
					//可以再加坐庄m_lBankerAdd局，加庄局数可设置。

					//金币超过m_lBankerScoreMAX之后，
					//就算是下面玩家的金币值大于他的金币值，他也可以再加庄m_lBankerScoreAdd局。
					bool bScoreMAX = true;
					m_bExchangeBanker = false;

					for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
					{
						//获取玩家
						WORD wChairID = m_ApplyUserArray[i];
						IServerUserItem *pIUserItem = m_pITableFrame->GetServerUserItem(wChairID);
						LONGLONG lScore = pIServerUserItem->GetUserScore()->lScore;

						if ( wChairID != m_wCurrentBanker && lBankerScore <= lScore )
						{
							bScoreMAX = false;
							break;
						}
					}

					if ( bScoreMAX || (lBankerScore > m_lBankerScoreMAX && m_lBankerScoreMAX != 0l) )
					{
						if ( bScoreMAX )
						{
							m_lPlayerBankerMAX += m_lBankerAdd;
						}
						if ( lBankerScore > m_lBankerScoreMAX && m_lBankerScoreMAX != 0l )
						{
							m_lPlayerBankerMAX += m_lBankerScoreAdd;
						}
						return true;
					}
				}

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
				TakeTurns();

				bChangeBanker=true;
				m_bExchangeBanker = true;

				//提示消息
				TCHAR szTipMsg[128];
				if (lBankerScore<m_lApplyBankerCondition)
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]分数少于(%I64d)，强行换庄!"),pIServerUserItem->GetAccounts(),m_lApplyBankerCondition);
				else
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]做庄次数达到(%d)，强行换庄!"),pIServerUserItem->GetAccounts(),m_lPlayerBankerMAX);

				//发送消息
				SendGameMessage(INVALID_CHAIR,szTipMsg);	
			}
		}
		else
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
		}

	}
	//系统做庄
	else if (m_wCurrentBanker==INVALID_CHAIR && m_ApplyUserArray.GetCount()!=0)
	{
		//轮换判断
		TakeTurns();

		bChangeBanker=true;
		m_bExchangeBanker = true;
	}

	//切换判断
	if (bChangeBanker)
	{
		//最大坐庄数
		m_lPlayerBankerMAX = m_lBankerMAX;

		//设置变量
		m_wBankerTime = 0;
		m_lBankerWinScore=0;

		//发送消息
		CMD_S_ChangeBanker sChangeBanker;
		ZeroMemory(&sChangeBanker,sizeof(sChangeBanker));
		sChangeBanker.wBankerUser=m_wCurrentBanker;
		sChangeBanker.lBankerScore = 0x7fffffffffffffff;
		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
			sChangeBanker.lBankerScore=pIServerUserItem->GetUserScore()->lScore;
		}
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&sChangeBanker,sizeof(sChangeBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&sChangeBanker,sizeof(sChangeBanker));

		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			//读取消息
			LONGLONG lMessageCount=GetPrivateProfileInt(m_szGameRoomName,TEXT("MessageCount"),0,m_szConfigFileName);
			if (lMessageCount!=0)
			{
				//读取配置
				LONGLONG lIndex=rand()%lMessageCount;
				TCHAR szKeyName[32],szMessage1[256],szMessage2[256];				
				_sntprintf(szKeyName,CountArray(szKeyName),TEXT("Item%I64d"),lIndex);
				GetPrivateProfileString(m_szGameRoomName,szKeyName,TEXT("恭喜[ %s ]上庄"),szMessage1,CountArray(szMessage1),m_szConfigFileName);

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

//轮换判断
void CTableFrameSink::TakeTurns()
{
	//变量定义
	int nInvalidApply = 0;

	for (int i = 0; i < m_ApplyUserArray.GetCount(); i++)
	{
		if (m_pITableFrame->GetGameStatus() == GS_FREE)
		{
			//获取分数
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_ApplyUserArray[i]);
			if (pIServerUserItem != NULL)
			{
				if (pIServerUserItem->GetUserScore()->lScore >= m_lApplyBankerCondition)
				{
					m_wCurrentBanker=m_ApplyUserArray[i];
					break;
				}
				else
				{
					nInvalidApply = i + 1;

					//发送消息
					CMD_S_CancelBanker CancelBanker = {};

					//设置变量
					lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));

					//发送消息
					m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
					m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

					//提示消息
					TCHAR szTipMsg[128] = {};
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("由于你的金币数（%I64d）少于坐庄必须金币数（%I64d）,你无法上庄！"),
						pIServerUserItem->GetUserScore()->lScore, m_lApplyBankerCondition);
					SendGameMessage(m_ApplyUserArray[i],szTipMsg);
				}
			}
		}
	}

	//删除玩家
	if (nInvalidApply != 0)
		m_ApplyUserArray.RemoveAt(0, nInvalidApply);
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

//用户断线
bool __cdecl CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) 
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
		lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

	return true;
}
void   CTableFrameSink::GetAllWinArea(BYTE bcWinArea[],BYTE bcAreaCount,BYTE InArea)
{
	if (InArea==0xFF)
	{
		return ;
	}
	ZeroMemory(bcWinArea,bcAreaCount);


	LONGLONG lMaxSocre = 0;

	for (int i = 0;i<32;i++)
	{
		BYTE bcOutCadDataWin[AREA_COUNT];
		BYTE bcData[1];
		bcData[0]=i+1;
		m_GameLogic.GetCardType(bcData,1,bcOutCadDataWin);
		for (int j= 0;j<AREA_COUNT;j++)
		{

			if(bcOutCadDataWin[j]>1&&j==InArea-1)
			{
				LONGLONG Score = 0; 
				for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
				{
					if(bcOutCadDataWin[nAreaIndex-1]>1)
					{
						Score += m_lAllJettonScore[nAreaIndex]*(bcOutCadDataWin[nAreaIndex-1]);
					}
				}
				if(Score>=lMaxSocre)
				{
					lMaxSocre = Score;
					CopyMemory(bcWinArea,bcOutCadDataWin,bcAreaCount);

				}
				break;
			}
		}
	}
}
//最大下注
LONGLONG CTableFrameSink::GetUserMaxJetton(WORD wChairID,BYTE Area)
{
	IServerUserItem *pIMeServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	if (NULL==pIMeServerUserItem) return 0L;


	//已下注额
	LONGLONG lNowJetton = 0;
	ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lNowJetton += m_lUserJettonScore[nAreaIndex][wChairID];

	//庄家金币
	LONGLONG lBankerScore=0x7fffffffffffffff;
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		IServerUserItem *pIUserItemBanker=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		if (NULL!=pIUserItemBanker) lBankerScore=pIUserItemBanker->GetUserScore()->lScore;
	}

	BYTE bcWinArea[AREA_COUNT];
	LONGLONG LosScore = 0;
	LONGLONG WinScore = 0;

	GetAllWinArea(bcWinArea,AREA_COUNT,Area);

	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
	{
		if(bcWinArea[nAreaIndex-1]>1)
		{
			LosScore+=m_lAllJettonScore[nAreaIndex]*(bcWinArea[nAreaIndex-1]);
		}else
		{
			if(bcWinArea[nAreaIndex-1]==0)
			{
				WinScore+=m_lAllJettonScore[nAreaIndex];

			}
		}
	}
	lBankerScore = lBankerScore + WinScore - LosScore;

	if ( lBankerScore < 0 )
	{
		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			IServerUserItem *pIUserItemBanker=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
			if (NULL!=pIUserItemBanker) 
				lBankerScore=pIUserItemBanker->GetUserScore()->lScore;
		}
		else
		{
			lBankerScore = 0x7fffffffffffffff;
		}
	}

	//个人限制
	LONGLONG lMeMaxScore = min((pIMeServerUserItem->GetUserScore()->lScore-lNowJetton), m_lUserLimitScore);

	//区域限制
	lMeMaxScore=min(lMeMaxScore,m_lAreaLimitScore);

	BYTE diMultiple[AREA_COUNT];

	for (int i = 0;i<32;i++)
	{
		BYTE bcData[1];
		bcData[0]= i+1;
		BYTE  bcOutCadDataWin[AREA_COUNT];
		m_GameLogic.GetCardType(bcData,1,bcOutCadDataWin);
		for (int j = 0;j<AREA_COUNT;j++)
		{
			if(bcOutCadDataWin[j]>1)
			{
				diMultiple[j] = bcOutCadDataWin[j];

			}
		}
	}
	//庄家限制
	lMeMaxScore=min(lMeMaxScore,lBankerScore/(diMultiple[Area-1]));

	//非零限制
	ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return (LONGLONG)(lMeMaxScore);
}
//计算得分
LONGLONG CTableFrameSink::CalculateScore()
{
	//变量定义
	LONGLONG static cbRevenue=m_pGameServiceOption->wRevenue;

	//推断玩家
	bool static bWinTianMen, bWinDiMen, bWinXuanMen,bWinHuang;
	BYTE TianMultiple,diMultiple,TianXuanltiple,HuangMultiple;
	TianMultiple  = 1;
	diMultiple = 1 ;
	TianXuanltiple = 1;
	HuangMultiple = 1;

	BYTE  bcResulteOut[AREA_COUNT];
	memset(bcResulteOut,0,AREA_COUNT);
	m_GameLogic.GetCardType(&m_cbTableCardArray[0][0],1,bcResulteOut);

	//游戏记录
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];

	BYTE  cbMultiple[AREA_COUNT]={1};

	for (WORD wAreaIndex = 1; wAreaIndex <= AREA_COUNT; ++wAreaIndex)
	{

		if(bcResulteOut[wAreaIndex-1]>0)
		{
			GameRecord.bWinMen[wAreaIndex-1] = 4;
		}
		else
		{
			GameRecord.bWinMen[wAreaIndex-1] = 0;
		}
	}

	//移动下标
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst ) m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;

	//庄家总量
	LONGLONG lBankerWinScore = 0;

	//玩家成绩
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	LONGLONG lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	//玩家下注
	LONGLONG *pUserScore[AREA_COUNT+1];
	pUserScore[0]=NULL;
	for (int i = 1;i<AREA_COUNT+1;i++)
	{
		pUserScore[i]=m_lUserJettonScore[i];
	}

	//计算积分
	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//庄家判断
		if (m_wCurrentBanker==wChairID) continue;

		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;

		for (WORD wAreaIndex = 1; wAreaIndex <= AREA_COUNT; ++wAreaIndex)
		{

			if (bcResulteOut[wAreaIndex-1]>0) 
			{
				m_lUserWinScore[wChairID] += ( pUserScore[wAreaIndex][wChairID] *(bcResulteOut[wAreaIndex-1])) ;
				m_lUserReturnScore[wChairID] += pUserScore[wAreaIndex][wChairID] ;
				lBankerWinScore -= ( pUserScore[wAreaIndex][wChairID] * (bcResulteOut[wAreaIndex-1]) ) ;
			}
			else
			{
				if (bcResulteOut[wAreaIndex-1]==0)
				{
					lUserLostScore[wChairID] -= pUserScore[wAreaIndex][wChairID];
					lBankerWinScore += pUserScore[wAreaIndex][wChairID];
				}
				else
				{
					//如果为1则不少分
					m_lUserWinScore[wChairID] += 0;
					m_lUserReturnScore[wChairID] += pUserScore[wAreaIndex][wChairID] ;
				}
			}
		}

		//计算税收
		if (0 < m_lUserWinScore[wChairID])
		{
			float fRevenuePer=float(cbRevenue/1000.+0.000000);
			m_lUserRevenue[wChairID]  = LONGLONG(m_lUserWinScore[wChairID]*fRevenuePer);
			m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];
		}

		//总的分数
		m_lUserWinScore[wChairID] += lUserLostScore[wChairID];
	}

	//庄家成绩
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;

		//计算税收
		if (0 < m_lUserWinScore[m_wCurrentBanker])
		{
			float fRevenuePer=float(cbRevenue/1000.+0.000000);
			m_lUserRevenue[m_wCurrentBanker]  = LONGLONG(m_lUserWinScore[m_wCurrentBanker]*fRevenuePer);
			m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
			lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
		}	
	}

	//累计积分
	m_lBankerWinScore += lBankerWinScore;

	//当前积分
	m_lBankerCurGameScore=lBankerWinScore;

	return lBankerWinScore;
}

//试探性判断
bool CTableFrameSink::ProbeJudge()
{
	BYTE  bcResulteOut[AREA_COUNT];
	memset(bcResulteOut,0,AREA_COUNT);
	m_GameLogic.GetCardType(&m_cbTableCardArray[0][0],1,bcResulteOut);

	//系统输赢
	LONGLONG lSystemScore = 0l;

	//玩家下注
	LONGLONG *pUserScore[AREA_COUNT+1];
	pUserScore[0] = NULL;
	for (int i = 1;i<AREA_COUNT+1;i++)
	{
		pUserScore[i] = m_lUserJettonScore[i];
	}

	//庄家是不是机器人
	bool bIsBankerAndroidUser = false;
	if ( m_wCurrentBanker != INVALID_CHAIR )
	{
		IServerUserItem * pIBankerUserItem = m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		if (pIBankerUserItem != NULL) 
		{
			bIsBankerAndroidUser = pIBankerUserItem->IsAndroidUser();
		}
	}

	//计算积分
	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//庄家判断
		if (m_wCurrentBanker == wChairID) continue;

		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (pIServerUserItem==NULL) 
			continue;

		bool bIsAndroidUser = pIServerUserItem->IsAndroidUser();

		for (WORD wAreaIndex = 1; wAreaIndex <= AREA_COUNT; ++wAreaIndex)
		{

			if (bcResulteOut[wAreaIndex-1]>0) 
			{
				if ( bIsAndroidUser )
					lSystemScore += (pUserScore[wAreaIndex][wChairID] *(bcResulteOut[wAreaIndex-1]));

				if (m_wCurrentBanker == INVALID_CHAIR || bIsBankerAndroidUser)
					lSystemScore -= (pUserScore[wAreaIndex][wChairID] *(bcResulteOut[wAreaIndex-1]));
			}
			else
			{
				if (bcResulteOut[wAreaIndex-1]==0)
				{
					if ( bIsAndroidUser )
						lSystemScore -= pUserScore[wAreaIndex][wChairID];

					if (m_wCurrentBanker == INVALID_CHAIR || bIsBankerAndroidUser)
						lSystemScore += pUserScore[wAreaIndex][wChairID];
				}
			}
		}
	}

	//系统分值计算
	if ((lSystemScore + m_StorageStart) < 0l)
	{
		return false;
	}
	else
	{
		m_StorageStart += lSystemScore;
		m_StorageStart = m_StorageStart - (m_StorageStart * m_StorageDeduct / 1000);
		return true;
	}
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
	if (wBufferSize>0) m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
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

//读取配置
void CTableFrameSink::ReadConfigInformation(bool bReadFresh)
{
	TCHAR OutBuf[255];

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("Score"),TEXT("10000"),OutBuf,255,m_szConfigFileName);
	_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_lApplyBankerCondition);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("AreaLimitScore"),TEXT("100000000"),OutBuf,255,m_szConfigFileName);
	_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_lAreaLimitScore);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("UserLimitScore"),TEXT("100000000"),OutBuf,255,m_szConfigFileName);
	_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_lUserLimitScore);

	BYTE cbEnableSysBanker=GetPrivateProfileInt(m_szGameRoomName, TEXT("EnableSysBanker"), 1, m_szConfigFileName);
	m_bEnableSysBanker = cbEnableSysBanker != 0 ? true : false;

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("StorageStart"),TEXT("0"),OutBuf,255,m_szConfigFileName);
	_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_StorageStart);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("StorageDeduct"),TEXT("1"),OutBuf,255,m_szConfigFileName);
	_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_StorageDeduct);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("Time"),TEXT("10"),OutBuf,255,m_szConfigFileName);
	_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_lBankerMAX);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("TimeAdd"),TEXT("10"),OutBuf,255,m_szConfigFileName);
	_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_lBankerAdd);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("BankerScore"),TEXT("100000000000"),OutBuf,255,m_szConfigFileName);
	_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_lBankerScoreMAX);

	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("BankerAdd"),TEXT("10"),OutBuf,255,m_szConfigFileName);
	_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_lBankerScoreAdd);

	//下注机器人数目
	m_nMaxChipRobot = GetPrivateProfileInt(m_szGameRoomName, TEXT("RobotBetCount"), 10, m_szConfigFileName);
	if (m_nMaxChipRobot < 0)	m_nMaxChipRobot = 10;

	//机器人下注限制
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_szGameRoomName, TEXT("RobotAreaLimit"), _T("50000000"), OutBuf, 255, m_szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), _T("%I64d"), &m_lRobotAreaLimit);


	//时间控制
	m_cbFreeTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("FreeTime"), TIME_FREE, m_szConfigFileName);

	m_cbBetTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("BetTime"), TIME_PLACE_JETTON, m_szConfigFileName);

	m_cbEndTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("EndTime"), TIME_GAME_END, m_szConfigFileName);

	if (m_cbFreeTime <= 0 || m_cbFreeTime > 99)			
		m_cbFreeTime = TIME_FREE;
	if (m_cbBetTime <= 0 || m_cbBetTime > 99)		
		m_cbBetTime = TIME_PLACE_JETTON;
	if (m_cbEndTime <= 0 || m_cbEndTime > 99)			
		m_cbEndTime = TIME_GAME_END;

	if ( m_lBankerScoreMAX <= m_lApplyBankerCondition)
		m_lBankerScoreMAX = 0l;

	m_lPlayerBankerMAX = m_lBankerMAX;
}

//////////////////////////////////////////////////////////////////////////
//银行操作
#ifdef __SPECIAL___
bool __cdecl CTableFrameSink::OnActionUserBank(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return true;
}
#endif

//控制
bool CTableFrameSink::OnAdminControl( CMD_C_ControlApplication* pData , IServerUserItem * pIServerUserItem)
{
	switch(pData->cbControlAppType)
	{
	case C_CA_UPDATE:	//更新
		{
			CMD_S_ControlReturns ControlReturns;
			ZeroMemory(&ControlReturns,sizeof(ControlReturns));
			ControlReturns.cbReturnsType = S_CR_UPDATE_SUCCES;
			ControlReturns.cbControlArea = m_cbControlArea;
			ControlReturns.cbControlTimes = m_cbControlTimes;
			m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
		}
		return true;
	case C_CA_SET:		//设置
		{
			if ( pData->cbControlArea == 0xff || pData->cbControlArea  > 7 )
			{
				//设置失败
				CMD_S_ControlReturns ControlReturns;
				ZeroMemory(&ControlReturns,sizeof(ControlReturns));
				ControlReturns.cbReturnsType = S_CR_FAILURE;
				ControlReturns.cbControlArea = 0xff;
				m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
				return true;
			}
			m_cbControlArea = pData->cbControlArea;
			m_cbControlTimes = pData->cbControlTimes;
			CMD_S_ControlReturns ControlReturns;
			ZeroMemory(&ControlReturns,sizeof(ControlReturns));
			ControlReturns.cbReturnsType = S_CR_SET_SUCCESS;
			ControlReturns.cbControlArea = m_cbControlArea;
			ControlReturns.cbControlTimes = m_cbControlTimes;
			m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
		}
		return true;
	case C_CA_CANCELS:	//取消
		{
			m_cbControlArea = 0xff;
			m_cbControlTimes = 0;
			CMD_S_ControlReturns ControlReturns;
			ZeroMemory(&ControlReturns,sizeof(ControlReturns));
			ControlReturns.cbReturnsType = S_CR_CANCEL_SUCCESS;
			ControlReturns.cbControlArea = 0xff;
			m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
		}
		return true;
	}
	//设置失败
	CMD_S_ControlReturns ControlReturns;
	ZeroMemory(&ControlReturns,sizeof(ControlReturns));
	ControlReturns.cbReturnsType = S_CR_FAILURE;
	ControlReturns.cbControlArea = 0xff;
	m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
	return true;
}
//////////////////////////////////////////////////////////////////////////
