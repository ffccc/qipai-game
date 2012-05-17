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
	WORD							m_wCurrentUser;							//��ǰ���
	BYTE                            m_wPassCount;							 //pass�û�������
	BYTE                            m_bResidualCardCount;                  //������
	bool                            m_bAnyFriendEnd;						//����Ҵ����˿�
	BYTE							m_bTempCardData[54];					//�������ƺͲ��Ƶ��˿�
	//�û�����
protected:
	CArray<WORD,WORD>               m_UserScoreCard[GAME_PLAYER];			 //��ҵ÷�
	CArray<WORD,WORD>               m_bOneTurnScoreCard;					 //�����ܳ�Ϊ��ʳ���û���
	CArray<WORD,WORD>               m_wEndUsers;							 //�����û���
	//Ӯ�ұ�־
	int                            m_bWinKind;								 //��Ӯ����� 
	//��ʷ����
protected:
	LONG                            m_lAllTurnScore[GAME_PLAYER];		//�ֵܾ÷�
	LONG                            m_lLastTurnScore[GAME_PLAYER];		//�Ͼֵ÷�

	//������Ϣ
protected:
	BYTE							m_bCount1[4];						//��һ����
	BYTE							m_bCount2[4];						//��������
	BYTE							m_bTurnCardCount;					//������Ŀ
	BYTE							m_bTurnCardData[10];				//�����б�


	//�˿���Ϣ
protected:
	BYTE							m_bCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							m_bHandCardData[GAME_PLAYER][10];	//�����˿�

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

	//��Ϸ�¼�
protected:
	//���÷�����ʱ��
	void SetGameTimer(WORD wTimerID,UINT nElapse);
	//ɾ����ʱ��
	void KillGameTimer(WORD wTimerID);
	//����
	void SendCard();
	//�û�����
	bool OnUserOutCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount);
	//�û�����
	bool OnUserPassCard(WORD wChairID);
protected:
	//��Ϸ�Ƿ����
	bool IsGameOver();
	//������������Լ��Ķ��ѽӷ�
	WORD OnGetMeFriend(WORD wChairID);
	//�ж��ĸ����û���ʤ
	int GetSpecialWinKind();
	//����
	void RecruitCard(WORD wChairID);

};

//////////////////////////////////////////////////////////////////////////

#endif