#include "Stdafx.h"
#include "AndroidUserItemSink.h"
#include ".\androiduseritemsink.h"

//////////////////////////////////////////////////////////////////////////

//辅助时间
#define TIME_LESS					3									//最少时间
#define TIME_DISPATCH				8									//发牌时间

//游戏时间
#define TIME_OUT_CARD				5									//出牌时间
#define TIME_START_GAME				5									//开始时间
#define TIME_CALL_SCORE				5									//叫分时间

//游戏时间
#define IDI_TIMER_JG				(IDI_ANDROID_ITEM_SINK+0)			//休息时间
#define IDI_START_GAME				(IDI_ANDROID_ITEM_SINK+1)			//开始时间
#define IDI_ADD_JEETOND				(IDI_ANDROID_ITEM_SINK+2)			//叫分时间
#define IDI_TIMER_FREE				(IDI_ANDROID_ITEM_SINK+4)			//出牌时间


#define	IDI_TIMER_JEETTON			18									//下注总时间
#define IDI_TIMER_WAIBAO			6									//外包总时间
#define	IDI_TIMER_SENDCARD			22									//发牌算发时间
#define IDI_TIMER_APPLYBANKER       23                                  //申请坐庄
//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink():
m_cbTurnCardCount(0),
m_first(true)
{
	srand((unsigned)time(0));
	m_wCurrentBanker = INVALID_CHAIR;
	m_lMyMaxStake=0;
	ZeroMemory(m_ShakeType,sizeof(m_ShakeType));
	//const tagUserScore* taUser =m_pIAndroidUserItem->GetMeUserItem()->GetUserScore();
	//m_Money = taUser->lGold;
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
	//扑克变量
	//m_cbTurnCardCount=0;
	//m_bCardTempCount=0;
	//ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
	//m_bTurnOutType=CT_INVALID;

	////手上扑克
	//m_cbHandCardCount=0;
	//ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//memset(m_bCardTempData,0,sizeof(m_bCardTempData));
	//m_wCurrentBanker = INVALID_CHAIR;
	//m_first = true;
	//m_lMyMaxStake=0;
	//ZeroMemory(m_ShakeType,sizeof(m_ShakeType));
	return true;
}

//时间消息
bool __cdecl CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	//switch (nTimerID)
	//{
	//case IDI_START_GAME:	//开始游戏
	//	{
	//		//开始判断
	//		m_pIAndroidUserItem->SendUserReady(NULL,0);

	//		return true;
	//	}
	//case IDI_TIMER_JG:	//休息一秒钟
	//	{			
	//		//const tagUserScore* taUser =m_pIAndroidUserItem->GetMeUserItem()->GetUserScore();
	//		//m_Money = taUser->lGold;
	//		//m_cbTurnCardCount = 0;
	//		////构造变量
	//		//m_pIAndroidUserItem->KillGameTimer(IDI_TIMER_JG);
	//		//m_pIAndroidUserItem->SetGameTimer(IDI_ADD_JEETOND,100);

	//		printf("休息一秒钟\n");

	//		return true;
	//	}
	//case IDI_ADD_JEETOND:		//用户加注
	//	{

	//		//if(m_Money>100000)
	//		//{ 
	//		//	OnAddJeetton();
	//		//}
	//		//m_pIAndroidUserItem->KillGameTimer(IDI_ADD_JEETOND);

	//		//if (++m_cbTurnCardCount < 4)
	//		//{
	//		//	m_pIAndroidUserItem->SetGameTimer(IDI_ADD_JEETOND,100+rand()%4*100);
	//		//}
	//		return true;
	//	}
	//case IDI_TIMER_APPLYBANKER://申请坐庄
	//	{
	//			//m_pIAndroidUserItem->KillGameTimer(IDI_TIMER_APPLYBANKER);
	//			//if (m_wCurrentBanker==INVALID_CHAIR)
	//			//{
	//			//	OnAppyBanker();
	//			//}
	//	}
	//	//case IDI_SEND_OUT_CARD:		//用户出牌
	//	//	{
	//	//		//AutoOutCard(true);
	//	//	}
	//	return true;
	//}

	return false;
}
int CAndroidUserItemSink::AutoOutCard(bool bOutCard)
{

	return 0;
}

//游戏消息
bool __cdecl CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	//case SUB_S_TIME_MODE:  //时间状态
	//	{
	//		return OnSubTimerType(pBuffer,wDataSize);
	//	}
	case SUB_S_GAME_START:		//游戏开始
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//游戏结束
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON:	//用户加注
		{
			return OnSubPlaceJetton(pBuffer,wDataSize);
		}
	case SUB_S_APPLY_BANKER:	//申请做庄
		{
			return OnSubApplyBanker( pBuffer, wDataSize );
		}
	case SUB_S_CHANGE_BANKER:	//切换庄家
		{
			return OnSubChangeBanker( pBuffer, wDataSize );
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
bool __cdecl CAndroidUserItemSink::OnEventGameScene(BYTE cbGameStatus, bool bLookonOther, void * pBuffer, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_FREE:			//空闲状态
		{


			return true;
		}
	case GS_PLAYING:		//游戏状态
		{
			return true;
		}
	}

	return false;
}

//用户进入
void __cdecl CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
#ifdef _DEBUG
	TRACE( pIAndroidUserItem->GetMeUserItem()->GetAccounts());
#endif
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

//切换庄家
bool CAndroidUserItemSink::OnSubChangeBanker(void * pData, WORD wDataSize)
{
	//效验数据
	//ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	//if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	////消息处理
	//CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pData;

	//m_wCurrentBanker = pChangeBanker->wChairID;

	//OnAppyBanker();


	return true;
}
//时间状态
bool CAndroidUserItemSink::OnSubChangeCard(void * pData, WORD wDataSize)
{
	return true;
}
//时间状态
bool CAndroidUserItemSink::OnSubTimerType(void * pData, WORD wDataSize)
{
	//效验数据
	//ASSERT(wDataSize==sizeof(CMD_S_GameTimeMode));
	//if (wDataSize!=sizeof(CMD_S_GameTimeMode)) return false;
	////消息处理
	//CMD_S_GameTimeMode * pTimerType=(CMD_S_GameTimeMode *)pData;
	//switch(pTimerType->TimeMode)
	//{
	//case 1: //空闲时间
	//	{
	//		if(m_wCurrentBanker==m_pIAndroidUserItem->GetChairID())
	//		{
	//			CMD_C_ShakeType ShakeType;
	//			ZeroMemory(&ShakeType,sizeof(ShakeType));
	//			ZeroMemory(m_ShakeType,sizeof(m_ShakeType));
	//			int nTpye=0;
	//			int nShakeType=rand()%3;
	//			if(nTpye==0)
	//			{
	//				m_ShakeType[0]=true;
	//				ShakeType.nShakeDiceType=1;
	//			}
	//			else if(nTpye==1)
	//			{
	//				m_ShakeType[1]=true;
	//				ShakeType.nShakeDiceType=2;
	//			}
	//			else
	//			{
	//				m_ShakeType[2]=true;
	//				ShakeType.nShakeDiceType=3;
	//			}
	//			m_pIAndroidUserItem->SendSocketData(SUB_C_SHAKETYPE,&ShakeType,sizeof(ShakeType));
	//		}
	//		break;
	//	}
	//case 2://下注时间
	//	{
	//		m_pIAndroidUserItem->SetGameTimer(IDI_TIMER_JG,100/*+rand()%4*/);//休息一秒钟后下注
	//		break;
	//	}
	//case 3://摇骰子算分时间
	//	{
	//		if(m_wCurrentBanker==m_pIAndroidUserItem->GetChairID())
	//		{
	//			Sleep(1);
	//			CMD_C_ShakeDice   ShakeDice;
	//			ZeroMemory(&ShakeDice,sizeof(ShakeDice));
	//			CopyMemory(ShakeDice.bDrawDiceAnimation,m_ShakeType,sizeof(ShakeDice.bDrawDiceAnimation));
	//			m_pIAndroidUserItem->SendSocketData(SUB_C_SHAKEDICE,&ShakeDice,sizeof(ShakeDice));
	//		}
	//		break;
	//	}
	//default:
	//	break;
	//}

	return true;
}

//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(void * pData, WORD wDataSize)
{


	return true;
}

//用户加注
bool CAndroidUserItemSink::OnSubPlaceJetton(void * pData, WORD wDataSize)
{

	return true;
}

//申请做庄
bool CAndroidUserItemSink::OnSubApplyBanker(void * pData, WORD wDataSize)
{


	return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(void * pData, WORD wDataSize)
{


	return true;
}


void CAndroidUserItemSink::OnAddJeetton(void)
{

}

// 自己申请坐庄
void CAndroidUserItemSink::OnAppyBanker(void)
{
	
	return;	
}
