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
	WORD							m_wBombTime;						//ը������
	WORD							m_wOutBombCount;					//ը����Ŀ
	WORD							m_wFirstUser;						//�׽��û�
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ���
	BYTE							m_bMingCard;						//�����˿�
	BYTE							m_bCurrentState;					//��ǰ״̬
	BYTE							m_bCallScoreTimes[GAME_PLAYER];		//�зִ���
	BYTE							m_bCallScoreTag[GAME_PLAYER];		//�з��ж�
	BYTE							m_bScoreTimes[GAME_PLAYER];			//��������
	BYTE							m_bCallBanker[GAME_PLAYER];			//��ׯ��Ϣ
	BYTE							m_bHaveSendCard;					//�����ж�
	BYTE							m_bBackCatchTag;					//��ץ��־
	BYTE							m_bLookCardTag;						//���Ʊ�־

	//������Ϣ
protected:
	WORD							m_wTurnWiner;						//ʤ�����
	BYTE							m_bTurnCardCount;					//������Ŀ
	BYTE							m_bTurnCardData[20];				//�����б�
	bool							m_bUserTrustee[GAME_PLAYER];		//����й�

	//�˿���Ϣ
protected:
	BYTE							m_bBackCard[3];						//��Ϸ����
	BYTE							m_bCardCount[3];					//�˿���Ŀ
	BYTE							m_bOutCardCount[3];					//���ƴ���
	BYTE							m_bHandCardData[3][20];				//�����˿�

	//�������
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;					//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;				//���ò���

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;						//��Ϸ����
	static const enStartMode		m_GameStartMode;					//��ʼģʽ

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
protected:
	//������Ϣ
	bool SendUserInfoData(WORD wLandUser,BYTE bLandScore,WORD wCurrentUser,BYTE bCurrentState,WORD wBankerUser);
	//�����˿�
	bool SendUserCard();
	//��ʼ����
	bool UserStartOutCard();

	//��Ϸ�¼�
protected:
	//�з��¼�
	bool OnUserLandScore(WORD wChairID, BYTE bLandScore);
	//�û�����
	bool OnUserOutCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount);
	//�û�����
	bool OnUserPassCard(WORD wChairID);
};

//////////////////////////////////////////////////////////////////////////

#endif
