#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//辅助时间
#define TIME_LESS					1									//最少时间
#define TIME_LESS_CHANGE_BULLET		60									//交换子弹的最少时间

//游戏时间
#define TIME_BUY_BULLET				3									//买子弹时间
#define TIME_FIRE					2									//发射子弹时间
#define TIME_CHANGE_BULLET			120									//交换子弹时间

////游戏时间
#define IDI_BUY_BULLET				(IDI_ANDROID_ITEM_SINK+0)			//买子弹定时器
#define IDI_FIRE					(IDI_ANDROID_ITEM_SINK+1)			//发射子弹定时器
#define IDI_CHANGE_BULLET			(IDI_ANDROID_ITEM_SINK+2)			//交换子弹定时器
#define IDI_CHANGE_BULLET_ONCE		(IDI_ANDROID_ITEM_SINK+3)			//交换子弹定时器

//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//接口变量
	m_pIAndroidUserItem=NULL;

	srand((UINT)time(NULL));

	//m_bType = rand()%3;

	m_nFishGold = 0;
	m_cbScene = 0;
	m_lCellScore = 0;
	m_CannonType = 0;
	m_SwapCannonType = 0;
	m_fLastFireRote = 0.0f;
	
    for (int i=0; i<GAME_PLAYER; i++)
    {
        m_RoleObjects[i].wID = INVALID_CHAIR;
        m_RoleObjects[i].wFishGold = 0;
        m_RoleObjects[i].wCannonType = 0;
    }

    //for (int i=0; i<MAX_FISH_OBJECT; i++)
    //{
    //    m_FishObjects[i].wID = INVALID_WORD;
    //    m_FishObjects[i].wRoundID= 0;
    //    m_FishObjects[i].wType = 0;
    //    m_FishObjects[i].dwTime = 0;
    //}

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
	//m_pIAndroidUserItem=GET_OBJECTPTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	srand((UINT)time(NULL));

	return true;
}

//重置接口
bool __cdecl CAndroidUserItemSink::RepositUserItemSink()
{
	m_nFishGold = 0;
	m_fLastFireRote = 0.0f;

	return true;
}

//时间消息
bool __cdecl CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_BUY_BULLET:			//买子弹
		{
			BuyBullet();

			return true;
		}
	case IDI_CHANGE_BULLET:			//切换子弹
		{
			ChangeCannon();

			return true;
		}
	case IDI_CHANGE_BULLET_ONCE:	//切换子弹(一次)
		{
			ChangeCannonOnce();

			return true;
		}
	case IDI_FIRE:					//发射子弹
		{
			Fire();

			return true;
		}
	}

	return true;
}

//游戏消息
bool __cdecl CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	switch(wSubCmdID)
	{
	case SUB_S_BUY_BULLET_SUCCESS:					//子弹购买成功消息
		{
			ASSERT(wDataSize == sizeof(CMD_S_Buy_Bullet_Success));
			if (wDataSize!=sizeof(CMD_S_Buy_Bullet_Success)) return false;

			CMD_S_Buy_Bullet_Success * pBuyBulletSuccess = (CMD_S_Buy_Bullet_Success *)pData;
			
			if (pBuyBulletSuccess->wChairID == m_pIAndroidUserItem->GetChairID())
			{
				m_nFishGold += pBuyBulletSuccess->wCount;

				/// 切换子弹
				if (rand()%100 < 50)
					m_pIAndroidUserItem->SetGameTimer(IDI_CHANGE_BULLET, rand()%TIME_CHANGE_BULLET+TIME_LESS_CHANGE_BULLET);
				else
					ChangeCannon();

				m_pIAndroidUserItem->SetGameTimer(IDI_FIRE, rand()%TIME_FIRE+TIME_LESS);
			}
			
			return true;
		}
	case SUB_S_BUY_BULLET_FAILED:					//子弹购买失败消息
		{
			//ASSERT(wDataSize == sizeof(CMD_S_Buy_Bullet_Failed));
			//if (wDataSize!=sizeof(CMD_S_Buy_Bullet_Failed)) return false;

			//CMD_S_Buy_Bullet_Failed * pBuyBulletFailed = (CMD_S_Buy_Bullet_Failed *)pData;

			return true;
		}
	case SUB_S_CHANGE_CANNON:						//子弹切换消息
		{
			ASSERT(wDataSize == sizeof(CMD_S_Change_Cannon));
			if (wDataSize!=sizeof(CMD_S_Change_Cannon)) return false;

			CMD_S_Change_Cannon *pChangeCannon = (CMD_S_Change_Cannon *)pData;

			ASSERT(pChangeCannon->wStyle < MAX_CANNON_STYLE);

			if (pChangeCannon->wChairID == m_pIAndroidUserItem->GetChairID())
			{
				m_CannonType = pChangeCannon->wStyle;
			}

			return true;
		}
	case SUB_S_FIRE_SUCCESS:						//子弹发射成功
		{			
			ASSERT(wDataSize == sizeof(CMD_S_Fire_Success));
			if (wDataSize!=sizeof(CMD_S_Fire_Success)) return false;

			//CMD_S_Fire_Success *pFireSuccess = (CMD_S_Fire_Success *)pBuffer;

			m_nFishGold -= (m_CannonType+1)*m_lCellScore;

			if (m_nFishGold < 0) m_nFishGold = 0;


			//WORD wChairID = pFireSuccess->wChairID;
			//float fRote = pFireSuccess->fRote;

			//if (wChairID != GetMeChairID())
			//{
			//	m_layRoles[wChairID]->SetCannonRatation(fRote);
			//	m_layBulletObject->BulletFire(m_layRoles[wChairID]->GetCannonPosition(), fRote, wChairID, m_layRoles[wChairID]->GetConnonType());
			//	m_layRoles[wChairID]->GunAnimation();
			//	m_layRoles[wChairID]->GunBaseAnimation();

			//	WORD wFishGold = m_layRoles[wChairID]->GetConnonType()+1;
			//	if (wFishGold >= m_layRoles[wChairID]->GetFishGold())
			//	{
			//		wFishGold=0;
			//	}
			//	else
			//	{
			//		wFishGold = m_layRoles[wChairID]->GetFishGold() - wFishGold;
			//	}
			//	if (wFishGold <= 10)
			//	{
			//		m_layRoles[wChairID]->ShowFishGoldEmpty(true);
			//	}
			//	else
			//	{
			//		m_layRoles[wChairID]->ShowFishGoldEmpty(false);
			//	}

			//	m_layRoles[wChairID]->SetFishGold(wFishGold);

			//}

			return true;
		}
	case SUB_S_CAST_NET_SUCCESS:
		{
			CMD_S_Cast_Net_Success *pCastNetSuccess = (CMD_S_Cast_Net_Success *)pData;

			ASSERT(wDataSize == sizeof(CMD_S_Cast_Net_Success)-(MAX_FISH_IN_NET-pCastNetSuccess->cbCount)*sizeof(Fish_Net_Object));
			if (wDataSize!=sizeof(CMD_S_Cast_Net_Success)-(MAX_FISH_IN_NET-pCastNetSuccess->cbCount)*sizeof(Fish_Net_Object)) return false;
			
			if (pCastNetSuccess->wChairID == m_pIAndroidUserItem->GetChairID())
			{
				int nCoinCount = 0;

				for (BYTE i=0; i<pCastNetSuccess->cbCount; i++)
				{
					nCoinCount += m_GameLogic.FishGoldByStyle(pCastNetSuccess->FishNetObjects[i].wType);
				}

				if (nCoinCount>0)
					m_nFishGold += nCoinCount;
			}

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
	case GS_FREE:				//空闲状态
		{
			ASSERT(wDataSize == sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree = (CMD_S_StatusFree *)pData;

			m_cbScene = pStatusFree->cbScene;
			m_lCellScore = pStatusFree->lCellScore;
			
            CopyMemory(m_RoleObjects, pStatusFree->RoleObjects, GAME_PLAYER*sizeof(Role_Net_Object));

			IServerUserItem * pIServerUserItem=m_pIAndroidUserItem->GetMeUserItem();

			if (pIServerUserItem == NULL) return true;

			//玩家设置
			if (pIServerUserItem->GetUserStatus() == US_SIT)
			{
				UINT nElapse=rand()%TIME_BUY_BULLET+TIME_LESS*3;
				m_pIAndroidUserItem->SetGameTimer(IDI_BUY_BULLET,nElapse);
			}
			
			return true;
		}
	case GS_PLAYING:			//游戏状态
		{
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

/// 购买子弹
void CAndroidUserItemSink::BuyBullet()
{
	ASSERT(m_nFishGold <= 0);
	if (m_nFishGold > 0) return;

	int nBulletCount = 0;

	if (m_nFishGold == 0)
		nBulletCount = (rand()%BULLET_MAX_COUNT+1)*100;
	else
		return;

	CMD_C_Buy_Bullet BuyBullet;

	BuyBullet.wCount = nBulletCount;

	//发送数据
	m_pIAndroidUserItem->SendSocketData(SUB_C_BUY_BULLET,&BuyBullet,sizeof(BuyBullet));
}

/// 切换子弹
void CAndroidUserItemSink::ChangeCannon()
{
	//enum enCannonType
	//{
	//	CannonType_0 = 0,
	//	CannonType_1,
	//	CannonType_2,
	//	CannonType_3,
	//	CannonType_4,
	//	CannonType_5,
	//	CannonType_6,
	//	CannonTypeCount
	//};

	//切换子弹
	m_SwapCannonType = rand()%MAX_CANNON_STYLE;

	if (m_CannonType != m_SwapCannonType)
	{
		ChangeCannonOnce();
	}
}

/// 切换子弹
void CAndroidUserItemSink::ChangeCannonOnce()
{
	if (m_CannonType != m_SwapCannonType)
	{
		m_CannonType = (m_CannonType+1)%MAX_CANNON_STYLE;

		ASSERT(m_CannonType < MAX_CANNON_STYLE);

		//切换子弹
		CMD_C_Change_Cannon ChangeCannon;
		ChangeCannon.wStyle = m_CannonType;

		//发送数据
		m_pIAndroidUserItem->SendSocketData(SUB_C_CHANGE_CANNON,&ChangeCannon,sizeof(ChangeCannon));

		if (m_CannonType != m_SwapCannonType)
		{
			m_pIAndroidUserItem->SetGameTimer(IDI_CHANGE_BULLET_ONCE, 1);
		}
	}
}

/// 发射子弹
void CAndroidUserItemSink::Fire()
{
	if (m_nFishGold > 0)
	{		
		CMD_C_Fire Fire;
		
		const float fFireRote[] = {/* 4.7f, */-1.36f, -1.00f, -0.72f, -0.52f, -0.25f, -0.11f, 0.00f, 0.30f, 0.60f, 0.90f, 1.20f, 1.37f/*1.57f*/};
		int index = 0;

		for (int i = 0; i < CountArray(fFireRote); ++i)
		{
			if (m_fLastFireRote == fFireRote[i])
			{
				index = i;
				break;
			}
		}

		/// 取邻近范围内的五个值 -2,-1,0,1,2
		int n = rand()%5-2;

		if (index + n < 0)
		{
			index = rand()%5;
		}
		else if (index + n >= CountArray(fFireRote))
		{
			index = CountArray(fFireRote) - rand()%5 - 1;
		}
		else
		{
			index = index + n;
		}

		Fire.fRote = fFireRote[index];

		WORD wChairID = m_pIAndroidUserItem->GetChairID();

		if (wChairID == 0 || wChairID == 1)
			Fire.fRote += 180;

		m_fLastFireRote = Fire.fRote;

		m_pIAndroidUserItem->SendSocketData(SUB_C_FIRE, &Fire, sizeof(CMD_C_Fire));

		m_pIAndroidUserItem->SetGameTimer(IDI_FIRE, rand()%TIME_FIRE+TIME_LESS);
	}
	else
	{
		/// 购买子弹
		BuyBullet();
	}
}
//////////////////////////////////////////////////////////////////////////