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
	BYTE							m_bOperaCount;							//��������
	WORD							m_wCurrentUser;							//��ǰ�û�
	WORD							m_wBankerUser;							//ׯ���û�
	WORD							m_wFlashUser[GAME_PLAYER];				//�����û�

	//�û�״̬
protected:
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	LONG							m_lCompareCount;						//����״̬
	bool							m_bGameEnd;								//����״̬
	CMD_S_GameEnd					m_StGameEnd;							//��������

	//�˿˱���
protected:
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//�����˿�
	CArrayTemplate<WORD>			m_wCompardUser[GAME_PLAYER];			//�˿�����

	//��ע��Ϣ
protected:
	LONG							m_lTableScore[GAME_PLAYER];				//��ע��Ŀ
	LONG							m_lUserMaxScore[GAME_PLAYER];			//�����ע
	LONG							m_lMaxCellScore;						//��Ԫ����
	LONG							m_lCellScore;							//��Ԫ��ע
	LONG							m_lCurrentTimes;						//��ǰ����
	bool							m_bMingZhu[GAME_PLAYER];				//������ע

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
	//�����¼�
	bool OnUserGiveUp(WORD wChairID);
	//�����¼�
	bool OnUserLookCard(WORD wChairID);
	//�����¼�
	bool OnUserCompareCard(WORD wFirstChairID,WORD wNextChairID);
	//�����¼�
	bool OnUserOpenCard(WORD wUserID);
	//��ע�¼�
	bool OnUserAddScore(WORD wChairID, LONG lScore, bool bGiveUp, bool bCompareUser);

};

//////////////////////////////////////////////////////////////////////////

#endif