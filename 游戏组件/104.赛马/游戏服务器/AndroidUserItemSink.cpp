#include "Stdafx.h"
#include "AndroidUserItemSink.h"
#include <math.h>

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
	m_lMeScore2=0L;
	m_lMeScore3=0L;
	m_lMeScore1=0L;
	m_lMeScore6=0L;
	m_lMeScore5=0L;
	m_lMeScore4=0L;
	m_lMeScore7=0L;
	m_lMeScore8=0L;

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
	////个人下注
	//m_lMeScore2=0L;
	//m_lMeScore3=0L;
	//m_lMeScore1=0L;
	//m_lMeScore6=0L;
	//m_lMeScore5=0L;
	//m_lMeScore4=0L;
	//m_lMeScore7=0L;
	//m_lMeScore8=0L;
	return true;
}

//时间消息
bool __cdecl CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_PLACE_JETTON:
		{
			//变量定义
			CMD_C_PlaceJetton PlaceJetton;
			ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

			//CMD_APlaceJetton aPlaceJetton;
			//ZeroMemory(&aPlaceJetton,sizeof(aPlaceJetton));

			BYTE TempTimes = rand()%10 + 1;
			if (m_wCurrentBanker != 0)
			{
				//构造变量
				for (BYTE i=0; i<TempTimes; i++ )
				{
					PlaceJetton.cbJettonArea = rand()%8+1;
					int TemNum = rand()%1000;
					if (TemNum <= 400)
					{
						PlaceJetton.lJettonScore = 0;
						continue;
					}
					else if (TemNum>400 && TemNum<=600)
					{
						PlaceJetton.lJettonScore = 100;
					}
					else if (TemNum>600 && TemNum<=750)
					{
						PlaceJetton.lJettonScore = 1000;
					}
					else if (TemNum>750 && TemNum<=850)
					{
						PlaceJetton.lJettonScore = 10000;
					}
					else if (TemNum>850 && TemNum<=900)
					{
						PlaceJetton.lJettonScore = 50000;
					}
					else if (TemNum>900 && TemNum<=940)
					{
						PlaceJetton.lJettonScore = 100000;
					}
					else if (TemNum>940 && TemNum<=970)
					{
						PlaceJetton.lJettonScore = 500000;
					}
					else if (TemNum>970 && TemNum<=990)
					{
						PlaceJetton.lJettonScore = 1000000;
					}
					else if (TemNum>990 && TemNum<=1000) 
					{
						PlaceJetton.lJettonScore = 5000000;
					}

					//发送下注消息
					m_pIAndroidUserItem->SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
					
				}
			}
			
			return false;
		}
	default:
		{
			break;
		}
	}

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

	//AfxMessageBox("游戏空闲");
	//清理桌面
	for (BYTE nAreaIndex=ID_HORSE1; nAreaIndex<=ID_HORSE8; ++nAreaIndex) 
		SetMePlaceJetton(nAreaIndex,0);
	return true;
}

////个人下注
//void CAndroidUserItemSink::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
//{
//	//效验参数
//	ASSERT(cbViewIndex<=ID_HUANG_MARK);
//	if (cbViewIndex>ID_HUANG_MARK) return;
//
//	if (cbViewIndex==ID_TIAN_MARK) m_lMeTianScore=lJettonCount;
//	else if (cbViewIndex==ID_DI_MARK) m_lMeDiScore=lJettonCount;
//	else if (cbViewIndex==ID_XUAN_MARK) m_lMeXuanScore=lJettonCount;
//	else if (cbViewIndex==ID_HUANG_MARK) m_lMeHuangScore=lJettonCount;
//	else {ASSERT(FALSE);return;}
//}

//个人下注
void CAndroidUserItemSink::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	//效验参数
	ASSERT(cbViewIndex<=ID_HORSE8);
	if (cbViewIndex>ID_HORSE8) return;

	if (cbViewIndex==ID_HORSE1) m_lMeScore1=lJettonCount;
	else if (cbViewIndex==ID_HORSE2) m_lMeScore2=lJettonCount;
	else if (cbViewIndex==ID_HORSE3) m_lMeScore3=lJettonCount;
	else if (cbViewIndex==ID_HORSE4) m_lMeScore4=lJettonCount;
	else if (cbViewIndex==ID_HORSE5) m_lMeScore5=lJettonCount;
	else if (cbViewIndex==ID_HORSE6) m_lMeScore6=lJettonCount;
	else if (cbViewIndex==ID_HORSE7) m_lMeScore7=lJettonCount;
	else if (cbViewIndex==ID_HORSE8) m_lMeScore8=lJettonCount;
	else {ASSERT(FALSE);return;}
}

//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//庄家信息
	SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

	//玩家信息
	m_lMeMaxScore=pGameStart->lUserMaxScore;

	//设置时间
	m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON,0);
	return true;
}

//设置庄家
void CAndroidUserItemSink::SetBankerInfo(WORD wBanker,LONGLONG lScore)
{
	m_wCurrentBanker=wBanker;
	m_lBankerScore=lScore;
	if (m_wCurrentBanker == INVALID_CHAIR)
	{
		WORD bTempNum = rand()%100+1;
		if (m_pIAndroidUserItem->GetChairID()>=(bTempNum-5) && m_pIAndroidUserItem->GetChairID()<=(bTempNum+5))
		{
			m_pIAndroidUserItem->SendSocketData(SUB_C_APPLY_BANKER,NULL,0);
		}
	}
	else
	{
		WORD bTempNum = rand()%100+1;
		if (bTempNum <= 10)
		{
			m_pIAndroidUserItem->SendSocketData(SUB_C_APPLY_BANKER,NULL,0);
		}
	}
}

//切换庄家
bool CAndroidUserItemSink::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	//消息处理
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

	//庄家信
	SetBankerInfo(pChangeBanker->wBankerUser,pChangeBanker->lBankerScore);
	return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	////效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
