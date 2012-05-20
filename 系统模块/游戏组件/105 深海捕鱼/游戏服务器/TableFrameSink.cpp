#include "StdAfx.h"
#include "TableFrameSink.h"

#include <fstream>

//////////////////////////////////////////////////////////////////////////

//静态变量
const int       m_MaxFish = 20;
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

    m_nScene = 0;                               
    for (int i=0; i<GAME_PLAYER; i++)
    {
        m_RoleObject[i].nID = INVALID_CHAIR;
        m_RoleObject[i].nBonus = 0;
        m_RoleObject[i].nCannonStyle = 0;
        m_RoleObject[i].nBulletStyle = 0;
        m_RoleObject[i].nBulletCount = 0;
    }

    m_dwFishTime = 0;
    m_nFishCount = 0;

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
            ////胜利类型
            //enScoreKind ScoreKind=(m_lUserWinScore[wChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;

            ////写入积分
            //if (m_lUserWinScore[wChairID]!=0L) m_pITableFrame->WriteUserScore(wChairID,m_lUserWinScore[wChairID], m_lUserRevenue[wChairID], ScoreKind);

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
            StatusFree.nScene = m_nScene;
            CopyMemory(StatusFree.Role, m_RoleObject, GAME_PLAYER*sizeof(RoleNetObject));

            return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));


            return true;
        }

    case GS_PLAYING:		//游戏状态
        {

            //发送场景
            //return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
        }
    }

    return false;
}

//定时器事件
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
    switch (wTimerID)
    {
    case IDI_ADD_FISH:
        {             
            CMD_S_ADD_FISH AddFish;

            FishNetObject Fish[10];
            for (int i=0; i<10; i++)
            {
                Fish[i].nID = (m_nFishCount)%1024;
                Fish[i].nStyle = rand()%11;
                Fish[i].nPath = Fish[i].nStyle*10+rand()%10;

                m_nFishCount++;
            }

            CopyMemory(AddFish.Fish, Fish, 10*sizeof(FishNetObject));

            m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_ADD_FISH, &AddFish, sizeof(CMD_S_ADD_FISH));


            DWORD dwPassTime=(DWORD)time(NULL)-m_dwFishTime;
            if (dwPassTime>30)
            {
                m_dwFishTime = (DWORD)time(NULL);
                m_pITableFrame->SetGameTimer(IDI_CHANGE_SCENE,1000,1,0L);
            }
            else
            {
                m_pITableFrame->SetGameTimer(IDI_ADD_FISH,TIME_ADD_FISH,1,0L);
            }

            break;
        }

        case IDI_CHANGE_SCENE:
            {
                if (m_nScene == 0)
                {
                    m_nScene = 1;
                }
                else
                {
                    m_nScene = 0;
                }

                CMD_S_CHANGE_SCENE Scene;
                Scene.nScene = m_nScene;

                
                m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CHANGE_SCENE, &Scene, sizeof(CMD_S_CHANGE_SCENE));

                m_pITableFrame->SetGameTimer(IDI_ADD_FISH,15*1000,1,0L);

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
            ASSERT(wDataSize==sizeof(CMD_C_BUY_BULLET));
            if (wDataSize!=sizeof(CMD_C_BUY_BULLET)) return false;

            //用户效验
            tagServerUserData * pUserData=pIServerUserItem->GetUserData();
            if (pUserData->cbUserStatus!=US_SIT) return true;

            CMD_C_BUY_BULLET * pBuyBullet=(CMD_C_BUY_BULLET *)pDataBuffer;
            return OnBuyBullet(pIServerUserItem, pBuyBullet->nStyle, pBuyBullet->nCount);
        }

    case SUB_C_FIRE:
        {
            //效验数据
            ASSERT(wDataSize==sizeof(CMD_C_FIRE));
            if (wDataSize!=sizeof(CMD_C_FIRE)) return false;

            //用户效验
            tagServerUserData * pUserData=pIServerUserItem->GetUserData();
            if (pUserData->cbUserStatus!=US_SIT) return true;

            CMD_C_FIRE *fire =  (CMD_C_FIRE *)pDataBuffer;
            return OnFire(pIServerUserItem, fire);
        }
    case SUB_C_CHANGE_CANNON:
        {                      
            //效验数据
            ASSERT(wDataSize==sizeof(CMD_C_CHANGE_CANNON));
            if (wDataSize!=sizeof(CMD_C_CHANGE_CANNON)) return false;

            //用户效验
            tagServerUserData * pUserData=pIServerUserItem->GetUserData();
            if (pUserData->cbUserStatus!=US_SIT) return true;

            CMD_C_CHANGE_CANNON *change =  (CMD_C_CHANGE_CANNON *)pDataBuffer;

            return OnChangeCannon(pIServerUserItem, change);
        }

    case SUB_C_CAPTURE:
        {
            //效验数据
            ASSERT(wDataSize==sizeof(CMD_C_CAPTURE));
            if (wDataSize!=sizeof(CMD_C_CAPTURE)) return false;

            //用户效验
            tagServerUserData * pUserData=pIServerUserItem->GetUserData();
            if (pUserData->cbUserStatus!=US_SIT) return true;

            CMD_C_CAPTURE *capture =  (CMD_C_CAPTURE *)pDataBuffer;
            return OnCapture(pIServerUserItem, capture);
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
    if (bLookonUser==false && m_dwFishTime==0)
    {
        m_RoleObject[wChairID].nID = wChairID;
        m_RoleObject[wChairID].nBonus = 0;
        m_RoleObject[wChairID].nCannonStyle = 0;
        m_RoleObject[wChairID].nBulletStyle = 0;
        m_RoleObject[wChairID].nBulletCount = 0;
        m_dwFishTime=(DWORD)time(NULL);

		    m_pITableFrame->SetGameTimer(IDI_ADD_FISH,TIME_ADD_FISH,1,0L);
    }

    return true;
}

//用户起来
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
    //记录成绩
    if (bLookonUser==false)
    {
        m_RoleObject[wChairID].nID = INVALID_CHAIR;
        m_RoleObject[wChairID].nBonus = 0;
        m_RoleObject[wChairID].nCannonStyle = 0;
        m_RoleObject[wChairID].nBulletStyle = 0;
        m_RoleObject[wChairID].nBulletCount = 0;
    }

    return true;
}

//用户断线
bool __cdecl CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) 
{
    return true;
}

bool CTableFrameSink::OnBuyBullet(IServerUserItem * pIServerUserItem, int nBulletStyle, int nBulletCount)
{
    const tagUserScore *score = pIServerUserItem->GetUserScore();
    if (score->lScore >= (nBulletStyle+1)*nBulletCount)
    {
       m_RoleObject[pIServerUserItem->GetChairID()].nBulletStyle = nBulletStyle;
       m_RoleObject[pIServerUserItem->GetChairID()].nBulletCount = nBulletCount;

       CMD_S_BUY_BULLET buy;
       buy.bSuccess = true;
       buy.nChairID = pIServerUserItem->GetChairID();
       buy.nStyle = nBulletStyle;
       buy.nCount = nBulletCount;
       m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_BUY_BULLET, &buy, sizeof(CMD_S_BUY_BULLET));

       m_pITableFrame->WriteUserScore(pIServerUserItem->GetChairID(),(nBulletStyle+1)*nBulletCount,0,enScoreKind_Lost);
    }
    else
    {
       CMD_S_BUY_BULLET buy;
       buy.bSuccess = false;
       buy.nChairID = pIServerUserItem->GetChairID();
       m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_BUY_BULLET, &buy, sizeof(CMD_S_BUY_BULLET));
    }

    return true;
}

bool CTableFrameSink::OnFire(IServerUserItem * pIServerUserItem, CMD_C_FIRE *fire)
{
    CMD_S_FIRE sfire;

    sfire.nChairID = pIServerUserItem->GetChairID();
    sfire.fRote= fire->fRote;

    return m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_FIRE, &sfire, sizeof(CMD_S_FIRE));
}

bool CTableFrameSink::OnChangeCannon(IServerUserItem * pIServerUserItem, CMD_C_CHANGE_CANNON *change)
{
    CMD_S_CHANGE_CANNON ChangeCannon;

    ChangeCannon.nChairID = pIServerUserItem->GetChairID();
    ChangeCannon.nStyle = change->nStyle;

    return m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CHANGE_CANNON, &ChangeCannon, sizeof(CMD_S_CHANGE_CANNON));

}

bool CTableFrameSink::OnCapture(IServerUserItem * pIServerUserItem, CMD_C_CAPTURE *capture)
{
    CMD_S_CAPTURE Capture;

    WORD wChairID = pIServerUserItem->GetChairID();
    float fPro = m_GameLogic.GetCaptureProbability(m_RoleObject[wChairID].nCannonStyle, capture->nFishStyle, m_RoleObject[wChairID].nBulletStyle);

    if (fPro >= 100)
    {
        Capture.bSuccess = true;
        Capture.nChairID = pIServerUserItem->GetChairID();
        Capture.nFishID = capture->nFishID;
        Capture.nBonus = 100;
    }
    else
    {
        Capture.bSuccess = false;
    }

    return m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CAPTURE, &Capture, sizeof(CMD_S_CAPTURE));
}

//////////////////////////////////////////////////////////////////////////

