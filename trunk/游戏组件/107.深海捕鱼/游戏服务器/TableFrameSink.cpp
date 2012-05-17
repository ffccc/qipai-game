#include "StdAfx.h"
#include "TableFrameSink.h"

#include <fstream>

//////////////////////////////////////////////////////////////////////////
void Log(const std::string error)
{
	std::ofstream ofs("fishsever.txt", std::ios::app);
	ofs << error << std::endl;
}
//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//游戏人数
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_TimeControl;	//开始模式

//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{
    //组件变量
    m_pITableFrame=NULL;
    m_pGameServiceOption=NULL;
    m_pITableFrameControl=NULL;
    m_pITableFrameManager=NULL;

    //logic
    m_cbScene = 0;
    m_dwSceneStartTime = 0;
    m_wSceneSendFishCount = 0;
	m_dwProbability = GAME_PROBABILITY;
	m_wAndroidLogicUserID = 0;
	m_wAndroidLogicChairID = INVALID_CHAIR;

    for (int i=0; i<GAME_PLAYER; i++)
    {
        m_RoleObjects[i].wID = INVALID_CHAIR;
        m_RoleObjects[i].wFishGold = 0;
        m_RoleObjects[i].wCannonType = 0;
        m_RoleObjects[i].wFireCount = 0;
    }

    for (int i=0; i<MAX_FISH_OBJECT; i++)
    {
        m_FishObjects[i].wID = INVALID_WORD;
        m_FishObjects[i].wRoundID= 0;
        m_FishObjects[i].wType = 0;
        m_FishObjects[i].dwTime = 0;
    }

    for (int i=0; i<GAME_PLAYER; i++)
    {
        m_nFireCount[i] = 0;
		m_bTaskSended[i] = false;
        m_wCookFishCount[i] = 0;
    }

    LoadLinePaths();

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

    //框架管理接口
    m_pITableFrameManager=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrameManager);
    if (m_pITableFrameManager==NULL) return false;

    //获取参数
    m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
    ASSERT(m_pGameServiceOption!=NULL);

    return true;
}

//复位桌子
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
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
    return true;
}

//游戏结束
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
    switch (cbReason)
    {
    case GER_NORMAL:		//常规结束
        {
            //结束游戏
            m_pITableFrame->ConcludeGame();

            return true;
        }
    case GER_USER_LEFT:		//用户离开
        {
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
            CMD_S_StatusFree StatusFree;

            StatusFree.cbScene = m_cbScene;
            StatusFree.lCellScore = m_pGameServiceOption->lCellScore;
            CopyMemory(StatusFree.RoleObjects, m_RoleObjects, GAME_PLAYER*sizeof(Role_Net_Object));

            m_pITableFrame->SendGameScene(pIServerUserItem, &StatusFree, sizeof(StatusFree));

            return true;
        }
    }

    return false;
}

//定时器事件
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{     
    switch (wTimerID)
    {
    case IDI_SCENE_START:
        {
            m_dwSceneStartTime=(DWORD)time(NULL);

            if (!SendNormalFishObject(true))
			{
				Log("CTableFrameSink::OnTimerMessage IDI_SCENE_START SendNormalFishObject()");
                return false;
			}

            m_pITableFrame->SetGameTimer(IDI_NORMAL_ADD_FISH_START,TIME_NORMAL_ADD_FISH_START,1,0L);

            break;
        }
    case IDI_SCENE_END:
        {
            m_cbScene ++;
            m_cbScene %= MAX_SCENE;

            CMD_S_Change_Scene ChangeScene;

            ChangeScene.cbScene = m_cbScene;
            m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CHANGE_SCENE, &ChangeScene, sizeof(CMD_S_Change_Scene));

            m_pITableFrame->SetGameTimer(IDI_SCENE_ADD_FISH_START,TIME_SCENE_ADD_FISH_START,1,0L);

            break;
        }
    case IDI_NORMAL_ADD_FISH_START:
        {
            DWORD dwPassTime=(DWORD)time(NULL)-m_dwSceneStartTime;
            if ((dwPassTime>240 && dwPassTime<270)
                ||(dwPassTime>480 && dwPassTime<510))
            {
				m_dwProbability = GAME_PROBABILITY*650/1000.0f;
				if (m_pGameServiceOption->lCellScore>=10) m_dwProbability = GAME_PROBABILITY*0.9*650/1000.0f;//1000
				if (m_pGameServiceOption->lCellScore>=100) m_dwProbability = GAME_PROBABILITY*0.75*650/1000.0f;//1w
				if (m_pGameServiceOption->lCellScore>=200) m_dwProbability = GAME_PROBABILITY*0.58*650/1000.0f;//2w
				if (m_pGameServiceOption->lCellScore>=500) m_dwProbability = GAME_PROBABILITY*0.52*650/1000.0f;//5w
				if (m_pGameServiceOption->lCellScore>=1000) m_dwProbability = GAME_PROBABILITY*0.45*650/1000.0f;//10w
                if (!SendSpecialFishObject())
				{
					Log("CTableFrameSink::OnTimerMessage IDI_NORMAL_ADD_FISH_START SendNormalFishObject()");
                    return false;
				}
            }
            else
            {
				m_dwProbability = GAME_PROBABILITY;
				if (m_pGameServiceOption->lCellScore>=10) m_dwProbability = GAME_PROBABILITY*0.9;
				if (m_pGameServiceOption->lCellScore>=100) m_dwProbability = GAME_PROBABILITY*0.75;
				if (m_pGameServiceOption->lCellScore>=200) m_dwProbability = GAME_PROBABILITY*0.58;
				if (m_pGameServiceOption->lCellScore>=500) m_dwProbability = GAME_PROBABILITY*0.52;
				if (m_pGameServiceOption->lCellScore>=1000) m_dwProbability = GAME_PROBABILITY*0.45;
                if (!SendNormalFishObject())
				{
					Log("CTableFrameSink::OnTimerMessage IDI_NORMAL_ADD_FISH_START1 SendNormalFishObject()");
                    return false;
				}
            }

            break;
        }
    case IDI_NORMAL_ADD_FISH_END:
        {
            DWORD dwPassTime=(DWORD)time(NULL)-m_dwSceneStartTime;
            if (dwPassTime>CHANGE_SCENE_TIME_COUNT)
            {
                m_dwSceneStartTime = (DWORD)time(NULL);
                m_pITableFrame->SetGameTimer(IDI_SCENE_END,TIME_SCENE_END,1,0L);
            }
            else
            {
                m_pITableFrame->SetGameTimer(IDI_NORMAL_ADD_FISH_START,TIME_NORMAL_ADD_FISH_START,1,0L);
            }

            break;
        }

    case IDI_SCENE_ADD_FISH_START:
        {
            SendSceneFishObject();

            break;
        }
    case IDI_SCENE_ADD_FISH_END:
        {
            m_pITableFrame->SetGameTimer(IDI_SCENE_ADD_FISH_START,TIME_SCENE_ADD_FISH_START,1,0L);

            break;
        }

    case IDI_CHECK_FISH_DESTORY_START:
        {
            CheckFishDestroy();
            m_pITableFrame->SetGameTimer(IDI_CHECK_FISH_DESTORY_END,TIME_CHECK_FISH_DESTORY_START,1,0L);

            break;
        }
    case IDI_CHECK_FISH_DESTORY_END:
        {
            m_pITableFrame->SetGameTimer(IDI_CHECK_FISH_DESTORY_START,TIME_CHECK_FISH_DESTORY_END,1,0L);

            break;
        }
    }

    return true;
}

//游戏消息处理
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
    switch (wSubCmdID)
    {
    case SUB_C_BUY_BULLET:
        {
            //效验数据
            ASSERT(wDataSize==sizeof(CMD_C_Buy_Bullet));
            if (wDataSize!=sizeof(CMD_C_Buy_Bullet)) 
			{
				Log("if (wDataSize!=sizeof(CMD_C_Buy_Bullet))  ");
				return false;
			}

            //用户效验
            tagServerUserData * pUserData=pIServerUserItem->GetUserData();
            if (pUserData->cbUserStatus!=US_SIT) return true;

            CMD_C_Buy_Bullet *pBuyBullet=(CMD_C_Buy_Bullet *)pDataBuffer;
            return OnBuyBullet(pIServerUserItem, pBuyBullet);
        }
    case SUB_C_FIRE:
        {
            //效验数据
            ASSERT(wDataSize==sizeof(CMD_C_Fire));
            if (wDataSize!=sizeof(CMD_C_Fire))
			{
				Log("if (wDataSize!=sizeof(CMD_C_Fire))  ");
				return false;
			}

            //用户效验
            tagServerUserData * pUserData=pIServerUserItem->GetUserData();
            if (pUserData->cbUserStatus!=US_SIT) return true;

            CMD_C_Fire *pFire =  (CMD_C_Fire *)pDataBuffer;
            return OnFire(pIServerUserItem, pFire);
        }
    case SUB_C_CAST_NET:
        {
            CMD_C_Cast_Net *pCastNet =  (CMD_C_Cast_Net *)pDataBuffer;

            //效验数据
            ASSERT(wDataSize==sizeof(CMD_C_Cast_Net)-(MAX_FISH_IN_NET-pCastNet->cbCount)*sizeof(Fish_Net_Object));
            if (wDataSize!=sizeof(CMD_C_Cast_Net)-(MAX_FISH_IN_NET-pCastNet->cbCount)*sizeof(Fish_Net_Object))
			{
				Log("case SUB_C_CAST_NET");
				return false;
			}

            //用户效验
            tagServerUserData * pUserData=pIServerUserItem->GetUserData();
            if (pUserData->cbUserStatus!=US_SIT) return true;


            return OnCastNet(pIServerUserItem, pCastNet);
        }

    case SUB_C_CHANGE_CANNON:
        {                      
            //效验数据
            ASSERT(wDataSize==sizeof(CMD_C_Change_Cannon));
            if (wDataSize!=sizeof(CMD_C_Change_Cannon)) return false;

            //用户效验
            tagServerUserData * pUserData=pIServerUserItem->GetUserData();
            if (pUserData->cbUserStatus!=US_SIT) return true;

            CMD_C_Change_Cannon *pChangeCanon =  (CMD_C_Change_Cannon *)pDataBuffer;
            return OnChangeCannon(pIServerUserItem, pChangeCanon);
        }
    case SUB_C_ACCOUNT:
        {
            //效验数据
            ASSERT(wDataSize==sizeof(CMD_C_Account));
            if (wDataSize!=sizeof(CMD_C_Account)) 
			{
				Log("SUB_C_ACCOUNT wDataSize!=sizeof(CMD_C_Account)");
				return false;
			}

            //用户效验
            tagServerUserData * pUserData=pIServerUserItem->GetUserData();
            if (pUserData->cbUserStatus!=US_SIT) return true;

            CMD_C_Account *pAccount =  (CMD_C_Account *)pDataBuffer;
            return OnAccount(pIServerUserItem, pAccount);
        }
    case SUB_C_LASER_BEAN:
        {
            CMD_C_Laser_Bean *pLaserBean =  (CMD_C_Laser_Bean *)pDataBuffer;

            //效验数据
            ASSERT(wDataSize==sizeof(CMD_C_Laser_Bean)-(MAX_FISH_IN_NET-pLaserBean->cbCount)*sizeof(Fish_Net_Object));
            if (wDataSize!=sizeof(CMD_C_Laser_Bean)-(MAX_FISH_IN_NET-pLaserBean->cbCount)*sizeof(Fish_Net_Object))
			{
				Log("SUB_C_LASER_BEAN =sizeof(wDataSize!=sizeof(CMD_C_Laser_Bean)");
				return false;
			}


            //用户效验
            tagServerUserData * pUserData=pIServerUserItem->GetUserData();
            if (pUserData->cbUserStatus!=US_SIT) return true;

            return OnLaserBean(pIServerUserItem, pLaserBean);
        }
    case SUB_C_BOMB:
        {
            CMD_C_Bomb *pBomb =  (CMD_C_Bomb *)pDataBuffer;

            //效验数据
            ASSERT(wDataSize==sizeof(CMD_C_Bomb)-(MAX_FISH_IN_NET-pBomb->cbCount)*sizeof(Fish_Net_Object));
            if (wDataSize!=sizeof(CMD_C_Bomb)-(MAX_FISH_IN_NET-pBomb->cbCount)*sizeof(Fish_Net_Object))
			{
				Log("SUB_C_BOMB (wDataSize!=sizeof(CMD_C_Bomb)");
				return false;
			}


            //用户效验
            tagServerUserData * pUserData=pIServerUserItem->GetUserData();
            if (pUserData->cbUserStatus!=US_SIT) return true;

            return OnBomb(pIServerUserItem, pBomb);
        }
    case SUB_C_BONUS:
        {
            CMD_C_Bonus *pBonus =  (CMD_C_Bonus *)pDataBuffer;

            //效验数据
            ASSERT(wDataSize==sizeof(CMD_C_Bonus));
            if (wDataSize!=sizeof(CMD_C_Bonus))
			{
				Log("(wDataSize!=sizeof(CMD_C_Bomb)");
				return false;
			}


            //用户效验
            tagServerUserData * pUserData=pIServerUserItem->GetUserData();
            if (pUserData->cbUserStatus!=US_SIT) return true;

            return OnBonus(pIServerUserItem, pBonus);
        }
    case SUB_C_COOK:
        {
            CMD_C_Cook *pCook =  (CMD_C_Cook *)pDataBuffer;

            //效验数据
            ASSERT(wDataSize==sizeof(CMD_C_Cook));
            if (wDataSize!=sizeof(CMD_C_Cook))
			{
				Log(" SUB_C_COOK  if (wDataSize!=sizeof(CMD_C_Cook))");
				return false;
			}


            //用户效验
            tagServerUserData * pUserData=pIServerUserItem->GetUserData();
            if (pUserData->cbUserStatus!=US_SIT) return true;

            return OnCook(pIServerUserItem, pCook);
        }
    case SUB_C_TASK_PREPARED:
        {
            CMD_C_Task_Prepared *TaskPrepared =  (CMD_C_Task_Prepared *)pDataBuffer;

            //效验数据
            ASSERT(wDataSize==sizeof(CMD_C_Task_Prepared));
            if (wDataSize!=sizeof(CMD_C_Task_Prepared)) 
			{
				Log("SUB_C_TASK_PREPARED  if (wDataSize!=sizeof(CMD_C_Task_Prepared)) ");
				return false;
			}


            //用户效验
            tagServerUserData * pUserData=pIServerUserItem->GetUserData();
            if (pUserData->cbUserStatus!=US_SIT) return true;

            return OnTaskPrepared(pIServerUserItem, TaskPrepared);
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
    if (bLookonUser==false )
    {
        m_RoleObjects[wChairID].wID = wChairID;
        m_RoleObjects[wChairID].wFishGold = 0;
        m_RoleObjects[wChairID].wCannonType = 0;
        m_RoleObjects[wChairID].wFireCount = m_nFireCount[wChairID];
        m_wCookFishCount[wChairID] = 0;
		//m_bTaskSended[wChairID] = false;

		if (m_wAndroidLogicChairID == INVALID_CHAIR && !pIServerUserItem->IsAndroidUser())
		{
			m_wAndroidLogicUserID = pIServerUserItem->GetUserID();
			m_wAndroidLogicChairID = pIServerUserItem->GetChairID();
		}

        if (m_dwSceneStartTime==0)
        {
            m_cbScene = 0;
            m_dwSceneStartTime=(DWORD)time(NULL);

             m_pITableFrame->SetGameTimer(IDI_SCENE_START,TIME_SCENE_START,1,0L);

            m_pITableFrame->SetGameTimer(IDI_CHECK_FISH_DESTORY_START,TIME_CHECK_FISH_DESTORY_START,1,0L);
        }
        //else
        //{
        //    SendPointPathNormalFishObjects();
        //}


    }

    return true;
}

//用户起来
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
    //记录成绩
    if (bLookonUser==false)
    {
        // 剩余子弹
        int nScore = m_RoleObjects[wChairID].wFishGold*m_pGameServiceOption->lCellScore;

        m_pITableFrame->WriteUserScore(pIServerUserItem, nScore , 0, enScoreKind_Draw);

        m_RoleObjects[wChairID].wID = INVALID_CHAIR;
        m_RoleObjects[wChairID].wFishGold = 0;
        m_RoleObjects[wChairID].wCannonType = 0;
        m_RoleObjects[wChairID].wFireCount = 0;
        m_wCookFishCount[wChairID] = 0;
		//m_bTaskSended[wChairID] = false;

		m_TaskObjects[wChairID].m_enType =  CTaskDate::TYPE_NULL;

		//if (m_wAndroidLogicChairID == pIServerUserItem->GetChairID() && !pIServerUserItem->IsAndroidUser())
		if (m_wAndroidLogicUserID == pIServerUserItem->GetUserID())
		{
			m_wAndroidLogicUserID = 0;
			m_wAndroidLogicChairID = INVALID_CHAIR;
		}
    }

    return true;
}

//用户断线
bool __cdecl CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) 
{
    int nScore = m_RoleObjects[wChairID].wFishGold*m_pGameServiceOption->lCellScore;
    m_pITableFrame->WriteUserScore(pIServerUserItem, nScore , 0, enScoreKind_Draw);

    m_RoleObjects[wChairID].wID = INVALID_CHAIR;
    m_RoleObjects[wChairID].wFishGold = 0;
    m_RoleObjects[wChairID].wCannonType = 0;
    m_RoleObjects[wChairID].wFireCount = 0;
    m_wCookFishCount[wChairID] = 0;
	//m_bTaskSended[wChairID] = false;
	m_TaskObjects[wChairID].m_enType =  CTaskDate::TYPE_NULL;

	if (m_wAndroidLogicUserID == pIServerUserItem->GetUserID())
	{
		m_wAndroidLogicUserID = 0;
		m_wAndroidLogicChairID = INVALID_CHAIR;
	}

    return true;
}

bool CTableFrameSink::OnBuyBullet(IServerUserItem * pIServerUserItem, CMD_C_Buy_Bullet *pBuyBullet)
{
    WORD wChairID = pIServerUserItem->GetChairID();
    int nBuyBulletPrice = pBuyBullet->wCount*m_pGameServiceOption->lCellScore;
    const tagUserScore *pUserScore = pIServerUserItem->GetUserScore();

    if (pUserScore->lScore > nBuyBulletPrice)
    {
        m_RoleObjects[wChairID].wFishGold += pBuyBullet->wCount;
        m_pITableFrame->WriteUserScore(wChairID,-nBuyBulletPrice,0,enScoreKind_Draw);

        CMD_S_Buy_Bullet_Success BuyBulletSuccess;

        BuyBulletSuccess.wChairID = wChairID;
        BuyBulletSuccess.wCount = pBuyBullet->wCount;
        return m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_BUY_BULLET_SUCCESS, &BuyBulletSuccess, sizeof(CMD_S_Buy_Bullet_Success));

    }
    else
    {
        CMD_S_Buy_Bullet_Failed BuyBulletFailed;
        BuyBulletFailed.wChairID = wChairID;
        /*BuyBulletFailed.szReason = "金币不足！"*/

        /// 机器人没有钱起立
        if (pIServerUserItem->IsAndroidUser())
            return m_pITableFrameManager->PerformStandUpAction(pIServerUserItem);

        return m_pITableFrame->SendTableData(wChairID, SUB_S_BUY_BULLET_FAILED, &BuyBulletFailed, sizeof(CMD_S_Buy_Bullet_Failed));
    }

    return true;
}

bool CTableFrameSink::OnFire(IServerUserItem * pIServerUserItem, CMD_C_Fire *pFire)
{
    WORD wChairID = pIServerUserItem->GetChairID();
    WORD wFishGold = m_RoleObjects[wChairID].wFishGold;

    if (wFishGold > 0) // 送子弹不足炮类型要求 送子弹
    {
		/// 桌子上没有人的时候机器人不发炮
		if (m_wAndroidLogicChairID == INVALID_CHAIR) return true;

        if (wFishGold < m_RoleObjects[wChairID].wCannonType+1)
        {
            m_RoleObjects[wChairID].wFishGold=0;
        }
        else
        {
            m_RoleObjects[wChairID].wFishGold =  wFishGold - (m_RoleObjects[wChairID].wCannonType+1);
        }

        CMD_S_Fire_Success FireSuccess;

        FireSuccess.wChairID = wChairID;
		FireSuccess.wAndroidLogicChairID = m_wAndroidLogicChairID;
        FireSuccess.fRote = pFire->fRote;

        int nLevel = 0;
        bool bTask = false;

        //m_nFireCount[wChairID]+=m_RoleObjects[wChairID].wCannonType+1;
        m_nFireCount[wChairID] += m_GameLogic.GetExp(m_RoleObjects[wChairID].wCannonType);
        m_RoleObjects[wChairID].wFireCount = m_nFireCount[wChairID];

		FireSuccess.nFireCount = m_nFireCount[wChairID];

        bool bCarryOn = m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_FIRE_SUCCESS, &FireSuccess, sizeof(CMD_S_Fire_Success));
        if (!bCarryOn)
        {
            return false;
        }

        if ( m_nFireCount[wChairID]>=1000 && m_nFireCount[wChairID]<2000 && m_bTaskSended[wChairID]==false)
        {
            nLevel = 0;
            bTask = true;
			m_bTaskSended[wChairID] = true;

			m_TaskObjects[wChairID].m_enType =  (CTaskDate::Type)m_GameLogic.GetTaskByGroup(0);
            m_TaskObjects[wChairID].m_enState = CTaskDate::STATE_PREPARE;
            RandNumer1(m_TaskObjects[wChairID].m_nStartWheel, 3);
			RandNumer1(m_TaskObjects[wChairID].m_nEndWheel, 3);

			int index = rand()%CountArray(g_KillFishTask);
			m_TaskObjects[wChairID].m_nDuration = g_KillFishTask[index].nDuration;
            m_TaskObjects[wChairID].m_nFishType = g_KillFishTask[index].nFishType;
            m_TaskObjects[wChairID].m_nFishCount = g_KillFishTask[index].nFishCount;
            m_TaskObjects[wChairID].m_nBonus = g_KillFishTask[index].m_nBonus;

            if (m_TaskObjects[wChairID].m_enType != CTaskDate::TYPE_NULL)
            {
                m_nFireCount[wChairID] = 0;
                m_RoleObjects[wChairID].wFireCount = m_nFireCount[wChairID];

				m_bTaskSended[wChairID] = false;

				m_TaskObjects[wChairID].m_nEndWheel[0] = (int)m_TaskObjects[wChairID].m_enType;
				m_TaskObjects[wChairID].m_nEndWheel[1] = (int)m_TaskObjects[wChairID].m_enType;
				m_TaskObjects[wChairID].m_nEndWheel[2] = (int)m_TaskObjects[wChairID].m_enType;
			}


        }
        else if ( m_nFireCount[wChairID]>=2000 && m_nFireCount[wChairID]<3000 && m_bTaskSended[wChairID])
        {
            nLevel = 1;
            bTask = true;

			m_bTaskSended[wChairID] = false;

            m_TaskObjects[wChairID].m_enType =  (CTaskDate::Type)m_GameLogic.GetTaskByGroup(1);
            m_TaskObjects[wChairID].m_enState = CTaskDate::STATE_PREPARE;
            RandNumer1(m_TaskObjects[wChairID].m_nStartWheel, 3);
			RandNumer1(m_TaskObjects[wChairID].m_nEndWheel, 3);

			int index = rand()%CountArray(g_KillFishTask);
			m_TaskObjects[wChairID].m_nDuration = g_KillFishTask[index].nDuration;
            m_TaskObjects[wChairID].m_nFishType = g_KillFishTask[index].nFishType;
            m_TaskObjects[wChairID].m_nFishCount = g_KillFishTask[index].nFishCount;
            m_TaskObjects[wChairID].m_nBonus = g_KillFishTask[index].m_nBonus;

            if (m_TaskObjects[wChairID].m_enType != CTaskDate::TYPE_NULL)
            {
                m_nFireCount[wChairID] = 0;
                m_RoleObjects[wChairID].wFireCount = m_nFireCount[wChairID];

				m_bTaskSended[wChairID] = false;

				m_TaskObjects[wChairID].m_nEndWheel[0] = (int)m_TaskObjects[wChairID].m_enType;
				m_TaskObjects[wChairID].m_nEndWheel[1] = (int)m_TaskObjects[wChairID].m_enType;
				m_TaskObjects[wChairID].m_nEndWheel[2] = (int)m_TaskObjects[wChairID].m_enType;
            }

        }
        else if ( m_nFireCount[wChairID]>=3000 && m_bTaskSended[wChairID]==false)
        {
            nLevel = 2;
            bTask = true;

			m_bTaskSended[wChairID] = true;

            m_TaskObjects[wChairID].m_enType =  (CTaskDate::Type)m_GameLogic.GetTaskByGroup(2);
            m_TaskObjects[wChairID].m_enState = CTaskDate::STATE_PREPARE;
            RandNumer1(m_TaskObjects[wChairID].m_nStartWheel, 3);
			RandNumer1(m_TaskObjects[wChairID].m_nEndWheel, 3);

			int index = rand()%CountArray(g_KillFishTask);
			m_TaskObjects[wChairID].m_nDuration = g_KillFishTask[index].nDuration;
            m_TaskObjects[wChairID].m_nFishType = g_KillFishTask[index].nFishType;
            m_TaskObjects[wChairID].m_nFishCount = g_KillFishTask[index].nFishCount;
            m_TaskObjects[wChairID].m_nBonus = g_KillFishTask[index].m_nBonus;

            if (m_TaskObjects[wChairID].m_enType != CTaskDate::TYPE_NULL)
            {
                m_nFireCount[wChairID] = 0;
                m_RoleObjects[wChairID].wFireCount = m_nFireCount[wChairID];

				m_bTaskSended[wChairID] = false;

				m_TaskObjects[wChairID].m_nEndWheel[0] = (int)m_TaskObjects[wChairID].m_enType;
				m_TaskObjects[wChairID].m_nEndWheel[1] = (int)m_TaskObjects[wChairID].m_enType;
				m_TaskObjects[wChairID].m_nEndWheel[2] = (int)m_TaskObjects[wChairID].m_enType;
            }


        }

        if (bTask)
        {
            CMD_S_Task Task;
            Task.wChairID = wChairID;
            Task.cbLevel = nLevel;
            Task.nTask =(int)m_TaskObjects[wChairID].m_enType;
            Task.nBonus = m_TaskObjects[wChairID].m_nBonus; 
            Task.nDuration = m_TaskObjects[wChairID].m_nDuration;
            Task.nStartWheel[0] = m_TaskObjects[wChairID].m_nStartWheel[0];
            Task.nStartWheel[1] = m_TaskObjects[wChairID].m_nStartWheel[1];
            Task.nStartWheel[2] = m_TaskObjects[wChairID].m_nStartWheel[2];
            Task.nEndWheel[0] = m_TaskObjects[wChairID].m_nEndWheel[0];
            Task.nEndWheel[1] = m_TaskObjects[wChairID].m_nEndWheel[1];
            Task.nEndWheel[2] = m_TaskObjects[wChairID].m_nEndWheel[2];
            Task.cbFishType = m_TaskObjects[wChairID].m_nFishType;
            Task.cbFishCount = m_TaskObjects[wChairID].m_nFishCount;

            bCarryOn = m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_TASK, &Task, sizeof(CMD_S_Task));

            return bCarryOn;
        }

        return true;
    }
    else
    {
        CMD_S_Fire_Failed FireFailed;

        FireFailed.wChairID = wChairID;
        /*FireFailed.szReason = "金币不足！";*/

        return m_pITableFrame->SendTableData(wChairID, SUB_S_FIRE_FAILED, &FireFailed, sizeof(CMD_S_Fire_Failed));
    }
}

bool CTableFrameSink::OnCastNet(IServerUserItem * pIServerUserItem, CMD_C_Cast_Net *pCastNet)
{
	if (!(pIServerUserItem->GetChairID() == pCastNet->wChairID || m_wAndroidLogicUserID == pIServerUserItem->GetUserID())) return true;

    WORD wFishID;
    WORD wRoundID;
    int nSucessCount = 0;
    //WORD wChairID = pIServerUserItem->GetChairID();
	WORD wChairID = pCastNet->wChairID;
    int nFishInNetCount = pCastNet->cbCount;

	if (m_wAndroidLogicChairID == INVALID_CHAIR && pIServerUserItem->GetUserStatus() != US_OFFLINE && !pIServerUserItem->IsAndroidUser())
	{
		m_wAndroidLogicUserID = pIServerUserItem->GetUserID();
		m_wAndroidLogicChairID = pIServerUserItem->GetChairID();
	}

    CMD_S_Cast_Net_Success CastNetSuccess;
    CastNetSuccess.wChairID = wChairID;

    for (int i=0; i<nFishInNetCount; i++)
    {
		if (nSucessCount >= MAX_FISH_IN_NET)
        	break;

        wFishID = pCastNet->FishNetObjects[i].wID;
        wRoundID = pCastNet->FishNetObjects[i].wRoundID;

        if (m_FishObjects[wFishID].wID!=wFishID || m_FishObjects[wFishID].wRoundID!=wRoundID )
        {
            continue;
        }
        else
        {
            int nDropValue = m_GameLogic.GetProbability(m_RoleObjects[wChairID].wCannonType, m_FishObjects[wFishID].wType);

            if ((rand()%1000) < nDropValue*m_dwProbability/1000.0)
            {
                if (m_TaskObjects[wChairID].m_enType == CTaskDate::TYPE_COOK && m_TaskObjects[wChairID].m_nFishType==m_FishObjects[wFishID].wType
                    && m_TaskObjects[wChairID].m_enState==CTaskDate::STATE_RUNNING)
                {
                    m_wCookFishCount[wChairID]++;
                }

                CastNetSuccess.FishNetObjects[nSucessCount].wID = wFishID;
                CastNetSuccess.FishNetObjects[nSucessCount].wRoundID = wRoundID;
				CastNetSuccess.FishNetObjects[nSucessCount].wType = m_FishObjects[wFishID].wType;

                nSucessCount++;

                m_FishObjects[wFishID].wID = INVALID_WORD;
                m_RoleObjects[wChairID].wFishGold += m_GameLogic.FishGoldByStyle(m_FishObjects[wFishID].wType);
            }
        }
    }

    CastNetSuccess.cbCount = nSucessCount;

    return m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CAST_NET_SUCCESS, &CastNetSuccess, sizeof(CMD_S_Cast_Net_Success)-(MAX_FISH_IN_NET-nSucessCount)*sizeof(Fish_Net_Object));
}

bool CTableFrameSink::OnBomb(IServerUserItem * pIServerUserItem, CMD_C_Bomb *pBomb)
{
    WORD wFishID;
    WORD wRoundID;
    int nSucessCount = 0;
    WORD wChairID = pIServerUserItem->GetChairID();
    int nFishInNetCount = pBomb->cbCount;

    if (m_TaskObjects[wChairID].m_enType != CTaskDate::TYPE_BOMB)
    {
        Log("TableFrameSink::OnBomb");
        return false;
    }

    CMD_S_Bomb_Success BombSuccess;
    BombSuccess.wChairID = wChairID;

    for (int i=0; i<nFishInNetCount; i++)
    {
		if (nSucessCount >= MAX_FISH_IN_NET)
        	break;

        wFishID = pBomb->FishNetObjects[i].wID;
        wRoundID = pBomb->FishNetObjects[i].wRoundID;

        if (m_FishObjects[wFishID].wID!=wFishID || m_FishObjects[wFishID].wRoundID!=wRoundID )
        {
            continue;
        }
        else
        {
			if (nSucessCount >= MAX_FISH_IN_NET)
				break;

            BombSuccess.FishNetObjects[nSucessCount].wID = wFishID;
            BombSuccess.FishNetObjects[nSucessCount].wRoundID = wRoundID;

            nSucessCount++;

            m_FishObjects[wFishID].wID = INVALID_WORD;
            m_RoleObjects[wChairID].wFishGold += m_GameLogic.FishGoldByStyle(m_FishObjects[wFishID].wType);

        }
    }

    BombSuccess.cbCount = nSucessCount;

    m_TaskObjects[wChairID].m_enType = CTaskDate::TYPE_NULL;

    return m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_BOMB_SUCCESS, &BombSuccess, sizeof(CMD_S_Bomb_Success)-(MAX_FISH_IN_NET-nSucessCount)*sizeof(Fish_Net_Object));
}

bool CTableFrameSink::OnBonus(IServerUserItem * pIServerUserItem, CMD_C_Bonus *pBonus)
{
    WORD wChairID = pIServerUserItem->GetChairID();

    if (m_TaskObjects[wChairID].m_enType != CTaskDate::TYPE_BONUS)
    {
        Log("CTableFrameSink::OnBonus");
        return false;
    }

    CMD_S_Bonus Bonus;
    Bonus.wChairID = wChairID;
    Bonus.nBonus = m_TaskObjects[wChairID].m_nBonus;

    m_RoleObjects[wChairID].wFishGold +=  m_TaskObjects[wChairID].m_nBonus;

    m_TaskObjects[wChairID].m_enType = CTaskDate::TYPE_NULL;

    return m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_BONUS, &Bonus, sizeof(CMD_S_Bonus));

}

bool CTableFrameSink::OnCook(IServerUserItem * pIServerUserItem, CMD_C_Cook *pCook)
{
    WORD wChairID = pIServerUserItem->GetChairID();

    if (m_TaskObjects[wChairID].m_enType != CTaskDate::TYPE_COOK)
    {
        Log ("CTableFrameSink::OnCook  if (m_TaskObjects[wChairID].m_enType != CTaskDate::TYPE_COOK)") ;
        return false;
    }

    CMD_S_Cook Cook;
    Cook.wChairID = wChairID;
    Cook.nBonus = m_TaskObjects[wChairID].m_nBonus;
   
    if (m_wCookFishCount[wChairID] >= m_TaskObjects[wChairID].m_nFishCount)
    {
         Cook.cbSucess = 1;
         m_RoleObjects[wChairID].wFishGold +=  m_TaskObjects[wChairID].m_nBonus;
    }
    else
    {
        Cook.cbSucess = 0;
    }

    m_wCookFishCount[wChairID] = 0;
    m_TaskObjects[wChairID].m_enType = CTaskDate::TYPE_NULL;

    return m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_COOK, &Cook, sizeof(CMD_S_Cook));

}

bool CTableFrameSink::OnTaskPrepared(IServerUserItem * pIServerUserItem, CMD_C_Task_Prepared *pTaskPrepared)
{
     WORD wChairID = pIServerUserItem->GetChairID();

     if (m_TaskObjects[wChairID].m_enType !=CTaskDate::TYPE_COOK)
	 {
		  Log ("CTableFrameSink::OnTaskPrepared") ;
         return false;
	 }

     m_TaskObjects[wChairID].m_enState = CTaskDate::STATE_RUNNING;

     return true;
}

bool CTableFrameSink::OnLaserBean(IServerUserItem * pIServerUserItem, CMD_C_Laser_Bean *pLaserBean)
{
    WORD wFishID;
    WORD wRoundID;
    int nSucessCount = 0;
    WORD wChairID = pIServerUserItem->GetChairID();
    int nFishInNetCount = pLaserBean->cbCount;

    if (m_TaskObjects[wChairID].m_enType != CTaskDate::TYPE_BEAN)
    {
        Log ("CTableFrameSink::OnLaserBean  ") ;
        return false;
    }

    CMD_S_Laser_Bean_Success LaserBeanSuccess;
    LaserBeanSuccess.wChairID = wChairID;
	LaserBeanSuccess.fRote = pLaserBean->fRote;

    for (int i=0; i<nFishInNetCount; i++)
    {
        if (nSucessCount >= MAX_FISH_IN_NET)
        	break;

        wFishID = pLaserBean->FishNetObjects[i].wID;
        wRoundID = pLaserBean->FishNetObjects[i].wRoundID;

        if (m_FishObjects[wFishID].wID!=wFishID || m_FishObjects[wFishID].wRoundID!=wRoundID )
        {
            continue;
        }
        else
        {
            LaserBeanSuccess.FishNetObjects[nSucessCount].wID = wFishID;
            LaserBeanSuccess.FishNetObjects[nSucessCount].wRoundID = wRoundID;

            nSucessCount++;

            m_FishObjects[wFishID].wID = INVALID_WORD;
            m_RoleObjects[wChairID].wFishGold += m_GameLogic.FishGoldByStyle(m_FishObjects[wFishID].wType);
        }
    }

    LaserBeanSuccess.cbCount = nSucessCount;

    m_TaskObjects[wChairID].m_enType = CTaskDate::TYPE_NULL;

    return m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_LASER_BEAN_SUCCESS, &LaserBeanSuccess, sizeof(CMD_S_Laser_Bean_Success)-(MAX_FISH_IN_NET-nSucessCount)*sizeof(Fish_Net_Object));
}

bool CTableFrameSink::OnChangeCannon(IServerUserItem * pIServerUserItem, CMD_C_Change_Cannon *pChangeCannon)
{
    ASSERT(pChangeCannon->wStyle>=0 && pChangeCannon->wStyle<MAX_CANNON_STYLE);
    if (pChangeCannon->wStyle<0 || pChangeCannon->wStyle>=MAX_CANNON_STYLE)
	{
		Log ("CTableFrameSink::OnChangeCannon") ;
		return false;
	}

    WORD wChairID = pIServerUserItem->GetChairID();
    m_RoleObjects[wChairID].wCannonType = pChangeCannon->wStyle;

    CMD_S_Change_Cannon ChangeCannon;

    ChangeCannon.wChairID = pIServerUserItem->GetChairID();
    ChangeCannon.wStyle = pChangeCannon->wStyle;

    return m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CHANGE_CANNON, &ChangeCannon, sizeof(CMD_S_Change_Cannon));

}

bool CTableFrameSink::OnAccount(IServerUserItem * pIServerUserItem, CMD_C_Account *pAccount)
{
    WORD wChairID = pIServerUserItem->GetChairID();
    m_pITableFrame->WriteUserScore(pIServerUserItem,m_RoleObjects[wChairID].wFishGold*m_pGameServiceOption->lCellScore,0,enScoreKind_Draw);
    m_RoleObjects[wChairID].wFishGold = 0;

    CMD_S_Account Account;
    Account.wChairID = wChairID;

    return m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_ACCOUNT, &Account, sizeof(CMD_S_Account));
}


bool CTableFrameSink::SendNormalFishObject(bool bInit) 
{
    SendPointPathNormalFishObjects();

    SendGroupPointPathNormalFishObjects();
    SendGroupPointPathNormalFishObjects();
    SendGroupPointPathNormalFishObjects();

	if (!bInit)
       m_pITableFrame->SetGameTimer(IDI_NORMAL_ADD_FISH_END,5000,1,0L);

	return true;
}

bool CTableFrameSink::SendGroupPointPathNormalFishObjects()
{
    CMD_S_Send_Group_Point_Path_Fish SendGroupPath;

    SendGroupPath.wPath = rand()%MAX_SMALL_POINT_PATH;
    SendGroupPath.cbPahtType = rand()%2+1; // 跟随
    SendGroupPath.dwTime = 16;
    SendGroupPath.cbCount = rand()%6+6;

    int nFreeFishID;
    int nFishType = rand()%4;
    DWORD wFishTime = (DWORD)time(NULL);

    for (int i=0; i<SendGroupPath.cbCount; i++)
    {
        nFreeFishID = GetFreeFishID();
        if (nFreeFishID>=MAX_FISH_OBJECT)
        {
            Log ("SendGroupPointPathNormalFishObjects nFreeFishID no id");
            return false;
        }

        SendGroupPath.FishNetObject[i].wID = nFreeFishID;
        SendGroupPath.FishNetObject[i].wRoundID = m_FishObjects[nFreeFishID].wRoundID+1;
        SendGroupPath.FishNetObject[i].wType = nFishType;
        SendGroupPath.FishNetObject[i].dwTime = wFishTime;

        m_FishObjects[nFreeFishID] = SendGroupPath.FishNetObject[i];
    }

    return m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_SEND_GROUP_POINT_PATH_FISH, &SendGroupPath, sizeof(SendGroupPath)-(MAX_FISH_SEND-SendGroupPath.cbCount)*sizeof(Fish_Net_Object));

}

bool CTableFrameSink::SendPointPathNormalFishObjects()
{
    CMD_S_Send_Point_Path_Fish SendPointPath;
    SendPointPath.cbCount = 0;

    int nFreeFishID;
    WORD wFishType ;
    WORD wFishPath;
    DWORD wFishTime = (DWORD)time(NULL);

    int nHugeFishPath[MAX_HUGE_POINT_PATH];
    RandNumer(nHugeFishPath, MAX_HUGE_POINT_PATH);
    int nBigFishPath[MAX_BIG_POINT_PATH];
    RandNumer(nBigFishPath, MAX_BIG_POINT_PATH);
    int nSmallFishPath[MAX_SMALL_POINT_PATH];
    RandNumer(nSmallFishPath, MAX_SMALL_POINT_PATH);

    for (int i=0; i<12; i++)
    {
        nFreeFishID = GetFreeFishID();
        wFishType = i;

        if (wFishType >= 7)
        {
            wFishPath = nHugeFishPath[i];
        }
        else if (wFishType > 3 && wFishType < 7)
        {

            wFishPath = nBigFishPath[i];
        }
        else
        {
            wFishPath = nSmallFishPath[i];
        }


        if (nFreeFishID>=MAX_FISH_OBJECT)
        {
            Log("SendPointPathNormalFishObjects nFreeFishID no id");
            return false;
        }

        SendPointPath.FishPaths[SendPointPath.cbCount].FishNetObject.wID = nFreeFishID;
        SendPointPath.FishPaths[SendPointPath.cbCount].FishNetObject.wRoundID = m_FishObjects[nFreeFishID].wRoundID+1;
        SendPointPath.FishPaths[SendPointPath.cbCount].FishNetObject.wType = wFishType;
        SendPointPath.FishPaths[SendPointPath.cbCount].FishNetObject.dwTime = wFishTime;
        SendPointPath.FishPaths[SendPointPath.cbCount].wPath = wFishPath;
        SendPointPath.FishPaths[SendPointPath.cbCount].dwTime = 16;

        m_FishObjects[nFreeFishID] = SendPointPath.FishPaths[SendPointPath.cbCount].FishNetObject;
        SendPointPath.cbCount++;
    }

    return m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_SEND_POINT_PATH_FISH, &SendPointPath, sizeof(SendPointPath)-(MAX_FISH_SEND-SendPointPath.cbCount)*sizeof(Fish_With_Point_Path));
}

bool CTableFrameSink::SendSpecialPointPathNormalFishObject()
{
    return true;
}

bool CTableFrameSink::SendSpecialFishObject()
{
    if (m_cbScene == 0)
    {
        SendSpecialFishObject0();
        m_pITableFrame->SetGameTimer(IDI_NORMAL_ADD_FISH_END,3000,1,0L);
    }
    else if (m_cbScene == 1)
    {
        SendSpecialFishObject1();
        m_pITableFrame->SetGameTimer(IDI_NORMAL_ADD_FISH_END,2000,1,0L);
    }
    else
    {
        SendSpecialFishObject2();
        m_pITableFrame->SetGameTimer(IDI_NORMAL_ADD_FISH_END,2000,1,0L);
    }


    return true;
}

bool CTableFrameSink::SendSpecialFishObject0()
{
    CMD_S_Send_Special_Point_Path SpecialPointPath;

    SpecialPointPath.cbCount = 0;

    int nFreeFishID;
    WORD wFishPath = 0;
    WORD wFishType = rand()%2+8;
    DWORD wFishTime = (DWORD)time(NULL);

    for (int i=0; i<4; i++)
    {
        wFishPath = i;

        for (int j=0; j<5; j++)
        {
            nFreeFishID = GetFreeFishID();
			if (nFreeFishID>=MAX_FISH_OBJECT)
            {
                Log("SendSpecialFishObject0 nFreeFishID no id");
                return false;
            }

            SpecialPointPath.SpecialPointPath[SpecialPointPath.cbCount].FishNetObject.wID = nFreeFishID;
            SpecialPointPath.SpecialPointPath[SpecialPointPath.cbCount].FishNetObject.wRoundID = m_FishObjects[nFreeFishID].wRoundID+1;
            SpecialPointPath.SpecialPointPath[SpecialPointPath.cbCount].FishNetObject.wType = wFishType;
            SpecialPointPath.SpecialPointPath[SpecialPointPath.cbCount].FishNetObject.dwTime = wFishTime;
            SpecialPointPath.SpecialPointPath[SpecialPointPath.cbCount].wPath = wFishPath;
            SpecialPointPath.SpecialPointPath[SpecialPointPath.cbCount].dwTime = 16;
            SpecialPointPath.SpecialPointPath[SpecialPointPath.cbCount].fDelay = 0.8f*j+0.1f;

            m_FishObjects[nFreeFishID] = SpecialPointPath.SpecialPointPath[SpecialPointPath.cbCount].FishNetObject;
            SpecialPointPath.cbCount++;
        }
    }

    return m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_SEND_SPECIAL_POINT_PATH, &SpecialPointPath, sizeof(SpecialPointPath)-(MAX_FISH_SEND-SpecialPointPath.cbCount)*sizeof(Fish_With_Special_Point_Path));
}

bool CTableFrameSink::SendSpecialFishObject1()
{
    CMD_S_Send_Special_Point_Path SpecialPointPath;

    SpecialPointPath.cbCount = 0;

    int nFreeFishID;
    WORD wFishPath = 0;
    WORD wFishType = 8;
    DWORD wFishTime = (DWORD)time(NULL);

    for (int i=0; i<12; i++)
    {
        wFishPath = 4+i;

        for (int j=0; j<3; j++)
        {
            nFreeFishID = GetFreeFishID();
            if (nFreeFishID>=MAX_FISH_OBJECT)
            {
                Log("SendSpecialFishObject1 nFreeFishID no id");
                return false;
            }
            SpecialPointPath.SpecialPointPath[SpecialPointPath.cbCount].FishNetObject.wID = nFreeFishID;
            SpecialPointPath.SpecialPointPath[SpecialPointPath.cbCount].FishNetObject.wRoundID = m_FishObjects[nFreeFishID].wRoundID+1;
            SpecialPointPath.SpecialPointPath[SpecialPointPath.cbCount].FishNetObject.wType = wFishType;
            SpecialPointPath.SpecialPointPath[SpecialPointPath.cbCount].FishNetObject.dwTime = wFishTime;
            SpecialPointPath.SpecialPointPath[SpecialPointPath.cbCount].wPath = wFishPath;
            SpecialPointPath.SpecialPointPath[SpecialPointPath.cbCount].dwTime = 16;
            SpecialPointPath.SpecialPointPath[SpecialPointPath.cbCount].fDelay = 0.8f*j+0.1f;

            m_FishObjects[nFreeFishID] = SpecialPointPath.SpecialPointPath[SpecialPointPath.cbCount].FishNetObject;
            SpecialPointPath.cbCount++;
        }
    }

    return m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_SEND_SPECIAL_POINT_PATH, &SpecialPointPath, sizeof(SpecialPointPath)-(MAX_FISH_SEND-SpecialPointPath.cbCount)*sizeof(Fish_With_Special_Point_Path));

}

bool CTableFrameSink::SendSpecialFishObject2()
{
    CMD_S_Send_Special_Point_Path SpecialPointPath;

    SpecialPointPath.cbCount = 0;

    int nFreeFishID;
    WORD wFishPath = 16;
    WORD wFishType = 0;
    DWORD wFishTime = (DWORD)time(NULL);


    for (int m=0; m<4; m++)
    {
        SpecialPointPath.cbCount = 0;

        for (int i=0; i<16; i++)
            for (int j=0; j<3; j++)
            {
                WORD wFishPath = 16+j+m*3;
                nFreeFishID = GetFreeFishID();
                if (nFreeFishID>=MAX_FISH_OBJECT)
                {
                    Log(" SendSpecialFishObject2 nFreeFishID no id");
                    return false;
                }

                SpecialPointPath.SpecialPointPath[SpecialPointPath.cbCount].FishNetObject.wID = nFreeFishID;
                SpecialPointPath.SpecialPointPath[SpecialPointPath.cbCount].FishNetObject.wRoundID = m_FishObjects[nFreeFishID].wRoundID+1;
                SpecialPointPath.SpecialPointPath[SpecialPointPath.cbCount].FishNetObject.wType = wFishType;
                SpecialPointPath.SpecialPointPath[SpecialPointPath.cbCount].FishNetObject.dwTime = wFishTime;
                SpecialPointPath.SpecialPointPath[SpecialPointPath.cbCount].wPath = wFishPath;
                SpecialPointPath.SpecialPointPath[SpecialPointPath.cbCount].dwTime = 16;
                SpecialPointPath.SpecialPointPath[SpecialPointPath.cbCount].fDelay = 0.2f*i+0.1f;

                m_FishObjects[nFreeFishID] = SpecialPointPath.SpecialPointPath[SpecialPointPath.cbCount].FishNetObject;
                SpecialPointPath.cbCount++;
            }


            m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_SEND_SPECIAL_POINT_PATH, &SpecialPointPath, sizeof(SpecialPointPath)-(MAX_FISH_SEND-SpecialPointPath.cbCount)*sizeof(Fish_With_Special_Point_Path));
    }

    return true;
}

bool CTableFrameSink::SendSceneFishObject()
{
    if (m_wSceneSendFishCount<2)
    {
        SendSceneLeftFishObject();
        m_wSceneSendFishCount++;
        m_pITableFrame->SetGameTimer(IDI_SCENE_ADD_FISH_END,4000,1,0L);
    }
    else if(m_wSceneSendFishCount==2)
    {
        SendSceneLeftFishObject();
        m_wSceneSendFishCount++;
        m_pITableFrame->SetGameTimer(IDI_SCENE_ADD_FISH_END,16000,1,0L);
    }
    else if (m_wSceneSendFishCount>2 && m_wSceneSendFishCount < 6)
    {
        SendSceneRightFishObject();
        m_wSceneSendFishCount++;
        m_pITableFrame->SetGameTimer(IDI_SCENE_ADD_FISH_END,4000,1,0L);
    }
    else
    {
        m_wSceneSendFishCount = 0;
        m_pITableFrame->SetGameTimer(IDI_SCENE_START,16000,1,0L);
    }


    return true;
}

bool CTableFrameSink::SendSceneLeftFishObject()
{
    int nFishPathsCount = m_LeftSceneFishPaths.size();
    int nFishPathsIndex = rand()%MAX_LEFT_LINE_SPATH;
    int nFishCount = m_LeftSceneFishPaths[nFishPathsIndex].cbCount;

    int nFreeFishID;
    DWORD wFishTime = (DWORD)time(NULL);
    CMD_S_Send_Line_Path_Fish SendLinePathFish;
    SendLinePathFish.cbCount = 0;


    for (int i=0; i<nFishCount; i++)
    {
        nFreeFishID = GetFreeFishID();
        if (nFreeFishID>=MAX_FISH_OBJECT)
        {
            Log(" SendSceneLeftFishObject nFreeFishID no id");
            return false;
        }

        SendLinePathFish.FishPaths[i].FishNetObject.wID = nFreeFishID;
        SendLinePathFish.FishPaths[i].FishNetObject.wRoundID = m_FishObjects[nFreeFishID].wRoundID+1;
        SendLinePathFish.FishPaths[i].FishNetObject.wType = m_LeftSceneFishPaths[nFishPathsIndex].FishPaths[i].FishNetObject.wType;
        SendLinePathFish.FishPaths[i].FishNetObject.dwTime = wFishTime;
        SendLinePathFish.FishPaths[i].LinePath.spStart.x = m_LeftSceneFishPaths[nFishPathsIndex].FishPaths[i].LinePath.spStart.x+1280+256;
        SendLinePathFish.FishPaths[i].LinePath.spStart.y = m_LeftSceneFishPaths[nFishPathsIndex].FishPaths[i].LinePath.spStart.y;
        SendLinePathFish.FishPaths[i].LinePath.spEnd.x = m_LeftSceneFishPaths[nFishPathsIndex].FishPaths[i].LinePath.spStart.x-700-256;
        SendLinePathFish.FishPaths[i].LinePath.spEnd.y = m_LeftSceneFishPaths[nFishPathsIndex].FishPaths[i].LinePath.spStart.y;
        SendLinePathFish.FishPaths[i].LinePath.dwTime = 16;

        m_FishObjects[nFreeFishID] = SendLinePathFish.FishPaths[i].FishNetObject;
        SendLinePathFish.cbCount++;
    }

    return m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_SEND_LINE_PATH_FISH, &SendLinePathFish, sizeof(SendLinePathFish)-(MAX_FISH_SEND-SendLinePathFish.cbCount)*sizeof(Fish_With_Line_Path));
}

bool CTableFrameSink::SendSceneRightFishObject()
{
    int nFishPathsCount = m_LeftSceneFishPaths.size();
    int nFishPathsIndex = rand()%MAX_RIGHT_LINE_SPATH;
    int nFishCount = m_RightSceneFishPaths[nFishPathsIndex].cbCount;

    int nFreeFishID;
    DWORD wFishTime = (DWORD)time(NULL);
    CMD_S_Send_Line_Path_Fish SendLinePathFish;
    SendLinePathFish.cbCount = 0;


    for (int i=0; i<nFishCount; i++)
    {
        nFreeFishID = GetFreeFishID();
        if (nFreeFishID>=MAX_FISH_OBJECT)
        {
            Log(" SendSceneRightFishObject nFreeFishID no id");
            return false;
        }

        SendLinePathFish.FishPaths[i].FishNetObject.wID = nFreeFishID;
        SendLinePathFish.FishPaths[i].FishNetObject.wRoundID = m_FishObjects[nFreeFishID].wRoundID+1;
        SendLinePathFish.FishPaths[i].FishNetObject.wType = m_RightSceneFishPaths[nFishPathsIndex].FishPaths[i].FishNetObject.wType;
        SendLinePathFish.FishPaths[i].FishNetObject.dwTime = wFishTime;
        SendLinePathFish.FishPaths[i].LinePath.spStart.x = m_RightSceneFishPaths[nFishPathsIndex].FishPaths[i].LinePath.spStart.x-700-256;
        SendLinePathFish.FishPaths[i].LinePath.spStart.y = m_RightSceneFishPaths[nFishPathsIndex].FishPaths[i].LinePath.spStart.y;
        SendLinePathFish.FishPaths[i].LinePath.spEnd.x = m_RightSceneFishPaths[nFishPathsIndex].FishPaths[i].LinePath.spStart.x+1280+256;
        SendLinePathFish.FishPaths[i].LinePath.spEnd.y = m_RightSceneFishPaths[nFishPathsIndex].FishPaths[i].LinePath.spStart.y;
        SendLinePathFish.FishPaths[i].LinePath.dwTime = 16;

        m_FishObjects[nFreeFishID] = SendLinePathFish.FishPaths[i].FishNetObject;
        SendLinePathFish.cbCount++;
    }

    return m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_SEND_LINE_PATH_FISH, &SendLinePathFish, sizeof(SendLinePathFish)-(MAX_FISH_SEND-SendLinePathFish.cbCount)*sizeof(Fish_With_Line_Path));
}

bool CTableFrameSink::LoadLinePaths()
{
    std::ostringstream ostr;
    CMD_S_Send_Line_Path_Fish SendLinePath;

    for (int i=0; i<MAX_LEFT_LINE_SPATH; i++)
    {
        ostr.str("");
        ostr << "FishServer\\left\\" << i <<".spth";

        std::ifstream ifs(ostr.str().c_str(), std::ios::binary);
        if (!ifs.is_open())
        {
            return false;
        }

        ifs.read((char*)&SendLinePath, sizeof(CMD_S_Send_Line_Path_Fish));
        m_LeftSceneFishPaths.push_back(SendLinePath);
    }

    for (int i=0; i<MAX_RIGHT_LINE_SPATH; i++)
    {
        ostr.str("");
        ostr << "FishServer\\right\\" << i <<".spth";

        std::ifstream ifs(ostr.str().c_str(), std::ios::binary);
        if (!ifs.is_open())
        {
            MessageBox(0,"CTableFrameSink::LoadLinePaths","",0);
            return false;
        }

        ifs.read((char*)&SendLinePath, sizeof(CMD_S_Send_Line_Path_Fish));
        m_RightSceneFishPaths.push_back(SendLinePath);
    }

    return true;
}

int CTableFrameSink::GetFreeFishID()
{
    for (int i=0; i<MAX_FISH_OBJECT; i++)
    {
        if (m_FishObjects[i].wID == INVALID_WORD)
        {
            return i;
        }
    }

    return MAX_FISH_OBJECT;
}

void CTableFrameSink::CheckFishDestroy()
{
    DWORD dwTime = (DWORD)time(NULL);

    for (int i=0; i<MAX_FISH_OBJECT; i++)
    {
        if (m_FishObjects[i].wID == i)
        {
            if ((dwTime-m_FishObjects[i].dwTime) >= FISH_DESTROY_TIME)
            {
                m_FishObjects[i].wID = INVALID_WORD;
            }
        }
    }
}

void CTableFrameSink::RandNumer(int nPaths[], int nCount)
{
    for(int i=0; i<nCount; ++i) nPaths[i]=i;
    for(int i=nCount-1; i>=1; --i) std::swap(nPaths[i], nPaths[rand()%i]);
}

void CTableFrameSink::RandNumer1(int nPaths[], int nCount)
{
	for(int i=0;i<nCount;i++)
	{
		nPaths[i]=rand()%4;
		for(int j=0;j<i;j++)
			while(nPaths[j]==nPaths[i])
			{
				nPaths[i]=rand()%4;
				j=0;
			}
	}
}



//////////////////////////////////////////////////////////////////////////

