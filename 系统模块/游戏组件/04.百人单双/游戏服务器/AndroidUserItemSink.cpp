#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_FREE					99									//空闲时间
#define IDI_PLACE_JETTON			100									//下注时间
#define IDI_DISPATCH_CARD			301									//发牌时间


//时间标识
#define IDI_PLACE_JETTON			100									//下注时间

#ifdef _DEBUG
#define TIME_PLACE_JETTON			1									//下注时间
#define LESS_TIME					2									//下注时间
#else 
#define TIME_PLACE_JETTON			5									//下注时间
#define LESS_TIME					2									//下注时间
#endif


//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
WORD CAndroidUserItemSink::m_wAndroidApplyBankerCount = 0;			//机器人申请上庄数量
BOOL CAndroidUserItemSink::m_blApply = false;
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
	m_nMeBankerCount=0;
	m_nMePlayerCount=0;
	m_nApplyUserCount=0;
	//m_AppyBankerAdroidUserArray.RemoveAll();

	//状态变量
	m_bMeApplyBanker=false;
	m_bCancelBanker=false;

	m_blInUserArray = false;


	//设置文件名
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_snprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\OddEvenBattleConfig.ini"),szPath);

	
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
	switch (nTimerID)
	{
	case IDI_PLACE_JETTON:
		{
			if (m_wCurrentBanker!=m_pIAndroidUserItem->GetChairID())
			{
				//变量定义
				CMD_C_PlaceJetton PlaceJetton;
				ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));
				//构造变量
				PlaceJetton.cbJettonArea=(rand()%2)+1;
				
				LONG lPlaceScore[]={100L,1000L,10000L,100000L,500000L,1000000L,5000000L};
				int iCount = 0;

				TCHAR Session[255];
				memset(Session,0,255);
				_snprintf(Session,sizeof(Session),TEXT("Jetton%d"),m_wServerID);

				LONG Lmin = GetPrivateProfileInt(Session, TEXT("Min"), 10, m_szConfigFileName); 
				LONG Lmax = GetPrivateProfileInt(Session, TEXT("Max"), 10, m_szConfigFileName); 
				if(Lmax<Lmin||Lmin<100L||Lmax>5000000L)
				{
					m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON,rand()%TIME_PLACE_JETTON+LESS_TIME);
					return false;
				}
				INT nIndex = 0;//CountArray(lPlaceScore)-1;
				for (int n = 0;n<200;n++){

					nIndex=rand()%CountArray(lPlaceScore);
					if(Lmin<=lPlaceScore[nIndex] && lPlaceScore[nIndex]<=Lmax)
					{
						break;
					}
				}
				if (lPlaceScore[nIndex]<=m_pIAndroidUserItem->GetMeUserItem()->GetUserScore()->lScore)
					PlaceJetton.lJettonScore=lPlaceScore[nIndex];
				else
				{
					PlaceJetton.lJettonScore=0;
					return false;
				}
			
				//发送消息
				m_pIAndroidUserItem->SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

				//设置时间
				m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON,rand()%TIME_PLACE_JETTON+LESS_TIME);

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
	case SUB_S_APPLY_BANKER:	//申请做庄
		{
			return OnSubUserApplyBanker(pBuffer, wDataSize);
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


	return true;
}


//个人下注
void CAndroidUserItemSink::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	
}

//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	SetBankerInfo(pGameStart->wCurBankerID,100);
	m_wServerID = pGameStart->wServerID;


	TCHAR Session[255];
	memset(Session,0,255);
	_snprintf(Session,sizeof(Session),TEXT("BankerCondition%d"),m_wServerID);


	//设置时间
	m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON,rand()%pGameStart->cbGameTime+1);

	BYTE cbEnableAndroidUserBanker=GetPrivateProfileInt(Session, TEXT("EnableAndroidUserBanker"), 1, m_szConfigFileName);
	m_bEnableAndroidUserBanker=cbEnableAndroidUserBanker?true:false;
	m_nAndroidUserBankerCount=GetPrivateProfileInt(Session, TEXT("AndroidUserBankerCount"), 1, m_szConfigFileName);
	m_nAndroidUserPlayerCount=GetPrivateProfileInt(Session, TEXT("AndroidUserPlayerCount"), 2, m_szConfigFileName);
	m_nMinApplyBankerCount=GetPrivateProfileInt(Session, TEXT("MinApplyBankerCount"), 1, m_szConfigFileName);
	m_lApplyBankerCondition=GetPrivateProfileInt(Session, TEXT("Score"), 100, m_szConfigFileName);


	return true;
}

//设置庄家
void CAndroidUserItemSink::SetBankerInfo(WORD wBanker,LONGLONG lScore)
{
	m_wCurrentBanker=wBanker;
	//m_lBankerScore=lScore;
}



//切换庄家
bool CAndroidUserItemSink::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	//消息处理
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

	return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;



	return true;
}


//申请做庄
bool CAndroidUserItemSink::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;

	CMD_S_ApplyBanker *pApplyBanker  = (CMD_S_ApplyBanker *)pBuffer;

	
	return true;
}

//取消做庄
bool CAndroidUserItemSink::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	
	return true;
}
//////////////////////////////////////////////////////////////////////////
