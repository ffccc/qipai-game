#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//�������
protected:
	bool							m_bGameRule;						//�����־
	tagGameRuleInfo					m_GameRuleInfo;						//��Ϸ����

	//ʱ��״̬
protected:
	DWORD							m_dwLastClock;						//�ϴ�ʱ��
	WORD							m_wUseClock[2];						//ʹ��ʱ��
	WORD							m_wLeftClock[2];					//ʣ��ʱ��
	WORD							m_wTimeOutCount[2];					//��ʱ����

	//��Ϸ����
protected:
	WORD							m_wBlackUser;						//�������
	WORD							m_wCurrentUser;						//��ǰ�û�
	WORD							m_cbBegStatus[2];					//����״̬
	BYTE							m_cbPeaceCount[2];					//��ʹ���
	BYTE							m_cbRegretCount[2];					//�������

	//�������
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	ITableFrame	*					m_pITableFrame;						//��ܽӿ�
	const tagGameServiceOption *	m_pGameServiceOption;				//���ò���

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
	virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStation, bool bSendSecret);

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

	//�û�ʱ��
public:
	//�û�����
	virtual bool __cdecl OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool __cdecl OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser) { return true; }
	//�û�����
	virtual bool __cdecl OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser) ;
	//�û�ͬ��
	virtual bool __cdecl OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize){ return true; }

	//��Ϸ�¼�
protected:
	//�����¼�
	bool OnUserPlaceChess(WORD wChairID, BYTE cbXPos, BYTE cbYPos);
	//�����¼�
	bool OnUserPassChess(WORD wChairID);
	//��ʱ�¼�
	bool OnUserTimeOut(WORD wChairID);
	//��������
	bool OnUserRegretReq(WORD wChairID);
	//����Ӧ��
	bool OnUserRegretAnswer(WORD wChairID, BYTE cbApprove);
	//�������
	bool OnUserPeaceReq(WORD wChairID);
	//���Ӧ��
	bool OnUserPeaceAnswer(WORD wChairID, BYTE cbApprove);
	//�����¼�
	bool OnUserGiveUp(WORD wChairID);
	//��Ŀ����
	bool OnUserChessCount(WORD wChairID);
	//��ĿӦ��
	bool OnUserUnAgreeChessCount(WORD wChairID);
};

//////////////////////////////////////////////////////////////////////////

#endif