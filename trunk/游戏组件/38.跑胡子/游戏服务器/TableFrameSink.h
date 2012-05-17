#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink
{
	//��Ϸ����
protected:
	WORD							m_wBankerUser;							//ׯ���û�
	WORD							m_wCurrentUser;							//��ǰ�û�
	BYTE							m_bUserHuXiCount[GAME_PLAYER];			//��Ϣ��Ŀ
	BYTE							m_cbCardIndex[GAME_PLAYER][MAX_INDEX];	//�û��˿�

	//��־����
protected:
	bool							m_bOutCard;								//���Ʊ�־
	bool							m_bEstimate;							//�жϱ�־
	bool							m_bDispatch;							//�ɷ���־

	//���˱���
protected:
	bool							m_bTiCard;								//���Ʊ�־
	bool							m_bSendNotify;							//������ʾ

	//��������
protected:
	WORD							m_wResumeUser;							//��ԭ�û�
	WORD							m_wOutCardUser;							//�����û�
	BYTE							m_cbOutCardData;						//�����˿�

	//������Ϣ
protected:
	BYTE							m_bLeftCardCount;						//ʣ����Ŀ
	BYTE							m_cbRepertoryCard[80];					//����˿�

	//�û�״̬
public:
	bool							m_bResponse[GAME_PLAYER];				//��Ӧ��־
	BYTE							m_bUserAction[GAME_PLAYER];				//�û�����
	BYTE							m_bChiCardKind[GAME_PLAYER];			//��������
	BYTE							m_bPerformAction[GAME_PLAYER];			//ִ�ж���

	//����״̬
public:
	bool							m_bMingPao[GAME_PLAYER];				//���ܱ�־
	bool							m_bAbandonCard[GAME_PLAYER][MAX_INDEX];	//�����˿�

	//����˿�
protected:
	BYTE							m_bWeaveItemCount[GAME_PLAYER];			//�����Ŀ
	tagWeaveItem					m_WeaveItemArray[GAME_PLAYER][7];		//����˿�

	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;					//���ò���

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;							//��Ϸ����
	static const enStartMode		m_GameStartMode;						//��ʼģʽ

	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { if (IsValid()) delete this; return; }
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
	virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);

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
	virtual bool __cdecl OnEventUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool __cdecl OnEventUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool __cdecl OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);

	//��Ϸ�¼�
protected:
	//�û�����
	bool OnUserOutCard(WORD wChairID, BYTE cbCardData);
	//�û�����
	bool OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbChiKind);
	//��������
	bool OnUserContinueCard(WORD wChairID);

	//��������
protected:
	//�ɷ��˿�
	bool DispatchCardData(WORD wCurrentUser);
	//������ʾ
	bool SendOutCardNotify(WORD wCurrentUser, bool bOutCard);
	//��Ӧ�ж�
	bool EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, bool bDispatch);

	//���ܺ���
protected:
	//�����ж�
	bool IsAllowOutCard(WORD wChairID);
	//���º�Ϣ
	bool UpdateUserHuXiCount(WORD wChairID);
};

//////////////////////////////////////////////////////////////////////////

#endif