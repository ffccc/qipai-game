#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_FREE					99									//空闲时间
#define IDI_PLACE_JETTON			101									//下注时间
#define IDI_DISPATCH_CARD			301									//发牌时间
#define IDI_DEDUCE_BANKER			302

#define TIME_JETTON_ELAPSE			2									//两秒下注一次	
#define TIME_BANKER_ELAPSE			10
#define TIME_BANKER_ELAPSE2			30

//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	m_pIAndroidUserItem = NULL;
	m_iMeMaxScore = 0;
	m_iApplyBankerCondition = 0;
	ZeroMemory(m_iMeAreaScoreArray, sizeof(m_iMeAreaScoreArray));
	ZeroMemory(m_iAreaLimitScore, sizeof(m_iAreaLimitScore));
	ZeroMemory(m_iAreaTotalScore, sizeof(m_iAreaTotalScore));
	m_iBankerScore = 0;
	m_wCurrentBanker = INVALID_CHAIR;
	m_bEnableSysBanker = false;
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
	m_pIAndroidUserItem=GET_OBJECTPTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}

//重置接口
bool __cdecl CAndroidUserItemSink::RepositUserItemSink()
{
	ZeroMemory(m_iMeAreaScoreArray, sizeof(m_iMeAreaScoreArray));
	ZeroMemory(m_iAreaLimitScore, sizeof(m_iAreaLimitScore));
	ZeroMemory(m_iAreaTotalScore, sizeof(m_iAreaTotalScore));
	return true;
}

//时间消息
bool __cdecl CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch(nTimerID)
	{
	case IDI_PLACE_JETTON:
		{
			OnPlaceJetton();
			return false;
		}
	case IDI_DEDUCE_BANKER:
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_DEDUCE_BANKER);
			WORD w = INVALID_CHAIR;
			m_pIAndroidUserItem->SendSocketData(SUB_C_APPLY_BANKER, &w,sizeof(w));
			m_pIAndroidUserItem->SetGameTimer(IDI_DEDUCE_BANKER, TIME_BANKER_ELAPSE2);
			return false;
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
	case SUB_S_PLACE_JETTON:	//用户加注
		{
			return OnSubPlaceJetton(pBuffer,wDataSize);
		}
	case SUB_S_APPLY_BANKER:	//申请做庄
		{
			return true;
		}
	case SUB_S_CANCEL_BANKER:	//取消做庄
		{
			return true;
		}
	case SUB_S_CHANGE_BANKER:	//切换庄家
		{
			return OnSubChangeBanker(pBuffer, wDataSize);
		}
	case SUB_S_GAME_END:		//游戏结束
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_SEND_RECORD:		//游戏记录
		{
			return true;
		}
	case SUB_S_PLACE_JETTON_FAIL:	//下注失败
		{
			return OnSubPlaceJettonFail(pBuffer,wDataSize);
		}
	}

	//错误断言
	//ASSERT(FALSE);

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
	case GS_FREE:			//空闲状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) 
				return false;

			//消息处理
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
			if( pStatusFree == NULL )
				return FALSE;

			//玩家信息
			m_iMeMaxScore=pStatusFree->lUserMaxScore;


			//庄家信息
			SetBankerInfo(pStatusFree->wBankerUser,pStatusFree->lBankerScore);
			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;

			//控制信息
			m_iApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			for( int i=0; i<JETTON_AREA_COUNT; i++ )
				m_iAreaLimitScore[i] = pStatusFree->lAreaLimitScore;

			m_pIAndroidUserItem->SetGameTimer(IDI_DEDUCE_BANKER, TIME_BANKER_ELAPSE);
	
			//设置状态
			m_pIAndroidUserItem->SetGameStatus(GS_FREE);
			return true;
		}
	case GS_PLACE_JETTON:	//下注状态
	case GS_GAME_END:		//结束状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) 
				return false;

			//消息处理
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;
			if( pStatusPlay == NULL )
				return FALSE;

			//全局下注
			for( int i=0; i<JETTON_AREA_COUNT; i++)
				m_iAreaTotalScore[i] = pStatusPlay->lAreaLimitScore;
			LONG  lUserAreaScore[JETTON_AREA_COUNT]={pStatusPlay->lUserBigSharkScore,pStatusPlay->lUserFeiQinScore,pStatusPlay->lUserZouShouScore,
													pStatusPlay->lUserYanZiScore,pStatusPlay->lUserTuZiScore,
													pStatusPlay->lUserGeZiScore,pStatusPlay->lUserXiongMaoScore,
													pStatusPlay->lUserKongQueScore,pStatusPlay->lUserHouZiScore,
													pStatusPlay->lUserLaoYingScore,pStatusPlay->lUserShiZiScore};
									
			//玩家下注
			for( int i=0; i<JETTON_AREA_COUNT; i++ )
				SetMePlaceJetton(i, lUserAreaScore[i]);

			//玩家积分
			m_iMeMaxScore=pStatusPlay->lUserMaxScore;			
		
			//控制信息
			m_iApplyBankerCondition=pStatusPlay->lApplyBankerCondition;
			for( int i=0; i<JETTON_AREA_COUNT; i++ )
				m_iAreaLimitScore[i] = pStatusPlay->lAreaLimitScore;


			//庄家信息
			SetBankerInfo(pStatusPlay->wBankerUser,pStatusPlay->lBankerScore);
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
		
			//设置状态
			m_pIAndroidUserItem->SetGameStatus(pStatusPlay->cbGameStatus);

			//设置时间
			int cbTimeID = 0;
			int  nElapse = 0;
			if( pStatusPlay->cbGameStatus == GS_GAME_END )
			{
				cbTimeID = IDI_DISPATCH_CARD;
				nElapse = pStatusPlay->cbTimeLeave;
			}
			else
			{
				cbTimeID = IDI_PLACE_JETTON;
				nElapse = TIME_JETTON_ELAPSE;
			}
			m_pIAndroidUserItem->SetGameTimer(cbTimeID, nElapse);
			return true;
		}
	}

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

// 所有玩家都开始了
void __cdecl CAndroidUserItemSink::OnEventAllUserStart(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return ;
}


/*****************************************************************************************************************
*游戏特有函数定义开始
*****************************************************************************************************************/
//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) 
		return false;

	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	if( pGameStart == NULL )
		return FALSE;

	ZeroMemory(m_iMeAreaScoreArray, sizeof(m_iMeAreaScoreArray));
	ZeroMemory(m_iAreaLimitScore, sizeof(m_iAreaLimitScore));
	ZeroMemory(m_iAreaTotalScore, sizeof(m_iAreaTotalScore));

	//庄家信息
	SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

	//玩家信息
	m_iMeMaxScore=pGameStart->lUserMaxScore;
		

	//设置各下注区域初始可下分

	//根据庄家金币数量，计算各区域的初始化可下分
	int iOddsArray[JETTON_AREA_COUNT] = {40,5,30,5,20,5,10,5};
	
	for( int i=0; i<0; i++ )
		m_iAreaLimitScore[i] = __min(pGameStart->lBankerScore/(iOddsArray[i]+1), m_iAreaLimitScore[i] );

	//设置时间
	m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON, TIME_JETTON_ELAPSE);

	//设置状态
	m_pIAndroidUserItem->SetGameStatus(GS_PLACE_JETTON);
	return true;
}

//游戏空闲
bool CAndroidUserItemSink::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	m_pIAndroidUserItem->KillGameTimer(IDI_PLACE_JETTON);
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) 
		return false;

	//消息处理
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;
	if( pGameFree == NULL )
		return false;

	//设置状态
	m_pIAndroidUserItem->SetGameStatus(GS_FREE);
	return true;
}

//用户加注成功
bool CAndroidUserItemSink::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) 
		return false;

	//消息处理
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;
	if( pPlaceJetton == NULL )
		return false;
	if( pPlaceJetton->cbJettonArea>ID_SHI_ZI || pPlaceJetton->cbJettonArea<ID_BIG_SHARK )
		return false;

	//修改下注记录数组
	m_iAreaTotalScore[pPlaceJetton->cbJettonArea] += pPlaceJetton->lJettonScore;
	m_iAreaLimitScore[pPlaceJetton->cbJettonArea] -= pPlaceJetton->lJettonScore;

	return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) 
		return false;

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	if( pGameEnd == NULL )
		return FALSE;
	
	m_pIAndroidUserItem->KillGameTimer(IDI_PLACE_JETTON);

	//庄家信息
	m_iBankerScore = pGameEnd->lBankerTotallScore;

	//本人信息
	m_iMeMaxScore += pGameEnd->lUserScore;
	
	//设置状态
	m_pIAndroidUserItem->SetGameStatus(GS_GAME_END);
	return true;
}


//切换庄家
bool CAndroidUserItemSink::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) 
		return false;

	//消息处理
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;
	if( pChangeBanker == NULL )
		return FALSE;


	//庄家信
	SetBankerInfo(pChangeBanker->wBankerUser,pChangeBanker->lBankerScore);

	if( m_iMeMaxScore >= m_iApplyBankerCondition)
	{
		WORD w = INVALID_CHAIR;
		m_pIAndroidUserItem->SendSocketData(SUB_C_APPLY_BANKER, &w, sizeof(WORD));
	}

	return true;
}

//下注失败
bool CAndroidUserItemSink::OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJettonFail));
	if (wDataSize!=sizeof(CMD_S_PlaceJettonFail)) 
		return false;

	//消息处理
	CMD_S_PlaceJettonFail * pPlaceJettonFail=(CMD_S_PlaceJettonFail *)pBuffer;
	if( pPlaceJettonFail == NULL )
		return FALSE;

	//效验参数
	BYTE cbViewIndex=pPlaceJettonFail->lJettonArea;	//取得下注区域
	ASSERT(cbViewIndex<=ID_SHI_ZI);
	if (cbViewIndex>ID_SHI_ZI) 

		return false;

	//下注数量
	LONG iJettonCount=pPlaceJettonFail->lPlaceScore;
	
	m_iMeAreaScoreArray[cbViewIndex] -= iJettonCount;
	return true;
}

//设置庄家
void CAndroidUserItemSink::SetBankerInfo(WORD wBanker,LONG iScore)
{
	m_wCurrentBanker=wBanker;
	m_iBankerScore=iScore;
}

//个人下注
void CAndroidUserItemSink::SetMePlaceJetton(BYTE cbViewIndex, LONG iJettonCount)
{
	//效验参数
	ASSERT(cbViewIndex<= ID_SHI_ZI);
	if (cbViewIndex>ID_SHI_ZI) 
		return;

	m_iMeAreaScoreArray[cbViewIndex] = iJettonCount;
}

//加注消息
void CAndroidUserItemSink::OnPlaceJetton( )
{
	//变量定义
	BYTE cbJettonArea=0xFF;			//下注区域
	bool bCanJettonArray[6]={false,false,false,false,false,false};	//哪些筹码能下注
	LONG iCurJetton = 0;			//本次下注的筹码大小
	int	nCanJettonNum = 0;			//能下注筹码的数量
	int nIndex = 0;					//随机产生的下注筹码数组下标
	LONG iLeaveScore = 0;		//本人还剩多少筹码
	LONG	iTemp = 0;
	int nLoop = 0;

	//计算本人剩余的金币
	for( int i=0; i<JETTON_AREA_COUNT; i++ )
		iTemp += m_iMeAreaScoreArray[i];
	iLeaveScore = m_iMeMaxScore - iTemp;


	//以我当前的最大金币数，我能下多大的筹码
	LONG iScoreJetton[]={100L,1000L,10000L,100000L,1000000L,5000000L};

	//如果是系统做庄，让机器人敞开来下注，否则机器人只会下前三种筹码
	if( m_wCurrentBanker == INVALID_CHAIR && m_bEnableSysBanker )
		nLoop = 6;
	else		
		nLoop = 3;

	for( int i=0; i<nLoop; i++)
	{
		if( iLeaveScore >= iScoreJetton[i] )
		{
			bCanJettonArray[i] = true;
			nCanJettonNum++;
		}
	}

	//随机取得一个筹码
	if(nCanJettonNum <= 0 || nCanJettonNum >nLoop)	//没钱了
	{
		return;
	}
	nIndex = rand()%nCanJettonNum;
	if( ! bCanJettonArray[nIndex])
	{
		return ;
	}
	iCurJetton = iScoreJetton[rand()%nCanJettonNum];

	//随机取得一个下注区域
	cbJettonArea = rand()%JETTON_AREA_COUNT+1;
	if( cbJettonArea>ID_SHI_ZI || cbJettonArea<ID_BIG_SHARK )
	{
		return;
	}

	IServerUserItem * pUserItem = m_pIAndroidUserItem->GetMeUserItem();
	if( pUserItem == NULL )
	{
		return;
	}

	//庄家不能下注
	if ( pUserItem->GetChairID() == m_wCurrentBanker )
	{
		return;
	}

	//根据下注区域，设置本人下注筹码
	m_iMeAreaScoreArray[cbJettonArea] += iCurJetton;

	//变量定义
	CMD_C_PlaceJetton PlaceJetton;
	ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

	//构造变量
	PlaceJetton.cbJettonArea=cbJettonArea;
	PlaceJetton.lJettonScore=iCurJetton;

	//发送网络消息
	m_pIAndroidUserItem->SendSocketData(SUB_C_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));

	if( m_pIAndroidUserItem->GetGameStatus() == GS_PLACE_JETTON)
	{
		int nElapse = rand()%5;
		nElapse = __max(nElapse, 1);
		m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON, nElapse);
	}
	return;
}
/*****************************************************************************************************************
*游戏特有函数定义结束
*****************************************************************************************************************/


//////////////////////////////////////////////////////////////////////////
