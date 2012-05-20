#include "StdAfx.h"
#include <algorithm>

using namespace std;
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//常量定义
#define SEND_COUNT					300									//发送次数


//索引定义
#define INDEX_PLAYER				0									//闲家索引
#define INDEX_BANKER				1									//庄家索引

//空闲时间
#define IDI_FREE					1									//空闲时间
#ifdef _DEBUG
#define TIME_FREE					3									//空闲时间
#else
#define TIME_FREE					8									//空闲时间
#endif

//下注时间
#define IDI_PLACE_JETTON			2									//下注时间

#ifdef _DEBUG
#define TIME_PLACE_JETTON			10									//下注时间
#else
#define TIME_PLACE_JETTON			25									//下注时间
#endif

//比赛时间
#define IDI_GAME_END				3									//比赛时间

#ifdef _DEBUG
#define TIME_GAME_END				35									//比赛时间
#else
#define TIME_GAME_END				35									//比赛时间
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
	m_lScore2=0L;
	m_lScore3=0L;
	m_lScore1=0L;		
	m_lScore6=0L;
	m_lScore5=0L;
	m_lScore4=0L;	
	m_lScore7=0L;
	m_lScore8=0L;

	//控制变量
	m_cbWinSideControl=0;
	m_nSendCardCount=0;

	//个人下注
	ZeroMemory(m_lUserScore2,sizeof(m_lUserScore2));
	ZeroMemory(m_lUserScore1,sizeof(m_lUserScore1));
	ZeroMemory(m_lUserScore6,sizeof(m_lUserScore6));
	ZeroMemory(m_lUserScore5,sizeof(m_lUserScore5));
	ZeroMemory(m_lUserScore4,sizeof(m_lUserScore4));
	ZeroMemory(m_lUserScore7,sizeof(m_lUserScore7));
	ZeroMemory(m_lUserScore8,sizeof(m_lUserScore8));
	ZeroMemory(m_lUserScore3,sizeof(m_lUserScore3));

	//玩家成绩	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	////扑克信息
	//ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	//ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

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


	//设置文件名
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_snprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\ServerIni\\RaceHorseConfig.ini"),szPath);

	//AfxMessageBox(m_szConfigFileName);
	//控制变量

	//m_lApplyBankerCondition=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Score"), 10000*10, m_szConfigFileName);
	m_lAreaLimitScore=GetPrivateProfileInt(TEXT("ScoreLimit"), TEXT("AreaLimitScore"), 1000000 , m_szConfigFileName);
	m_lUserLimitScore=GetPrivateProfileInt(TEXT("ScoreLimit"), TEXT("UserLimitScore"), 10000000/*lMaxScore*/, m_szConfigFileName);
	BYTE cbEnableSysBanker=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("EnableSysBanker"), 1, m_szConfigFileName);
	m_bEnableSysBanker=cbEnableSysBanker?true:false;

	if (m_pGameServiceOption->lLessScore == 100000)									//初级房
	{
		m_lApplyBankerCondition=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Score1"), 100000*10, m_szConfigFileName);
	}
	else if (m_pGameServiceOption->lLessScore == 1000000)							//中级房
	{
		m_lApplyBankerCondition=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Score2"), 1000000*10, m_szConfigFileName);
	}
	else if (m_pGameServiceOption->lLessScore == 10000000)							//高级房
	{
		m_lApplyBankerCondition=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Score3"), 10000000*10, m_szConfigFileName);
	}
	else if (m_pGameServiceOption->lLessScore == 100000000)							//富豪房
	{
		m_lApplyBankerCondition=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Score4"), 100000000*10, m_szConfigFileName);
	}else
	{
		m_lApplyBankerCondition = 100000;
	}

	//TCHAR str[100];
	//AfxMessageBox(str);
	return true;
}

//复位桌子
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//总下注数
	m_lScore2=0L;
	m_lScore3=0L;
	m_lScore1=0L;		
	m_lScore6=0L;
	m_lScore5=0L;
	m_lScore4=0L;	
	m_lScore7=0L;
	m_lScore8=0L;

	//个人下注
	ZeroMemory(m_lUserScore2,sizeof(m_lUserScore2));
	ZeroMemory(m_lUserScore1,sizeof(m_lUserScore1));
	ZeroMemory(m_lUserScore6,sizeof(m_lUserScore6));
	ZeroMemory(m_lUserScore5,sizeof(m_lUserScore5));
	ZeroMemory(m_lUserScore4,sizeof(m_lUserScore4));
	ZeroMemory(m_lUserScore7,sizeof(m_lUserScore7));
	ZeroMemory(m_lUserScore8,sizeof(m_lUserScore8));
	ZeroMemory(m_lUserScore3,sizeof(m_lUserScore3));

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
	{
		pIBankerServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		//m_lAreaLimitScore = pIBankerServerUserItem->GetUserScore()->lGameGold/8;
		//m_lUserLimitScore = pIBankerServerUserItem->GetUserScore()->lGameGold;
	}
	//else
	//{
	//	LONGLONG lBankerCondition=m_pGameServiceOption->lLessScore;
	//	if (lBankerCondition != 0)
	//	{
	//		//m_lApplyBankerCondition = lBankerCondition*10;
	//		m_lAreaLimitScore = m_lApplyBankerCondition;
	//		m_lUserLimitScore = m_lApplyBankerCondition * 8;
	//	}else
	//	{
	//		//m_lApplyBankerCondition = 10000000;
	//		m_lAreaLimitScore = m_lApplyBankerCondition;
	//		m_lUserLimitScore = m_lApplyBankerCondition * 8;
	//	}
	//}

	//限制提示

	TCHAR szTipMsg[128] = _T("");
	int count = GetPrivateProfileInt(TEXT("GameMsg"),TEXT("GameMsgCount"),0,m_szConfigFileName);
	if (count !=0)
	{
		TCHAR szMsg1[128]=_T(""), szMsg2[128]=_T(""), szMsg3[128]= _T(""),szMsg[128] = _T("");
		GetPrivateProfileString(TEXT("GameMsg"),_T("GameMsg0"),TEXT("上庄条件为："),szMsg1,CountArray(szMsg1),m_szConfigFileName);
		GetPrivateProfileString(TEXT("GameMsg"),_T("GameMsg1"),TEXT("区域限制为："),szMsg2,CountArray(szMsg2),m_szConfigFileName);
		GetPrivateProfileString(TEXT("GameMsg"),_T("GameMsg2"),TEXT("玩家限制为："),szMsg3,CountArray(szMsg3),m_szConfigFileName);
		
		sprintf(szMsg,TEXT("\n%s%s\n%s%s\n%s%s"),szMsg1,TEXT("%I64d"),szMsg2,TEXT("%I64d"),szMsg3,TEXT("%I64d"));
		sprintf(szTipMsg,szMsg,m_lApplyBankerCondition,m_lAreaLimitScore,m_lUserLimitScore);
	}
	else
	{
		_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("\n  上庄条件为：%I64d\n  区域限制为：%I64d\n  玩家限制为：%I64d"),m_lApplyBankerCondition,m_lAreaLimitScore,m_lUserLimitScore);	
	}	

	//设置变量
	GameStart.cbTimeLeave=TIME_PLACE_JETTON;
	GameStart.wBankerUser=m_wCurrentBanker;
	if (pIBankerServerUserItem!=NULL) GameStart.lBankerScore=pIBankerServerUserItem->GetUserScore()->lScore;
	GameStart.lAreaLimitScore = m_lAreaLimitScore;

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

		m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);
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
					m_pITableFrame->SetGameStatus(GS_FREE);
					//设置时间
					m_pITableFrame->SetGameTimer(GS_FREE,TIME_FREE*1000,1,0L);
				}
				else
				{
					//变量定义
					LONGLONG lScore=0;
					LONGLONG lRevenue=0;
					enScoreKind ScoreKind=enScoreKind_Flee;

					//统计成绩
					//lScore=-( m_lUserScore2[wChairID] + m_lUserScore3[wChairID] + m_lUserScore1[wChairID] + 
					//	m_lUserScore5[wChairID] + m_lUserScore4[wChairID] + m_lUserScore6[wChairID]+
					//	m_lUserScore7[wChairID]+m_lUserScore8[wChairID]);

					//写入积分
					if (lScore!=0L) m_pITableFrame->WriteUserScore(pIServerUserItem, lScore,lRevenue, ScoreKind);
				}
			}

			//庄家判断
			if (cbReason==GER_USER_LEFT && wChairID!=m_wCurrentBanker) return true;

			
			//派发扑克
			DispatchTableCard();

			//计算分数
			LONGLONG lBankerWinScore=CalculateScore();

			//逃跑判断
			if (cbReason==GER_USER_LEFT && wChairID==m_wCurrentBanker && m_lUserWinScore[m_wCurrentBanker]<0)
			{
				m_pITableFrame->WriteUserScore(m_wCurrentBanker,m_lUserWinScore[m_wCurrentBanker], m_lUserRevenue[m_wCurrentBanker], enScoreKind_Flee);
			}

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

			CopyMemory(GameEnd.cbHorseSpeed,m_cbHorseSpeed,sizeof(GameEnd.cbHorseSpeed));

			
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

					//游戏记录
			tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
			GameRecord.cbWinner = m_GameLogic.GetNumBigList(m_cbHorseSpeed,HORSE_COUNT);

			//移动下标
			m_nRecordLast = (m_nRecordLast+1) % MAX_S_SCORE_HISTORY;
			if ( m_nRecordLast == m_nRecordFirst ) m_nRecordFirst = (m_nRecordFirst+1) % MAX_S_SCORE_HISTORY;

			
			//切换庄家
			if (cbReason==GER_USER_LEFT && wChairID==m_wCurrentBanker) ChangeBanker(true);

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
			//SendGameRecord(pIServerUserItem);

			LONGLONG lBankerCondition=m_pGameServiceOption->lLessScore;
			if (lBankerCondition != 0)
			{
				m_lApplyBankerCondition = lBankerCondition*10;
			}			

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
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄条件为：%I64d"),m_lApplyBankerCondition);
			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);
						
			//发送申请者
			SendApplyUser(pIServerUserItem);

			return bSuccess;
		}
	case GS_PLACE_JETTON:		//游戏状态
	case GS_GAME_END:			//结束状态
		{
				
			
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//全局下注
			StatusPlay.lAllScore1=m_lScore1;		
			StatusPlay.lAllScore2=m_lScore2;		
			StatusPlay.lAllScore3=m_lScore3;		
			StatusPlay.lAllScore4=m_lScore4;
			StatusPlay.lAllScore5=m_lScore5;	
			StatusPlay.lAllScore6=m_lScore6;	
			StatusPlay.lAllScore7=m_lScore7;	
			StatusPlay.lAllScore8=m_lScore8;	

			//玩家下注
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusPlay.lUserScore1=m_lUserScore1[wChiarID];			
				StatusPlay.lUserScore2=m_lUserScore2[wChiarID];		
				StatusPlay.lUserScore3=m_lUserScore3[wChiarID];		
				StatusPlay.lUserScore4=m_lUserScore4[wChiarID];	
				StatusPlay.lUserScore5=m_lUserScore5[wChiarID];	
				StatusPlay.lUserScore6=m_lUserScore6[wChiarID];	
				StatusPlay.lUserScore7=m_lUserScore7[wChiarID];		
				StatusPlay.lUserScore8=m_lUserScore8[wChiarID];

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

				//信息
				CopyMemory(StatusPlay.cbHorseSpeed,m_cbHorseSpeed,sizeof(StatusPlay.cbHorseSpeed));

				//发送记录
				SendGameRecord(pIServerUserItem);	
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
				if (m_lUserWinScore[wUserChairID]!=0L) 
					m_pITableFrame->WriteUserScore(wUserChairID,m_lUserWinScore[wUserChairID], m_lUserRevenue[wUserChairID], ScoreKind);

				//坐庄判断
				LONGLONG lUserScore=pIServerUserItem->GetUserScore()->lScore;
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
						_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("由于你的金币数少于坐庄必须金币数（%I64d）,你自动下庄！"),m_lApplyBankerCondition);
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
	case SUB_C_OPEN_BANK:
		{
			//金币房间
			if(m_pGameServiceOption->wServerType == GAME_GENRE_SCORE)
			{
				m_pITableFrame->SendGameMessage(pIServerUserItem,"点值房间不能使用银行！",SMT_INFO);
			}else if(m_pGameServiceOption->wServerType == GAME_GENRE_MATCH)
			{
				m_pITableFrame->SendGameMessage(pIServerUserItem,"比赛房间不能使用银行！",SMT_INFO);
			}else if(m_pGameServiceOption->wServerType == GAME_GENRE_EDUCATE)
			{
				m_pITableFrame->SendGameMessage(pIServerUserItem,"练习房间不能使用银行！",SMT_INFO);
			}
			
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
		m_pITableFrame->SetGameStatus(GS_FREE);
	}
	

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
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONGLONG lJettonScore)
{
	//效验参数
	ASSERT((cbJettonArea<=ID_HORSE8)&&(lJettonScore>0L));
	if ((cbJettonArea>ID_HORSE8)||(lJettonScore<=0L)) return false;

	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus()==GS_PLACE_JETTON);
	if (m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON) return true;

	//庄家判断
	if (m_wCurrentBanker==wChairID) return true;
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) return true;

	//变量定义
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	LONGLONG lJettonCount=m_lUserScore2[wChairID]+m_lUserScore3[wChairID]+m_lUserScore1[wChairID]+
		m_lUserScore6[wChairID] + m_lUserScore4[wChairID] + m_lUserScore5[wChairID]+
		m_lUserScore7[wChairID]+m_lUserScore8[wChairID];

	LONGLONG lUserScore = pIServerUserItem->GetUserScore()->lScore;

	//合法校验
	if (lUserScore < lJettonCount + lJettonScore) return true;
	if (m_lUserLimitScore < lJettonCount + lJettonScore) return true;

	//成功标识
	bool bPlaceJettonSuccess=true;

	//合法验证
	if ( ID_HORSE1 == cbJettonArea )
	{
		if ( GetMaxHorse1(wChairID) >= lJettonScore )
		{
			//保存下注
			m_lScore1 += lJettonScore;
			m_lUserScore1[wChairID] += lJettonScore;			
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_HORSE2 == cbJettonArea )
	{
		if ( GetMaxHorse2(wChairID)>=lJettonScore ) 
		{
			//保存下注
			m_lScore2 += lJettonScore;
			m_lUserScore2[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_HORSE3 == cbJettonArea )
	{
		if ( GetMaxHorse3(wChairID)>=lJettonScore ) 
		{
			//保存下注
			m_lScore3 += lJettonScore;
			m_lUserScore3[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_HORSE4 == cbJettonArea )
	{
		if ( GetMaxHorse4(wChairID) >= lJettonScore ) 
		{
			//保存下注
			m_lScore4 += lJettonScore;
			m_lUserScore4[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_HORSE5 == cbJettonArea )
	{
		if ( GetMaxHorse5(wChairID) >=lJettonScore ) 
		{
			//保存下注
			m_lScore5 += lJettonScore;
			m_lUserScore5[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_HORSE6 == cbJettonArea )
	{
		if ( GetMaxHorse6(wChairID)>=lJettonScore )
		{
			//保存下注
			m_lScore6 += lJettonScore;
			m_lUserScore6[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}	
	else if (ID_HORSE7==cbJettonArea)
	{
		if ( GetMaxHorse7(wChairID)>=lJettonScore ) 
		{
			//保存下注
			m_lScore7+=lJettonScore;
			m_lUserScore7[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if (ID_HORSE8==cbJettonArea)
	{
		if ( GetMaxHorse8(wChairID)>=lJettonScore ) 
		{
			//保存下注
			m_lScore8+=lJettonScore;
			m_lUserScore8[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
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
bool CTableFrameSink::DispatchSpeed()
{

	return true;
}
//发送速度
bool CTableFrameSink::DispatchTableCard()
{

	for (int i=0; i < HORSE_COUNT; i++)
	{
		m_GameLogic.RandNumList(m_cbHorseSpeed[i],HORSE_COUNT);
	}
	

	BYTE cbWinner = m_GameLogic.GetNumBigList(m_cbHorseSpeed,HORSE_COUNT);
	
////////////////////////////////////////////////////////////////////////////////////////////////////
	//规范速度，使其保持在一定范围内
	BYTE cbCount = m_GameLogic.GetNumListPip(m_cbHorseSpeed[cbWinner-1],HORSE_COUNT);

	if (cbCount > 60)
	{
		BYTE temp = cbCount - 60;
		for (int i=0; i < HORSE_COUNT; i++)
		{
			BYTE temp2 = m_GameLogic.GetNumListPip(m_cbHorseSpeed[i],HORSE_COUNT);
			if (temp2 >= 60)
			{
				m_cbHorseSpeed[i][4] -= temp2 - 60;
			}
		}

	}else if (cbCount < 60)
	{
		BYTE temp = cbCount - 60;
		for (int i=0; i < HORSE_COUNT; i++)
			m_cbHorseSpeed[i][4] += temp;

	}
///////////////////////////////////////////////////////////////////////////////////////////////////
	//游戏控制
	BYTE cbTempWinner = 0;

	//读取配置
	int   nAccountsCount = 0;
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	//读取配置
	TCHAR szFileName[MAX_PATH];
	_snprintf(szFileName,sizeof(szFileName),TEXT("%s\\ServerIni\\RaceHorseWaiGua.ini"),szPath);
	nAccountsCount = GetPrivateProfileInt( TEXT("WAIGUA"), TEXT("AccountsCount"), 0, szFileName );

	if (nAccountsCount != 0)
	{
		TCHAR szCtrlName[50][MAX_PATH],szTempName[MAX_PATH]=TEXT("");		
		for (WORD i=0;i<nAccountsCount;i++)
		{
			ZeroMemory(szCtrlName[i],sizeof(szCtrlName[i]));
			ZeroMemory(szTempName,sizeof(szTempName));
			sprintf(szTempName,TEXT("p%dname"),i);
			GetPrivateProfileString( TEXT("WAIGUA"), szTempName, "123",szCtrlName[i],sizeof(szCtrlName[i]),szFileName);
		}

		BYTE  cbChairIDCtrl = INVALID_CHAIR;
		TCHAR szNewName[MAX_PATH];//测试完了不要输出去
		ZeroMemory(szNewName,sizeof(szNewName));
		for (WORD i=0;i<nAccountsCount;i++)
		{
			bool bCtrl = false;
			for (WORD wChairID=0;wChairID<GAME_PLAYER;wChairID++)
			{
				//校验是否存在玩家
				IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
				if (pIServerUserItem == NULL)
				{
					continue;
				}

				if(lstrcmp(pIServerUserItem->GetAccounts(),szCtrlName[i]) == 0)
				{
					CopyMemory(szNewName,szCtrlName[i],sizeof(szCtrlName[i]));
					cbChairIDCtrl = wChairID;
					bCtrl = true;
					break;
				}
			 }
			if (bCtrl)
			{
				break;
			}
		}

		//IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(cbChairIDCtrl);
		//if (pIServerUserItem != NULL)
		//{
		//	m_pITableFrame->SendGameMessage(pIServerUserItem,pIServerUserItem->GetAccounts(),SMT_INFO);
		//}

		//判断那个区域下注了，如果存在下注到多个区域，只取前一个区域，后面的忽略
		//如果作弊玩家不下注，
		//如果是庄家，下注最少的区域赢

		LONGLONG lScore[] = {m_lUserScore1[cbChairIDCtrl],m_lUserScore2[cbChairIDCtrl],m_lUserScore3[cbChairIDCtrl],m_lUserScore4[cbChairIDCtrl],m_lUserScore5[cbChairIDCtrl],m_lUserScore6[cbChairIDCtrl],m_lUserScore7[cbChairIDCtrl],m_lUserScore8[cbChairIDCtrl]};
		for (BYTE i=0;i<HORSE_COUNT;i++)
		{
			if (lScore[i] > 0)
			{
				cbTempWinner = i+1;
				break;
			}
		}

		//作弊玩家已经下注
		if(cbTempWinner > 0)
		{
			cbWinner = cbTempWinner;
		}
		else if (cbChairIDCtrl == m_wCurrentBanker)
		{
			LONGLONG lScoreAll[] = {m_lScore1, m_lScore2, m_lScore3, m_lScore4, m_lScore5, m_lScore6, m_lScore7, m_lScore8};

			//判断一下有人下注了
			for (BYTE i=0;i<HORSE_COUNT;i++)
			{
				if (lScoreAll[i]>0)
				{
					cbTempWinner = 1;
					break;
				}
			}

			//有人下注再去判断哪个区域下注最少
			if (cbTempWinner == 1)
			{
				LONGLONG lTempScore =lScoreAll[0];
				for (BYTE i=1;i<HORSE_COUNT;i++)
				{					
					if (lTempScore > lScoreAll[i])
					{
						lTempScore = lScoreAll[i];
						cbTempWinner = i+1;
					}
				}
			}
			

			if(cbTempWinner > 0)
			{
				cbWinner = cbTempWinner;
			}
		}	
		
	}	

	//读取分数
	m_cbWinSideControl = GetPrivateProfileInt( TEXT("ManagerControl"), TEXT("WinSide"), 0, m_szConfigFileName );

	//控制输赢
	if ( m_cbWinSideControl != 0 )
	{
		//重发判断
		if ( cbWinner!=m_cbWinSideControl)
		{
			cbWinner = m_cbWinSideControl;
		}
	}

	m_cbHorseSpeed[cbWinner-1][0] = 8;
	m_cbHorseSpeed[cbWinner-1][1] = 7;
	m_cbHorseSpeed[cbWinner-1][2] = 8;
	m_cbHorseSpeed[cbWinner-1][3] = 7;
	m_cbHorseSpeed[cbWinner-1][4] = 8;
	m_cbHorseSpeed[cbWinner-1][5] = 8;
	m_cbHorseSpeed[cbWinner-1][6] = 8;
	m_cbHorseSpeed[cbWinner-1][7] = 8;
////////////////////////////////////////////////////////////////////////////////////////////////////

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
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

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
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]分数少于(%I64d)，强行换庄!"),pIServerUserItem->GetAccounts(),m_lApplyBankerCondition);
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
			LONGLONG lMessageCount=GetPrivateProfileInt(TEXT("Message"),TEXT("MessageCount"),0,m_szConfigFileName);
			if (lMessageCount!=0)
			{
				//读取配置
				LONGLONG lIndex=rand()%lMessageCount;
				TCHAR szKeyName[32],szMessage1[256],szMessage2[256];				
				_sntprintf(szKeyName,CountArray(szKeyName),TEXT("Item%I64d"),lIndex);
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
LONGLONG CTableFrameSink::GetUserMaxJetton(WORD wChairID)
{
	//已下注额
	LONGLONG lNowJetton = 0;
	lNowJetton += m_lUserScore1[wChairID];
	lNowJetton += m_lUserScore2[wChairID];
	lNowJetton += m_lUserScore3[wChairID];
	lNowJetton += m_lUserScore4[wChairID];
	lNowJetton += m_lUserScore5[wChairID];
	lNowJetton += m_lUserScore6[wChairID];
	lNowJetton += m_lUserScore7[wChairID];
	lNowJetton += m_lUserScore8[wChairID];

	//获取玩家
	IServerUserItem *pIMeServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);

	//玩家下注
	LONGLONG lMeMaxScore = min(pIMeServerUserItem->GetUserScore()->lScore, m_lUserLimitScore);
	lMeMaxScore -= lNowJetton;
	ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return lMeMaxScore;
}

//最大下注
LONGLONG CTableFrameSink::GetMaxHorse1(WORD wChairID)
{
	//玩家限制
	LONGLONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//区域限制
	LONGLONG lAreaLimit = m_lAreaLimitScore - m_lScore1;

	//最大下注
	LONGLONG lMaxJetton=0L;

	//庄家判断
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//其他区域
		LONGLONG lOtherAreaScore = m_lScore2+ m_lScore3+ m_lScore4+ m_lScore5+ m_lScore6+ m_lScore7+ m_lScore8;

		//庄家积分
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//庄家限制
		LONGLONG lMaxPlayerScore = lBankerScore + lOtherAreaScore;
		lMaxPlayerScore -= m_lScore1;
		ASSERT(lMaxPlayerScore>=0);

		//最大下注
		lMaxJetton = min(lMaxPlayerScore/7, lUserMaxJetton);
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//最大下注
LONGLONG CTableFrameSink::GetMaxHorse2(WORD wChairID)
{	
	//玩家限制
	LONGLONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//区域限制
	LONGLONG lAreaLimit = m_lAreaLimitScore - m_lScore2;

	//最大下注
	LONGLONG lMaxJetton=0L;

	//庄家判断
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//其他区域
		LONGLONG lOtherAreaScore = m_lScore1+ m_lScore3+ m_lScore4+ m_lScore5+ m_lScore6+ m_lScore7+ m_lScore8;

		//庄家积分
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//庄家限制
		LONGLONG lMaxPlayerScore = lBankerScore + lOtherAreaScore;
		lMaxPlayerScore -= m_lScore2;
		ASSERT(lMaxPlayerScore>=0);

		//最大下注
		lMaxJetton = min(lMaxPlayerScore/7, lUserMaxJetton);
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//最大下注
LONGLONG CTableFrameSink::GetMaxHorse3(WORD wChairID)
{
	//玩家限制
	LONGLONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//区域限制
	LONGLONG lAreaLimit = m_lAreaLimitScore - m_lScore3;

	//最大下注
	LONGLONG lMaxJetton=0L;

	//庄家判断
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//其他区域
		LONGLONG lOtherAreaScore = m_lScore1+ m_lScore2+ m_lScore4+ m_lScore5+ m_lScore6+ m_lScore7+ m_lScore8;

		//庄家积分
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//庄家限制
		LONGLONG lMaxPlayerScore = lBankerScore + lOtherAreaScore;
		lMaxPlayerScore -= m_lScore3;
		ASSERT(lMaxPlayerScore>=0);

		//最大下注
		lMaxJetton = min(lMaxPlayerScore/7, lUserMaxJetton);
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//最大下注
LONGLONG CTableFrameSink::GetMaxHorse4(WORD wChairID)
{
	//玩家限制
	LONGLONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//区域限制
	LONGLONG lAreaLimit = m_lAreaLimitScore - m_lScore4;

	//最大下注
	LONGLONG lMaxJetton=0L;

	//庄家判断
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//其他区域
		LONGLONG lOtherAreaScore = m_lScore1+ m_lScore2+ m_lScore3+ m_lScore5+ m_lScore6+ m_lScore7+ m_lScore8;

		//庄家积分
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//庄家限制
		LONGLONG lMaxPlayerScore = lBankerScore + lOtherAreaScore;
		lMaxPlayerScore -= m_lScore4;
		ASSERT(lMaxPlayerScore>=0);

		//最大下注
		lMaxJetton = min(lMaxPlayerScore/7, lUserMaxJetton);
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//最大下注
LONGLONG CTableFrameSink::GetMaxHorse5(WORD wChairID)
{
	//玩家限制
	LONGLONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//区域限制
	LONGLONG lAreaLimit = m_lAreaLimitScore - m_lScore5;

	//最大下注
	LONGLONG lMaxJetton=0L;

	//庄家判断
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//其他区域
		LONGLONG lOtherAreaScore = m_lScore1+ m_lScore2+ m_lScore3+ m_lScore4+ m_lScore6+ m_lScore7+ m_lScore8;

		//庄家积分
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//庄家限制
		LONGLONG lMaxPlayerScore = lBankerScore + lOtherAreaScore;
		lMaxPlayerScore -= m_lScore5;
		ASSERT(lMaxPlayerScore>=0);

		//最大下注
		lMaxJetton = min(lMaxPlayerScore/7, lUserMaxJetton);
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//最大下注
LONGLONG CTableFrameSink::GetMaxHorse6(WORD wChairID)
{
	//玩家限制
	LONGLONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//区域限制
	LONGLONG lAreaLimit = m_lAreaLimitScore - m_lScore6;

	//最大下注
	LONGLONG lMaxJetton=0L;

	//庄家判断
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//其他区域
		LONGLONG lOtherAreaScore = m_lScore1+ m_lScore2+ m_lScore3+ m_lScore4+ m_lScore5+ m_lScore7+ m_lScore8;

		//庄家积分
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//庄家限制
		LONGLONG lMaxPlayerScore = lBankerScore + lOtherAreaScore;
		lMaxPlayerScore -= m_lScore6;
		ASSERT(lMaxPlayerScore>=0);

		//最大下注
		lMaxJetton = min(lMaxPlayerScore/7, lUserMaxJetton);
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//最大下注
LONGLONG CTableFrameSink::GetMaxHorse7(WORD wChairID)
{
	//玩家限制
	LONGLONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//区域限制
	LONGLONG lAreaLimit = m_lAreaLimitScore - m_lScore7;

	//最大下注
	LONGLONG lMaxJetton=0L;

	//庄家判断
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//其他区域
		LONGLONG lOtherAreaScore = m_lScore1+ m_lScore2+ m_lScore3+ m_lScore4+ m_lScore5+ m_lScore6+ m_lScore8;

		//庄家积分
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//庄家限制
		LONGLONG lMaxPlayerScore = lBankerScore + lOtherAreaScore;
		lMaxPlayerScore -= m_lScore7;
		ASSERT(lMaxPlayerScore>=0);

		//最大下注
		lMaxJetton = min(lMaxPlayerScore/7, lUserMaxJetton);
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//最大下注
LONGLONG CTableFrameSink::GetMaxHorse8(WORD wChairID)
{
	//玩家限制
	LONGLONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//区域限制
	LONGLONG lAreaLimit = m_lAreaLimitScore - m_lScore8;

	//最大下注
	LONGLONG lMaxJetton=0L;

	//庄家判断
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//其他区域
		LONGLONG lOtherAreaScore = m_lScore1+ m_lScore2+ m_lScore3+ m_lScore4+ m_lScore5+ m_lScore6+ m_lScore7;

		//庄家积分
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//庄家限制
		LONGLONG lMaxPlayerScore = lBankerScore + lOtherAreaScore;
		lMaxPlayerScore -= m_lScore8;
		ASSERT(lMaxPlayerScore>=0);

		//最大下注
		lMaxJetton = min(lMaxPlayerScore/7, lUserMaxJetton);	
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//计算得分
LONGLONG CTableFrameSink::CalculateScore()
{
	//变量定义
	LONGLONG cbRevenue=m_pGameServiceOption->wRevenue;

	//推断玩家
	BYTE cbWinner = m_GameLogic.GetNumBigList(m_cbHorseSpeed,HORSE_COUNT);
	
	////游戏记录
	//tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	//GameRecord.cbWinner = cbWinner;

	////移动下标
	//m_nRecordLast = (m_nRecordLast+1) % MAX_S_SCORE_HISTORY;
	//if ( m_nRecordLast == m_nRecordFirst ) m_nRecordFirst = (m_nRecordFirst+1) % MAX_S_SCORE_HISTORY;

	//庄家总量
	LONGLONG lBankerWinScore = 0;

	//玩家成绩
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	LONGLONG lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	//玩家下注
	LONGLONG *pUserScore[] = {NULL, m_lUserScore1, m_lUserScore2, m_lUserScore3, m_lUserScore4, 
		m_lUserScore5, m_lUserScore6, m_lUserScore7, m_lUserScore8};
	
	//区域倍率
	BYTE cbTime = 7;

	//计算积分
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//庄家判断
		if (m_wCurrentBanker==i) continue;

		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem==NULL) continue;

		for ( WORD wAreaIndex = ID_HORSE1; wAreaIndex <= ID_HORSE8; wAreaIndex++ )
		{
			if (wAreaIndex == cbWinner)
			{
				m_lUserWinScore[i] += pUserScore[wAreaIndex][i] * cbTime ;
				m_lUserReturnScore[i] += m_lUserScore1[i] ;
				lBankerWinScore -= pUserScore[wAreaIndex][i] * cbTime ;
			} 
			else
			{
				m_lUserWinScore[i] -= pUserScore[wAreaIndex][i];
				m_lUserReturnScore[i] += 0 ;
				lBankerWinScore += pUserScore[wAreaIndex][i];
			}
			
		}	

		////计算税收
		//if (0 < m_lUserWinScore[i])
		//{
		//	float fRevenuePer=float(cbRevenue/1000.+0.000001);
		//	m_lUserRevenue[i]  = LONGLONG(m_lUserWinScore[i]*fRevenuePer);
		//	m_lUserWinScore[i] -= m_lUserRevenue[i];
		//}

		
		//总的分数
		m_lUserWinScore[i] += lUserLostScore[i];
	}

	
	//庄家成绩
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;

		////计算税收
		//if (0 < m_lUserWinScore[m_wCurrentBanker])
		//{
		//	float fRevenuePer=float(cbRevenue/1000.+0.000001);
		//	m_lUserRevenue[m_wCurrentBanker]  = LONGLONG(m_lUserWinScore[m_wCurrentBanker]*fRevenuePer);
		//	m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
		//	lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
		//}		
	}

	//累计积分
	m_lBankerWinScore += lBankerWinScore;

	//当前积分
	m_lBankerCurGameScore=lBankerWinScore;

	return lBankerWinScore;
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

		nIndex = (nIndex+1) % MAX_S_SCORE_HISTORY;
	}
	//if (wBufferSize>0) m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
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

