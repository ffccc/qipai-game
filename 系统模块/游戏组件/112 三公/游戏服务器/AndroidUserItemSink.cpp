#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//辅助时间
#define TIME_LESS					1									//最少时间

//游戏时间
#define TIME_START_GAME				3									//开始时间

//游戏时间
#define IDI_BANKER_START			(IDI_ANDROID_ITEM_SINK+0)			//开始时间
#define IDI_PLAYER_START			(IDI_ANDROID_ITEM_SINK+1)			//下注时间
#define IDI_CHANGE_BANKER			(IDI_ANDROID_ITEM_SINK+2)			//抢庄时间
#define IDI_CONTINUE				(IDI_ANDROID_ITEM_SINK+3)			//继续时间

//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//下注信息
	m_lMaxScore=0L;
	m_lLessScore=0L;

	//游戏变量
	m_wBankerUser=INVALID_CHAIR;

	//辅助变量
	m_bGameScene=false;

	//接口变量
	m_pIAndroidUserItem=NULL;

	return;
}

//析构函数
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//接口查询
void * __cdecl CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//初始接口
bool __cdecl CAndroidUserItemSink::InitUserItemSink(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}

//重置接口
bool __cdecl CAndroidUserItemSink::RepositUserItemSink()
{
	//下注信息
	m_lMaxScore=0L;
	m_lLessScore=0L;

	//游戏变量
	m_wBankerUser=INVALID_CHAIR;

	//辅助变量
	m_bGameScene=false;

	return true;
}

//时间消息
bool __cdecl CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_BANKER_START:		//庄家开始
		{
			ASSERT(m_wBankerUser==m_pIAndroidUserItem->GetChairID());
			if (m_wBankerUser!=m_pIAndroidUserItem->GetChairID()) return true;

			//发送准备
			m_pIAndroidUserItem->SendUserReady(NULL,0);

			return true;
		}
	case IDI_PLAYER_START:		//闲家下注
		{
			ASSERT(m_wBankerUser!=m_pIAndroidUserItem->GetChairID());
			if (m_wBankerUser==m_pIAndroidUserItem->GetChairID()) return true;

			//变量定义
			CMD_C_UserChipin UserChipin;
			ZeroMemory(&UserChipin,sizeof(UserChipin));

			//构造变量
			LONG lMaxTime = m_lMaxScore/m_lLessScore;
			UserChipin.lScore=(rand()%lMaxTime+1)*m_lLessScore;

			//发送消息
			m_pIAndroidUserItem->SendSocketData(SUB_C_USER_CHIPIN,&UserChipin,sizeof(UserChipin));

			return true;
		}
	case IDI_CHANGE_BANKER:		//抢庄时间
		{
			ASSERT(m_wBankerUser!=m_pIAndroidUserItem->GetChairID());
			if (m_wBankerUser==m_pIAndroidUserItem->GetChairID()) return true;

			//构造变量
			CMD_C_ChangeInfo ChangeInfo;
			ZeroMemory(&ChangeInfo,sizeof(ChangeInfo));

			//设置变量
			ChangeInfo.cbChange=TRUE;

			//发送消息
			m_pIAndroidUserItem->SendSocketData(SUB_C_CHANGE_INFO,&ChangeInfo,sizeof(ChangeInfo));

			return true;
		}
	case IDI_CONTINUE:			//继续游戏
		{
			//发送消息
			m_pIAndroidUserItem->SendSocketData(SUB_C_CONTINUE_GAME);

			return true;
		}
	}

	return false;
}

//游戏消息
bool __cdecl CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_TIMER_INFO:		//时间信息
		{
			if (m_bGameScene==false) return true;
			return OnSubTimeInfo(pData,wDataSize);
		}
	case SUB_S_SCORE_INFO:		//下注信息
		{
			if (m_bGameScene==false) return true;
			return OnSubScoreInfo(pData,wDataSize);
		}
	case SUB_S_BANKER_USER:		//庄家信息
		{
			if (m_bGameScene==false) return true;
			return OnSubBankerUser(pData,wDataSize);
		}
	case SUB_S_CHANGE_INFO:		//抢庄信息
		{
			if (m_bGameScene==false) return true;
			return OnSubChangeInfo(pData,wDataSize);
		}
	case SUB_S_GAME_START:		//游戏开始
		{
			if (m_bGameScene==false) return true;
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_USER_CHIPIN:		//用户下注
		{
			if (m_bGameScene==false) return true;
			return OnSubUserChipin(pData,wDataSize);
		}
	case SUB_S_SEND_CARD:		//发送扑克
		{
			if (m_bGameScene==false) return true;
			return OnSubSendCard(pData,wDataSize);
		}
	case SUB_S_GAME_END:		//游戏结束
		{
			if (m_bGameScene==false) return true;
			return OnSubGameEnd(pData,wDataSize);
		}
	}
	//错误断言
	ASSERT(FALSE);

	return true;
}

//游戏消息
bool __cdecl CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	return true;
}

//场景消息
bool __cdecl CAndroidUserItemSink::OnEventGameScene(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_FREE:		//空闲状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//变量定义
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			//辅助变量
			m_bGameScene=true;

			//设置变量
			m_lMaxScore=pStatusFree->lMaxScore;
			m_lLessScore=pStatusFree->lLessScore;
			m_wBankerUser=pStatusFree->wBankerUser;

			//开始设置
			if (pStatusFree->wLeaveTime>0 && m_pIAndroidUserItem->GetChairID()==m_wBankerUser)
			{
				UINT nElapse=rand()%(TIME_START_GAME+3)+TIME_LESS*5;
				//nElapse=9;
				m_pIAndroidUserItem->SetGameTimer(IDI_BANKER_START,nElapse);
			}

			return true;
		}
	case GS_TK_PLAYING:	//游戏状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//变量定义
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//辅助变量
			m_bGameScene=true;

			//设置变量
			m_lMaxScore=pStatusPlay->lMaxScore;
			m_lLessScore=pStatusPlay->lLessScore;
			m_wBankerUser=pStatusPlay->wBankerUser;

			//闲家设置
			if (m_pIAndroidUserItem->GetChairID()!=m_wBankerUser)
			{
				//设置状态
				if ((pStatusPlay->cbSendCard==FALSE)&&(pStatusPlay->lUserChipIn[m_pIAndroidUserItem->GetChairID()]==0))
				{
					//设置下注时间
					UINT nElapse=rand()%(TIME_START_GAME+2)+TIME_LESS*2;
					m_pIAndroidUserItem->SetGameTimer(IDI_PLAYER_START,nElapse);
				}
			}

			//过滤发牌动作,直接发送"发牌完成"
			if (pStatusPlay->cbSendCard==TRUE && pStatusPlay->cbNeedFinish==TRUE)
			{
				m_pIAndroidUserItem->SendSocketData(SUB_C_SEND_FINISH);
			}
			
			return true;
		}
	}
	ASSERT(FALSE);

	return false;
}

//用户进入
void __cdecl CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户离开
void __cdecl CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户积分
void __cdecl CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户状态
void __cdecl CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户段位
void __cdecl CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//时间信息
bool CAndroidUserItemSink::OnSubTimeInfo(const void * pData, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_TimerInfo));
	if (wDataSize!=sizeof(CMD_S_TimerInfo)) return false;

	//消息处理
	CMD_S_TimerInfo * pTimerInfo=(CMD_S_TimerInfo *)pData;

	//设置时间
	if (pTimerInfo->wLeaveTime!=0L && m_pIAndroidUserItem->GetChairID()==m_wBankerUser)
	{
		//设置时间
		UINT nElapse=rand()%(TIME_START_GAME+3)+TIME_LESS*5;
		//nElapse=9;
		m_pIAndroidUserItem->SetGameTimer(IDI_BANKER_START,nElapse);
	}
	else
	{
		//删除时间
		m_pIAndroidUserItem->KillGameTimer(IDI_BANKER_START);
	}

	return true;
}

//下注信息
bool CAndroidUserItemSink::OnSubScoreInfo(const void * pData, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ScoreInfo));
	if (wDataSize!=sizeof(CMD_S_ScoreInfo)) return false;

	//消息处理
	CMD_S_ScoreInfo * pScoreInfo=(CMD_S_ScoreInfo *)pData;

	//设置变量
	m_lMaxScore=pScoreInfo->lMaxScore;
	m_lLessScore=pScoreInfo->lLessScore;

	return true;
}

//庄家信息
bool CAndroidUserItemSink::OnSubBankerUser(const void * pData, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_BankerUser));
	if (wDataSize!=sizeof(CMD_S_BankerUser)) return false;

	//消息处理
	CMD_S_BankerUser * pBankerUser=(CMD_S_BankerUser *)pData;

	//设置变量
	m_wBankerUser=pBankerUser->wBankerUser;

	//删除时间
	m_pIAndroidUserItem->KillGameTimer(IDI_BANKER_START);

	//换庄设置  强性做庄
	if (m_pIAndroidUserItem->GetChairID()==m_wBankerUser)
	{
		//删除时间
		m_pIAndroidUserItem->KillGameTimer(IDI_CHANGE_BANKER);
	}

	return true;
}

//抢庄信息
bool CAndroidUserItemSink::OnSubChangeInfo(const void * pData, WORD wDataSize)
{
	//设置抢庄时间
	UINT nElapse=rand()%(TIME_START_GAME+5)+TIME_LESS*2;
	m_pIAndroidUserItem->SetGameTimer(IDI_CHANGE_BANKER,nElapse);

	return true;
}

//下注信息
bool CAndroidUserItemSink::OnSubUserChipin(const void * pData, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_UserChipin));
	if (wDataSize!=sizeof(CMD_S_UserChipin)) return false;

	return true;
}

//发送扑克
bool CAndroidUserItemSink::OnSubSendCard(const void * pData, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//过滤发牌动作,直接发送"发牌完成"
	m_pIAndroidUserItem->SendSocketData(SUB_C_SEND_FINISH);

	//删除时间
	//m_pIAndroidUserItem->KillGameTimer(0);

	return true;
}

//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(const void * pData, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	CMD_S_GameStart *pGameStart=(CMD_S_GameStart *)pData;

	//设置变量
	m_lMaxScore=pGameStart->lMaxScore;
	m_lLessScore=pGameStart->lLessScore;

	m_pIAndroidUserItem->KillGameTimer(IDI_BANKER_START);

	//设置下注时间
	if (m_pIAndroidUserItem->GetChairID()!=m_wBankerUser)
	{
		UINT nElapse=rand()%(TIME_START_GAME*2)+TIME_LESS*2;
		m_pIAndroidUserItem->SetGameTimer(IDI_PLAYER_START,nElapse);
	}

	return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(const void * pData, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//继续游戏
	UINT nElapse=rand()%(TIME_START_GAME+2)+TIME_LESS*2;
	m_pIAndroidUserItem->SetGameTimer(IDI_CONTINUE,nElapse);

	//删除时间

	return true;
}


//////////////////////////////////////////////////////////////////////////
