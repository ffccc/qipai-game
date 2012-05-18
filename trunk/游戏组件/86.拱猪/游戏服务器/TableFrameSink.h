#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "..\��Ϸ�ͻ���\GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink
{
	//��Ϸ����
protected:
	WORD							m_wCurrentUser;						//��ǰ���
	WORD	                        m_wShowCardUser[GAME_PLAYER];       //�������
	BYTE                            m_bShowCard[GAME_PLAYER][4];        //�������
	BYTE                            m_bShowCardUserCount;               //�����Ŀ
	BYTE                            m_bShowCardTime;                    //���ƴ���
	BYTE                            m_bTotalShowCardCount;              //������Ŀ
	BYTE							m_bScoreCard[GAME_PLAYER][16];      //�����˿�
	BYTE                            m_bScoreCardCount[GAME_PLAYER];     //�˿���Ŀ
	BYTE							m_bShowCardCount[GAME_PLAYER];		//������Ŀ

	//������Ϣ
protected:
	WORD							m_wTurnWiner;						//ʤ�����
	WORD                            m_wFirstOutUser;                    //���ȳ���
    WORD                            m_wGetGpadeUser;                    //�ú���Q
	BYTE							m_bTurnCardCount;					//��������
	BYTE							m_bTurnCardData[GAME_PLAYER];		//�������

	//�˿���Ϣ
protected:
	BYTE							m_bCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							m_bHandCardData[GAME_PLAYER][HAND_CARD_COUNT];	//�����˿�

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
	virtual void __cdecl Release() { }
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
	//��Ϸ�¼�
protected:
	//�û�����
	bool OnUserOutCard(WORD wChairID, BYTE bCardData, BYTE bCardCount);
	//�������
	bool OnUserShowCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount);
	
};

//////////////////////////////////////////////////////////////////////////

#endif