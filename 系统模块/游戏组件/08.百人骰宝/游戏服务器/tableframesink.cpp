#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//常量定义
#define SEND_COUNT					300									//发送次数


//下注时间
#define IDI_FREE					1									//空闲时间
#ifdef _DEBUG
#define TIME_FREE					10									//空闲时间
#else
#define TIME_FREE					10									//空闲时间
#endif

//下注时间
#define IDI_PLACE_JETTON			2									//下注时间

#ifdef _DEBUG
#define TIME_PLACE_JETTON			15									//下注时间
#else
#define TIME_PLACE_JETTON			20									//下注时间
#endif

//结束时间
#define IDI_GAME_END				3									//结束时间

#ifdef _DEBUG
#define TIME_GAME_END				20									//结束时间
#else
#define TIME_GAME_END				20									//结束时间
#endif

#define  MONEY_NAME               TEXT("豪币")                          // 货币名称

//////////////////////////////////////////////////////////////////////////
#define _GFIX(a) (a>0?a:0)
#define _GABS(a) (a>0?a:-a)


CString FormatNumber(__int64 i64)
{
	CString str;
	str.Format(TEXT("%I64d"),i64>=0?i64:-i64);
	//int p=str.GetLength()-4;
	//while(p>0)
	//{
	//	str.Insert(p,TEXT(","));
	//	p-=4;
	//}
	if(i64<0)
		str=TEXT("-")+str;
	return str;
}
//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//游戏人数
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_TimeControl;	//开始模式

//////////////////////////////////////////////////////////////////////////
//构造函数
CTableFrameSink::CTableFrameSink()
{
	//总下注数	
	ZeroMemory(m_lAllScore,sizeof(m_lAllScore));

	//个人下注
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));

	//控制变量
	m_cbWinSideControl=0;
	m_nSendCardCount=0;
	m_iNTWinPercent = 0;
	m_arPensonInfo.RemoveAll();
	
	//玩家成绩	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//扑克信息
	ZeroMemory(m_enCards,sizeof(m_enCards));
	ZeroMemory(m_bWinner,sizeof(m_bWinner));   // 输赢结果

	//状态变量
	m_dwJettonTime=0L;

	//庄家信息
	m_wApplyUserArray.RemoveAll();
	m_iMaxAndroidBanker=MAX_ANDROID_BANKER;
	m_wCurrentBanker=INVALID_CHAIR;
	m_wBankerTime=0;
	m_lBankerWinScore=0L;		
	m_lBankerCurGameScore=0L;
	m_bEnableSysBanker=true;
	m_lAllAndroidWinScore = 0;

	//记录变量
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;
	m_nRecordLast=0;
	m_dwRecordCount=0;

	//设置文件名
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_snprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\SicBoConfig.ini"),szPath);

	//控制变量
	__int64 lMaxScore=9223372036854775807;
	CString str;

	TCHAR szBuffer[MAX_PATH];
	//m_lApplyBankerCondition=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Score"), 100, m_szConfigFileName);
	GetPrivateProfileString(TEXT("BankerCondition"),TEXT("Score"),TEXT("100"),szBuffer,CountArray(szBuffer),m_szConfigFileName);
	m_lApplyBankerCondition=_atoi64(szBuffer);

	//m_lAreaLimitScore=GetPrivateProfileInt(TEXT("ScoreLimit"), TEXT("AreaLimitScore"), lMaxScore, m_szConfigFileName);
	GetPrivateProfileString(TEXT("ScoreLimit"),TEXT("AreaLimitScore"),TEXT("5000000000000000000"),szBuffer,CountArray(szBuffer),m_szConfigFileName);
	m_lAreaLimitScore=_atoi64(szBuffer);

	//m_lUserLimitScore=GetPrivateProfileInt(TEXT("ScoreLimit"), TEXT("UserLimitScore"), lMaxScore, m_szConfigFileName);
	GetPrivateProfileString(TEXT("ScoreLimit"),TEXT("UserLimitScore"),TEXT("50000000000000000"),szBuffer,CountArray(szBuffer),m_szConfigFileName);
	m_lUserLimitScore= _atoi64(szBuffer);
	BYTE byCtrlWinner = GetPrivateProfileInt(TEXT("ManagerControl"), TEXT("WinSide"), 0, m_szConfigFileName);
	m_cbWinSideControl=byCtrlWinner;
	m_iNTWinPercent = GetPrivateProfileInt(TEXT("ManagerControl"), TEXT("NTWinPercent"), 0, m_szConfigFileName);
	//m_lMaxAndroidLost = GetPrivateProfileInt(TEXT("ManagerControl"), TEXT("MaxAndroidLost"), 80000000, m_szConfigFileName);
	//m_lMaxAndroidLost = 0-m_lMaxAndroidLost;
	BYTE cbEnableSysBanker=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("EnableSysBanker"), 1, m_szConfigFileName);
	m_bEnableSysBanker=cbEnableSysBanker?true:false;
	m_bChangeSicboType=false;                      // 是否已经改变摇骰子的方式
	m_enSicboType=enSicboType_Sicbo;               // 摇骰子的方式
	m_bIsAllAndroid = false;
	m_iSameCount = 0;
	m_iAndroidRandCount = 0;
	m_iAndroidNTCount = -1;
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

	//控制接口
	m_pITableFrameControl=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrameControl);
	if (m_pITableFrameControl==NULL) return false;

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	TCHAR szMsg[_MAX_PATH]={"ManagerControl"};
	if (NULL != m_pGameServiceOption)
	{
		_sntprintf(szMsg, sizeof(szMsg), TEXT("ManagerControl%d"), m_pGameServiceOption->wServerID);
	}	
	m_iNTWinPercent = GetPrivateProfileInt(szMsg, TEXT("NTWinPercent"), m_iNTWinPercent, m_szConfigFileName);

	m_iAILostLimitPercent=GetPrivateProfileInt("ScoreLimit","AILostLimitPercent",600,m_szConfigFileName);

	m_iSameCount =  0;
	m_iAndroidRandCount= 0;
	m_iAndroidNTCount = -1;
	return true;
}

//复位桌子
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//总下注数	
	ZeroMemory(m_lAllScore,sizeof(m_lAllScore));

	//个人下注
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));

	//玩家成绩	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));
	m_bIsAllAndroid = false;
	m_iAndroidRandCount = 0;
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
	m_iUseWinner=enCardType_Illegal;
	m_iAndroidRandCount=0;

// 加载控制变量
	m_arPensonInfo.RemoveAll();
	//读取配置
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	//读取配置
	TCHAR szFileName[MAX_PATH];
	_snprintf(szFileName,sizeof(szFileName),TEXT("%s\\UserList.ini"),szPath);
	int iPersonNum = GetPrivateProfileInt( TEXT("Winner"), TEXT("PersonCount"), 0, szFileName );
	if (iPersonNum>50)  // 最多控制50人
	{
		iPersonNum = 50;
	}

	// 数值太大，则重置小一点
	if (m_lAllAndroidWinScore>9999999999999L)
	{
		m_lAllAndroidWinScore=8000000000000;
	}

	// 从配置文件文件中读取玩家名字
	for (int i=0; i<iPersonNum; ++i)
	{
		TCHAR szKeyName[_MAX_PATH]={0};
		S_PERSONINFO sPensonInfo;
		ZeroMemory(&sPensonInfo, sizeof(sPensonInfo));
		_sntprintf(szKeyName, sizeof(szKeyName), "UserID%d",(i+1));
		// 读取玩家名称
		sPensonInfo.dwUserID = GetPrivateProfileInt( TEXT("Winner"), szKeyName, 0, szFileName );
		if (0 == sPensonInfo.dwUserID)  // 为空时不设置
		{
			continue;
		}
		// 控制胜率的读取
		_sntprintf(szKeyName, sizeof(szKeyName), "Percent%d",(i+1));
		sPensonInfo.iPercent = GetPrivateProfileInt( TEXT("Winner"), szKeyName, 0, szFileName );
		// 初始化座位号
		m_arPensonInfo.Add(sPensonInfo);
	}
	m_iAILostLimitPercent=GetPrivateProfileInt("ScoreLimit","AILostLimitPercent",600,m_szConfigFileName);

	//获取庄家
	IServerUserItem *pIBankerServerUserItem=NULL;
	if (INVALID_CHAIR!=m_wCurrentBanker)
	{
		pIBankerServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
	}
	ZeroMemory(m_bWinner,sizeof(m_bWinner));        // 输赢结果
	m_bChangeSicboType=false;                       // 是否已经改变摇骰子的方式
	m_enSicboType=enSicboType_Sicbo;                // 摇骰子的方式
	
	//设置变量
	GameStart.cbTimeLeave=(BYTE)TIME_PLACE_JETTON;
	GameStart.wBankerUser=m_wCurrentBanker;
	if (pIBankerServerUserItem!=NULL)
	{
		GameStart.lBankerScore=pIBankerServerUserItem->GetUserScore()->lScore;
		GameStart.lAllMaxScore =GameStart.lBankerScore;
		if (m_iAndroidNTCount<0) // 检查庄家队列
		{
			BOOL bHaveUser = false;
			for (INT i=0; i<m_wApplyUserArray.GetCount(); ++i)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wApplyUserArray[i]);
				if (NULL != pIServerUserItem)
				{
					if (!pIServerUserItem->IsAndroidUser())
					{
						bHaveUser = true;
						break;
					}
				}
			}
			if (bHaveUser)
			{
				m_iAndroidNTCount = rand()%3 + 1;
			}
			else
			{
				m_iAndroidNTCount = -1;
			}
		}
	}
	else
	{
		GameStart.lAllMaxScore = m_lAreaLimitScore;
	}

    //旁观玩家
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	

	//游戏玩家
	m_bIsAllAndroid = true;
	bool bAllUser = true;  // 所有的人都是玩家
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;
		if (pIServerUserItem->IsAndroidUser())
		{
			bAllUser =false;
		}
		else
		{
			m_bIsAllAndroid = false;
		}

		//设置积分
		GameStart.lUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_lUserLimitScore);

		// 有人坐庄时，考虑玩家通赔时是否足够
		if (pIBankerServerUserItem!=NULL)
		{
			GameStart.lUserMaxScore = min(GameStart.lBankerScore, GameStart.lUserMaxScore);
		}
		m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	
	}
	// 所有的都是玩家，与所有都是机器人相同
	if (bAllUser)
	{
		m_bIsAllAndroid=true;
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
			//计算分数
			__int64 lBankerWinScore=CalculateScore();

			//递增次数
			m_wBankerTime++;

			//结束消息
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//庄家信息
			GameEnd.nBankerTime = m_wBankerTime;
			GameEnd.lBankerTotallScore=m_lBankerWinScore;
			GameEnd.lBankerScore=lBankerWinScore;

			// 骰子点数
			CopyMemory(GameEnd.enCards, m_enCards,sizeof(m_enCards));
			for (int i=0; i<COUNT_AZIMUTH; ++i)
			{
				GameEnd.bWinner[i] = m_bWinner[i];
			}

			//发送积分
			GameEnd.cbTimeLeave=TIME_GAME_END;
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
				__int64 lScore=0;
				__int64 lRevenue=0;
				enScoreKind ScoreKind=enScoreKind_Flee;

				//统计成绩, 每个方位下注的值统计
				for (int i=0; i<COUNT_AZIMUTH; ++i)
				{
					lScore += m_lUserScore[i][wChairID];
					//上面的不翻倍，下面的翻倍
					//lScore -= m_lUserScore[i][wChairID] * CGameLogic::m_i64Loss_Percent[i];
					m_lUserScore[i][wChairID]=-m_lUserScore[i][wChairID];
				}

				IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
				if(lScore>pIServerUserItem->GetUserData()->UserScoreInfo.lScore)
					lScore=pIServerUserItem->GetUserData()->UserScoreInfo.lScore;

				//写入积分
				if (0L != lScore)
				{
					m_pITableFrame->WriteUserScore(pIServerUserItem, -lScore,lRevenue, ScoreKind);
				}

				return true;
			}

			//状态判断
			if (GS_GAME_END != m_pITableFrame->GetGameStatus())
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
				m_pITableFrame->SetGameTimer(IDI_GAME_END,TIME_GAME_END*1000,1,0L);

				//计算分数
				CalculateScore();

				//结束消息
				CMD_S_GameEnd GameEnd;
				ZeroMemory(&GameEnd,sizeof(GameEnd));

				//庄家信息
				GameEnd.nBankerTime = m_wBankerTime;
				GameEnd.lBankerTotallScore=m_lBankerWinScore;
				if (m_lBankerWinScore>0) GameEnd.lBankerScore=0;

				// 骰子点数
				CopyMemory(GameEnd.enCards, m_enCards,sizeof(m_enCards));
				for (int i=0; i<COUNT_AZIMUTH; ++i)
				{
					GameEnd.bWinner[i] = m_bWinner[i];
				}

				//发送积分
				GameEnd.cbTimeLeave=TIME_GAME_END;	
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
			if (m_lUserWinScore[m_wCurrentBanker]<0)
			{
				m_pITableFrame->WriteUserScore(m_wCurrentBanker,m_lUserWinScore[m_wCurrentBanker], m_lUserRevenue[m_wCurrentBanker], enScoreKind_Flee);
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

			//庄家信息
			StatusFree.bEnableSysBanker=m_bEnableSysBanker;
			StatusFree.wBankerUser=m_wCurrentBanker;	
			StatusFree.cbBankerTime=m_wBankerTime;
			StatusFree.lBankerWinScore=m_lBankerWinScore;
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIBankerItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
				StatusFree.lBankerScore=pIBankerItem->GetUserScore()->lScore;
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
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//全局下注
			CopyMemory(StatusPlay.lAllScore, m_lAllScore, sizeof(m_lAllScore));

			IServerUserItem *pIBankerItem = NULL;
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				pIBankerItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
				StatusPlay.lBankerScore=pIBankerItem->GetUserScore()->lScore;
				StatusPlay.lAllMaxScore = StatusPlay.lBankerScore;
			}
			else
			{
				StatusPlay.lAllMaxScore = m_lAreaLimitScore;
			}

			for (BYTE i=0; i<COUNT_AZIMUTH; ++i)
			{
				StatusPlay.lUserScore[i] = m_lUserScore[i][wChiarID];
			}

			//玩家下注
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{				
				//最大下注
				StatusPlay.lUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_lUserLimitScore);
				if (GS_PLACE_JETTON == cbGameStatus)
				{
					// 考虑已经下过多少注
					//设置积分
					StatusPlay.lUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_lUserLimitScore);

					// 有人坐庄时，考虑玩家通赔时是否足够
					if (pIServerUserItem!=NULL)
					{
						StatusPlay.lUserMaxScore = min(StatusPlay.lBankerScore, StatusPlay.lUserMaxScore);
					}
				}
			}

			//控制信息
			StatusPlay.lApplyBankerCondition=m_lApplyBankerCondition;		
			StatusPlay.lAreaLimitScore=m_lAreaLimitScore;	

			//庄家信息
			StatusPlay.bEnableSysBanker=m_bEnableSysBanker;
			StatusPlay.wBankerUser=m_wCurrentBanker;			
			StatusPlay.cbBankerTime=m_wBankerTime;
			StatusPlay.lBankerWinScore=m_lBankerWinScore;

			//全局信息
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusPlay.cbTimeLeave=(BYTE)(TIME_PLACE_JETTON-__min(dwPassTime,TIME_PLACE_JETTON));
			StatusPlay.cbGameStatus=m_pITableFrame->GetGameStatus();			
			StatusPlay.bySicboType = (BYTE)m_enSicboType;

			//结束判断
			if (cbGameStatus==GS_GAME_END)
			{
				StatusPlay.cbTimeLeave=(BYTE)(TIME_GAME_END-__min(dwPassTime,TIME_GAME_END));
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

				// 骰子点数
				CopyMemory(StatusPlay.enCards, m_enCards,sizeof(m_enCards));

				for (BYTE i=0; i<COUNT_AZIMUTH; ++i)
				{
					StatusPlay.bWinner[i] = m_bWinner[i];
				}
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
	case IDI_FREE:		//空闲时间
		{
			//开始游戏
			m_pITableFrameControl->StartGame();

			//设置时间
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,TIME_PLACE_JETTON*1000,1,0L);

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
				
				// 无人坐庄时，控制输赢
				CtrlWinner();

				//结束游戏
				OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);

				//设置时间
				m_dwJettonTime=(DWORD)time(NULL);
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
				if ( pIServerUserItem == NULL ) continue;

				//胜利类型
				enScoreKind ScoreKind=(m_lUserWinScore[wUserChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;

				//写入积分
				if (m_lUserWinScore[wUserChairID]!=0L) m_pITableFrame->WriteUserScore(wUserChairID,m_lUserWinScore[wUserChairID], m_lUserRevenue[wUserChairID], ScoreKind);

				//坐庄判断
				__int64 lUserScore=pIServerUserItem->GetUserScore()->lScore;
				if (wUserChairID!=m_wCurrentBanker && lUserScore<m_lApplyBankerCondition)
				{
					for (WORD i=0; i<m_wApplyUserArray.GetCount(); ++i)
					{
						if (wUserChairID!=m_wApplyUserArray[i]) continue;

						//删除玩家
						m_wApplyUserArray.RemoveAt(i);

						//发送消息
						CMD_S_CancelBanker CancelBanker;
						ZeroMemory(&CancelBanker,sizeof(CancelBanker));

						//设置变量
						lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));
						CancelBanker.dwUserID = pIServerUserItem->GetUserID();

						//发送消息
						m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
						m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

						//提示消息
						TCHAR szTipMsg[_MAX_PATH]={0};
						_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("由于您的%s数少于坐庄必须%s数（%I64d）,你自动下庄！"),
							MONEY_NAME, MONEY_NAME, m_lApplyBankerCondition);
						SendGameMessage(wUserChairID,szTipMsg);

						break;
					}
				}
			}

			//结束游戏
			m_pITableFrame->ConcludeGame();

			// 判断队列中是否有庄
			--m_iAndroidNTCount;
			bool bChangeNT = false;
			if (0 == m_iAndroidNTCount)
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
				if (NULL != pIServerUserItem)
				{
					if (pIServerUserItem->IsAndroidUser())
					{
						bChangeNT = true;
					}
				}
			}

			//切换庄家
			ChangeBanker(bChangeNT);

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
			if ((0 != wDataSize) && (m_wApplyUserArray.GetCount()>m_iMaxAndroidBanker))
			{
				return true;
			}
			return OnUserApplyBanker(pIServerUserItem);	
		}
	case SUB_C_CANCEL_BANKER:		//取消做庄
		{
			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus==US_LOOKON) return true;

			if (0 != wDataSize)				
			{
				if (m_wCurrentBanker == pIServerUserItem->GetChairID())
				{
					return true;
				}
				if (m_wApplyUserArray.GetCount()<=(m_iMaxAndroidBanker+1))
				{
					return true;
				}
			}
			return OnUserCancelBanker(pIServerUserItem);	
		}
	case SUB_C_CHANGE_SICBO_TYPE:
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_ChangeSicboType));
			if (wDataSize!=sizeof(CMD_C_ChangeSicboType)) return false;

			return true;

			////用户效验
			//tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			//if (pUserData->cbUserStatus!=US_PLAY)
			//{
			//	return true;
			//}

			//// 不是庄家不能发
			//if (pUserData->wChairID != m_wCurrentBanker)
			//{
			//	return true;
			//}

			//// 发过了，不能再发
			//if (m_bChangeSicboType)
			//{
			//	return true;
			//}

			////消息处理
			//CMD_C_ChangeSicboType * pChangeSicboType=(CMD_C_ChangeSicboType *)pDataBuffer;
			//if ((pChangeSicboType->enSicboType <enSicboType_Desktop)
			//	|| (pChangeSicboType->enSicboType > enSicboType_InTheAir))
			//{
			//	return true;
			//}
			//m_bChangeSicboType = true;
			//m_enSicboType=pChangeSicboType->enSicboType;   // 摇骰子的方式
			////发送消息
			//m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_C_CHANGE_SICBO_TYPE,(void*)pChangeSicboType, sizeof(CMD_C_ChangeSicboType));
			//m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_C_CHANGE_SICBO_TYPE, (void*)pChangeSicboType, sizeof(CMD_C_ChangeSicboType));
			//return true;
		}
	case SUB_C_USE_WINNER:
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_UseWinner));
			if (wDataSize!=sizeof(CMD_C_UseWinner)) return false;
			// 非下注状态不处理
			if (GS_PLACE_JETTON != m_pITableFrame->GetGameStatus())
			{
				return true;
			}
			//消息处理
			CMD_C_UseWinner * pUseWinner=(CMD_C_UseWinner *)pDataBuffer;
			if ((pUseWinner->byArea>= COUNT_AZIMUTH)
				|| ((USER_LOSER != pUseWinner->byType)
				&& (USER_WINNER != pUseWinner->byType)))
			{
				return false;
			}

			// 当前账号不是控制账号则不处理
			for (int i=0; i<m_arPensonInfo.GetCount(); ++i)
			{
				if (pIServerUserItem->GetUserID() == m_arPensonInfo[i].dwUserID)
				{
					// 控制胜负
					m_iUseWinner = pUseWinner->byArea;
					break;
				}
			}
			return true;
		}
	default:
		break;
	}
	OUTPUT_DEBUG_STRING(_T("server 接收到玩家 其他消息"));
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
	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("此房间上庄条件为：%I64d,区域限制为：%I64d,玩家限制为：%I64d"),m_lApplyBankerCondition,
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
		if (wChairID==m_wCurrentBanker) ChangeBanker(true);

		//取消申请
		for (WORD i=0; i<m_wApplyUserArray.GetCount(); ++i)
		{
			if (wChairID!=m_wApplyUserArray[i]) continue;

			//删除玩家
			m_wApplyUserArray.RemoveAt(i);

			//构造变量
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//设置变量
			lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));
			CancelBanker.dwUserID = pIServerUserItem->GetUserID();

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
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, __int64 lJettonScore)
{
	//效验参数
	ASSERT((cbJettonArea<COUNT_AZIMUTH)&&(lJettonScore>0L));
	if ((cbJettonArea>=COUNT_AZIMUTH)||(lJettonScore<=0L))
	{
		return false;
	}

	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus()==GS_PLACE_JETTON);
	if (m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON)
	{
		goto PlaceJettonFail;
	}

	//庄家判断
	if (m_wCurrentBanker==wChairID) return true;
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR)
	{
		goto PlaceJettonFail;
	}

	//变量定义
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	if (NULL == pIServerUserItem)
	{
		goto PlaceJettonFail;
	}

	int iArea = cbJettonArea;
	__int64 iMaxUserScore = GetMaxPlaceJetton(iArea, wChairID);
	if (iMaxUserScore < lJettonScore)
	{
		goto PlaceJettonFail;
	}
	//变量定义
	CMD_S_PlaceJetton PlaceJetton;
	ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

	//构造变量
	PlaceJetton.wChairID=wChairID;
	PlaceJetton.cbJettonArea=cbJettonArea;
	PlaceJetton.lJettonScore=lJettonScore;
	m_lUserScore[cbJettonArea][wChairID] += lJettonScore;
	m_lAllScore[cbJettonArea] += lJettonScore;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
	return true;

PlaceJettonFail:

	CMD_S_PlaceJettonFail PlaceJettonFail;
	ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
	PlaceJettonFail.lJettonArea=cbJettonArea;
	PlaceJettonFail.lPlaceScore=lJettonScore;

	//发送消息
	m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));

	return true;
}

//申请庄家
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	//合法判断
	__int64 lUserScore=pIApplyServerUserItem->GetUserScore()->lScore;
	if (lUserScore<m_lApplyBankerCondition)
	{
		TCHAR szMsg[_MAX_PATH]={0};
		_sntprintf(szMsg, sizeof(szMsg), TEXT("你的%s不足以申请庄家，申请失败！"), MONEY_NAME);
		m_pITableFrame->SendGameMessage(pIApplyServerUserItem,szMsg,SMT_INFO|SMT_EJECT);
		return true;
	}

	//存在判断
	WORD wApplyUserChairID=pIApplyServerUserItem->GetChairID();
	for (INT_PTR nUserIdx=0; nUserIdx<m_wApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_wApplyUserArray[nUserIdx];
		if (wChairID==wApplyUserChairID)
		{
			m_pITableFrame->SendGameMessage(pIApplyServerUserItem,
				TEXT("你已经申请了庄家，不需要再次申请！"),SMT_INFO|SMT_EJECT);
			return true;
		}
	}

	//保存信息 
	m_wApplyUserArray.Add(wApplyUserChairID);

	//构造变量
	CMD_S_ApplyBanker ApplyBanker;
	ZeroMemory(&ApplyBanker,sizeof(ApplyBanker));

	//设置变量
	ApplyBanker.wApplyUser=wApplyUserChairID;
	ApplyBanker.dwUserID = pIApplyServerUserItem->GetUserID();

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

	//切换判断
	if (m_pITableFrame->GetGameStatus()==GS_FREE && m_wApplyUserArray.GetCount()==1)
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
		m_pITableFrame->SendGameMessage(pICancelServerUserItem,
			TEXT("游戏已经开始，不可以取消当庄！"),SMT_INFO|SMT_EJECT);
		return true;
	}

	//存在判断
	for (WORD i=0; i<m_wApplyUserArray.GetCount(); ++i)
	{
		//获取玩家
		WORD wChairID=m_wApplyUserArray[i];
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);

		//条件过滤
		if (pIServerUserItem==NULL) continue;
		if (pIServerUserItem->GetUserID()!=pICancelServerUserItem->GetUserID()) continue;

		//删除玩家
		m_wApplyUserArray.RemoveAt(i);

		if (m_wCurrentBanker!=wChairID)
		{
			//构造变量
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//设置变量
			lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));
			CancelBanker.dwUserID = pIServerUserItem->GetUserID();

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
	m_iMaxAndroidBanker = 1+ rand()%MAX_ANDROID_BANKER;
	
	//做庄次数
	WORD wBankerTime=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Time"), 10, m_szConfigFileName);

	//取消当前
	if (bCancelCurrentBanker)
	{
		for (WORD i=0; i<m_wApplyUserArray.GetCount(); ++i)
		{
			//获取玩家
			WORD wChairID=m_wApplyUserArray[i];

			//条件过滤
			if (wChairID!=m_wCurrentBanker) continue;

			//删除玩家
			m_wApplyUserArray.RemoveAt(i);

			break;
		}

		//设置庄家
		m_wCurrentBanker=INVALID_CHAIR;

		//轮换判断
		if (m_pITableFrame->GetGameStatus()==GS_FREE && m_wApplyUserArray.GetCount()!=0)
		{
			m_wCurrentBanker=m_wApplyUserArray[0];
		}

		//设置变量
		bChangeBanker=true;
	}
	//轮庄判断
	else if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//获取庄家
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		__int64 lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//次数判断
		if (wBankerTime<=m_wBankerTime || lBankerScore<m_lApplyBankerCondition)
		{
			//撤销玩家
			for (WORD i=0; i<m_wApplyUserArray.GetCount(); ++i)
			{
				//获取玩家
				WORD wChairID=m_wApplyUserArray[i];

				//条件过滤
				if (wChairID!=m_wCurrentBanker) continue;

				//删除玩家
				m_wApplyUserArray.RemoveAt(i);

				break;
			}

			//设置庄家
			m_wCurrentBanker=INVALID_CHAIR;

			//轮换判断
			if (m_wApplyUserArray.GetCount()!=0)
			{
				m_wCurrentBanker=m_wApplyUserArray[0];
			}

			bChangeBanker=true;

			//提示消息
			TCHAR szTipMsg[128];
			if (lBankerScore<m_lApplyBankerCondition)
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]分数少于(%I64d)，强行换庄!"),pIServerUserItem->GetAccounts(),m_lApplyBankerCondition);
			else
            	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]做庄次数达到(%d)，强行换庄!"),pIServerUserItem->GetAccounts(),wBankerTime);
			
			//发送消息
			SendGameMessage(INVALID_CHAIR,szTipMsg);	
		}
	}
	//系统做庄
	else if (m_wCurrentBanker==INVALID_CHAIR && m_wApplyUserArray.GetCount()!=0)
	{
		m_wCurrentBanker=m_wApplyUserArray[0];
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
			ChangeBanker.dwUserID = pIServerUserItem->GetUserID();
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
				m_iAndroidNTCount = -1;
			}
		}
	}

	return bChangeBanker;
}



//发送庄家
void CTableFrameSink::SendApplyUser( IServerUserItem *pRcvServerUserItem )
{
	for (INT_PTR nUserIdx=0; nUserIdx<m_wApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_wApplyUserArray[nUserIdx];

		//获取玩家
		IServerUserItem *pServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (!pServerUserItem) continue;

		//庄家判断
		if (pServerUserItem->GetChairID()==m_wCurrentBanker) continue;

		//构造变量
		CMD_S_ApplyBanker ApplyBanker;
		ApplyBanker.wApplyUser=wChairID;
		ApplyBanker.dwUserID = pServerUserItem->GetUserID();

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
	for (WORD i=0; i<m_wApplyUserArray.GetCount(); ++i)
	{
		if (wChairID!=m_wApplyUserArray[i]) continue;

		//删除玩家
		m_wApplyUserArray.RemoveAt(i);

		//构造变量
		CMD_S_CancelBanker CancelBanker;
		ZeroMemory(&CancelBanker,sizeof(CancelBanker));

		//设置变量
		lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));
		CancelBanker.dwUserID = pIServerUserItem->GetUserID();

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

	return true;
}

//计算得分
__int64 CTableFrameSink::CalculateScore(void)
{
	//变量定义
	__int64 cbRevenue=m_pGameServiceOption->wRevenue;
	
	//推断玩家
	ZeroMemory(&m_bWinner, sizeof(m_bWinner));
	DeduceWinner(m_bWinner);

	// 游戏记录
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	memcpy(&GameRecord.enCards, m_enCards, sizeof(m_enCards));
	//移动下标
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst )
	{
		m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
	}

	//庄家总量
	__int64 lBankerWinScore = 0;

	//玩家成绩
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	__int64 lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	for (int j=0; j<COUNT_AZIMUTH; ++j)
	{
		lBankerWinScore += m_lAllScore[j];
	}

	//计算积分
	for (WORD i=0; i<GAME_PLAYER; ++i)
	{
		//庄家判断
		if (m_wCurrentBanker==i) continue;

		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem==NULL) continue;

		int j=0;
		for (int j=0; j<COUNT_AZIMUTH; ++j )
		{
			lUserLostScore[i] -= _GFIX(m_lUserScore[j][i]);
			// 该区域是否赢了
			if (m_bWinner[j])
			{
				__int64 i64WinScore = 0;
				__int64 i64TmpScore = 0;
				if ((j<enCardType_SicboOne) ||(j>enCardType_SicboSix))
				{
					if(m_lUserScore[j][i]>0)
						i64WinScore = (m_lUserScore[j][i] * CGameLogic::m_i64Loss_Percent[j]);
					else
						i64TmpScore = (-m_lUserScore[j][i]) * CGameLogic::m_i64Loss_Percent[j];
				}
				else
				{
					// 需要换算个数
					E_SICBO_NUMBER enSicboNum = (E_SICBO_NUMBER)(j-enCardType_SicboOne+1);
					int iCountNum = m_GameLogic.GetSicboCountByNumber(m_enCards, enSicboNum);
					int iMuti[]={0,2,3,4};
					if(m_lUserScore[j][i]>0)
						i64WinScore = (m_lUserScore[j][i] * iMuti[iCountNum]);
					else
						i64TmpScore = (-m_lUserScore[j][i]) * iMuti[iCountNum];
				}
				m_lUserWinScore[i] += i64WinScore;
				m_lUserReturnScore[i] += (m_lUserScore[j][i]>0)?m_lUserScore[j][i]:0;
				lBankerWinScore -= i64WinScore-i64TmpScore;
			}
			//else
			//{
			//	__int64 i64LoseScore = m_lUserScore[j][i];
			//	lUserLostScore[i] -= i64LoseScore;
				//lBankerWinScore  += i64LoseScore;
			//}
		}

		//计算税收
		if (0 < m_lUserWinScore[i])
		{
			m_lUserRevenue[i]  = (m_lUserWinScore[i]*cbRevenue)/1000L;
			m_lUserWinScore[i] -= m_lUserRevenue[i];
		}

		//总的分数
		m_lUserWinScore[i] += lUserLostScore[i];

		if (!m_bIsAllAndroid && (INVALID_CHAIR != m_wCurrentBanker)
			&& pIServerUserItem->IsAndroidUser())
		{
			m_lAllAndroidWinScore += m_lUserWinScore[i];
		}

		OUTPUT_DEBUG_STRING(_T("Server 所有玩家下注 %I64d,  当前玩家%d下注 %I64d 输赢%I64d"),
			lBankerWinScore, i, -lUserLostScore[i], m_lUserWinScore[i]);
	}

	//庄家成绩
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;

		//计算税收
		if (0 < m_lUserWinScore[m_wCurrentBanker])
		{
			m_lUserRevenue[m_wCurrentBanker]  = (m_lUserWinScore[m_wCurrentBanker]*cbRevenue)/1000L;
			m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
			lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
		}
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		if (NULL != pIServerUserItem)
		{
			if (!m_bIsAllAndroid && pIServerUserItem->IsAndroidUser())
			{
				m_lAllAndroidWinScore += m_lUserWinScore[m_wCurrentBanker];
			}
		}
	}

	//累计积分
	m_lBankerWinScore += lBankerWinScore;

	// 当前积分
	m_lBankerCurGameScore = lBankerWinScore;

	return lBankerWinScore;
}

void CTableFrameSink::DeduceWinner(bool bWinner[])
{
	// 初始化牌型
	E_CARD_TYPE enCardResult[COUNT_AZIMUTH];
	int i=0;
	for (i=0; i<COUNT_AZIMUTH; ++i)
	{
		enCardResult[i]=enCardType_Illegal;
	}

	int iTypeCount = 0; // 牌型数量
	m_GameLogic.GetCardType(m_enCards, enCardResult, iTypeCount);
	for (i=0; i<iTypeCount; ++i)
	{
		if (enCardType_Illegal != enCardResult[i])
		{
			bWinner[enCardResult[i]]=true;
		}
	}
}

// 控制输赢
void CTableFrameSink::CtrlWinner(void)
{
	// 查看是否需要控制
	E_CARD_TYPE  enCardType[COUNT_AZIMUTH];
	int iCardTypeCount = 0;
	INT i=0;
	for (i=0; i<COUNT_AZIMUTH; ++i)
	{
		enCardType[i] = enCardType_Illegal;
	}
	do 
	{
		m_enCards[0]=(E_SICBO_NUMBER)(rand()%6+1);
		m_enCards[1]=(E_SICBO_NUMBER)(rand()%6+1);
		m_enCards[2]=(E_SICBO_NUMBER)(rand()%6+1);		
		int iCountNum = m_enCards[0] + m_enCards[1] + m_enCards[2];
		if (((m_enCards[0] == m_enCards[1]) && (m_enCards[1]==m_enCards[2]))  // 三个相同
			|| (enCardType_NumberFour==iCountNum) || (enCardType_NumberSeventeen==iCountNum))
		{
			++m_iSameCount;
			if (m_iSameCount < 30)
			{
				Sleep(10);
				continue;
			}
			m_iSameCount = 0;
		}
	} while (false);

	if (enCardType_Illegal == m_iUseWinner)
	{
		// 控制庄家人
		//CtrlNTWinner();

		int iRate=rand()%1000;
		// 机器人输的太多
		if (m_lAllAndroidWinScore<0 && iRate<=m_iAILostLimitPercent)
		{
			// 机器人一定要赢钱
			CtrlAndroidWinner();
		}
		return ;
	}

	m_GameLogic.GetCardType(m_enCards, enCardType, iCardTypeCount);
	for (i=0; i<iCardTypeCount; ++i)
	{
		if (enCardType[i] == m_iUseWinner)
		{
			return ;
		}
	}
	int iCountRand = 0;
	switch (m_iUseWinner)
	{
	case enCardType_Big:
	case enCardType_Small:
	case enCardType_Single:
	case enCardType_Double:
		{
			do 
			{
				m_enCards[0]=(E_SICBO_NUMBER)(rand()%6+1);
				m_enCards[1]=(E_SICBO_NUMBER)(rand()%6+1);
				m_enCards[2]=(E_SICBO_NUMBER)(rand()%6+1);
				m_GameLogic.GetCardType(m_enCards, enCardType, iCardTypeCount);
				for (i=0; i<iCardTypeCount; ++i)
				{
					if (enCardType[i] == m_iUseWinner)
					{
						return ;
					}
				}
				++iCountRand;
				if (iCountRand < 500)  // 500次都没随机到
				{
					continue;
				}
			} while (false);
			break;
		}
	case enCardType_NumberFour:
		{
			// 1 2 1 随机
			m_enCards[0]= enSicboNum_One;
			m_enCards[1]= enSicboNum_Two;
			m_enCards[2]= enSicboNum_One;			
			break;
		}
	case enCardType_NumberSeventeen:
		{
			m_enCards[0]= enSicboNum_Six;
			m_enCards[1]= enSicboNum_Five;
			m_enCards[2]= enSicboNum_Six;
			break;
		}
	case enCardType_SicboOne:
		{
			m_enCards[0]= enSicboNum_One;
			m_enCards[1]=(E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[2]=(E_SICBO_NUMBER)(rand()%6+1);
			break;
		}
	case enCardType_SicboTwo:
		{
			m_enCards[0]= enSicboNum_Two;
			m_enCards[1]=(E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[2]=(E_SICBO_NUMBER)(rand()%6+1);
			break;
		}
	case enCardType_SicboThree:
		{
			m_enCards[0]= (E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[1]=enSicboNum_Three;
			m_enCards[2]=(E_SICBO_NUMBER)(rand()%6+1);
			break;
		}
	case enCardType_SicboFour:
		{
			m_enCards[0]= (E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[1]=enSicboNum_Four;
			m_enCards[2]=(E_SICBO_NUMBER)(rand()%6+1);
			break;
		}
	case enCardType_SicboFive:
		{
			m_enCards[0]= (E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[1]=(E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[2]=enSicboNum_Five;
			break;
		}
	case enCardType_SicboSix:
		{
			m_enCards[0]= (E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[1]=(E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[2]=enSicboNum_Six;
			break;
		}
	case enCardType_SicboOneAndTwo:  // 骰点1,2
		{
			m_enCards[0]= enSicboNum_One;
			m_enCards[1]=(E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[2]= enSicboNum_Two;
			break;
		}
	case enCardType_SicboOneAndThree:       // 骰点1,3
		{
			m_enCards[0]= enSicboNum_One;
			m_enCards[1]=(E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[2]= enSicboNum_Three;
			break;
		}
	case enCardType_SicboOneAndFour:       // 骰点1,4
		{
			m_enCards[0]= enSicboNum_One;
			m_enCards[1]=(E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[2]= enSicboNum_Four;
			break;
		}
	case enCardType_SicboOneAndFive:        // 骰点1,5
		{
			m_enCards[0]= enSicboNum_One;
			m_enCards[1]=enSicboNum_Five;
			m_enCards[2]= (E_SICBO_NUMBER)(rand()%6+1);
			break;
		}
	case enCardType_SicboOneAndSix:         // 骰点1,6{
		{
			m_enCards[0]= enSicboNum_One;
			m_enCards[1]= enSicboNum_Six;
			m_enCards[2]= (E_SICBO_NUMBER)(rand()%6+1);
			break;
		}
	case enCardType_SicboTwoAndThree:       // 骰点2,3
		{
			m_enCards[0]= enSicboNum_Two;
			m_enCards[1]= enSicboNum_Three;
			m_enCards[2]= (E_SICBO_NUMBER)(rand()%6+1);
			break;
		}
	case enCardType_SicboTwoAndFour:        // 骰点2,4
		{
			m_enCards[0]= enSicboNum_Two;
			m_enCards[1]= enSicboNum_Four;
			m_enCards[2]= (E_SICBO_NUMBER)(rand()%6+1);
			break;
		}
	case enCardType_SicboTwoAndFive:        // 骰点2,5
		{
			m_enCards[0]= (E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[1]= enSicboNum_Two;
			m_enCards[2]= enSicboNum_Five;
			break;
		}
	case enCardType_SicboTwoAndSix:         // 骰点2,6
		{
			m_enCards[0]= (E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[1]= enSicboNum_Two;
			m_enCards[2]= enSicboNum_Six;
			break;
		}
	case enCardType_SicboThreeAndFour:      // 骰点3,4
		{
			m_enCards[0]= (E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[1]= enSicboNum_Three;
			m_enCards[2]= enSicboNum_Four;
			break;
		}
	case enCardType_SicboThreeAndFive:      // 骰点3,5
		{
			m_enCards[0]= (E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[1]= enSicboNum_Three;
			m_enCards[2]= enSicboNum_Five;
			break;
		}
	case enCardType_SicboThreeAndSix:       // 骰点3,6
		{
			m_enCards[0]= (E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[1]= enSicboNum_Three;
			m_enCards[2]= enSicboNum_Six;
			break;
		}
	case enCardType_SicboFourAndFive:       // 骰点4,5
		{
			m_enCards[0]= enSicboNum_Five;
			m_enCards[1]= (E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[2]= enSicboNum_Four;
			break;
		}
	case enCardType_SicboFourAndSix:        // 骰点4,6
		{
			m_enCards[0]= enSicboNum_Six;
			m_enCards[1]= (E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[2]= enSicboNum_Four;
			break;
		}
	case enCardType_SicboFiveAndSix:        // 骰点5,6
		{
			m_enCards[0]= enSicboNum_Five;
			m_enCards[1]= (E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[2]= enSicboNum_Six;
			break;
		}
	case enCardType_SicboDoubleOne:
		{
			m_enCards[0]= enSicboNum_One;
			m_enCards[1]= (E_SICBO_NUMBER)(rand()%5+1);
			if (m_enCards[0]==m_enCards[1])
			{
				m_enCards[1] = enSicboNum_Six;
			}
			m_enCards[2]= enSicboNum_One;
			break;
		}
	case enCardType_SicboDoubleTwo:
		{
			m_enCards[0]= enSicboNum_Two;
			m_enCards[1]= (E_SICBO_NUMBER)(rand()%5+1);
			m_enCards[2]= enSicboNum_Two;
			if (m_enCards[0]==m_enCards[1])
			{
				m_enCards[1] = enSicboNum_Six;
			}
			break;
		}
	case enCardType_SicboDoubleThree:
		{
			m_enCards[0]= enSicboNum_Three;
			m_enCards[1]= enSicboNum_Three;
			m_enCards[2]= (E_SICBO_NUMBER)(rand()%5+1);
			if (m_enCards[0]==m_enCards[2])
			{
				m_enCards[2] = enSicboNum_Six;
			}
			break;
		}
	case enCardType_SicboDoubleFour:
		{
			m_enCards[0]= enSicboNum_Four;
			m_enCards[1]= enSicboNum_Four;
			m_enCards[2]= (E_SICBO_NUMBER)(rand()%5+1);
			if (m_enCards[0]==m_enCards[2])
			{
				m_enCards[2] = enSicboNum_Six;
			}
			break;
		}
	case enCardType_SicboDoubleFive:
		{
			m_enCards[0]= (E_SICBO_NUMBER)(rand()%5+1);
			m_enCards[1]= enSicboNum_Five;
			m_enCards[2]= enSicboNum_Five;
			if (m_enCards[0]==m_enCards[1])
			{
				m_enCards[0] = enSicboNum_Six;
			}
			break;
		}
	case enCardType_SicboDoubleSix:
		{
			m_enCards[0]= (E_SICBO_NUMBER)(rand()%5+1);
			m_enCards[1]= enSicboNum_Six;
			m_enCards[2]= enSicboNum_Six;
			break;
		}
	case enCardType_SicboThreeOne:
		{
			m_enCards[0]=m_enCards[1]=m_enCards[2]=enSicboNum_One;
			break;
		}
	case enCardType_SicboThreeTwo:
		{
			m_enCards[0]=m_enCards[1]=m_enCards[2]=enSicboNum_Two;
			break;
		}
	case enCardType_SicboThreeThree:
		{
			m_enCards[0]=m_enCards[1]=m_enCards[2]=enSicboNum_Three;
			break;
		}
	case enCardType_SicboThreeFour:
		{
			m_enCards[0]=m_enCards[1]=m_enCards[2]=enSicboNum_Four;
			break;
		}
	case enCardType_SicboThreeFive:
		{
			m_enCards[0]=m_enCards[1]=m_enCards[2]=enSicboNum_Five;
			break;
		}
	case enCardType_SicboThreeSix:
		{
			m_enCards[0]=m_enCards[1]=m_enCards[2]=enSicboNum_Six;
			break;
		}
	default: break;
	}
	iCountRand = 3;
	while (iCountRand>0)
	{
		int iIndex = rand()%iCountRand;
		if (iIndex != (iCountRand-1))
		{
			E_SICBO_NUMBER enTemp = m_enCards[iIndex];
			m_enCards[iIndex] = m_enCards[iCountRand-1];
			m_enCards[iCountRand-1] = enTemp;
		}
		--iCountRand;
	}
}

// 控制系统坐庄输赢
void CTableFrameSink::CtrlNTWinner(void)
{
	// 目前只控制系统坐庄时的输赢情况
	if (INVALID_CHAIR != m_wCurrentBanker)
	{
		return ;
	}

	// 85%的机会赢
	if ((0 == m_cbWinSideControl) || (rand()%100 > m_iNTWinPercent))
	{
		return ;
	}

	// 都没下注就不用控制胜率了
	bool bAllZero = true;
	for (int i=0; i<COUNT_AZIMUTH; ++i)
	{
		if (m_lAllScore[i] > 0)
		{
			bAllZero = false;
			break;
		}
	}
	if (bAllZero)
	{
		return ;
	}

	//BYTE byLoseMin = ID_BANKER;
	//__int64 lMinLoseScore=0;

	//// 输赢结果
	//bool bWinner[COUNT_AZIMUTH];
	//for (int j=ID_BANKER; j>=ID_EAST; --j)
	//{
	//	BYTE byMaxCard = 0;
	//	BYTE byCount = MAX_COUNT_SICBO;
	//	E_CARD_TYPE enCardType[(COUNT_AZIMUTH+1)];
	//	enCardType[ID_BANKER] = enCardType_SicboThreeAndFive;//m_GameLogic.GetCardType(m_byCards[j], byCount, byMaxCard);
	//	for (BYTE i=ID_EAST; i<ID_BANKER; ++i)
	//	{
	//		BYTE byIndex = (i == j)?ID_BANKER:i;  // 如果是庄家，则应该用庄家的牌的替换
	//		E_COMPARE_RESULT enCompRet = enCompResult_Less;//m_GameLogic.CompareCards(m_byCards[j], MAX_COUNT_SICBO, m_byCards[byIndex], MAX_COUNT_SICBO);
	//		bWinner[i] = (enCompRet > enCompResult_Less)?false:true;
	//		enCardType[i] = enCardType_SicboThreeAndFive;//m_GameLogic.GetCardType(m_byCards[byIndex], byCount, byMaxCard);
	//	}

	//	// 查看输赢结果
	//	__int64 lBankerWinScore = 0;

	//	//牌型的倍率
	//	BYTE cbMultiple[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

	//	//计算积分
	//	for (WORD i=0; i<GAME_PLAYER; ++i)
	//	{
	//		//庄家判断
	//		if (m_wCurrentBanker==i) continue;

	//		//获取用户
	//		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
	//		if (pIServerUserItem==NULL) continue;

	//		for ( WORD wAreaIndex = ID_EAST; wAreaIndex <= ID_NORTH; ++wAreaIndex )
	//		{
	//			// 该区域是否赢了，东南西北
	//			if (bWinner[wAreaIndex])
	//			{
	//				__int64 i64WinScore = (m_lUserScore[wAreaIndex][i] * cbMultiple[enCardType[wAreaIndex]]);
	//				lBankerWinScore -= i64WinScore;
	//			}
	//			else
	//			{
	//				__int64 i64LoseScore = (m_lUserScore[wAreaIndex][i] * cbMultiple[enCardType[ID_BANKER]]);
	//				lBankerWinScore  += i64LoseScore;
	//			}
	//		}
	//	}

	//	if ((ID_BANKER == j) && (lBankerWinScore > 0))
	//	{
	//		return ;
	//	}

	//	if (lBankerWinScore > 0)
	//	{
	//		// 与庄家的牌调换
	//		BYTE byCardsTemp[MAX_COUNT_SICBO];
	//		memcpy(byCardsTemp, m_byCards[ID_BANKER], MAX_COUNT_SICBO);
	//		memcpy(m_byCards[ID_BANKER],m_byCards[j], MAX_COUNT_SICBO);
	//		memcpy(m_byCards[j],byCardsTemp, MAX_COUNT_SICBO);
	//		return ;
	//	}
	//	if (ID_BANKER == j)
	//	{
	//		lMinLoseScore = lBankerWinScore;
	//	}
	//	else if (lMinLoseScore < lBankerWinScore)
	//	{
	//		lMinLoseScore = lBankerWinScore;
	//		byLoseMin = j;
	//	}
	//}

	//if (ID_BANKER != byLoseMin)
	//{
	//	// 与庄家的牌调换
	//	BYTE byCardsTemp[MAX_COUNT_SICBO];
	//	memcpy(byCardsTemp, m_byCards[ID_BANKER], MAX_COUNT_SICBO);
	//	memcpy(m_byCards[ID_BANKER],m_byCards[byLoseMin], MAX_COUNT_SICBO);
	//	memcpy(m_byCards[byLoseMin],byCardsTemp, MAX_COUNT_SICBO);
	//}
}

// 控制机器人要赢
void CTableFrameSink::CtrlAndroidWinner(void)
{
	if (INVALID_CHAIR == m_wCurrentBanker)
	{
		return ;
	}

	// 所有的人是机器人不用控制
	if (m_bIsAllAndroid)
	{
		return;
	}

	// 没有人坐庄
	IServerUserItem * pIBankerItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
	if (NULL == pIBankerItem)
	{
		return ;
	}

	// 都没下注就不用控制胜率了
	bool bAllZero = true;
	for (int i=0; i<COUNT_AZIMUTH; ++i)
	{
		if (m_lAllScore[i] > 0)
		{
			bAllZero = false;
			break;
		}
	}
	if (bAllZero)
	{
		return ;
	}

	//变量定义
	__int64 cbRevenue=m_pGameServiceOption->wRevenue;

	//推断玩家
	bool bWinnerTemp[COUNT_AZIMUTH];
	ZeroMemory(&bWinnerTemp, sizeof(bWinnerTemp));
	DeduceWinner(bWinnerTemp);

	//庄家总量
	__int64 lBankerWinScore = 0;

	//玩家成绩
	__int64 lAroidWinerScore=0;
	__int64 lUserWinerScore[GAME_PLAYER];
	__int64 lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserWinerScore, sizeof(lUserWinerScore));
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	for (int j=0; j<COUNT_AZIMUTH; ++j)
	{
		lBankerWinScore += m_lAllScore[j];
	}	

	//计算积分
	for (WORD i=0; i<GAME_PLAYER; ++i)
	{
		//庄家判断
		if (m_wCurrentBanker==i) continue;

		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem==NULL) continue;

		int j=0;
		for (int j=0; j<COUNT_AZIMUTH; ++j )
		{
			lUserLostScore[i] -= m_lUserScore[j][i];
			// 该区域是否赢了
			if (bWinnerTemp[j])
			{
				__int64 i64WinScore = 0;
				if ((j<enCardType_SicboOne) ||(j>enCardType_SicboSix))
				{
					i64WinScore = (m_lUserScore[j][i] * CGameLogic::m_i64Loss_Percent[j]);
				}
				else
				{
					// 需要换算个数
					E_SICBO_NUMBER enSicboNum = (E_SICBO_NUMBER)(j-enCardType_SicboOne+1);
					int iCountNum = m_GameLogic.GetSicboCountByNumber(m_enCards, enSicboNum);
					int iMuti[]={0,2,3,4};
					i64WinScore = (m_lUserScore[j][i] * iMuti[iCountNum]);
				}
				lUserWinerScore[i] += i64WinScore;
				lBankerWinScore -= i64WinScore;
			}
		}

		// 计算税收
		if (0 < lUserWinerScore[i])
		{
			lUserWinerScore[i] -= ((lUserWinerScore[i]*cbRevenue)/1000L);
		}

		//总的分数
		lUserWinerScore[i] += lUserLostScore[i];

		if (!m_bIsAllAndroid && (INVALID_CHAIR != m_wCurrentBanker)
			&& pIServerUserItem->IsAndroidUser())
		{
			lAroidWinerScore += lUserWinerScore[i];
		}
	}

	//庄家成绩
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{

		//计算税收
		if (0 < lBankerWinScore)
		{
			lBankerWinScore -= ((lBankerWinScore*cbRevenue)/1000L);
		}
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		if (NULL != pIServerUserItem)
		{
			if (!m_bIsAllAndroid && pIServerUserItem->IsAndroidUser())
			{
				lAroidWinerScore += lBankerWinScore;
			}
		}
	}

	if ((lAroidWinerScore>0) || (m_iAndroidRandCount>100))
	{
		return ;
	}
	++m_iAndroidRandCount;
	m_enCards[0]=(E_SICBO_NUMBER)(rand()%6+1);
	m_enCards[1]=(E_SICBO_NUMBER)(rand()%6+1);
	m_enCards[2]=(E_SICBO_NUMBER)(rand()%6+1);
	CtrlAndroidWinner();
	return ;
}

//发送记录
void CTableFrameSink::SendGameRecord(IServerUserItem *pIServerUserItem)
{
	WORD wBufferSize=0;
	BYTE cbBuffer[SOCKET_BUFFER];
	int nIndex = m_nRecordFirst;
	int nLast = (m_pITableFrame->GetGameStatus()==GS_GAME_END)?((m_nRecordLast+MAX_SCORE_HISTORY-1)%MAX_SCORE_HISTORY):m_nRecordLast;
	while ( nIndex != nLast )
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

// 获取每个区域可以下注数
__int64 CTableFrameSink::GetMaxPlaceJetton(int iArea, WORD wChairID)
{
	if ((iArea < 0) || (iArea>=COUNT_AZIMUTH))
	{
		return 0L;
	}

	CSingleLock SingleLockHandle(&m_CriticalSection);
	SingleLockHandle.Lock();
	__int64 i64UserAllScore = 0;    // 玩家自己已经下注的分
	__int64 i64AllUserAllScore=0;   // 所有玩家下注数 
	__int64 i64AllLossScore=0;      // 庄家总赔数
	__int64 i64LoseAreaScore=0;     // 庄家输的区域总的下注数
	for (int i=0; i<COUNT_AZIMUTH; ++i)
	{
		i64UserAllScore += m_lUserScore[i][wChairID];
		i64AllUserAllScore += m_lAllScore[i];
	}

	for (int i=1; i<=MAX_SICBO_NUMBER; ++i)
	{
		for (int j=i; j<=MAX_SICBO_NUMBER; ++j)
		{
			for (int h=j; h<=MAX_SICBO_NUMBER; ++h)
			{
				E_SICBO_NUMBER enBuffer[]={(E_SICBO_NUMBER)i,(E_SICBO_NUMBER)j,(E_SICBO_NUMBER)h};
				E_CARD_TYPE enCardType[COUNT_AZIMUTH];
				int iCount = 0;
				m_GameLogic.GetCardType(enBuffer,enCardType, iCount);
				__int64 i64AllLossScoreTemp=0;
				__int64 i64LoseAreaScoreTemp=0;
				bool bFind = false;
				for (int l=0; l<iCount; ++l)
				{
					if (enCardType_Illegal == enBuffer[l])
					{
						continue;
					}
					if (iArea == enCardType[l])
					{
						bFind = true;
					}
				}
				if (!bFind)
				{
					continue;
				}

				// 算出这手牌输多少钱
				for (int l=0; l<iCount; ++l)
				{
					if (enCardType_Illegal == enCardType[l])
					{
						continue;
					}
					i64LoseAreaScoreTemp += m_lAllScore[enCardType[l]];   // 庄家输的区域，需要把金币扣除
					if ((enCardType[l]<enCardType_SicboOne) ||(enCardType[l]>enCardType_SicboSix))
					{
						i64AllLossScoreTemp += (m_lAllScore[enCardType[l]] * m_GameLogic.m_i64Loss_Percent[enCardType[l]]);
					}
					else
					{
						// 需要换算个数
						E_SICBO_NUMBER enSicboNum = (E_SICBO_NUMBER)(enCardType[l]-enCardType_SicboOne+1);
						int iCountNum = m_GameLogic.GetSicboCountByNumber(enBuffer, enSicboNum);
						int iMuti[]={0,2,3,4};
						i64AllLossScoreTemp += (m_lAllScore[enCardType[l]] * iMuti[iCountNum]);
					}
				}
				
				if (i64AllLossScoreTemp > i64AllLossScore)
				{
					i64AllLossScore = i64AllLossScoreTemp;
				}

				if (i64LoseAreaScoreTemp>i64LoseAreaScore)
				{
					i64LoseAreaScore = i64LoseAreaScoreTemp;
				}
			}
		}
	}

	// 获取基本上限
	__int64 lAllMaxScore = 0;
	__int64 lUserMaxScore = 0;
	IServerUserItem *pIBankerServerUserItem=NULL;
	if (INVALID_CHAIR!=m_wCurrentBanker)
	{
		pIBankerServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
	}

	if (NULL != pIBankerServerUserItem)
	{
		lAllMaxScore = pIBankerServerUserItem->GetUserScore()->lScore;
	}
	else
	{
		lAllMaxScore = m_lAreaLimitScore;
	}


	IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	if (NULL == pIServerUserItem)
	{
		return 0L;
	}

	// 设置积分
	lUserMaxScore = min(pIServerUserItem->GetUserScore()->lScore,m_lUserLimitScore);
	// 减去已经下注的
	lUserMaxScore -= i64UserAllScore;
	ASSERT(lUserMaxScore>0);
	if (lUserMaxScore<=0)
	{
		return 0L;
	}

	OUTPUT_DEBUG_STRING(_T("Server 最大下注 %I64d, 所有玩家下注 %I64d,  庄家当前输 %I64d  玩家下注区域%d"),
		lAllMaxScore, i64AllUserAllScore, i64AllLossScore, iArea);

	// 庄家可以赔的钱
	lAllMaxScore = lAllMaxScore - i64LoseAreaScore + i64AllUserAllScore;
	lAllMaxScore -= i64AllLossScore;
	if (lAllMaxScore<0)
	{
		return 0L;
	}

	if ((iArea<enCardType_SicboOne) ||(iArea>enCardType_SicboSix))
	{
		lAllMaxScore = lAllMaxScore/(m_GameLogic.m_i64Loss_Percent[iArea] -1);
	}
	else
	{
		lAllMaxScore = lAllMaxScore/3; // 以最大倍数算
	}
	//ASSERT(lAllMaxScore>=0);
	if (lAllMaxScore<0)
	{
		return 0L;
	}
	lUserMaxScore = min(lAllMaxScore, lUserMaxScore);
	return lUserMaxScore;

}
//////////////////////////////////////////////////////////////////////////

