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
	WORD							m_wRuleRoundID;						//�����ʶ
	tagGameRuleInfo					m_GameRuleInfo;						//��Ϸ����

	//�û�״̬
protected:
	WORD							m_wBankerUser;						//ׯ�����
	WORD							m_wCurrentUser;						//��ǰ�û�
	WORD							m_wLeaveDrawTime[GAME_PLAYER];		//ʣ���ʱ

	//����״̬
protected:
	BYTE							m_cbConcludeReason;					//����ԭ��
	BYTE							m_cbPeaceCount[GAME_PLAYER];		//��ʹ���
	BYTE							m_cbRegretCount[GAME_PLAYER];		//�������
	BYTE							m_cbRequestStatus[GAME_PLAYER];		//����״̬

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
	virtual bool __cdecl SendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool __cdecl OnTimerMessage(WORD wTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ
	virtual bool __cdecl OnGameMessage(WORD wSubCmdID,const VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ
	virtual bool __cdecl OnFrameMessage(WORD wSubCmdID,const VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//�����¼�
public:
	//�û�����
	virtual bool __cdecl OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool __cdecl OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool __cdecl OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool __cdecl OnActionUserReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);

	//��Ϸ��Ϣ
protected:
	//�ƶ�����
	bool OnUserMoveChess(WORD wChairID, BYTE cbXSourcePos, BYTE cbYSourcePos, BYTE cbXTargetPos, BYTE cbYTargetPos, WORD wUserTimeCount);
	//��������
	bool OnUserPeaceRequest(WORD wChairID);
	//�����Ӧ
	bool OnUserPeaceRespond(WORD wChairID, BYTE cbApprove);
	//��������
	bool OnUserRegretRequest(WORD wChairID);
	//�����Ӧ
	bool OnUserRegretRespond(WORD wChairID, BYTE cbApprove);
	//��������
	bool OnUserConcludeRequest(WORD wChairID, BYTE cbReason);

	//��������
protected:
	//���ּ���
	bool CalculateGameScore(WORD wWinnerUser, LONGLONG lGameScore[GAME_PLAYER]);
};

//////////////////////////////////////////////////////////////////////////

#endif
