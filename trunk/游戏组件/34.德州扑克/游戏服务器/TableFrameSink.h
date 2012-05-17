#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��ұ���
protected:
	WORD							m_wDUser;								//D���
	WORD							m_wCurrentUser;							//��ǰ���

	//���״̬
protected:
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬

	//��ע��Ϣ
protected:
	LONG							m_lCellScore;							//��Ԫ��ע
	LONG							m_lTurnLessScore;						//��С��ע
	LONG							m_lAddLessScore;						//����Сע
	LONG							m_lTurnMaxScore;						//�����ע
	LONG							m_lBalanceScore;						//ƽ����ע
	WORD							m_wOperaCount;							//��������
	BYTE							m_cbBalanceCount;						//ƽ�����
	LONG							m_lTableScore[GAME_PLAYER];				//������ע
	LONG							m_lTotalScore[GAME_PLAYER];				//�ۼ���ע
	LONG							m_lUserMaxScore[GAME_PLAYER];			//�����ע
	BYTE							m_cbShowHand[GAME_PLAYER];				//����û�

	//˰�ձ���
protected:
	//LONG							m_bUserTax[GAME_PLAYER];				//�û�˰��,�����ô������,�ɲ��ñ���
	//LONG							m_bLastTax[GAME_PLAYER];				//���˰��

	//�˿���Ϣ
protected:
	BYTE							m_cbSendCardCount;						//������Ŀ
	BYTE							m_cbCenterCardData[MAX_CENTERCOUNT];	//�����˿�
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//�����˿�

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

	//�����¼� 
public:
	//�û�����
	virtual bool __cdecl OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool __cdecl OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool __cdecl OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool __cdecl OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool __cdecl OnEventGameStart();
	//��Ϸ����
	virtual bool __cdecl OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//��Ϣ�¼�
protected:
	//�����¼�
	bool OnUserGiveUp(WORD wChairID);
	//��ע�¼�
	bool OnUserAddScore(WORD wChairID, LONG lScore, bool bGiveUp);

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool __cdecl OnTimerMessage(WORD wTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ����
	virtual bool __cdecl OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool __cdecl OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
};

//////////////////////////////////////////////////////////////////////////

#endif