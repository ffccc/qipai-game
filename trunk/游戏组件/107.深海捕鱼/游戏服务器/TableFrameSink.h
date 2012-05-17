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

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
protected:
    BYTE                                    m_cbScene;
    DWORD                                   m_dwSceneStartTime;					//����ʱ�����

	DWORD									m_dwProbability;					//���弸��
	DWORD									m_wAndroidLogicUserID;
	WORD									m_wAndroidLogicChairID;				//��������ײ�߼�����û�����ID

    WORD                                    m_wFishCount; // �����
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

    //�������
protected:
    CGameLogic						         m_GameLogic;						//��Ϸ�߼�
    ITableFrame						         * m_pITableFrame;				    //��ܽӿ�
    ITableFrameControl				         * m_pITableFrameControl;			//��ܽӿ�
    ITableFrameManager						 * m_pITableFrameManager;			//��ܹ���ӿ�
    const tagGameServiceOption		         * m_pGameServiceOption;			//���ò���

    //���Ա���
protected:
    static const WORD				         m_wPlayerCount;					//��Ϸ����
    static const enStartMode		         m_GameStartMode;					//��ʼģʽ

    //��������
public:
    //���캯��
    CTableFrameSink();
    //��������
    virtual ~CTableFrameSink();

    //�����ӿ�
public:
    //�ͷŶ���
    virtual VOID __cdecl Release() { }
	//virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }  //��
    //�Ƿ���Ч
    virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
    //�ӿڲ�ѯ
    virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

    //����ӿ�
public:
    //��ʼ��
    virtual bool __cdecl InitTableFrameSink(IUnknownEx * pIUnknownEx);
    //��λ����
    virtual void __cdecl RepositTableFrameSink();

    //��Ϣ�ӿ�
public:
    //��ʼģʽ
    virtual enStartMode __cdecl GetGameStartMode();
    //��Ϸ״̬
    virtual bool __cdecl IsUserPlaying(WORD wChairID);

    //��Ϸ�¼�
public:
    //��Ϸ��ʼ
    virtual bool __cdecl OnEventGameStart();
    //��Ϸ����
    virtual bool __cdecl OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
    //���ͳ���
    virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

    //�¼��ӿ�
public:
    //��ʱ���¼�
    virtual bool __cdecl OnTimerMessage(WORD wTimerID, WPARAM wBindParam);
    //��Ϸ��Ϣ����
    virtual bool __cdecl OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
    //�����Ϣ����
    virtual bool __cdecl OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

    //�����¼�
public:
    //����ͬ��
    virtual bool __cdecl OnEventUserReqReady(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
    //�������
    virtual bool __cdecl OnEventUserReqOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
    //��������
    virtual bool __cdecl OnEventUserReqReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
    //��������
    virtual bool __cdecl OnEventUserReqSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon) { return true; }
    //��������
    virtual bool __cdecl OnEventUserReqStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon) { return true; }

    //�����¼�
public:
    //�û�����
    virtual bool __cdecl OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) ;
    //�û�����
    virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem){ return true; }
    //�û�����
    virtual bool __cdecl OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
    //�û�����
    virtual bool __cdecl OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
    //�û�ͬ��
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