#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"


//////////////////////////////////////////////////////////////////////////
//ö�ٶ���

//Ч������
enum enEstimatKind
{
	EstimatKind_OutCard,			//����Ч��
	EstimatKind_GangCard,			//����Ч��
};



//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink
{
	//��Ϸ����
protected:
	WORD							m_wDoorDiceCount;						//�������ӵ���
	WORD							m_wGoldDiceCount;						//�������ӵ���
	WORD							m_wBankerUser;							//ׯ���û�
	BYTE							m_cbCardIndex[GAME_PLAYER][MAX_INDEX];	//�û��齫
	//BYTE							m_cbDisPengIndex[GAME_PLAYER][MAX_INDEX];//�����еĳ���
	//BYTE							m_cbDisHuIndex[GAME_PLAYER][MAX_INDEX];	//�����еĳ���
	enGoldSwim						m_GoldSwim;								//����״̬
	bool							m_bGameStart;							//��ʼ��־
	enGoldSwim						m_UserGoldSwim[GAME_PLAYER];			//����״̬
	BYTE							m_cbCurCardIndex[GAME_PLAYER][MAX_INDEX];//����˿� 



	//������Ϣ
protected:
	WORD							m_wOutCardUser;							//�����û�
	BYTE							m_cbOutCardData;						//�����齫
	BYTE							m_cbOutCardCount;						//������Ŀ
	BYTE							m_cbDiscardCount[GAME_PLAYER];			//������Ŀ
	BYTE							m_cbDiscardCard[GAME_PLAYER][55];		//������¼
	BYTE							m_cbMaxLeftCount;						//��������

	

	//������Ϣ
protected:
	BYTE							m_cbSendCardData;						//�����齫
	BYTE							m_cbSendCardCount;						//������Ŀ
	BYTE							m_cbLeftCardCount;						//ʣ���齫
	BYTE							m_cbRepertoryCard[MAX_REPERTORY];		//����齫
	BYTE							m_cbGoldCard[4];						//�����齫
	BYTE							m_cbRepairFlowerPos;					//����λ��
	BYTE							m_cbFlowerCard[8];						//�����齫
	BYTE							m_cbFlowerCount[GAME_PLAYER];			//�������
	BYTE							m_cbGangCount[GAME_PLAYER];				//���Ʒ���
	BYTE							m_cbGoldCount[GAME_PLAYER];				//���Ƹ���
	BYTE							m_cbDoorCard;							//�����齫


	//���б���
protected:
	WORD							m_wResumeUser;							//��ԭ�û�
	WORD							m_wCurrentUser;							//��ǰ�û�
	WORD							m_wProvideUser;							//��Ӧ�û�
	BYTE							m_cbProvideCard;						//��Ӧ�齫
	bool							m_bHuUser[4];							//�����û�

	

	//״̬����
protected:
	bool							m_bSendStatus;							//����״̬
	bool							m_bGangStatus;							//���Ϻ�״̬
	bool							m_bEnjoinChiHu[GAME_PLAYER];			//��ֹ�Ժ�
	bool							m_bEnjoinChiPeng[GAME_PLAYER];			//��ֹ����
	BYTE							m_bGoOnBankerTime;						//��ׯ����

	
	
	

	//�û�״̬
public:
	bool							m_bResponse[GAME_PLAYER];				//��Ӧ��־
	BYTE							m_cbUserAction[GAME_PLAYER];			//�û�����
	BYTE							m_cbOperateCard[GAME_PLAYER];			//�����齫
	BYTE							m_cbPerformAction[GAME_PLAYER];			//ִ�ж���
	LONG							m_lUserCapital[GAME_PLAYER];			//����ʱ�
	

	//����齫
protected:
	BYTE							m_cbWeaveItemCount[GAME_PLAYER];		//�����Ŀ
	tagWeaveItem					m_WeaveItemArray[GAME_PLAYER][5];		//����齫

	//������Ϣ
protected:
	BYTE							m_cbChiHuCard;							//�Ժ��齫
	tagChiHuResult					m_ChiHuResult[GAME_PLAYER];				//�Ժ����

	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;					//���ò���

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;							//��Ϸ����
	static const enStartMode		m_GameStartMode;						//��ʼģʽ
	//test
	char							*msg;									//������Ϣ


	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { if (IsValid()) delete this; return ; }
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
	//�û�ͬ��
	virtual bool __cdecl OnEventUserReady(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool __cdecl OnEventUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool __cdecl OnEventUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) {return true;}
	//�û�����
	virtual bool __cdecl OnEventUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool __cdecl OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);

	//��Ϸ�¼�
protected:
	//�û�����
	bool OnUserOutCard(WORD wChairID, BYTE cbCardData);
	//�û�����
	bool OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard);

	//��������
protected:
	//���Ͳ���
	bool SendOperateNotify();
	//�ɷ��齫
	bool DispatchCardData(WORD wCurrentUser,bool bRepairFlower);
	//��Ӧ�ж�
	bool EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind);	
	//����״̬
	enGoldSwim GetCurrentGoldSwim();
	//����״̬
	enGoldSwim GetUserGoldSwim(WORD wChairID);
	
	
	
public:
	// ������Ϣ
	bool OnUserOpenDoor();
	// ������Ϣ
	bool OnUserOpenGold();	
	// ������Ϣ
	bool OnUserRepairFlower(const void *pBuffer);
};

//////////////////////////////////////////////////////////////////////////

#endif