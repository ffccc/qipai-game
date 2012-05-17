#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_FREE					99									//空闲时间
#define IDI_PLACE_JETTON			100									//下注时间
#define IDI_DISPATCH_CARD			301									//发牌时间

//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//限制信息
	m_lMeMaxScore=0L;			
	m_lAreaLimitScore=0L;		
	m_lApplyBankerCondition=0L;	

	//个人下注
	m_lMeTieScore=0L;
	m_lMeBankerScore=0L;
	m_lMePlayerScore=0L;
	m_lMeTieSamePointScore=0L;
	m_lMePlayerKingScore=0L;
	m_lMeBankerKingScore=0L;
	m_lMePlayerTwoPair=0L;
	m_lMeBankerTwoPair=0L;

	//庄家信息
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;

	//状态变量
	m_bMeApplyBanker=false;

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

	return true;
}

//时间消息
bool __cdecl CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	//switch (nTimerID)
	//{
	//case IDI_PLACE_JETTON:
	//	{
	//		//变量定义
	//		CMD_C_PlaceJetton PlaceJetton;
	//		ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

	//		//构造变量
	//		PlaceJetton.cbJettonArea=(rand()%2)==0?ID_XIAN_JIA:ID_ZHUANG_JIA;
	//		PlaceJetton.lJettonScore=100;

	//		//发送消息
	//		m_pIAndroidUserItem->SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

	//		return false;
	//	}
	//default:
	//	{
	//		break;
	//	}
	//}

	return false;
}

//游戏消息
bool __cdecl CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_FREE:		//游戏空闲
		{
			return OnSubGameFree(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:		//游戏开始
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_CHANGE_BANKER:	//切换庄家
		{
			return OnSubChangeBanker(pBuffer, wDataSize);
		}
	case SUB_S_GAME_END:		//游戏结束
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	}

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

	return true;
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


//游戏空闲
bool CAndroidUserItemSink::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;

	//清理桌面
//	for (int nAreaIndex=ID_XIAN_JIA; nAreaIndex<=ID_BANKER_TWO_PAIR; ++nAreaIndex) SetMePlaceJetton(nAreaIndex,0);

	return true;
}


//个人下注
void CAndroidUserItemSink::SetMePlaceJetton(BYTE cbViewIndex, LONG lJettonCount)
{
	//效验参数
	//ASSERT(cbViewIndex<=ID_BANKER_TWO_PAIR);
	//if (cbViewIndex>ID_BANKER_TWO_PAIR) return;

	//if (cbViewIndex==ID_XIAN_JIA) m_lMePlayerScore=lJettonCount;
	//else if (cbViewIndex==ID_PING_JIA) m_lMeTieScore=lJettonCount;
	//else if (cbViewIndex==ID_ZHUANG_JIA) m_lMeBankerScore=lJettonCount;
	//else if (cbViewIndex==ID_XIAN_TIAN_WANG) m_lMePlayerKingScore=lJettonCount;
	//else if (cbViewIndex==ID_ZHUANG_TIAN_WANG) m_lMeBankerKingScore=lJettonCount;
	//else if (cbViewIndex==ID_TONG_DIAN_PING) m_lMeTieSamePointScore=lJettonCount;
	//else if (cbViewIndex==ID_BANKER_TWO_PAIR) m_lMeBankerTwoPair=lJettonCount;
	//else if (cbViewIndex==ID_PLAYER_TWO_PAIR) m_lMePlayerTwoPair=lJettonCount;
	//else {ASSERT(FALSE);return;}
}

//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	////效验数据
	//ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	//if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	////消息处理
	//CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	////庄家信息
	//SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

	////玩家信息
	//m_lMeMaxScore=pGameStart->lUserMaxScore;

	////设置时间
	//m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON,rand()%pGameStart->cbTimeLeave+1);

	return true;
}

//设置庄家
void CAndroidUserItemSink::SetBankerInfo(WORD wBanker,LONG lScore)
{
	//m_wCurrentBanker=wBanker;
	//m_lBankerScore=lScore;
}



//切换庄家
bool CAndroidUserItemSink::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	//ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	//if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	////消息处理
	//CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

	////庄家信
	//SetBankerInfo(pChangeBanker->wBankerUser,pChangeBanker->lBankerScore);

	return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	////效验数据
	//ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	//if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////
