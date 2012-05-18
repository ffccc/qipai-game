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
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ���
//////////////////////////////////////////////////////////////////////////////////
	WORD							m_wChaUser;							//�������
	bool							m_bgou;								//����
	bool							m_bdian;							//����

	//////////////////////////////////////////��ӵı���////////////////////////////////////////
	bool                            m_bLiOrJue[GAME_PLAYER];            //��¼�Ƿ��������߾��
	BYTE                            m_cbLiSuperCount;                   //��������������¼������ʱ������
	bool                            m_bLiRequest[GAME_PLAYER];          //������¼����ǲ����Ѿ���������������
	bool                            m_bCurPlayer[GAME_PLAYER];          //��ǰ��Ϸ��ң�������ͬ�����������߾������ôΪfalse��
	bool                            m_bNotLiOrJue[GAME_PLAYER];         //����ҵ��˲������򲻾����ť��ʱ�򣬷��������Ϣ
	BYTE                            m_cbNotJue;                         //��������������¼�����������
	WORD                            m_cbMiCi[GAME_PLAYER];              //�������飬������¼��1-4������λ��
	BYTE                            m_cbCiXu[GAME_PLAYER];              //�������飬������¼��0-3����ҵ�����
	BYTE                            m_cbMingCi;                         //����
	BYTE                            m_cbEndType;                        //��������
	WORD                            m_wLiGun;
	WORD                            m_wJueGun;
//////////////////////////////////////////////////////////////////////////////////
	//������Ϣ
protected:
	WORD							m_wTurnWiner;						//ʤ�����
	BYTE							m_bTurnCardCount;					//������Ŀ
	BYTE							m_bTurnCardData[15];				//�����б�
	bool							m_bUserTrustee[GAME_PLAYER];		//����й�

	//�˿���Ϣ
protected:
	BYTE							m_bCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							m_bOutCardCount[GAME_PLAYER];		//���ƴ���
	BYTE							m_bHandCardData[GAME_PLAYER][15];	//�����˿�

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
	virtual void __cdecl Release() {delete this; }
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
	//�з��¼�
	bool OnUserLandScore(WORD wChairID, BYTE bLandScore);
	//�û�����
	bool OnUserOutCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount);
	//�û�����
	bool OnUserChaCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount);
	//�û�����
	bool OnUserDianCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount);
	//�û�����
	bool OnUserPassCard(WORD wChairID);
	//��������
	bool OnUserPassChaCard(WORD wChairID);
	//��������
	bool OnUserPassDianCard(WORD wChairID);
	//�ж���Ϸ����
	BYTE IsGameOver();
	//��ֺ���
	void CountScore(LONG lScore[]);
};

//////////////////////////////////////////////////////////////////////////

#endif