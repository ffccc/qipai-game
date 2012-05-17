#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

#include <list>
#include <vector>
#include <sstream>
#define INVALID_WORD ((WORD)-1)

struct CTaskDate
{
    enum Type 
    {
        TYPE_NULL = -1,
        TYPE_BONUS = 0,
        TYPE_COOK,
        TYPE_BEAN,
        TYPE_BOMB,
        TYPE_COUNT
    };

    enum State
    {
        STATE_NULL = -1,
        STATE_PREPARE = 0,
        STATE_RUNNING ,
        STATE_COMPLETE,
        STATE_COUNT
    };

    Type m_enType;
    State m_enState; 

    int m_nBonus;
    int m_nDuration;

    int m_nStartWheel[3];
    int m_nEndWheel[3];

    int m_nFishType;
    int m_nFishCount;

    CTaskDate() :m_enType(TYPE_NULL), m_enState(STATE_NULL), 
        m_nDuration(0), m_nBonus(0), m_nFishType(0), m_nFishCount(0)
    { 
    }
};

//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
protected:
    BYTE                                    m_cbScene;
    DWORD                                   m_dwSceneStartTime;					//场景时间计数

	DWORD									m_dwProbability;					//总体几率
	DWORD									m_wAndroidLogicUserID;
	WORD									m_wAndroidLogicChairID;				//机器人碰撞逻辑检测用户椅子ID

    WORD                                    m_wFishCount; // 鱼记数
    WORD                                    m_wSceneSendFishCount;
    WORD                                    m_wCookFishCount[GAME_PLAYER];

    int                                     m_nFireCount[GAME_PLAYER];
    Role_Net_Object                         m_RoleObjects[GAME_PLAYER];
    Fish_Net_Object                         m_FishObjects[MAX_FISH_OBJECT];

    CTaskDate                               m_TaskObjects[GAME_PLAYER];

	bool                                    m_bTaskSended[GAME_PLAYER];

protected:
    std::vector<CMD_S_Send_Line_Path_Fish>  m_LeftSceneFishPaths;
    std::vector<CMD_S_Send_Line_Path_Fish>  m_RightSceneFishPaths;

    //组件变量
protected:
    CGameLogic						         m_GameLogic;						//游戏逻辑
    ITableFrame						         * m_pITableFrame;				    //框架接口
    ITableFrameControl				         * m_pITableFrameControl;			//框架接口
    ITableFrameManager						 * m_pITableFrameManager;			//框架管理接口
    const tagGameServiceOption		         * m_pGameServiceOption;			//配置参数

    //属性变量
protected:
    static const WORD				         m_wPlayerCount;					//游戏人数
    static const enStartMode		         m_GameStartMode;					//开始模式

    //函数定义
public:
    //构造函数
    CTableFrameSink();
    //析构函数
    virtual ~CTableFrameSink();

    //基础接口
public:
    //释放对象
    virtual VOID __cdecl Release() { }
	//virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }  //改
    //是否有效
    virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
    //接口查询
    virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

    //管理接口
public:
    //初始化
    virtual bool __cdecl InitTableFrameSink(IUnknownEx * pIUnknownEx);
    //复位桌子
    virtual void __cdecl RepositTableFrameSink();

    //信息接口
public:
    //开始模式
    virtual enStartMode __cdecl GetGameStartMode();
    //游戏状态
    virtual bool __cdecl IsUserPlaying(WORD wChairID);

    //游戏事件
public:
    //游戏开始
    virtual bool __cdecl OnEventGameStart();
    //游戏结束
    virtual bool __cdecl OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
    //发送场景
    virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

    //事件接口
public:
    //定时器事件
    virtual bool __cdecl OnTimerMessage(WORD wTimerID, WPARAM wBindParam);
    //游戏消息处理
    virtual bool __cdecl OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
    //框架消息处理
    virtual bool __cdecl OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

    //请求事件
public:
    //请求同意
    virtual bool __cdecl OnEventUserReqReady(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
    //请求断线
    virtual bool __cdecl OnEventUserReqOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
    //请求重入
    virtual bool __cdecl OnEventUserReqReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
    //请求坐下
    virtual bool __cdecl OnEventUserReqSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon) { return true; }
    //请求起来
    virtual bool __cdecl OnEventUserReqStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon) { return true; }

    //动作事件
public:
    //用户断线
    virtual bool __cdecl OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) ;
    //用户重入
    virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem){ return true; }
    //用户坐下
    virtual bool __cdecl OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
    //用户起来
    virtual bool __cdecl OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
    //用户同意
    virtual bool __cdecl OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize){ return true; }

protected:
    bool OnBuyBullet(IServerUserItem * pIServerUserItem, CMD_C_Buy_Bullet *pBuyBullet);

    bool OnFire(IServerUserItem * pIServerUserItem, CMD_C_Fire *pFire);

    bool OnCastNet(IServerUserItem * pIServerUserItem, CMD_C_Cast_Net *pCastNet);

    bool OnChangeCannon(IServerUserItem * pIServerUserItem, CMD_C_Change_Cannon *pChangeCannon);

    bool OnAccount(IServerUserItem * pIServerUserItem, CMD_C_Account *pAccount);

    bool OnLaserBean(IServerUserItem * pIServerUserItem, CMD_C_Laser_Bean *pLaserBean);

    bool OnBomb(IServerUserItem * pIServerUserItem, CMD_C_Bomb *pBomb);

    bool OnBonus(IServerUserItem * pIServerUserItem, CMD_C_Bonus *pBonus);

    bool OnCook(IServerUserItem * pIServerUserItem, CMD_C_Cook *pCook);

    bool OnTaskPrepared(IServerUserItem * pIServerUserItem, CMD_C_Task_Prepared *pTaskPrepared);

protected:
    bool SendNormalFishObject(bool bInit=false) ;
    bool SendPointPathNormalFishObjects();
    bool SendGroupPointPathNormalFishObjects();
    bool SendSpecialPointPathNormalFishObject();

    bool SendSpecialFishObject();
    bool SendSpecialFishObject0();
    bool SendSpecialFishObject1();
    bool SendSpecialFishObject2();

    bool SendSceneFishObject();
    bool SendSceneLeftFishObject();
    bool SendSceneRightFishObject();

    bool LoadLinePaths();

    int GetFreeFishID();
    WORD GetNextFishID();

    void CheckFishDestroy();

    void RandNumer(int nPaths[], int nCount);
	void RandNumer1(int nPaths[], int nCount);
};

//////////////////////////////////////////////////////////////////////////

#endif