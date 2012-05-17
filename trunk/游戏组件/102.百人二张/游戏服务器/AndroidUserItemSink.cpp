#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_FREE					99									//空闲时间
#define IDI_PLACE_JETTON			100									//下注时间
#define IDI_DISPATCH_CARD			301									//发牌时间

#define IDI_GAME_ACTION				105									//游戏时间

//////////////////////////////////////////////////////////////////////////

static const LONGLONG lScoreArray[]={100L,1000L,10000L,100000L,1000000L,5000000L};

//////////////////////////////////////////////////////////////////////////


BOOL CAndroidUserItemSink::m_bAllowApplyBanker=TRUE;
WORD CAndroidUserItemSink::m_wMaxBankerTime=0;
INT CAndroidUserItemSink::m_nMaxJettonRange=0;
CString CAndroidUserItemSink::m_strConfigFile;
BOOL CAndroidUserItemSink::m_bLoadConfig=TRUE;

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	m_lAreaLimitScore=0L;
	m_lUserLimitScore=0L;
	m_lApplyBankerCondition=0L;
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	m_lMeMaxScore=0L;
	m_lBankerScore=0L;
	m_wBankerUser=INVALID_CHAIR;
	m_bEnableSysBanker=false;

	m_cbJettonArea=0;

	m_bMeCurrentBanker=false;
	m_bApplyingBanker=false;
	m_bCancelingBanker=false;

	m_wRandBankerTime=0;

	LoadConfig();
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
	m_bMeCurrentBanker=false;
	m_bApplyingBanker=false;
	m_bCancelingBanker=false;

	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
	return true;
}

//时间消息
bool __cdecl CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (m_pIAndroidUserItem->GetGameStatus())
	{
	case GS_FREE:
		{
			if (m_bMeCurrentBanker==false)	//申请做庄
			{
				if (m_bApplyingBanker==true) return false;

				const tagUserScore *pUserScore=m_pIAndroidUserItem->GetMeUserItem()->GetUserScore();
				ASSERT(pUserScore!=NULL);
				if (pUserScore->lScore < m_lApplyBankerCondition) return false;

				m_pIAndroidUserItem->SendSocketData(SUB_C_APPLY_BANKER,NULL,0);
				m_bApplyingBanker=true;
			}
			else if (m_bMeCurrentBanker==true)
			{
				if (m_bCancelingBanker==true) return false;
				m_pIAndroidUserItem->SendSocketData(SUB_C_CANCEL_BANKER,NULL,0);
				m_bCancelingBanker=true;
			}
			return true;
		}
	case GS_PLACE_JETTON:
		{
			if (m_wJettonCount--==0) return false;

			int nMaxIndex=0;
			for (int i=0; i < CountArray(lScoreArray); i++)
				if (m_lMeMaxScore>lScoreArray[i]) nMaxIndex=i;

			//限制索引
			if (nMaxIndex>m_nMaxJettonRange) nMaxIndex=m_nMaxJettonRange;

			LONGLONG lJettonScore=lScoreArray[rand()%nMaxIndex];
			ASSERT(lJettonScore<=m_lMeMaxScore);

			if (lJettonScore>GetUserMaxJetton()) return false;
			TRACE("Jetton Count=%d Area=%d Score=%I64d\n", m_wJettonCount,m_cbJettonArea,lJettonScore);

			//设置变量
			m_lUserJettonScore[m_cbJettonArea]+=lJettonScore;

			//发送消息
			CMD_C_PlaceJetton PlaceJetton;
			PlaceJetton.cbJettonArea=m_cbJettonArea;
			PlaceJetton.lJettonScore=lJettonScore;

			m_pIAndroidUserItem->SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

			UINT nElapse=rand()%4+1;
			m_pIAndroidUserItem->SetGameTimer(IDI_GAME_ACTION,nElapse);

			return true;
		}
	case GS_GAME_END:
		{
			return true;
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
			return OnSubUserApplyBanker(pBuffer, wDataSize);
		}
	case SUB_S_CANCEL_BANKER:	//取消做庄
		{
			return OnSubUserCancelBanker(pBuffer, wDataSize);
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
			return OnSubGameRecord(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON_FAIL:	//下注失败
		{
			return OnSubPlaceJettonFail(pBuffer,wDataSize);
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
	switch(cbGameStatus)
	{
	case GS_FREE:
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//消息处理
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
			m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusFree->lAreaLimitScore;
			m_wBankerUser=pStatusFree->wBankerUser;
			m_wBankerTime=pStatusFree->cbBankerTime;
			m_lBankerScore=pStatusFree->lBankerScore;
			m_lBankerWinScore=pStatusFree->lBankerWinScore;
			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;

			if (bLookonOther==false)
			{
				m_lMeMaxScore=pStatusFree->lUserMaxScore;
			}

			return true;

		}
	case GS_PLAYING:
	case GS_GAME_END:
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//消息处理
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;
			m_lApplyBankerCondition=pStatusPlay->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusPlay->lAreaLimitScore;
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_wBankerTime=pStatusPlay->cbBankerTime;
			m_lBankerScore=pStatusPlay->lBankerScore;
			m_lBankerWinScore=pStatusPlay->lBankerWinScore;
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;

			CopyMemory(m_lAllJettonScore,pStatusPlay->lAllJettonScore,sizeof(m_lAllJettonScore));

			if (bLookonOther==false)
			{
				m_lMeMaxScore=pStatusPlay->lUserMaxScore;
				CopyMemory(m_lUserJettonScore,pStatusPlay->lUserJettonScore,sizeof(m_lUserJettonScore));
			}

			return true;
		}
	}
	return true;
}

bool CAndroidUserItemSink::OnSubGameStart( const void * pBuffer, WORD wDataSize )
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//设置状态
	m_pIAndroidUserItem->SetGameStatus(GS_PLACE_JETTON);

	//设置变量
	m_lMeMaxScore=pGameStart->lUserMaxScore;
	m_wBankerUser=pGameStart->wBankerUser;
	m_lBankerScore=pGameStart->lBankerScore;

	//下注条件
	if (m_lMeMaxScore>2000L && m_wBankerUser!=INVALID_CHAIR && m_wBankerUser!=m_pIAndroidUserItem->GetChairID())
	{
		//随机种子
		static DWORD dwRandCount=0;
		srand((unsigned)time(NULL)+dwRandCount);
		dwRandCount++;

		m_cbJettonArea=rand()%AREA_COUNT+1;
		m_wJettonCount=rand()%10+3;
		
		ASSERT(m_cbJettonArea<=ID_JIAO_R && m_cbJettonArea>=ID_SHUN_MEN);
		TRACE("Android begin jetton area %d count %d........\n", m_cbJettonArea, m_wJettonCount);

		UINT nElapse=rand()%2+1;
		m_pIAndroidUserItem->SetGameTimer(IDI_GAME_ACTION,nElapse);
	}
	

	m_bLoadConfig=TRUE;

	return true;
}

bool CAndroidUserItemSink::OnSubGameFree( const void * pBuffer, WORD wDataSize )
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;

	//消息处理
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;

	m_pIAndroidUserItem->SetGameStatus(GS_FREE);

	IServerUserItem *pServerUserItem=m_pIAndroidUserItem->GetMeUserItem();
	ASSERT(pServerUserItem!=NULL);

	if (m_wBankerUser==INVALID_CHAIR||m_wBankerUser!=pServerUserItem->GetChairID())
		m_bMeCurrentBanker=false;

	
	if (pServerUserItem->GetUserStatus()!=US_LOOKON)
	{
		//上庄条件
		if (m_bApplyingBanker==false && m_bMeCurrentBanker==false && m_bAllowApplyBanker==TRUE && 
			pServerUserItem->GetUserScore()->lScore > m_lApplyBankerCondition)
		{
			m_wRandBankerTime=rand()%m_wMaxBankerTime+1;
			UINT nElapse=rand()%3+1;
			m_pIAndroidUserItem->SetGameTimer(IDI_GAME_ACTION, nElapse);
			TRACE0("Android apply banker........\n");
		}
		else if (m_bMeCurrentBanker==true && m_bCancelingBanker==false)
		{
			if (m_wBankerTime>=m_wRandBankerTime)
			{
				UINT nElapse=rand()%3+1;
				m_pIAndroidUserItem->SetGameTimer(IDI_GAME_ACTION, nElapse);
				TRACE0("Android cancel banker........\n");
			}
		}
	}

	//装入配置
	CAndroidUserItemSink::LoadConfig();

	return true;
}

bool CAndroidUserItemSink::OnSubPlaceJetton( const void * pBuffer, WORD wDataSize )
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) return false;

	//消息处理
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;

	ASSERT(pPlaceJetton->cbJettonArea<=ID_JIAO_R && pPlaceJetton->cbJettonArea>=ID_SHUN_MEN);
	ASSERT(pPlaceJetton->wChairID!=INVALID_CHAIR);
	ASSERT(pPlaceJetton->lJettonScore>=lScoreArray[0] && pPlaceJetton->lJettonScore<=lScoreArray[5]);

	//设置变量
	m_lAllJettonScore[pPlaceJetton->cbJettonArea]+=pPlaceJetton->lJettonScore;

	return true;
}

bool CAndroidUserItemSink::OnSubGameEnd( const void * pBuffer, WORD wDataSize )
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	m_pIAndroidUserItem->SetGameStatus(GS_GAME_END);

	//设置变量
	m_lBankerScore=pGameEnd->lBankerScore;
	m_wBankerTime=pGameEnd->nBankerTime;

	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	return true;

}

bool CAndroidUserItemSink::OnSubUserApplyBanker( const void * pBuffer, WORD wDataSize )
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;

	//消息处理
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;

	if (pApplyBanker->wApplyUser==m_pIAndroidUserItem->GetChairID())
	{
	}

	return true;
}

bool CAndroidUserItemSink::OnSubUserCancelBanker( const void * pBuffer, WORD wDataSize )
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;

	//消息处理
	CMD_S_CancelBanker * pCancelBanker=(CMD_S_CancelBanker *)pBuffer;

	return true;
}

bool CAndroidUserItemSink::OnSubChangeBanker( const void * pBuffer, WORD wDataSize )
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	//消息处理
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
	if (m_wBankerUser==wMeChairID && pChangeBanker->wBankerUser!=wMeChairID)
	{
		m_bMeCurrentBanker=false;
		m_bCancelingBanker=false;
		m_wRandBankerTime=0;
	}
	else if (pChangeBanker->wBankerUser==wMeChairID)
	{
		m_bMeCurrentBanker=true;
		m_bApplyingBanker=false;
	}

	TRACE0("Change banker user........\n");

	m_wBankerUser=pChangeBanker->wBankerUser;
	m_lBankerScore=pChangeBanker->lBankerScore;

	return true;

}

bool CAndroidUserItemSink::OnSubGameRecord( const void * pBuffer, WORD wDataSize )
{
	return true;
}

bool CAndroidUserItemSink::OnSubPlaceJettonFail( const void * pBuffer, WORD wDataSize )
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJettonFail));
	if (wDataSize!=sizeof(CMD_S_PlaceJettonFail)) return false;

	//消息处理
	CMD_S_PlaceJettonFail * pPlaceJettonFail=(CMD_S_PlaceJettonFail *)pBuffer;

	m_pIAndroidUserItem->KillGameTimer(IDI_GAME_ACTION);

	if (pPlaceJettonFail->wPlaceUser==m_pIAndroidUserItem->GetChairID())
	{
		ASSERT(pPlaceJettonFail->lJettonArea<=ID_JIAO_R && pPlaceJettonFail->lJettonArea>=ID_SHUN_MEN);
		m_lUserJettonScore[pPlaceJettonFail->lJettonArea]-=pPlaceJettonFail->lPlaceScore;
	}
	return true;
}

LONGLONG CAndroidUserItemSink::GetUserMaxJetton()
{
	//已下注额
	LONGLONG lNowJetton = 0;
	ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
		lNowJetton += m_lUserJettonScore[nAreaIndex];

	//庄家金币
	LONGLONG lBankerScore=2147483647;
	if (m_wBankerUser!=INVALID_CHAIR) 
		lBankerScore=m_lBankerScore;

	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
		lBankerScore-=m_lAllJettonScore[nAreaIndex];

	//区域限制
	LONGLONG lMeMaxScore=min(m_lMeMaxScore-lNowJetton,m_lAreaLimitScore);

	//庄家限制
	lMeMaxScore=min(lMeMaxScore,lBankerScore);

	//非零限制
	ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return lMeMaxScore;
}

void CAndroidUserItemSink::LoadConfig()
{
	if (m_bLoadConfig)
	{
		if (m_strConfigFile.GetLength()==0)
		{
			TCHAR szPath[MAX_PATH]=TEXT("");
			GetModuleFileName(AfxGetInstanceHandle(),szPath,MAX_PATH);
			PathRemoveFileSpec(szPath);
			m_strConfigFile.Format(TEXT("%s\\RedNine100Config.ini"),szPath);
		}

		m_nMaxJettonRange=GetPrivateProfileInt(TEXT("AndroidUser"),TEXT("JettonRange"),4,m_strConfigFile);
		m_wMaxBankerTime=GetPrivateProfileInt(TEXT("AndroidUser"),TEXT("BankerTime"),8,m_strConfigFile);
		m_bAllowApplyBanker=GetPrivateProfileInt(TEXT("AndroidUser"),TEXT("AllowApplyBanker"),1,m_strConfigFile);
		m_bLoadConfig=FALSE;
	}
}
//////////////////////////////////////////////////////////////////////////
