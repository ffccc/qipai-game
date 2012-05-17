#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//游戏时间
#define TIME_PLACE_JETTON			10									//出牌时间
#define TIME_GAME_FREE				4									//开始时间
#define TIME_CALL_SCORE				5									//叫分时间

//游戏时间
#define IDI_PLACE_JETTON			(IDI_ANDROID_ITEM_SINK+0)			//开始下注
#define IDI_OPEN_CARD				(IDI_ANDROID_ITEM_SINK+4)			//游戏开牌
#define IDI_GAME_FREE				(IDI_ANDROID_ITEM_SINK+5)			//空闲时间

#define  IDT_DEAL_MSG				1

//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//接口变量
	m_pIAndroidUserItem=NULL;

	m_lApplyBankerCondition=0L;
	m_wCurrentBanker=INVALID_CHAIR;
	m_lCurrentBankerScore=0L;
	m_bMeCurrentBanker=false;
	m_bMeApplyBanker=false;
	m_bAllowApplyBanker=(rand()%2==0) ? true : false;
	m_nBankerTime=0;

	m_lMeMaxScore=0L;
	m_lMeTieScore=0L;
	m_lMeBankerScore=0L;
	m_lMePlayerScore=0L;

	m_lAreaLimitScore=0L;
	m_nBetMoney=0;
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
	m_lMeMaxScore=0L;
	m_lMeTieScore=0L;
	m_lMeBankerScore=0L;
	m_lMePlayerScore=0L;
	m_lAreaLimitScore=0L;
	return true;
}

//时间消息
bool __cdecl CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	if( nTimerID == IDT_DEAL_MSG ) 
	{
		if( m_pIAndroidUserItem->GetGameStatus() == GS_BACCARAT_JETTON )
		{
	
			if( m_wCurrentBanker == INVALID_CHAIR )	return true;

			LONGLONG Gold = m_pIAndroidUserItem->GetMeUserItem()->GetUserScore()->lGameGold;

			if( Gold < m_nLimitBet )
			{
				return true;
			}

			switch( rand()%6 )
			{
			case 0:
				m_nCell=100;
				break;
			case 1:
				m_nCell=1000;
				break;
			case 2:
				m_nCell=10000;
				break;
			case 3:
				m_nCell=100000;
				break;
			case 4:
				m_nCell=1000000;
				break;
			case 5:
				m_nCell=100000;
				break;
			}

			if( m_nCell >= m_nMaxMoney )
			{
				m_nCell = 1000;
			}

			if( Gold>m_nCell )
			{
				CMD_C_PlaceJetton PlaceJetton;
				int i = rand()%14;
				PlaceJetton.cbAnimal = i;
				PlaceJetton.lJettonScore = m_nCell;
				PlaceJetton.cbViewJettonArea = i;

				if( i<=11 )
				{
					int k = i;
					int pos = k%4==0?k:k-1;

					if(i%2==1)
					{
						if(k==3||k==7||k==11)
							pos=k;
						else
							pos=k+1;
					}
					PlaceJetton.cbViewJettonArea = pos;
				}

				m_nBetMoney += m_nCell;
				m_pIAndroidUserItem->SendSocketData( SUB_C_PLACE_JETTON ,&PlaceJetton , sizeof(PlaceJetton) );
			}

			//继续压注
			if ( m_nBetMoney < m_nMinMoney  )
			{
				m_pIAndroidUserItem->SetGameTimer( IDT_DEAL_MSG , (rand()%3 + 1) );
			}
			//中间
			else if( m_nBetMoney > m_nMinMoney && m_nBetMoney <m_nMaxMoney  )
			{
				if( rand()%3 )
					m_pIAndroidUserItem->SetGameTimer( IDT_DEAL_MSG , (rand()%3 + 1) );
			}
			//超过最大下注
			else
			{
				m_nCanBetMoney=0;
				m_nBetArea=-1;
				m_nBetMoney=0;
			}

		}
		if( m_pIAndroidUserItem->GetGameStatus() == GS_BACCARAT_FREE )
		{
		//	srand(time(NULL));
			int i = rand()%2;
			if( i )
			{
				if( m_bIsAllowBanker && m_pIAndroidUserItem->GetChairID() != m_wCurrentBanker )
				{
					CMD_C_ApplyBanker ApplyBanker;
					ApplyBanker.bApplyBanker = true;

					//发送消息
					m_pIAndroidUserItem->SendSocketData(SUB_C_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
					return true;
				}
			}

			if(  m_pIAndroidUserItem->GetChairID() == m_wCurrentBanker )
			{
				CMD_C_ApplyBanker ApplyBanker;
				ApplyBanker.bApplyBanker = false;
				
				if(  m_BankerMinTimes <= m_nBankerTime+1 &&  m_nBankerTime+1 < m_BankerMaxTimes )
				{
					if( i )  m_pIAndroidUserItem->SendSocketData(SUB_C_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
					return true;
				}

				if( m_nBankerTime+1 >= m_BankerMaxTimes )
					m_pIAndroidUserItem->SendSocketData(SUB_C_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
			}

			return true;
		}

	}
	

	return true;
}

//游戏消息
bool __cdecl CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:		//游戏开始
		{
			//设置文件名
			TCHAR szPath[MAX_PATH]=TEXT("");
			TCHAR ConfigFileName[MAX_PATH]=TEXT("");
			GetModuleFileName(AfxGetInstanceHandle(),szPath,MAX_PATH);
			PathRemoveFileSpec(szPath);
			_snprintf(ConfigFileName,sizeof(ConfigFileName),TEXT("%s\\shengxiaoConfig.ini"),szPath);

			TCHAR OutBuf[255];
			memset(OutBuf,0,255);
			GetPrivateProfileString(TEXT("OtherOption"),TEXT("MinBetGold"),"0",OutBuf,255,ConfigFileName);
			m_nMinMoney = _atoi64(OutBuf);
			if ( m_nMinMoney==0 )
			{
				m_nMinMoney = 100000;
			}

			GetPrivateProfileString(TEXT("OtherOption"),TEXT("MaxBetGold"),"0",OutBuf,255,ConfigFileName);
			m_nMaxMoney = _atoi64(OutBuf);
			if ( m_nMaxMoney==0 )
			{
				m_nMaxMoney = 1000000;
			}

			if ( m_nMinMoney >= m_nMaxMoney )
			{
				m_nMinMoney = 100000;
				m_nMaxMoney = 1000000;
			}

			GetPrivateProfileString(TEXT("OtherOption"),TEXT("LimitBetGold"),"0",OutBuf,255,ConfigFileName);
			m_nLimitBet = _atoi64(OutBuf);
			if( m_nLimitBet == 0 )
			{
				m_nLimitBet = 20000;
			}
			m_bIsAllowBanker = GetPrivateProfileInt(TEXT("OtherOption"),TEXT("AllowApplyBanker"),1 ,ConfigFileName);
			m_BankerMinTimes = GetPrivateProfileInt(TEXT("OtherOption"),TEXT("MinBankerTime"),2 ,ConfigFileName);
			m_BankerMaxTimes = GetPrivateProfileInt(TEXT("OtherOption"),TEXT("MaxBankerTime"),4 ,ConfigFileName);

			m_pIAndroidUserItem->SetGameStatus(GS_BACCARAT_JETTON);
			m_pIAndroidUserItem->KillGameTimer(IDI_PLACE_JETTON);

			OnSubGameStart( pData , wDataSize );

			if (m_wCurrentBanker!=m_pIAndroidUserItem->GetChairID())
			{
				m_pIAndroidUserItem->SetGameTimer(IDT_DEAL_MSG, 1 );	
			}

			return true;
		}
	case SUB_S_OPEN_CARD:       //游戏开牌
		{
			return OnSubOpenCard(pData,wDataSize);
		}
	case SUB_S_PLACE_JETTON:	//用户加注
		{
			return OnSubPlaceJetton(pData,wDataSize);
		}
	case SUB_S_APPLY_BANKER:	//申请做庄
		{
			return OnUserApplyBanker(pData, wDataSize);
		}
	case SUB_S_CHANGE_BANKER:	//切换庄家
		{
			return OnChangeBanker(pData, wDataSize);
		}
	case SUB_S_CHANGE_USER_SCORE://更新积分
		{
			return true;
		}
	case SUB_S_FREE:		//游戏结束
		{
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_SEND_RECORD:		//游戏记录
		{
			return true;
		}
	case SUB_S_GAME_SCORE:		//游戏积分
		{
			return true;
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
	switch (cbGameStatus)
	{
	case GS_BACCARAT_FREE:		//空闲状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//消息处理
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			m_wCurrentBanker=pStatusFree->wCurrentBankerChairID;
			m_lCurrentBankerScore=pStatusFree->lBankerTreasure;
			m_nBankerTime=pStatusFree->cbBankerTime;
			m_lAreaLimitScore=pStatusFree->lAreaLimitScore;

			if (bLookonOther==false && m_wCurrentBanker==m_pIAndroidUserItem->GetChairID())
			{
				m_bMeCurrentBanker=true;
				m_bMeApplyBanker=false;
			}

			return true;
		}
	case GS_BACCARAT_JETTON:	//下注状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusJetton));
			if (wDataSize!=sizeof(CMD_S_StatusJetton)) return false;

			//消息处理
			CMD_S_StatusJetton * pStatusJetton=(CMD_S_StatusJetton *)pData;

			//庄家变量
			m_lApplyBankerCondition = pStatusJetton->lApplyBankerCondition;
			m_wCurrentBanker = pStatusJetton->wCurrentBankerChairID;
			m_lCurrentBankerScore = pStatusJetton->lBankerTreasure;

			m_lAreaLimitScore=pStatusJetton->lAreaLimitScore;

			return true;
		}
	case GS_BACCARAT_OPENCARD:     //开牌状态
		{
			ASSERT(wDataSize==sizeof(CMD_S_StatusOpenCard));
			if (wDataSize!=sizeof(CMD_S_StatusOpenCard)) return false;

			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//用户进入
void __cdecl CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
}

//用户离开
void __cdecl CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
}

//用户积分
void __cdecl CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
}

//用户状态
void __cdecl CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
}

//用户段位
void __cdecl CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
}

bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	m_nBankerTime = pGameStart->cbBankerTimes;
	m_wCurrentBanker = pGameStart->wBanker;
	return true;
}

bool CAndroidUserItemSink::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_OpenCard));
	if (wDataSize!=sizeof(CMD_S_OpenCard)) return false;

	//消息处理
	CMD_S_OpenCard * pOpenCard=(CMD_S_OpenCard *)pBuffer;

	m_pIAndroidUserItem->SetGameStatus(GS_BACCARAT_OPENCARD);

	return true;
}

bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	//ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	//if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	////消息处理
	//CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	m_pIAndroidUserItem->SetGameStatus(GS_BACCARAT_FREE);

	//清除下注
	m_lMeTieScore=0L;
	m_lMeBankerScore=0L;
	m_lMePlayerScore=0L;

	//庄家信处
	//if (m_wCurrentBanker!=INVALID_CHAIR)
	//{
	//	m_lCurrentBankerScore=pGameEnd->lBankerTotalScore;
	//	m_lCurrentBankerScore=pGameEnd->lBankerTreasure;
	//	m_nBankerTime=pGameEnd->nBankerTime;
	//}

	//if (m_bMeCurrentBanker==false && m_wCurrentBanker!=INVALID_CHAIR)
	//{
	//	UINT nElapse=rand()%TIME_GAME_FREE;
	//	m_pIAndroidUserItem->SetGameTimer(IDI_GAME_FREE, nElapse);
	//}

	m_pIAndroidUserItem->KillGameTimer(IDT_DEAL_MSG);		
	m_pIAndroidUserItem->SetGameTimer(IDT_DEAL_MSG, 1 );	


	return true;
}

bool CAndroidUserItemSink::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{

	return true;
}


bool CAndroidUserItemSink::OnUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;

	//消息处理
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;
	return true;
}

bool CAndroidUserItemSink::OnChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	//消息处理
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

	//设置信息
	if (m_wCurrentBanker!=INVALID_CHAIR && m_wCurrentBanker==m_pIAndroidUserItem->GetChairID() && 
		pChangeBanker->wChairID!=m_pIAndroidUserItem->GetChairID())
	{
		m_bMeApplyBanker=false;
		m_bMeCurrentBanker=false;
	}
	else if (pChangeBanker->wChairID==m_pIAndroidUserItem->GetChairID())
	{
		m_bMeApplyBanker=false;
		m_bMeCurrentBanker=true;
	}

	//庄家信息
	m_wCurrentBanker = pChangeBanker->wChairID;
	m_nBankerTime = pChangeBanker->cbBankerTime;
	m_lCurrentBankerScore = pChangeBanker->lBankerTreasure;


	return true;
}

BYTE CAndroidUserItemSink::RandJettonArea()
{
	return 0;
}
//////////////////////////////////////////////////////////////////////////
