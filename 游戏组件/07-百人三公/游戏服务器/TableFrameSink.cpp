#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//常量定义
#define SEND_COUNT					300									//发送次数

//索引定义
#define INDEX_BANKER				0									//庄家索引
#define INDEX_PLAYER1				1									//天
#define INDEX_PLAYER2				2									//地
#define INDEX_PLAYER3				3									//玄
#define INDEX_PLAYER4				4									//黄


//下注时间
#define IDI_FREE					1									//空闲时间
#ifdef _DEBUG
#define TIME_FREE					4									//空闲时间
#else
//#define TIME_FREE					30//10									//空闲时间
//test
#define TIME_FREE					20									//空闲时间
#endif

//下注时间
#define IDI_PLACE_JETTON			2									//下注时间

#ifdef _DEBUG
#define TIME_PLACE_JETTON			18									//下注时间
#else
#define TIME_PLACE_JETTON			18									//下注时间
#endif

//结束时间
#define IDI_GAME_END				3									//结束时间

#ifdef _DEBUG
#define TIME_GAME_END				12									//结束时间
#else
#define TIME_GAME_END				10									//结束时间
#endif

//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//游戏人数
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_TimeControl;	//开始模式

//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{
	//总下注数
	m_lTianScore=0L;							//天
	m_lDiScore=0L;								//地
	m_lXuanScore=0L;							//玄
	m_lHuangScore=0L;							//黄


	//控制变量
	m_cbWinSideControl=0;
	m_nSendCardCount=0;

	//个人下注
	ZeroMemory(m_lUserTianScore,sizeof(m_lUserTianScore));
	ZeroMemory(m_lUserDiScore,sizeof(m_lUserDiScore));
	ZeroMemory(m_lUserXuanScore,sizeof(m_lUserXuanScore));
	ZeroMemory(m_lUserHuangScore,sizeof(m_lUserHuangScore));


	//玩家成绩	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//扑克信息
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

	//记录变量
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;
	m_nRecordLast=0;
	m_dwRecordCount=0;


	ZeroMemory(m_cbTimers,sizeof(m_cbTimers));

	//设置文件名
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_snprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\BaiRenSanGongConfig.ini"),szPath);

	//控制变量
	LONG lMaxScore=2147483647;
	m_lApplyBankerCondition=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Score"), 100, m_szConfigFileName);
	m_lAreaLimitScore=GetPrivateProfileInt(TEXT("ScoreLimit"), TEXT("AreaLimitScore"), lMaxScore, m_szConfigFileName);
	m_lUserLimitScore=GetPrivateProfileInt(TEXT("ScoreLimit"), TEXT("UserLimitScore"), lMaxScore, m_szConfigFileName);
	BYTE cbEnableSysBanker=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("EnableSysBanker"), 1, m_szConfigFileName);
	m_bEnableSysBanker=cbEnableSysBanker?true:false;

	InitializeCriticalSection(&m_csPlaceJetton);


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
	m_lTianScore=0L;							//天
	m_lDiScore=0L;								//地
	m_lXuanScore=0L;							//玄
	m_lHuangScore=0L;							//黄

	//个人下注
	ZeroMemory(m_lUserTianScore,sizeof(m_lUserTianScore));
	ZeroMemory(m_lUserDiScore,sizeof(m_lUserDiScore));
	ZeroMemory(m_lUserXuanScore,sizeof(m_lUserXuanScore));
	ZeroMemory(m_lUserHuangScore,sizeof(m_lUserHuangScore));

	//玩家成绩	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	ZeroMemory(m_cbTimers,sizeof(m_cbTimers));

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
	if (INVALID_CHAIR!=m_wCurrentBanker) pIBankerServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);

	//设置变量
	GameStart.cbTimeLeave=TIME_PLACE_JETTON;
	GameStart.wBankerUser=m_wCurrentBanker;
	if (pIBankerServerUserItem!=NULL) GameStart.lBankerScore=pIBankerServerUserItem->GetUserScore()->lScore;

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
			if (cbReason==GER_USER_LEFT  && GS_PLACE_JETTON == m_pITableFrame->GetGameStatus())
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

					//设置时间
					m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
					m_pITableFrame->SetGameTimer(IDI_GAME_END,TIME_GAME_END*1000,1,0L);
				}
			}
			//	else
			//	{

			//		//if(GS_PLACE_JETTON == m_pITableFrame->GetGameStatus())
			//		//{

			//		//}

			//		//变量定义
			//		LONG lScore=0;
			//		LONG lRevenue=0;
			//		enScoreKind ScoreKind=enScoreKind_Flee;

			//		//统计成绩
			//		/*lScore=-( m_lUserTieScore[wChairID] + m_lUserBankerScore[wChairID] + m_lUserPlayerScore[wChairID] + 
			//			m_lUserBankerKingScore[wChairID] + m_lUserPlayerKingScore[wChairID] + m_lUserTieSamePointScore[wChairID]+
			//			m_lUserPlayerTwoPair[wChairID]+m_lUserBankerTwoPair[wChairID]);*/
			//		//--统计成绩  这里还没加赔率
			//		lScore=-( m_lUserTianScore[wChairID] + m_lUserDiScore[wChairID] + m_lUserXuanScore[wChairID] + 
			//			m_lUserHuangScore[wChairID]);

			//		//写入积分
			//		if (lScore!=0L) m_pITableFrame->WriteUserScore(pIServerUserItem, lScore,lRevenue, ScoreKind);
			//	}
			//}

			//庄家判断
			//if (cbReason==GER_USER_LEFT && wChairID!=m_wCurrentBanker) return true;
			
			LONG lBankerWinScore=0L;
			if(cbReason!=GER_USER_LEFT || wChairID==m_wCurrentBanker)
			{
				//派发扑克
				DispatchTableCard();

				/*CString str;
				::OutputDebugString("Server Dispach card:");
				for(BYTE i=0; i<6; i++)
				{
					str.Format("User:%d Card:%x, %x, %x", i, m_cbTableCardArray[i][0], m_cbTableCardArray[i][1], m_cbTableCardArray[i][2]);
					::OutputDebugString(str);
				}*/

				//计算分数
				lBankerWinScore=CalculateScore();
				/*::OutputDebugString("最后成绩:");
				str.Format("%d", lBankerWinScore);
				::OutputDebugString(str);*/
			}

			
//////////////////////////

			if(cbReason==GER_USER_LEFT && wChairID!=m_wCurrentBanker)
			{
				//胜利类型
				enScoreKind ScoreKind=(m_lUserWinScore[wChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;
				//写入积分
				if (m_lUserWinScore[wChairID]!=0L) m_pITableFrame->WriteUserScore(wChairID,m_lUserWinScore[wChairID], m_lUserRevenue[wChairID], ScoreKind);
				return true;
			}
///////////////////////////
			//逃跑判断
			//if (cbReason==GER_USER_LEFT && wChairID==m_wCurrentBanker && m_lUserWinScore[m_wCurrentBanker]<0)
			if (cbReason==GER_USER_LEFT && wChairID==m_wCurrentBanker)
			{
				enScoreKind ScoreKind=(m_lUserWinScore[m_wCurrentBanker]>0L)?enScoreKind_Win:enScoreKind_Lost;
				//m_pITableFrame->WriteUserScore(m_wCurrentBanker,m_lUserWinScore[m_wCurrentBanker], m_lUserRevenue[m_wCurrentBanker], enScoreKind_Flee);
				m_pITableFrame->WriteUserScore(m_wCurrentBanker,m_lUserWinScore[m_wCurrentBanker], m_lUserRevenue[m_wCurrentBanker], ScoreKind);
			}

			//递增次数
			m_wBankerTime++;

			//if(cbReason!=GER_USER_LEFT)
			{
				//结束消息
				CMD_S_GameEnd GameEnd;
				ZeroMemory(&GameEnd,sizeof(GameEnd));

				//庄家信息
				GameEnd.nBankerTime = m_wBankerTime;
				GameEnd.lBankerTotallScore=m_lBankerWinScore;
				if (cbReason==GER_USER_LEFT && lBankerWinScore>0) GameEnd.lBankerScore=0;
				else GameEnd.lBankerScore=lBankerWinScore;

				//扑克信息
				CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));

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

					/*CString str;
					::OutputDebugString("Server Game end:");
					for(BYTE i=0; i<6; i++)
					{
						str.Format("User:%d Card:%x, %x, %x", i, GameEnd.cbTableCardArray[i][0], GameEnd.cbTableCardArray[i][1], GameEnd.cbTableCardArray[i][2]);
						::OutputDebugString(str);
					}*/
					/*str.Format("成绩:%d, %d, %d", GameEnd.lUserScore, GameEnd.lBankerScore, GameEnd.lBankerTotallScore);
					::OutputDebugString(str);*/
					//发送消息					
					m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				}

			}

			//切换庄家
			if (cbReason==GER_USER_LEFT && wChairID==m_wCurrentBanker) ChangeBanker(true);

			//记录结果
			//try
			//{
			//	//设置文件名
			//	TCHAR szPath[MAX_PATH]=TEXT("");
			//	TCHAR szFileName[MAX_PATH];
			//	GetCurrentDirectory(sizeof(szPath),szPath);
			//	_snprintf(szFileName,sizeof(szFileName),TEXT("%s\\游戏记录%ld.txt"),szPath,m_dwRecordCount++/66);
			//	if (m_FileRecord.Open(szFileName,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
			//	{
			//		TCHAR *szRecord[4]={TEXT(""),TEXT("1\r\n"),TEXT("2\r\n"),TEXT("0\r\n")};

			//		//推断玩家
			//		BYTE cbWinner,cbKingWinner;
			//		bool bPlayerTwoPair,bBankerTwoPair;
			//		DeduceWinner(cbWinner, cbKingWinner, bPlayerTwoPair,bBankerTwoPair);		
			//		m_FileRecord.SeekToEnd();
			//		m_FileRecord.Write(szRecord[cbWinner],CountStringBuffer(szRecord[cbWinner]));

			//		m_FileRecord.Close();
			//	}
			//}
			//catch(...)
			//{
			//}

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
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄条件为：%ld,区域限制为：%ld,玩家限制为：%ld"),m_lApplyBankerCondition,
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
			//StatusPlay.lAllTieScore=m_lTieScore;		
			//StatusPlay.lAllBankerScore=m_lBankerScore;		
			//StatusPlay.lAllPlayerScore=m_lPlayerScore;		
			//StatusPlay.lAllTieSamePointScore=m_lTieSamePointScore;
			//StatusPlay.lAllBankerKingScore=m_lBankerKingScore;	
			//StatusPlay.lAllPlayerKingScore=m_lPlayerKingScore;	
			//StatusPlay.lAllPlayerTwoPair=m_lPlayerTwoPair;	
			//StatusPlay.lAllBankerTwoPair=m_lBankerTwoPair;
			StatusPlay.lAllTianScore=m_lTianScore;
			StatusPlay.lAllDiScore=m_lDiScore;
			StatusPlay.lAllXuanScore=m_lXuanScore;
			StatusPlay.lAllHuangScore=m_lHuangScore;

			//玩家下注
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				//StatusPlay.lUserTieScore=m_lUserTieScore[wChiarID];			
				//StatusPlay.lUserBankerScore=m_lUserBankerScore[wChiarID];		
				//StatusPlay.lUserPlayerScore=m_lUserPlayerScore[wChiarID];		
				//StatusPlay.lUserTieSamePointScore=m_lUserTieSamePointScore[wChiarID];	
				//StatusPlay.lUserBankerKingScore=m_lUserBankerKingScore[wChiarID];	
				//StatusPlay.lUserPlayerKingScore=m_lUserPlayerKingScore[wChiarID];	
				//StatusPlay.lUserPlayerTwoPair=m_lUserPlayerTwoPair[wChiarID];		
				//StatusPlay.lUserBankerTwoPair=m_lUserBankerTwoPair[wChiarID];
				StatusPlay.lUserTianScore=m_lUserTianScore[wChiarID];
				StatusPlay.lUserDiScore=m_lUserDiScore[wChiarID];
				StatusPlay.lUserXuanScore=m_lUserXuanScore[wChiarID];
				StatusPlay.lUserHuangScore=m_lUserHuangScore[wChiarID];

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
			if (cbGameStatus==GS_GAME_END)
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

				//扑克信息
				CopyMemory(StatusPlay.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
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
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,TIME_PLACE_JETTON*1000,1,0L);

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

				//胜利类型
				enScoreKind ScoreKind=(m_lUserWinScore[wUserChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;


				//写入积分
				if (m_lUserWinScore[wUserChairID]!=0L) m_pITableFrame->WriteUserScore(wUserChairID,m_lUserWinScore[wUserChairID], m_lUserRevenue[wUserChairID], ScoreKind);

				//坐庄判断
				LONG lUserScore=pIServerUserItem->GetUserScore()->lScore;
				if (wUserChairID!=m_wCurrentBanker && lUserScore<m_lApplyBankerCondition)
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
						_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("由于你的银子数少于坐庄必须银子数（%ld）,你自动下庄！"),m_lApplyBankerCondition);
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
	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄条件为：%ld,区域限制为：%ld,玩家限制为：%ld"),m_lApplyBankerCondition,
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

//加注事件
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONG lJettonScore)
{
	//效验参数
	ASSERT((cbJettonArea<=ID_HUANG_MARK)&&(lJettonScore>0L));
	if ((cbJettonArea>ID_HUANG_MARK)||(lJettonScore<=0L)) return false;

	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus()==GS_PLACE_JETTON);
	if (m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON) return true;

	//庄家判断
	if (m_wCurrentBanker==wChairID) return true;
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) return true;

	EnterCriticalSection(&m_csPlaceJetton);
	//变量定义
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	//LONG lJettonCount=m_lUserTieScore[wChairID]+m_lUserBankerScore[wChairID]+m_lUserPlayerScore[wChairID]+
	//	m_lUserTieSamePointScore[wChairID] + m_lUserPlayerKingScore[wChairID] + m_lUserBankerKingScore[wChairID]+
	//	m_lUserPlayerTwoPair[wChairID]+m_lUserBankerTwoPair[wChairID];

	//--是否还可以加注
	LONG lJettonCount=m_lUserTianScore[wChairID]+m_lUserDiScore[wChairID]+m_lUserXuanScore[wChairID]+
		m_lUserHuangScore[wChairID];

	LONG lUserScore = pIServerUserItem->GetUserScore()->lScore;

	//合法校验
	if (lUserScore < lJettonCount *10 + lJettonScore*10) return true;
	if (m_lUserLimitScore < lJettonCount + lJettonScore) return true;

	//成功标识
	bool bPlaceJettonSuccess=true;
//--加注区域

	//合法验证
	if ( ID_TIAN_MARK == cbJettonArea )
	{
		if ( GetMaxTianScore(wChairID) >= lJettonScore )
		{
			//保存下注
			m_lTianScore += lJettonScore;
			m_lUserTianScore[wChairID] += lJettonScore;			
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_DI_MARK == cbJettonArea )
	{
		if ( GetMaxDiScore(wChairID) >= lJettonScore ) 
		{
			//保存下注
			m_lDiScore += lJettonScore;
			m_lUserDiScore[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_XUAN_MARK == cbJettonArea )
	{
		if ( GetMaxXuanScore(wChairID)>=lJettonScore ) 
		{
			//保存下注
			m_lXuanScore += lJettonScore;
			m_lUserXuanScore[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_HUANG_MARK == cbJettonArea )
	{
		if ( GetMaxHuangScore(wChairID)>=lJettonScore )
		{
			//保存下注
			m_lHuangScore += lJettonScore;
			m_lUserHuangScore[wChairID] += lJettonScore;
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

//发送扑克
bool CTableFrameSink::DispatchTableCard()
{
	srand(time(NULL));
	m_GameLogic.RandCardList(m_cbTableCardArray[0],sizeof(m_cbTableCardArray)/sizeof(m_cbTableCardArray[0][0]));
	//首次发牌
	
/*
	//计算点数
	BYTE cbBankerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER],m_cbCardCount[INDEX_BANKER]);
	BYTE cbPlayerTwoCardCount = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER],m_cbCardCount[INDEX_PLAYER]);

	//闲家补牌
	BYTE cbPlayerThirdCardValue = 0 ;	//第三张牌点数
	if(cbPlayerTwoCardCount<=5 && cbBankerCount<8)
	{
		//计算点数
		m_cbCardCount[INDEX_PLAYER]++;
		cbPlayerThirdCardValue = m_GameLogic.GetCardPip(m_cbTableCardArray[INDEX_PLAYER][2]);
	}
*/

	////读取分数
	//m_cbWinSideControl = GetPrivateProfileInt( TEXT("ManagerControl"), TEXT("WinSide"), 0, m_szConfigFileName );

	////控制输赢
	//if ( m_cbWinSideControl != 0 )
	//{
	//	//推断输赢
	//	BYTE cbWinner;
	//	DeduceWinner(cbWinner);

	//	//重发判断
	//	if ( cbWinner!=m_cbWinSideControl && m_nSendCardCount < SEND_COUNT )
	//	{
	//		m_nSendCardCount++;
	//		DispatchTableCard();
	//	}
	//	else
	//	{
	//		m_nSendCardCount=0;
	//	}

	//}

	return true;
}

//申请庄家
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	//合法判断
	LONG lUserScore=pIApplyServerUserItem->GetUserScore()->lScore;
	if (lUserScore<m_lApplyBankerCondition)
	{
		m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("你的银子不足以申请庄家，申请失败！"),SMT_INFO|SMT_EJECT);
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

//更换庄家
bool CTableFrameSink::ChangeBanker(bool bCancelCurrentBanker)
{
	//切换标识
	bool bChangeBanker=false;
	
	//做庄次数
	WORD wBankerTime=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Time"), 10, m_szConfigFileName);

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
		CancelBanker.wCancelUser=wChairID;

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

	return true;
}

//最大下注
LONG CTableFrameSink::GetUserMaxJetton(WORD wChairID)
{
	//已下注额
	LONG lNowJetton = 0;
	//lNowJetton += m_lUserPlayerScore[wChairID];
	//lNowJetton += m_lUserPlayerKingScore[wChairID];
	//lNowJetton += m_lUserBankerScore[wChairID];
	//lNowJetton += m_lUserTieScore[wChairID];
	//lNowJetton += m_lUserTieSamePointScore[wChairID];
	//lNowJetton += m_lUserBankerKingScore[wChairID];
	//lNowJetton += m_lUserPlayerTwoPair[wChairID];
	//lNowJetton += m_lUserBankerTwoPair[wChairID];
	lNowJetton +=m_lUserTianScore[wChairID];
	lNowJetton +=m_lUserDiScore[wChairID];
	lNowJetton +=m_lUserXuanScore[wChairID];
	lNowJetton +=m_lUserHuangScore[wChairID];

	//获取玩家
	IServerUserItem *pIMeServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);

	LONG lMeMaxScore=pIMeServerUserItem->GetUserScore()->lScore-lNowJetton*10;

	lMeMaxScore=min(lMeMaxScore, m_lUserLimitScore);
	//玩家下注
	//LONG lMeMaxScore = min(pIMeServerUserItem->GetUserScore()->lScore, m_lUserLimitScore);
	//lMeMaxScore -= lNowJetton*10;
	//ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return lMeMaxScore;
}

//最大下注
LONG CTableFrameSink::GetMaxTianScore(WORD wChairID)//天
{
	//玩家限制
	LONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//区域限制
	LONG lAreaLimit = m_lAreaLimitScore - m_lTianScore;

	//最大下注
	LONG lMaxJetton=0L;

	//庄家判断
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//其他区域
//		LONG lOtherAreaScore = m_lBankerScore+ m_lTieScore+ m_lTieSamePointScore+ m_lBankerKingScore;

		//庄家积分
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//庄家限制
		LONG lMaxTianScore = lBankerScore;
		lMaxTianScore -=(m_lTianScore*10+m_lDiScore*10+m_lXuanScore*10+m_lHuangScore*10);
		//lMaxTianScore -= (m_lPlayerScore+m_lPlayerKingScore*2+(m_lPlayerTwoPair+m_lBankerTwoPair)*RATE_TWO_PAIR);
		ASSERT(lMaxTianScore>=0);

		//最大下注
		lMaxJetton = min(lMaxTianScore/10, lUserMaxJetton);	
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//最大下注
LONG CTableFrameSink::GetMaxDiScore(WORD wChairID)
{	

	//玩家限制
	LONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//区域限制
	LONG lAreaLimit = m_lAreaLimitScore - m_lDiScore;

	//最大下注
	LONG lMaxJetton=0L;

	//庄家判断
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//其他区域
		//		LONG lOtherAreaScore = m_lBankerScore+ m_lTieScore+ m_lTieSamePointScore+ m_lBankerKingScore;

		//庄家积分
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//庄家限制
		LONG lMaxTianScore = lBankerScore;
		lMaxTianScore -=(m_lTianScore*10+m_lDiScore*10+m_lXuanScore*10+m_lHuangScore*10);
		//lMaxTianScore -= (m_lPlayerScore+m_lPlayerKingScore*2+(m_lPlayerTwoPair+m_lBankerTwoPair)*RATE_TWO_PAIR);
		ASSERT(lMaxTianScore>=0);

		//最大下注
		lMaxJetton = min(lMaxTianScore/10, lUserMaxJetton);	
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//最大下注
LONG CTableFrameSink::GetMaxXuanScore(WORD wChairID)
{
	//玩家限制
	LONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//区域限制
	LONG lAreaLimit = m_lAreaLimitScore - m_lXuanScore;

	//最大下注
	LONG lMaxJetton=0L;

	//庄家判断
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//其他区域
		//		LONG lOtherAreaScore = m_lBankerScore+ m_lTieScore+ m_lTieSamePointScore+ m_lBankerKingScore;

		//庄家积分
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//庄家限制
		LONG lMaxTianScore = lBankerScore;
		lMaxTianScore -=(m_lTianScore*10+m_lDiScore*10+m_lXuanScore*10+m_lHuangScore*10);
		//lMaxTianScore -= (m_lPlayerScore+m_lPlayerKingScore*2+(m_lPlayerTwoPair+m_lBankerTwoPair)*RATE_TWO_PAIR);
		ASSERT(lMaxTianScore>=0);

		//最大下注
		lMaxJetton = min(lMaxTianScore/10, lUserMaxJetton);	
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//最大下注
LONG CTableFrameSink::GetMaxHuangScore(WORD wChairID)
{
	//玩家限制
	LONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//区域限制
	LONG lAreaLimit = m_lAreaLimitScore - m_lHuangScore;

	//最大下注
	LONG lMaxJetton=0L;

	//庄家判断
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//其他区域
		//		LONG lOtherAreaScore = m_lBankerScore+ m_lTieScore+ m_lTieSamePointScore+ m_lBankerKingScore;

		//庄家积分
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//庄家限制
		LONG lMaxTianScore = lBankerScore;
		lMaxTianScore -=(m_lTianScore*10+m_lDiScore*10+m_lXuanScore*10+m_lHuangScore*10);
		//lMaxTianScore -= (m_lPlayerScore+m_lPlayerKingScore*2+(m_lPlayerTwoPair+m_lBankerTwoPair)*RATE_TWO_PAIR);
		ASSERT(lMaxTianScore>=0);

		//最大下注
		lMaxJetton = min(lMaxTianScore/10, lUserMaxJetton);	
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//计算得分
LONG CTableFrameSink::CalculateScore()
{
	//变量定义
	LONG cbRevenue=m_pGameServiceOption->wRevenue;

	BYTE cbWinner=0;
	//推断玩家
	DeduceWinner(cbWinner);

	//游戏记录
	//--
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.cbWinner=cbWinner;

	//移动下标
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst ) m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;

	//计算牌型
	BYTE cbMultiple[5];
	ZeroMemory(&cbMultiple, sizeof(cbMultiple));

	//计算各牌型的倍数
	for(BYTE i=0; i<5; i++)
	{
		cbMultiple[i] = m_GameLogic.GetMultiple(m_cbTableCardArray[i], 3);
	}

	LONG cbTotalScore = 0;
	//个闲家与庄家比牌结果
	bool bResult[4] = {false, false, false, false};
	for(BYTE i=1; i<5; i++)
	{
		if(m_GameLogic.IsBigger(m_cbTableCardArray[i], m_cbTableCardArray[0], 3, 3))
			bResult[i-1] = true;
		else
			bResult[i-1] = false;
	}

	//计算成绩
	for(BYTE i=0; i<GAME_PLAYER; i++)
	{
		m_lUserWinScore[i] = 0;

		//天
		if(m_lUserTianScore[i] > 0)
		{
			if(bResult[0])
				m_lUserWinScore[i] += m_lUserTianScore[i]*cbMultiple[1];
			else
				m_lUserWinScore[i] -= m_lUserTianScore[i]*cbMultiple[0];
		}
		//地
		if(m_lUserDiScore[i]>0)
		{
			if(bResult[1])
				m_lUserWinScore[i] += m_lUserDiScore[i]*cbMultiple[2];
			else
				m_lUserWinScore[i] -= m_lUserDiScore[i]*cbMultiple[0];
		}
		//玄
		if( m_lUserXuanScore[i] > 0 )
		{
			if(bResult[2])
				m_lUserWinScore[i] += m_lUserXuanScore[i]*cbMultiple[3];
			else
				m_lUserWinScore[i] -= m_lUserXuanScore[i]*cbMultiple[0];
		}
		//黄
		if( m_lUserHuangScore[i] > 0 )
		{
			if(bResult[3])
				m_lUserWinScore[i] += m_lUserHuangScore[i]*cbMultiple[4];
			else
				m_lUserWinScore[i] -= m_lUserHuangScore[i]*cbMultiple[0];
		}

		//总成绩
		cbTotalScore += m_lUserWinScore[i];

		//扣税
		if(m_lUserWinScore[i] > 0)
			m_lUserWinScore[i] = m_lUserWinScore[i]*(1000L-cbRevenue)/1000;
	}

	//庄家扣税
	cbTotalScore = cbTotalScore*(1000L-cbRevenue)/1000;

	return cbTotalScore;

	/*//计算牌点
//	BYTE cbPlayerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER],m_cbCardCount[INDEX_PLAYER]);
//	BYTE cbBankerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER],m_cbCardCount[INDEX_BANKER]);

	BYTE cbWinner=0;
	//推断玩家
	DeduceWinner(cbWinner);

	//游戏记录
	//--
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	//GameRecord.cbBankerCount=cbBankerCount;
	//GameRecord.cbPlayerCount=cbPlayerCount;
	//GameRecord.cbKingWinner=cbKingWinner;
	//GameRecord.bPlayerTwoPair=bPlayerTwoPair;
	//GameRecord.bBankerTwoPair=bBankerTwoPair;
	GameRecord.cbWinner=cbWinner;

	//移动下标
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst ) m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;



	//庄家总量
	LONG lBankerWinScore = 0;

	//玩家成绩
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	LONG lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));


	for(BYTE i=0;i<5;i++)
	{
		m_cbTimers[i]=m_GameLogic.GetTimes(m_cbTableCardArray[i],5);
	}
	

	//玩家下注
	//LONG *pUserScore[] = {NULL, m_lUserPlayerScore, m_lUserTieScore, m_lUserBankerScore, m_lUserPlayerKingScore, 
	//	m_lUserBankerKingScore, m_lUserTieSamePointScore};

	//区域倍率
	//BYTE cbMultiple[] = {0, 1, 8, 1, 2, 2, 33};

	//计算积分
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//庄家判断
		if (m_wCurrentBanker==i) continue;

		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem==NULL) continue;

		//for ( WORD wAreaIndex = ID_XIAN_JIA; wAreaIndex <= ID_TONG_DIAN_PING; ++wAreaIndex )
		//{
		//	if ( wAreaIndex == cbWinner || wAreaIndex == cbKingWinner ) 
		//	{
		//		m_lUserWinScore[i] += ( pUserScore[wAreaIndex][i] * cbMultiple[wAreaIndex] ) ;
		//		m_lUserReturnScore[i] += pUserScore[wAreaIndex][i] ;
		//		lBankerWinScore -= ( pUserScore[wAreaIndex][i] * cbMultiple[wAreaIndex] ) ;
		//	}
		//	else if ( cbWinner == ID_PING_JIA )
		//	{
		//		m_lUserReturnScore[i] += pUserScore[wAreaIndex][i] ;
		//	}
		//	else
		//	{
		//		lUserLostScore[i] -= pUserScore[wAreaIndex][i];
		//		lBankerWinScore += pUserScore[wAreaIndex][i];
		//	}
		//}

		////闲对子
		//if (bPlayerTwoPair)
		//{
		//	//玩家赢分
		//	m_lUserWinScore[i] += (m_lUserPlayerTwoPair[i] * RATE_TWO_PAIR) ;
		//	m_lUserReturnScore[i] += m_lUserPlayerTwoPair[i] ;
		//	lBankerWinScore -= (m_lUserPlayerTwoPair[i] * RATE_TWO_PAIR) ;
		//}
		//else
		//{
		//	//玩家输分
		//	lUserLostScore[i] -= m_lUserPlayerTwoPair[i] ;
		//	lBankerWinScore += m_lUserPlayerTwoPair[i] ;
		//}

		//if (bBankerTwoPair)
		//{
		//	//玩家赢分
		//	m_lUserWinScore[i] += (m_lUserBankerTwoPair[i] * RATE_TWO_PAIR) ;
		//	m_lUserReturnScore[i] += m_lUserBankerTwoPair[i] ;
		//	lBankerWinScore -= (m_lUserBankerTwoPair[i] * RATE_TWO_PAIR) ;
		//}
		//else
		//{
		//	//玩家输分
		//	lUserLostScore[i] -= m_lUserBankerTwoPair[i];
		//	lBankerWinScore += m_lUserBankerTwoPair[i];
		//}
//--还没加赔率


		if(ID_TIAN_MARK & cbWinner)
		{
			m_lUserWinScore[i]+=m_lUserTianScore[i]*m_cbTimers[1];
			m_lUserReturnScore[i] += m_lUserTianScore[i] ;
			lBankerWinScore -= m_lUserTianScore[i]*m_cbTimers[1];
		}
		else
		{
			lUserLostScore[i] -=m_lUserTianScore[i]* m_cbTimers[0];
			lBankerWinScore += m_lUserTianScore[i] *m_cbTimers[0];		
		}

		if(ID_DI_MARK & cbWinner)
		{
			m_lUserWinScore[i]+=m_lUserDiScore[i]*m_cbTimers[2];
			m_lUserReturnScore[i] += m_lUserDiScore[i] ;
			lBankerWinScore -= m_lUserDiScore[i]*m_cbTimers[2];
		}
		else
		{
			lUserLostScore[i] -=m_lUserDiScore[i]*m_cbTimers[0] ;
			lBankerWinScore += m_lUserDiScore[i]*m_cbTimers[0] ;
		}

		if(ID_XUAN_MARK & cbWinner)
		{
			m_lUserWinScore[i]+=m_lUserXuanScore[i]*m_cbTimers[3];
			m_lUserReturnScore[i] += m_lUserXuanScore[i] ;
			lBankerWinScore -= m_lUserXuanScore[i]*m_cbTimers[3];
		}
		else
		{
			lUserLostScore[i] -=m_lUserXuanScore[i]*m_cbTimers[0] ;
			lBankerWinScore += m_lUserXuanScore[i]*m_cbTimers[0] ;
		}

		if(ID_HUANG_MARK & cbWinner)
		{
			m_lUserWinScore[i]+=m_lUserHuangScore[i]*m_cbTimers[4] ;
			m_lUserReturnScore[i] += m_lUserHuangScore[i] ;
			lBankerWinScore -= m_lUserHuangScore[i]*m_cbTimers[4] ;
		}
		else
		{
			lUserLostScore[i] -=m_lUserHuangScore[i] *m_cbTimers[0] ;
			lBankerWinScore += m_lUserHuangScore[i] *m_cbTimers[0] ;

		}


		//计算税收
		if (0 < m_lUserWinScore[i])
		{
			float fRevenuePer=float(cbRevenue/1000.+0.000001);
			m_lUserRevenue[i]  = LONG(m_lUserWinScore[i]*fRevenuePer);
			m_lUserWinScore[i] -= m_lUserRevenue[i];
		}

		//总的分数
		m_lUserWinScore[i] += lUserLostScore[i];
	}

	//庄家成绩
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;

		//计算税收
		if (0 < m_lUserWinScore[m_wCurrentBanker])
		{
			float fRevenuePer=float(cbRevenue/1000.+0.000001);
			m_lUserRevenue[m_wCurrentBanker]  = LONG(m_lUserWinScore[m_wCurrentBanker]*fRevenuePer);
			m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
			lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
		}		
	}

	//累计积分
	m_lBankerWinScore += lBankerWinScore;

	//当前积分
	m_lBankerCurGameScore=lBankerWinScore;

	return lBankerWinScore;*/

	return 1;
}


void CTableFrameSink::DeduceWinner(BYTE &cbWinner)
{
	cbWinner = 0x00;

	if (m_GameLogic.IsBigger(m_cbTableCardArray[1], m_cbTableCardArray[0], 3, 3) )
		cbWinner |= 0x01;
	if (m_GameLogic.IsBigger(m_cbTableCardArray[2], m_cbTableCardArray[0], 3, 3) )
		cbWinner |= 0x02;
	if (m_GameLogic.IsBigger(m_cbTableCardArray[3], m_cbTableCardArray[0], 3, 3) )
		cbWinner |= 0x04;
	if (m_GameLogic.IsBigger(m_cbTableCardArray[4], m_cbTableCardArray[0], 3, 3) )
		cbWinner |= 0x08;
	//计算牌点
//	BYTE cbPlayerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER],m_cbCardCount[INDEX_PLAYER]);
//	BYTE cbBankerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER],m_cbCardCount[INDEX_BANKER]);

	//BOOL bUserOxData[5];
	//ZeroMemory(bUserOxData,sizeof(bUserOxData));
	//for(WORD i=0;i<5;i++)
	//{
	//	bUserOxData[i] = (m_GameLogic.GetCardType(m_cbTableCardArray[i],MAX_COUNT)>0)?TRUE:FALSE;
	//}

	/*for(int i=INDEX_PLAYER1;i<=INDEX_PLAYER4;i++)
	{
		BYTE cbMarkType=0;
		switch(i)
		{
		case 1:
			cbMarkType=ID_TIAN_MARK;
			break;
		case 2:
			cbMarkType=ID_DI_MARK;
			break;
		case 3:
			cbMarkType=ID_XUAN_MARK;
			break;
		case 4:
			cbMarkType=ID_HUANG_MARK;
			break;
		}
		if (m_GameLogic.CompareCard(m_cbTableCardArray[i],m_cbTableCardArray[INDEX_BANKER],MAX_COUNT))
		{

			if(cbMarkType)
			{

				cbWinner|=cbMarkType;
			}
			
		}
		else
		{
			if(cbMarkType)
			{
				cbWinner = (cbWinner & ~cbMarkType);
			}

		}
	}*/
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
//////////////////////////////////////////////////////////////////////////

